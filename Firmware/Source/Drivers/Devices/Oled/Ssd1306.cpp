/**
  *  @file       Ssd1306.cpp (module file)
  *  @version    0.1.0
  *  @author     utuM (Kostyantyn Komarov)
  *  @date       19.04.2019 (creation)
  *  @brief      SSD1306 OLED display controller class.
  **/

#include "Ssd1306.h"
  
/**
  *
  **/
bool Driver::Ssd1306::_writeCommand(uint8_t command)
{
    if (m_interface.sendByte(0x00, command)) {
        return true;
    }
    
    return false;
}

/**
  *
  **/
void Driver::Ssd1306::_drawPixel(u8 x, u8 y, flag isWhite)
{    
    if (isWhite) {
		_m_scrBuffer[x + (y / 8) * s_kSsd1306Width] |= 1 << (y % 8);
	} 
	else {
		_m_scrBuffer[x + (y / 8) * s_kSsd1306Width] &= ~(1 << (y % 8));
	}
}    

/**
  *
  **/
Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::_init(void)
{
    // Wait for the screen to boot.
	HAL_Delay(100);
	
	// Init OLED.
	_writeCommand(0xAE); //display off
    _writeCommand(0xD5); //--set display clock divide ratio/oscillator frequency
	_writeCommand(0x80); //--set divide ratio
    _writeCommand(0xA8); //--set multiplex ratio(1 to 64)
	_writeCommand(0x1F); //
    _writeCommand(0xD3); //-set display offset
	_writeCommand(0x00); //-not offset
    _writeCommand(0x40); //--set start line address
    _writeCommand(0x8D); //--set DC-DC enable
	_writeCommand(0x14); //
    _writeCommand(0x20); //Set Memory Addressing Mode   
	_writeCommand(0x00); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    _writeCommand(0xA1); //--set segment re-map 0 to 127
    _writeCommand(0xC8);
    _writeCommand(0xDA); //--set com pins hardware configuration
	_writeCommand(0x02);
    _writeCommand(0x81); //--set contrast control register
	_writeCommand(0x8F);
    _writeCommand(0xD9); //--set pre-charge period
	_writeCommand(0x22); //
    _writeCommand(0xDB); //--set vcomh
	_writeCommand(0x20); //0x20,0.77xVcc
    _writeCommand(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    _writeCommand(0xA6); //--set normal display
    _writeCommand(0x2E); //--set normal display
    _writeCommand(0xAF); //--turn on SSD1306 panel
	
    _m_isInit = true;
	// Clear screen.
    fillScreen(false);
	// Flush buffer to screen.
	updateScreen();
    // Prepare object class before using.
    memset(_m_scrBuffer, 0x00, s_kSsd1306ScnBufSize);
    
    return (_m_error = kSsd1306NoError);
}

/**
  *
  **/
Driver::Ssd1306::Ssd1306(void) : _m_isInit(false),
                                 _m_error(kSsd1306NoError)
{	
	// Prepare interface for communication.
    I2cParameters l_parameters = {
        0,
        s_kSsd1306Address,
        kI2cNormalMode,
        true
    };
    // Initialize the sensor only if used interface was initialized
    // successfully.
    if (!m_interface.init(l_parameters)) {
        _init();
    }
    else {
        _m_error = kSsd1306ErrInterfaceIsntInit;
    }
}

/**
  *
  **/
Driver::Ssd1306::~Ssd1306(void)
{
    // Turn the screen into the sleep mode on object destruction.
    if (_m_isInit && _m_error == kSsd1306NoError) {
        toggle(false);
        _m_isInit = false;
    }
}

/**
  *
  **/
Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::drawLine(u8 x,
                                                            u8 y,
                                                            u8 length,
                                                            u8 thickness,
                                                            flag isWhite)
{
    if (!_m_isInit) {
        return (_m_error = kSsd1306ErrDeviceIsntInit);
    }
    u8 l_x1 = x + length;
    u8 l_y1 = y + thickness;
    if (x >= s_kSsd1306Width || y >= s_kSsd1306Height ||
        l_x1 >= s_kSsd1306Width || l_y1 >= s_kSsd1306Height) {
        return (_m_error = kSsd1306ErrInvalidParams);
    }
    
    // Draw line according to input parameters.
    for (u8 i = y; i <= l_y1; i ++) {
        for (u8 j = x; j <= l_x1; j ++) {
            _drawPixel(j, i, isWhite);
        }
    }
    
    return (_m_error = kSsd1306NoError);
}

/**
  *
  **/
Driver::Ssd1306::Ssd1306ErrorCode
                              Driver::Ssd1306::drawRectangle(u8 x0,
                                                             u8 y0,
                                                             u8 x1,
                                                             u8 y1,
                                                             flag isWhite,
                                                             flag needToFill)
{
    if (!_m_isInit) {
        return (_m_error = kSsd1306ErrDeviceIsntInit);
    }
    if (x0 >= s_kSsd1306Width || y0 >= s_kSsd1306Height ||
        x1 >= s_kSsd1306Width || y1 >= s_kSsd1306Height) {
        return (_m_error = kSsd1306ErrInvalidParams);
    }
    
    // Draw rectangle according to input parameters.
    for (u8 i = y0; i <= y1; i ++) {
        for (u8 j = x0; j <= x1; j ++) {
            if (needToFill) {
                _drawPixel(i, j, isWhite);
            }
            else if ((j == x0 || i == y0) ||
                     (j == x1 || i == y1)) {
                _drawPixel(j, i, isWhite);
            }
        }
    }
    
    return (_m_error = kSsd1306NoError);
}

/**
  *
  **/
Driver::Ssd1306::Ssd1306ErrorCode
                            Driver::Ssd1306::drawText(u8 x,
                                                      u8 y,
                                                      const str text,
                                                      const Font& rFont,
                                                      const TextAlign alignment,
                                                      flag isWhite)
{
    if (!_m_isInit) {
        return (_m_error = kSsd1306ErrDeviceIsntInit);
    }
  
    // Check if font size is valid for current screen height.
    u8 l_symbHeight = rFont.m_height; ///< Maximal symbol height for input font.
    if (x >= s_kSsd1306Width || y >= s_kSsd1306Height ||
        (y + l_symbHeight) >= s_kSsd1306Height) {
        return (_m_error = kSsd1306ErrInvalidParams);
    }
    // Count total text width in pixels and update the area that need to be used
    // for input text.
    u8 l_areaWidth = 0;               ///< Width of text area.
    u16 l_textLength = text.length(); ///< Characters amount of the input text.
    u8 l_symIndex = 0;                ///< Symbol index inside the font.
    u8 l_currSymWidth = 0;            ///< Current symbol width in pixels.
    for (u16 i = 0; i < l_textLength; i ++) {
        if (text[i] != ' ') {
            l_symIndex = (u8)(text[i]) - (u8)(rFont.m_startSymbol);
            l_currSymWidth = rFont.m_pDesc[l_symIndex].m_width;
        }
        else {
            l_currSymWidth = rFont.m_spaceWidth;
        }
        l_areaWidth += l_currSymWidth;
        if (i < l_textLength - 1) {
            l_areaWidth += rFont.m_gapWidth;
        }
    }
    // Limit the right vertical area border position.
    u8 l_areaX1 = x + l_areaWidth; ///< The text area vertical limit.
    if (l_areaX1 >= s_kSsd1306Width) {
        l_areaX1 = s_kSsd1306Width - 1;
    }
    // TODO: rectangle's drawing is badly, need to fix this.
    //drawRectangle(x, y, l_areaX1, y + l_symbHeight, isWhite, yes);
    // Prepare some additional variables before drawing.
    u16 l_currSymOffset = 0;          ///< Current symbol offset inside the font
                                      ///  data buffer.
    u8 l_x1 = x;                      ///< Next X-ending position.
    u8 l_y1 = y + l_symbHeight;       ///< Bottom Y position.
    u16 l_currPosition = 0;           ///< Current byte position in data buffer.
    u8 l_currBit = 0;                 ///< Current bit offset of data byte.
    u8 l_widthInBytes = 0;            ///< Amount of bytes in a row for current
                                      ///  character.
    // Draw every symbol from the input text.
    for (u16 n = 0; n < l_textLength; n ++) {
        // Check if current character is not a 'space'.
        if (text[n] != ' ') {
            // Prepare set variables before draw a character.
            l_symIndex = (u8)(text[n]) - (u8)(rFont.m_startSymbol);
            l_currSymWidth = rFont.m_pDesc[l_symIndex].m_width;
            if (n) {
                x = l_x1 += rFont.m_gapWidth;
            }
            l_x1 += l_currSymWidth;
            
            l_widthInBytes = (l_currSymWidth / 8) +
                             ((l_currSymWidth % 8 > 0) ? 1 : 0);
            l_currSymOffset = rFont.m_pDesc[l_symIndex].m_offset;
            l_x1 = x + l_currSymWidth;
            // Prevent false drawing, when end X-position is over the screen
            // border.
            if (l_x1 >= s_kSsd1306Width) {
                return (_m_error = kSsd1306ErrOutOfXBorder);
            }
            // Draw current symbol.
            for (u8 j = y; j < l_y1; j ++) {
                for (u8 i = x; i < l_x1; i ++) {
                    l_currPosition = l_currSymOffset +
                                     (l_widthInBytes * (j - y)) + ((i - x) / 8);
                    l_currBit = 7 - ((i - x) % 8);
                    if ((rFont.m_pData[l_currPosition] >> l_currBit) & 0x01) {
                        _drawPixel(i, j, isWhite);
                    }
                }
            }
        }
        else {
            if (n) {
                x = l_x1 += rFont.m_gapWidth;
            }
            l_x1 += rFont.m_spaceWidth;
        }
    }
    
    return (_m_error = kSsd1306NoError);
}

/**
  *
  **/
Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::fillScreen(flag isWhite)
{
    if (!_m_isInit) {
        return (_m_error = kSsd1306ErrDeviceIsntInit);
    }
  
	for (u16 i = 0; i < s_kSsd1306ScnBufSize; i ++) {
		_m_scrBuffer[i] = (isWhite ? 0xFF : 0x00);
	}
    
    return (_m_error = kSsd1306NoError);
}

/**
  *
  **/
Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::updateScreen(void)
{
    if (!_m_isInit) {
        return (_m_error = kSsd1306ErrDeviceIsntInit);
    }
  
    for (u8 i = 0; i < 8; i++) {
		_writeCommand(0xB0 + i);
		_writeCommand(0x00);
		_writeCommand(0x10);

		m_interface.sendData(0x40, &_m_scrBuffer[s_kSsd1306Width * i], s_kSsd1306Width);
	}
    
    return (_m_error = kSsd1306NoError);
}

/**
  *
  **/
Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::toggle(flag isPowerOn)
{
    if (!_m_isInit) {
        return (_m_error = kSsd1306ErrDeviceIsntInit);
    }
  
    // TODO: implement properly power toggle process.
    
    return (_m_error = kSsd1306NoError);
}