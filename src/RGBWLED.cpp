#include "Arduino.h"
#include "RGBWLED.h"


int RGBWLED::RED[3] = {255, 0, 0};
int RGBWLED::GREEN[3] = {0, 255, 0};
int RGBWLED::BLUE[3] = {0, 0, 255};
int RGBWLED::YELLOW[3] = {255, 255, 0};
int RGBWLED::MAGENTA[3] = {255, 0, 255};
int RGBWLED::PURPLE[3] = {80, 0, 80};
int RGBWLED::CYAN[3] = {0, 255, 255};
int RGBWLED::WHITE[3] = {255, 255, 255};
int RGBWLED::BLACK[3] = {0, 0, 0}; // OFF


RGBWLED::RGBWLED(int _ledPins[][4], int _leds, LedType ledType, CommonLeg _commonLeg) {	
	leds = _leds;
	commonLeg = _commonLeg;
	RGBW = ((ledType == RGBW_LED) ? true : false);
	
	// Dynamically allocate memory for the data arrays, based on how many LEDs we have
	// Single dimention array example fadeData = (int*)malloc((leds + 1) * sizeof(int));
	ledMode = (char**)malloc(leds * sizeof(char*));
	ledPins = (int**)malloc(leds * sizeof(int*));
	fadeData = (int**)malloc((leds + 1) * sizeof(int*));
	stageDelay = (unsigned long**)malloc((leds + 1) * sizeof(unsigned long*));
	rainbowData = (int**)malloc((leds + 2) * sizeof(int*));
	
    for(int ledIndex = 0; ledIndex <= (leds + 1); ledIndex++) {
		if (ledIndex < leds) {
			ledMode[ledIndex] = (char*)malloc(15 * sizeof(char));
			ledPins[ledIndex] = (int*)malloc((RGBW ? 4 : 3) * sizeof(int));
		}
		if (ledIndex <= leds) {
			fadeData[ledIndex] = (int*)malloc(3 * sizeof(int));
			stageDelay[ledIndex] = (unsigned long*)malloc(2 * sizeof(unsigned long));
		}
		rainbowData[ledIndex] = (int*)malloc(4 * sizeof(int));
    }
	
	// Copy array data into the arrays & initialise setting default values
	for(int ledIndex = 0; ledIndex <= (leds + 1); ledIndex++) {
		if (ledIndex < leds) {
			for(int pinIndex = 0; pinIndex < (RGBW ? 4 : 3); pinIndex++) {
				ledPins[ledIndex][pinIndex] = _ledPins[ledIndex][pinIndex];
			}

			// Init pin modes
			#if !defined(ESP32)
				pinMode(ledPins[ledIndex][0], OUTPUT); // R
				pinMode(ledPins[ledIndex][1], OUTPUT); // G
				pinMode(ledPins[ledIndex][2], OUTPUT); // B
				if (RGBW) {
					pinMode(ledPins[ledIndex][3], OUTPUT); // W
				}
			#else
				if (((!RGBW) && (leds > 5)) || ((RGBW) && (leds > 4))) {
					#error Too many LEDs! ESP32 can only control a MAX of 16 LED pins, due to the amount of channels in the ledc library
				}
			
				// R
				ledcSetup(esp32LEDChannel, esp32Freq, esp32DutyResolution);
				ledcAttachPin(ledPins[ledIndex][0], esp32LEDChannel);
				ledPins[ledIndex][0] = esp32LEDChannel++;
			
				// G
				ledcSetup(esp32LEDChannel, esp32Freq, esp32DutyResolution);
				ledcAttachPin(ledPins[ledIndex][1], esp32LEDChannel);
				ledPins[ledIndex][1] = esp32LEDChannel++;
			
				// B
				ledcSetup(esp32LEDChannel, esp32Freq, esp32DutyResolution);
				ledcAttachPin(ledPins[ledIndex][2], esp32LEDChannel);
				ledPins[ledIndex][2] = esp32LEDChannel++;
			
				// W
				if (RGBW) {
					ledcSetup(esp32LEDChannel, esp32Freq, esp32DutyResolution);
					ledcAttachPin(ledPins[ledIndex][3], esp32LEDChannel); // W
					ledPins[ledIndex][3] = esp32LEDChannel++;
				}
			#endif
	
			// Set default values
			ledMode[ledIndex] = "";
		}
		
		// Set default values
		for(int dataIndex = 0; dataIndex < 4; dataIndex++) {
			if (ledIndex <= leds) {
				if (dataIndex <= 1) {
					stageDelay[ledIndex][dataIndex] = 0;
				}
				if (dataIndex <= 2) {
					fadeData[ledIndex][dataIndex] = 0;
				}
			}
			if (dataIndex != 3) {
				rainbowData[ledIndex][dataIndex] = 0;
			}
			else {
				rainbowData[ledIndex][dataIndex] = 1; 
			}
		}
	}
}




// Convert HEX value to RGB values
void RGBWLED::hexToRGB(int *rgb, String hexColor) {
	long long number = strtoll(&hexColor[1], NULL, 16);
	rgb[0] = number >> 16;
	rgb[1] = number >> 8 & 0xFF;
	rgb[2] = number & 0xFF;
}




void RGBWLED::solid(String hex, int brightness, int led) {
	int rgb[3] = {};
	hexToRGB(rgb, hex);
	
	if (led == 0) {
		for(int _led = 1; _led <= leds; _led++) {
			ledMode[(_led - 1)] = "solid";
			setColour(rgb, brightness, false, _led);
		}
	}
	else {
		ledMode[(led - 1)] = "solid";
		setColour(rgb, brightness, false, led);
	}
}

void RGBWLED::solid(int *rgb, int brightness, int led) {
	if (led == 0) {
		for(int _led = 1; _led <= leds; _led++) {
			ledMode[(_led - 1)] = "solid";
			setColour(rgb, brightness, false, _led);
		}
	}
	else {
		ledMode[(led - 1)] = "solid";
		setColour(rgb, brightness, false, led);
	}
}

// W
void RGBWLED::white(int brightness, int led) {
	int rgb[3] = {};
	
	if (led == 0) {
		for(int _led = 1; _led <= leds; _led++) {
			ledMode[(_led - 1)] = "white";
			setColour(rgb, brightness, true, _led);
		}
	}
	else {
		ledMode[(led - 1)] = "white";
		setColour(rgb, brightness, true, led);
	}
}




void RGBWLED::flash(String hex1, String hex2, int delay1, int delay2, int brightness, bool alternate, int led) {
	int rgb1[3] = {};
	int rgb2[3] = {};
	hexToRGB(rgb1, hex1);
	hexToRGB(rgb2, hex2);
	
	if ((alternate) || (led == 0)) {
		for(int _led = 1; _led <= leds; _led++) {
			if ((alternate) && (!(_led % 2))) {
				setFlash(rgb2, rgb1, delay1, delay2, brightness, true, false, _led);
			}
			else {
				setFlash(rgb1, rgb2, delay1, delay2, brightness, true, false, _led);
			}
		}
	}
	else {
		setFlash(rgb1, rgb2, delay1, delay2, brightness, false, false, led);
	}
}

void RGBWLED::flash(int *rgb1, int *rgb2, int delay1, int delay2, int brightness, bool alternate, int led) {
	if ((alternate) || (led == 0)) {
		for(int _led = 1; _led <= leds; _led++) {
			if ((alternate) && (!(_led % 2))) {
				setFlash(rgb2, rgb1, delay1, delay2, brightness, true, false, _led);
			}
			else {
				setFlash(rgb1, rgb2, delay1, delay2, brightness, true, false, _led);
			}
		}
	}
	else {
		setFlash(rgb1, rgb2, delay1, delay2, brightness, false, false, led);
	}
}

// W
void RGBWLED::flash(int delay1, int delay2, int brightness, bool alternate, int led) {
	int rgb1[3] = {};
	int rgb2[3] = {};
	
	if ((alternate) || (led == 0)) {
		for(int _led = 1; _led <= leds; _led++) {
			if ((alternate) && (!(_led % 2))) {
				setFlash(rgb2, rgb1, delay1, delay2, brightness, true, true, _led);
			}
			else {
				setFlash(rgb1, rgb2, delay1, delay2, brightness, true, true, _led);
			}
		}
	}
	else {
		setFlash(rgb1, rgb2, delay1, delay2, brightness, false, true, led);
	}
}	

void RGBWLED::setFlash(int *rgb1, int *rgb2, int delay1, int delay2, int brightness, bool all, bool _RGBW, int led) {
	stageDelay[(all ? 0 : (led))][0] = (((!stageDelay[(all ? 0 : (led))][0] || (stageDelay[(all ? 0 : (led))][1] < millis()) || (ledMode[(led - 1)] != "flash"))) ? (millis() + (unsigned long)delay1) : stageDelay[(all ? 0 : (led))][0]);
	ledMode[(led - 1)] = "flash";
	
	if (stageDelay[(all ? 0 : (led))][0] >= millis()) {
		setColour(rgb1, brightness, (_RGBW ? true : false), led);
		stageDelay[(all ? 0 : (led))][1] = (millis() + (unsigned long)delay2);
	}
	else if (stageDelay[(all ? 0 : (led))][1] >= millis()) {
		if (_RGBW) {
			setColour(rgb1, 0, true, led);
		}
		else {
			setColour(rgb2, brightness, false, led);
		}
	}
}




void RGBWLED::fade(String hex, int steps, int duration, int brightness, bool fadeOut, bool cycle, bool resetCycle, bool alternate, int led) {
	int rgb[3] = {};
	hexToRGB(rgb, hex);
	
	if (stageDelay[((alternate || (led == 0)) ? 0 : (led))][0] < millis()) {	
		if ((alternate) || (led == 0)) {
			for(int _led = 1; _led <= leds; _led++) {
				if ((alternate) && (!(_led % 2))) {
					setFade(rgb, steps, duration, brightness, (!fadeOut), cycle, resetCycle, true, false, _led);
				}
				else {
					setFade(rgb, steps, duration, brightness, fadeOut, cycle, resetCycle, true, false, _led);
				}
			}
		}
		else {
			setFade(rgb, steps, duration, brightness, fadeOut, cycle, resetCycle, false, false, led);
		}
	}
}

void RGBWLED::fade(int *rgb, int steps, int duration, int brightness, bool fadeOut, bool cycle, bool resetCycle, bool alternate, int led) {
	if (stageDelay[((alternate || (led == 0)) ? 0 : (led))][0] < millis()) {	
		if ((alternate) || (led == 0)) {
			for(int _led = 1; _led <= leds; _led++) {
				if ((alternate) && (!(_led % 2))) {
					setFade(rgb, steps, duration, brightness, (!fadeOut), cycle, resetCycle, true, false, _led);
				}
				else {
					setFade(rgb, steps, duration, brightness, fadeOut, cycle, resetCycle, true, false, _led);
				}
			}
		}
		else {
			setFade(rgb, steps, duration, brightness, fadeOut, cycle, resetCycle, false, false, led);
		}
	}
}

// W
void RGBWLED::fade(int steps, int duration, int brightness, bool fadeOut, bool cycle, bool resetCycle, bool alternate, int led) {
	int rgb[3] = {};
	
	if (stageDelay[((alternate || (led == 0)) ? 0 : (led))][0] < millis()) {	
		if ((alternate) || (led == 0)) {
			for(int _led = 1; _led <= leds; _led++) {
				if ((alternate) && (!(_led % 2))) {
					setFade(rgb, steps, duration, brightness, (!fadeOut), cycle, resetCycle, true, true, _led);
				}
				else {
					setFade(rgb, steps, duration, brightness, fadeOut, cycle, resetCycle, true, true, _led);
				}
			}
		}
		else {
			setFade(rgb, steps, duration, brightness, fadeOut, cycle, resetCycle, false, true, led);
		}
	}
}

void RGBWLED::setFade(int *rgb, int steps, int duration, int brightness, bool fadeOut, bool cycle, bool resetCycle, bool all, bool _RGBW, int led) {
	fadeData[(all ? 0 : (led))][1] = ((fadeData[(all ? 0 : (led))][2] != resetCycle) ? true : false);
	fadeData[(all ? 0 : (led))][2] = resetCycle;
	
    fadeData[(all ? 0 : (led))][0] = ((((all) && (led != leds)) || (!all)) ? (((fadeData[(all ? 0 : (led))][0] < 1000) && (ledMode[(led - 1)] == (fadeOut ? "fade-out" : "fade-in")) && (!fadeData[(all ? 0 : (led))][1])) ? (fadeData[(all ? 0 : (led))][0] + steps) : (((cycle) || (fadeData[(all ? 0 : (led))][1]) || (ledMode[(led - 1)] != (fadeOut ? "fade-out" : "fade-in"))) ? 0 : 1000)) : fadeData[(all ? 0 : (led))][0]);
	
	ledMode[(led - 1)] = (char*)(fadeOut ? "fade-out" : "fade-in");
	brightness = ((((fadeData[(all ? 0 : (led))][0] > 1000) ? 1000 : fadeData[(all ? 0 : (led))][0]) * brightness) / 1000); // Adjust to max brightness that we have set	
	
	setColour(rgb, (fadeOut ? (map(brightness, 0, 100, 100, -1)) : brightness), (_RGBW ? true : false), led); 
	
	stageDelay[(all ? 0 : (led))][0] = (millis() + (unsigned long)(duration / steps));
}




void RGBWLED::rainbow(int delay, int brightness, bool alternate, int led) {		
	if (stageDelay[led][0] < millis()) {
		stageDelay[led][0] = (millis() + (unsigned long)delay);
		
		if ((alternate) || (led == 0)) {
			for(int _led = 1; _led <= leds; _led++) {
				setRainbow(brightness, alternate, true, _led);
			}
		}
		else {
			setRainbow(brightness, alternate, false, led);
		}
	}
}

void RGBWLED::setRainbow(int brightness, bool alternate, bool all, int led) {
	if (((alternate) && (((all) && (ledMode[(led - 1)] != "all-rainbow-alt")) || ((!all) && (ledMode[(led - 1)] != "rainbow-alt")))) || ((!alternate) && (((all) && (ledMode[(led - 1)] != "all-rainbow")) || ((!all) && (ledMode[(led - 1)] != "rainbow"))))) { 
		if (!all) {
			rainbowData[(led + 1)][0] = rainbowData[(led + 1)][1] = rainbowData[(led + 1)][2] = 0;
			rainbowData[(led + 1)][3] = 1;
		}
		else {
			for (int index = 0; index <= 1; index++) {
				rainbowData[index][0] = rainbowData[index][1] = rainbowData[index][2] = 0;
				rainbowData[index][3] = ((index == 1) ? 7 : 0);
			}
		}
	} 
	ledMode[(led - 1)] = (char*)(alternate ? (all ? "all-rainbow-alt" : "rainbow-alt") : (all ? "all-rainbow" : "rainbow"));
	int *rgb = ((rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][3] == 7) ? RGBWLED::RED : ((rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][3] == 6) ? RGBWLED::GREEN : ((rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][3] == 5) ? RGBWLED::BLUE : ((rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][3] == 4) ? RGBWLED::YELLOW : ((rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][3] == 3) ? RGBWLED::MAGENTA : ((rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][3] == 2) ? RGBWLED::PURPLE : RGBWLED::CYAN))))));
		
	if (rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][0] != rgb[0] || rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][1] != rgb[1] || rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][2] != rgb[2]) {	
		rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][0] = ((rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][0] < rgb[0]) ? (rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][0] + 1) : ((rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][0] > rgb[0]) ? (rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][0] - 1) : rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][0]));
		rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][1] = ((rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][1] < rgb[1]) ? (rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][1] + 1) : ((rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][1] > rgb[1]) ? (rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][1] - 1) : rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][1]));
		rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][2] = ((rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][2] < rgb[2]) ? (rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][2] + 1) : ((rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][2] > rgb[2]) ? (rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][2] - 1) : rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][2]));

		setColour(rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))], brightness, false, led);
    }
    else {
        rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][3] = (rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][3] == 7 ? 1 : rainbowData[(((!(led % 2)) && alternate) ? 1 : (all ? 0 : (led + 1)))][3] + 1);
    }
}




void RGBWLED::off(int led) {
	int rgb[3] = {};
	
	// Turn all LEDs off
	if (led == 0) {
		for(int _led = 1; _led <= leds; _led++) {
			ledMode[(_led - 1)] = "off";
			if (RGBW) {
				setColour(rgb, 0, true, _led);
			}	
			setColour(rgb, 0, false, _led);
		}
	}
	// Turn specific LED off
	else {
		ledMode[(led - 1)] = "off";
		if (RGBW) {
			setColour(rgb, 0, true, led);
		}	
		setColour(rgb, 0, false, led);
	}
}




void RGBWLED::setColour(int *rgb, int brightness, bool _RGBW, int led) {
	int red = ((rgb[0] * brightness) / (100));
	int green = ((rgb[1] * brightness) / (100));
	int blue = ((rgb[2] * brightness) / (100));
	
	#if !defined(ESP32)     
		analogWrite(ledPins[(led-1)][0], ((commonLeg == COMMON_ANODE) ? (~((!_RGBW) ? (map(red, 0, 255, 0, _analogWriteResolution)) : 0)) : ((!_RGBW) ? (map(red, 0, 255, 0, _analogWriteResolution)) : 0)));
		analogWrite(ledPins[(led-1)][1], ((commonLeg == COMMON_ANODE) ? (~((!_RGBW) ? (map(green, 0, 255, 0, _analogWriteResolution)) : 0)) : ((!_RGBW) ? (map(green, 0, 255, 0, _analogWriteResolution)) : 0)));
		analogWrite(ledPins[(led-1)][2], ((commonLeg == COMMON_ANODE) ? (~((!_RGBW) ? (map(blue, 0, 255, 0, _analogWriteResolution)) : 0)) : ((!_RGBW) ? (map(blue, 0, 255, 0, _analogWriteResolution)) : 0)));
		// W
		if (RGBW) {
			analogWrite(ledPins[(led-1)][3], ((commonLeg == COMMON_ANODE) ? (~((_RGBW) ? (map(brightness, 0, 100, 0, _analogWriteResolution)) : 0)) : ((_RGBW) ? (map(brightness, 0, 100, 0, _analogWriteResolution)) : 0)));
		}
	#else
		ledcWrite(ledPins[(led-1)][0], ((commonLeg == COMMON_ANODE) ? (~((!_RGBW) ? (map(red, 0, 255, 0, _analogWriteResolution)) : 0)) : ((!_RGBW) ? (map(red, 0, 255, 0, _analogWriteResolution)) : 0)));
		ledcWrite(ledPins[(led-1)][1], ((commonLeg == COMMON_ANODE) ? (~((!_RGBW) ? (map(green, 0, 255, 0, _analogWriteResolution)) : 0)) : ((!_RGBW) ? (map(green, 0, 255, 0, _analogWriteResolution)) : 0)));
		ledcWrite(ledPins[(led-1)][2], ((commonLeg == COMMON_ANODE) ? (~((!_RGBW) ? (map(blue, 0, 255, 0, _analogWriteResolution)) : 0)) : ((!_RGBW) ? (map(blue, 0, 255, 0, _analogWriteResolution)) : 0)));
		// W
		if (RGBW) {
			ledcWrite(ledPins[(led-1)][3], ((commonLeg == COMMON_ANODE) ? (~((_RGBW) ? (map(brightness, 0, 100, 0, _analogWriteResolution)) : 0)) : ((_RGBW) ? (map(brightness, 0, 100, 0, _analogWriteResolution)) : 0)));
		}
	#endif
}