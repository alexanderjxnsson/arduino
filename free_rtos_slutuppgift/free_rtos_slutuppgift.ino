/*  1.  Körde queueset med två köer, en för vent och en för fuel som mceb tar emot.
        Jag avnände mig av en struct för att representera delarna av motorn.
    2.  Använde mig av mutex som shared dashboard för outputs.
    3.  Byt ut car_one till car_two för att ändra scenario
*/

// Including libraries
#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include "semphr.h"

// Defining pins
#define RED_PIN     6
#define YELLOW_PIN  7
#define GREEN_PIN   8
#define BLUE_PIN    9

// Create two queues and queue set handle
QueueHandle_t iQueue1, iQueue2;
QueueSetHandle_t iQueueSet;

// Create semaphore handle
SemaphoreHandle_t xMutex;

// Tick to wait set to one second
TickType_t xTickToWait = pdMS_TO_TICKS(1000);

// Creating struct for car parts and status
typedef struct {
  bool engine_gearbox_status;    // true if it works, false if broken
  uint8_t speed;
  uint16_t RPM;
  bool vent_status;             // true if it works, false if broken
  float fuel_status;
} sCar;

// Created car scenarios out of the struct
//              M.G, Speed, RPM, Vent, Fuel
sCar car_one = {true, 90, 2500, false, 50.03};
sCar car_two = {true, 90, 2500, true, 9.3};

// Creating char strings messegaes to send from vent and fuel
static const char* vent_string[] = {"x02: Error: Vent", "Ventilation is ok"};
static const char* fuel_string[] = {"0x3: low fuel", "0x4: good fuel"};

void setup() {
  Serial.begin(9600);
  while (!Serial){}
  
  // Setting up pin modes
  pinMode(RED_PIN, OUTPUT);     // Red light for low gas
  pinMode(GREEN_PIN, OUTPUT);   // Green light for good gas
  pinMode(YELLOW_PIN, OUTPUT);  // Yellow light for bad vent
  pinMode(BLUE_PIN, OUTPUT);    // Blue light for good vent

  // Setting xSemaphoreCreateMutex to xMutex
  xMutex = xSemaphoreCreateMutex();

  // Creating queues and queueset
  iQueue1 = xQueueCreate(2, sizeof(char*));
  iQueue2 = xQueueCreate(2, sizeof(char*));
  iQueueSet = xQueueCreateSet(2+2+1);
  xQueueAddToSet(iQueue1, iQueueSet);
  xQueueAddToSet(iQueue2, iQueueSet);

  // Creating our tasks and use our struct as value
  xTaskCreate(mceb, "motor controller embedded board", 128, &car_two, 1, NULL);
  xTaskCreate(fuel_task, "Fuel task", 128, &car_two, 1, NULL);
  xTaskCreate(ventilation_task, "Ventilation task", 128, &car_two, 1, NULL);
}

void fuel_task(void* input_struct){
  sCar * local_struct = (sCar *) input_struct;
  BaseType_t qStatus;

  while (1) {
    mutex_print("Checking fuel: ");
    if(local_struct->fuel_status >= 10){
      qStatus = xQueueSend(iQueue1, &(fuel_string[1]), portMAX_DELAY);
      if(qStatus == pdPASS){
        mutex_print("H$H$");
        digitalWrite(GREEN_PIN, HIGH);
      }
      else{mutex_print("FAILED TO SEND FUEL");}
    }
    else if(local_struct->fuel_status < 10){
      qStatus = xQueueSend(iQueue1, &(fuel_string[0]), portMAX_DELAY);
      if(qStatus == pdPASS){
        mutex_print("U$U$");
        digitalWrite(RED_PIN, HIGH);
      }
      else{mutex_print("FAILED TO SEND FUEL");}
    }
    vTaskDelay(xTickToWait);
  } // while
} // function

void ventilation_task(void* input_struct){
  sCar * local_struct = (sCar *) input_struct;
  BaseType_t qStatus;
  
  while (1) {
    mutex_print("Checking vent: ");
    if (local_struct->vent_status == false) {
      qStatus = xQueueSend(iQueue2, &(vent_string[0]), portMAX_DELAY);
      if(qStatus == pdPASS){
        mutex_print("N*N*");
        digitalWrite(YELLOW_PIN, HIGH);
      }
      else{mutex_print("FAILED TO SEND VENT");}
    }
    else if (local_struct->vent_status == true) {
      qStatus = xQueueSend(iQueue2, &(vent_string[1]), portMAX_DELAY);
      if(qStatus == pdPASS){
        mutex_print("Y*Y*");
        digitalWrite(BLUE_PIN, HIGH);
      }
      else{mutex_print("FAILED TO SEND VENT");}
    }
    vTaskDelay(xTickToWait);
  } // while
} // function

void mceb(void* input_struct){
  sCar * local_struct = (sCar *) input_struct;
  char * msg;
  QueueHandle_t activeQueue;
  BaseType_t qStatus;
  
  while(1){
    activeQueue = (QueueHandle_t)xQueueSelectFromSet(iQueueSet, portMAX_DELAY);
    qStatus = xQueueReceive(activeQueue, &msg, portMAX_DELAY);
    mutex_print("Checking motor: ");
    if (local_struct->engine_gearbox_status == true) {
      mutex_print("M.G is OK");
    }
    else if (local_struct->engine_gearbox_status == false) {
      mutex_print("x01:Error: M.||Gb.");
    }
    mutex_print("External message: ");
    mutex_print(msg);
    mutex_print("Gas level: ");
    mutex_print_float(local_struct->fuel_status);
    mutex_print("RPM: ");
    mutex_print_int(local_struct->RPM);
    mutex_print("Speed: ");
    mutex_print_int(local_struct->speed);
    vTaskDelay(xTickToWait);
  } // while
} // function

void mutex_print(const char* printout){
  xSemaphoreTake(xMutex, xTickToWait);
  Serial.println(printout);
  xSemaphoreGive(xMutex);
}

void mutex_print_int(int printout){
  xSemaphoreTake(xMutex, xTickToWait);
  Serial.println(printout);
  xSemaphoreGive(xMutex);
}

void mutex_print_float(float printout){
  xSemaphoreTake(xMutex, xTickToWait);
  Serial.println(printout);
  xSemaphoreGive(xMutex);
}

void loop() {}