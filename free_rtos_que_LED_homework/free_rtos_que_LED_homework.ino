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
  TickType_t xTickToWait = pdMS_TO_TICKS(300);
  uint32_t payloadToSend = 0;

  while (1) {
    if (uxQueueSpacesAvailable(iQueue) >= 1) {
      qStatus = xQueueSend(iQueue, &payloadToSend, xTickToWait);
    }
    else {Serial.println("Queue is full from red!");}
    vTaskDelay(xTickToWait);
  }
}

void bGREEN(void* parameter){
  BaseType_t qStatus;
  TickType_t xTickToWait = pdMS_TO_TICKS(300);
  uint32_t payloadToSend = 1;

  while (1) {
    if (uxQueueSpacesAvailable(iQueue) >= 1) {
      qStatus = xQueueSend(iQueue, &payloadToSend, xTickToWait);
    }
    else {Serial.println("Queue is full from green!");}
    vTaskDelay(xTickToWait);
  }
}


void vReceiver(void* parameter){
  // declaration to payload will be recieved
  uint32_t payloadToReceive;
  BaseType_t qStatus;
  TickType_t xTickToWait = pdMS_TO_TICKS(300);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  while (1) {
    qStatus = xQueueReceive(iQueue, &payloadToReceive, xTickToWait);

    if (qStatus == pdPASS) {
      Serial.print("Received value is :");
      Serial.println(payloadToReceive);
      switch (payloadToReceive){
        case 0:
          Serial.println("RED LIGHT ON");
          digitalWrite(RED_PIN, HIGH);
          delay(100);
          digitalWrite(RED_PIN, LOW);
          break;
        case 1:
          Serial.println("GREEN LIGHT ON");
          digitalWrite(GREEN_PIN, HIGH);
          delay(100);
          digitalWrite(GREEN_PIN, LOW);
          break;
        default:
          Serial.println("Invalid input!");
          break;
      }
    }
    else {Serial.println("!!!-Error receving-!!!");}
  }
}

void loop() {}
