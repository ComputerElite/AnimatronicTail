#include "params.h"

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