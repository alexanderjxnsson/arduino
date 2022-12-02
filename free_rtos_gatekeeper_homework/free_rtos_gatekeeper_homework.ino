#include <Arduino_FreeRTOS.h>
#include "semphr.h"

static const char*printStr[]={
  "[1] TASK 1",
  "[2] TASK 2",
  "[3] Tickhook"
};

#define RED  7
#define GREEN   8

QueueHandle_t xQueueGateKeeper;

void setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  xQueueGateKeeper = xQueueCreate(5, sizeof(char*));

  xTaskCreate(output, " print task 1 and blink red", 128, (void*)0, 1, NULL);
  xTaskCreate(output, " print task 2 and blink green", 128, (void*)1, 1, NULL);
  xTaskCreate(gatekeeper, "gatekeeper", 128, NULL, 1, NULL);
}

void output(void* param){
  uint8_t index = (uint8_t)param;
  while (1) {
    xQueueSend(xQueueGateKeeper, &(printStr[index]), portMAX_DELAY);
  }
}

void gatekeeper(void * param){
  char* payloadToPrint;
  while (1) {
    xQueueReceive(xQueueGateKeeper, &payloadToPrint, portMAX_DELAY);
    if (payloadToPrint == printStr[0]) {
        Serial.println(payloadToPrint);
        digitalWrite(RED, digitalRead(RED)^1);
    }
      case 1:
        Serial.println(payloadToPrint);
        digitalWrite(GREEN, digitalRead(GREEN)^1);
        break;
      case 2:
        Serial.println(payloadToPrint);
        break;
      default:
        Serial.println("ERROR VALUE");
        break;
    }
  }
}

void vApplicationTickHook(void){
  xQueueSendToFront(xQueueGateKeeper, &(printStr[2]), NULL);
}

void loop() {}
