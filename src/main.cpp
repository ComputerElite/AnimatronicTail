#define VRX_PIN A2
#define VRY_PIN A1

#include "animations.h"
#include "leds.h"
#include "preferences.h"
#include "wifi.h"
#include "server.h"
#include "animations.h"


void setup() {
  Serial.begin(9600);
  strip.begin();
  LoadPreferences();
  BeginWifi();
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  SetupServer();
  RestartServer();
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
}


void loop() {
  deltaTime = GetDeltaTime();
  lastLoop = millis();
  HandleAnimation();
  //PrintDebug();
  HandleLEDS();
  HandleWifi();

  HandleServos();
}