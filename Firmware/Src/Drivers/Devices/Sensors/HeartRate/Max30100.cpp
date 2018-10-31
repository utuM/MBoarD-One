#include "Max30100.h"
#include "i2c.h"

#define     MAX30100_STANDARD_PART_ID   0x11

extern I2C_HandleTypeDef hi2c1;

void Driver::Max30100::_init(void)
{
    // Configure working mode: enable temperature and measure
    // only heart rate.
    uint8_t l_config = 0x0A;
    HAL_I2C_Mem_Write(&hi2c1, s_kMax30100Address, kModeConfig, 
                      I2C_MEMADD_SIZE_8BIT, &l_config, 1, 1000);
    // Configure SpO2 measurement.
    l_config = 0x40 | k50Samples | k1600us16bits;
    HAL_I2C_Mem_Write(&hi2c1, s_kMax30100Address, kSpo2Config, 
                      I2C_MEMADD_SIZE_8BIT, &l_config, 1, 1000);
    // Configure Red and IR LEDs current levels.
    l_config = (k11p0mA << 4) | k11p0mA;
    HAL_I2C_Mem_Write(&hi2c1, s_kMax30100Address, kLedConfig, 
                      I2C_MEMADD_SIZE_8BIT, &l_config, 1, 1000);
}

Driver::Max30100::Max30100(void)
{
	_init();
}

Driver::Max30100::~Max30100(void)
{
	
}

bool Driver::Max30100::checkId(uint8_t& revision, uint8_t& part)
{
    // Read ID registers.
    uint8_t l_buffer[2] = {0};
    HAL_I2C_Mem_Read(&hi2c1, s_kMax30100Address, kRevisionId, 
                     I2C_MEMADD_SIZE_8BIT, &l_buffer[0], 2, 1000);
    revision = l_buffer[0];
    part = l_buffer[1];
    // Check 'Part ID' value is device valid.
    return (part == MAX30100_STANDARD_PART_ID ? true : false);
}

bool Driver::Max30100::getHeartRate(void)
{
    uint8_t l_byte[8] = {0};
    HAL_I2C_Mem_Read(&hi2c1, s_kMax30100Address, kFifoDataRegister, 
                     I2C_MEMADD_SIZE_8BIT, &l_byte[0], 1, 1000);
    printf("%u\n", l_byte[0]);

    return true;
}