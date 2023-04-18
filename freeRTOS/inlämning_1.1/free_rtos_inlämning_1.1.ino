// Including libraries
#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include "timers.h"

// Defining LED pins
#define RED_PIN     6
#define YELLOW_PIN  7
#define GREEN_PIN   8

// Creating struct for the microwave with it's members
typedef struct {
  uint32_t prog_length_s;       // Length of the program
  uint8_t door;                 // Door open or closed
  uint8_t lamp;                 // Lamp on or off
  uint16_t effect_of_heater;    // The effect of
  uint16_t disc_spin;           // Variable for the disc which we'll count in degrees
} sMicrowave;

// Here we've initilaized the different programs of the microwave from the struct
// First we add program time in seconds, door, lamp, effect and then disc
sMicrowave defrost_meat = {300, 0, 0, 800, 0};
sMicrowave defrost_veg = {60, 0, 0, 400, 0};
sMicrowave general_prog = {30, 0, 0, 800, 0};

// Creating task handle for the different programs
TaskHandle_t door_Handle, lamp_Handle, effect_Handle, disc_Handle;



#define PRINT_DELAY (pdMS_TO_TICKS(1000)) // Delay set to 1000ms(1sec), easier to calculate disc spin degree.

bool bChoice = true;
uint8_t prog_choice;


void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);     // Red light represents door open
  pinMode(YELLOW_PIN, OUTPUT);  // Yellow light represents lamp on
  pinMode(GREEN_PIN, OUTPUT);   // Green light represents program done

  xTaskCreate(door_task, "Display door output", 128, 1, 1, &door_Handle);
  xTaskCreate(lamp_task, "Display lamp output", 128, NULL, 1, &lamp_Handle);
  xTaskCreate(effect_task, "Display effect output", 128, 30, 1, &effect_Handle);             // We insert time in seconds of program as parameter
  xTaskCreate(disc_task, "Display disc and time output", 128, 0, 1, &disc_Handle);
} // Setup

void door_task(void * param){
  if (param == 1) {
    Serial.println("DOOR IS OPEN");
    vTaskDelay(PRINT_DELAY);
    vTaskSuspend(effect_Handle);
    vTaskSuspend(disc_Handle);
    vTaskSuspend(disc_Handle);
    digitalWrite(RED_PIN, HIGH);
  }
  else{
    Serial.println("DOOR IS CLOSED");
    vTaskDelay(PRINT_DELAY);
    vTaskResume(effect_Handle);
    vTaskResume(disc_Handle);
    vTaskResume(disc_Handle);
  }

}

void lamp_task(void * param){
  Serial.println("Lamp is on");
  digitalWrite(YELLOW_PIN, OUTPUT);
  vTaskDelay(PRINT_DELAY);

}

void effect_task(void * param){
  Serial.println("PROGRAM IS RUNNING");
  
  while (param != 0) {
    param -= 1;
    if(param == 0){                       // When prog_length reach 0 we exit the while loop
      Serial.println("PROGRAM DONE!");
      vTaskDelay(PRINT_DELAY);

    } // If
    vTaskDelay(PRINT_DELAY);
  }
}

void disc_task(void * param){
  uint16_t disc_status = param;
  while (1) {
    // Disc spin calculation
    Serial.print("Disc is at: ");
    Serial.println(disc_status);
    disc_status += 30;                                              // += 30 for 30 degrees/s
    if (disc_status >= 360) {
      disc_status = 0;
    } // If
    vTaskDelay(PRINT_DELAY);
  }
}

void loop() {}
