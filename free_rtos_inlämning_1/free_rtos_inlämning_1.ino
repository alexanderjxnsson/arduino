// Including libraries
#include <Arduino_FreeRTOS.h>

// Defining LED pins
#define RED_PIN     6
#define YELLOW_PIN  7
#define GREEN_PIN   8

// Creating struct for the microwave with it's members
typedef struct {
  uint32_t prog_length_s;       // Length of the program
  uint16_t effect_of_heater;    // The effect of
} sMicrowave;

// Creating task handle for the different programs
TaskHandle_t defrost_meat_Handle, defrost_veg_Handle, general_prog_Handle, menu_Handle, heater_Handle, lamp_Handle, disc_Handle;

#define PRINT_DELAY (pdMS_TO_TICKS(1000)) // Delay set to 1000ms(1sec)
#define PROG_DELAY (pdMS_TO_TICKS(500)) // Delay set to 1000ms(1sec)

// Here we've initilaized the different programs of the microwave from the struct
// First we add program time in seconds, door, lamp, effect and then disc
sMicrowave defrost_meat = {300, 800};
sMicrowave defrost_veg = {60, 400};
sMicrowave general_prog = {30, 800};

bool bChoice = true, bHeater = false, bLamp = false, bDoor = false;
uint8_t prog_choice;

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);     // Red light represents heater
  pinMode(YELLOW_PIN, OUTPUT);  // Yellow light represents lamp on
  pinMode(GREEN_PIN, OUTPUT);   // Green light represents door open

  // Creating our first task for menu and chosing program
  xTaskCreate(door_and_program, "Function for chosing program", 128, NULL, 1, &menu_Handle);
  xTaskCreate(lamp, "Lamp task", 128, NULL, 1, &lamp_Handle);
  xTaskCreate(disc, "Disc task calculation and heater", 128, NULL, 1, &disc_Handle);
  //xTaskCreate(heater, "Heater task", 128, NULL, 1, &heater_Handle);
}

void door_and_program(void * param){
  prog_choice = Serial.read();
  menu_choice();
  // Menu for input to chose program
  while(1){
    while (Serial.available() == 0) {} // Wait for User to Input Data
    prog_choice = Serial.parseInt();
    switch(prog_choice){
      case 1:
        Serial.println("Defrost meat chosen");
        vTaskDelay(PRINT_DELAY);
        xTaskCreate(microwave_output, "Display microwave output", 128, &defrost_meat, 1, &defrost_meat_Handle);
        vTaskSuspend(NULL);
        break;
      case 2:
        Serial.println("Defrost veg chosen");
        vTaskDelay(PRINT_DELAY);
        xTaskCreate(microwave_output, "Display microwave output", 128, &defrost_veg, 1, &defrost_veg_Handle);
        vTaskSuspend(NULL);
        break;
      case 3:
        Serial.println("General prog chosen");
        vTaskDelay(PRINT_DELAY);
        xTaskCreate(microwave_output, "Display microwave output", 128, &general_prog, 1, &general_prog_Handle);
        vTaskSuspend(NULL);
        break;
      default:
        Serial.println("Invalid input!");
        vTaskDelay(PRINT_DELAY);
        break;
    } // Switch
    prog_choice = Serial.read();
  } // While
} // Function

void microwave_output(void* input_struct){
  sMicrowave * local_struct = (sMicrowave *) input_struct;
  // Printing program effect
  Serial.print("Effect set to: ");
  Serial.println(local_struct->effect_of_heater);
  bLamp = true;
  bHeater = true;

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
    else{Serial.println(prog_s);}

    if(local_struct->prog_length_s == 0){                       // When prog_length reach 0 we exit the while loop
      Serial.println("PROGRAM DONE!");
      bHeater = false;
      vTaskDelay(PRINT_DELAY);
      Serial.println("We open door");
      vTaskDelay(PRINT_DELAY);
      Serial.println("Take out the food");
      vTaskDelay(PRINT_DELAY);
      Serial.println("Close the door");
      bLamp = false;
      vTaskResume(menu_Handle);
    } // If
    vTaskDelay(PRINT_DELAY);
  } // While
}// Function

void heater(void * param){
  while (1) {
    if(bHeater == true){
      digitalWrite(RED_PIN, HIGH);
    }
    else {
      digitalWrite(RED_PIN, LOW);
    }
  }
}

void lamp(void * param){
  while (1) {
    if ((bDoor == true) || (bHeater == true)) {
    digitalWrite(YELLOW_PIN, HIGH);
    }
    else{
      digitalWrite(YELLOW_PIN, LOW);
    }
  } // While
} // Function

void disc(void * param){
  uint16_t disc_degree = 0;
   while(1){
    if ((bHeater == true) && (bLamp == true)) {
      // Disc spin calculation
      Serial.print("Disc is at: ");
      Serial.println(disc_degree);
      disc_degree += 30;                              // += 30 for 30 degrees/s
      if (disc_degree >= 360) {
        disc_degree = 0;
      } // If
    } // If
    vTaskDelay(PRINT_DELAY);
  } // While
} // Function

void menu_choice(){
  Serial.println(" ");
  Serial.println("Choose program: ");
  Serial.println("1. Defrost meat");
  Serial.println("2. Defrost veggies");
  Serial.println("3. General 30s");
}

void loop() {}