#ifndef __GUI_FONT_H
#define __GUI_FONT_H

#include "Typedefs.h"

typedef u8* font_data;

/* Font interconnected structures. */

struct Symbol
{
    u8 m_width;   ///< Width of character.
    u16 m_offset; ///< Character offset in the 'm_pData' data buffer.
};

struct Font
{
    str m_name;        ///< Font name.
    u8  m_height;      ///< Maximal character height.
	sym m_startSymbol; ///< Start symbol in the font sequence.
	sym m_endSymbol;   ///< End symbol in the font sequence.
    u8  m_gapWidth;    ///< Common space size between symbols.
    u8  m_spaceWidth;  ///< Width of ' ' symbol in pixels;
    Symbol* m_pDesc;   ///< Pointer to character description data.
    u8* m_pData;       ///< Pointer to font data buffer.
    u8* m_pSmooth;     ///< Pointer to smooth information ('nullptr' if isn't
                       ///  in use).
};

extern const Font s_fontEbrima14pt;

#endif // __GUI_FONT_H