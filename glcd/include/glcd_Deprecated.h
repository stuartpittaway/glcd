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
/**
 * @defgroup glcd_deprecated Deprecated GLCD functions
 * The following functions have been deprecated from the GLCD inerface.
 * They are temporarily available by adding the following include
 * @code
 * #include "include/glcd_Deprecated.h"
 * @endcode
 * to your sketch.
 */

/**
 * @ingroup glcd_deprecated
 * @hideinitializer
 * @function void glcd::DrawVertLine(uint8_t x, uint8_t y, uint8_t height, uint8_t color)
 * @brief Draw a Vertial line
 * @deprecated
 * This functions has been replaced by glcd::DrawVLine()
 * @see glcd::DrawVLine()
 * @see glcd::DrawHLine()
 */

#define DrawVertLine(x, y, height, color)   DrawVLine(x, y, height, color)

/**
 * @ingroup glcd_deprecated
 * @hideinitializer
 * @function void glcd::DrawHoriLine(uint8_t x, uint8_t y, uint8_t width, uint8_t color)
 * @brief Draw a Horizontal line
 * @deprecated
 * This functions has been replaced by glcd::DrawHLine()
 * @see glcd::DrawHLine()
 * @see glcd::DrawVLine()
 */
#define DrawHoriLine(x, y, width, color)   DrawHLine(x, y, width, color)

/**
 * @ingroup glcd_deprecated
 * @hideinitializer
 * @function void glcd::ClearSysTextLine(uint8_t x, line)
 * @brief Clear a line of system text
 * @deprecated
 * This function should no longer be used.
 * It has been replaced by the text area function gText::EraseTextLine(uint8_t row)
 * and gText::EraseTextLine(eraseLine_t type).
 * See the individual functions form more details.
 * @see gText::EraseTextLine(uint8_t row)
 * @see gText::EraseTextLine(eraseLine_t type)
 */
#define ClearSysTextLine(line) FillRect(0, (line*8), (DISPLAY_WIDTH-1), ((line*8)+ 7), WHITE )

/**
 * @ingroup glcd_deprecated
 * @hideinitializer
 * @function void glcd::SetInverted(uint8_t invert)
 * @brief Set LCD display mode
 * @deprecated
 * This functions has been replaced by glcd::SetDisplayMode()
 * @see glcd::SetDisplayMode()
 */
#define SetInverted(invert)	SetDisplayMode(invert)  // function renamed

#endif
