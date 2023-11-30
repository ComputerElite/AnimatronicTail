#include "params.h"

long hue = 0;
long lastDelta = 0;
long tmpStepRes = 0;
long animationStart = 0;
long lastAnimationStart = 0;
long timeSinceAnimationStart = 0;
long lastTimeSinceAnimationStart = 0;
LEDAnimation currentLEDAnimation = RAINBOW_FADE;

void SetLED(LEDAnimation animation) {
  Serial.println(animation);
  lastAnimationStart = animationStart;
  animationStart = millis();
  currentLEDAnimation = animation;
}

long GetStepForTime() {
  if(lastDelta == deltaTime) return tmpStepRes;
  tmpStepRes = deltaTime * 1000.0 * percentPerSecond;
  return tmpStepRes;
}

void RainbowFade() {
  long perPixel = 65536L / strip.numPixels();
  for(int i=0; i<strip.numPixels(); i++) { 
    int pixelHue = hue + (i * perPixel);
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
  }
  hue += static_cast<long>(GetStepForTime());
  strip.show();
}

void SetColor(uint32_t color) {
  for(int i=0; i<strip.numPixels(); i++) { 
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void Breathe(uint32_t color) {
  double brightness = sin(static_cast<double>(timeSinceAnimationStart) / 1000 * PI * 2) * 0.5 + 0.5;
  SetColor(color);
  strip.setBrightness(brightness * 255);
  strip.show();
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

void WiFiConnected() {
  double brightness = sin(static_cast<double>(timeSinceAnimationStart) / 1000 * PI * 2) * 0.5 + 0.5;
  long brightnessValue = brightness *255;
  SetColor(LerpColor(strip.Color(0, 229, 255), strip.Color(0, 255, 0), static_cast<double>(timeSinceAnimationStart) / 1000));
  if(brightnessValue < strip.getBrightness()) {
    // strip gets darker
    brightnessValue = strip.getBrightness();
  }
  strip.setBrightness(brightnessValue);
  strip.show();
}

void HandleLEDS() {
  lastTimeSinceAnimationStart = millis() - animationStart;
  timeSinceAnimationStart = millis() - animationStart;
  switch (currentLEDAnimation)
  {
  case RAINBOW_FADE:
    RainbowFade();
    break;
  case WIFI_SOFT_AP_OPEN:
    Breathe(strip.Color(255, 0, 0));
    break;
  case WIFI_CONNECTING:
    Breathe(strip.Color(0, 229, 255));
    break;
  case WIFI_CONNECTED:
    WiFiConnected();
    break;

  default:
    break;
  }
}