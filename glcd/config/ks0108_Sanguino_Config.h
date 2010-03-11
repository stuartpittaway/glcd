/*
 * glcd_Sanguino_Config.h - ATmega644 specific configuration for Arduino GLCD library
 *
 * vi:ts=4
 *
 * Use this file to set io pins and LCD panel parameters 
 *
 * The configuration below uses a single port for data 
 * and has the same wiring as the Sanguino config from the previous ks0108 library 
 *
*/

#ifndef	GLCD_PANEL_CONFIG_H
#define GLCD_PANEL_CONFIG_H

/*
 * define name for configuration
 */
#define glcd_ConfigName "Sanguino-ks0108"

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

// panel controller chips
#define CHIP_WIDTH     64  // pixels per chip 
#define CHIP_HEIGHT    64  // pixels per chip 

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/
/*
 * pins used for Commands
 */

#define glcdCSEL1			24 		// CS1 Bit   // swap pin assignments with CSEL2 if left/right image is reversed
#define glcdCSEL2			25 		// CS2 Bit
#define glcdRW				26 		// R/W Bit
#define glcdDI				27 		// D/I Bit 
#define glcdEN				28 		// EN Bit

/*
 * Data pin definitions
 * This version uses pins 0-7 for LCD Data 
 */
#define glcdData0Pin		0
#define glcdData1Pin		1
#define glcdData2Pin		2
#define glcdData3Pin		3
#define glcdData4Pin		4
#define glcdData5Pin		5
#define glcdData6Pin		6
#define glcdData7Pin		7


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
