#include <Arduino_FreeRTOS.h>
#include "semphr.h"

// Create Handle for our counting semaphore
SemaphoreHandle_t cSemaphore;

void setup() {
  Serial.begin(9600);

  // Init our semaphore
  cSemaphore = xSemaphoreCreateCounting(3, 0);

  // Create our tasks
  xTaskCreate(msg1Writer, "Write msg 1", 128, NULL, 1, NULL);
  xTaskCreate(msg2Writer, "Write msg 2", 128, NULL, 1, NULL);
  xTaskCreate(msg3Writer, "Write msg 3", 128, NULL, 1, NULL);
  xSemaphoreGive(cSemaphore);
  xSemaphoreGive(cSemaphore);
  xSemaphoreGive(cSemaphore);

}

void msg1Writer(void * param){
  BaseType_t cSemStatus;

  while (1) {
    // Get semaphore
    cSemStatus = xSemaphoreTake(cSemaphore, portMAX_DELAY);
    if (cSemStatus == pdPASS) {
      Serial.println("[1] MSG 1");
      xSemaphoreGive(cSemaphore);
      vTaskDelay(10);
    } // If
  } // While
} // Function

void msg2Writer(void * param){
  BaseType_t cSemStatus;

  while (1) {
    // Get semaphore
    cSemStatus = xSemaphoreTake(cSemaphore, portMAX_DELAY);
    if (cSemStatus == pdPASS) {
      Serial.println("[2] MSG 2");
      xSemaphoreGive(cSemaphore);
      vTaskDelay(10);
    } // If
  } // While
} // Function

void msg3Writer(void * param){
  BaseType_t cSemStatus;

  while (1) {
    // Get semaphore
    cSemStatus = xSemaphoreTake(cSemaphore, portMAX_DELAY);
    if (cSemStatus == pdPASS) {
      Serial.println("[3] MSG 3");
      xSemaphoreGive(cSemaphore);
      vTaskDelay(10);
    } // If
  } // While
} // Function

void loop() {}
