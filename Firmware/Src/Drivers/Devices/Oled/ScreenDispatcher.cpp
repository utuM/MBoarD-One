#include "ScreenDispatcher.h"

/*****************************************************************************/

#define OLED_ADDRESS    0x78

extern I2C_HandleTypeDef hi2c1;
static u8g_t s_handler;
static uint8_t s_control = 0;

static uint8_t Oled_function(u8g_t *u8g, uint8_t msg, uint8_t arg_val, 
                             void *arg_ptr);

/*****************************************************************************/

void u8g_Delay(uint16_t val)
{
    HAL_Delay(val);
}

static uint8_t Oled_function(u8g_t *u8g, uint8_t msg, uint8_t arg_val, 
                             void *arg_ptr)
{
    switch (msg) {
        case U8G_COM_MSG_ADDRESS: {
            //SWITCH FROM DATA TO COMMAND MODE (arg_val == 0 for command mode)
            if (arg_val == 0) {
                s_control = 0x00;
            }
            else {
                s_control = 0x40;
            }
        } break;

        case U8G_COM_MSG_WRITE_BYTE: {
            //WRITE BYTE TO DEVICE
            HAL_I2C_Mem_Write(&hi2c1, OLED_ADDRESS, s_control, 
                              I2C_MEMADD_SIZE_8BIT, &arg_val, 1,
                              HAL_TIMEOUT_MAX);
        } break;

        case U8G_COM_MSG_WRITE_SEQ: case U8G_COM_MSG_WRITE_SEQ_P: {
            //WRITE A SEQUENCE OF BYTES TO THE DEVICE
            HAL_I2C_Mem_Write(&hi2c1, OLED_ADDRESS, s_control,
                              I2C_MEMADD_SIZE_8BIT, (uint8_t*)arg_ptr,
                              arg_val, HAL_TIMEOUT_MAX);
        } break;

        default: {
            
        } break;
    }
    
    return 1;
}

/*****************************************************************************/

void Oled_initDisplay(void)
{
#if   defined(SSD1306_64x48)
    u8g_InitComFn(&s_handler, &u8g_dev_ssd1306_64x48_i2c, Oled_function);
#elif defined(SSD1306_128x32)
    u8g_InitComFn(&s_handler, &u8g_dev_ssd1306_128x32_i2c, Oled_function);
#else
#error "Need to define one of 2 possible OLED controller."
#endif
    
    u8g_Begin(&s_handler);
}

void Oled_changeFont(const u8g_fntpgm_uint8_t* font)
{
    u8g_SetFont(&s_handler, font);
}

void Oled_startPageUpdate(void)
{
    u8g_FirstPage(&s_handler);
}

uint8_t Oled_prepareNextPage(void)
{
    return u8g_NextPage(&s_handler);
}

void Oled_drawText(uint16_t xPos, uint16_t yPos, std::string text)
{
    u8g_DrawStr(&s_handler, xPos, yPos, text.c_str());
}