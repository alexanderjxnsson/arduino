#include <Arduino_FreeRTOS.h>
#include <stdio.h>

#define RED 6
#define YELLOW 7
#define GREEN 8

bool countdownGREEN = true;
bool countdown = true;


typedef int TaskProfiler;
TaskProfiler REDprofiler;
TaskProfiler YELLOWprofiler;
TaskProfiler GREENprofiler;

void setup() {

  Serial.begin(9600);
  xTaskCreate(toggleRED_YELLOW, "Toggle RED & YELLOW LED", 128, NULL, 1, NULL);
  xTaskCreate(toggleGREEN, "Toggle GREEN LED", 128, NULL, 1, NULL);
}


void toggleRED_YELLOW(void *pvParameters){
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);

  while (countdown == true) {
    REDprofiler++;
    YELLOWprofiler++;

    Serial.print("RED = ");
    Serial.print(REDprofiler);
    Serial.print("\n");
    Serial.print("YELLOW = ");
    Serial.print(YELLOWprofiler);
    Serial.print("\n");

    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, HIGH);
    delay(250);
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    delay(5000);

    if (REDprofiler == 10 || YELLOWprofiler == 10) {
      vTaskSuspend(NULL);
    }
  }
}

 void toggleGREEN(void *pvParameters){
  pinMode(GREEN, OUTPUT);

  while (countdown == true) {
    GREENprofiler++;
    Serial.print("GREEN = ");
    Serial.print(GREENprofiler);
    Serial.print("\n");

    digitalWrite(GREEN, HIGH);
    delay(250);
    digitalWrite(GREEN, LOW);
    delay(5000);
    
    if (GREENprofiler == 20) {
      vTaskSuspend(NULL);
    }
  }
}

void loop() {
}