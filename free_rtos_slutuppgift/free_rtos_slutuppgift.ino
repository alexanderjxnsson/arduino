/*  1. Que or queset, struct
    2. Mutex?
    3. Outputs*/

// Including libraries
#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include "semphr.h"

// Create two queues and queue set handle
QueueHandle_t iQueue1, iQueue2;
QueueSetHandle_t iQueueSet;

// Create semaphore handle
SemaphoreHandle_t xMutex;

// Tick to wait set to one second
TickType_t xTickToWait = pdMS_TO_TICKS(1000);

// Creating struct for car parts and status
typedef struct {
  uint8_t engine_status;    // 1 if it works, 0 if broken
  uint8_t speed;
  uint16_t RPM;
  uint8_t vent_status;      // 1 if it works, 0 if broken
  float fuel_status;
} sCar;

// Created car scenarios out of the struct
sCar car_one = {true, 90, 2500, false, 50.03};
sCar car_two = {true, 90, 2500, true, 9.3};

static const char* vent_string[] = {"x02: Error: Vent", "Ventilation is ok"};
static const char* fuel_string[] = {"0x3: low fuel", "0x4: good fuel"};

void mutex_print(const char* printout);

void setup() {
  Serial.begin(9600);

  iQueue1 = xQueueCreate(2, sizeof(char*));
  iQueue2 = xQueueCreate(2, sizeof(char*));
  iQueueSet = xQueueCreateSet(2+2+1);
  xQueueAddToSet(iQueue1, iQueueSet);
  xQueueAddToSet(iQueue2, iQueueSet);

  xTaskCreate(fuel_task, "Fuel task", 128, &car_one, 1, NULL);
  xTaskCreate(ventilation_task, "Ventilation task", 128, &car_one, 1, NULL);
  xTaskCreate(mceb, "motor controller embedded board", 128, &car_one, 1, NULL);
}

void fuel_task(void* input_struct){
  sCar * local_struct = (sCar *) input_struct;
  BaseType_t qStatus;

  while (1) {
    Serial.println("Checking fuel");
    if(local_struct->fuel_status >= 10){
      qStatus = xQueueSend(iQueue1, &(fuel_string[1]), portMAX_DELAY);
      if(qStatus == pdPASS){
        mutex_print("h$h$");
      }
      else{
        Serial.println("FAILED TO SEND FUEL");
      }
    }
    else if(local_struct->fuel_status < 10){
      qStatus = xQueueSend(iQueue1, &(fuel_string[0]), portMAX_DELAY);
      if(qStatus == pdPASS){
        mutex_print("U$U$");
      }
      else{
        Serial.println("FAILED TO SEND FUEL");
      }
    }
    vTaskDelay(xTickToWait);
  } // while
} // function

void ventilation_task(void* input_struct){
  sCar * local_struct = (sCar *) input_struct;
  BaseType_t qStatus;
  
  while (1) {
    Serial.println("Checking vent");
    if (local_struct->vent_status == true) {
      qStatus = xQueueSend(iQueue2, &(vent_string[0]), portMAX_DELAY);
      if(qStatus == pdPASS){
        mutex_print("N*N*");
      }
      else{
        Serial.println("FAILED TO SEND VENT");
      }
    }
    else if (local_struct->vent_status == false) {
      qStatus = xQueueSend(iQueue2, &(vent_string[1]), portMAX_DELAY);
      if(qStatus == pdPASS){
        mutex_print("Y*Y*");
      }
      else{
        Serial.println("FAILED TO SEND VENT");
      }
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
    Serial.println("Checking motor");
    switch (local_struct->engine_status) {
      case 0:
        Serial.println("x01:Error: M.||Gb.");
        break;
      case 1:
        Serial.println("Motor is OK");
        break;
      Serial.println(msg);
    } // if
    vTaskDelay(xTickToWait);
  } // while
} // function

void mutex_print(const char* printout){
  xSemaphoreTake(xMutex, xTickToWait);
  Serial.println(printout);
  xSemaphoreGive(xMutex);
}

void loop() {}
