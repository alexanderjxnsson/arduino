/* Binary semaphore to sync three tasks */

#include <Arduino_FreeRTOS.h>
#include "semphr.h"

#define RED_PIN     6
#define YELLOW_PIN  7
#define GREEN_PIN   8

SemaphoreHandle_t bSemaphore;

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // Create binary
  bSemaphore = xSemaphoreCreateBinary();

  // Create tasks
  xTaskCreate(msg1Writer, "Write msg 1", 128, NULL, 1, NULL);
  xTaskCreate(msg2Writer, "Write msg 2", 128, NULL, 1, NULL);
  xTaskCreate(msg3Writer, "Write msg 3", 128, NULL, 1, NULL);
  xSemaphoreGive(bSemaphore);
}

void msg1Writer(void * param){
  BaseType_t bSemStatus;
  while (1) {
    // Take semaphore
    bSemStatus = xSemaphoreTake(bSemaphore, portMAX_DELAY);
    if (bSemStatus == pdPASS) {
      Serial.println("[1] MSG 1");
      digitalWrite(RED_PIN, digitalRead(RED_PIN)^1);
      vTaskDelay(pdMS_TO_TICKS(200));
      xSemaphoreGive(bSemaphore);
      vTaskDelay(pdMS_TO_TICKS(1));
    } // If
  } // While
} // Function msg1

void msg2Writer(void * param){
  BaseType_t bSemStatus;
  while (1) {
    // Take semaphore
    bSemStatus = xSemaphoreTake(bSemaphore, portMAX_DELAY);
    if (bSemStatus == pdPASS) {
      Serial.println("[2] MSG 2");
      digitalWrite(YELLOW_PIN, digitalRead(YELLOW_PIN)^1);
      vTaskDelay(pdMS_TO_TICKS(200));
      xSemaphoreGive(bSemaphore);
      vTaskDelay(pdMS_TO_TICKS(1));
    } // If
  } // While
} // Function msg2

void msg3Writer(void * param){
  BaseType_t bSemStatus;
  while (1) {
    // Take semaphore
    bSemStatus = xSemaphoreTake(bSemaphore, portMAX_DELAY);
    if (bSemStatus == pdPASS) {
      Serial.println("[3] MSG 3");
      digitalWrite(GREEN_PIN, digitalRead(GREEN_PIN)^1);
      vTaskDelay(pdMS_TO_TICKS(200));
      xSemaphoreGive(bSemaphore);
      vTaskDelay(pdMS_TO_TICKS(1));
    } // If
  } // While
} // Function msg3

void loop() {}
