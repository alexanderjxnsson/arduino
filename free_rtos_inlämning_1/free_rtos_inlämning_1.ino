// Including libraries
#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include "timers.h"


// Creating struct for the microwave with it's members
typedef struct {
  uint32_t prog_lenght_ms;      // Length of the program
  uint8_t door;                 // Door open or closed
  uint8_t lamp;                 // Lamp on or off
  uint16_t effect_of_heater;    // The effect of
  uint16_t disc_spin;           // Variable for the disc which we'll count in degrees
} sMicrowave;

#define PRINT_DELAY (pdMS_TO_TICKS(1000)) // Delay set to 1000ms(1sec), easier to calculate disc spin degree.

// Here we've initilaized the different programs of the microwave from the struct
// First we add program time in ms, door, lamp, effect and then disc
sMicrowave defrost_meat = {300000, 0, 0, 800, 0};
sMicrowave defrost_veg = {60000, 0, 0, 400, 0};
sMicrowave general_prog = {30000, 0, 0, 800, 0};

void setup() {
  Serial.begin(9600);

  // Creating our tasks
  //xTaskCreate(microwave_output, "Display microwave output", 128, &defrost_meat, 1, NULL);
  //xTaskCreate(microwave_output, "Display microwave output", 128, &defrost_veg, 1, NULL);
  xTaskCreate(microwave_output, "Display microwave output", 128, &general_prog, 1, NULL);
}

void microwave_output(void* input_struct){
  sMicrowave * local_struct = (sMicrowave *) input_struct;

  // Printing program effect
  Serial.print("Effect set to: ");
  Serial.println(local_struct->effect_of_heater);

  if (local_struct->door == 0 && local_struct->door == 0){
    Serial.println("Door is closed and light is off!");
  }

  float prog_length_sec = (local_struct->prog_lenght_ms / 1000); // Calculate program to seconds
  if (prog_length_sec >= 60) {                                   // If the program is 1 min or longer display it as minutes
    Serial.print("Program length is set to: ");
    Serial.print(prog_length_sec);
    Serial.println(" minutes.");
  }
  else {                                                        // If the program is less than 60 sec, display it as secnds

    Serial.print("Program length is set to: ");
    Serial.print(prog_length_sec, 0);                           // (prog_length_sec, 0) to remove de decimals to make it cleaner
    Serial.println(" seconds.");
  }

  while (local_struct->prog_lenght_ms != 0) {
    local_struct->prog_lenght_ms -= PRINT_DELAY;
    // Disc spin calculation
    Serial.print("Disc is at: ");
    Serial.println(local_struct->disc_spin);
    local_struct->disc_spin += 30;                              // += 30 for 30 degrees/s
    if (local_struct->disc_spin >= 360) {
      local_struct->disc_spin = 0;
    }
    vTaskDelay(PRINT_DELAY);
  }
}

void loop() {}
