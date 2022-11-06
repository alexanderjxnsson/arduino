// https://www.hackster.io/Niket/tasks-parametertotasks-freertos-tutorial-5-b8a7b7

#include <Arduino_FreeRTOS.h>

#define RED_PIN 6
#define YELLOW_PIN 7
#define GREEN_PIN 8


typedef struct {
  // uint16_t instead of regular int
  uint16_t sPIN;
  uint16_t sDELAY;
} LED_struct_t;

// Made structs global
LED_struct_t sRED = {RED_PIN, 500};
LED_struct_t sYELLOW = {YELLOW_PIN, 200};
LED_struct_t sGREEN = {GREEN_PIN, 1000};

bool debug_flag = true;

void setup() {
  Serial.begin(9600);

  if (debug_flag){
    Serial.println("SETUP: ");
    Serial.println(sRED.sPIN);
    Serial.println(sRED.sDELAY);
    Serial.println(sYELLOW.sPIN);
    Serial.println(sYELLOW.sDELAY);
    Serial.println(sGREEN.sPIN);
    Serial.println(sGREEN.sDELAY);
  }
  // & on struct parameter
  xTaskCreate(toggle_LED, "Toggle LED via struct", 128, &sRED, 1, NULL);
  xTaskCreate(toggle_LED, "Toggle LED via struct", 128, &sYELLOW, 1, NULL);
  xTaskCreate(toggle_LED, "Toggle LED via struct", 128, &sGREEN, 1, NULL);
}

void toggle_LED(void* xStruct){
  // Ammar's fix, made local struct for function
  LED_struct_t local_struct = *(LED_struct_t *) xStruct;
  uint16_t local_pin = local_struct.sPIN;
  uint16_t local_delay = local_struct.sDELAY;
  // end fix

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  while(1){
    if (debug_flag) {
      Serial.println("DEBUG: ");
      Serial.print("PIN: ");
      Serial.println(local_pin);
      Serial.print("DELAY: ");
      Serial.println(local_delay);
    }

    digitalWrite(local_pin, HIGH);
    delay(250);
    digitalWrite(local_pin, LOW);
    // Used vTaskDelay instead of regular delay()-function
    vTaskDelay(pdMS_TO_TICKS(local_delay));
  }
}

void loop() {}