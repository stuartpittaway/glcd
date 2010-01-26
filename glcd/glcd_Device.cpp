/*
  glcd_Device.cpp - Arduino library support for graphic LCDs 
  Copyright (c)2008 Michael Margolis All right reserved

 vi:ts=4
  
  The glcd_Device class impliments the protocol for sending and receiving data and commands to a GLCD device.
  It uses glcd_io.h to for the io primitives and glcd_COnfig.h for user specific configuration.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

*/

#include <avr/io.h>
#include <wiring.h> // needed for arduino io methods
#include "include/delay.h"  /* for _delayNanoseconds() functionality */

#include "include/glcd_Device.h"
#include "include/glcd_io.h"


#ifdef glcd_CHIP1
#define glcd_CHIP_COUNT (DISPLAY_WIDTH / CHIP_WIDTH)
static uint8_t chipSelect[glcd_CHIP_COUNT] ; //static array for sequencing chip selects
#endif

	
glcd_Device::glcd_Device(){
  
}

/**
 * set pixel at x,y to the given color
 *
 * @param x X coordinate, a value from 0 to GLCD.Width-1
 * @param y Y coordinate, a value from 0 to GLCD.Heigh-1
 * @param color WHITE or BLACK
 *
 * Sets the pixel at location x,y to the specified color.
 * x and y are relative to the 0,0 origin of the display which
 * is the upper left corner.
 *
 * @note If the display has been set to INVERTED mode then the colors
 * will be automically reversed.
 *
 */

void glcd_Device::SetDot(uint8_t x, uint8_t y, uint8_t color) 
{
	uint8_t data;
	
	this->GotoXY(x, y-y%8);					// read data from display memory
  	
	data = this->ReadData();
	if(color == BLACK){
		data |= 0x01 << (y%8);				// set dot
	} else {
		data &= ~(0x01 << (y%8));			// clear dot
	}	
	this->WriteData(data);					// write data back to display
}

/**
 * set an area of pixels
 *
 * @param x X coordinate of upper left corner
 * @param y Y coordinate of upper left corner
 * @param x2 X coordinate of lower right corner
 * @param y2 Y coordinate of lower right corner
 *
 * sets the pixels an area bounded by x,y to x2,y2 inclusive
 * to the specified color.
 *
 * The width of the area is x2-x + 1. 
 * The height of the area is y2-y+1 
 * 
 *
 */

// set pixels from upper left edge x,y to lower right edge x1,y1 to the given color
// the width of the region is x1-x + 1, height is y1-y+1 

void glcd_Device::SetPixels(uint8_t x, uint8_t y,uint8_t x2, uint8_t y2, uint8_t color)
{
uint8_t mask, pageOffset, h, i, data;
uint8_t height = y2-y+1;
uint8_t width = x2-x+1;
	
	pageOffset = y%8;
	y -= pageOffset;
	mask = 0xFF;
	if(height < 8-pageOffset) {
		mask >>= (8-height);
		h = height;
	} else {
		h = 8-pageOffset;
	}
	mask <<= pageOffset;
	
	this->GotoXY(x, y);
	for(i=0; i < width; i++) {
		data = this->ReadData();
		
		if(color == BLACK) {
			data |= mask;
		} else {
			data &= ~mask;
		}

		this->WriteData(data);
	}
	
	while(h+8 <= height) {
		h += 8;
		y += 8;
		this->GotoXY(x, y);
		
		for(i=0; i <width; i++) {
			this->WriteData(color);
		}
	}
	
	if(h < height) {
		mask = ~(0xFF << (height-h));
		this->GotoXY(x, y+8);
		
		for(i=0; i < width; i++) {
			data = this->ReadData();
		
			if(color == BLACK) {
				data |= mask;
			} else {
				data &= ~mask;
			}
	
			this->WriteData(data);
		}
	}
}

/**
 * set current x,y coordinate on display device
 *
 * @param x X coordinate
 * @param y Y coordinate
 *
 * Sets the current pixel location to x,y.
 * x and y are relative to the 0,0 origin of the display which
 * is the upper left most pixel on the display.
 */

void glcd_Device::GotoXY(uint8_t x, uint8_t y)
{
  uint8_t chip, cmd;

  if( (x > DISPLAY_WIDTH-1) || (y > DISPLAY_HEIGHT-1) ) {	// exit if coordinates are not legal  
#ifdef BAPDEBUG_BADVALUES
printf("gotoxy: badXY: %d,%d\n", x, y);
#endif
    return;
  }

  this->Coord.x = x;								// save new coordinates
  this->Coord.y = y;

  chip = glcd_DevXYval2Chip(x, y);

	if(y/8 != this->Coord.chip[chip].page){
  		this->Coord.chip[chip].page = y/8;
		cmd = LCD_SET_PAGE | this->Coord.chip[chip].page;
	   	this->WriteCommand(cmd, chip);	
	}
	x = glcd_DevXval2ChipCol(x);
	
	/*
	 * NOTE: For now, the "if" below is intentionally commented out.
	 * In order for this to work, the code must properly track
	 * the x coordinate of the chips and not allow it go beyond proper
	 * boundaries. It isnn't complicated to do, it jsut isn't done that
	 * way right now.
	 */

	//if(x != this->Coord.chip[chip].col)
	{
//		this->Coord.chip[chip].col = x;		// No column tracking yet
#ifdef LCD_SET_ADDLO
		cmd = LCD_SET_ADDLO | glcd_DevCol2addrlo(x);
	   	this->WriteCommand(cmd, chip);	

		cmd = LCD_SET_ADDHI | glcd_DevCol2addrhi(x);
	   	this->WriteCommand(cmd, chip);	
#else
		cmd = LCD_SET_ADD | x;
	   	this->WriteCommand(cmd, chip);	
#endif
	}
}
/**
 * Low level h/w initialization of display and AVR pins
 *
 * @param invert specifices whether display is in normal mode or inverted mode.
 *
 * This should only be called prior by other library code.
 *
 * It does all the low level hardware initalization of the display device.
 *
 * The optional invert parameter specifies if the display should be run in a normal
 * mode, dark pixels on light background or inverted, light pixels on a dark background.
 *
 * To specify dark pixels use the define @b NON-INVERTED and to use light pixels use
 * the define @b INVERTED
 *
 * Upon completion of the initialization, then entire display will be cleared.
 *
 */

void glcd_Device::Init(uint8_t invert)
{  
    // initialize the chip select array, this provides the select sequence as defined in glcd_Config.h
#ifdef glcd_CHIP1
    chipSelect[0] =  glcd_CHIP1;
    chipSelect[1] =  glcd_CHIP2;
#endif	
#if glcd_CHIP_COUNT > 2	
	chipSelect[2] =  glcd_CHIP3;
#endif

	/*
	 * Now setup the pinmode for all of our control pins.
	 * The data lines will be configured as necessary when needed.
	 */

	lcdPinMode(glcdDI,1); 	
	lcdPinMode(glcdRW,1); 	

#ifdef glcdE1
	lcdPinMode(glcdE1,1); 	
	lcdfastWrite(glcdE1,LOW); 	
#endif
#ifdef glcdE2
	lcdPinMode(glcdE2,1); 	
	lcdfastWrite(glcdE2,LOW); 	
#endif

#ifdef glcdEN
	lcdPinMode(glcdEN,1); 	
	lcdfastWrite(glcdEN, LOW);
#endif

#ifdef glcdCSEL1
	lcdPinMode(glcdCSEL1,1);
	lcdfastWrite(glcdCSEL1, LOW);
#endif

#ifdef glcdCSEL2
	lcdPinMode(glcdCSEL2,1);
	lcdfastWrite(glcdCSEL2, LOW);
#endif

	/*
	 * If reset control
	 */
#ifdef glcdRES
	lcdPinMode(glcdRES,1);
#endif


	lcdfastWrite(glcdDI, LOW);
	lcdfastWrite(glcdRW, LOW);

	this->Coord.x = 0;
	this->Coord.y = 0;
	
	this->Inverted = invert;

	/*
	 * Reset the glcd module if there is a reset pin defined
	 */ 
#ifdef glcdRES
	lcdReset();
	delay(2);  
	lcdUnReset();
	delay(5);
#endif
	
#ifdef glcd_DeviceInit // this provides override for chip specific init -  mem 8 Dec 09
	
	for(uint8_t chip=0; chip < DISPLAY_WIDTH/CHIP_WIDTH; chip++){

		/*
		 * flush out internal state to force first GotoXY() to work
		 */
		this->Coord.chip[chip].page = 0xff;
		//this->Coord.chip[chip].col = 0xff; // not used yet

       	delay(10);  			
        glcd_DeviceInit(chip);  // call device specific initialization if defined    
	}
#else
	for(uint8_t chip=0; chip < DISPLAY_WIDTH/CHIP_WIDTH; chip++){
       		delay(10);  

		/*
		 * flush out internal state to force first GotoXY() to work
		 */
		this->Coord.chip[chip].page = 0xff;
		//this->Coord.chip[chip].col = 0xff;// not used yet

		this->WriteCommand(LCD_ON, chip);			// power on
		this->WriteCommand(LCD_DISP_START, chip);	// display start line = 0

	}
#endif
   // the screeen is cleared in the init method of the derived class 	


}


#ifdef glcd_CHIP1
__inline__ void glcd_Device::SelectChip(uint8_t chip)
{  
#ifndef NEW_CHIPSELECT

	if(chipSelect[chip] & 1)
       lcdfastWrite(glcdCSEL1, HIGH);
	else
	   lcdfastWrite(glcdCSEL1, LOW);

	if(chipSelect[chip] & 2)
       lcdfastWrite(glcdCSEL2, HIGH);
	else
	   lcdfastWrite(glcdCSEL2, LOW);
#else
	if(chip)
	{
       lcdfastWrite(glcdCSEL2, HIGH);
	   lcdfastWrite(glcdCSEL1, LOW);
	}
	else
	{
       lcdfastWrite(glcdCSEL1, HIGH);
	   lcdfastWrite(glcdCSEL2, LOW);
	}
		

#endif
}
#endif

void glcd_Device::WaitReady( uint8_t chip)
{
	// wait until LCD busy bit goes to zero
	glcd_DevSelectChip(chip);
	lcdDataDir(0x00);
	lcdfastWrite(glcdDI, LOW);	
	lcdfastWrite(glcdRW, HIGH);	
	_delayNanoseconds(GLCD_tAS);
	glcd_DevENstrobeHi(chip);
	_delayNanoseconds(GLCD_tDDR);

	while(lcdIsBusy()){
       ;
	}
	glcd_DevENstrobeLo(chip);
}

uint8_t glcd_Device::DoReadData(uint8_t first)
{
	uint8_t data, chip;

	chip = glcd_DevXYval2Chip(this->Coord.x, this->Coord.y);
	this->WaitReady(chip);
	lcdfastWrite(glcdDI, HIGH);		// D/I = 1
	lcdfastWrite(glcdRW, HIGH);		// R/W = 1
	
	_delayNanoseconds(GLCD_tAS);
	glcd_DevENstrobeHi(chip);
	_delayNanoseconds(GLCD_tDDR);

	data = lcdDataIn();	// Read the data bits from the LCD

	glcd_DevENstrobeLo(chip);
    if(first == 0) 
	  this->GotoXY(this->Coord.x, this->Coord.y);	
	if(this->Inverted)
		data = ~data;
	return data;
}
/**
 * read a data byte from display device memory
 *
 * @return the data byte at the current x,y position
 *
 * @note the current x,y location is not modified by the routine.
 *	This allows a read/modify/write operation.
 *	Code can call ReadData() modify the data then
 *  call WriteData() and update the same location.
 *
 * @see WriteData()
 */

inline uint8_t glcd_Device::ReadData(void)
{  
	if(this->Coord.x >= DISPLAY_WIDTH){
		return(0);
	}
	this->DoReadData(1);				// dummy read
	return this->DoReadData(0);			// "real" read
}

void glcd_Device::WriteCommand(uint8_t cmd, uint8_t chip)
 {
	this->WaitReady(chip);
	lcdfastWrite(glcdDI, LOW);					// D/I = 0
	lcdfastWrite(glcdRW, LOW);					// R/W = 0	
	lcdDataDir(0xFF);

	lcdDataOut(cmd);		/* This could be done before or after raising E */
	_delayNanoseconds(GLCD_tAS);
	glcd_DevENstrobeHi(chip);
	_delayNanoseconds(GLCD_tWH);
	glcd_DevENstrobeLo(chip);
}

/**
 * Write a byte to display device memory
 *
 * @param data date byte to write to memory
 *
 * The data specified is written to glcd memory at the current
 * x,y position. If the y location is not on a byte boundary, the write
 * is fragemented up into multiple writes.
 *
 * @note the full behavior of this during split byte writes
 * currently varies depending on a compile time define. 
 * The code can be configured to either OR in 1 data bits or set all
 * the data bits.
 * @b TRUE_WRITE controls this behavior.
 *
 * @note the x,y address will not be the same as it was prior to this call.
 * 	The y address will remain the aame but the x address will advance by one.
 *	This allows back to writes to write sequentially through memory without having
 *	to do additional x,y positioning.
 *
 * @see ReadData()
 *
 */

void glcd_Device::WriteData(uint8_t data) {
	uint8_t displayData, yOffset, chip;
	//showHex("wrData",data);
    //showXY("wr", this->Coord.x,this->Coord.y);

#ifdef GLCD_DEBUG
	volatile uint16_t i;
	for(i=0; i<5000; i++);
#endif

	if(this->Coord.x >= DISPLAY_WIDTH){
		return;
	}

    chip = glcd_DevXYval2Chip(this->Coord.x, this->Coord.y);
	
	yOffset = this->Coord.y%8;

	if(yOffset != 0) {
#ifdef BAPDEBUG_WRITE
printf("--------- Split page write: %d,%d 0x%x\n", this->Coord.x, this->Coord.y, data); //BAPDEBUG
#endif
		// first page
		displayData = this->ReadData();
		this->WaitReady(chip);
   	    lcdfastWrite(glcdDI, HIGH);				// D/I = 1
	    lcdfastWrite(glcdRW, LOW);				// R/W = 0
		lcdDataDir(0xFF);						// data port is output
		_delayNanoseconds(GLCD_tAS);
		glcd_DevENstrobeHi(chip);
		
#ifdef TRUE_WRITE
		/*
		 * Strip out bits we need to update.
		 */
		displayData &= (_BV(yOffset)-1);
#endif

		displayData |= data << yOffset;

		if(this->Inverted){
			displayData = ~displayData;
		}
		lcdDataOut( displayData);					// write data
		_delayNanoseconds(GLCD_tWH);
		glcd_DevENstrobeLo(chip);

#ifdef BAPDEBUG_WRITE
printf("WriteData: 2nd page write\n");
delay(500);
#endif
		// second page

#ifdef LWRITE_FIX
		/*
		 * Make sure to goto y address of start of next page
		 * and ensure that we don't fall off the bottom of the display.
		 */
		uint8_t ysave = this->Coord.y;
		if(((ysave+8) & ~7) >= DISPLAY_HEIGHT)
		{
			this->GotoXY(this->Coord.x+1, ysave);
			return;
		}
	
		this->GotoXY(this->Coord.x, ((ysave+8) & ~7));
#else

		this->GotoXY(this->Coord.x, this->Coord.y+8);
#endif


		displayData = this->ReadData();
		this->WaitReady(chip);

   	    lcdfastWrite(glcdDI, HIGH);					// D/I = 1
	    lcdfastWrite(glcdRW, LOW); 					// R/W = 0	
		lcdDataDir(0xFF);				// data port is output
		_delayNanoseconds(GLCD_tAS);
		glcd_DevENstrobeHi(chip);

#ifdef TRUE_WRITE
		/*
		 * Strip out bits we need to update.
		 */
		displayData &= ~(_BV(yOffset)-1);

#endif
		displayData |= data >> (8-yOffset);
		if(this->Inverted){
			displayData = ~displayData;
		}
		lcdDataOut(displayData);		// write data
		_delayNanoseconds(GLCD_tWH);
		glcd_DevENstrobeLo(chip);
#ifdef LWRITE_FIX
		this->GotoXY(this->Coord.x+1, ysave);
#else
		this->GotoXY(this->Coord.x+1, this->Coord.y-8);
#endif

	}else 
	{
    	this->WaitReady(chip);

		lcdfastWrite(glcdDI, HIGH);				// D/I = 1
		lcdfastWrite(glcdRW, LOW);  				// R/W = 0	
		lcdDataDir(0xFF);						// data port is output

		// just this code gets executed if the write is on a single page
		if(this->Inverted)
			data = ~data;	  

		_delayNanoseconds(GLCD_tAS);
		glcd_DevENstrobeHi(chip);
	
		lcdDataOut(data);				// write data

		_delayNanoseconds(GLCD_tWH);

		glcd_DevENstrobeLo(chip);

		this->Coord.x++;


		/*
		 * Check for crossing into the next chip.
		 */
		if( glcd_DevXYval2Chip(this->Coord.x, this->Coord.y) != chip)
		{
			if(this->Coord.x < DISPLAY_WIDTH)
				this->GotoXY(this->Coord.x, this->Coord.y);
 		}
	    //showXY("WrData",this->Coord.x, this->Coord.y); 
	}
}


void glcd_Device::write(uint8_t) // for Print base class
{

}

#ifndef USE_ARDUINO_FLASHSTR
// functions to store and print strings in Progmem
// these should be removed when Arduino supports FLASH strings in the base print class
void glcd_Device::printFlash(FLASHSTRING str)
{
  char c;
  const prog_char *p = (const prog_char *)str;

  while (c = pgm_read_byte(p++))
    write(c);
}

void glcd_Device::printFlashln(FLASHSTRING str)
{
  printFlash(str);
  write('\n');
}
#endif

