/*
 * glcd_Config.h - User specific configuration for Arduino GLCD library
 * Copyright (c)2008 Michael Margolis All right reserved
 *
 * vi:ts=4
 *
 * Use this file to set io pins and LCD panel parameters
 *
*/

#ifndef	glcd_KS0108_CONFIG_H
#define glcd_KS0108_CONFIG_H

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
/* Arduino pins used for Commands
 * default assignment uses the first five analog pins
 */

 // control pins
#define glcdCSEL1			14
#define glcdCSEL2			15
#define glcdRW				16
#define glcdDI				17
#define glcdEN				18
// Reset Bit  - uncomment the next line if reset is connected to an output pin
#define glcdRES		    19	// Reset Bit 

// Data pin definitions
#define glcdData0Pin		8
#define glcdData1Pin		9
#define glcdData2Pin		10
#define glcdData3Pin		11
#define glcdData4Pin		4
#define glcdData5Pin		5
#define glcdData6Pin		6
#define glcdData7Pin		7


// bitmask to sequence chip select 
// you can swap around the elements below if your display is reversed
#define glcd_CHIP1  1
#define glcd_CHIP2  2
#define glcd_CHIP3  0 // used only for a three chip (192 pixel) panel (0, 1, 2)



// defines for panel specific timing 
/*
 *	Nov 2009
 *	Low level additions added by Bill Perry
 *	bill@billsworld.billandterrie.com
 *
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
#endif
