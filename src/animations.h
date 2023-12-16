
#define SERVOL_PIN D1
#define SERVOR_PIN D0


#define TAIL_ANIMATIONS \
X(ENUM_START_TAIL, "Enum start", 0, -1) \
X(REST_LOW, "Rest low", 30, 1) \
X(REST_MID, "Rest mid", 30, 1) \
X(REST_HIGH, "Rest high", 30, 1) \
X(V_WAG, "V Wag", 30, 2) \
X(WAG, "Wag", 30, 2) \
X(EXCITED_WAG, "Excited Wag", 30, 2) \
X(CIRCLE, "Circle-ish", 30, 3) \
X(UP_AND_DOWN, "Up and down", 20, 3) \
X(ENUM_END_TAIL, "Enum end", 0, -1)

#define X(a, name, speed, group) a,
enum TailAnimation
{
    TAIL_ANIMATIONS
};
#undef X

extern char const *tail_animation_names[];

extern int const tail_animation_groups[];
extern int const tail_animation_speeds[];



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