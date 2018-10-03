#include "DriverI2c.h"

#define I2C_TIMEOUT     0x1000

Driver::I2c::I2c(void)
{
    
}

Driver::I2c::I2c(uint8_t address) : m_address(address)
{
    if (address > 0) {
        printf("Good address: %02X\n", m_address);
    }
}

Driver::I2c::~I2c(void)
{
    m_handler = nullptr;
}

/// TODO: change reloaded functions to handling SPI properly for every device.

uint8_t Driver::I2c::sendByte(uint8_t reg, uint8_t value)
{
    if (HAL_I2C_Mem_Write(m_handler, m_address, reg, I2C_MEMADD_SIZE_8BIT, 
                          &value, 1, I2C_TIMEOUT) != HAL_OK) {
        return 0x01;
    }

    return 0x00;
}

uint8_t Driver::I2c::sendData(uint8_t reg, uint8_t* data, uint16_t size)
{
    if (HAL_I2C_Mem_Write(m_handler, m_address, reg, I2C_MEMADD_SIZE_8BIT,
                          data, size, I2C_TIMEOUT) != HAL_OK) {
        return 0x01;
    }

    return 0x00;
}

uint8_t Driver::I2c::recvByte(uint8_t reg, uint8_t* value)
{
    if (HAL_I2C_Mem_Read(m_handler, m_address, reg, I2C_MEMADD_SIZE_8BIT, 
                         value, 1, I2C_TIMEOUT) != HAL_OK) {
        return 0x01;
    }

    return 0x00;
}   

uint8_t Driver::I2c::recvData(uint8_t reg, uint8_t* data, uint16_t size)
{
    if (HAL_I2C_Mem_Read(m_handler, m_address, reg, I2C_MEMADD_SIZE_8BIT,
                         data, size, I2C_TIMEOUT) != HAL_OK) {
        return 0x01;
    }

    return 0x00;
}