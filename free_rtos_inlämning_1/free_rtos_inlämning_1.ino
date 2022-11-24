// Including libraries
#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include "timers.h"
#include "semphr.h"

// Defining LED pins
#define RED_PIN     6
#define YELLOW_PIN  7
#define GREEN_PIN   8

// Creating struct for the microwave with it's members
typedef struct {
  uint32_t prog_length_s;       // Length of the program
  uint8_t door;                 // Door open or closed
  uint8_t lamp;                 // Lamp on or off
  uint16_t effect_of_heater;    // The effect of
  uint16_t disc_spin;           // Variable for the disc which we'll count in degrees
} sMicrowave;

// Creating task handle for the different programs
TaskHandle_t defrost_meat_Handle, defrost_veg_Handle, general_prog_Handle, menu_Handle, door_Handle;

#define PRINT_DELAY (pdMS_TO_TICKS(1000)) // Delay set to 1000ms(1sec)

// Here we've initilaized the different programs of the microwave from the struct
// First we add program time in seconds, door, lamp, effect and then disc
sMicrowave defrost_meat = {300, 0, 0, 800, 0};
sMicrowave defrost_veg = {60, 0, 0, 400, 0};
sMicrowave general_prog = {30, 0, 0, 800, 0};

bool bChoice = true, bDoor_status true;
uint8_t prog_choice;
uint8_t global_door_status = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);     // Red light represents heater
  pinMode(YELLOW_PIN, OUTPUT);  // Yellow light represents lamp on
  pinMode(GREEN_PIN, OUTPUT);   // Green light represents door open

  // Creating our first task for menu and chosing program
  xTaskCreate(main_door, "Main task to trigger the others", 128, global_door_status, 0, &door_Handle);
}

void main_door(void * param){
  uint8_t door_status = (uint8_t)param;
  uint8_t choice;

  while(bDoor_status == true){
      if (door_status == 0) {
      Serial.println("Door is closed");
      Serial.println("Do you want to open start heating food?");
      Serial.println("1. Yes");
      Serial.println("2. No");
      while (Serial.available() == 0) {} // Wait for User to Input Data
      choice = Serial.parseInt();
      switch (choice) {
        case 1:
          Serial.print("Door open");
          digitalWrite(GREEN_PIN, HIGH);
          vTaskDelay(PRINT_DELAY);
          Serial.println("Insert food");
          vTaskDelay(PRINT_DELAY);
          Serial.println("Closing door");
          digitalWrite(GREEN_PIN, LOW);
          vTaskDelay(PRINT_DELAY);
          Serial.println("Let's chose a program");
          xTaskCreate(choose_program, "Function for chosing program", 128, NULL, 1, &menu_Handle);
          bDoor_status = false;
          //vTaskSuspend(door_Handle);
          break;
        case 2:
          Serial.println("Okay");
          break;
        default:
          Serial.println("Invalid input");
          break;        
      } // Switch
    } // If
    else{Serial.println("The door is open");}
    prog_choice = Serial.read();
  } // While
} // Function

void choose_program(void * param){
  prog_choice = Serial.read();
  menu_choice();
  // Menu for input to chose program
  while(bChoice == true){
    vTaskDelay(PRINT_DELAY);
    while (Serial.available() == 0) {} // Wait for User to Input Data
    prog_choice = Serial.parseInt();
    switch(prog_choice){
      case 1:
        Serial.println("Defrost meat chosen");
        xTaskCreate(microwave_output, "Display microwave output", 128, &defrost_meat, 1, &defrost_meat_Handle);
        break;
      case 2:
        Serial.println("Defrost veg chosen");
        xTaskCreate(microwave_output, "Display microwave output", 128, &defrost_veg, 1, &defrost_veg_Handle);
        break;
      case 3:
        Serial.println("General prog chosen");
        xTaskCreate(microwave_output, "Display microwave output", 128, &general_prog, 1, &general_prog_Handle);
        break;
      default:
        Serial.println("Invalid input!");
        break;
    } // Switch
    prog_choice = Serial.read();
  } // While bChoice
} // Function

void microwave_output(void* input_struct){
  sMicrowave * local_struct = (sMicrowave *) input_struct;
  digitalWrite(RED_PIN, HIGH);        // Heater on
  digitalWrite(YELLOW_PIN, HIGH);     // Lamp on

  // Printing program effect
  Serial.print("Effect set to: ");
  Serial.println(local_struct->effect_of_heater);

  // Initalizing variables for clock timer
  uint8_t prog_h = (local_struct->prog_length_s / 3600); 
  uint8_t prog_m = (local_struct->prog_length_s - (3600 * prog_h)) /60;
  uint8_t prog_s = (local_struct->prog_length_s - (3600 * prog_h) - (prog_m * 60));

  if (local_struct->prog_length_s >= 60) {                      // If the program is 1 min or longer display it as minutes
    Serial.print("Program length is set to: ");
    Serial.print(prog_h); Serial.print(":");  Serial.print(prog_m); Serial.print(":");  Serial.println(prog_s);
  } // If
  else {                                                        // If the program is less than 60 sec, display it as secnds
    Serial.print("Program length is set to: ");
    Serial.print(local_struct->prog_length_s);
    Serial.println(" seconds.");
  } // Else

  while (local_struct->prog_length_s != 0) {
    // Time calculations within the while loop to represent countdown timer
    prog_h = (local_struct->prog_length_s / 3600); 
    prog_m = (local_struct->prog_length_s - (3600 * prog_h)) /60;
    prog_s = (local_struct->prog_length_s - (3600 * prog_h) - (prog_m * 60));
    local_struct->prog_length_s -= 1;

    // Print time remaining
    Serial.print("Time remaining: ");
    Serial.print(prog_h); Serial.print(":");  Serial.print(prog_m); Serial.print(":");
    if(prog_s < 10){
      Serial.print(0);
      Serial.println(prog_s);
    }
    else{
      Serial.println(prog_s);
    }

    // Disc spin calculation
    Serial.print("Disc is at: ");
    Serial.println(local_struct->disc_spin);
    local_struct->disc_spin += 30;                              // += 30 for 30 degrees/s
    if (local_struct->disc_spin >= 360) {
      local_struct->disc_spin = 0;
    } // If
    if(local_struct->prog_length_s == 0){                       // When prog_length reach 0 we exit the while loop
      Serial.println("PROGRAM DONE!");
      digitalWrite(RED_PIN, LOW);
      vTaskDelay(PRINT_DELAY);      
    } // If
    vTaskDelay(PRINT_DELAY);
  } // While
}// Function

void menu_choice(){
  Serial.println(" ");
  Serial.println("Choose program: ");
  Serial.println("1. Defrost meat");
  Serial.println("2. Defrost veggies");
  Serial.println("3. General 30s");
}

void loop() {}