/*
 * GLCDLibMode
 *
 * vi:ts=4
 *
 * Displays information about current library and the way
 * it is built.
 *
 * This is not a good example of a uesr sketch as it mucks around
 * with low level internal fields that should not be known
 * or used by a user application.
 */


#include <glcd.h>
#include "include/glcd_io.h"
#include "fonts/SystemFont5x7.h"       // system font

#define CONFIG_DELAY 1000 

void setup()
{
	// Serial.begin(9600);
	// delay(500);    // allow the hardware time settle (useful for teensy USB)
	GLCD.Init();   // initialise the library, non inverted writes pixels onto a clear screen

	GLCD.Text.SelectFont(System5x7); // select fixed width system font 
}


void showGLCDconfig(void);

void  loop()
{  

	showGLCDconfig();
	delay(5000);
}


void showGLCDconfig(void)
{
	
	GLCD.Text.ClearArea();

	/*
	 * Show GLCD data mode
	 *
	 * Requires getting down and dirty and mucking around done
	 * in avrio land.
	 */

	GLCD.Text.print("Data mode: ");
	/*
	 * First check for full 8 bit mode
	 *
	 */
	if(AVRDATA_8BIT(glcdData0Pin, glcdData1Pin, glcdData2Pin, glcdData3Pin,
		glcdData4Pin, glcdData5Pin, glcdData6Pin, glcdData7Pin))
	{
		/*
		 * full 8 bit mode
		 */
		GLCD.Text.print("byte\n");
	}
	else
	{
		GLCD.Text.print("\n d0-d3:");
		if(AVRDATA_4BITHI(glcdData0Pin, glcdData1Pin, glcdData2Pin, glcdData3Pin) ||
			AVRDATA_4BITLO(glcdData0Pin, glcdData1Pin, glcdData2Pin, glcdData3Pin))
		{
			GLCD.Text.print("nibble");
#ifndef GLCD_ATOMIC_IO
			GLCD.Text.print("-na");
#endif
		}
		else
		{
			GLCD.Text.print("bit");
		}

		GLCD.Text.print("\n d4-d7:");
	
		if(AVRDATA_4BITHI(glcdData4Pin, glcdData5Pin, glcdData6Pin, glcdData7Pin) ||
			AVRDATA_4BITLO(glcdData4Pin, glcdData5Pin, glcdData6Pin, glcdData7Pin))
		{
			GLCD.Text.print("nibble");
#ifndef GLCD_ATOMIC_IO
			GLCD.Text.print("-na");
#else
			GLCD.Text.print("-dis"); // for now this "knows" avrio disabled nibbles when in atomic mode.
#endif
		}
		else
		{
			GLCD.Text.print("bit");
		}
		GLCD.Text.print("\n");
	}

	/*
	 * Show number of text areas
	 */

	delay(CONFIG_DELAY);
	GLCD.Text.print("Text Areas: ");
	GLCD.Text.println(GLCD_TAREA_CNT);

	/*
 	 * Show font rendering:
	 */

	delay(CONFIG_DELAY);
	GLCD.Text.print("Text Render: ");
#ifdef GLCD_OLD_FONTDRAW
	GLCD.Text.println("OLD");
#else
	GLCD.Text.println("NEW");
#endif

	/*
	 * show no scroll down if disabled.
	 */

	delay(CONFIG_DELAY);
#ifdef GLCD_NO_SCROLLDOWN
	GLCD.Text.println("NO Down Scroll");
#endif

}
