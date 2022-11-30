#include <Arduino_FreeRTOS.h>
#include "event_groups.h"

// Create handle for group event
EventGroupHandle_t xEventGroup;

// Setting bits for two tasks
#define TASK1_BIT (1UL<<0UL)
#define TASK2_BIT (1UL<<1UL)
#define TASK3_BIT (1UL<<2UL)
#define TASK4_BIT (2UL<<3UL)

void setup() {
  Serial.begin(9600);

  xEventGroup = xEventGroupCreate();

  // Create and set tasks
  xTaskCreate(EventBitSetter, "Setter for bits", 128, NULL, 1, NULL);
  xTaskCreate(EventBitGetter, "Getter of bits", 128, NULL, 1, NULL);
}

void EventBitSetter(void* par){
  // Set bit to task 1 and 2 with delay 500ms
  while (1) {
    xEventGroupSetBits(xEventGroup, TASK1_BIT);
    vTaskDelay(pdMS_TO_TICKS(500));
    xEventGroupSetBits(xEventGroup, TASK2_BIT);
    vTaskDelay(pdMS_TO_TICKS(500));
    xEventGroupSetBits(xEventGroup, TASK3_BIT);
    vTaskDelay(pdMS_TO_TICKS(500));
    xEventGroupSetBits(xEventGroup, TASK4_BIT);
  } // While
} // Function

void EventBitGetter(void* par){
  const EventBits_t xBitsToWaitFor = (TASK1_BIT | TASK2_BIT | TASK3_BIT | TASK4_BIT);
  EventBits_t xEventGroupValue;

  while (1) {
    xEventGroupValue = xEventGroupWaitBits(xEventGroup, xBitsToWaitFor, pdTRUE, pdTRUE, portMAX_DELAY);
    if ((xEventGroupValue & TASK1_BIT) != 0) {
      Serial.println("[1] TASK 1");
    }
    if ((xEventGroupValue & TASK2_BIT) != 0) {
      Serial.println("[2] TASK 2");
    }
    if ((xEventGroupValue & TASK3_BIT) != 0) {
      Serial.println("[3] TASK 3");
    }
    if ((xEventGroupValue & TASK4_BIT) != 0) {
      Serial.println("[4] TASK 4");
    }
  } // While
} // Function

void loop() {}
