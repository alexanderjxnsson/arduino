#include <Arduino_FreeRTOS.h>
#include "semphr.h"
#include "queue.h"

static const char* printStr[] = {"[1] Task1", "[2] Task2", "[3] Tick Hook"};

QueueHandle_t xPrintQueue;

void setup() {
  Serial.begin(9600);
  // Create queue
  xPrintQueue = xQueueCreate(5, sizeof(char*));
  // Create tasks
  xTaskCreate(output, "Print task 1", 128, (void*)0, 1, NULL);
  xTaskCreate(output, "Print task 2", 128, (void*)1, 1, NULL);
  xTaskCreate(gatekeeperTask, "Gatekeeper", 128, NULL, 1, NULL);
}

void output(void * param){
  uint8_t index = (uint8_t)param;
  while(1){
    xQueueSend(xPrintQueue, &(printStr[index]), portMAX_DELAY);
  } // While
} // Function

void gatekeeperTask(void * param){
  char * payloadToPrint;
  while(1){
    xQueueReceive(xPrintQueue, &payloadToPrint, portMAX_DELAY);
    Serial.println(payloadToPrint);
  } // While
} // Function

void vApplicationTickHook(void){
  xQueueSend(xPrintQueue, &(printStr[2]), portMAX_DELAY);
  //xQueueSendToFrontFromISR(xPrintQueue, &(printStr[2]), portMAX_DELAY);
}

void loop(){}