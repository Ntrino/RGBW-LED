I have modified this extensively, is it now non blocking, after getting rid of the dalay()'s for the flash/fade in/out functions!
Ive also changed/added & removed some functions,
Ive added the capability to pass in HEX colours, as well as RGB colour values, relevant functions have a brightness parameter
Added the ability to control a W (white) component, eg for a RGBW LED
Can create the rgbLEDPins array with as many LED's as i want, every second LED, would alternate for the rainbow split & flash functions!
RGB colours can either be passed in as a HEX string or an int array or indevidual RGB int values

See the All example for examples on the various functions & effects