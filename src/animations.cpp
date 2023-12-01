#include "animations.h"
#include "main.h"
#include <Servo.h>

double percentageL = 0;
double percentageR = 0;
double targetPercentageL = 20;
double targetPercentageR = 20;

int currentAnimation = 0;

int currentAnimationStep = 0;
bool animationStepComplete = false;

double percentPerSecond = 35;

bool oneShot = false;

int TAIL_POS_0 = 0;
int TAIL_POS_LOW = 20;
int TAIL_POS_MID = 55;
int TAIL_POS_HIGH = 70;

void SetAnimation(int animation) {
  currentAnimation = animation;
  currentAnimationStep = 0;
  animationStepComplete = true;
}

void SetSpeed(int speed) {
  percentPerSecond = speed;
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
    targetPercentageL = TAIL_POS_0;
    targetPercentageR = TAIL_POS_0;
    break;


  case 1:
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
    targetPercentageL = TAIL_POS_MID;
    targetPercentageR = TAIL_POS_MID;
    break;
  case 6:
    targetPercentageL = TAIL_POS_HIGH;
    targetPercentageR = TAIL_POS_HIGH;
    break;
  default:
    currentAnimation = 0;
    break;
  }
}

Servo left;
Servo right;

void SetL(double percentage) {
  left.writeMicroseconds(map((long)(percentage * 10), 0, 1000, 500, 2500));
}
void SetR(double percentage) {
  right.writeMicroseconds(map((long)(percentage * 10), 0, 1000, 2500, 500));
}
void BeginServos() {
  left.attach(SERVOL_PIN);
  right.attach(SERVOR_PIN);
  SetL(percentageL);
  SetR(percentageR);
}
void HandleServos() {
    if(targetPercentageL < percentageL) {
    percentageL -= percentPerSecond * deltaTimeSeconds;
    if(percentageL < targetPercentageL) {
      percentageL = targetPercentageL;
    }
  } else if(targetPercentageL > percentageL) {
    percentageL += percentPerSecond * deltaTimeSeconds;
    if(percentageL > targetPercentageL) {
      percentageL = targetPercentageL;
    }
  }

  if(targetPercentageR < percentageR) {
    percentageR -= percentPerSecond * deltaTimeSeconds;
    if(percentageR < targetPercentageR) {
      percentageR = targetPercentageR;
    }
  } else if(targetPercentageR > percentageR) {
    percentageR += percentPerSecond * deltaTimeSeconds;
    if(percentageR > targetPercentageR) {
      percentageR = targetPercentageR;
    }
  }

  if(percentageL == targetPercentageL && percentageR == targetPercentageR) {
    animationStepComplete = true;
  }


  SetL(percentageL);
  SetR(percentageR);
}