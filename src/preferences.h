extern void LoadPreferences();
extern void SavePreferences();
extern void SaveAnimation(int animation);
extern int LoadSavedAnimation();
extern void SaveLEDAnimation(int animation);
extern int LoadSavedLEDAnimation();
extern void SaveSpeed(int speed);
extern int LoadSavedSpeed();
extern void SaveLEDSpeed(double speed);
extern double LoadSavedLEDSpeed();
extern void SaveBrightness(int brightness);
extern int LoadSavedBrightness();

extern void SaveTimeRunning();
extern void GetTimeRunning();
extern bool ShouldResetPreferences();
extern void  ResetPreferences(bool alsoWifi);