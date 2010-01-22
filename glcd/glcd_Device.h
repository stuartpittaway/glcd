/*
  glcd_Device.h - support for specific graphical LCDs
  Copyright (c)2008 Michael Margolis All right reserved
  
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
  public:
    glcd_Device();
    void Init(uint8_t invert);      // now public
	void GotoXY(uint8_t x, uint8_t y);   
	void SetDot(uint8_t x, uint8_t y, uint8_t color);
	void SetPixels(uint8_t x, uint8_t y,uint8_t x1, uint8_t y1, uint8_t color);
    uint8_t ReadData(void);        // now public
    void WriteData(uint8_t data); 

	uint8_t				Inverted; 
    lcdCoord			Coord;
};
  
#endif
