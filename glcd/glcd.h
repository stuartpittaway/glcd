/*
  glcd.h - Arduino library support for graphic LCDs 
  Copyright (c)2008 Michael Margolis All right reserved
  
  vi:ts=4
  
  This file contains high level functions based on the previous ks0108 library
  The routines were dervived from code written and copyright by Fabian Maximilian Thiele.
  you can obtain a copy of his original work here:
  http://www.scienceprog.com/wp-content/uploads/2007/07/glcd_ks0108.zip
 
  The glcd class impliments high level graphics routines. 
  It is derived from the glcd_Device class that impliments the protocol for sending and
  receiving data and commands to a GLCD device
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

*/

/**
 * @mainpage Arduino Graphical LCD Library
 *
 * @section sec_intro Introduction
 *
 * This library supports the use of Graphical LCDs (GLCD) for use on Arduino.
 * This is an extensive modification of the ks0108 library that supports more
 * chips and is easier to integrate with different panels.
 * The graphical functions are backwards compatible (except where noted) so existing sketches
 * should work with little or no modification to the code.  The configuration mechanism has been
 * changed to facilitate use with a broad range of GLCD chips and ATmega controllers so the
 * configuration information will need modification to work with the new library. 
 */

#ifndef	GLCD_H
#define GLCD_H

#include <inttypes.h>
#include <avr/pgmspace.h>

#include "include/glcd_Device.h"
#include "include/gText.h"


#define GLCD_VERSION 3 // software version of this library

// useful user contants
#define NON_INVERTED false
#define INVERTED     true

class glcd : public glcd_Device  // glcd_Device has low level device access routines
{
  private:
	void ClearPage(uint8_t page, uint8_t startX, uint8_t length, uint8_t color = WHITE); 
	void write(uint8_t c);  // character output for print base class
  public:
    glcd();
	
	// Control functions
	void Init(uint8_t invert = NON_INVERTED);
	void SetDisplayMode(uint8_t mode); //NON_INVERTED or INVERTED,   was SetInverted(uint8_t invert);
	
	// Graphic Functions
	void ClearScreen(uint8_t color = WHITE);
	void ClearPage(uint8_t page, uint8_t color = WHITE); // bill, this was public in ks0108, why did we make it private?
	void DrawVLine(uint8_t x, uint8_t y, uint8_t height, uint8_t color= BLACK);
	void DrawHLine(uint8_t x, uint8_t y, uint8_t width, uint8_t color= BLACK);
    void DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color= BLACK);
    void DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color= BLACK);
	void DrawRoundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t color= BLACK);
    void FillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color= BLACK);
    void InvertRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
	void DrawCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius, uint8_t color= BLACK);	
	void FillCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius, uint8_t color= BLACK);	
	void DrawBitmap(const uint8_t * bitmap, uint8_t x, uint8_t y, uint8_t color= BLACK);
	
	// Text class
	gText Text; // making this class public allows access to undocumented functions, not sure if we should allow that.
	

/// @cond hide_from_doxygen

    // Font Functions - this is public API for functionality in the Text class:

    void SelectFont(const uint8_t* font, uint8_t color=BLACK); // default arguments added, callback removed from public method
	void SetFontColor(uint8_t color); // new method to change the selected fonts color
	void SetTextMode(textMode mode);  // change to the given text mode (currently only scroll direction is supported)
    void Puts_P(PGM_P str);  // this prints a string defined explicitly as a constant in program memory

	void CursorTo( uint8_t column, uint8_t row); // 0 based coordinates for character columns and rows
	void GotoXY(uint8_t x, uint8_t y);  // overrride for GotoXY in device class
	void CursorToXY( uint8_t x, uint8_t y); // pixel coordinates 
	void EraseTextLine( eraseLine_t type=eraseFROM_EOL); // default erases from the end of line
	void EraseTextLine( uint8_t row); // erase the entire text line in the given row    
	// void ClearSysTextLine( uint8_t row); // note the old ClearSysTextLine behaviour is supported using glcd_Deprecated.h
	
	// legacy text output functions 
	int PutChar(char c);
    void Puts(char* str);
	void PrintNumber(long n);

    uint8_t CharWidth(char c);
    uint16_t StringWidth(const char* str);
    uint16_t StringWidth_P(PGM_P str);
/// @endcond

	//Device Properties - these are read only constants	 
	static const uint8_t Width = DISPLAY_WIDTH;
	static const uint8_t Height = DISPLAY_HEIGHT;
	static const uint8_t Right = DISPLAY_WIDTH-1;
	static const uint8_t Bottom = DISPLAY_HEIGHT-1;
	static const uint8_t CenterX = DISPLAY_WIDTH/2;
	static const uint8_t CenterY = DISPLAY_HEIGHT/2;
	
};

extern glcd GLCD;   
#endif
