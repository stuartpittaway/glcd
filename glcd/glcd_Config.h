/*
 * glcd_Config.h - User specific configuration for Arduino GLCD library
 *
 * vi:ts=8
 *
 * Use this file to select the active glcd configuration file
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

//#include "config/ks0108_Arduino_Config.h"   // config file for standard Arduino using documented wiring 
//#include "config/ks0108_Mega_Config.h"      // config for Mega board using same pins as previous library
//#include "config/ks0108_Sanguino_Config.h"  // config for Sanguino using same pins as previous library
//#include "config/ks0108_Teensy_Config.h"    // config for Teensy and Teensy++  


//#include "config/sed1520_Config.h"          // generic SED1520 configuraition file 
//#include "config/Modvk5121_Config.h"        // config file for vk5121 panel  

/*========================== Optional User Defines ==================================*/

//#define GLCD_NO_SCROLLDOWN	// disable reverse scrolling (saves ~600 bytes of code)
				// This will allow those tight on FLASH space to save a bit of code space

//#define GLCD_ATOMIC_IO	// Generate code that ensures all pin i/o operations are atomic
				// including any potential nibble operations.
				// Without this option enabled, nibble operations will be slightly faster but
				// might have issues if a pin used shares a processor i/o port with an
				// interrupt routine that updates pins/bits on the same port.

//#define GLCD_OLD_FONTDRAW	// uncomment this define to get old font rendering (not recommended)

//#define GLCD_NODEFER_SCROLL	// uncomment to disable deferred newline processing
//#define GLCD_READDATA_XFAST	// uncomment to enable experimental code that speeds up LCD read operations.

#endif
