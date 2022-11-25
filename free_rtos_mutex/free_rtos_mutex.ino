/* Protect Serial with mutex from two tasks */
#include <Arduino_FreeRTOS.h>
#include "semphr.h"

SemaphoreHandle_t xMutex;

void setup() {
  Serial.begin(9600);

  // Create mutex
  xMutex = xSemaphoreCreateMutex();

  // Create tasks
  xTaskCreate(print_out, "Prints msg one", 128, "Task one\r\n", 1, NULL);
  xTaskCreate(print_out, "Prints msg two", 128, "Task two\r\n", 1, NULL);
}

void print_out(void * param){
  char* stringToPrint = (char*)param;
  while (1) {
    printer(stringToPrint);
    vTaskDelay(pdMS_TO_TICKS(1));
  } // While
} // Function print_out

void printer(const char* pcString){
  xSemaphoreTake(xMutex, portMAX_DELAY);
  Serial.println(pcString);
  xSemaphoreGive(xMutex);
}

void loop() {}
