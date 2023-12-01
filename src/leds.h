#include <Adafruit_NeoPixel.h>
#define N_LEDS 28
#define LED_PIN D2

enum LEDAnimation {
    RAINBOW_FADE = 0,
    WIFI_SOFT_AP_OPEN = 1,
    WIFI_CONNECTED = 2,
    WIFI_CONNECTING = 3,
    WIFI_CONNECTION_FAILED = 4,
    OFF = 5
};

extern void SetLED(LEDAnimation animation);
extern LEDAnimation GetLED();
extern void SetLEDSpeed(double speed);
extern double GetLEDSpeed();
extern void HandleLEDS();
extern void Begin();
extern void SetBrightness(int brightness);
extern int GetBrightness();
extern Adafruit_NeoPixel strip;