#include <Arduino_FreeRTOS.h>

#define RED 6
#define YELLOW 7
#define GREEN 8

const TickType_t _300ms = pdMS_TO_TICKS(300);

typedef int TaskProfiler;
TaskProfiler REDprofiler = 0;
TaskProfiler YELLOWprofiler = 0;
TaskProfiler GREENprofiler = 0;

int yellow_exec_cycle = 0, green_exec_cycle = 0;

TaskHandle_t red_Handle , yellow_Handle, green_Handle;

void setup() {
  Serial.begin(9600);

  xTaskCreate(red_master, "Toggle RED LED", 128, NULL, 1, &red_Handle);
  xTaskCreate(yellow_slave, "Toggle YELLOW LED", 128, NULL, 1, &yellow_Handle);
  xTaskCreate(green_slave, "Toggle GREEN LED", 128, NULL, 1, &green_Handle);
  void led_on_off(int GPIO);
}


void red_master(void *pvParameters){
  pinMode(RED, OUTPUT);

  while (1) {
    yellow_exec_cycle++;
    green_exec_cycle++;
    REDprofiler++;
    Serial.print("[RED P] = ");
    Serial.println(REDprofiler);
    if (yellow_exec_cycle == 500) {
      Serial.println("YELLOW SUSPENDED");
      vTaskSuspend(yellow_Handle);
    }
    else if (yellow_exec_cycle == 1000) {
      Serial.println("YELLOW RESUME");
      vTaskResume(yellow_Handle);
      yellow_exec_cycle = 0;
    }
    else if (green_exec_cycle == 300){
      Serial.println("GREEN SUSPENDED");
      vTaskSuspend(green_Handle);
    }
    else if (green_exec_cycle == 900){
      Serial.println("GREEN RESUME");
      vTaskResume(green_Handle);
      green_exec_cycle = 0;
    }
    led_on_off(RED);
    vTaskDelay(_300ms);
  }
}

 void yellow_slave(void *pvParameters){
  pinMode(YELLOW, OUTPUT);
  
  while(1){
    YELLOWprofiler++;
    Serial.print("[YELLOW P] = ");
    Serial.println(YELLOWprofiler);

    led_on_off(YELLOW);
    
    vTaskDelay(_300ms);
  }
}

void green_slave(void *pvParameters){
  pinMode(GREEN, OUTPUT);

  while (1) {
    GREENprofiler++;
    Serial.print("[GREEN P] = ");
    Serial.println(GREENprofiler);

    led_on_off(GREEN);
    vTaskDelay(_300ms);
  }
}

void led_on_off(int GPIO){
  digitalWrite(GPIO, HIGH);
  delay(100);
  digitalWrite(GPIO, LOW);
}

void loop() {
}