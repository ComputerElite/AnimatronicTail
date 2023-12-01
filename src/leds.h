#include <FastLED.h>
#define N_LEDS 28
#define LED_PIN D2
extern CRGB leds[N_LEDS];

#define LED_ANIMATIONS \
X(ENUM_START, "Enum start", -1) \
X(RAINBOW_FADE, "Rainbow Fade", 1) \
X(RAINBOW_FRONT_BACK, "Rainbow Front to Back", 1) \
X(WIFI_SOFT_AP_OPEN, "Wifi Soft AP Open", 0) \
X(WIFI_CONNECTING, "Wifi Connecting", 0) \
X(WIFI_CONNECTION_FAILED, "Wifi Connection Failed", 0) \
X(WIFI_CONNECTED, "Wifi Connected", 0) \
X(OFF, "Off", 1) \
X(CHRISTMAS_1, "Candy cane",1) \
X(MOVING_LIGHT, "Moving light",1) \
X(STATIC_LIGHT, "Static",1) \
X(BREATHE_SLOW, "Breathe slow",1) \
X(BREATHE_MID, "Breathe mid",1) \
X(BREATHE_FAST, "Breathe fast",1) \
X(ENUM_END, "Enum end", -1)

#define X(a, name, group) a,
enum LEDAnimation
{
    LED_ANIMATIONS
};
#undef X

extern char const *led_animation_names[];

extern int const led_animation_groups[];

extern CRGB color0;


extern void SetLED(LEDAnimation animation);
extern LEDAnimation GetLED();
extern void SetLEDSpeed(double speed);
extern double GetLEDSpeed();
extern void HandleLEDS();
extern void Begin();
extern void SetBrightness(int brightness);
extern int GetBrightness();