#include "DriverOled.h"

#if defined(OLED_BUFFERED_MODE)
static 
#endif

#define ADDRESS         0x3C

// Fundamental command registers.
#define SET_CONTRAST        0x81 
#define ENTIRE_DISP_ON      0xA4
#define ENTIRE_DISP_OFF     0xA5
#define NORMAL_DISPLAY      0xA6
#define INVERSE_DISPLAY     0xA7
#define DISPLAY_OFF         0xAE
#define DISPLAY_ON          0xAF
// Scrolling command registers.
#define RIGHT_H_SCROLL      0x26
#define LEFT_H_SCROLL       0x27
#define RIGHT_V_H_SCROLL    0x29
#define LEFT_V_H_SCROLL     0x2A
#define SCROLL_STOP         0x2E
#define SCROLL_START        0x2F
#define SET_SCROLL_AREA     0xA3
// Addressing command registers.


bool Driver::Oled::init(void)
{
    
}

Driver::Oled::Oled(uint8_t width, uint8_t height) : m_width(width),
                                                    m_height(height),
                                                    m_needToUpdate(false)
{
    
}

Driver::Oled::~Oled(void)
{

}