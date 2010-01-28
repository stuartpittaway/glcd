/*
 * glcd_Config.h - User specific configuration for Arduino GLCD library
 *
 * vi:ts=4
 *
 * Use this file to configure which glcd module is used
 * Then edit the selected module file to configure parameters for that module.
 *
*/

#ifndef	glcd_CONFIG_H
#define glcd_CONFIG_H

 /*
  * the following list are the files to be included in the release 
  * all other display specific files are in config/dev_Config.h
  * Only select one of the following:
  *
  */
//#include "config/ks0108_Config.h"
//#include "config/sed1520_Config.h"

/*========================== experimental Defines ============================*/

// All these eperimental defines are only intended to be temporary until we make
// up our minds on a final solution.
// My thought is that all these "experimental" defines will go away.

//#define TRUE_WRITE	// does writes to memory on page crossings vs ORs
						// When off, WriteData() works just like it used to.
						// This option only affects writes that span LCD pages.
						// None of the graphic rouintes nor the NEW_FONTDRAW option do this.
						// only the font rendering and bitmap rendering to unaligned PAGE writes.
						// While this fixes a few issus for the old routines,
						// it also creates new ones.
						// The issue is routines like putchar() and the bitmap rendering
						// routine attempt to use a drawing method that does not work.
						// when this is on, pixels are no longer ORd in but are written in.
						// so all the good/desired pixels get set, but then so do some
						// undesired pixels.
						// When on, bitmaps that are multiples of 8 in height will always
						// render correctly.
						//
						// Without this on, WHITE text mode has many issue.
						//
						// This option has no effect on the NEW_FONDRAW option.
						//
						// RECOMMENDED setting: OFF

/*========================== End of experimental Defines ============================*/

/*
 * Defines below this point are only used with new putchar code.
 * i.e. If ORIG_PUTCHAR is turned on, they are ignored.
 */

#define GLCD_TAREA_CNT	3 		// number of text areas, each text area costs ~12 bytes of RAM.


//#define GLCD_NO_SCROLLDOWN	// disable reverse scrolling (saves ~600 bytes of code)
								// This will allow those tight on FLASH space to save a bit of code space

#define NEW_FONTDRAW			// turn on to get new font rendering


#endif
