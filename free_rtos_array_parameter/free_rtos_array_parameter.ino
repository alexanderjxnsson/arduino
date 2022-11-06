// https://esp32.com/viewtopic.php?t=18796

#include <Arduino_FreeRTOS.h>

#define RED_PIN 6
#define YELLOW_PIN 7
#define GREEN_PIN 8

bool debug_flag = true;
uint16_t task_array[3][2] = {{RED_PIN, 200},
                            {YELLOW_PIN, 500},
                            {GREEN_PIN, 1000}};

void setup() {
  Serial.begin(9600);

  if (debug_flag) {
    Serial.println("SETUP: ");
    for (int x = 0; x < 3; x++) {
      for (int y = 0; y < 2; y++) {
        Serial.println(task_array[x][y]);
      }
    }

  }
  // for loop för varje, kanske inte.
  /* xTaskCreate(toggle_LED, "Toggle leds via array", 128, &task_array[0][0], NULL);
  xTaskCreate(toggle_LED, "Toggle leds via array", 128, &task_array[1][1], NULL);
  xTaskCreate(toggle_LED, "Toggle leds via array", 128, &task_array[2][2], NULL); */
}


void loop() {}