/**
  *
  **/

#include "Ssd1306.h"

// Screen object
/*static SSD1306_t SSD1306;


//
//  Send a byte to the command register
//
static void ssd1306_WriteCommand(uint8_t command)
{
    
	HAL_I2C_Mem_Write(&hi2c1,SSD1306_I2C_ADDR, 0x00,1,&command,1,10);
}*/

bool Driver::Ssd1306::_writeCommand(uint8_t command)
{
    if (m_interface.sendByte(0x00, command)) {
        return true;
    }
    
    return false;
}

Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::_init(void)
{
    // Wait for the screen to boot.
	HAL_Delay(100);
	
	/* Init LCD */
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

	/*ssd1306_WriteCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	ssd1306_WriteCommand(0xC0); //Set COM Output Scan Direction
	ssd1306_WriteCommand(0x00); //---set low column address
	ssd1306_WriteCommand(0x10); //---set high column address*/
	
	// Clear screen.
    fillScreen(true);
	// Flush buffer to screen.
	updateScreen();
    // Prepare object class before using.
    memset(m_scrBuffer, 0x00, s_kSsd1306ScnBufSize);	
	m_isInit = true;
}

Driver::Ssd1306::Ssd1306(void) : m_isInit(false),
                                 m_x(0),
                                 m_y(0),
                                 m_error(kSsd1306NoError)
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
        m_error = kSsd1306InterfaceIsntInit;
    }
}

Driver::Ssd1306::~Ssd1306(void)
{
    // Turn the screen into the sleep mode on object destruction.
    if (m_isInit && m_error == kSsd1306NoError) {
        toggle(false);
        m_isInit = false;
    }
}

Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::drawPixel(uint8_t x,
                                                             uint8_t y,
                                                             bool isWhite)
{
    if (!m_isInit && x >= s_kSsd1306Width && y >= s_kSsd1306Height) {
        return (m_error = kSsd1306InterfaceIsntInit);
    }
    
    if (isWhite) {
		m_scrBuffer[x + (y / 8) * s_kSsd1306Width] |= 1 << (y % 8);
	} 
	else {
		m_scrBuffer[x + (y / 8) * s_kSsd1306Width] &= ~(1 << (y % 8));
	}
    
    return (m_error = kSsd1306NoError);
}

Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::fillScreen(bool isWhite)
{
    if (!m_isInit) {
        return (m_error = kSsd1306InterfaceIsntInit);
    }
  
	for (uint16_t i = 0; i < s_kSsd1306ScnBufSize; i ++) {
		m_scrBuffer[i] = (isWhite ? 0xFF : 0x00);
	}
    
    return (m_error = kSsd1306NoError);
}

Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::updateScreen(void)
{
    if (!m_isInit) {
        return (m_error = kSsd1306InterfaceIsntInit);
    }
  
    for (uint8_t i = 0; i < 8; i++) {
		_writeCommand(0xB0 + i);
		_writeCommand(0x00);
		_writeCommand(0x10);

		m_interface.sendData(0x40, &m_scrBuffer[s_kSsd1306Width * i], s_kSsd1306Width);
	}
    
    return (m_error = kSsd1306NoError);
}

Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::toggle(bool isPowerOn)
{

}

//
////
////  Fill the whole screen with the given color
////
//void ssd1306_Fill(SSD1306_COLOR color) 
//{
//	/* Set memory */
//	uint32_t i;
//
//	for(i = 0; i < sizeof(SSD1306_Buffer); i++)
//	{
//		SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
//	}
//}
//
////
////  Write the screenbuffer with changed to the screen
////
//void ssd1306_UpdateScreen(void) 
//{
//	uint8_t i;
//	
//	for (i = 0; i < 8; i++) {
//		ssd1306_WriteCommand(0xB0 + i);
//		ssd1306_WriteCommand(0x00);
//		ssd1306_WriteCommand(0x10);
//
//		HAL_I2C_Mem_Write(&hi2c1,SSD1306_I2C_ADDR,0x40,1,&SSD1306_Buffer[SSD1306_WIDTH * i], SSD1306_WIDTH, 100);
//	}
//}
//
/////
////	Draw one pixel in the screenbuffer
////	X => X Coordinate
////	Y => Y Coordinate
////	color => Pixel color
////
//void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color)
//{
//	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) 
//	{
//		// Don't write outside the buffer
//		return;
//	}
//	
//	// Check if pixel should be inverted
//	if (SSD1306.Inverted) 
//	{
//		color = (SSD1306_COLOR)!color;
//	}
//	
//	// Draw in the right color
//	if (color == White)
//	{
//		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
//	} 
//	else 
//	{
//		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
//	}
//}
//
////
////	Position the cursor
////
//void ssd1306_SetCursor(uint8_t x, uint8_t y) 
//{
//	SSD1306.CurrentX = x;
//	SSD1306.CurrentY = y;
//}