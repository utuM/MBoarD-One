#include "Max30100.h"

#include <cstring>
#include "i2c.h"

#define     MAX30100_STANDARD_PART_ID   0x11

extern I2C_HandleTypeDef hi2c1;

void Driver::Max30100::_init(void)
{    
    // Configure working mode: enable temperature and measure
    // only heart rate.
    uint8_t l_config = kHrEnabled;
    HAL_I2C_Mem_Write(&hi2c1, s_kMax30100Address, kModeConfig, 
                      I2C_MEMADD_SIZE_8BIT, &l_config, 1, 1000);
    // Configure interrupts.
    l_config = kFullFifo | kTempIsReady | kHrIsReady;
    HAL_I2C_Mem_Write(&hi2c1, s_kMax30100Address, kInterruptEnable, 
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

Driver::Max30100::Max30100(void) : m_currSample(0), m_availSamples(0)
{
	_init();
}

Driver::Max30100::~Max30100(void)
{
	
}

bool Driver::Max30100::checkId(uint8_t& revision, uint8_t& part)
{
    // Zeroing raw data arrays.
    memset(&m_irData[0], 0x00, s_kMaxFifoSize * sizeof(uint16_t));    
    memset(&m_redData[0], 0x00, s_kMaxFifoSize * sizeof(uint16_t));
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
    uint8_t l_byte[3] = {0};
    
    // Clear FIFO registers.
    HAL_I2C_Mem_Write(&hi2c1, s_kMax30100Address, kFifoWritePtr,
                      I2C_MEMADD_SIZE_8BIT, &l_byte[0], 3, 1000);
    // Check if any data in FIFO.
    HAL_I2C_Mem_Read(&hi2c1, s_kMax30100Address, kInterruptStatus, 
                     I2C_MEMADD_SIZE_8BIT, &l_byte[0], 1, 1000);
    if (!(l_byte[0] & 0x20)) {
        return false;
    }
    // 1st transaction: get the FIFO_WR_PTR data.
    l_byte[0] = 8;
    HAL_I2C_Mem_Read(&hi2c1, s_kMax30100Address, kFifoWritePtr, 
                     I2C_MEMADD_SIZE_8BIT, &m_availSamples, 1, 1000);
    // 2nd transaction: read all sample from the FIFO.
    m_currSample = 0;
    do {
        // Get IR data.
        HAL_I2C_Mem_Read(&hi2c1, s_kMax30100Address, kFifoDataRegister, 
                         I2C_MEMADD_SIZE_8BIT, &l_byte[0], 1, 1000);
        HAL_I2C_Mem_Read(&hi2c1, s_kMax30100Address, kFifoDataRegister, 
                         I2C_MEMADD_SIZE_8BIT, &l_byte[1], 1, 1000);
        m_irData[m_currSample] = (uint16_t)l_byte[0] << 8 | l_byte[1];
        // Get LED data.
        HAL_I2C_Mem_Read(&hi2c1, s_kMax30100Address, kFifoDataRegister, 
                         I2C_MEMADD_SIZE_8BIT, &l_byte[0], 1, 1000);
        HAL_I2C_Mem_Read(&hi2c1, s_kMax30100Address, kFifoDataRegister, 
                         I2C_MEMADD_SIZE_8BIT, &l_byte[1], 1, 1000);
        m_redData[m_currSample] = (uint16_t)l_byte[0] << 8 | l_byte[1];
    } while ((++ m_currSample) < m_availSamples);
    // 3rd transaction: write data into the FIFO_RD_PTR.
    HAL_I2C_Mem_Write(&hi2c1, s_kMax30100Address, kFifoReadPtr, 
                      I2C_MEMADD_SIZE_8BIT, &m_currSample, 1, 1000);
    
    return true;
}

bool Driver::Max30100::getTemperature(float& value)
{
    uint8_t l_byte[2] = {0};
    
    // Set bit to get new temperature value.
    HAL_I2C_Mem_Read(&hi2c1, s_kMax30100Address, kModeConfig, 
                     I2C_MEMADD_SIZE_8BIT, &l_byte[0], 1, 1000);
    l_byte[0] |= kTempIsEnabled;
    HAL_I2C_Mem_Write(&hi2c1, s_kMax30100Address, kModeConfig, 
                     I2C_MEMADD_SIZE_8BIT, &l_byte[0], 1, 1000);
    // Get new temperature value.
    HAL_I2C_Mem_Read(&hi2c1, s_kMax30100Address, kTempInteger, 
                     I2C_MEMADD_SIZE_8BIT, &l_byte[0], 2, 1000);
    value = (char)l_byte[0] + (l_byte[1] * s_kTempFractCoef);
    
    return true;
}