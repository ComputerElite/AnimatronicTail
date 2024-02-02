#include <Preferences.h>
#include "wifi.h"
#include "animations.h"
#include "leds.h"
Preferences prefs;

const char* ns = "Tail";

void SavePreferences() {
    prefs.putString("ssid", ssid);
    prefs.putString("password", password);
}

void ResetPreferences(bool alsoWifi = false) {
    prefs.remove("animation");
    prefs.remove("ledAnimation");
    prefs.remove("speed");
    prefs.remove("ledSpeed");
    prefs.remove("brightness");
    prefs.remove("timeRunning");
    if(alsoWifi) {

        prefs.remove("ssid");
        prefs.remove("password");
    }
}

void SaveAnimation(int animation) {
    prefs.putInt("animation", animation);
}
int LoadSavedAnimation() {
    return prefs.getInt("animation", 0);
}
void SaveLEDAnimation(int animation) {
    prefs.putInt("ledAnimation", animation);
}
int LoadSavedLEDAnimation() {
    return prefs.getInt("ledAnimation", 0);
}
void SaveSpeed(int speed) {
    prefs.putInt("speed", speed);
}
int LoadSavedSpeed() {
    return prefs.getInt("speed", 30);
}

void SaveLEDSpeed(double speed) {
    prefs.putDouble("ledSpeed", speed);
}
double LoadSavedLEDSpeed() {
    return prefs.getDouble("ledSpeed", 30);
}

void SaveBrightness(int brightness) {
    prefs.putInt("brightness", brightness);
}
int LoadSavedBrightness() {
    return prefs.getInt("brightness", 255);
}

void LoadPreferences() {
    prefs.begin(ns);
    ssid = prefs.getString("ssid", "ComputerElite-Tail-Setup");
    password = prefs.getString("password", "VerySecure");
    //currentAnimation = LoadSavedAnimation();
    SetSpeed(LoadSavedSpeed());
    SetLED(static_cast<LEDAnimation>(LoadSavedLEDAnimation()));
    SetLEDSpeed(LoadSavedLEDSpeed());
    brightnessValueInternal = LoadSavedBrightness();
}

long getCurrentTimeMillis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long)(tv.tv_sec) * 1000 + (long)(tv.tv_usec) / 1000;
}

void SaveTimeRunning() {
    prefs.putLong64("timeRunning", getCurrentTimeMillis());
}

long GetTimeRunning() {
    return prefs.getLong64("timeRunning");
}

bool ShouldResetPreferences() {
    if(getCurrentTimeMillis() - GetTimeRunning() >= 6000) return true;
    return false;
}