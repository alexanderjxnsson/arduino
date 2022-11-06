// https://www.hackster.io/Niket/tasks-parametertotasks-freertos-tutorial-5-b8a7b7

#include <Arduino_FreeRTOS.h>

#define RED_PIN 6
#define YELLOW_PIN 7
#define GREEN_PIN 8


typedef struct LED_struct{
  int sPIN;
  int sDELAY;
} sLEDS;

bool debug_flag = true;

void setup() {
  Serial.begin(9600);
  sLEDS sRED = {RED_PIN, 500};
  sLEDS sYELLOW = {YELLOW_PIN, 200};
  sLEDS sGREEN = {GREEN_PIN, 1000};

  Serial.println("SETUP: ");
  Serial.println(sRED.sPIN);
  Serial.println(sRED.sDELAY);
  Serial.println(sYELLOW.sPIN);
  Serial.println(sYELLOW.sDELAY);
  Serial.println(sGREEN.sPIN);
  Serial.println(sGREEN.sDELAY);

  xTaskCreate(toggle_LED, "Toggle LED via struct", 128, &sRED, 1, NULL);
  xTaskCreate(toggle_LED, "Toggle LED via struct", 128, &sYELLOW, 1, NULL);
  xTaskCreate(toggle_LED, "Toggle LED via struct", 128, &sGREEN, 1, NULL);
}

void toggle_LED(struct LED_struct *xStruct){
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  while(1){
    if (debug_flag == true) {
      Serial.println("DEBUG: ");
      Serial.print("PIN: ");
      Serial.println(xStruct->sPIN);
      Serial.print("DELAY: ");
      Serial.println(xStruct->sDELAY);
    }

    digitalWrite(xStruct->sPIN, HIGH);
    delay(250);
    digitalWrite(xStruct->sPIN, LOW);
    delay(xStruct->sDELAY);
  }
}

void loop() {
}