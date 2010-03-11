/*
 * glcd_Mega_Config.h - User specific configuration for Arduino GLCD library
 *
 * vi:ts=4
 *
 * Use this file to set io pins and LCD panel parameters 
 *
 * The configuration below uses a single port for data 
 * and has the same wiring as the mega config from the previous ks0108 library 
 *
*/

#ifndef	GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for configuration
 */
#define glcd_ConfigName "Mega-ks0108"

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

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/
/*
 * Pins used for Commands
 */
#define glcdCSEL1			33		// CS1 Bit  
#define glcdCSEL2			35		// CS2 Bit
#define glcdRW				35		// R/W Bit
#define glcdDI				36		// D/I Bit 
#define glcdEN				37		// EN Bit

/*
 * Data pin definitions
 * This version uses pins 22-29 for LCD Data 
 */
#define glcdData0Pin		22
#define glcdData1Pin		23
#define glcdData2Pin		24
#define glcdData3Pin		25
#define glcdData4Pin		26
#define glcdData5Pin		27
#define glcdData6Pin		28
#define glcdData7Pin		29

/*********************************************************/
/*  Chip Select pin assignments                          */
/*********************************************************/
#define CS_2Chips_2Pins    // default has two chips selected using two pins

/*
 * Two Chip panels using two select pins
 * you can swap the HIGH and LOW values if you need to reverse chip order
 */

#ifdef CS_2Chips_2Pins
#define glcd_CHIP0 glcdCSEL1,HIGH, glcdCSEL2,LOW
#define glcd_CHIP1 glcdCSEL1,LOW, glcdCSEL2,HIGH
#endif

// defines for panel specific timing 
/*
 * The following defines are for low level timing.
 * They have been derived from looking at many ks0108 data sheets.
 * Nearly all of the datasheets have the same low level timing.
 * The names used here were derived from the AGM1264F series graphic module.
 * See the data sheet for the exact timing and waveforms.
 * All defines below are in nanoseconds.
 */

#define GLCD_tDDR	320	/* Data Delay time (E high to valid read data) 		*/
#define GLCD_tAS	140	/* Address setup time (ctrl line changes to E high)	*/
#define GLCD_tDSW	200	/* Data setup time (data lines setup to dropping E)	*/
#define GLCD_tWH    450	/* E hi level width (minimum E hi pulse width)		*/
#define GLCD_tWL	450	/* E lo level width (minimum E lo pulse width)		*/


#include "device/ks0108_Device.h"
#endif //GLCD_PANEL_CONFIG_H
