#include <Arduino_FreeRTOS.h>
#include "queue.h"
// define pins
#define RED_PIN     6
#define YELLOW_PIN  7
#define GREEN_PIN   8

QueueHandle_t iQueue;

void setup() {
  Serial.begin(9600);
  // Created queue
  iQueue = xQueueCreate(5, sizeof(uint32_t));
  if (iQueue != NULL) {
    xTaskCreate(bRED, "Red sender", 128, NULL, 1, NULL);
    xTaskCreate(bGREEN, "Green sender", 128, NULL, 1, NULL);
    xTaskCreate(vReceiver, "Reciver task", 128, NULL, 1, NULL);
  }
  else{
    for(;;){}
  }
  
}

void bRED(void* parameter){
  BaseType_t qStatus;
  TickType_t xTickToWait = pdMS_TO_TICKS(100);
  uint32_t payloadToSend;

  while (1) {
    payloadToSend = 1;
    if (uxQueueSpacesAvailable(iQueue) >= 1) {
      qStatus = xQueueSend(iQueue, &payloadToSend, 100);
      payloadToSend = 0;
    }
    else {Serial.println("Queue is full!");}
  }
}

void bGREEN(void* parameter){
  BaseType_t qStatus;
  TickType_t xTickToWait = pdMS_TO_TICKS(100);
  uint32_t payloadToSend = 1;

  while (1) {
    payloadToSend = 1;
    if (uxQueueSpacesAvailable(iQueue) >= 1) {
      qStatus = xQueueSend(iQueue, &payloadToSend, 100);
      payloadToSend = 0;
    }
    else {Serial.println("Queue is full!");}
  }
}


void vReceiver(void* parameter){
  // declaration to payload will be recieved
  uint32_t payloadToReceive;
  BaseType_t qStatus;
  TickType_t xTickToWait = pdMS_TO_TICKS(100);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  while (1) {
    qStatus = xQueueReceive(iQueue, &payloadToReceive, 100);
    if (qStatus == pdPASS) {
      Serial.print("Received value is :");
      Serial.println(payloadToReceive);

      // turn leds on and off

    }
    else {Serial.println("!!!-Error receving-!!!");}
  }
}

void loop() {}
