/*
 * ks0108_Arduino.h - User specific configuration for Arduino GLCD library
 *
 * Use this file to set io pins
 * This version is for a standard ks0108 display
 * connected using the default Arduino wiring
 *
*/

#ifndef GLCD_PIN_CONFIG_H
#define GLCD_PIN_CONFIG_H

/*
 * define name for pin configuration
 */
#define glcd_PinConfigName "ks0108-Arduino"

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/
/* Data pin definitions
 */

#ifndef GLCD_BITSHIFT_COMMS

#define glcdData0Pin        8
#define glcdData1Pin        9
#define glcdData2Pin        10
#define glcdData3Pin        11
#define glcdData4Pin        4
#define glcdData5Pin        5
#define glcdData6Pin        6
#define glcdData7Pin        7

/* Arduino pins used for Commands
 * default assignment uses the first five analog pins
 */

#if NBR_CHIP_SELECT_PINS > 2
#define glcdCSEL3         3   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#define glcdCSEL4         2   // fourth chip select if needed
#endif

#endif

#define glcdEN			 18
#define glcdRW           17
#define glcdDI           16
#define glcdCSEL2        15
#define glcdCSEL1        14


#define PULSE_WIDTH_USEC   1
#define POLL_DELAY_MSEC   1

//#define glcdclockPin 2	//pin 11 on HC595
//#define glcdlatchPin 3 //pin 12 on HC595
//#define glcdhc595outputenable 4	//pin 13  on HC595
//#define glcddataPin 5	//pin 14 on HC595
//#define glcdploadPin        19	// Connects to Parallel load pin the 165
//#define glcdclockEnablePin  6   // Connects to Clock Enable pin the 165


// Reset Bit  - uncomment the next line if reset is connected to an output pin
//#define glcdRES          19    // Reset Bit

#endif //GLCD_PIN_CONFIG_H
