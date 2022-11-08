// https://esp32.com/viewtopic.php?t=18796

#include <Arduino_FreeRTOS.h>

#define RED_PIN 6
#define YELLOW_PIN 7
#define GREEN_PIN 8

bool debug_flag = true;
uint16_t task_array[3][2] = {{RED_PIN, 200},
                            {YELLOW_PIN, 500},
                            {GREEN_PIN, 1000}};

uint16_t array1[6] = {RED_PIN, 200,
                      YELLOW_PIN, 500,
                      GREEN_PIN, 1000};

void setup() {
  Serial.begin(9600);

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  if (debug_flag == true) {
    for (int x = 0; x < 3; x++) {
      for (int y = 0; y < 2; y++) {
        Serial.println(task_array[x][y]);
      }
    }

  }
  // for loop för varje, kanske inte.
  for (int x = 0; x < 6; x++){
    xTaskCreate(toggle_LED, "Toggle leds via array", 128, &array1, 1, NULL);
  }

}

void toggle_LED(uint16_t *arr){
  uint16_t* local_array = (uint16_t *) arr;

  if (debug_flag) {
    Serial.println("FUNCTION: ");
    for (int x = 0; x < sizeof(local_array); x++) {
        Serial.println(local_array[x]);
    }
  }
  
}

void loop() {}