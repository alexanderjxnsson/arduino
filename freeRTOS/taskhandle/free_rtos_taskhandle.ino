// (1) import freeRTOS
#include <Arduino_FreeRTOS.h>

#define RED 6
#define YELLOW 7
#define GREEN 8

const TickType_t _50ms = pdMS_TO_TICKS(50);

// create task proflier
typedef int TaskProfiler;
TaskProfiler REDprofiler = 0;
TaskProfiler YELLOWprofiler = 0;
TaskProfiler GREENprofiler = 0;

TaskHandle_t red_Handle , yellow_Handle, green_Handle;

void setup() {
  Serial.begin(9600);
  // (2) Setup of our Tasks
  //xTaskCreate(taskFunctionName, taskFuction desc, stack size = 128, Task parameter if any, prio = 1, task handle);
  xTaskCreate(toggleRED, "Toggle RED LED", 128, NULL, 1, &red_Handle);
  xTaskCreate(toggleYELLOW, "Toggle YELLOW LED", 128, NULL, 1, &yellow_Handle);
  xTaskCreate(toggleGREEN, "Toggle GREEN LED", 128, NULL, 1, &green_Handle);
}


void toggleRED(void *pvParameters){
  pinMode(RED, OUTPUT);
  
  while (1) {
    REDprofiler++;
    Serial.print("[RED P] = ");
    Serial.println(REDprofiler);

    digitalWrite(RED, digitalRead(RED) ^ 1);
    vTaskSuspend(green_Handle);
    vTaskPrioritySet(red_Handle,4);
    if (REDprofiler > 20) {
      vTaskPrioritySet(red_Handle, 1);
      vTaskDelay(_50ms);
    }
    
  }
}

 void toggleYELLOW(void *pvParameters){
  pinMode(YELLOW, OUTPUT);
  while(1){
    YELLOWprofiler++;
    Serial.print("[YELLOW P] = ");
    Serial.println(YELLOWprofiler);

    digitalWrite(YELLOW, digitalRead(YELLOW) ^ 1);
    
    vTaskResume(green_Handle);
    vTaskDelay(_50ms);
  }
}

void toggleGREEN(void *pvParameters){
  pinMode(GREEN, OUTPUT);

  while (1) {
    GREENprofiler++;
    Serial.print("[GREEN P] = ");
    Serial.println(GREENprofiler);

    digitalWrite(GREEN, digitalRead(GREEN) ^ 1);
    vTaskDelay(_50ms);
  }
}

void loop() {
}