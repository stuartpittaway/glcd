/*
  glcd_Device.h - support for specific graphical LCDs
  Copyright (c) 2009 Michael Margolis and Bill Perry
  
 vi:ts=4

  the glcd_Device class impliments the protocol for sending and receiving data and commands to a GLCD device.
     
*/

#ifndef	GLCD_DEVICE_H
#define GLCD_DEVICE_H

#include "glcd_Config.h"

#if defined WIRING
#include <WPrint.h> // used when deriving this class in Wiring
#else
#include "Print.h" // used when deriving this class in Arduino 
#endif

#ifndef USE_ARDUINO_FLASHSTR
// these should be replaced when Arduino supports FLASH strings in the base print class
#include <avr/pgmspace.h>
typedef class _FlashString {} *FLASHSTRING;
#define flashStr(x) ((FLASHSTRING)(PSTR((x))))
#endif

#define GLCD_Device 1 // software version of this class

// Colors
#define BLACK				0xFF
#define WHITE				0x00

typedef struct {
	uint8_t x;
	uint8_t y;
	struct {
		uint8_t col;
		uint8_t page;
	} chip[(DISPLAY_WIDTH / CHIP_WIDTH)];
} lcdCoord;
	
class glcd_Device : public Print   
{
  private:
  // Control functions
	uint8_t DoReadData(uint8_t first);
	void WriteCommand(uint8_t cmd, uint8_t chip);
	inline void Enable(void);
	inline void SelectChip(uint8_t chip); 
	void WaitReady( uint8_t chip);
	void write(uint8_t); // for Print base class
	// Coord moved to private 22 Jan 10 and gText added as friend
    lcdCoord  Coord;  
	friend class gText;  // give gText access to private data - (only use Coord)
  public:
    glcd_Device();
    void Init(uint8_t invert = false);      // now public, default is non-inverted
	void GotoXY(uint8_t x, uint8_t y);   
	void SetDot(uint8_t x, uint8_t y, uint8_t color);
	void SetPixels(uint8_t x, uint8_t y,uint8_t x1, uint8_t y1, uint8_t color);
    uint8_t ReadData(void);        // now public
    void WriteData(uint8_t data); 

#ifndef USE_ARDUINO_FLASHSTR	
	// when the following function is supported in arduino it will be removed from this library
	void printFlash(FLASHSTRING str); //this overrides the Arduino print function to implicilty store the string in flash (progmem)
    void printFlashln(FLASHSTRING str);
#endif
	uint8_t				Inverted; 
};
  
#endif
