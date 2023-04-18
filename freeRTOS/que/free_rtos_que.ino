#include <Arduino_FreeRTOS.h>
#include "queue.h"

QueueHandle_t iQueue;

void setup() {
  Serial.begin(9600);
  iQueue = xQueueCreate(5, sizeof(uint32_t));
  if (iQueue != NULL) {
    xTaskCreate(vSender, "Sender task", 128, NULL, 1, NULL);
    xTaskCreate(vReceiver, "Reciver task", 128, NULL, 1, NULL);
  }
  else{
    for(;;){}
  }
  
}

void vSender(void* parameter){
  // create parameter for return value of Queue send
  BaseType_t qStatus;
  TickType_t xTickToWait = pdMS_TO_TICKS(100);
  uint32_t payloadToSend = 2022;

  while (1) {
    // send payload to iQueue
    if (uxQueueSpacesAvailable(iQueue) >= 1) {
      qStatus = xQueueSend(iQueue, &payloadToSend, xTickToWait);
      payloadToSend += 10;
    }
    else {Serial.println("Queue is  full!");}
  }
}

void vReceiver(void* parameter){
  // declaration to payload will be recieved
  uint32_t payloadToReceive;
  BaseType_t qStatus;
  TickType_t xTickToWait = pdMS_TO_TICKS(100);

  while (1) {
    qStatus = xQueueReceive(iQueue, &payloadToReceive, xTickToWait);
    if (qStatus == pdPASS) {
      Serial.print("Received value is :");
      Serial.println(payloadToReceive);
    }
    else {Serial.println("!!!-Error receving-!!!");}
  }
}

void loop() {}
