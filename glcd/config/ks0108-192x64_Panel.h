/*
 * ks0108-192x64_Panel.h - User specific configuration for Arduino GLCD library
 *
 * Use this file to set LCD panel parameters
 * This version is for a standard ks0108 display
 * connected using the default Arduino wiring
 *
*/

#ifndef	GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for panel configuration
 */
#define glcd_PanelConfigName "ks0108-192x64"

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 192
#define DISPLAY_HEIGHT 64

// panel controller chips
#define CHIP_WIDTH     64  // pixels per chip
#define CHIP_HEIGHT    64  // pixels per chip

/*********************************************************/
/*  Chip Select pin assignments                          */
/*********************************************************/
//#define CS_2Chips_2Pins    // default has two chips selected using two pins
//#define CS_2Chips_1Pin   // 2 chips selected using one CS pin (the other CS is inverted in hardware)
#define CS_3Chips_2Pins  // 3 chips selected using two CS pins
//#define CS_3Chips_3Pins  // 3 chips selected using three CS pins
//#define CS_4Chips_2Pins  // 4 chips selected using two CS pins
//#define CS_4Chips_4Pins  // 4 chips selected using 4 CS pins

/*
 * Two Chip panels using two select pins
 */

#ifdef CS_2Chips_2Pins
#define glcd_CHIP0 glcdCSEL1,HIGH, glcdCSEL2,LOW
#define glcd_CHIP1 glcdCSEL1,LOW, glcdCSEL2,HIGH

/*
 * Two Chip panels using one select pin
 */
 #elif defined  CS_2Chips_1Pin
#define glcd_CHIP0 glcdCSEL1,LOW
#define glcd_CHIP1 glcdCSEL1,HIGH

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
 * Don't forget that glcdCSEL3 needs to defined (uncommented in the command pin assignments)
 */
#elif defined CS_3Chips_3Pins
#define glcd_CHIP0 glcdCSEL1,HIGH, glcdCSEL2,LOW,glcdCSEL3,LOW
#define glcd_CHIP1 glcdCSEL1,LOW, glcdCSEL2,HIGH,glcdCSEL3,LOW
#define glcd_CHIP2 glcdCSEL1,LOW, glcdCSEL2,LOW,glcdCSEL3,HIGH

/*
 * Four Chip panel using four select pins
 *
 * Don't forget that glcdCSEL3 and glcdCSEL4 need to defined
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


 /*
  * The code below selects a configuration file based on the board selected in the IDE 
  * These configurations are compatable with wiring used in earlier versions of the library
  * 
  */
 
#if defined(__AVR_ATmega1280__)
#include "config/ks0108_Mega.h"      // config for Mega or other 1280 board
#elif defined(__AVR_ATmega644P__)  || defined(__AVR_ATmega644__)           
#include "config/ks0108_Sanguino.h"  // config for Sanguino or other ATmega644/p board
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) || defined(__AVR_ATmega32U4__)// Teensy
#include "config/ks0108_Teensy.h"    // config for Teensy and Teensy++  
#else
#include "config/ks0108_Arduino.h"   // config file for standard Arduino using documented wiring 
#endif

// calculate number of chips & round up if width is not evenly divisable
#define glcd_CHIP_COUNT ((DISPLAY_WIDTH + CHIP_WIDTH - 1)  / CHIP_WIDTH)

#include "device/ks0108_Device.h"
#endif //GLCD_PANEL_CONFIG_H
