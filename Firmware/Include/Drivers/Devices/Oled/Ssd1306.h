/**
  *  @file       Ssd1306.h (header file)
  *  @version    0.1.0
  *  @author     utuM (Kostyantyn Komarov)
  *  @date       19.04.2019 (creation)
  *  @brief      SSD1306 OLED display controller class.
  **/

#ifndef __DEVICE_SSD1306_H
#define __DEVICE_SSD1306_H

#include "Device.h"
#include "Font.h"
#include "GuiBasics.h"
#include "I2c.h"
#include "Typedefs.h"

namespace Driver
{
    const u8  s_kSsd1306Address    = 0x78;
    const u16 s_kSsd1306ScnBufSize = 512;
    const u8  s_kSsd1306Width      = 128;
    const u8  s_kSsd1306Height     = 32;
    
    /**
      * SSD1306 class.
      **/
    class Ssd1306 : public Device<I2c>
    {
        public:
            /**
              * Possible SSD1306 errors.
              **/
            enum Ssd1306ErrorCode : u8
            {
                kSsd1306NoError               = 0x00,
                kSsd1306ErrInterfaceIsntInit,
                kSsd1306ErrDeviceIsntInit,
                kSsd1306ErrInvalidParams,
                kSsd1306ErrOutOfXBorder
            };

            Ssd1306(void);
			~Ssd1306(void);
            
            Ssd1306ErrorCode drawLine(u8 x, u8 y, u8 length, u8 thickness,
                                                                  flag isWhite);
            Ssd1306ErrorCode drawRectangle(u8 x0, u8 y0, u8 x1, u8 y1,
                                           flag isWhite, flag needToFill);
            Ssd1306ErrorCode drawText(u8 x, u8 y, const str text,
                                      const Font& rFont,
                                      const TextAlign alignment, flag isWhite);
            
            Ssd1306ErrorCode fillScreen(flag isWhite);
            Ssd1306ErrorCode updateScreen(void);
            Ssd1306ErrorCode toggle(flag isPowerOn);
          
        private:
            flag _m_isInit;
            u8 _m_scrBuffer[s_kSsd1306ScnBufSize];
            Ssd1306ErrorCode _m_error;

            Ssd1306ErrorCode _init(void);             
            flag _writeCommand(u8 command); 
            void _drawPixel(u8 x, u8 y, flag isWhite);
    };
}

#endif // __DEVICE_SSD1306_H