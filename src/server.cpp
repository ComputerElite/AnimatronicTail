#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "server.h"
#include "html.h"
#include "animations.h"
#include "wifi.h"
#include "leds.h"
AsyncWebServer server(80);

void RestartServer() {
  server.end();
  server.begin();
}

char* extractString(uint8_t *data, size_t len) {
  char* str = (char*)data;
  str[len] = '\0';
  return str;
}

int extractNumber(uint8_t *data, size_t len) {
  return atoi(extractString(data, len));
}

void SetupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.on("/joy.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "application/javascript", joyJs);
  });
  server.on("/wheel.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", wheelJs);
  });
  server.on("/wheel.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/style", wheelCss);
  });
  // get/set animation
  server.on("/animation", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(GetAnimation()));
  });
  server.on("/animation", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    SetAnimation(extractNumber(data, len));
    request->send(200);
  });
  // get led patterns
  server.on("/ledpatterns", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<2048> doc;
    for (int i = LEDAnimation::ENUM_START + 1; i < LEDAnimation::ENUM_END; ++i)
    {
      JsonObject pattern = doc.createNestedObject();
      pattern["id"] = i;
      pattern["name"] = led_animation_names[i];
      pattern["group"] = led_animation_groups[i];
    }
    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
  });
  server.on("/patterns", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<2048> doc;
    for (int i = TailAnimation::ENUM_START_TAIL + 1; i < TailAnimation::ENUM_END_TAIL; ++i)
    {
      JsonObject pattern = doc.createNestedObject();
      pattern["id"] = i;
      pattern["name"] = tail_animation_names[i];
      pattern["group"] = tail_animation_groups[i];
      pattern["speed"] = tail_animation_speeds[i];
    }
    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
  });
  server.on("/goto", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    
    StaticJsonDocument<128> doc;
    deserializeJson(doc, data, len);
    currentAnimation = 0;
    if(!doc["brake"]) {
      percentageL = doc["l"];
      percentageR = doc["r"];
    }
    targetPercentageL = doc["l"];
    targetPercentageR = doc["r"];
    request->send(200, "text/plain", "Moved to position");
  });
  // get/set led animation
  server.on("/color0", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String());
  });
  server.on("/color0", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    long num = strtol(extractString(data, len), NULL, 16);
    color0 = CRGB(num >> 16, num >> 8, num);
    request->send(200);
  });
  // get/set led animation
  server.on("/led", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(GetLED()));
  });
  server.on("/led", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    SetLED(static_cast<LEDAnimation>(extractNumber(data, len)));
    request->send(200);
  });
  // get/set speed
  server.on("/speed", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(GetSpeed()));
  });
   server.on("/speed", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    SetSpeed(extractNumber(data, len));
    request->send(200);
  });
  // get/set led speed
  server.on("/ledspeed", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(GetLEDSpeed()));
  });
  server.on("/ledspeed", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    SetLEDSpeed(extractNumber(data, len));
    request->send(200);
  });
  // get/set brightness
  server.on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(GetBrightness() * 255));
  });
  server.on("/brightness", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    SetBrightness(extractNumber(data, len));
    request->send(200);
  });
  // get/set tail positions
  server.on("/positions", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    StaticJsonDocument<128> doc;
    deserializeJson(doc, data, len);
    TAIL_POS_0 = doc["0"];
    TAIL_POS_LOW = doc["low"];
    TAIL_POS_MID = doc["mid"];
    TAIL_POS_HIGH = doc["high"];

    request->send(200);
  });
  server.on("/positions", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<128> doc;
    doc["0"] = TAIL_POS_0;
    doc["low"] = TAIL_POS_LOW;
    doc["mid"] = TAIL_POS_MID;
    doc["high"] = TAIL_POS_HIGH;
    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
  });
  // get/set wifi
  server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<256> doc;
    doc["ssid"] = ssid;
    doc["password"] = password;
    doc["status"] = wifiStatus;
    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
  });
  server.on("/wifi", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    StaticJsonDocument<256> doc;
    deserializeJson(doc, data, len);
    String newSSID = doc["ssid"].as<String>();
    String newPassword = doc["password"].as<String>();
    SetSSIDAndPassword(newSSID, newPassword);
    request->send(200);
  });
}