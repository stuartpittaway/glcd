/*
 * glcd_Teensy_Config.h - User specific configuration for Arduino GLCD library
 *
 * vi:ts=4
 *
 * Use this file to set io pins and LCD panel parameters
 * This version is for a standard ks0108 display
 * connected to a Teesny or Teesny++
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

// calculate number of chips & round up if width is not evenly divisable
#define glcd_CHIP_COUNT ((DISPLAY_WIDTH + CHIP_WIDTH - 1)  / CHIP_WIDTH)

/**********************************************************/
/*  Configuration for assigning LCD bits to Teensy++ Pins */
/**********************************************************/
 
#if defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) // Teensy++

#define glcd_ConfigName "Teensy++ ks0108"   // define name for configuration

#define glcdCSEL1               18 
#define glcdCSEL2               19
#define glcdRW                  8
#define glcdDI                  9
#define glcdEN                  7
//#define glcdRES		            5	// Reset Bit 

#define glcdData0Pin            10
#define glcdData1Pin            11
#define glcdData2Pin            12
#define glcdData3Pin            13
#define glcdData4Pin            14
#define glcdData5Pin            15
#define glcdData6Pin            16
#define glcdData7Pin            17

#warning "KS0108 using pins for teensy++"

/*********************************************************/
/*  Configuration for assigning LCD bits to Teensy Pins  */
/*********************************************************/
#elif defined(__AVR_ATmega32U4__)   // Teensy 2.0

#define glcd_ConfigName "Teensy ks0108"   // define name for configuration

#define glcdCSEL1               7	// normal connection for control signals
#define glcdCSEL2               8
#define glcdRW                  6
#define glcdDI                  5
#define glcdEN                  9
//#define glcdRES		            17	// Reset Bit 

#define glcdData0Pin            0
#define glcdData1Pin            1
#define glcdData2Pin            2
#define glcdData3Pin            3
#define glcdData4Pin            13
#define glcdData5Pin            14
#define glcdData6Pin            15
#define glcdData7Pin            4

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
