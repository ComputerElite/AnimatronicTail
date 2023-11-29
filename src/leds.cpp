#include "params.h"

long hue = 0;
long lastDelta = 0;
long tmpStepRes = 0;
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

void HandleLEDS() {
  RainbowFade();
}