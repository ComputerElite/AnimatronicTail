
#define N_LEDS 28

#include <Adafruit_NeoPixel.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>

enum LEDAnimation {
    RAINBOW_FADE = 0,
    WIFI_SOFT_AP_OPEN = 1,
    WIFI_CONNECTED = 2,
    WIFI_CONNECTING = 3,
    WIFI_CONNECTION_FAILED = 4,
};

extern Adafruit_NeoPixel strip;
extern AsyncWebServer server;

extern const char index_html[] PROGMEM;

extern double percentageL;
extern double percentageR;
extern double targetPercentageL;
extern double targetPercentageR;

extern int currentAnimation;

extern int currentAnimationStep;
extern bool animationStepComplete;

extern long lastLoop;
extern double deltaTime;
extern double percentPerSecond;

extern bool oneShot;

extern int TAIL_POS_0;
extern int TAIL_POS_LOW;
extern int TAIL_POS_MID;
extern int TAIL_POS_HIGH;

extern String ssid;
extern String password;
extern String wifiStatus;

extern void SetAnimation(int animation);
extern void SetSpeed(int speed);
extern void SetupServer();
extern void RestartServer();
extern void SetLED(LEDAnimation animation);
extern void HandleLEDS();
extern void HandleAnimation();
extern void BeginWifi();
extern void HandleWifi();
extern void SetSSIDAndPassword(String newSSID, String newPassword);

extern void LoadPreferences();
extern void SavePreferences();