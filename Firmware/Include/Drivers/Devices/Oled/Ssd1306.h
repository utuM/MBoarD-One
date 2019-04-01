/**
  *
  **/

#ifndef __DEVICE_SSD1306_H
#define __DEVICE_SSD1306_H

#include "Device.h"
#include "I2c.h"

namespace Driver {
    const uint8_t s_kSsd1306Address = 0x78;
    const uint16_t s_kSsd1306ScnBufSize = 512;
    const uint8_t s_kSsd1306Width = 128;
    const uint8_t s_kSsd1306Height = 32;
    
    class Ssd1306 : public Device<I2c>
    {
        public:
            enum Ssd1306ErrorCode : uint8_t
            {
                kSsd1306NoError            = 0x00,
                kSsd1306InterfaceIsntInit,
            };
          
        private:
            bool m_isInit;
            uint8_t m_x;
            uint8_t m_y;
            uint8_t m_scrBuffer[s_kSsd1306ScnBufSize];
            Ssd1306ErrorCode m_error;

            Ssd1306ErrorCode _init(void);             
            bool _writeCommand(uint8_t command);

        public:
            Ssd1306(void);
			~Ssd1306(void);
            
            Ssd1306ErrorCode drawPixel(uint8_t x, uint8_t y, bool isWhite);
            
            Ssd1306ErrorCode fillScreen(bool isWhite);
            Ssd1306ErrorCode updateScreen(void);
            Ssd1306ErrorCode toggle(bool isPowerOn);
    };
}


////
////  Enumeration for screen colors
////
//typedef enum {
//	Black = 0x00, // Black color, no pixel
//	White = 0x01  //Pixel is set. Color depends on LCD
//} SSD1306_COLOR;
//
////
////  Struct to store transformations
////
//typedef struct {
//	uint16_t CurrentX;
//	uint16_t CurrentY;
//	uint8_t Inverted;
//	uint8_t Initialized;
//} SSD1306_t;
//
////	Definition of the i2c port in main
//extern I2C_HandleTypeDef SSD1306_I2C_PORT;
//
////
////  Function definitions
////
//uint8_t ssd1306_Init(void);
//void ssd1306_Fill(SSD1306_COLOR color);
//void ssd1306_UpdateScreen(void);
//void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
//void ssd1306_SetCursor(uint8_t x, uint8_t y);

#endif // __DEVICE_SSD1306_H