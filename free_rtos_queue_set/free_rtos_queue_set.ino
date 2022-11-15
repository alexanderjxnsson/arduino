#include <Arduino_FreeRTOS.h>
#include "queue.h"

// Create two queues and queue set handle
QueueHandle_t iQueue1, iQueue2;
QueueSetHandle_t iQueueSet;
TickType_t xTickToWait = pdMS_TO_TICKS(500);

void setup(){
  Serial.begin(9600);

  // Create two queues
  iQueue1 = xQueueCreate(5, sizeof(char*));
  iQueue2 = xQueueCreate(5, sizeof(char*));
  // create queueSet and add these two queues
  iQueueSet = xQueueCreateSet(5+5+1);
  xQueueAddToSet(iQueue1, iQueueSet);
  xQueueAddToSet(iQueue2, iQueueSet);
  // create tasks
  xTaskCreate(message_sender1, "Sender msg one", 128, NULL, 1, NULL);
  xTaskCreate(message_sender2, "Sender msg two", 128, NULL, 1, NULL);
  xTaskCreate(message_receiver, "Receive msg", 128, NULL, 2, NULL);
}

void message_sender1(void* param){
  const char * msg = "# Hi from 1";
  BaseType_t qStatus;
  while (1) {
    qStatus = xQueueSend(iQueue1, &msg, 0);
    if (qStatus == pdPASS) {
      Serial.println("Sender1 sent");
      vTaskDelay(xTickToWait);
    }
  } // while
} // function

void message_sender2(void* param){
  const char * msg = "# Hi from 2";
  BaseType_t qStatus;
  while (1) {
    qStatus = xQueueSend(iQueue2, &msg, 0);
    if (qStatus == pdPASS) {
      Serial.println("Sender2 sent");
      vTaskDelay(xTickToWait);
    }
  } // while
} // function

void message_receiver(void* param){
  char * msg;
  QueueHandle_t activeQueue;
  BaseType_t qStatus;
  
  while(1){
    activeQueue = (QueueHandle_t)xQueueSelectFromSet(iQueueSet, portMAX_DELAY);
    qStatus = xQueueReceive(activeQueue, &msg, 0);
    if(qStatus == pdPASS){
      Serial.println(msg);
    }
    else {
      Serial.println("ERROR RECEIVING MSG");
    } // if & else
  } // while
} // function

void loop() {}