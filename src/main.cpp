#define SERVOL_PIN D1
#define SERVOR_PIN D0
#define LED_PIN D2

#define VRX_PIN A2
#define VRY_PIN A1

#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "params.h"

Servo left;
Servo right;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

const char *ssid = "ComputerElite-Tail";
const char *password = "VerySecure";


// All extern ///
AsyncWebServer server(80);
double percentageL = 0;
double percentageR = 0;
double targetPercentageL = 20;
double targetPercentageR = 20;

int currentAnimation = 0;

int currentAnimationStep = 0;
bool animationStepComplete = false;

long lastLoop = 0;
double deltaTime = 0;
double percentPerSecond = 35;

bool oneShot = false;

int TAIL_POS_0 = 0;
int TAIL_POS_LOW = 20;
int TAIL_POS_MID = 55;
int TAIL_POS_HIGH = 70;
///


void SetL(double percentage) {
  left.writeMicroseconds(map((long)(percentage * 10), 0, 1000, 500, 2500));
}
void SetR(double percentage) {
  right.writeMicroseconds(map((long)(percentage * 10), 0, 1000, 2500, 500));
} 

void setup() {
  left.attach(SERVOL_PIN);
  right.attach(SERVOR_PIN);
  SetL(percentageL);
  SetR(percentageR);
  Serial.begin(115200);
  strip.begin();

  WiFi.softAP(ssid,password);
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  SetupServer();
  server.begin();
}

double GetDeltaTime() {
  return static_cast<double>(millis() - lastLoop) / 1000.0;
}




void PrintDebug() {
  Serial.print("D: ");
  Serial.print(deltaTime);
  Serial.print(" L: ");
  Serial.print(map((long)(percentageL * 10), 0, 1000, 500, 2500));
  Serial.print(" R: ");
  Serial.print(map((long)(percentageR * 10), 0, 1000, 2500, 500));
  Serial.print(" T: ");
  Serial.print(targetPercentageL);
  Serial.print(" ");
  Serial.println(targetPercentageR);
  Serial.print("[Server Connected] ");
  Serial.println(WiFi.softAPIP());
}


void loop() {
  deltaTime = GetDeltaTime();
  lastLoop = millis();
  HandleAnimation();
  PrintDebug();
  HandleLEDS();

  if(targetPercentageL < percentageL) {
    percentageL -= percentPerSecond * deltaTime;
    if(percentageL < targetPercentageL) {
      percentageL = targetPercentageL;
    }
  } else if(targetPercentageL > percentageL) {
    percentageL += percentPerSecond * deltaTime;
    if(percentageL > targetPercentageL) {
      percentageL = targetPercentageL;
    }
  }

  if(targetPercentageR < percentageR) {
    percentageR -= percentPerSecond * deltaTime;
    if(percentageR < targetPercentageR) {
      percentageR = targetPercentageR;
    }
  } else if(targetPercentageR > percentageR) {
    percentageR += percentPerSecond * deltaTime;
    if(percentageR > targetPercentageR) {
      percentageR = targetPercentageR;
    }
  }

  if(percentageL == targetPercentageL && percentageR == targetPercentageR) {
    animationStepComplete = true;
  }


  SetL(percentageL);
  SetR(percentageR);
}