// (1) import freeRTOS
#include <Arduino_FreeRTOS.h>

#define RED 6
#define YELLOW 7
#define GREEN 8
// create task proflier
typedef int TaskProfiler;
TaskProfiler REDprofiler;
TaskProfiler YELLOWprofiler;
TaskProfiler GREENprofiler;

void setup() {
  Serial.begin(9600);
  // (2) Setup of our Tasks
  //xTaskCreate(taskFunctionName, taskFuction desc, stack size = 128, Task parameter if any, prio = 1, task handle);
  xTaskCreate(toggleRED, "Toggle RED LED", 128, NULL, 1, NULL);
  xTaskCreate(toggleYELLOW, "Toggle YELLOW LED", 128, NULL, 1, NULL);
  xTaskCreate(toggleGREEN, "Toggle GREEN LED", 128, NULL, 1, NULL);
}

// (3) Let's implement functions of our tasks
void toggleRED(void *pvParameters){
  // (4) Set pin mode
  pinMode(RED, OUTPUT);
  
  
  while (1) {
    REDprofiler++;
    Serial.print("[RED P] = ");
    Serial.print(REDprofiler);
    Serial.print("\n");
    digitalWrite(RED, digitalRead(RED) ^ 1);
    delay(50);
  }
}

 void toggleYELLOW(void *pvParameters){
  pinMode(YELLOW, OUTPUT);
  while(1){
    YELLOWprofiler++;
    Serial.print("[YELLOW P] = ");
    Serial.print(YELLOWprofiler);
    Serial.print("\n");
    digitalWrite(YELLOW, digitalRead(YELLOW) ^ 1);
    delay(50);
  }
}

void toggleGREEN(void *pvParameters){
  pinMode(GREEN, OUTPUT);

  while (1) {
    GREENprofiler++;
    Serial.print("[GREEN P] = ");
    Serial.print(GREENprofiler);
    Serial.print("\n");
    digitalWrite(GREEN, digitalRead(GREEN) ^ 1);
    delay(50);
  }
}

void loop() {
}