#include <Arduino_FreeRTOS.h>
#include "queue.h"

typedef struct {
  uint32_t prog_lenght_ms;
  uint8_t door;
  uint8_t lamp;
  uint16_t heater;
  uint16_t disc_spin;
} microwave_t;

microwave_t defrost_meat = {300000, 0, 0, 800, 0};
microwave_t defrost_veg = {60000, 0, 0, 400, 0};
microwave_t general_prog = {30000, 0, 0, 800, 0};

void setup() {
  Serial.begin(9600);

  xTaskCreate(microwave_output, "Display microwave output", 128, &defrost_meat, 1, NULL);
}

void microwave_output(void* input_struct){
  microwave_t * local_struct = (microwave_t *) input_struct;
  Serial.println(local_struct->prog_lenght_ms);
  Serial.println(local_struct->door);
  Serial.println(local_struct->lamp);
  Serial.println(local_struct->heater);
  Serial.println(local_struct->disc_spin);
  vTaskDelay(1000);
}

void loop() {}
