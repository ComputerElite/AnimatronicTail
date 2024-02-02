#include <WiFi.h>
#include "preferences.h"
#include "wifi.h"
#include "leds.h"
#include "main.h"


const char *setupSSID = "ComputerElite-Tail-Setup";
const char *setupPassword = "VerySecure";
const char *hostname = "computerelite-tail.local";
String ssid = "ComputerElite-Tail-AP";
String password = "VerySecure";
String wifiStatus = "Not connected";

String lastSSID = "";
String lastPassword = "";

bool firstConnect = true;

long disconnectedTime = 0;
bool softAPStarted = false;

void BeginSetup() {
    wifiStatus = "Please connect to a wifi network to save power";
    WiFi.disconnect();
    Serial.println("SoftAP starting");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(setupSSID, setupPassword);
    softAPStarted = true;
    SetLED(LEDAnimation::WIFI_SOFT_AP_OPEN);
}

bool isTryingToConnectToNewNetwork = false;


void ConnectWifi() {
    softAPStarted = false;
    disconnectedTime = 0;
    if(ssid != "") {
        Serial.print("Connecting to ");
        Serial.println(ssid);
        Serial.print("With password ");
        Serial.println(password);
        WiFi.begin(ssid.c_str(), password == "" ? NULL : password.c_str()); // only connect if ssid is present. If password is empty, connect without password
    } else {
        BeginSetup();
    }
}

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
    SetLED(LEDAnimation::WIFI_CONNECTING);
    ConnectWifi();
}

void BeginWifi() {
    isTryingToConnectToNewNetwork = true;
    WiFi.setHostname(hostname);
    WiFi.setAutoConnect(false);
    WiFi.mode(WIFI_STA);
    firstConnect = true;
    if(ssid == setupSSID) {
        // Open AP if ssid is ap ssid
        BeginSetup();
        return;
    }
    ConnectWifi();
}

int attempt = 0;

void ConnectToLastNetworkIfApplicable() {
    if(isTryingToConnectToNewNetwork) {
        attempt++;
        if(attempt >= 3) {
            if(firstConnect) {
                firstConnect = false;
                BeginSetup();
                return;
            }
            isTryingToConnectToNewNetwork = false;
            SetLED(LEDAnimation::WIFI_CONNECTION_FAILED);
            attempt = 0;
            ConnectToLastNetworkIfApplicable();
            return;
        }
        ConnectWifi();
        return;
    }
    
    // Fallback to old network
    if(lastSSID != "") {
        Serial.println("Trying to reconnect to last network");
        ssid = lastSSID;
        password = lastPassword;
        SavePreferences(); // make sure faulty wifi isn't saved
        // reset password
        lastSSID = "";
        lastPassword = "";
        attempt = 0;
        ConnectWifi();
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
        ConnectWifi();
    }
}

void HandleWifi() {
    if(WiFi.status() == WL_CONNECTED) {
        disconnectedTime = 0;
        firstConnect = false;
        if(isTryingToConnectToNewNetwork) {
            isTryingToConnectToNewNetwork = false;
            wifiStatus = "WiFi connected!";
            Serial.println("WiFi connected");
            attempt = 0;
            SetLED(LEDAnimation::WIFI_CONNECTED);
        }
        return;
    }
    if(WiFi.status() == WL_CONNECT_FAILED) {
        disconnectedTime = 0;
        wifiStatus = "Connection failed. Check password and ssid";
        Serial.println("Connection failed");
        ConnectToLastNetworkIfApplicable();
        return;
    }
    if(WiFi.status() == WL_NO_SSID_AVAIL) {
        disconnectedTime = 0;
        wifiStatus = "SSID not found";
        Serial.println("No SSID available");
        ConnectToLastNetworkIfApplicable();
        return;
    }
    if(WiFi.status() == WL_CONNECTION_LOST) {
        disconnectedTime = 0;
        Serial.println("Connection lost, reconnecting");
        ConnectWifi();
        return;
    }
    if(WiFi.status() == WL_DISCONNECTED) {
        disconnectedTime += deltaTime;
        if(disconnectedTime > 20000 && !softAPStarted) {
            disconnectedTime = 0;
            wifiStatus = "Timeout";
            Serial.println("Disconnected timeout");
            ConnectToLastNetworkIfApplicable();
        }
        return;
    }
}