/**
  *  @file       Ssd1306.h (header file)
  *  @version    1.0.0
  *  @author     utuM (Kostyantyn Komarov)
  *  @date       19.04.2019 (creation)
  *  @date       24.04.2019 (release)
  *  @brief      SSD1306 OLED-display controller class driver.
  *              Driver provides to draw graphical elements that useful to build
  *              any GUI page: pixel, line, rectangle, text and image.
  *              Current display frame is stored inside the object field as a
  *              byte buffer. Every byte's bit present the OLED pixel, which
  *              state decides to turn on/off pixel that is binded to it. To
  *              update the frame, the 'updateScreen' method need to be called.
  *              After that, the screen frame will be transmitted.
  *              Driver provides to driver OLED display with 128(w) to 32(h)
  *              resolution.
  *              User cannot draw pixels directly calling the class method:
  *              it is used only as very basic method that builds any other
  *              graphical element.
  *              The characters format presented by 'Font' structure, defined 
  *              and described in 'Font.h' file. In future, the format will be
  *              changed to support font smoothing.
  *              The image format for OLED display presented by 'ImageBinary'
  *              structure, defined and described in 'ImageBinary.h' file.
  **/

#ifndef __DEVICE_SSD1306_H
#define __DEVICE_SSD1306_H

#include "Device.h"
#include "Font.h"
#include "GuiBasics.h"
#include "ImageBinary.h"
#include "I2c.h"
#include "Typedefs.h"

namespace Driver
{
    const u8  s_kSsd1306Address    = 0x78; ///< SSD1306 address on the I2C-bus.
    const u16 s_kSsd1306ScnBufSize = 512;  ///< Size of screen frame in bytes.
    const u8  s_kSsd1306Width      = 128;  ///< Screen width in pixels.
    const u8  s_kSsd1306Height     = 32;   ///< Screen height in pixels.

    /**
      * SSD1306 class.
      **/
    class Ssd1306 : public Device<I2c>
    {
        public:
            /**
              * SSD1306 commands (only that are in usage).
              **/
            enum Ssd1306Commands : u8
            {
                kSsd1306LowerCol         = 0x00, ///< Low column start address.
                kSsd1306HigherCol        = 0x10, ///< High column start address.
                kSsd1306MemAddrMode      = 0x20, ///< Memory addressing mode.
                kSsd1306DeactScroll      = 0x2E, ///< Scroll deactivation.
                kSsd1306StartLineAddr    = 0x40, ///< Display start line.
                kSsd1306ContrastCtrl     = 0x81, ///< Contrast control.
                kSsd1306SetDcDcEn        = 0x8D, ///< Charge pump.
                kSsd1306SegmRemap        = 0xA1, ///< Segment re-map to 127.
                kSsd1306OutFollowsRam    = 0xA4, ///< Output follows RAM data.
                kSsd1306OutNotFolRam     = 0xA5, ///< Output doesn't folows RAM.
                kSsd1306NormMode         = 0xA6, ///< Normal display mode.
                kSsd1306InverseMode      = 0xA7, ///< Inversed display mode.
                kSsd1306MultRatio        = 0xA8, ///< Multiplex ratio.
                kSsd1306DisplayOff       = 0xAE, ///< Display power off.
                kSsd1306DisplayOn        = 0xAF, ///< Display power on.
                kSsd1306PageStart        = 0xB0, ///< Start page is PAGE0.
                kSsd1306OutScanDir       = 0xC8, ///< COM output from COM[N-1].
                kSsd1306DispOffset       = 0xD3, ///< Display offset.
                kSsd1306SetClkDivOscFreq = 0xD5, ///< Clock divide ratio /
                                                 ///  oscillator frequency.
                kSsd1306PreChargePeriod  = 0xD9, ///< Pre-charge period.
                kSsd1306PinHWConfig      = 0xDA, ///< COM pins HW configuration.
                kSsd1306VComHSel         = 0xDB  ///< Vcomh deselect level.
            };

            /**
              * Possible SSD1306 errors.
              **/
            enum Ssd1306ErrorCode : u8
            {
                kSsd1306NoError               = 0x00, ///< No error.
                kSsd1306ErrInterfaceIsntInit,         ///< Interface is not
                                                      ///  initialized properly.
                kSsd1306ErrDeviceIsntInit,            ///< Device is not
                                                      ///  initialized properly.
                kSsd1306ErrDisplayPwrDown,            ///< The display power is
                                                      ///  down.
                kSsd1306ErrInvalidParams,             ///< Input parameters are
                                                      ///  not valid.
                kSsd1306ErrOutOfXBorder,              ///< Current x position is
                                                      ///  out of borders.
                kSsd1306ErrCantTransmitFrame          ///< Current frame cannot
                                                      ///  be transmitted.
            };

            // Constuctor and desctructor.
            Ssd1306(void);
            ~Ssd1306(void);

            // Setters.
            Ssd1306ErrorCode setContrast(u8 value);
            Ssd1306ErrorCode setDisplayMode(flag isNormal);
            
            // Basic methods.
            Ssd1306ErrorCode drawLine(u8 x, u8 y, u8 length, u8 thickness,
                                                                  flag isWhite);
            Ssd1306ErrorCode drawRectangle(u8 x0, u8 y0, u8 x1, u8 y1,
                                           flag isWhite, flag needToFill);
            Ssd1306ErrorCode drawText(u8 x, u8 y, const str text,
                                      const Font& rFont,
                                      const TextAlign alignment, flag isWhite);
            Ssd1306ErrorCode drawImage(u8 x, u8 y, const ImageBinary& rImage,
                                              flag isWhite, flag isBackBlacked);
            Ssd1306ErrorCode fillScreen(flag isWhite);
            Ssd1306ErrorCode updateScreen(void);
            Ssd1306ErrorCode toggle(flag isPowerOn);

        private:
            flag _m_isInit;                        ///< Successful init flag.
            flag _m_isPoweredOn;                   ///< To prevent false on/off.
            u8 _m_scrBuffer[s_kSsd1306ScnBufSize]; ///< Screen frame.
            Ssd1306ErrorCode _m_error;             ///< Current error.

            // Basic private methods.
            Ssd1306ErrorCode _init(void);             
            flag _writeCommand(u8 command); 
            void _drawPixel(u8 x, u8 y, flag isWhite);
    };
}

#endif // __DEVICE_SSD1306_H