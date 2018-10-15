#include "Bh1750.h"
#include <math.h>

extern I2C_HandleTypeDef hi2c1;

Driver::Bh1750ErrorCodes Driver::Bh1750::_init(void)
{

}

Driver::Bh1750::Bh1750(void)
{

}

Driver::Bh1750::~Bh1750(void)
{
    
}

void Driver::Bh1750::getLx(float& value)
{
    uint8_t l_buffer[2] = {0x10, 0x00};
    uint16_t l_temp; 

    HAL_I2C_Master_Transmit(&hi2c1, Driver::s_kBh1750Address, &l_buffer[0], 1, 1000);
    HAL_Delay(180);
    HAL_I2C_Master_Receive(&hi2c1, Driver::s_kBh1750Address, &l_buffer[0], 1, 1000);
    l_temp = ((uint16_t)l_buffer[0] << 8) | l_buffer[1];
    
    value = 0;
    for (uint8_t i = 0; i < 16; i ++) {
        value += ((l_temp >> i) & 0x0001) * (0x0001 << i);
    }
    value /= 1.2;
}