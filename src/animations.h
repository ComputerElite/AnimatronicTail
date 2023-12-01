
#define SERVOL_PIN D1
#define SERVOR_PIN D0

extern double percentageL;
extern double percentageR;
extern double targetPercentageL;
extern double targetPercentageR;

extern int currentAnimation;

extern int currentAnimationStep;
extern bool animationStepComplete;

extern double percentPerSecond;

extern bool oneShot;

extern int TAIL_POS_0;
extern int TAIL_POS_LOW;
extern int TAIL_POS_MID;
extern int TAIL_POS_HIGH;
extern void HandleAnimation();
extern void SetAnimation(int animation);
extern int GetAnimation();
extern void SetSpeed(int speed);
extern int GetSpeed();

extern void BeginServos();
extern void HandleServos();