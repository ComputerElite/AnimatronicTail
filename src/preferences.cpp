#include <Preferences.h>
#include "wifi.h"
Preferences prefs;

const char* ns = "Tail";

void LoadPreferences() {
    prefs.begin(ns);
    ssid = prefs.getString("ssid", "ComputerElite-Tail-Setup");
    password = prefs.getString("password", "VerySecure");
}

void SavePreferences() {
    prefs.putString("ssid", ssid);
    prefs.putString("password", password);
}