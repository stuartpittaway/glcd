/*
 * glcd_io.h
 *
 * vi:ts=4
 *
 *	This file maps abstract io requests from glcd_Device to AVR port and pin abstractions
 *  arduino_avrio.h maps arduino pins to avr ports and pins.
 *  The physical io is handled by macros in avrio.h
 */


#include "arduino_io.h"    // these macros map arduino pins
#include "avrio.h"         // these macros do direct port io    

// lcdfastWrite Macro may be replaced by Paul's new Arduino macro 
#define lcdfastWrite(pin, pinval) avrio_WritePin(pin, pinval)

 
#ifdef  _AVRIO_AVRIO_
#define OUTPUT 1
#define lcdPinMode(pin, mode)  avrio_PinMode(pin, mode) 
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

/*
 * alias to read Busy status bit
 * In order to do this properly it takes concatenation
 * to convert the LCD status bit value to a pin
 */

#define xGLCD_STATUS_BIT2PIN(bit)    glcdData ## bit  ## Pin
#define GLCD_STATUS_BIT2PIN(bit)    xGLCD_STATUS_BIT2PIN(bit)    


#define lcdIsBusy()		(avrio_ReadPin(GLCD_STATUS_BIT2PIN(LCD_BUSY_BIT)))


#ifdef glcdRES
#define lcdReset()		avrio_WritePin(glcdRES, 0)
#define lcdUnReset()	avrio_WritePin(glcdRES, 1)
#else
#define lcdReset()		
#define lcdUnReset()		
#endif


