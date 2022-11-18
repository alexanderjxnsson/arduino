#include <Arduino_FreeRTOS.h>
#include "timers.h" // include timer file
// Pins
#define RED_PIN     6
#define YELLOW_PIN  7
#define GREEN_PIN   8
// Define frewuency timer
#define RED_500 (pdMS_TO_TICKS(500))
#define GREEN_600 (pdMS_TO_TICKS(600))
// Profiler
typedef uint32_t TaskProfiler;
TaskProfiler RED_Profiler;
TaskProfiler GREEN_Profiler;

// create timer handle
TimerHandle_t xTimer_RED, xTimer_GREEN;
// Timers start parameters
BaseType_t xTimer_RED_Started, xTimer_GREEN_Started;


void setup() {
  Serial.begin(9600);
  // Create timers
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  // Create timers
  xTimer_RED = xTimerCreate("Red timer", redTimer, pdTRUE, 0, blink_LEDS);
  xTimer_GREEN = xTimerCreate("Green blink", greenTimer, pdTRUE, 0, blink_LEDS);
  // Validation
  if ((xTimer_RED != NULL) && (xTimer_GREEN != 0)) {
    xTimer_RED_Started = xTimerStart(xTimer_RED, 0);
    xTimer_GREEN_Started = xTimerStart(xTimer_GREEN, 0);
  }

  while ((xTimer_RED_Start != pdPASS) && (xTimer_GREEN_Start != pdPASS)) {}
}

void blink_LEDS(TimerHandle_t xTimer){  
  // Get current ticks
  TickType_t current_tick = xTaskGetTickCount();

  if (xTimer == xTimer_RED){
    Serial.print("RED: ");
    Serial.println(current_tick);
    Serial.print("RED counter: ");
    Serial.println(RED_Profiler++);
    digitalWrite(RED_PIN, digitalRead(RED_PIN)^1);
  }
  else if(xTimer == xTimer_GREEN){
    Serial.print("GREEN: ");
    Serial.println(current_tick);
    Serial.print("GREEN counter: ");
    Serial.println(GREEN_Profiler++);
    digitalWrite(GREEN_PIN, digitalRead(GREEN_PIN)^1);
  }
  else if(RED_Profiler == 20){
    xTimerStop(xTimer_RED, 0);
    Serial.println("RED STOPPED!");
  }
  else if(GREEN_Profiler == 60){
    xTimer_RED_Start = xTimerStart(xTimer_RED, 0);
    Serial.println("RED RESTART!");    
  }
  
}

void loop() {}