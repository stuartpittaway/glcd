/*
  glcd_io.h
  Copyright (c) 2009 Bill Perry
  
  vi:ts=4

  This file is part of the Arduino GLCD library.

  GLCD is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 2.1 of the License, or
  (at your option) any later version.

  GLCD is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with GLCD.  If not, see <http://www.gnu.org/licenses/>.

  This file maps abstract io requests from glcd_Device to AVR port and pin abstractions
  arduino_avrio.h maps arduino pins to avr ports and pins.
  The physical io is handled by macros in avrio.h
 
 */

#ifndef	GLCD_IO_H
#define GLCD_IO_H

#include "include/arduino_io.h"    // these macros map arduino pins
#include "include/avrio.h"         // these macros do direct port io    

 
/*
 * Map a Busy status bit to a pin.
 * In order to do this properly it takes concatenation
 * to convert the LCD status bit value to a pin
 */

#define xGLCD_STATUS_BIT2PIN(bit)    glcdData ## bit  ## Pin
#define GLCD_STATUS_BIT2PIN(bit)    xGLCD_STATUS_BIT2PIN(bit)    


#ifdef  _AVRIO_AVRIO_

// lcdfastWrite Macro may be replaced by Paul's new Arduino macro 
#define lcdfastWrite(pin, pinval) avrio_WritePin(pin, pinval)

#define OUTPUT 1
#define lcdPinMode(pin, mode)  avrio_PinMode(pin, mode) 

#ifdef GLCD_ATOMIC_IO
#define AVRIO_NO4BIT // for now disable nibble mode
#endif


/*
 * Set up the configured LCD data lines 
 */

#define lcd_avrWriteByte(data) 					\
	avrio_Write8Bits(AVRIO_PORTREG,				\
			glcdData0Pin, glcdData1Pin,		\
			glcdData2Pin, glcdData3Pin,		\
			glcdData4Pin, glcdData5Pin,		\
			glcdData6Pin, glcdData7Pin, data)


/*
 * Read the configured LCD data lines and return data as 8 bit byte
 */
#define lcd_avrReadByte() 					\
	avrio_Read8Bits(AVRIO_PINREG,				\
			glcdData0Pin, glcdData1Pin,		\
			glcdData2Pin, glcdData3Pin,		\
			glcdData4Pin, glcdData5Pin,		\
			glcdData6Pin, glcdData7Pin)


/*
 * Configure the direction of the data pins.
 *	0x00 is for input and 0xff is for output.
 */
#define lcdDataDir(dirbits)					\
	avrio_Write8Bits(AVRIO_DDRREG, 				\
			glcdData0Pin, glcdData1Pin,		\
			glcdData2Pin, glcdData3Pin,		\
			glcdData4Pin, glcdData5Pin,		\
			glcdData6Pin, glcdData7Pin, dirbits)

/*
 * alias to setup LCD data lines.
 */
#define lcdDataOut(data)	lcd_avrWriteByte(data)

/*
 * alias to Read LCD data lines.
 */
#define lcdDataIn()		lcd_avrReadByte()


#define lcdIsBusy()		(avrio_ReadPin(GLCD_STATUS_BIT2PIN(LCD_BUSY_BIT)))


#ifdef glcdRES
#define lcdReset()		avrio_WritePin(glcdRES, 0)
#define lcdUnReset()	avrio_WritePin(glcdRES, 1)
#else
#define lcdReset()		
#define lcdUnReset()		
#endif

#endif // _AVRIO_AVRIO_

/*
 * Delay functions
 */

/*
 * For nano second delays there are two real options:
 * The AVR libC supplied <util/delay.h> is not good eough,
 * there is too much error in their calculations.
 *
 * - Hans Heinrichs _delay_cycles()
 * - avr-gcc __builtin_avr_delay_cycles() routine.
 *
 * The two generate very similar code, however the gcc
 * builtin function is only avialable in newer gcc revisions
 * (some older linux avr-gcc revisions don't have it) and Hans' code
 * is sometimes smaller and doesn't use loops which require a
 * a register when the number cycles is less than 12.
 */
#include "include/delay.h" // Hans' Heirichs delay macros

#define lcdDelayNanoseconds(__ns) _delay_cycles( (double)(F_CPU)*((double)__ns)/1.0e9 + 0.5 ) // Hans Heinrichs delay cycle routine

#define lcdDelayMilliseconds(__ms) delay(__ms)	// Arduino delay function

#endif // GLCD_IO_H
