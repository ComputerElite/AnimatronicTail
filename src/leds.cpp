#include "leds.h"
#include "main.h"
#include "animations.h"

long hue = 0;
long animationSetTime = 0;
double secondsSinceAnimationStart = 0;
double animationSpeed = 1;
LEDAnimation currentLEDAnimation = RAINBOW_FADE;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
double brightnessMultiplier = 255;

void SetBrightness(int brightness) {
  brightnessMultiplier = brightness / 255.0;
}
int GetBrightness() {
  return static_cast<int>(brightnessMultiplier * 255.0);
}

// animation variabls
uint32_t currentColor = 0;
uint32_t color0 = 0;
uint32_t color1 = 0;
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

uint32_t GetColorBrightness(uint32_t color, double brightness) {
  uint8_t r, g, b;
  r = (color >> 16) & 0xFF;
  g = (color >> 8) & 0xFF;
  b = color & 0xFF;
  r *= brightness;
  g *= brightness;
  b *= brightness;
  return strip.Color(r, g, b);
}

uint32_t LerpColor(uint32_t color1, uint32_t color2, double percentage) {
  if(percentage > 1) percentage = 1;
  uint8_t r1, g1, b1, r2, g2, b2;
  r1 = (color1 >> 16) & 0xFF;
  g1 = (color1 >> 8) & 0xFF;
  b1 = color1 & 0xFF;
  r2 = (color2 >> 16) & 0xFF;
  g2 = (color2 >> 8) & 0xFF;
  b2 = color2 & 0xFF;
  uint8_t r = r1 + (r2 - r1) * percentage;
  uint8_t g = g1 + (g2 - g1) * percentage;
  uint8_t b = b1 + (b2 - b1) * percentage;
  return strip.Color(r, g, b);
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
    color1 = strip.Color(0, 255, 0);
    Serial.println(color1);
    breathsPerSecond0 = currentBreathsPerSecond;
    breathsPerSecond1 = 0.3;
    breathsLerpFactor = 1;
    return;
  }
  if(animation == WIFI_CONNECTING) {
    color0 = currentColor;
    color1 = strip.Color(0, 229, 255);
    breathsPerSecond0 = currentBreathsPerSecond;
    breathsPerSecond1 = 1;
    breathsLerpFactor = 1;
    return;
  }
  if(animation == WIFI_SOFT_AP_OPEN) {
    color0 = currentColor;
    color1 = strip.Color(255, 255, 0);
    breathsPerSecond0 = currentBreathsPerSecond;
    breathsPerSecond1 = 0.5;
    breathsLerpFactor = 1;
    return;
  }
  if(animation == WIFI_CONNECTION_FAILED) {
    color0 = currentColor;
    color1 = strip.Color(255, 0, 0);
    breathsPerSecond0 = currentBreathsPerSecond;
    breathsPerSecond1 = 2;
    breathsLerpFactor = 0.5;
    return;
  }
}

long GetStepForTime() {
  return static_cast<long>(deltaTime * animationSpeed);
}

void SetPixelColor(int pixel, uint32_t color) {
  strip.setPixelColor(pixel, GetColorBrightness(color, brightnessMultiplier));
}

void RainbowFade() {
  long perPixel = 65536L / strip.numPixels();
  for(int i=0; i<strip.numPixels(); i++) { 
    int pixelHue = hue + (i * perPixel);
    SetPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
  }
  hue += GetStepForTime() * 0.5;
  strip.show();
}

void SetAllPixelsNonShow(uint32_t color) {
  for(int i=0; i<strip.numPixels(); i++) { 
    SetPixelColor(i, color);
  }
}

void SetAllPixelsNonShow(uint32_t color, double brightness) {
  for(int i=0; i<strip.numPixels(); i++) { 
    SetPixelColor(i, GetColorBrightness(color, brightness));
  }
}

void SetColor(uint32_t color) {
  SetAllPixelsNonShow(color);
  strip.show();
}

void Breathe(uint32_t color) {
  breathSecondCounter += deltaTimeSeconds * currentBreathsPerSecond;
  double brightness = sin(breathSecondCounter * 2*PI) * 0.5 + 0.5;

  SetAllPixelsNonShow(currentColor, brightness);
  strip.show();
}



void LerpColor0ToColor1() {
  currentColor = LerpColor(color0, color1, secondsSinceAnimationStart);
}

void LerpBreathsPerSecond0ToBreathsPerSecond1() {
  currentBreathsPerSecond = Lerp(breathsPerSecond0, breathsPerSecond1, secondsSinceAnimationStart * breathsLerpFactor);
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
    SetColor(strip.Color(0, 0, 0));
    break;
  case WIFI_CONNECTION_FAILED:
    LerpColor0ToColor1();
    LerpBreathsPerSecond0ToBreathsPerSecond1();
    Breathe(currentColor);
    break;

  default:
    break;
  }
}