#include <Arduino_FreeRTOS.h>

#define RED 6
#define YELLOW 7
#define GREEN 8

typedef struct data_t{
  uint8_t * _pin;
  TickType_t _delay = pdMS_TO_TICKS(delay);  
} sRED, sYELLOW, sGREEN;

typedef int TaskProfiler;
TaskProfiler REDprofiler;
TaskProfiler YELLOWprofiler;
TaskProfiler GREENprofiler;

const TickType_t _50ms = pdMS_TO_TICKS(50);
const uint8_t *redLED = (uint8_t *) RED;
const uint8_t *yellowLED = (uint8_t *) YELLOW;
//const uint8_t *greenLED = (uint8_t t *) GREEN;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  xTaskCreate(toggleLED, "Toggle RED", 128, (void *)redLED, 1, NULL);
  xTaskCreate(toggleLED, "Toggle YELLOW", 128, (void *)yellowLED, 1, NULL);
  //xTaskCreate(toggleGREEN, "Toggle GREEN", 128, (void *)greenLED, 1, NULL);
}

void toggleLED(void *pvParameters){
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);

  while(1){
    switch((uint8_t) pvParameters){
      case RED:{
        REDprofiler++;
        Serial.print("RED P = ");
        Serial.print(REDprofiler);
        Serial.print("\n");
      };
      case YELLOW:{
        YELLOWprofiler++;
        Serial.print("YELLOW P = ");
        Serial.print(YELLOWprofiler);
        Serial.print("\n");
      };
    }
    digitalWrite((uint8_t)pvParameters, digitalRead((uint8_t)pvParameters)^1);
    vTaskDelay(_50ms);
 }
}

void loop() {
  // put your main code here, to run repeatedly:

}
