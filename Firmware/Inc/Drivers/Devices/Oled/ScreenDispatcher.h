#ifndef DISPLAY_SCREENDISPATCHER_H
#define DISPLAY_SCREENDISPATCHER_H

/*****************************************************************************/

#include <fstream>
#include "i2c.h"
#include "u8g.h"

/*****************************************************************************/

#define  DEFAULT_FONT            u8g_font_helvR12                     

#define  HAL_TIMEOUT_MAX         0x1000

/*****************************************************************************/

// Test prototypes
void Oled_initDisplay(void);
void Oled_changeFont(const u8g_fntpgm_uint8_t* font);
void Oled_startPageUpdate(void);
uint8_t Oled_prepareNextPage(void);
void Oled_drawText(uint16_t xPos, uint16_t yPos, std::string text);

/*****************************************************************************/

#endif // DISPLAY_SCREENDISPATCHER_H