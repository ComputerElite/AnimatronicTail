#include "params.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <ArduinoJson.h>

void RestartServer() {
  server.end();
  server.begin();
}

void SetupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html);
  });
  server.on("/animation", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    SetAnimation(atoi((char*)data));
    request->send(200);
  });
  server.on("/led", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    SetLED(static_cast<LEDAnimation>(atoi((char*)data)));
    request->send(200);
  });
   server.on("/speed", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    SetSpeed(atoi((char*)data));
    request->send(200);
  });
  server.on("/positions", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    StaticJsonDocument<128> doc;
    deserializeJson(doc, data);
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
  server.on("/speed", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(percentPerSecond));
  });
  server.on("/speed", HTTP_POST, [](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if(len < 1) return request->send(400);
    SetSpeed(atoi((char*)data));
    request->send(200);
  });
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
    deserializeJson(doc, data);
    String newSSID = doc["ssid"].as<String>();
    String newPassword = doc["password"].as<String>();
    SetSSIDAndPassword(newSSID, newPassword);
    request->send(200);
  });
}