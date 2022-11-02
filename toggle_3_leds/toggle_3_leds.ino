/* #define RED 6
#define YELLOW 7
#define GREEN 8 */

const int RED = 6;
const int YELLOW = 7;
const int GREEN = 8;

void setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
}

void loop() {
  Serial.write("Red on\n");
  digitalWrite(RED, digitalRead(RED) ^ 1);
  Serial.write("Red off\n");

  delay(50);
  Serial.write("Yellow on\n");
  digitalWrite(YELLOW, digitalRead(YELLOW) ^ 1);
  Serial.write("Yellow off\n");

  delay(50);
  Serial.write("Green on\n");
  digitalWrite(GREEN, digitalRead(GREEN) ^ 1);
  Serial.write("Green off\n");
  delay(50);
}
