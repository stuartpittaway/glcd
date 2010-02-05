/*
  glcd_deprecated.h - GLCD functions supported for backward compatibility only
  Include this file if you are migrating from the ks0108 library and want to use the old style function names
  See the documentation for more information on migrating from the old library
  vi:ts=4

*/	

#ifndef	GLCD_DEPRECATED_H
#define GLCD_DEPRECATED_H

// the following line drawing functions have been renamed
// the new function will use the default color if the color parameter is omitted
#define DrawVertLine(x, y, length, color)   DrawVLine(x, y, length, color)
#define DrawHoriLine(x, y, length, color)   DrawHLine(x, y, length, color)
#define ClearSysTextLine(line) FillRect(0, (line*8), (DISPLAY_WIDTH-1), ((line*8)+ 7), WHITE )

#define SetInverted(mode)	SetDisplayMode(mode)  // function renamed
	
// the following deprecated functions are replaced by Arduino style print methods
#define PutChar(c)       print(c)
#define Puts(str)        print(str)
#define PrintNumber(n)   print(n)

#endif