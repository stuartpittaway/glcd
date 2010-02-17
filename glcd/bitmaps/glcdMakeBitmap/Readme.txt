This directory contians a utility that will convert image files to 
a header file that can be used in an Arduino sketch.

Run the utility and drop the image file to be converted into the window.
If the image can be converted, A header file is created in the bitmap directory.
If the image file name is image.bmp, the header is named image.h
Add the following line to the sketch:
#include "bitmaps/image.h"

and display the image using the following code:
GLCD.DrawBitmap(image, x, y);

where x and y are the desired location of the upper left edge of the bitmap

The utility is written in the Processing languge.
The pde file runs in the Proccessing environment on your computer, not on Arduino.
See: http://processing.org/
Executable versions of this utility are in directories for the relevant operating systems