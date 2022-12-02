#include <Arduino_FreeRTOS.h>
#include "semphr.h"

SemaphoreHandle_t xMutex;

#define RED     6
#define YELLOW  7
#define GREEN   8

oid setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  xMutex = xSemaphoreCreateMutex();

  xTaskCreate(action_output, " print task 1 & toggle red", 128, (void*)1, 1, NULL);
  xTaskCreate(action_output, " print task 2 & toggle yellow", 128, (void*)2, 1, NULL);
  xTaskCreate(action_output, " print task 3 & toggle green", 128, (void*)3, 1, NULL);
}

void action_output(void * param){
  uint8_t taskNum = (uint8_t)param;
  while (1) {
    xMutex_action(taskNum);
    xTaskDelay(pdMS_TO_TICK(100));
  }
}

void xMutex_action(const uint8_t param){
  xSemaphoreTake(xMutex, portMAX_DELAY);
  switch (param) {
    case 1:
      Serial.println("TASK 1");
      digitalWrite(RED, digitalRead(RED)^1);
      xSemaphoreGive(xMutex);
      break;
    case 2:
      Serial.println("TASK 2");
      digitalWrite(YELLOW, digitalRead(YELLOW)^1);
      xSemaphoreGive(xMutex);
      break;
    case 3:
      Serial.println("TASK 2");
      digitalWrite(YELLOW, digitalRead(YELLOW)^1);
      xSemaphoreGive(xMutex);
      break;
    default:
      Serial.println("ERROR VALUE");
      break;
  }
}

void loop() {}
