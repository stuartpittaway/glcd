/*
 * sed1520_Config.h - User specific configuration for Arduino GLCD library
 *
 * vi:ts=4
 *
 * Use this file to set io pins and LCD panel parameters for 
 *	sed1520 based modules. This includes modules like VK5121
 *
*/

#ifndef	glcd_SED1520_CONFIG_H
#define glcd_SED1520_CONFIG_H	

/*********************************************************/
/*  Configuration for LCD panel specific configuration   */
/*********************************************************/
#define DISPLAY_WIDTH 122
#define DISPLAY_HEIGHT 32

// panel controller chips
#define CHIP_WIDTH     122  // pixels per chip 
#define CHIP_HEIGHT    16  // pixels per chip 

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/

/*
 * Bill's avrio pin assignments 
 * for the Modern Device BBB "arduino" breadboard.project
 */

#define glcdData0Pin	PIN_B5
#define glcdData1Pin	PIN_B4
#define glcdData2Pin	PIN_B3
#define glcdData3Pin	PIN_B2
#define glcdData4Pin	PIN_B1
#define glcdData5Pin	PIN_B0
#define glcdData6Pin	PIN_D7
#define glcdData7Pin	PIN_D6

#define glcdRW			PIN_D5

#define glcdE1			PIN_D4

#define glcdDI			PIN_D3	// A0 line

#define glcdE2			PIN_D2


// defines for panel specific timing 
// Generic very conservative timing
#define GLCD_tDDR	320	/* tACC6: Data Delay time (E high to valid read data) 			*/
#define GLCD_tAS	20	/* tAW6:  Address setup time (ctrl line changes to E high)		*/
#define GLCD_tDSW	200	/* tDS6:  Data setup time (Write data lines setup to dropping E)*/
#define GLCD_tWH    450	/* tEW:   E hi level width (minimum E hi pulse width)			*/ 
#define GLCD_tWL	450	/* E lo level width (minimum E lo pulse width)					*/


#include "device/sed1520_Device.h"
#endif
