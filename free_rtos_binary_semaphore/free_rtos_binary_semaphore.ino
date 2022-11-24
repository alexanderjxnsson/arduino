#include <Arduino_FreeRTOS.h>
#include "semphr.h"

// Create binary semaphore handle
SemaphoreHandle_t bSemaphore;

void setup() {
  Serial.begin(9600);

  // Create semaphore
  bSemaphore = xSemaphoreCreateBinary();
  if(bSemaphore != NULL){ // Create three tasks
    xTaskCreate(m1Writer, "Write msg1", 128, NULL, 1, NULL);
    xTaskCreate(m2Writer, "Write msg2", 128, NULL, 1, NULL);
    xTaskCreate(m3Writer, "Write msg3", 128, NULL, 1, NULL);
    xSemaphoreGive(bSemaphore);
  } else{for(;;);}
}

void m1Writer(void *  par){
  BaseType_t bSemStatus;

  while (1) {
    // Take semaphore
    bSemStatus = xSemaphoreTake(bSemaphore, portMAX_DELAY);
    if (bSemStatus == pdPASS) {
      Serial.println("[1] MSG 1");
      // Give semaphore
      xSemaphoreGive(bSemaphore);
      vTaskDelay(pdMS_TO_TICKS(500));
    } // If
    else{
      Serial.println("! Fail 1!");
    }
  } // While
} // Function

void m2Writer(void * par){
  BaseType_t bSemStatus;

  while (1) {
    // Take semahpore
    bSemStatus = xSemaphoreTake(bSemaphore, portMAX_DELAY);
    if (bSemStatus == pdPASS) {
      Serial.println("[2] MSG 2");
      // Give semaphore
      xSemaphoreGive(bSemaphore);
      vTaskDelay(pdMS_TO_TICKS(500));
    } // If
    else{
      Serial.println("! Fail 2!");
    }
  } // While
} // Function

void m3Writer(void * par){
  BaseType_t bSemStatus;

  while (1) {
    // Take semahpore
    bSemStatus = xSemaphoreTake(bSemaphore, portMAX_DELAY);
    if (bSemStatus == pdPASS) {
      Serial.println("[3] MSG 3");
      // Give semaphore
      xSemaphoreGive(bSemaphore);
      vTaskDelay(pdMS_TO_TICKS(500));
    } // If
    else{
      Serial.println("! Fail 3!");
    }
  } // While
} // Function

void loop() {}
