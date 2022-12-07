// Including libraries
#include <Arduino_FreeRTOS.h>

// Defining LED pins
#define RED_PIN     6
#define YELLOW_PIN  7

// Creating struct for the microwave with it's members
typedef struct {
  uint32_t prog_length_s;       // Length of the program
  uint16_t effect_of_heater;    // The effect of
} sMicrowave;

// Here we've initilaized the different programs of the microwave from the struct
// First we add program time in seconds and then effect
sMicrowave defrost_meat = {300, 800};
sMicrowave defrost_veg = {60, 400};
sMicrowave general_prog = {30, 800};

#define PRINT_DELAY (pdMS_TO_TICKS(1000)) // Delay set to 1000ms(1sec)
bool bLamp = true, bHeater = true, bDoor;
TaskHandle_t heater_Handle, lamp_Handle, disc_Handle, door_Handle;

// Change this variable to simulate door open or closed
// 1 = Door closed
// 0 = Door open
uint8_t door_open_or_close = 1;

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);     // Red light represents heater
  pinMode(YELLOW_PIN, OUTPUT);  // Yellow light represents lamp on

  // Creating our first task for menu and chosing program
  xTaskCreate(door, "Main task to trigger the others", 128, door_open_or_close, 1, &door_Handle);
  xTaskCreate(lamp, "Lamp task", 128, NULL, 1, &lamp_Handle);

  // Change struct to simulate different programs
  xTaskCreate(disc, "Disc task calculation", 128, &general_prog, 1, &disc_Handle);
  xTaskCreate(heater, "Heater task", 128, &general_prog, 1, &heater_Handle);
}

void door(void * param){
  uint8_t door_status = (uint8_t)param;
  while (1) {
    if (door_status == 1) { // Door closed
      bLamp = true;
      Serial.println("Door closed");
      vTaskDelay(PRINT_DELAY);
      vTaskSuspend(NULL);
    }
    else if(door_status == 0){ // Door open
      Serial.println("Door is open");
      vTaskSuspend(disc_Handle);
      vTaskSuspend(heater_Handle);
      vTaskDelay(PRINT_DELAY);
      bLamp = false;
    }
  }
}

void heater(void * input_struct){
  sMicrowave * local_struct = (sMicrowave *) input_struct;

  Serial.print("Heater is set to: ");
  Serial.println(local_struct->effect_of_heater);
  while (1) {
    if (bHeater == true ) {
      digitalWrite(RED_PIN, HIGH);
    }
    else{
      digitalWrite(RED_PIN, LOW);
      vTaskSuspend(NULL);
    }
  } // While
} // Function

void lamp(void * param){
  while (1) {
    if (bLamp == true || bHeater == true) {
    digitalWrite(YELLOW_PIN, HIGH);
    }
    else{
      digitalWrite(YELLOW_PIN, LOW);
      vTaskSuspend(NULL);
    }
  } // While
} // Function

void disc(void * input_struct){
  sMicrowave * local_struct = (sMicrowave *) input_struct;

  uint16_t disc_degree = 0;

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

   while(local_struct->prog_length_s){
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
      vTaskDelay(PRINT_DELAY);
      Serial.println("Take out the food");
      vTaskDelay(PRINT_DELAY);
      vTaskDelay(PRINT_DELAY);
      Serial.println("Close the door");
      bLamp = false;
      vTaskSuspend(NULL);
    }
    if (bHeater == true && bLamp == true) {
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

void loop() {}
