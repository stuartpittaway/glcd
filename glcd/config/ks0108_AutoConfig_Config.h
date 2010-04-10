 /*
  * The code below selects a configuration file based on the board selected in the IDE 
  * These configurations are compatable with wiring used in earlier versions of the library
  * 
 */
 
#if defined(__AVR_ATmega1280__)
#include "config/ks0108_Mega_Config.h"      // config for Mega or other 1280 board
#elif defined(__AVR_ATmega644P__)  || defined(__AVR_ATmega644__)           
#include "config/ks0108_Sanguino_Config.h"  // config for Sanguino or other ATmega644/p board
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) || defined(__AVR_ATmega32U4__)// Teensy
#include "config/ks0108_Teensy_Config.h"    // config for Teensy and Teensy++  
#else
#include "config/ks0108_Arduino_Config.h"   // config file for standard Arduino using documented wiring 
#endif
