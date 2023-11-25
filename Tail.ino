#define SERVOL_PIN 9
#define SERVOR_PIN 10
#define BUTTON_0_PIN 3
#define BUTTON_1_PIN 4
#define BUTTON_2_PIN 5
#define BUTTON_3_PIN 6
#define BUTTON_4_PIN 7
#include <Servo.h>

Servo left;
Servo right;

void setup() {
  left.attach(SERVOL_PIN);
  right.attach(SERVOR_PIN);
  Serial.begin(9600);
}

int angleL = 1000;
int angleR = 1000;

bool button0down = false;
bool button1down = false;
bool button2down = false;
bool button3down = false;

void loop() {
  HandleButtons();
  if(button0down) angleL += 5;
  else if(button1down) angleL -= 5;
  if(button2down) angleR += 5;
  else if(button3down) angleR -= 5;
  angleR = constrain(angleR, 0, 100);
  angleL = constrain(angleL, 0, 100);
  Serial.print("R");
  Serial.println(angleR);
  Serial.print("L");
  Serial.println(angleL);
  SetL(angleL);
  SetR(angleR);
  delay(500);
}

void SetL(long percentage) {
  left.writeMicroseconds(map(percentage, 0, 100, 500, 2500));
}
void SetR(long percentage) {
  right.writeMicroseconds(map(percentage, 0, 100, 2500, 500));
}

void HandleButtons() {
  button0down = digitalRead(BUTTON_0_PIN) == HIGH;
  button1down = digitalRead(BUTTON_1_PIN) == HIGH;
  button2down = digitalRead(BUTTON_2_PIN) == HIGH;
  button3down = digitalRead(BUTTON_3_PIN) == HIGH;
}

void SendPulse(int pin, long width) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(width);
  digitalWrite(pin, LOW);
}