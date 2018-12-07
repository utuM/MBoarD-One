/**
 *  @file       I2c.cpp
 *  @version    0.8 (module file)
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       7.12.2018 (implementation)
 *  @brief      I2C interface class.
 *              Represented as a child of 'Interface' basic class. Can be used
 *              as template object for 'Device' class objects.
 **/

#include "I2c.h"

/**
 *
 **/
void HAL_I2C_MspInit(I2C_HandleTypeDef* pHandler)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (pHandler->Instance == I2C1) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**
         * I2C1 GPIO Configuration    
         * PB6     ------> I2C1_SCL
         * PB7     ------> I2C1_SDA 
         **/
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
    else if (pHandler->Instance == I2C2) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
        /**
         * I2C2 GPIO Configuration    
         * PF0     ------> I2C2_SDA
         * PF1     ------> I2C2_SCL 
         **/
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
        __HAL_RCC_I2C2_CLK_ENABLE();
    }
    else if (pHandler->Instance == I2C4) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
        /**
         * I2C4 GPIO Configuration    
         * PF14     ------> I2C4_SCL
         * PF15     ------> I2C4_SDA 
         **/
        GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C4;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
        __HAL_RCC_I2C4_CLK_ENABLE();
    }
}

/**
 *
 **/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* pHandler)
{
    if (pHandler->Instance == I2C1) {
        __HAL_RCC_I2C1_CLK_DISABLE();
        /**
         * I2C1 GPIO Configuration    
         * PB6     ------> I2C1_SCL
         * PB7     ------> I2C1_SDA 
         **/
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);
    }
    else if (pHandler->Instance == I2C2) {
        __HAL_RCC_I2C2_CLK_DISABLE();
        /**
         * I2C2 GPIO Configuration    
         * PF0     ------> I2C2_SDA
         * PF1     ------> I2C2_SCL 
         **/
        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0 | GPIO_PIN_1);
    }
    else if (pHandler->Instance == I2C4) {
        __HAL_RCC_I2C4_CLK_DISABLE();
        /**
         * I2C4 GPIO Configuration    
         * PF14     ------> I2C4_SCL
         * PF15     ------> I2C4_SDA 
         **/
        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_14 | GPIO_PIN_15);
    }
} 

/**
 *
 **/
Driver::I2cError Driver::I2c::_init(uint8_t index)
{
    memset(&m_handler, 0x00, sizeof(I2C_HandleTypeDef));
	// Interface basic initialization.
    switch (index) {
        case 0:
            m_handler.Instance = I2C1;
            break;
			
		case 1:
            m_handler.Instance = I2C2;
            break;
			
		default:
            break;
    }
    m_handler.Init.Timing = (uint32_t)m_speed;
    m_handler.Init.OwnAddress1 = 0;
    m_handler.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    m_handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    m_handler.Init.OwnAddress2 = 0;
    m_handler.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    m_handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    m_handler.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&m_handler) != HAL_OK) {
        
    }
	// Configure analogue filter.
    if (HAL_I2CEx_ConfigAnalogFilter(&m_handler, I2C_ANALOGFILTER_ENABLE) 
		                                                          != HAL_OK) {

    }
	// Configure digital filter.
    if (HAL_I2CEx_ConfigDigitalFilter(&m_handler, 0) != HAL_OK) {
    
    }
}

/**
 *
 **/
Driver::I2cError Driver::I2c::_deinit(void)
{

}

/**
 *
 **/
Driver::I2c::I2c(uint8_t address, I2cSpeed speed) : m_address(address << 1),
                                                    m_speed(speed)
{
    _init();
}

/**
 *
 **/
Driver::I2c::~I2c(void)
{
    _deinit();
    m_handler = nullptr;
}

/**
 *
 **/
bool Driver::I2c::sendByte(uint8_t reg, uint8_t value)
{
    if (HAL_I2C_Mem_Write(m_handler, m_address, reg, I2C_MEMADD_SIZE_8BIT, 
                          &value, 1, m_kI2cDefaultTimeout) != HAL_OK) {
        return true;
    }

    return false;
}

/**
 *
 **/
bool Driver::I2c::sendData(uint8_t reg, uint8_t* pData, const uint16_t size)
{
    if (HAL_I2C_Mem_Write(m_handler, m_address, reg, I2C_MEMADD_SIZE_8BIT,
                          pData, size, m_kI2cDefaultTimeout) != HAL_OK) {
        return true;
    }

    return false;
}

/**
 *
 **/
bool Driver::I2c::receiveByte(uint8_t reg, uint8_t value)
{
    if (HAL_I2C_Mem_Read(m_handler, m_address, reg, I2C_MEMADD_SIZE_8BIT, 
                         &rValue, 1, m_kI2cDefaultTimeout) != HAL_OK) {
        return true;
    }

    return false;
}   

/**
 *
 **/
bool Driver::I2c::receiveData(uint8_t reg, uint8_t* pData, const uint16_t size)
{
    if (HAL_I2C_Mem_Read(m_handler, m_address, reg, I2C_MEMADD_SIZE_8BIT,
                         pData, size, m_kI2cDefaultTimeout) != HAL_OK) {
        return true;
    }

    return false;
}