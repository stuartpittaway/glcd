/*
  ks0108_Device.h - Arduino library support for graphic LCDs 

 vi:ts=4
  
*/

#ifndef GLCD_GLCD_DEVICE_H
#define GLCD_GLCD_DEVICE_H

/*
 * define name for Device
 */
#define glcd_DeviceName "ks0108"

/*
 * Sanity check KS0108 config pins
 */

#ifndef glcdCSEL1
#error "KS0108 configuration missing glcdCSEL1"
#endif
#ifndef glcdCSEL2
#error "KS0108 configuration missing glcdCSEL2"
#endif

#ifndef glcdEN
#error "KS0108 configuration missing glcdEN"
#endif

#ifdef glcdE1
#error "KS0108 configuration does not use glcdE1"
#endif
#ifdef glcdE2
#error "KS0108 configuration does not use glcdE2"
#endif

/*
 * LCD commands -------------------------------------------------------------
 */

#define LCD_ON				0x3F
#define LCD_OFF				0x3E
#define LCD_SET_ADD			0x40
#define LCD_DISP_START		0xC0
#define LCD_SET_PAGE		0xB8

/*
 * Status register bits/flags -----------------------------------------------
 */
#define LCD_BUSY_FLAG		0x80 
#define LCD_BUSY_BIT		7

/*
 * Define primitives used by glcd_Device.cpp --------------------------------
 */

#define glcd_DevSelectChip(chip) 	SelectChip(chip)

/*
 * KS0108 doesn't use/need a chip # to strobe EN
 * as there is only a single EN line, so ignore the chip parameter.
 */
#define glcd_DevENstrobeHi(chip) lcdfastWrite(glcdEN, 1)
#define glcd_DevENstrobeLo(chip) lcdfastWrite(glcdEN, 0)

/*
 * Convert X & Y coordinates to chip values
 *
 * While it looks like espensive divides, it will
 * actually map to masking because chip widths/heights
 * on the ks0108 are powers of 2
 */
#if CHIP_HEIGHT < DISPLAY_HEIGHT
#define glcd_DevXYval2Chip(x,y)		((y/CHIP_HEIGHT))	
#else
#define glcd_DevXYval2Chip(x,y)		((x/CHIP_WIDTH))	
#endif

#define glcd_DevXval2ChipCol(x)		((x) % CHIP_WIDTH)

#endif //GLCD_GLCD_DEVICE_H
