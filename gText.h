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

#include "glcd_Device.h"


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
class gText
{
  private:
	glcd_Device    *device;
	FontCallback	FontRead;
	uint8_t			FontColor;
	const uint8_t*	Font;
	struct tarea tarea;

#if GLCD_TAREA_CNT > 0   
	uint8_t tarea_active;
	struct tareacntxt tarea_cntxt[GLCD_TAREA_CNT];
#endif
	void SpecialChar(char c);

	// Scroll routines are private for now
	void ScrollUp(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t pixels, uint8_t color);
	void ScrollDown(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t pixels, uint8_t color);

  public:
	gText();
	void Init(glcd_Device* _device);

	// Text area functions

	uint8_t DefineArea(uint8_t area_num, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, int8_t scrolldir=1);
	uint8_t DefineArea(uint8_t area_num, uint8_t x1, uint8_t y1, uint8_t columns, uint8_t rows, const uint8_t* font, int8_t scrolldir=1);
	void ClearArea(void);

#if GLCD_TAREA_CNT > 0
	void SelectArea(uint8_t area);
#endif

	// Font Functions
	void SelectFont(const uint8_t* font, uint8_t color=BLACK, FontCallback callback=ReadPgmData); // defualt arguments added, callback now last arg
	int PutChar(char c);
	void Puts(char *str);
	void Puts_P(PGM_P str);

	void DrawString(char* str,uint8_t x, uint8_t y);
	void DrawString_P(PGM_P str,uint8_t x, uint8_t y);
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
