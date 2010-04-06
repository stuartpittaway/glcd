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
  * The code below selects a configuration file based on the board selected in the IDE 
  * These configurations are compatable with wiring used in earlier versions of the library
  * 
  * You can select a cusomtized configuration file by uncommenting the next line
 */
//#define MANUAL_CONFIG_SELECT  // uncomment this to enable explicit manual config selection
#ifndef MANUAL_CONFIG_SELECT  // define this to bypass automatic config selection
 
#if defined(__AVR_ATmega1280__)
#include "config/ks0108_Mega_Config.h"      // config for Mega or other 1280 board
#elif defined (__AVR_ATmega644P__)           
#include "config/ks0108_Sanguino_Config.h"  // config for Sanguino or other ATmega644p board
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) || defined(__AVR_ATmega32U4__)// Teensy
#include "config/ks0108_Teensy_Config.h"    // config for Teensy and Teensy++  
#else
#include "config/ks0108_Arduino_Config.h"   // config file for standard Arduino using documented wiring 
#endif

#else
/*
 *    Customized configuration selection : 
 *   if MANUAL_CONFIG_SELECT is defined then select one only of the following config files 
 */
#include "config/ks0108_Arduino_Config.h"   // config file for standard Arduino using documented wiring 
//#include "config/ks0108_Mega_Config.h"      // config for Mega board using same pins as previous library
//#include "config/ks0108_Sanguino_Config.h"  // config for Sanguino using same pins as previous library
//#include "config/ks0108_Teensy_Config.h"    // config for Teensy and Teensy++  


//#include "config/sed1520_Config.h"          // generic SED1520 configuraition file 
//#include "config/Modvk5121_Config.h"        // config file for vk5121 panel  
#endif

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
