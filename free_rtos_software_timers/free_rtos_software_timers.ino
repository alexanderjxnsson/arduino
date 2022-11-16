#include <Arduino_FreeRTOS.h>
#include "timers.h" // include timer file

// Define timer treshhold
#define ONE_SHOT (pdMS_TO_TICKS(500))
#define PERIODIC (pdMS_TO_TICKS(600))
// create timer handle
TimerHandle_t xTimer_1_Shot, xTimer_Periodic;
// start parameters
BaseType_t xTimer_1_Shot_Started, xTimer_Periodic_Started;


void setup() {
  Serial.begin(9600);
  // Create timers
  xTimer_1_Shot = xTimerCreate("one_shot", ONE_SHOT, pdFALSE, 0, run_one_time);
  xTimer_Periodic = xTimerCreate("periodic", PERIODIC, pdTRUE, 0, run_periodic);

  // Validate the creation and start timers
  if ((xTimer_1_Shot != NULL) && (xTimer_Periodic != NULL)) {
    xTimer_1_Shot_Started = xTimerStart(xTimer_1_Shot, 0);
    xTimer_Periodic_Started = xTimerStart(xTimer_Periodic, 0);
  }
  // Don't start until timers got started
  while ((xTimer_1_Shot_Started != pdPASS) && (xTimer_Periodic_Started != pdPASS)) {}

}

void run_one_time(TimerHandle_t xTimer){
  TickType_t current_tick = xTaskGetTickCount();
  Serial.print("Welcome to Software Timer @ ");
  Serial.println(current_tick);
}

void run_periodic(TimerHandle_t xTimer){
  TickType_t current_tick = xTaskGetTickCount();
  Serial.print("I'm periodic timer @ ");
  Serial.println(current_tick);
}

void loop() {}