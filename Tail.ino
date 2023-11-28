
#define BUTTON_0_PIN 5
#define BUTTON_3_PIN 6
#define BUTTON_1_PIN 7
#define BUTTON_4_PIN 8
#define BUTTON_2_PIN 9
#define BUTTON_5_PIN 10
#define SERVOL_PIN 11
#define SERVOR_PIN 12
#define LED_PIN 3

#define VRX_PIN A2
#define VRY_PIN A1

#define TAIL_POS_0 0
#define TAIL_POS_LOW 20
#define TAIL_POS_MID 55
#define TAIL_POS_HIGH 70
#define N_LEDS 27
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

Servo left;
Servo right;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  left.attach(SERVOL_PIN);
  right.attach(SERVOR_PIN);
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(0);
}

double percentageL = 0;
double percentageR = 0;
double targetPercentageL = 20;
double targetPercentageR = 20;

bool lastButton0Pressed = false;
bool lastButton1Pressed = false;
bool lastButton2Pressed = false;
bool lastButton3Pressed = false;
bool lastButton4Pressed = false;
bool lastButton5Pressed = false;
long button0PressTime = 0;
long button1PressTime = 0;
long button2PressTime = 0;
long button3PressTime = 0;
long button4PressTime = 0;
long button5PressTime = 0;
long button0ReleaseTime = 0;
long button1ReleaseTime = 0;
long button2ReleaseTime = 0;
long button3ReleaseTime = 0;
long button4ReleaseTime = 0;
long button5ReleaseTime = 0;

int currentAnimation = 0;

int currentAnimationStep = 0;
bool animationStepComplete = false;

long lastLoop = 0;
double deltaTime = 0;
double percentPerSecond = 35;

bool oneShot = false;

double GetDeltaTime() {
  return static_cast<double>(millis() - lastLoop) / 1000.0;
}

void SetAnimation(int animation) {
  currentAnimation = animation;
  currentAnimationStep = 0;
  animationStepComplete = true;
}

void HandleAnimation() {
  if(animationStepComplete) {
    currentAnimationStep++;
    animationStepComplete = false;
  }

  switch (currentAnimation)
  {
  case 0:
    // Stop tail
    percentPerSecond = 30;
    targetPercentageL = 0;
    targetPercentageR = 0;
    break;


  case 1:
    percentPerSecond = 30;
    // go to default pos
    switch (currentAnimationStep)
    {
    case 0:
      targetPercentageL = TAIL_POS_LOW;
      targetPercentageR = TAIL_POS_LOW;  
      break;
    case 1:
      targetPercentageL = TAIL_POS_MID;
      targetPercentageR = TAIL_POS_LOW;  
      break;
    case 2:
      targetPercentageL = TAIL_POS_LOW;
      targetPercentageR = TAIL_POS_LOW;  
      break;
    case 3:
      targetPercentageL = TAIL_POS_LOW;
      targetPercentageR = TAIL_POS_MID;  
      break;
    default:
      if(oneShot) currentAnimation = 0;
      else {
        currentAnimationStep = 0;
        animationStepComplete = false;
        HandleAnimation();
      }
      break;
    }
    break;


  case 2:
    
    percentPerSecond = 30;
    switch (currentAnimationStep)
    {
    case 0:
      targetPercentageL = TAIL_POS_LOW;
      targetPercentageR = TAIL_POS_MID;  
      break;
    case 1:
      targetPercentageL = TAIL_POS_MID;
      targetPercentageR = TAIL_POS_LOW;  
      break;
    default:
      if(oneShot) currentAnimation = 0;
      else {
        currentAnimationStep = 0;
        animationStepComplete = false;
        HandleAnimation();
      }
      break;
    }
    break;
  case 3:
    
    percentPerSecond = 50;
    switch (currentAnimationStep)
    {
    case 0:
      targetPercentageL = TAIL_POS_MID;
      targetPercentageR = TAIL_POS_MID - 10;  
      break;
    case 1:
      targetPercentageL = TAIL_POS_MID - 10;
      targetPercentageR = TAIL_POS_MID;  
      break;
    default:
      if(oneShot) currentAnimation = 0;
      else {
        currentAnimationStep = 0;
        animationStepComplete = false;
        HandleAnimation();
      }
      break;
    }
    break;

  case 4:
    
    percentPerSecond = 20;
    switch (currentAnimationStep)
    {
    case 0:
      targetPercentageL = TAIL_POS_LOW;
      targetPercentageR = TAIL_POS_LOW;  
      break;
    case 1:
      targetPercentageL = TAIL_POS_MID;
      targetPercentageR = TAIL_POS_MID;  
      break;
    default:
      if(oneShot) currentAnimation = 0;
      else {
        currentAnimationStep = 0;
        animationStepComplete = false;
        HandleAnimation();
      }
      break;
    }
    break;
  case 5:
    percentPerSecond = 30;
    targetPercentageL = TAIL_POS_MID;
    targetPercentageR = TAIL_POS_MID;
    break;
  case 6:
    percentPerSecond = 30;
    targetPercentageL = TAIL_POS_HIGH;
    targetPercentageR = TAIL_POS_HIGH;
    break;
  default:
    currentAnimation = 0;
    break;
  }
}

void PrintDebug() {
  Serial.print("D: ");
  Serial.print(deltaTime);
  Serial.print(" L: ");
  Serial.print(map((long)(percentageL * 10), 0, 1000, 500, 2500));
  Serial.print(" R: ");
  Serial.print(map((long)(percentageR * 10), 0, 1000, 2500, 500));
  Serial.print(" T: ");
  Serial.print(targetPercentageL);
  Serial.print(" ");
  Serial.println(targetPercentageR);
}

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

void loop() {
  deltaTime = GetDeltaTime();
  lastLoop = millis();
  HandleButtons();
  HandleAnimation();
  //PrintDebug();
  HandleLEDS();

  if(targetPercentageL < percentageL) {
    percentageL -= percentPerSecond * deltaTime;
    if(percentageL < targetPercentageL) {
      percentageL = targetPercentageL;
    }
  } else if(targetPercentageL > percentageL) {
    percentageL += percentPerSecond * deltaTime;
    if(percentageL > targetPercentageL) {
      percentageL = targetPercentageL;
    }
  }

  if(targetPercentageR < percentageR) {
    percentageR -= percentPerSecond * deltaTime;
    if(percentageR < targetPercentageR) {
      percentageR = targetPercentageR;
    }
  } else if(targetPercentageR > percentageR) {
    percentageR += percentPerSecond * deltaTime;
    if(percentageR > targetPercentageR) {
      percentageR = targetPercentageR;
    }
  }

  if(percentageL == targetPercentageL && percentageR == targetPercentageR) {
    animationStepComplete = true;
  }
/*
  long x = analogRead(VRX_PIN);
  long y = analogRead(VRY_PIN);

  double xPercentage = map(x, 0, 1023, -100, 100);
  double xPercentageAbs = abs(xPercentage);
  double yPercentage = map(y, 0, 1023, 0, 100);

  

  percentageL = yPercentage * (1.0 - xPercentage / 100.0);
  percentageR = yPercentage * (1.0 - xPercentage / 100.0);

*/


  SetL(percentageL);
  SetR(percentageR);

  //delay(5);
}

void SetL(double percentage) {
  left.writeMicroseconds(map((long)(percentage * 10), 0, 1000, 500, 2500));
}
void SetR(double percentage) {
  right.writeMicroseconds(map((long)(percentage * 10), 0, 1000, 2500, 500));
}

void HandleButtons() {
  bool button0Pressed = digitalRead(BUTTON_0_PIN) == HIGH;
  if(button0Pressed != lastButton0Pressed && !button0Pressed) {
    // on release
    button0ReleaseTime = millis();
    if(millis() - button0PressTime < 200) {
      SetAnimation(0);
    } else if(millis() - button0PressTime < 1000) {
      SetAnimation(5);
    } else {
      SetAnimation(6);
    }
  }
  if(button0Pressed && button0PressTime > button0ReleaseTime) {
    // During press
  }
  if(button0Pressed != lastButton0Pressed && button0Pressed) {
    // on press
    button0PressTime = millis();
  }
  lastButton0Pressed = button0Pressed;



  bool button1down = digitalRead(BUTTON_1_PIN) == HIGH;
  bool button2down = digitalRead(BUTTON_2_PIN) == HIGH;
  bool button3down = digitalRead(BUTTON_3_PIN) == HIGH;
  bool button4down = digitalRead(BUTTON_4_PIN) == HIGH;
  bool button5down = digitalRead(BUTTON_5_PIN) == HIGH;

  if(button1down) {
    SetAnimation(1);
  }
  if(button2down) {
    SetAnimation(2);
  }
  if(button3down) {
    SetAnimation(3);
  }
  if(button4down) {
    SetAnimation(4);
  }
  if(button5down) {
    oneShot = true;
  }
}