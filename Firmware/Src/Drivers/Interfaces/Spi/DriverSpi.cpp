#include "DriverSpi.h"

extern SPI_HandleTypeDef hspi1;

Driver::Spi::Spi(void)
{
    m_handler = &hspi1;
}

Driver::Spi::~Spi(void)
{
    m_handler = nullptr;
}

/// TODO: change reloaded functions to handling SPI properly for every device.

uint8_t Driver::Spi::sendByte(uint8_t reg, uint8_t value)
{
    if (HAL_SPI_Transmit(m_handler, &value, 1, 0x1000) != HAL_OK) {
        return 0x01;
    }

    return (uint8_t)InterfaceNoError;
}

uint8_t Driver::Spi::sendData(uint8_t reg, uint8_t* data, uint16_t size)
{
    if (HAL_SPI_Transmit(m_handler, data, size, 0x1000) != HAL_OK) {
        return 0x01;
    }

    return (uint8_t)InterfaceNoError;
}

uint8_t Driver::Spi::recvByte(uint8_t reg, uint8_t* value)
{
    if (HAL_SPI_Receive(m_handler, value, 1, 0x1000) != HAL_OK) {
        return 0x01;
    }

    return (uint8_t)InterfaceNoError;
}   

uint8_t Driver::Spi::recvData(uint8_t reg, uint8_t* data, uint16_t size)
{
    if (HAL_SPI_Receive(m_handler, data, size, 0x1000) != HAL_OK) {
        return 0x01;
    }

    return (uint8_t)InterfaceNoError;
}