#include "leds.h"
#include "main.h"
#include "animations.h"

double hue = 0;
long animationSetTime = 0;
double secondsSinceAnimationStart = 0;
double animationSpeed = 7;
LEDAnimation currentLEDAnimation = RAINBOW_FADE;
uint8_t brightnessValue = 255;

#define X(a, name, group) name,
char const *led_animation_names[] =
{
    LED_ANIMATIONS
};
#undef X

#define X(a, name, group) group,
int const led_animation_groups[] =
{
    LED_ANIMATIONS
};
#undef X

CRGB leds[N_LEDS];

void SetBrightness(int brightness) {
  brightnessValue = brightness;
}
int GetBrightness() {
  return brightnessValue;
}

// animation variabls
CRGB currentColor = 0;
CRGB color0 = 0;
CRGB color1 = 0;
double currentBreathsPerSecond = 0;
double breathsPerSecond0 = 0;
double breathsPerSecond1 = 0;
double breathsLerpFactor = 1;

double breathSecondCounter = 0;

void SetLEDSpeed(double speed) {
  animationSpeed = speed;
}
double GetLEDSpeed() {
  return animationSpeed;
}

LEDAnimation GetLED() {
  return currentLEDAnimation;
}

CRGB GetColorBrightness(CRGB color, uint8_t brightness) {
  //return color;

  // Scale each 8 bytes
  color.r = color.r * brightness / 255;
  color.g = color.g * brightness / 255;
  color.b = color.b * brightness / 255;
  return color;
}

CRGB LerpColor(CRGB color1, CRGB color2, double percentage) {
  if(percentage > 1) percentage = 1;
  return CRGB(color1.r + (color2.r - color1.r) * percentage,
              color1.g + (color2.g - color1.g) * percentage, 
              color1.b + (color2.b - color1.b) * percentage);
}

double Lerp(double a, double b, double percentage) {
  if(percentage > 1) percentage = 1;
  return a + (b - a) * percentage;
}


void SetLED(LEDAnimation animation) {
  Serial.println(animation);
  currentLEDAnimation = animation;
  animationSetTime = millis();
  if(animation == WIFI_CONNECTED) {
    color0 = currentColor;
    color1 = CRGB(0, 255, 0);
    breathsPerSecond0 = currentBreathsPerSecond;
    breathsPerSecond1 = 0.3;
    breathsLerpFactor = 1;
    return;
  }
  if(animation == WIFI_CONNECTING) {
    color0 = currentColor;
    color1 = CRGB(0, 229, 255);
    breathsPerSecond0 = currentBreathsPerSecond;
    breathsPerSecond1 = 1;
    breathsLerpFactor = 1;
    return;
  }
  if(animation == WIFI_SOFT_AP_OPEN) {
    color0 = currentColor;
    color1 = CRGB(255, 255, 0);
    breathsPerSecond0 = currentBreathsPerSecond;
    breathsPerSecond1 = 0.5;
    breathsLerpFactor = 1;
    return;
  }
  if(animation == WIFI_CONNECTION_FAILED) {
    color0 = currentColor;
    color1 = CRGB(255, 0, 0);
    breathsPerSecond0 = currentBreathsPerSecond;
    breathsPerSecond1 = 2;
    breathsLerpFactor = 0.5;
    return;
  }
}

double GetStepForTime() {
  return deltaTime / 1000.0 * animationSpeed;
}

void IncrementHue() {
  hue += GetStepForTime();
  if(hue >= 255) hue -= 255;
}

void SetPixelColor(int pixel, CRGB color) {
  color = GetColorBrightness(color, brightnessValue);
  leds[pixel] = color;
}
void SetPixelColor(int pixel, CRGB color, uint8_t brightness) {
  color = GetColorBrightness(color, brightness);
  color = GetColorBrightness(color, brightnessValue);
  leds[pixel] = color;
}

void RainbowFade() {
  double perPixel = 255.0 / N_LEDS;
  for(int i=0; i<N_LEDS; i++) { 
    long pixelHue = static_cast<long>(hue+ (i * perPixel)) % 255;
    SetPixelColor(i, CHSV(static_cast<uint8_t>(pixelHue), 255, 255));
  }
  Serial.println(GetStepForTime());
  IncrementHue();
  FastLED.show();
}

void SetAllPixelsNonShow(CRGB color) {
  for(int i=0; i<N_LEDS; i++) { 
    SetPixelColor(i, color);
  }
}

void SetAllPixelsNonShow(CRGB color, uint8_t brightness) {
  for(int i=0; i<N_LEDS; i++) { 
    SetPixelColor(i, GetColorBrightness(color, brightness));
  }
}

void SetColor(CRGB color) {
  SetAllPixelsNonShow(color);
  FastLED.show();
}

void Breathe(CRGB color) {
  breathSecondCounter += deltaTimeSeconds * currentBreathsPerSecond;
  double brightness = sin(breathSecondCounter * 2*PI) * 0.5 + 0.5;
  if(brightness < 0) brightness = 0;
  if(brightness > 1) brightness = 1;


  SetAllPixelsNonShow(color, static_cast<uint8_t>(brightness * 255.0));
  FastLED.show();
}



void LerpColor0ToColor1() {
  currentColor = LerpColor(color0, color1, secondsSinceAnimationStart);
}

void LerpBreathsPerSecond0ToBreathsPerSecond1() {
  currentBreathsPerSecond = Lerp(breathsPerSecond0, breathsPerSecond1, secondsSinceAnimationStart * breathsLerpFactor);
}

void Christmas1() {
  IncrementHue();

  for(int i=0; i<N_LEDS; i++) {
    bool isRed = (i + 1) % 8 < 4;
    if(static_cast<int>(hue) % 64 < 32) isRed = !isRed;
    SetPixelColor(i, isRed ? CRGB(255, 0, 0) : CRGB(255, 255, 255));
  }
  FastLED.show();
}

int currentMovingLightPixel = 0;
double deltaTimeSecondsMovingLight = 0;
int direction = 1;
double additionalPixelData[N_LEDS];
void MovingLight() {
  // Light bounces back and forth
  deltaTimeSecondsMovingLight += deltaTimeSeconds;
  if(deltaTimeSecondsMovingLight > 2 / animationSpeed) {
    deltaTimeSecondsMovingLight = 0;
    currentMovingLightPixel += 1 * direction;
    additionalPixelData[currentMovingLightPixel] = 1.0;
    if(currentMovingLightPixel >= N_LEDS) direction = -1;
    if(currentMovingLightPixel <= 0) direction = 1;
  }
  SetAllPixelsNonShow(CRGB(0, 0, 0));
  for(int i=0; i<N_LEDS; i++) {
    if(additionalPixelData[i] > 0) {
      additionalPixelData[i] -= animationSpeed * deltaTimeSeconds * 0.07;
      if(additionalPixelData[i] < 0) additionalPixelData[i] = 0;
      SetPixelColor(i, CHSV(static_cast<uint8_t>(hue + i * 5), 200, 255), static_cast<uint8_t>(additionalPixelData[i] * 255));
    }
  }
  FastLED.show();
}

void RainbowFrontBack() {
  // Light bounces back and forth
  IncrementHue();
  deltaTimeSecondsMovingLight += deltaTimeSeconds;
  if(deltaTimeSecondsMovingLight > 2 / animationSpeed) {
    deltaTimeSecondsMovingLight = 0;
    currentMovingLightPixel += 1;
    if(currentMovingLightPixel >= N_LEDS) currentMovingLightPixel = 0;
  }
  SetPixelColor(currentMovingLightPixel, CHSV(static_cast<uint8_t>(hue), 255, 255));

  FastLED.show();
}

void HandleLEDS() {
  secondsSinceAnimationStart = static_cast<double>(millis() - animationSetTime) / 1000.0;
  switch (currentLEDAnimation)
  {
  case RAINBOW_FADE:
    RainbowFade();
    break;
  case WIFI_SOFT_AP_OPEN:
    LerpColor0ToColor1();
    LerpBreathsPerSecond0ToBreathsPerSecond1();
    Breathe(currentColor);
    break;
  case WIFI_CONNECTING:
    LerpColor0ToColor1();
    LerpBreathsPerSecond0ToBreathsPerSecond1();
    Breathe(currentColor);
    break;
  case WIFI_CONNECTED:
    LerpColor0ToColor1();
    LerpBreathsPerSecond0ToBreathsPerSecond1();
    Breathe(currentColor);
    break;
  case OFF:
    SetColor(CRGB(0, 0, 0));
    break;
  case WIFI_CONNECTION_FAILED:
    LerpColor0ToColor1();
    LerpBreathsPerSecond0ToBreathsPerSecond1();
    Breathe(currentColor);
    break;
  case CHRISTMAS_1:
    Christmas1();
    break;
  case MOVING_LIGHT:
    MovingLight();
    break;
  case STATIC_LIGHT:
    SetColor(color0);
    break;
  case BREATHE_SLOW:
    currentBreathsPerSecond = 0.25;
    Breathe(color0);
    break;
  case BREATHE_MID:
    currentBreathsPerSecond = 0.5;
    Breathe(color0);
    break;
  case BREATHE_FAST:
    currentBreathsPerSecond = 2;
    Breathe(color0);
    break;
  case RAINBOW_FRONT_BACK:
    RainbowFrontBack();
    break;
  default:
    break;
  }
}