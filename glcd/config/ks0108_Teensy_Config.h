/*
 * glcd_Teensy_Config.h - User specific configuration for Arduino GLCD library
 *
 * Use this file to set io pins and LCD panel parameters
 * This version is for a standard ks0108 display
 * connected to a Teensy or Teensy++
 *
 * Note that the Teensy and Teensy++ use different pin numbers 
 * so make sure that your wiring matches the device you are using
 *
*/

#ifndef	GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H


/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

// panel controller chips
#define CHIP_WIDTH     64  // pixels per chip
#define CHIP_HEIGHT    64  // pixels per chip

// calculate number of chips & round up if width is not evenly divisible
#define glcd_CHIP_COUNT ((DISPLAY_WIDTH + CHIP_WIDTH - 1)  / CHIP_WIDTH)

/**********************************************************/
/*  Configuration for assigning LCD bits to Teensy++ Pins */
/**********************************************************/

/*
 * Pins can be assigned using arduino pin numbers 0-n
 * Pins can also be assigned using PIN_Pb 
 *		where P is port A-L and b is bit 0-7
 *		Example: port D pin 3 is PIN_D3
 *
 */

 
#if defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) // Teensy++

#define glcd_ConfigName "Teensy++ ks0108"   // define name for configuration
/*
 *                         Arduino pin    AVR pin
 */ 
#define glcdCSEL1               18      // PIN_E6 
#define glcdCSEL2               19      // PIN_E7
#define glcdRW                  8       // PIN_E0
#define glcdDI                  9       // PIN_E1
#define glcdEN                  7       // PIN_D7
//#define glcdRES                 5       // PIN_D5

#define glcdData0Pin            10      // PIN_C0
#define glcdData1Pin            11      // PIN_C1
#define glcdData2Pin            12      // PIN_C2
#define glcdData3Pin            13      // PIN_C3
#define glcdData4Pin            14      // PIN_C4
#define glcdData5Pin            15      // PIN_C5
#define glcdData6Pin            16      // PIN_C6
#define glcdData7Pin            17      // PIN_C7

#warning "KS0108 using pins for teensy++"

/*********************************************************/
/*  Configuration for assigning LCD bits to Teensy Pins  */
/*********************************************************/
#elif defined(__AVR_ATmega32U4__)   // Teensy 2.0

#define glcd_ConfigName "Teensy ks0108"   // define name for configuration
/*
 *                         Arduino pin    AVR pin
 */ 
#define glcdCSEL1               7        // PIN_D2	// normal connection for control signals
#define glcdCSEL2               8        // PIN_D3
#define glcdRW                  6        // PIN_D1
#define glcdDI                  5        // PIN_D0
#define glcdEN                  9        // PIN_C6
//#define glcdRES                 17        //PIN_F6	// Reset Bit 

#define glcdData0Pin            0        // PIN_B0
#define glcdData1Pin            1        // PIN_B1
#define glcdData2Pin            2        // PIN_B2
#define glcdData3Pin            3        // PIN_B3
#define glcdData4Pin            13       // PIN_B4
#define glcdData5Pin            14       // PIN_B5
#define glcdData6Pin            15       // PIN_B6
#define glcdData7Pin            4        // PIN_B7

#warning "KS0108 using pins for teensy 2.0"
#endif

/*********************************************************/
/*  Chip Select pin assignments                          */
/*********************************************************/
#define CS_2Chips_2Pins    // default has two chips selected using two pins
//#define CS_3Chips_2Pins  // 3 chips selected using two pins
//#define CS_3Chips_3Pins  // 3 chips selected using three pins

/*
 * Two Chip panels using two select pins
 */

#ifdef CS_2Chips_2Pins
#define glcd_CHIP0 glcdCSEL1,HIGH, glcdCSEL2,LOW
#define glcd_CHIP1 glcdCSEL1,LOW, glcdCSEL2,HIGH

/*
 * Three Chip panel using two select pins
 */
#elif defined CS_3Chips_2Pins
#define glcd_CHIP0 glcdCSEL1,LOW, glcdCSEL2,HIGH
#define glcd_CHIP1 glcdCSEL1,HIGH, glcdCSEL2,LOW
#define glcd_CHIP2 glcdCSEL1,LOW, glcdCSEL2,LOW

 /*
 * example of three Chip panel using three select pins
 * in this example, the chip is selected by setting a corresponding pin LOW
 *
 * Don't forget that glcdCSEL3 needs to defined (uncommented in the command
pin assignments)
 */
#elif defined CS_3Chips_3Pins
#define glcd_CHIP0 glcdCSEL1,HIGH, glcdCSEL2,LOW,glcdCSEL3,LOW
#define glcd_CHIP1 glcdCSEL1,LOW, glcdCSEL2,HIGH,glcdCSEL3,LOW
#define glcd_CHIP2 glcdCSEL1,LOW, glcdCSEL2,LOW,glcdCSEL3,HIGH
#endif

/*
 * Four Chip panel using four select pins
 *
 * Don't forget that glcdCSEL3 and glcdCSEL4 need to be defined
 */
#elif defined CS_4Chips_4Pins
#define glcd_CHIP0 glcdCSEL1,HIGH, glcdCSEL2,LOW, glcdCSEL3, LOW, glcdCSEL4, LOW
#define glcd_CHIP1 glcdCSEL1,LOW, glcdCSEL2,HIGH, glcdCSEL3, LOW, glcdCSEL4, LOW
#define glcd_CHIP2 glcdCSEL1,LOW, glcdCSEL2,LOW, glcdCSEL3, HIGH, glcdCSEL4, LOW
#define glcd_CHIP3 glcdCSEL1,LOW, glcdCSEL2,LOW, glcdCSEL3, LOW, glcdCSEL4, HIGH

/*
 * Four Chip panel using two select pins
 */
#elif defined CS_4Chips_2Pins
#define glcd_CHIP0 glcdCSEL1,LOW, glcdCSEL2,LOW
#define glcd_CHIP1 glcdCSEL1,HIGH, glcdCSEL2,LOW
#define glcd_CHIP2 glcdCSEL1,HIGH, glcdCSEL2,HIGH
#define glcd_CHIP3 glcdCSEL1,LOW, glcdCSEL2,HIGH

/*
 * The following defines are for panel specific low level timing.
 *
 * See your data sheet for the exact timing and waveforms.
 * All defines below are in nanoseconds.
 */

#define GLCD_tDDR	320	/* Data Delay time (E high to valid read data) 		*/
#define GLCD_tAS	140	/* Address setup time (ctrl line changes to E high)	*/
#define GLCD_tDSW	200	/* Data setup time (data lines setup to dropping E)	*/
#define GLCD_tWH	450	/* E hi level width (minimum E hi pulse width)		*/
#define GLCD_tWL	450	/* E lo level width (minimum E lo pulse width)		*/


#include "device/ks0108_Device.h"
#endif //GLCD_PANEL_CONFIG_H
