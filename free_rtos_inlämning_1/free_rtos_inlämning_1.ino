// Including libraries
#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include "timers.h"


// Creating struct for the microwave with it's members
typedef struct {
  uint32_t prog_length_s;      // Length of the program
  uint8_t door;                 // Door open or closed
  uint8_t lamp;                 // Lamp on or off
  uint16_t effect_of_heater;    // The effect of
  uint16_t disc_spin;           // Variable for the disc which we'll count in degrees
} sMicrowave;

#define PRINT_DELAY (pdMS_TO_TICKS(1000)) // Delay set to 1000ms(1sec), easier to calculate disc spin degree.

// Here we've initilaized the different programs of the microwave from the struct
// First we add program time in seconds, door, lamp, effect and then disc
sMicrowave defrost_meat = {180, 0, 0, 800, 0};
sMicrowave defrost_veg = {60, 0, 0, 400, 0};
sMicrowave general_prog = {30, 0, 0, 800, 0};

void setup() {
  Serial.begin(9600);
  

  // Creating our tasks
  xTaskCreate(microwave_output, "Display microwave output", 128, &defrost_meat, 1, NULL);
  //xTaskCreate(microwave_output, "Display microwave output", 128, &defrost_veg, 1, NULL);
  //xTaskCreate(microwave_output, "Display microwave output", 128, &general_prog, 1, NULL);
}

void microwave_output(void* input_struct){
  sMicrowave * local_struct = (sMicrowave *) input_struct;

  // Printing program effect
  Serial.print("Effect set to: ");
  Serial.println(local_struct->effect_of_heater);

  if (local_struct->door == 0 && local_struct->door == 0){
    Serial.println("Door is closed and light is off!");
  }

  // Initalizing variables for clock timer
  uint8_t prog_h = (local_struct->prog_length_s / 3600); 
	uint8_t prog_m = (local_struct->prog_length_s - (3600 * prog_h)) /60;
	uint8_t prog_s = (local_struct->prog_length_s - (3600 * prog_h) - (prog_m * 60));

  if (local_struct->prog_length_s >= 60) {                      // If the program is 1 min or longer display it as minutes
    Serial.print("Program length is set to: ");
    Serial.print(prog_h);
    Serial.print(":");
    Serial.print(prog_m);
    Serial.print(":");
    Serial.println(prog_s);
  }
  else {                                                        // If the program is less than 60 sec, display it as secnds
    Serial.print("Program length is set to: ");
    Serial.print(local_struct->prog_length_s);
    Serial.println(" seconds.");
  }

  while (local_struct->prog_length_s != 0) {
    // Time calculations within the while loop to represent countdown timer
    prog_h = (local_struct->prog_length_s / 3600); 
	  prog_m = (local_struct->prog_length_s - (3600 * prog_h)) /60;
	  prog_s = (local_struct->prog_length_s - (3600 * prog_h) - (prog_m * 60));
    local_struct->prog_length_s -= 1;

    Serial.print("Time remaining: ");
    Serial.print(prog_h);
    Serial.print(":");
    Serial.print(prog_m);
    Serial.print(":");
    Serial.println(prog_s);

    // Disc spin calculation
    Serial.print("Disc is at: ");
    Serial.println(local_struct->disc_spin);
    local_struct->disc_spin += 30;                              // += 30 for 30 degrees/s
    if (local_struct->disc_spin >= 360) {
      local_struct->disc_spin = 0;
    }
    if(local_struct->prog_length_s == 0){
      vTaskSuspend(); // CREATE TASK HANDLE
    }
    vTaskDelay(PRINT_DELAY);
  }
}

void loop() {}
