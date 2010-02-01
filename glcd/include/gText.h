/*
  gText.h - Support for Text output on a grpahical device
  vi:ts=4

  The routines in this file are Copyright (c) Bill Perry 2009


  This version is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
*/

#include <inttypes.h>
#include <avr/pgmspace.h>

#include "include/glcd_Device.h"


#ifndef	GTEXT_H
#define GTEXT_H

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
#define isFixedWidtFont(font)  (this->FontRead(font+FONT_LENGTH) == 0 && this->FontRead(font+FONT_LENGTH+1) == 0))


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


uint8_t ReadPgmData(const uint8_t* ptr);	//Standard Read Callback

struct tarea
{
	uint8_t x1;
	uint8_t y1;
	uint8_t	x2;
	uint8_t y2;
	int8_t  scrolldir;	/* signed value, -1 is reverse scroll */
};

struct tareacntxt
{
	struct tarea 	tarea;
	const uint8_t*	Font;
	FontCallback 	FontRead;
	uint8_t			FontColor;
	uint8_t			x;
	uint8_t			y;
};


 // graphical device text routines
class gText : public Print
{
  private:
	glcd_Device    *device;
	FontCallback	FontRead;
	uint8_t			FontColor;
	const uint8_t*	Font;
	struct tarea tarea;

	uint8_t tarea_active;
	struct tareacntxt tarea_cntxt[GLCD_TAREA_CNT];
	void SpecialChar(char c);

	// Scroll routines are private for now
	void ScrollUp(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t pixels, uint8_t color);
	void ScrollDown(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t pixels, uint8_t color);

  public:
	gText();
	void Init(glcd_Device* _device);

	// Text area functions

	uint8_t DefineArea(uint8_t area, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, int8_t scrolldir=1);
	uint8_t DefineArea(uint8_t area, uint8_t x1, uint8_t y1, uint8_t columns, uint8_t rows, const uint8_t* font, int8_t scrolldir=1);
	uint8_t DefineArea(uint8_t area, predefinedArea selection, int8_t scrolldir=1);
	void ClearArea(void);
	void SelectArea(uint8_t area);

	// Font Functions
	void SelectFont(const uint8_t* font, uint8_t color=BLACK, FontCallback callback=ReadPgmData); // defualt arguments added, callback now last arg
	int PutChar(char c);
	void Puts(char *str);
	void Puts_P(PGM_P str);

	void write(uint8_t c);  // character output for print base class

	void CursorTo( uint8_t column, uint8_t row); // 0 based coordinates for character columns and rows
	void CursorToXY( uint8_t x, uint8_t y); // coordinates relative to active text area
	uint8_t CharWidth(char c);
	uint16_t StringWidth(const char* str);
	uint16_t StringWidth_P(PGM_P str);

	static const uint8_t AreaCount = GLCD_TAREA_CNT;	 //mem 15 Jan 10

	/*
 	 * ansi like line erase function
	 */
	void EraseInLine( uint8_t type=0); //  0 = EOL, 1= BOL, 2= full line
};

#endif
