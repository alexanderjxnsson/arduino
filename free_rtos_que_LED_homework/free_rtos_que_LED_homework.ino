#include <Arduino_FreeRTOS.h>
#include "queue.h"
// define pins
#define RED_PIN     6
#define YELLOW_PIN  7
#define GREEN_PIN   8

QueueHandle_t iQueue;
TickType_t xTickToWait = pdMS_TO_TICKS(300);

void setup() {
  Serial.begin(9600);
  // Created queue
  iQueue = xQueueCreate(5, sizeof(uint32_t));
  if (iQueue != NULL) {
    xTaskCreate(bRED, "Red sender", 128, 0, 1, NULL);
    xTaskCreate(bGREEN, "Green sender", 128, 1, 1, NULL);
    //xTaskCreate(vSender, "Sender", 128, 1 || 0, 1, NULL);
    //xTaskCreate(vSender, "Sender", 128, 1 || 0, 1, NULL);
    xTaskCreate(vReceiver, "Reciver task", 128, NULL, 2, NULL);
    void led_on_off(uint8_t GPIO);
  }
  else{
    for(;;){}
  }
}

void bRED(void* parameter){
  BaseType_t qStatus;

  uint8_t payloadToSend = (uint8_t)parameter;

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
  
  uint8_t payloadToSend = (uint8_t)payloadToSend;

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
          led_on_off(RED_PIN);
          break;
        case 1:
          Serial.println("GREEN LIGHT ON");
          led_on_off(GREEN_PIN);
          break;
        default:
          Serial.println("Invalid input!");
          break;
      }
    }
    else {Serial.println("!!!-Error receving-!!!");}
  }
}

void led_on_off(uint8_t GPIO){
  digitalWrite(GPIO, HIGH);
  delay(100);
  digitalWrite(GPIO, LOW);
}

void loop() {}
