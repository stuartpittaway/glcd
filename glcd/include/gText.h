/*
  gText.h - Support for Text output on a graphical device
  vi:ts=4

  The routines in this file are Copyright (c) Bill Perry and Michael Margolis 2009


  This version is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
*/

#ifndef	GTEXT_H
#define GTEXT_H

#include <inttypes.h>
#include <avr/pgmspace.h>

#include "include/streaming.h" 
#include "include/glcd_Device.h"



#define GTEXT_VERSION 1 // software version of this code

// Font Indices
#define FONT_LENGTH			0
#define FONT_FIXED_WIDTH	2
#define FONT_HEIGHT			3
#define FONT_FIRST_CHAR		4
#define FONT_CHAR_COUNT		5
#define FONT_WIDTH_TABLE	6

// the following returns true if the given font is fixed width
// zero length is flag indicating fixed width font (array does not contain width data entries)
#define isFixedWidtFont(font)  (FontRead(font+FONT_LENGTH) == 0 && FontRead(font+FONT_LENGTH+1) == 0))


typedef uint8_t (*FontCallback)(const uint8_t*);


/*
 * Coodinates for predefined areas are compressed into a single 32 bit token.
 *
 * This works as the coordinates are cuurenly only 8 bit values.
 *
 * This allows the creatation of an unlmited number of predefined areas with zero code or
 * data space overhead.
 * 
 * A macro is used to create the tokens from a set of x1,y1 x2,y2 coordinates.
 *
 * A union is used to extract the coordinates from the 32 bit token.
 *
 * WARNING:
 *	This is non portable code in that it will only work on little endian processors.
 *	If you use big endian you have to switch the byte ordering in the union.
 */

#define MK_TareaToken(x1, y1, x2, y2) \
	(uint32_t) (((uint32_t) (x1) << 24) | ((uint32_t)(y1) << 16) | ((uint32_t)(x2) << 8) | (uint32_t)(y2))
        
typedef union
{       
 struct
 {      
        uint8_t y2;
        uint8_t x2;
        uint8_t y1;
        uint8_t x1;
 }coord;
        
 uint32_t token; // swap byte order above for big endian
        
}TareaToken;

typedef uint8_t textMode;  // type holding mode for scrolling and future attributes like padding etc.
// the only textMode supported in the current release is scrolling
const textMode SCROLL_UP = 1;
const textMode SCROLL_DOWN = 0; // this was changed from -1 so it can used in a bitmask 
const textMode DEFAULT_SCROLLDIR = SCROLL_UP;


/*
 * Pre Defined Text areas
 */
typedef enum  {

	textAreaFULL         = MK_TareaToken( 0,               0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   ),
	textAreaTOP          = MK_TareaToken( 0,               0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT/2 -1 ),
	textAreaBOTTOM       = MK_TareaToken( 0,               DISPLAY_HEIGHT/2, DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   ),
	textAreaLEFT         = MK_TareaToken( 0,               0,                DISPLAY_WIDTH/2 -1, DISPLAY_HEIGHT -1   ),
	textAreaRIGHT        = MK_TareaToken( DISPLAY_WIDTH/2, 0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   ),
	textAreaTOPLEFT      = MK_TareaToken( 0,               0,                DISPLAY_WIDTH/2 -1, DISPLAY_HEIGHT/2 -1 ),
	textAreaTOPRIGHT     = MK_TareaToken( DISPLAY_WIDTH/2, 0,                DISPLAY_WIDTH -1,   DISPLAY_HEIGHT/2 -1 ),
	textAreaBOTTOMLEFT   = MK_TareaToken( 0,               DISPLAY_HEIGHT/2, DISPLAY_WIDTH/2 -1, DISPLAY_HEIGHT -1   ),
	textAreaBOTTOMRIGHT  = MK_TareaToken( DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, DISPLAY_WIDTH -1,   DISPLAY_HEIGHT -1   )
} predefinedArea;



/*
  * enums for ansi style erase function
  * These values match the ANSI EraseInLine terminal primitive: CSI n K
*/  
typedef enum {eraseTO_EOL, eraseFROM_BOL, eraseFULL_LINE} eraseLine_t;  	


uint8_t ReadPgmData(const uint8_t* ptr);	//Standard Read Callback
static glcd_Device    *device;              // static pointer to the device instance
static FontCallback	FontRead;               // font callback shared across all instances

struct tarea
{
	uint8_t x1;
	uint8_t y1;
	uint8_t	x2;
	uint8_t y2;
	int8_t  mode;
};

  
 // graphical device text routines
class gText : public Print
{
  private:
    //FontCallback	FontRead;     // now static, move back here if each instance needs its own callback
	uint8_t			FontColor;
	const uint8_t*	Font;
	struct tarea tarea;
	uint8_t			x;
	uint8_t			y;

	void SpecialChar(char c);

	// Scroll routines are private for now
	void ScrollUp(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t pixels, uint8_t color);
	void ScrollDown(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t pixels, uint8_t color);
	
  public:
	gText(); // default - uses the entire display
    gText(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, textMode mode=DEFAULT_SCROLLDIR);
	// 4 Feb - added two constuctors (and SetFontColor below) 
	gText(predefinedArea selection, textMode mode=DEFAULT_SCROLLDIR);
	gText(uint8_t x1, uint8_t y1, uint8_t columns, uint8_t rows, const uint8_t* font, textMode mode=DEFAULT_SCROLLDIR);

	//void Init(glcd_Device* _device); // no longer used

	// Text area functions
	uint8_t DefineArea(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, textMode mode=DEFAULT_SCROLLDIR);
	uint8_t DefineArea(uint8_t x1, uint8_t y1, uint8_t columns, uint8_t rows, const uint8_t* font, textMode mode=DEFAULT_SCROLLDIR);
	uint8_t DefineArea(predefinedArea selection, textMode mode=DEFAULT_SCROLLDIR);
	void SetTextMode(textMode mode); // change to the given text mode
	void ClearArea(void);

	// Font Functions
	void SelectFont(const uint8_t* font, uint8_t color=BLACK, FontCallback callback=ReadPgmData); // default arguments added, callback now last arg
	void SetFontColor(uint8_t color); // new method
	int PutChar(char c);
	void Puts(char *str);
	void Puts_P(PGM_P str);

	void write(uint8_t c);  // character output for print base class

	void CursorTo( uint8_t column, uint8_t row); // 0 based coordinates for character columns and rows
	void CursorTo( int8_t column); // move cursor on the current row
	void CursorToXY( uint8_t x, uint8_t y); // coordinates relative to active text area
	uint8_t CharWidth(char c);
	uint16_t StringWidth(const char* str);
	uint16_t StringWidth_P(PGM_P str);

	void EraseTextLine( eraseLine_t type=eraseTO_EOL); //ansi like line erase function 
	void EraseTextLine( uint8_t row); // erase the entire text line in the given row and move cursor to left position

#ifndef GLCD_NO_PRINTF
	void Printf(const char *format, ...);
#endif

};

#endif
