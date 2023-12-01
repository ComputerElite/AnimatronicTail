#include <ESP8266WiFi.h>
#include "preferences.h"
#include "wifi.h"
#include "leds.h"


const char *setupSSID = "ComputerElite-Tail-Setup";
const char *setupPassword = "VerySecure";
const char *hostname = "computerelite-tail.local";
String ssid = "ComputerElite-Tail-AP";
String password = "VerySecure";
String wifiStatus = "Not connected";

String lastSSID = "";
String lastPassword = "";

bool isConnecting = false;

void BeginSetup() {
    wifiStatus = "Please connect to a wifi network to save power";
    WiFi.disconnect();
    Serial.println("SoftAP starting");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(setupSSID, setupPassword);
    SetLED(LEDAnimation::WIFI_SOFT_AP_OPEN);
}

bool isTryingToConnectToNewNetwork = false;

void SetSSIDAndPassword(String newSSID, String newPassword) {
    // Save last SSID to fall back to if connection fails
    // Assume wifi connection suceeded. If it didn't status will be updated
    wifiStatus = "WiFi connected!";
    lastSSID = ssid;
    lastPassword = password;
    ssid = newSSID;
    password = newPassword;
    isTryingToConnectToNewNetwork = true;
    SavePreferences();
    BeginWifi();
    SetLED(LEDAnimation::WIFI_CONNECTING);
}

void BeginWifi() {
    WiFi.setHostname(hostname);
    WiFi.setAutoConnect(false);
    WiFi.mode(WIFI_STA);
    if(ssid != "") {
        Serial.print("Connecting to ");
        Serial.println(ssid);
        Serial.print("With password ");
        Serial.println(password);
        isConnecting = true;
        WiFi.begin(ssid.c_str(), password == "" ? NULL : password.c_str()); // only connect if ssid is present. If password is empty, connect without password
    } else {
        BeginSetup();
    }
}

int attempt = 0;

void ConnectToLastNetworkIfApplicable() {
    if(isTryingToConnectToNewNetwork) {
        attempt++;
        if(attempt >= 3) {
            isTryingToConnectToNewNetwork = false;
            SetLED(LEDAnimation::WIFI_CONNECTION_FAILED);
            attempt = 0;
            ConnectToLastNetworkIfApplicable();
            return;
        }
        BeginWifi();
        return;
    }
    if(lastSSID != "") {
        Serial.println("Trying to reconnect to last network");
        ssid = lastSSID;
        password = lastPassword;
        SavePreferences(); // make sure faulty wifi isn't saved
        // reset password
        lastSSID = "";
        lastPassword = "";
        attempt = 0;
        BeginWifi();
    } else {
        attempt++;
        if(attempt >= 3) {
            attempt = 0;
            Serial.println("Max number of attempts reached. Starting SoftAP");
            BeginSetup();
            return;
        }
        Serial.println("Retrying connection");
        Serial.print("Attempt ");
        Serial.println(attempt);
        BeginWifi();
    }
}

void HandleWifi() {
    if(WiFi.status() == WL_CONNECTED) {
        if(isConnecting) {
            isConnecting = false;
            wifiStatus = "WiFi connected!";
            Serial.println("WiFi connected");
            attempt = 0;
            SetLED(LEDAnimation::WIFI_CONNECTED);
        }
        return;
    }
    if(WiFi.status() == WL_CONNECT_FAILED) {
        wifiStatus = "Connection failed";
        Serial.println("Connection failed");
        ConnectToLastNetworkIfApplicable();
        return;
    }
    if(WiFi.status() == WL_NO_SSID_AVAIL) {
        wifiStatus = "SSID not found";
        Serial.println("No SSID available");
        ConnectToLastNetworkIfApplicable();
        return;
    }
    if(WiFi.status() == WL_WRONG_PASSWORD) {
        wifiStatus = "Wrong password";
        Serial.println("Wrong password");
        attempt = 1000; // wrong password is wrong. No need to retry
        ConnectToLastNetworkIfApplicable();
        return;
    }
    if(WiFi.status() == WL_CONNECTION_LOST) {
        Serial.println("Connection lost, reconnecting");
        BeginWifi();
        return;
    }
}