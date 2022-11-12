#include <Arduino_FreeRTOS.h>
#include "queue.h"

#define RED_PIN     6
#define YELLOW_PIN  7
#define GREEN_PIN   8

QueueHandle_t iQueue;
TickType_t xTickToWait = pdMS_TO_TICKS(300);
TaskHandle_t red_Handle, green_Handle;

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  void led_on_off(uint8_t GPIO);

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
  BaseType_t qStatus;
  uint8_t payloadToReceive;

  while (1) {
    qStatus = xQueueReceive(iQueue, &payloadToReceive, xTicksToWait);
    if (qStatus == pdPASS) {
      Serial.print("Received value is: ");
      Serial.println(payloadToReceive);

      switch (payloadToReceive) {
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
      } // switch // if
    }
    else {
      Serial.println("!!!-Error receving-!!!"); 
    } // else
  } // while  
} // func

void led_on_off(uint8_t GPIO){
  digitalWrite(GPIO, HIGH);
  delay(100);
  digitalWrite(GPIO, LOW);
}

void loop() {}
