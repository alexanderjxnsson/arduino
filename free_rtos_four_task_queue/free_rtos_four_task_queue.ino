#include <Arduino_FreeRTOS.h>
#include "queue.h"

#define RED_PIN     6
#define YELLOW_PIN  7
#define GREEN_PIN   8

QueueHandle_t iQueue;
TickType_t xTickToWait = pdMS_TO_TICKS(300);

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  iQueue = xQueueCreate(5, sizeof(uint8_t));
  if (iQueue != NULL){
    xTaskCreate(vSender, "RED SENDER", 128, 0, 1, NULL);
    xTaskCreate(vSender, "GREEN SENDER", 128, 1, 1, NULL);
    xTaskCreate(vReceiver, "RED RECEIVER", 128, NULL, 2, NULL);
    xTaskCreate(vReceiver, "GREEN RECEIVER", 128, NULL, 2, NULL);
  } else { for(;;){}}
}

void vSender(void * param){
  BaseType_t qStatus;
  uint8_t payloadToSend = (uint8_t)param;

  while(1){
    if(uxQueueSpacesAvailable(iQueue) >= 1){
      qStatus = xQueueSend(iQueue, &payloadToSend, xTickToWait);
      if(qStatus == pdPASS){
        Serial.print("Sent: ");
        Serial.println(payloadToSend);
      }
    }
    else{
      Serial.println("Queue is full!");
    }
    vTaskDelay(xTickToWait);
  }
}

void vReceiver(void * param){

}

void loop() {}
