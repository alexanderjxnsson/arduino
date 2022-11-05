// https://www.hackster.io/Niket/tasks-parametertotasks-freertos-tutorial-5-b8a7b7

#include <Arduino_FreeRTOS.h>

#define RED_PIN 6
#define YELLOW_PIN 7
#define GREEN_PIN 8


typedef struct LED_struct{
  uint8_t sPIN;
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

  xTaskCreate(LED_struct, "Toggle LED via struct", 128, (void *)&sRED, 1, NULL);
  xTaskCreate(LED_struct, "Toggle LED via struct", 128, (void *)&sYELLOW, 1, NULL);
  xTaskCreate(LED_struct, "Toggle LED via struct", 128, (void *)&sGREEN, 1, NULL);
}

void LED_struct(void *xStruct){
  while(1){
    sLEDS * local_struct = (sLEDS *) &xStruct;
    pinMode(RED_PIN, OUTPUT);
    pinMode(YELLOW_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);

    if (debug_flag == true) {
      Serial.println("DEBUG: ");
      Serial.print("PIN: ");
      Serial.println(local_struct->sPIN);
      Serial.print("DELAY: ");
      Serial.println(local_struct->sDELAY);
    }

    digitalWrite(local_struct->sPIN, HIGH);
    delay(250);
    digitalWrite(local_struct->sPIN, LOW);
    delay(local_struct->sDELAY);
  }
}

void loop() {

}