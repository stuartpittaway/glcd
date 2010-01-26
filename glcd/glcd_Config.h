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

/*========================== experimental Defines ============================*/

// All these eperimental defines are only intended to be temporary until we make
// up our minds on a final solution.
// My thought is that all these "experimental" defines will go away.

//#define CANNED_TAREAS	// defines up to the the first 4 text areas after #0 to be:
						// Top, bottom, left, right halves of the screen.
						// When GLCD_TAREA_CNT is less than 5 the areas are created in
						// in the above order up to the GLCD_TAREA_CNT
						//
						// RECOMMENDED Setting: OFF


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




#define BITMAP_FIX		// erases region behind bitmap before painting
						// It MUST be on if TRUE_WRITE is off.
						// until there is a new bitmap rendering routine, this is a better
						// method to use to fix bitmap rendering than TRUE_WRITE.
						// Because if TRUE_WRITE is off, the bits will be ORd in and
						// since the region behind the bitmap is erased, the ORs will or
						// in the bitmap data properly on any boundary and the
						// bitmap can be any height.
						//
						// TRUE_WRITE will render bitmaps on any boundary but not quite
						// correctly. It will cause pixels below the bitmap to be cleared
						// when the bitmap is not a multiple of 8 in height.
						//
						// this can be left on either way TRUE_WRITE is set.
						// but isn't needed if TRUE_WRITE is set.
						//
						// RECOMMENDED setting: ON
						
						


//#define SCROLL_WHITE	// always scroll in new WHITE pixels, rather than inverse of font color
						// This also affects what pixel color is used on TWinClear().
						// This only has effect when using the new PUTCHAR and must be used if
						// TRUE_WRITE is off and NEW_FONTDRAW is also off
						// it ensures the WHITE text works when using new putchar but the
						// old rendering code.
						//
						// RECOMMENDED setting: OFF 


#define LWRITE_FIX		// BUG fix in WriteData() - this should be a permanent fix
						// This define turns on code to fix a bug in WriteData()
						// WriteData() would attempt to write a byte below the
						// bottom edge of the display when an unaligned byte write
						// was done less than 8 pixels from the bottom.
						// this corrects that
						//
						// RECOMMENDED setting: ALWAYS ON

/*========================== End of experimental Defines ============================*/


//#define ORIG_PUTCHAR			// revert back to pre wrap/scroll/window code
								// also preserves old Puts newline behavior.
								// also forces the old style character rendering.
								// Basically a compability mode.
								// Maybe this should be a V2_LIB Compability define?
								// This would be useful for those that simply want to use
								// the newer GLCD library and easier configuration
								// but don't want any of the new stuff which might
								// break their current code or eventually be a 
								// different font format.

/*
 * Defines below this point are only used with new putchar code.
 * i.e. If ORIG_PUTCHAR is turned on, they are ignored.
 */

#define GLCD_TAREA_CNT	5 		// number of text areas, each text area costs ~12 bytes of RAM.


//#define GLCD_NO_SCROLLDOWN	// disable reverse scrolling (saves ~600 bytes of code)
								// This will allow those tight on FLASH space to save a bit of
								// code room.


//#define NEW_FONTDRAW			// turn on to get new font rendering


/*
 * Only select one of the following:
 */

//#include "config/Modagm1264f_Config-bill.h"
#include "config/Modagm1264f_Config-teensy++.h"
//#include "config/Modvk5121_Config-bill.h"

//#include "config/ks0108_Config.h"
//#include "config/Modagm1264f_Config.h"
//#include "config/sed1520_Config.h"
//#include "config/Mod4glm12232_Config.h"
//#include "config/Modvk5121_Config.h"
//#include "config/ks0713_Config.h"

#endif
