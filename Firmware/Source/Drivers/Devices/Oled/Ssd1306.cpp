/**
  *  @file       Ssd1306.cpp (module file)
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

#include "Ssd1306.h"
  
/**
  * @brief Write command to the OLED controller.
  * @param[i] command - input command to the controller.
  * @return no  - successful command transaction;
  *         yes - command hasn't been transmitted.
  **/
bool Driver::Ssd1306::_writeCommand(u8 command)
{
    if (m_interface.sendByte(0x00, command)) {
        return yes;
    }
    
    return no;
}

/**
  * @brief Pixel drawing.
  * @param[i] x       - start position on the X axis.
  * @param[i] y       - start position on the Y axis.
  * @param[i] isWhite - color of pixel.
  * @return None. 
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
  * @brief Device initialization method.
  * @param None.
  * @return Always returns 'kSsd1306NoError' value.
  **/
Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::_init(void)
{
    // Wait for the screen to boot up.
    HAL_Delay(100);

    // Init OLED.
    _writeCommand(kSsd1306DisplayOff);
    _writeCommand(kSsd1306SetClkDivOscFreq);
    _writeCommand(0x80); // Default one.
    _writeCommand(kSsd1306MultRatio);
    _writeCommand(0x1F); // Ratio is 31.
    _writeCommand(kSsd1306DispOffset);
    _writeCommand(0x00); // No offset.
    _writeCommand(kSsd1306StartLineAddr);
    _writeCommand(kSsd1306SetDcDcEn);
    _writeCommand(0x14); //
    _writeCommand(kSsd1306MemAddrMode);   
    _writeCommand(0x00); // Horizontal addressing mode.
    _writeCommand(kSsd1306SegmRemap);
    _writeCommand(0xA1);
    _writeCommand(kSsd1306OutScanDir);
    _writeCommand(kSsd1306PinHWConfig);
    _writeCommand(0x02);
    _writeCommand(kSsd1306ContrastCtrl);
    _writeCommand(0x8F); // Contrast is 143.
    _writeCommand(kSsd1306PreChargePeriod);
    _writeCommand(0x22);
    _writeCommand(kSsd1306VComHSel);
    _writeCommand(0x20); // 0.77 x Vcc.
    _writeCommand(kSsd1306OutFollowsRam);
    _writeCommand(kSsd1306NormMode);
    _writeCommand(kSsd1306DeactScroll);
    _writeCommand(kSsd1306DisplayOn);
    // To prevent screen clean up multiple times. 
    if (!_m_isInit) {
        _m_isInit = true;
        _m_isPoweredOn = true;
        // Clear screen.
        fillScreen(false);
        // Flush buffer to screen.
        updateScreen();
        // Prepare object class before using.
        memset(_m_scrBuffer, 0x00, s_kSsd1306ScnBufSize);
    }
    else {
        // Redraw screen with latest stored frame.
        updateScreen();
    }

    return (_m_error = kSsd1306NoError);
}

/**
  * @brief Class constructor.
  **/
Driver::Ssd1306::Ssd1306(void) : _m_isInit(false),
                                 _m_isPoweredOn(false),
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
  * @brief Class destructor.
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
  * @brief Setting up OLED contrast level.
  * @param[i] value - preffered contrast level.
  * @return kSsd1306NoError           - successful contrast level setting up;
  *         kSsd1306ErrDeviceIsntInit - device has not been initialized yet.
  **/
Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::setContrast(u8 value)
{
    if (!_m_isInit) {
        return (_m_error = kSsd1306ErrDeviceIsntInit);
    }
    
    // Setting up contrast level to input value.
    _writeCommand(kSsd1306ContrastCtrl);
    _writeCommand(value);

    return (_m_error = kSsd1306NoError);
}

/**
  * @brief Setting up display mode.
  * @param[i] isNormal - decides which display mode need to be toggle, 'yes' 
  *                      toggles normal, 'no' toggles inverse mode.
  * @return kSsd1306NoError           - successfully display mode setting up;
  *         kSsd1306ErrDeviceIsntInit - device has not been initialized yet.
  **/
Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::setDisplayMode(flag isNormal)
{
    if (!_m_isInit) {
        return (_m_error = kSsd1306ErrDeviceIsntInit);
    }
    
    // Command display turn into normal or inverse display mode.
    if (isNormal) {
        _writeCommand(kSsd1306NormMode);
    }
    else {
        _writeCommand(kSsd1306InverseMode);
    }

    return (_m_error = kSsd1306NoError);
}

/**
  * @brief Line drawing.
  *        Current method provides horizontal and vertical line drawing. To draw
  *        a horizontal line, need to set 'length' value which will be as a line
  *        width. To draw a vertical line, need to set 'thickness' which will be
  *        as a line width.
  * @param[i] x         - start position on X-axis.
  * @param[i] y         - start position on Y-axis.
  * @param[i] length    - the length of line.
  * @param[i] thickness - the thickness of line.
  * @param[i] isWhite   - if 'yes' than line will be white.
  * @return kSsd1306NoError           - line was drawn successfully;
  *         kSsd1306ErrDeviceIsntInit - device has not been initialized yet;
  *         kSsd1306ErrInvalidParams  - one or more of input parameters are 
  *                                     above the screen resolution.
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
  * @brief Rectangle drawing.
  * @param[i] x0         - X-axis start position, points to top-left corner.
  * @param[i] y0         - Y-axis start position, points to top-left corner.
  * @param[i] x1         - X-axis end position, points to bottom-right corner.
  * @param[i] y1         - Y-axis end position, points to bottom-right corner.
  * @param[i] isWhite    - color of rectangle borders, 'yes' if white.
  * @param[i] needToFill - decides to fill rectangle or not, 'yes' if need.
  * @return kSsd1306NoError           - rectangle was drawn successfully;
  *         kSsd1306ErrDeviceIsntInit - device has not been initialized yet;
  *         kSsd1306ErrInvalidParams  - one or more of input parameters are 
  *                                     above the screen resolution.
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
                _drawPixel(j, i, isWhite);
            }
            else { 
                if ((j == x0 || i == y0) ||
                    (j == x1 || i == y1)) {
                    _drawPixel(j, i, isWhite);
                }
            }
        }
    }

    return (_m_error = kSsd1306NoError);
}

/**
  * @brief Input text drawing.
  * @param[i] x         - start position on X-axis, top-left corner.
  * @param[i] y         - start position on Y-axis, top-left corner.
  * @param[i] text      - characters of text to be drawn.
  * @param[i] rFont     - reference to 'Font' object to be used for drawing.
  * @param[i] alignment - text alignment (reserved for future).
  * @param[i] isWhite   - color of characters, 'yes' if white.
  * @return kSsd1306NoError           - text was drawn successfully;
  *         kSsd1306ErrDeviceIsntInit - device has not been initialized yet;
  *         kSsd1306ErrInvalidParams  - one or more of input parameters are 
  *                                     above the screen resolution;
  *         kSsd1306ErrOutOfXBorder   - current character's position is behind
  *                                     the screen; actually is not an error,
  *                                     just prevent further drawing. 
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
        if (i < l_textLength) {
            l_areaWidth += rFont.m_gapWidth;
        }
    }
    // Limit the right vertical area border position.
    u8 l_areaX1 = x + l_areaWidth + 1; ///< The text area vertical limit.
    if (l_areaX1 >= s_kSsd1306Width) {
        l_areaX1 = s_kSsd1306Width - 1;
    }
    // Clear area that will be use for input text characters.
    drawRectangle(x, y, l_areaX1, y + l_symbHeight, !isWhite, yes);
    // Prepare some additional variables before drawing.
    u16 l_currSymOffset = 0;          ///< Current symbol offset inside the font
                                      ///  data buffer.
    u8 l_x1 = x;                      ///< Next X-ending position.
    u8 l_y1 = y + l_symbHeight;       ///< Bottom Y position.
    u16 l_currPosition = 0;           ///< Current byte position in data buffer.
    u8 l_currBit = 0;                 ///< Current bit offset in data byte.
    u8 l_widthInBytes = 0;            ///< Amount of bytes in a row to be
                                      ///  proceed for current character.
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
                // Prevent false drawing when Y-position is out of border.
                if (y < s_kSsd1306Height) {
                    for (u8 i = x; i < l_x1; i ++) {
                        // Prevent false drawing when X-position is out of
                        // border.
                        if (x < s_kSsd1306Width) {
                            l_currPosition = l_currSymOffset +
                                     (l_widthInBytes * (j - y)) + ((i - x) / 8);
                            l_currBit = 7 - ((i - x) % 8);
                            if ((rFont.m_pData[l_currPosition] >> l_currBit) &
                                                                         0x01) {
                                _drawPixel(i, j, isWhite);
                            }
                        }
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
  * @brief Image drawing.
  * @param[i] x             - start position on X-axis, top-left corner.
  * @param[i] y             - start position on Y-axis, top-left corner.
  * @param[i] rImage        - reference to 'ImageBinary' object which need to
  *                           be drawn.
  * @param[i] isWhite       - the color of image pixels, 'yes' to use white.
  * @param[i] isBackBlacked - desides if image background need to be filled with
  *                           black color, 'yes' if need.
  * @return kSsd1306NoError           - image was drawn successfully;
  *         kSsd1306ErrDeviceIsntInit - device has not been initialized yet;
  *         kSsd1306ErrInvalidParams  - one or more of input parameters are 
  *                                     above the screen resolution;
  **/
Driver::Ssd1306::Ssd1306ErrorCode
                           Driver::Ssd1306::drawImage(u8 x,
                                                      u8 y,
                                                      const ImageBinary& rImage,
                                                      flag isWhite,
                                                      flag isBackBlacked)
{
    if (!_m_isInit) {
        return (_m_error = kSsd1306ErrDeviceIsntInit);
    }
    
    // TODO: implement image drawing process.
    // Prevent false image drawing if image is above border limits.    
    if (x >= s_kSsd1306Width || y >= s_kSsd1306Height) {
        return (_m_error = kSsd1306ErrInvalidParams);
    }
    // Prepare the area where image need to be drawn.
    u8 l_x1 = x + rImage.m_width;  ///< Top pixel position on the X-axis.
    u8 l_y1 = y + rImage.m_height; ///< Top pixel position on the Y-axis.
    l_x1 = (l_x1 >= s_kSsd1306Width ? s_kSsd1306Width - 1 : l_x1);
    l_y1 = (l_y1 >= s_kSsd1306Height ? s_kSsd1306Height - 1 : l_y1);
    //drawRectangle(x, y, l_x1, l_y1, !isBackBlacked, true);
    // Prepare some additional variables before start the drawing process.
    u8 l_rowSize = rImage.m_width / 8 +
                   (rImage.m_width % 8 ? 1 : 0); ///< Row width in bytes of
                                                 ///  input image array.
    u16 l_currPosition = 0;                      ///< Current byte position in
                                                 ///  image array.
    u8 l_currBit = 0;                            ///< Current bit offset in
                                                 ///  image byte.
    // Draw the input image.
    for (u8 j = y; j < l_y1; j ++) {
        // Prevent false drawing when Y-position is out of bounds.
        if (j < s_kSsd1306Height) {
            for (u8 i = x; i < l_x1; i ++) {
                // Prevent false drawing when X-position is out of bounds.
                if (i < s_kSsd1306Width) {
                    l_currPosition = (l_rowSize * (j - y)) + ((i - x) / 8);
                    l_currBit = 7 - ((i - x) % 8);

                    if ((rImage.m_pImgData[l_currPosition] >> l_currBit) 
                                                                       & 0x01) {
                        _drawPixel(i, j, isWhite);
                    }
                }
            }
        }
    }

    return (_m_error = kSsd1306NoError);
}

/**
  * @brief Fill all the screen.
  * @param[i] isWhite - 'yes' if need to fill by white color, 'no' - with black.
  * @return kSsd1306NoError           - successfully screen filling;
  *         kSsd1306ErrDeviceIsntInit - device has not been initialized yet.
  **/
Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::fillScreen(flag isWhite)
{
    if (!_m_isInit) {
        return (_m_error = kSsd1306ErrDeviceIsntInit);
    }
  
    // Fill all frame byte values according to input 'isWhite' value.
    for (u16 i = 0; i < s_kSsd1306ScnBufSize; i ++) {
        _m_scrBuffer[i] = (isWhite ? 0xFF : 0x00);
    }

    return (_m_error = kSsd1306NoError);
}

/**
  * @brief Transmits current screen frame to the OLED-controller.
  * @param None.
  * @return kSsd1306NoError              - successful frame transcation;
  *         kSsd1306ErrDeviceIsntInit    - device has not been initialized yet;
  *         kSsd1306ErrCantTransmitFrame - frame cannot be transmitted properly.
  **/
Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::updateScreen(void)
{
    if (!_m_isInit) {
        return (_m_error = kSsd1306ErrDeviceIsntInit);
    }
  
    // Update screen in one transaction.
    u8 l_pageSize = s_kSsd1306ScnBufSize / 8;
    for (u8 i = 0; i < 8; i++) {
        _writeCommand(kSsd1306PageStart + i);
        _writeCommand(kSsd1306LowerCol);
        _writeCommand(kSsd1306HigherCol);

        if (m_interface.sendData(l_pageSize,
                                 &_m_scrBuffer[s_kSsd1306Width * i],
                                 s_kSsd1306Width)) {
            return (_m_error = kSsd1306ErrCantTransmitFrame);
        }
    }

    return (_m_error = kSsd1306NoError);
}

/**
  * @brief Screen power toggling.
  * @param[i] isPowerOn - decides to power on/off the screen, 'yes' - power on.
  * @return kSsd1306NoError              - successful power switching;
  *         kSsd1306ErrDeviceIsntInit    - device has not been initialized yet.
  **/
Driver::Ssd1306::Ssd1306ErrorCode Driver::Ssd1306::toggle(flag isPowerOn)
{
    if (!_m_isInit) {
        return (_m_error = kSsd1306ErrDeviceIsntInit);
    }

    // Prevents power on/off repeated sequence. It's not an error.
    if (_m_isPoweredOn == isPowerOn) {
        return (_m_error = kSsd1306NoError);
    }
    // Power on/off sequence.
    _m_isPoweredOn = isPowerOn;
    if (isPowerOn) {
        _init();
    }
    else {
        _writeCommand(kSsd1306DisplayOff);
    }

    return (_m_error = kSsd1306NoError);
}