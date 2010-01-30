/*
  gText.cpp - Support for Text output on a grpahical device 
  The routines in this file are Copyright (c) Bill Perry 2009

  vi:ts=4
  
  This version is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
*/

#include <avr/pgmspace.h>
#include "include/gText.h"
#include "glcd_Config.h" 


//    gText::gText(glcd_Device* _device){
//      this->device = _device;

gText::gText()
{
}

void gText::Init(glcd_Device* _device)
{
    this->device = _device;

	/*
	 * Set up default/active text area to be entire display.
	 * with up/normal scrolling
	 * This is used for wrapping and scrolling.
	 */

	this->tarea_active = 0;

	this->tarea.x1 = 0;
	this->tarea.y1 = 0;
	this->tarea.x2 = DISPLAY_WIDTH -1;
	this->tarea.y2 = DISPLAY_HEIGHT -1;
	this->tarea.scrolldir = 1;
}

/**
 * Select/Switch to desired text area
 *
 * @param area is the text area number
 * @see DefineArea()
 * @see ClearArea()
 */
void gText::SelectArea(uint8_t area)
{
uint8_t area_active = this->tarea_active;

	/*
	 * Sanity check area value.
	 */

	if(area >= GLCD_TAREA_CNT)
		return;

	if(area != area_active)
	{
		/*
		 * Changing area, so save off current area context
		 */

		this->tarea_cntxt[area_active].tarea = this->tarea; /* struct assignment */
		this->tarea_cntxt[area_active].FontRead = this->FontRead;
		this->tarea_cntxt[area_active].FontColor = this->FontColor;
		this->tarea_cntxt[area_active].Font = this->Font;
		this->tarea_cntxt[area_active].x = device->Coord.x;
		this->tarea_cntxt[area_active].y = device->Coord.y;

		/*
		 * Now restore context of desired area
		 */
		this->tarea = this->tarea_cntxt[area].tarea;	/* struct assignment */
		this->FontRead = this->tarea_cntxt[area].FontRead;
		this->FontColor = this->tarea_cntxt[area].FontColor;
		this->Font = this->tarea_cntxt[area].Font;

		/*
		 * Now send LCD to back to location where it was when area was last selected.
		 *
		 * WARNING: WARNING:
		 *
		 * The code below is intentionally done as two steps.
		 *
		 * The first step sets up the previous s/w X & Y coordinates.
		 * The second step calls the GotoXY() function to set the X & Y position
		 * in the lcd hardware.
		 *
		 * Normally just calling the GotoXY() routine would be good enough
		 * as that routine also saves the s/w X & Y coordinates, however,
		 * There are certain allowed conditions were the X coordinate will
		 * be outsided the legal X value for the LCD display.
		 *
		 * Summary is that whenever you write to the lcd page of the right most 
		 * column, the  s/w X coordinate will bump beyond the hardware boundary.
		 * This is intentional.
		 * See the WriteData() function for details on this condition.
		 *
		 * Whenever GotoXY() sees invalid coordinates, it tosses the request.
		 * So in order ensure that the s/w coordinates are set in cases when
		 * they are "technically" invalid, you have to manually set them.
		 *
		 * So rather than check their validity and only assign them in the invalid case,
		 * it easiest to always set them just prior to the GotoXY().
		 *
		 */


		device->Coord.x = this->tarea_cntxt[area].x;
		device->Coord.y = this->tarea_cntxt[area].y;

		/*
		 * New font rendering always does a GotoXY() up front so there is
		 * no need to load the LCD h/w right here.
		 */

#ifndef GLCD_NEW_FONDDRAW
		device->GotoXY(device->Coord.x, device->Coord.y);
#endif

		/*
		 * keep track of active area.
		 */

		this->tarea_active = area;
	}
}

/**
 * Clear the active text area with the current font background color
 * and home the cursor to upper left corner.
 *
 * @see DefineArea()
 * @see SelectArea()
 */
void gText::ClearArea(void)
{
	/*
	 * fill the area with font background color
	 */

	device->SetPixels(this->tarea.x1, this->tarea.y1, 
		this->tarea.x2, this->tarea.y2, 
			this->FontColor == BLACK ? WHITE : BLACK);
	/*
	 * put cursor at home position of text area to ensure we are always inside area.
	 */

	CursorToXY(0,0);
}

/**
 * Define a Text area by columns and rows
 *
 * @param area the desired text area (0 to GLCD.Text.AreaCount)
 * @param x X coordinate of upper left corner
 * @param y Y coordinate of upper left corner
 * @param columns number of text columns
 * @param rows number of text rows
 * @param font a pointer defined in a font defintion file
 * @param scrolldir	<0 it scrolls down/reverse, >0 up/normal
 *
 *
 * Defines a text area sized to hold columns characters across and rows characters tall.
 * It is properly sized for the specified font.
 * 
 * The area within the newly defined text area is intentionally not cleared.
 *
 * While intended for fixed width fonts, sizing will work for variable
 * width fonts.
 *
 * When variable width fonts are used, the column is based on assuming a width
 * of the widest character.
 *
 * x,y is an absolute coordinate and is relateive to the 0,0 origin of the
 * display.
 *
 * scrolldir is an optional parameter and defaults to normal/up
 *
 * @note
 * Upon creation of the text area, the cursor position for the text area will be set to x,y
 * While the x,y position of the text area being defined will be set to x,y 
 * if the text are being defined is different from the currently active text postion, the
 * x,y postion of the currently active text area will not be modified.
 *
 *
 * @see ClearArea()
 * @see SelectArea()
 */

uint8_t
gText::DefineArea(uint8_t area, uint8_t x, uint8_t y, uint8_t columns, uint8_t rows, const uint8_t* font, int8_t scrolldir)
{
uint8_t arearval;
uint8_t x2,y2;
uint8_t active_area;

	active_area = this->tarea_active;
	
	/*
	 * Because the font read routines only work on the "active" variables,
	 * the area about to defined has to be selected before it is sized.
	 */

	/*
	 * Now fill in font information
	 */

	this->SelectArea(area);
	this->SelectFont(font);

	x2 = x + columns * (this->FontRead(this->Font+FONT_FIXED_WIDTH)+1) -1;
	y2 = y + rows * (this->FontRead(this->Font+FONT_HEIGHT)+1) -1;

	arearval = DefineArea(area, x,y, x2, y2, scrolldir);

	/*
	 * Restore which ever area was previously active
	 */
	this->SelectArea(active_area);

	return(arearval);
}

/**
 * Define a text area by absolute coordinates
 *
 * @param area the desired text area (0 to GLCD.Text.AreaCount)
 * @param x1 X coordinate of upper left corner
 * @param y1 Y coordinate of upper left corner
 * @param x2 X coordinate of lower right corner
 * @param y2 Y coordinate of lower right corner
 * @param	scrolldir	<0 it scrolls down/reverse, >0 up/normal
 *
 * Defines a text area based on absolute coordinates.
 * The pixel coordinates for the text area are inclusive so x2,y2 is the lower right
 * pixel of the text area.
 *
 * x1,y1 and x2,y2 are an absolute coordinates and are relateive to the 0,0 origin of the
 * display.
 *
 * The area within the newly defined text area is intentionally not cleared.
 *
 * scrolldir is an optional parameter and defaults to normal/up
 *
 * @return returns @em area if successful.
 *
 *
 * @note
 * Upon creation of the text area, the cursor position for the text area will be set to x,y
 * While the x,y position of the text area being defined will be set to x,y 
 * if the text are being defined is different from the currently active text postion, the
 * x,y postion of the currently active text area will not be modified.
 *
 * @see ClearArea()
 * @see SelectArea()
 *
 */

uint8_t
gText::DefineArea(uint8_t area, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, int8_t scrolldir)
{

	if(area >= GLCD_TAREA_CNT)
		return (area | 0x80); /* return something other than area */

	/*
	 * Sanity Check Params
	 * FIXME
	 *	What should be done for invalid params?
	 *		- Silently, give them the entire display?
	 *		- Ignore it?
	 *		- Correct bad/invalid params?
	 *		- return a bad status?
	 *		- Give them entire display and slam a
	 *			mssage to the LCD?
	 *
	 *	For now, you silently get the entire LCD
	 *  on errors.
	 *
	 */


	if(		(x1 >= x2)
		||	(y1 >= y2)
		||	(x1 >= DISPLAY_WIDTH)
		||	(y1 >= DISPLAY_HEIGHT)
		||	(x2 >= DISPLAY_WIDTH)
		||	(y2 >= DISPLAY_WIDTH)
	)
	{
		x1 = 0;
		y1 = 0;
		x2 = DISPLAY_WIDTH -1;
		y2 = DISPLAY_HEIGHT -1;
	}

	/*
	 * If the area is not the active active area
	 * fudge up the areas context information so that its
	 * ready to be selected.
	 * The cursor position for the area will be set to the
	 * upper left corner of the text area.
	 */
	if(area != this->tarea_active)
	{
		this->tarea_cntxt[area].tarea.x1 = x1;
		this->tarea_cntxt[area].tarea.y1 = y1;
		this->tarea_cntxt[area].tarea.x2 = x2;
		this->tarea_cntxt[area].tarea.y2 = y2;
		this->tarea_cntxt[area].tarea.scrolldir = scrolldir;
		/*
		 * set cursor position for the area
		 */
		this->tarea_cntxt[area].x = x1;
		this->tarea_cntxt[area].y = y1;
	}
	else
	{
		this->tarea.x1 = x1;
		this->tarea.y1 = y1;
		this->tarea.x2 = x2;
		this->tarea.y2 = y2;
		this->tarea.scrolldir = scrolldir;
		device->GotoXY(x1, y1);
	}
	return(area);
}

/**
 * Define a predefined generic text area
 *
 * @param area the desired text area (0 to GLCD.Text.AreaCount)
 * @param selection one of: textAreaFULL, textAreaTOP,  textAreaBOTTOM, textAreaLEFT, textAreaRIGHT,
 *                                textAreaTOPLEFT,textAreaTOPRIGHT,textAreaBOTTOMLEFT,textAreaBOTTOMRIGHT
 * @param	scrolldir	<0 it scrolls down/reverse, >0 up/normal
 *
 * Defines a pre-selected text area
 *
 * The area within the newly defined text area is intentionally not cleared.
 *
 * scrolldir is an optional parameter and defaults to normal/up
 *
 * @return returns @em area if successful.
 *
 *
 * @note
 * Upon creation of the text area, the cursor position for the text area will be set to 
 * the upper left coordinate of the given preselected area
 *
 * @see ClearArea()
 * @see SelectArea()
 *
 */

uint8_t
gText::DefineArea(uint8_t area, predefinedArea selection, int8_t scrolldir)
{
uint8_t x1,y1,x2,y2;
TareaToken tok;

	tok.token = selection;

	x1 =  tok.coord.x1;
	y1 =  tok.coord.y1;
	x2 =  tok.coord.x2;
	y2 =  tok.coord.y2;

	return DefineArea(area,x1,y1,x2,y2, scrolldir);
}

/*
 * Scroll a pixel region up.
 * 	Area scrolled is defined by x1,y1 through x2,y2 inclusive.
 *  x1,y1 is upper left corder, x2,y2 is lower right corner.
 *
 *	color is the color to be used for the created space along the
 *	bottom.
 *
 *	pixels is the *exact* pixels to scroll. 1 is 1 and 9 is 9 it is
 *  not 1 less or 1 more than what you want. It is *exact*.
 */

void gText::ScrollUp(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, 
	uint8_t pixels, uint8_t color)
{
uint8_t dy;
uint8_t dbyte;
uint8_t sy;
uint8_t sbyte;
uint8_t col;

	/*
	 * Scrolling up more than area height?
	 */
	if(y1 + pixels > y2)
	{
		/*
		 * fill the region with "whitespace" because
		 * it is being totally scrolled out.
		 */
		device->SetPixels(x1, y1, x2, y2, color);
		return;
	}

	for(col = x1; col <= x2; col++)
	{
		dy = y1;
		device->GotoXY(col, dy & ~7);
		dbyte = device->ReadData();


		/*
		 * preserve bits outside/above scroll region
		 */

		dbyte &= (_BV((dy & 7)) - 1);

		sy = dy + pixels;
		device->GotoXY(col, sy & ~7);
		sbyte = device->ReadData();

		while(sy <= y2)
		{
			if(sbyte & _BV(sy & 7))
			{
				dbyte |= _BV(dy & 7);
			}

			sy++;
			if((sy & 7) == 0)
			{
				/*
				 * If we just crossed over, then we should be done.
				 */
				if(sy < DISPLAY_HEIGHT)
				{
					device->GotoXY(col, sy & ~7);
					sbyte = device->ReadData();
				}
			}

			if((dy & 7) == 7)
			{
				device->GotoXY(col, dy & ~7);	// Should be able to remove this
				device->WriteData(dbyte);
				dbyte = 0;
			}
			dy++;
		}

		/*
		 * Handle the new area at the bottom of the region
		 */

		for(uint8_t p = pixels; p; p--)
		{
			if(color == BLACK)
			{
				dbyte |= _BV(dy & 7);
			}
			else
			{
				dbyte &= ~_BV(dy & 7);
			}

			if((dy & 7) == 7)
			{
				device->GotoXY(col, dy & ~7); // should be able to remove this.
				device->WriteData(dbyte);
				dbyte = 0;
			}
			dy++;
		}

		/*
		 * Flush out the final destination byte
		 */


		if(dy & 7)
		{
			dy--;

			device->GotoXY(col, dy & ~7);
			sbyte = device->ReadData();
			/*
			 * Preserver bits outside/below region
			 */

			dy++;
			sbyte &= ~(_BV((dy & 7)) - 1);
			dbyte |= sbyte;

			device->WriteData(dbyte);
		}
	}

}

#ifndef GLCD_NO_SCROLLDOWN

void gText::ScrollDown(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, 
	uint8_t pixels, uint8_t color)
{
uint8_t dy;
uint8_t dbyte;
uint8_t sy;
uint8_t sbyte;
uint8_t col;

	/*
	 * Scrolling up more than area height?
	 */
	if(y1 + pixels > y2)
	{
		/*
		 * fill the region with "whitespace" because
		 * it is being totally scrolled out.
		 */
		device->SetPixels(x1, y1, x2, y2, color);
		return;
	}

	/*
	 * Process region from left to right
	 */
	for(col = x1; col <= x2; col++)
	{
		dy = y2;
		device->GotoXY(col, dy & ~7);
		dbyte = device->ReadData();

		/*
		 * preserve bits outside/below scroll region
		 */

		dbyte &= ~(_BV(((dy & 7)+1)) - 1);
		sy = dy - pixels;
		device->GotoXY(col, sy & ~7);
		sbyte = device->ReadData();

		while(sy >= y1)
		{
			if(sbyte & _BV(sy & 7))
			{
				dbyte |= _BV(dy & 7);
			}
			if((dy & 7) == 0)
			{
				device->GotoXY(col, dy & ~7);	// Should be able to remove this
				device->WriteData(dbyte);
				dbyte = 0;
			}
			dy--;

			if(!sy)
				break; /* if we bottomed out, we are done */
			sy--;
			if((sy & 7) == 7)
			{
				device->GotoXY(col, sy & ~7);
				sbyte = device->ReadData();
			}

		}

		/*
		 * Handle the new area at the top of the column
		 */

		for(uint8_t p = pixels; p; p--)
		{
			if(color == BLACK)
			{
				dbyte |= _BV(dy & 7);
			}
			else
			{
				dbyte &= ~_BV(dy & 7);
			}

			if((dy & 7) == 0)
			{
				device->GotoXY(col, dy & ~7); // should be able to remove this.
				device->WriteData(dbyte);
				dbyte = 0;
			}
			dy--;
		}

		dy++; /* point dy back to last destination row */

		/*
		 * Flush out the final destination byte
		 */

		if(dy & 7)
		{
			device->GotoXY(col, dy & ~7);
			sbyte = device->ReadData();
			/*
			 * Preserve bits outside/above region
			 */

			sbyte &= (_BV((dy & 7)) - 1);
			dbyte |= sbyte;
			device->WriteData(dbyte);
		}

	}

}
#endif //GLCD_NO_SCROLLDOWN


/*
 * Handle all special processing characters
 */
void gText::SpecialChar(char c)
{


	if(c == '\n')
	{
		uint8_t x = device->Coord.x;
		uint8_t y = device->Coord.y;
		uint8_t height = this->FontRead(this->Font+FONT_HEIGHT);

//printf("Newline: x: %d y:%d, tarea.x2: %d, tarea.y2: %d, height: %d\n", x, y, tarea.x2, tarea.y2, height);


		/*
		 * Erase all pixels remaining to edge of text area.on all wraps
		 * It looks better when using inverted (WHITE) text, on proportional fonts, and
		 * doing WHITE scroll fills.
		 *
		 * REMEMBER: current drawing routines like FillRect() take
		 * width/height arguments that are 1 less than what they really do.
		 * The width/height arguments below take that into consideration.
		 */

		if(x < this->tarea.x2)
			device->SetPixels(x, y, this->tarea.x2, y+height, this->FontColor == BLACK ? WHITE : BLACK);

		/*
		 * Check for scroll up vs scroll down (scrollup is normal)
		 */
#ifndef GLCD_NO_SCROLLDOWN
		if(this->tarea.scrolldir >= 0)
#endif
		{
			/*
			 * Normal/up scroll
			 */

			/*
			 * Note this comparison and the pixel calcuation below takes into 
			 * consideration that fonts
			 * are atually 1 pixel taller when rendered. 
			 * This extra pixel is along the bottom for a "gap" between the character below.
			 */
			if(y + 2*height >= this->tarea.y2)
			{
//printf("Scroll Required: y: %d, height: %d, tarea.y2: %d\n", y, height, this->tarea.y2);

				/*
				 * forumula for pixels to scroll is:
				 *	(assumes "height" is one less than rendered height)
				 *
				 *		pixels = height - ((this->tarea.y2 - y)  - height) +1;
				 *
				 *		The forumala below is unchanged 
				 *		But has been re-written/simplified in hopes of better code
				 *
				 */

				uint8_t pixels = 2*height + y - this->tarea.y2 +1;
		
				/*
				 * Scroll everything to make room
				 * * NOTE: (FIXME, slight "bug")
				 * When less than the full character height of pixels is scrolled,
				 * There can be an issue with the newly created empty line.
				 * This is because only the # of pixels scrolled will be colored.
				 * What it means is that if the area starts off as white and the text
				 * color is also white, the newly created empty text line after a scroll 
				 * operation will not be colored BLACK for the full height of the character.
				 * The only way to fix this would be alter the code use a "move pixels"
				 * rather than a scroll pixels, and then do a clear to end line immediately
				 * after the move and wrap.
				 *
				 * Currently this only shows up when
				 * there are are less than 2xheight pixels below the current Y coordinate to
				 * the bottom of the text area
				 * and the current background of the pixels below the current text line
				 * matches the text color
				 * and  a wrap was just completed.
				 *
				 * After a full row of text is printed, the issue will resolve itself.
				 * 
				 * 
				 */
				this->ScrollUp(this->tarea.x1, this->tarea.y1, 
					this->tarea.x2, this->tarea.y2, pixels, this->FontColor == BLACK ? WHITE : BLACK);

//printf("New XY: %d, %d\n", this->tarea.x1, this->tarea.y2 - height);
				device->GotoXY(this->tarea.x1, this->tarea.y2 - height);
//kbwait();
			
			}
			else
			{
//printf("Simple Wrap: to %d,%d\n", this->tarea.x1, y + height + 1);
				/*
				 * Room for simple wrap
				 */

				x = this->tarea.x1;
				y = y+height+1;

#ifdef NOTNEEDED
				if(x < this->tarea.x2)
					//device->SetPixels(x, y, this->tarea.x2, y+height, this->FontColor == BLACK ? WHITE : BLACK);
					device->SetPixels(x, y, this->tarea.x2, y+height, WHITE);
#endif

				device->GotoXY(x, y);

			}
		}
#ifndef GLCD_NO_SCROLLDOWN
		else
		{
			/*
			 * Reverse/Down scroll
			 */

			/*
			 * Check for Wrap vs scroll.
			 *
			 * Note this comparison and the pixel calcuation below takes into 
			 * consideration that fonts
			 * are atually 1 pixel taller when rendered. 
			 *
			 */
			if(y > this->tarea.y1 + height)
			{
				/*
				 * There is room so just do a simple wrap
				 */
				x = this->tarea.x1;
				y = y - (height+1);
#ifdef NOTNEEDED
				if(x < this->tarea.x2)
					//device->SetPixels(x, y, this->tarea.x2, y+height, this->FontColor == BLACK ? WHITE : BLACK);
					device->SetPixels(x, y, this->tarea.x2, y+height, WHITE);
#endif

				device->GotoXY(x, y);
			}
			else
			{
				/*
				 * Scroll down everything to make room for new line
				 *	(assumes "height" is one less than rendered height)
				 */

				uint8_t pixels = height+1 - (this->tarea.y1 - y);

				this->ScrollDown(this->tarea.x1, this->tarea.y1, 
					this->tarea.x2, this->tarea.y2, pixels, this->FontColor == BLACK ? WHITE : BLACK);

//printf("New XY: %d, %d\n", this->tarea.x1, this->tarea.y1);
				device->GotoXY(this->tarea.x1, this->tarea.y1);
			}
		}
#endif
	}

}

/**
 * output a character to the currently selected text area
 *
 * @param c the character to output
 *
 * If the character will not fit on the current text line
 * inside the currently selected text area,
 * the text position is wrapped to the next line. This might be
 * the next lower or the next higher line depending on the
 * scroll direction.
 *
 * If there is not enough room to fit a full line of new text after
 * wrapping, the entire text area will be scrolled to make room for a new
 * line of text. The scroll direction will be up or down
 * depending on the scroll direction for the text area.
 * 
 * @see Puts()
 * @see Puts_P()
 * @see write()
 */

int gText::PutChar(char c)
{

//printf("PutChar: <0x%x>\n", c);
//uint8_t waitflg = 0; // BAPDEBUG


    if(this->FontRead == 0)
	  return 0; // no font selected

	/*
	 * check for special character processing
	 */

	if(c < 0x20)
	{
		SpecialChar(c);
		return 1;
	}
	   
	uint8_t width = 0;
	uint8_t height = this->FontRead(this->Font+FONT_HEIGHT);
	uint8_t bytes = (height+7)/8; /* calculates height in rounded up bytes */
	
	uint8_t firstChar = this->FontRead(this->Font+FONT_FIRST_CHAR);
	uint8_t charCount = this->FontRead(this->Font+FONT_CHAR_COUNT);
	
	uint16_t index = 0;
	uint8_t x = device->Coord.x, y = device->Coord.y;


	if(c < firstChar || c >= (firstChar+charCount)) {
		return 0; // invalid char
	}
	c-= firstChar;

	if( isFixedWidtFont(this->Font) {
	   width = this->FontRead(this->Font+FONT_FIXED_WIDTH); 
	   index = c*bytes*width+FONT_WIDTH_TABLE;
	}
	else{
	// variable width font, read width data, to get the index
		/*
		 * Because there is no table for the offset of where the data
		 * for each character glyph starts, run the table and add up all the
		 * widths of all the characters prior to the character we
		 * need to locate.
		 */
	   for(uint8_t i=0; i<c; i++) {  
		 index += this->FontRead(this->Font+FONT_WIDTH_TABLE+i);
	   }
		/*
		 * Calculate the offset of where the font data
		 * for our character starts.
		 * The index value from above has to be adjusted because
		 * there is potentialy more than 1 byte per column in the glyph,
		 * when the characgter is taller than 8 bits.
		 * To account for this, index has to be multiplied
		 * by the height in bytes because there is one byte of font
		 * data for each vertical 8 pixels.
		 * The index is then adjusted to skip over the font width data
		 * and the font header information.
		 */

	   index = index*bytes+charCount+FONT_WIDTH_TABLE;

		/*
		 * Finally, fetch the width of our character
		 */
	   width = this->FontRead(this->Font+FONT_WIDTH_TABLE+c);
    }

	/*
	 * If the character won't fit in the text area,
	 * fake a newline to get the text area to wrap and 
	 * scroll if necessary.
	 * NOTE/WARNING: the below calculation assumes a 1 pixel pad.
	 * This will need to be changed if/when configurable pixel padding is supported.
	 */
	if(x + width > this->tarea.x2)
	{
		this->PutChar('\n'); // fake a newline to cause wrap/scroll
		/*
		 * Re-fetch x/y coordinates after wrap/scroll.
		 */
		x = device->Coord.x;
		y = device->Coord.y;

		//waitflg++; // BAPDEBUG
		
	}

	// last but not least, draw the character

#ifndef GLCD_NEW_FONTDRAW
/*================== OLD FONT DRAWING ============================*/
	/*
	 * Draw each column of the glyph (character) horizontally
	 * 8 bits (1 page) at a time.
	 * i.e. if a font is taller than 8 bits, draw upper 8 bits first,
	 * Then drop down and draw next 8 bits and so on, until done.
	 * This code depends on WriteData() doing writes that span LCD
	 * memory pages, which has issues because the font data isn't
	 * always a multiple of 8 bits.
	 */

	for(uint8_t i=0; i<bytes; i++)	/* each vertical byte */
	{
		uint8_t page = i*width;
		for(uint8_t j=0; j<width; j++) /* each column */
		{
			uint8_t data = this->FontRead(this->Font+index+page+j);
		
			/*
			 * This funkyness is because when the character glyph is not a
			 * multiple of 8 in height, the residual bits in the font data
			 * were aligned to the incorrect end of the byte with respect 
			 * to the GLCD. I believe that this was an initial oversight (bug)
			 * in Thieles font creator program. It is easily fixed
			 * in the font program but then creates a potential backward
			 * compatiblity problem.
			 *	--- bperrybap
			 */

			if(height > 8 && height < (i+1)*8)	/* is it last byte of multibyte tall font? */
			{
				data >>= (i+1)*8-height;
			}
			
			if(this->FontColor == BLACK) {
				device->WriteData(data);
			} else {
				device->WriteData(~data);
			}
		}
		// 1px gap between chars
		if(this->FontColor == BLACK) {
			device->WriteData(0x00);
		} else {
			device->WriteData(0xFF);
		}
		device->GotoXY(x, device->Coord.y+8);
	}
	device->GotoXY(x+width+1, y);

/*================== END of OLD FONT DRAWING ============================*/
#else

/*================== NEW FONT DRAWING ===================================*/

	/*
	 * Paint font data bits and write them to LCD memory 1 LCD page at a time.
	 * This is very different from simply writing 1 page of font data
	 * bits to LCD memory.
	 */

//printf("NewFontDraw: c:<%c>, 0x%x> Font: %d (0x%x)\n", c, c, this->Font, this->Font);

	uint8_t pixels = height +1; /* 1 for gap below character*/
	uint8_t p;
	uint8_t dy;
	uint8_t tfp;
	uint8_t dp;
	uint8_t dbyte;
	uint8_t fdata;

//printf("painting char: %d,%d  pixels: %d\n", x, y, pixels);

	for(p = 0; p < pixels;)
	{
		dy = y + p;

//printf("pixel loop: dy: %d, y: %d, p: %d\n", dy, y, p);

		/*
		 * Align to proper Column and page in LCD memory
		 */

		device->GotoXY(x, (dy & ~7));

		uint8_t page = p/8 * width;

		for(uint8_t j=0; j<width; j++) /* each column of font data */
		{
			
			/*
			 * Fetch proper byte of font data.
			 * Note:
			 * This code "cheats" to add the horizontal space/pixel row
			 * below the font.
			 * It essentially creates a font pixel of 0 when the pixels are
			 * out of the defined pixel map.
			 *
			 * fake a fondata read read when we are on the very last
			 * bottom "pixel". This lets the loop logic continue to run
			 * with the extra fake pixel. If the loop is not the
			 * the last pixel the pixel will come from the actual
			 * font data, but that is ok as it is 0 padded.
			 *
			 */

			if(p >= height)
			{
				/*
				 * fake a font data read for padding below character.
				 */
				fdata = 0;
			}
			else
			{
				fdata = this->FontRead(this->Font+index+page+j);

				/*
				 * Have to shift font data because Thiele shifted residual
				 * font bits the wrong direction for LCD memory.
				 *
				 * FIXME
				 * NOTE: the check for height> 8 is because the system font
				 * is not in thiele format. So this is a hack that treats
				 * fonts with height less than 8 to be "fixed" font format
				 * rather than thiele format.
				 * This really should use !isFixedWidtFont(this->Font)
				 * instead. but it will be a little slower.
				 *
				 * The real solution to this is fix font format to
				 * not shift the residual bits the wrong direction!!!!
				 */
				if((height > 8) && (height - (p&~7)) < 8)
				{
					fdata >>= 8 - (height & 7);
//printf("Shifting font data: %d, p: %d\n", (8 - (height&7)), p);

				}
			}

			if(this->FontColor == WHITE)
				fdata ^= 0xff;	/* inverted data for "white" font color	*/


			/*
			 * Check to see if a quick full byte write of font
			 * data can be done.
			 */

			if(!(dy & 7) && !(p & 7) && ((pixels -p) >= 8))
			{
				/*
				 * destination pixel is on a page boundary
				 * Font data is on byte boundary
				 * And there are 8 or more pixels left
				 * to paint so a full byte write can be done.
				 */

//printf("no paint 8 bit write: j: %d p, %d, y: %d, dy: %d\n", j, p, y, dy);
					
					device->WriteData(fdata);
					continue;
			}
			else
			{
					/*
					 * No, so must fetch byte from LCD memory.
					 */
//printf("Reading LCD data, %d,%d\n", device->Coord.x, device->Coord.y);
					dbyte = device->ReadData();
			}

			/*
			 * At this point there is either not a full page of data
			 * left to be painted  or the font data spans multiple font
			 * data bytes. (or both) So, the font data bits will be painted
			 * into a byte and then written to the LCD memory.page.
			 */


			tfp = p;		/* font pixel bit position 		*/
			dp = dy & 7;	/* data byte pixel bit position */

//printf("painting: j: %d dy: %d, p: %d, tfp %d, dp: %d\n", j, dy, p, tfp, dp);

			/*
			 * paint bits until we hit bottom of page/byte
			 * or run out of pixels to paint.
			 */
			while((dp <= 7) && (tfp) < pixels)
			{
//printf("paintloop: dp: %d, p: %d tfp: %d\n", dp, p, tfp);
				if(fdata & _BV(tfp & 7))
				{
					dbyte |= _BV(dp);
				}
				else
				{
					dbyte &= ~_BV(dp);
				}

				/*
				 * Check for crossing font data bytes
				 */
				if((tfp & 7)== 7)
				{
//printf("paintloop: fetching new font byte, page: %d j: %d\n", page, j);
					fdata = this->FontRead(this->Font+index+page+j+width);

					/*
					 * Have to shift font data because Thiele shifted residual
					 * font bits the wrong direction for LCD memory.
					 *
					 * Again, the height >8 is a hack for the system font.
					 * and probably should be replaced with
				 	 * !isFixedWidtFont(this->Font)
					 *
					 */

					if((height > 8) && ((height - tfp) < 8))
					{
						fdata >>= (8 - (height & 7));
//printf("paintloop: Shifting font data: %d, p: %d, tfp: %d\n", (8 - (height&7)), p, tfp);
					}

					if(this->FontColor == WHITE)
						fdata ^= 0xff;	/* inverted data for "white" color	*/
				}
				tfp++;
				dp++;
			}

			/*
			 * Now flush out the painted byte.
			 */
//printf("Flush painted byte %d,%d 0x%x\n", device->Coord.x, device->Coord.y, dbyte);

			device->WriteData(dbyte);
		}
//printf("adding H gap: dy: %d p: %d, dp: %d\n", dy, p, dp);


		/*
		 * now create a horizontal gap (vertical line of pixels) between characters.
		 * Since this gap is "white space", the pixels painted are oposite of the
		 * font color.
		 *
		 * Since full LCD pages are being written, there are 4 combinations of filling
		 * in the this gap page.
		 *	- pixels start at bit 0 and go down less than 8 bits
		 *	- pixels don't start at 0 but go down through bit 7
		 *	- pixels don't start at 0 and don't go down through bit 7 (fonts shorter than 6 hi)
		 *	- pixels start at bit 0 and go down through bit 7 (full byte)
		 *
		 * The code below creates a mask of the bits that should not be painted.
		 *
		 * Then it is easy to paint the desired bits since if the color is WHITE,
		 * the paint bits are set, and if the coloer is not WHITE the paint bits are stripped.
		 * and the paint bits are the inverse of the desired bits mask.
		 */


		
		if((dy & 7) || (pixels - p < 8))
		{
		uint8_t mask = 0;

			dbyte = device->ReadData();

			if(dy & 7)
				mask |= _BV(dy & 7) -1;

			if((pixels-p) < 8)
				mask |= ~(_BV(pixels - p) -1);


			if(this->FontColor == WHITE)
				dbyte |= ~mask;	
			else
				dbyte &= mask;

		}
		else
		{
			if(this->FontColor == WHITE)
				dbyte = 0xff;
			else
				dbyte = 0;
		}

//printf("H gap write %d,%d 0x%x\n", device->Coord.x, device->Coord.y, dbyte);

		device->WriteData(dbyte);

		/*
		 * advance the font pixel for the pixels
		 * just painted.
		 */

		p += 8 - (dy & 7);

//printf("Bump p by %d\n", 8  - (dy & 7));
//printf("New p: %d\n", p);

		
	}


	/*
	 * At this point the X position has been advanced by writing the pages
	 * so all that is necessary is to set the y position back to the upper pixel
	 * where we started, to get ready for the next character.
	 *
	 * Since this rendering code always starts off with a GotoXY() it really isn't necessary
	 * to do a real GotoXY() to set the h/w location. We can get away with only setting
	 * the s/w version of X & Y.
	 *
	 * NOTE/WARNING:
	 * DO NOT change the code below to use the function GotoXY() to set the Y position.
	 * Doing so, will break a specific wrap condition.
	 * If the last page written is exactly the right most page of the display, the s/w X
	 * postion will be beyond the valid limit and so a GotoXY() will fail.
	 * An illegal s/w X value is intentional.
	 * See the WriteData() routine for details.
	 *
	 * So to get around this issue, this code simply patches the s/w verions of the Y
	 * value so that the next character written, will set it.
	 *
	 * While ugly, it is necessary and actually does save a few cycles over GotoXY()
	 */

	device->Coord.y = y;

/*================== END of NEW FONT DRAWING ============================*/

#endif // NEW_FONTDRAW

//if(waitflg)
//	kbwait();

	return 1; // valid char
}

/**
 * output a character string
 *
 * @param str pointer to a null terminated character string.
 *
 * Outputs all the characters in the string to the currently
 * selected text area. 
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 * @see PutChar()
 * @see Puts_P()
 * @see write()
 * @see DrawString()
 */


void gText::Puts(char* str)
{
    while(*str)
	{
        this->PutChar(*str);
        str++;
    }
}

/**
 * output a program memory character string
 *
 * @param str pointer to a null terminated character string stored in program memory
 *
 * Outputs all the characters in the string to the currently
 * selected text area. 
 * See PutChar() for a full description of how characters are
 * written to the text area.
 *
 * @see PutChar()
 * @see Puts()
 * @see write()
 * @see DrawString()
 */

void gText::Puts_P(PGM_P str)
{
char c;

    while((c = pgm_read_byte(str)) != 0)
	{
        this->PutChar(c);
        str++;
    }
}

/**
 * Draw a character string at a specified position
 *
 * @param str pointer to a null terminated character string
 * @param x X coordinate of first character position
 * @param x Y coordinate of first character position
 *
 * The character string is drawn in the currently selected
 * text area at position x,y.
 * x & y are relative the the upper left corner of the currently
 * selected text area.
 *
 * This function is the equivalent of calling CursorToXY() follow by
 * Puts()
 *
 * @see PutChar()
 * @see Puts()
 * @see Puts_P()
 * @see write()
 * @see DrawSrring_P()
 *
 */

void gText::DrawString(char *str, uint8_t x, uint8_t y)
{
	this->CursorToXY(x, y);
	this->Puts(str);
}
/**
 * Draw a program memory based character string at a specified position
 *
 * @param str pointer to a null terminated character string stored in program memory
 * @param x X coordinate of first character position
 * @param y Y coordinate of first character position
 *
 * The program memory based character string is drawn in the currently selected
 * text area at position x,y.
 * x & y are relative the the upper left corner of the currently
 * selected text area.
 *
 * This function is the equivalent of calling CursorToXY() follow by
 * Puts_P()
 *
 * @see PutChar()
 * @see Puts()
 * @see Puts_P()
 * @see write()
 * @see DrawSrring()
 *
 */

void gText::DrawString_P(PGM_P str, uint8_t x, uint8_t y)
{
	this->CursorToXY(x, y);
	this->Puts_P(str);
}

/**
 * Positions cursor to a character based column and row.
 *
 * @param column specifies the horizontal position 
 * @param row  specifies the vertical position
 *
 *	Column and Row are zero based character positions
 *	and are relative the the upper left corner of the currently
 *	selected text area base on the size of the currently selected font.
 *
 * While intended for fixed width fonts, positioning will work for variable
 * width fonts.
 *
 * When variable width fonts are used, the column is based on assuming a width
 * of the widest character.
 *
 * @see CursorToXY()
 */

void gText::CursorTo( uint8_t column, uint8_t row)
{

	/*
	 * Text position is relative to current text area
	 */
  device->GotoXY( column * (this->FontRead(this->Font+FONT_FIXED_WIDTH)+1) + this->tarea.x1,
	       row * (this->FontRead(this->Font+FONT_HEIGHT)+1) + this->tarea.y1
		 ) ; 
}

/**
 * Positions cursor to a X,Y position
 *
 * @param x specifies the horizontal locaion
 * @param y  specifies the vertical locaion
 *
 *	X & Y are zero based pixel coordinates and are relative to 
 *	the upper left corner of the currently selected text area.
 *
 * @see CursorTo()
 */

void gText::CursorToXY( uint8_t x, uint8_t y)
{

	/*
	 * Text position is relative to current text area
	 */
	device->GotoXY(this->tarea.x1 +x, this->tarea.y1 + y);
}

/**
 * Erase in Line
 *
 * @param type type of line erase
 *
 * @arg 0 Erase from cursor to end of line
 * @arg 1 Erase from cursor to begining of line
 * @arg 2 Erase entire line
 *
 * Erases all or part of a line of text depending on the type
 * of erase specified.

 * If type is not specified it is assumed to be 0
 *
 * The cursor position does not change.
 *
 * @see ClearArea()
 */

void gText::EraseInLine(uint8_t type)
{

	uint8_t x = device->Coord.x;
	uint8_t y = device->Coord.y;
	uint8_t height = this->FontRead(this->Font+FONT_HEIGHT);
	uint8_t color = (this->FontColor == BLACK) ? WHITE : BLACK;

	switch(type)
	{
		/*
		 * These numbers are based on the ANSI EraseInLine terminal primitive: CSI n K
		 */
		case 0:
				device->SetPixels(x, y, this->tarea.x2, y+height, color);
				break;
		case 1:
				device->SetPixels(this->tarea.x1, y, x, y+height, color);
				break;
		case 2:
				device->SetPixels(this->tarea.x1, y, this->tarea.x2, y+height, color);
				break;
	}

	/*
	 * restore cursor position
	 */
	device->GotoXY(x,y);
}

/**
 * Select a Font and font color
 *
 * @param font a pointer defined in a font defintion file
 * @param color  can be WHITE or BLACK and defaults to black
 * @param callback optional font read routine
 *
 *
 * Selects the font definition as the current font for the currently
 * selected text area.
 *
 * All subsequent printing functions will use this font. 
 *
 * Font definitions from included font definition files are stored in program memory 
 * You can have as many fonts defines as will fit in program memory up to 64k and can
 * switch between them with this function.
 *
 * If the optional callback argument is ommitted, a default routine
 * is selected that assumes that the font is in program memory (flash).
 *
 * @note
 * When the display is initilized in normal mode, BLACK renders dark 
 * pixels on a white background and WHITE renders white pixels on 
 * black background; however, if the display is set to INVERTED mode
 * all colors are inverted. 
 *
 */

void gText::SelectFont(const uint8_t* font,uint8_t color, FontCallback callback)
{
	this->Font = font;
	this->FontRead = callback;
	this->FontColor = color;
}

/**
 * Returns the pixel width of a character
 *
 * @param c character to be sized
 *
 * @return The width in pixels of the given character
 * including any inter-character gap pixels following the character when the character is
 * rendered on the display.
 *
 * @note The font for the character is the font of the currently selected text area.
 *
 * @see StringWidth()
 * @see StringWidth_P()
 */

uint8_t gText::CharWidth(char c)
{
	uint8_t width = 0;
	
    if(isFixedWidtFont(this->Font){
		width = this->FontRead(this->Font+FONT_FIXED_WIDTH)+1;  // there is 1 pixel pad here BAP
	} 
    else{ 
	    // variable width font 
		uint8_t firstChar = this->FontRead(this->Font+FONT_FIRST_CHAR);
		uint8_t charCount = this->FontRead(this->Font+FONT_CHAR_COUNT);
	
		// read width data
		if(c >= firstChar && c < (firstChar+charCount)) {
			c -= firstChar;
			width = this->FontRead(this->Font+FONT_WIDTH_TABLE+c)+1;
		}
	}	
	return width;
}

/**
 * Returns the pixel width of a string
 *
 * @param str pointer to string stored in RAM
 *
 * @return the width in pixels of the sum of all the characters in the
 * the string pointed to by str. 
 *
 * @see CharWidth()
 * @see StringWidth_P()
 */

uint16_t gText::StringWidth(const char* str)
{
	uint16_t width = 0;
	
	while(*str != 0) {
		width += this->CharWidth(*str++);
	}
	
	return width;
}

/**
 * Returns the pixel width of a character
 *
 * @param str pointer to string stored in program memory
 *
 * @return the width in pixels of the sum of all the characters in the
 * the string pointed to by str. 
 *
 * @see CharWidth()
 * @see StringWidth()
 */

uint16_t gText::StringWidth_P(PGM_P str)
{
	uint16_t width = 0;
	
	while(pgm_read_byte(str) != 0) {
		width += this->CharWidth(pgm_read_byte(str++));
	}
	
	return width;
}
/**
 * output a character to the currently selected text area
 * @param c the character to output
 *
 * This method is needed for the Print base class
 */

void gText::write(uint8_t c) 
{
	PutChar(c);
} 

 
