#define VRX_PIN A2
#define VRY_PIN A1
#include <FastLED.h>
#include "leds.h"
#include "preferences.h"
#include "wifi.h"
#include "server.h"
#include "animations.h"
#include "main.h"
#include "bluetooth.h"

double deltaTimeSeconds = 0;
long deltaTime = 0;
long lastLoop = 0;


void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, LED_PIN>(ledsRGB, getRGBWsize(N_LEDS));
  SetLED(LEDAnimation::WIFI_CONNECTING);
  LoadPreferences();
  //if(ShouldResetPreferences()) ResetPreferences(false);
  BeginWifi();
  BeginServos();
  //Serial.print("Soft-AP IP address = ");
  //Serial.println(WiFi.softAPIP());

  SetupServer();
  //SetupBluetooth();
  RestartServer();
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
  deltaTime = millis() - lastLoop;
  deltaTimeSeconds = static_cast<double>(millis() - lastLoop) / 1000.0;
  lastLoop = millis();
  HandleAnimation();
  //PrintDebug();
  HandleLEDS();
  HandleWifi();
  HandleServos();
  //HandleBluetooth();
  delay(5);
}