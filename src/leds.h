#include <FastLED.h>
#define N_LEDS 144
#define LED_PIN 16
//#define HALF
#define MAX_BRIGHTNESS 255
#define RGBW // uncomment for RGBW strip
struct CRGBW  {
	union {
		struct {
			union {
				uint8_t g;
				uint8_t green;
			};
			union {
				uint8_t r;
				uint8_t red;
			};
			union {
				uint8_t b;
				uint8_t blue;
			};
            #ifdef RGBW
			union {
				uint8_t w;
				uint8_t white;
			};
            #endif
			#ifdef HALF
			union {
				uint8_t f1;
			};
			union {
				uint8_t f2;
			};
			union {
				uint8_t f3;
			};
            #ifdef RGBW
			union {
				uint8_t f4;
			};
			#endif
			#endif
		};

		#ifdef HALF
			#ifdef RGBW
			uint8_t raw[8];
			#else
			uint8_t raw[6];
			#endif
		#else
			#ifdef RGBW
			uint8_t raw[4];
			#else
			uint8_t raw[3];
			#endif
		#endif
	};
	CRGBW(){}
	CRGBW(uint8_t rd, uint8_t grn, uint8_t blu){
		r = rd;
		g = grn;
		b = blu;
        #ifdef RGBW
		w = 0;
        #endif
	}

	CRGBW(uint8_t rd, uint8_t grn, uint8_t blu, uint8_t wht){
		r = rd;
		g = grn;
		b = blu;
        #ifdef RGBW
		w = wht;
        #endif
	}
	inline void operator = (const CRGB c) __attribute__((always_inline)){ 
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;

        #ifdef RGBW
		this->white = 0;
        #endif
	}
	inline void operator = (const CRGBW c) __attribute__((always_inline)){ 
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
        #ifdef RGBW
		this->white = c.w;
        #endif
	}
    inline void operator = (const int c) __attribute__((always_inline)){ 
		this->r = c >> 24 && 0xFF;
		this->g = c >> 16 && 0xFF;
		this->b = c >> 8 && 0xFF;
        #ifdef RGBW
		this->white = c && 0xFF;
        #endif
	}
};
inline uint16_t getRGBWsize(uint16_t nleds){
	uint16_t nbytes = nleds * 4;
	#ifdef HALF
		#ifdef RGBW
		if(nbytes % 3 > 0) return (nbytes / 3 + 1)*2;
		else return (nbytes / 3)*2;
		#else
		return nleds;
		#endif
	#else
		#ifdef RGBW
		if(nbytes % 3 > 0) return nbytes / 3 + 1;
		else return nbytes / 3;
		#else
		return nleds;
		#endif
	#endif
}

extern CRGBW leds[N_LEDS];
extern CRGB *ledsRGB;

#define LED_ANIMATIONS \
X(ENUM_START, "Enum start", -1) \
X(RAINBOW_FADE, "Rainbow Fade", 1) \
X(RAINBOW_FRONT_BACK, "Rainbow Front to Back", 1) \
X(WIFI_SOFT_AP_OPEN, "Wifi Soft AP Open", 0) \
X(WIFI_CONNECTING, "Wifi Connecting", 0) \
X(WIFI_CONNECTION_FAILED, "Wifi Connection Failed", 0) \
X(WIFI_CONNECTED, "Wifi Connected", 0) \
X(OFF, "Off", 1) \
X(CHRISTMAS_1, "Candy cane",1) \
X(MOVING_LIGHT, "Moving light",1) \
X(STATIC_LIGHT, "Static",1) \
X(BREATHE_SLOW, "Breathe slow",1) \
X(BREATHE_MID, "Breathe mid",1) \
X(BREATHE_FAST, "Breathe fast",1) \
X(ENUM_END, "Enum end", -1)

#define X(a, name, group) a,
enum LEDAnimation
{
    LED_ANIMATIONS
};
#undef X

extern char const *led_animation_names[];

extern int const led_animation_groups[];

extern CRGB color0;


extern void SetLED(LEDAnimation animation);
extern LEDAnimation GetLED();
extern uint8_t brightnessValueInternal;
extern void SetLEDSpeed(double speed);
extern double GetLEDSpeed();
extern void HandleLEDS();
extern void Begin();
extern void SetBrightness(int brightness);
extern int GetBrightness();