#include <Arduino.h>

extern void BeginWifi();
extern void HandleWifi();
extern void SetSSIDAndPassword(String newSSID, String newPassword);

extern String ssid;
extern String password;
extern String wifiStatus;