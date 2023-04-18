#include <Arduino_FreeRTOS.h>
#include "queue.h"

#define RED_PIN     6
#define YELLOW_PIN  7
#define GREEN_PIN   8

// Create two queues and queue set handle
QueueHandle_t iQueue1, iQueue2;
QueueSetHandle_t iQueueSet;
TickType_t xTickToWait = pdMS_TO_TICKS(500);

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  iQueue1 = xQueueCreate(2, sizeof(uint8_t));
  iQueue2 = xQueueCreate(2, sizeof(uint8_t));
  iQueueSet = xQueueCreateSet(2+2+1);
  xQueueAddToSet(iQueue1, iQueueSet);
  xQueueAddToSet(iQueue2, iQueueSet);

  xTaskCreate(send_LED, "Red signal task", 128, 0, 1, NULL);
  xTaskCreate(send_LED, "Green signal task", 128, 1, 1, NULL);
  xTaskCreate(led_Receiver, "Receive signal", 128, NULL, 1, NULL);

  void led_on_off(uint8_t GPIO);
}

void send_LED(void* param){
  uint8_t payloadToSend = param;
  BaseType_t qStatus;
  while (1) {
    qStatus = xQueueSend(iQueue1, &payloadToSend, 0);
    if (qStatus == pdPASS) {
      Serial.println("Sender1 sent");
      vTaskDelay(xTickToWait);
    }
  } // while
} // function

void led_Receiver(void* param){
  uint8_t payloadToReceive = param;
  QueueHandle_t activeQueue;
  BaseType_t qStatus;
  
  while(1){
    activeQueue = (QueueHandle_t)xQueueSelectFromSet(iQueueSet, portMAX_DELAY);
    qStatus = xQueueReceive(activeQueue, &payloadToReceive, 0);
    if(qStatus == pdTRUE){
      Serial.print("We got value: ");
      Serial.println(payloadToReceive);
      switch (payloadToReceive) {
        case 0:
          Serial.println("RED");
          led_on_off(RED_PIN);
          break;
        case 1:
          Serial.println("GREEN");
          led_on_off(GREEN_PIN);
          break;
        default:
          Serial.println("Invalid value!");
          break;
      } // switch
    } // if
    else if (qStatus == pdFALSE) {
      Serial.println("Error receiving value");
      led_on_off(YELLOW_PIN);
    }
    else {
      Serial.println("ERROR RECEIVING MSG");
    } // else
  } // while
} // function

void led_on_off(uint8_t GPIO){
  
  digitalWrite(GPIO, HIGH);
  delay(50);
  digitalWrite(GPIO, LOW);
}

void loop() {}
