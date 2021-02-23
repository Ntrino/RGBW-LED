// This library will work with Arduino/ESP8266/ESP32 Boards!

#include <RGBWLED.h>

// Set LED pins
// Add in as many LED pin arrays as we need - in the order of (R,G,B), or if a white component is used (R,G,B,W) include the W pin on the end as well
// If using ESP32, it has a limit of 16 pins which we can control, eg. 5x RGB LEDs in the array, or 4x RGBW LEDs (Due to the ledc library having only 16 controllable channels)
// Arduino & ESP8266 are only limited to the amount of compatible PWM pins

//int rgbwLEDPins[1][4] = {{21, 17, 16}};  // ESP32 Pins
//int rgbwLEDPins[1][4] = {{D3, D8, D7}};  // ESP8266 Pins - RGBW Example, int rgbwLEDPins[1][4] = {{D3, D8, D7, D9}};  

// If more than 1 LED, just add more LED pin sets into the pin array, make sure to change the size of the array to match how many LEDs
//int rgbwLEDPins[2][4] = {{21, 17, 16}, {18, 19, 23}}; // ESP32 Pins
int rgbwLEDPins[2][4] = {{D3, D8, D7}, {D2, D5, D6}};   // ESP8266 Pins - RGBW Example, int rgbwLEDPins[2][4] = {{D3, D8, D7, D9}, {D2, D5, D6, D1}};

String function; // For testing

// If an RGBW LED is used, then change RGB_LED to RGBW_LED, if LEDs use a common anode instead then change COMMON_CATHODE to COMMON_ANODE
RGBWLED LED(rgbwLEDPins, (sizeof(rgbwLEDPins) / sizeof(rgbwLEDPins[0])), RGB_LED, COMMON_CATHODE);      



void setup() {
    Serial.begin(115200); 
    Serial.print("\n\nDevice Booted!\n\nRGBW-LED Library Examples\n\n"); 
}


void loop() {
    /* 
    Uncomment examples as needed
    Library accepts many colour values as input
    To obtain RGB / HEX colour values, use - https://www.google.com/search?q=hex+colour+picker

    If any function has 'alternate' set, or if no 'LED' parameter is specified (Which if blank will set ALL LEDs)
    Then even if another function the same has a LED specified, the first 'alternate' / No LED parameter function will win and set all LEDs
    
    Parameters enclosed in {} can be optionally passed in, as the library has default values for them

    
    To set colours we can either use built in class colours variables
  
    RGBWLED::RED
    RGBWLED::GREEN
    RGBWLED::BLUE
    RGBWLED::YELLOW
    RGBWLED::MAGENTA
    RGBWLED::PURPLE
    RGBWLED::CYAN
    RGBWLED::WHITE
    RGBWLED::BLACK (OFF)
    */

    // Or can use any HEX colour String
    String colour1 = "#ff00ff"; // Magenta
    String colour2 = "#00ffff"; // Cyan
    
    // Or an array of RGB colour values
    int rgb1[3] = {255, 0, 255}; // Magenta
    int rgb2[3] = {0, 255, 255}; // Cyan




    // To set a Solid colour:
    
    // Parameters - (colour, brightness, {LED})
    // colour (HEX String or rgb array)
    // brightness (0 - 100)
    // LED (LED, 1, 2 etc, leave blank to change all LEDs, (default is 0 - 'ALL LEDs'))

    //LED.solid(RGBWLED::MAGENTA, 100);    // Sets ALL LEDs magenta
    //LED.solid(RGBWLED::MAGENTA, 100, 1); // Sets LED 1 magenta
    //LED.solid(RGBWLED::CYAN, 100, 2);    // Sets LED 2 cyan
    
    //LED.solid(colour1, 100);
    //LED.solid(rgb1, 100);


    // RGBW white Component, (If RGBW LED is used with the 'W' pin set):
    
    // Parameters - (brightness, {LED})

    //LED.white(100);    // Sets ALL LEDs white components ON
    //LED.white(100, 1); // Sets LED 1 white component ON, will turn OFF the LEDs RGB components




    // To set a Flashing effect, (LEDs can flash two different colours alternatively, or ON/OFF):
    
    // Parameters - (colour1, colour2, delay1, delay2, brightness, {alternate, LED}) 
    // colour 1 & 2 (HEX colour string or RGB colour value array, leaving a HEX colour blank "" or setting the RGB array to black (r=0, g=0, b=0) will turn the LED/s ON/OFF instead of alternating a colour
    // delay1 (Time LED stays ON with a particular colour)
    // delay2 (time LED stays ON with the alternative colour, or time it stays OFF)
    // brightness (0 - 100)
    // alternate (If 'true' every 2nd LED with flash out of sync, it overrides the LED number if set - ALL LEDs will flash alternatively, (default is 'false')) 
    // LED (LED, 1, 2 etc, leave blank to change ALL LEDs, (default is 0 - 'ALL LEDs'))
    
    //LED.flash(RGBWLED::MAGENTA, RGBWLED::CYAN, 500, 500, 100);           // Sets ALL LEDs to flash magenta & cyan
    //LED.flash(RGBWLED::MAGENTA, RGBWLED::CYAN, 500, 500, 100, true);     // Sets ALL LEDs to flash magenta & cyan out of sync
    //LED.flash(RGBWLED::MAGENTA, RGBWLED::CYAN, 500, 500, 100, false, 1); // Sets LED 1 to flash magenta & cyan
    //LED.flash(RGBWLED::CYAN, RGBWLED::MAGENTA, 500, 500, 100, false, 2); // Sets LED 2 to flash cyan & magenta
     
    //LED.flash(colour1, colour2, 500, 500, 100);
    //LED.flash(rgb1, rgb2, 500, 500, 100);


    // RGBW white Component, (If RGBW LED is used with the 'W' pin set):
    
    // Parameters - (delay1, delay2, brightness, {alternate, LED}) 

    //LED.flash(500, 500, 100);           // Sets ALL LEDs to flash white & OFF 
    //LED.flash(500, 500, 100, true);     // Sets ALL LEDs to flash white & OFF out of sync
    //LED.flash(500, 500, 100, false, 1); // Sets LED 1 to flash white & OFF



    
    // To set a Fading In/Out effect:
    
    // Parameters - (colour, steps, duration, brightness, {fadeOut, cycle, cycleReset, alternate, LED});
    // colour (HEX colour string or RGB colour value array
    // steps (The lower the steps the slower it fades)
    // duration (Fading duration, works together with steps)
    // brightness (0 - 100) 
    // fadeOut (Fade effect, fading out or in, if fadeOut is 'true', it will start fading from ON to OFF, (default is 'false'))
    // cycle (If true, the fade cycle will contineously fade in/out, or if false it will stop once the fading has reached either is maximum or minimum, leaving it either ON or OFF, (default is 'true'))
    // cycleReset (If true, then it will re-cycle the function if 'cycle' was set to false, eg. triggering the function to cycle only once again)
    // alternate (If 'true' every 2nd LED will fade in/out out of sync, it overrides the LED number if set - ALL LEDs will fade in/out alternatively, (default is 'false'))
    // LED (LED, 1, 2 etc, leave blank to change ALL LEDs, (default is 0 - 'ALL LEDs'))
    
    //LED.fade(RGBWLED::MAGENTA, 20, 1000, 100);                       // Sets ALL LEDs to fade in magenta contineously
    //LED.fade(RGBWLED::MAGENTA, 20, 1000, 100, true);                 // Sets ALL LEDs to fade out magenta contineously
    //LED.fade(RGBWLED::MAGENTA, 20, 1000, 100, false, false);         // Sets ALL LEDs to fade in magenta once
    //LED.fade(RGBWLED::MAGENTA, 20, 1000, 100, false, true, true);    // Sets ALL LEDs to fade in/out magenta alternatively once 
    //LED.fade(RGBWLED::CYAN, 20, 1000, 100, false, true, false);      // Sets ALL LEDs to fade in/out magenta alternatively once 
    
    //LED.fade(RGBWLED::MAGENTA, 20, 1000, 100, true, true, false, 1); // Sets LED 1 to fade in magenta once
    
    //LED.fade(colour1, 20, 1000, 100);                            
    //LED.fade(rgb1, 20, 1000);                               


    // RGBW white Component, (If RGBW LED is used with the 'W' pin set):
    
    // Parameters - (steps, duration, brightness, {fadeOut, cycle, cycleReset, alternate, LED});
    
    //LED.fade(5, 1000, 100);                         // Sets ALL LEDs to fade in white contineously
    //LED.fade(5, 1000, 100, true);                   // Sets ALL LEDs to fade out white contineously
    //LED.fade(5, 1000, 100, false, false);           // Sets ALL LEDs to fade in white once
    //LED.fade(5, 1000, 100, false, false, true);     // Sets ALL LEDs to fade in/out white alternatively once 
    //LED.fade(5, 1000, 100, false, false, false, 1); // Sets LED 1 to fade in white once




    // To set a Rainbow effect:
    
    // Parameters - (delay, brightness, {alternate, LED})
    // (delay (Speed of rainbow effect), 
    // brightness (0 - 100), 
    // alternate (If 'true' every 2nd LED with rainbow cycle out of sync, it overrides the LED number if set - ALL LEDs will cycle through alternatively, (default is 'false'))
    // LED (LED, 1, 2 etc, leave blank to change ALL LEDs, (default is 0 - 'ALL LEDs'))
    
    //LED.rainbow(10, 100);           // Sets ALL LEDs to raibow cycle
    //LED.rainbow(10, 100, true);     // Sets ALL LEDs to raibow cycle alternatively
    //LED.rainbow(10, 100, false, 2); // Sets LED 1 to raibow cycle  




    // To turn LEDs OFF:
    
    // Parameters - {LED}
    // LED (LED, 1, 2 etc, leave blank to change ALL LEDs, (default is 0 - 'ALL LEDs')) 
    
    //LED.off();  // Sets ALL LEDs to OFF
    //LED.off(1); // Sets LED 1 to OFF

    // NOTE: Always use the .off function to turn LEDs off, rather than setting LED.solid to black (eg. RGB values - 0,0,0), 
    // or setting the brightness to 0, they may turn the LED off but they wont reset some library values!





    // Serial testing
    if (Serial.available() > 0) {
        function = Serial.readStringUntil('\n');
    }
    
    if (function == "1") {
        LED.rainbow(10, 100, false, 1);    
    }
    else if (function == "2") {
        LED.rainbow(10, 100, false, 2);    
    }
    else if (function == "3") {
        LED.rainbow(10, 100, false);    
    }
    else if (function == "4") {
        LED.rainbow(10, 100, true);    
    }                                 
    else if (function == "5") {
        LED.fade(RGBWLED::MAGENTA, 20, 1000, 100, false, false, false, false);    
    }
    else if (function == "6") {
        LED.fade(RGBWLED::MAGENTA, 20, 1000, 100, false, false, true, false); 
    }
    else if (function == "7") {
        LED.fade(RGBWLED::MAGENTA, 20, 1000, 100, true, false, false, false);  
    }
    else if (function == "8") {
        LED.fade(RGBWLED::MAGENTA, 20, 1000, 100, true, false, true, false);    
    }
    else if (function == "9") {
        LED.fade(RGBWLED::MAGENTA, 30, 1000, 100, true, false, false, false);    
    }
    else if (function == "10") {
        LED.fade(RGBWLED::MAGENTA, 30, 1000, 100, true, true, false, false);    
    }
    else if (function == "11") {
        LED.fade(RGBWLED::MAGENTA, 30, 1000, 100, false, true, false, false);    
    }
    else if (function == "12") {
        LED.fade(RGBWLED::MAGENTA, 10, 1000, 100, false, true, false, true);    
    }
}
