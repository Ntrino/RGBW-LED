#ifndef RGBWLED_h
#define RGBWLED_h

#include "Arduino.h"

#if (defined ESP8266) || (defined ESP32)
	// ESP8266 & ESP32 Duty Resoluton
	#define _analogWriteResolution 1023			
#else
	// Arduino Duty Resolution
	#define _analogWriteResolution 255			
#endif


enum LedType { 
	RGB_LED, 
	RGBW_LED 
};

enum CommonLeg { 
	COMMON_ANODE, 
	COMMON_CATHODE 
};

class RGBWLED {

	public:
		RGBWLED(int _ledPins[][4], int _leds, LedType ledType, CommonLeg _commonLeg);

		void hexToRGB(int *rgb, String hexColor);
		
		void solid(String hex, int brightness, int led = 0);
		void solid(int *rgb, int brightness, int led = 0);
		
		void white(int brightness, int led = 0); // W
		
		void flash(String hex1, String hex2, int delay1, int delay2, int brightness, bool alternate = false, int led = 0);
		void flash(int *rgb1, int *rgb2, int delay1, int delay2, int brightness, bool alternate = false, int led = 0);
		void flash(int delay1, int delay2, int brightness, bool alternate = false, int led = 0); // W

		void fade(String hex, int steps, int duration, int brightness, bool fadeOut = false, bool cycle = true, bool resetCycle = false, bool alternate = false, int led = 0);
		void fade(int *rgb, int steps, int duration, int brightness, bool fadeOut = false, bool cycle = true, bool resetCycle = false, bool alternate = false, int led = 0);
		void fade(int steps, int duration, int brightness, bool fadeOut = false, bool cycle = true, bool resetCycle = false, bool alternate = false, int led = 0); // W
		
		void rainbow(int delay, int brightness, bool alternate = false, int led = 0);
		
		void off(int led = 0);

		static int RED[3];
		static int GREEN[3];
		static int BLUE[3];
		static int MAGENTA[3];
		static int PURPLE[3];
		static int CYAN[3];
		static int YELLOW[3];
		static int WHITE[3];
		static int BLACK[3]; // OFF
		
	private:
		int leds = 0;
		int commonLeg = 0;
		bool RGBW = false;

		char **ledMode;
		int **ledPins;
		int **fadeData;
		unsigned long **stageDelay;
		int **rainbowData;
		
		// ESP32 ledcWrite function settings
		int esp32Freq = 5000;			// 5Khz, ESP8266 & Arduino are 1khz i believe
		int esp32LEDChannel = 0; 		// 0 - 15 Channels
		int esp32DutyResolution = 10;  	// 10 Bit, same as ESP8266 (1023), PWM frequency of 5kHz can have a maximum duty resolution of 13 bit (Resolution 8, 10, 12, 15)
		
		void setFlash(int *rgb1, int *rgb2, int delay1, int delay2, int brightness, bool all, bool _RGBW, int led);
		void setFade(int *rgb, int steps, int duration, int brightness, bool fadeOut, bool cycle, bool resetCycle, bool all, bool _RGBW, int led);
		void setRainbow(int brightness, bool alternate, bool all, int led);
		void setColour(int *rgb, int brightness, bool _RGBW, int led);			
};

#endif