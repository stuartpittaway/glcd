/*
  glcd_depricated.h - GLCD functions supported for backward compatability only
  vi:ts=4

*/	

#ifndef	GLCD_DEPRICATED_H
#define GLCD_DEPRICATED_H

// the following line drawing functions have been renamed
// the new function will use the default color if the color parameter is omitted
#define DrawVertLine(x, y, length, color)   DrawVLine(x, y, length, color)
#define DrawHoriLine(x, y, length, color)   DrawHLine(x, y, length, color)

// ClearPage is replaced by ClearSysTextLine
#define ClearPage(page, color)   ClearSysTextLine(page)  // note that color is ignored !!!
	
// the following deprecated functions are replaced by arduino style print methods
#define PutChar(c)       print(c)
#define Puts(str)        print(str)
#define PrintNumber(n)   print(n)

#endif