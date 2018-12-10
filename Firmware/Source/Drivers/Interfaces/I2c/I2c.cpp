/**
 *  @file       I2c.cpp
 *  @version    1.0 (module file)
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       10.12.2018 (release)
 *  @brief      I2C interface class.
 *              Represented as a child of 'Interface' basic class. Can be used
 *              as template object for 'Device' class objects.
 **/

#include "I2c.h"

#include <string.h>

static bool s_isI2cInit[2] = { false, false };             ///< I2C bus
                                                           ///  initialization
                                                           ///  flags.
static I2C_HandleTypeDef s_globalI2cHandler[2] = { 0, 0 }; ///< I2C global
                                                           ///  handlers.

/**
 * I2C MSP initialization.
 * @param[i] pHandler - pointer to the I2C structure object.
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
}

/**
 * I2C MSP deinitialization.
 * @param[i] pHandler - pointer to the I2C structure object.
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
} 

/**
 * I2C initialization. 
 * NOTICE!!! It must to be called to release transmission.
 * @param[i] I2cParameters& rParams - reference to parameters structure object.
 * return true - some error during initialization process;
 *        false - no error.
 **/
bool Driver::I2c::init(I2cParameters& rParams)
{  
	// Check if requered I2C bus by index is not initialized.
	if (!s_isI2cInit[rParams.m_index]) {
		// Basic I2C handler initialization.
		memset(&m_handler, 0x00, sizeof(I2C_HandleTypeDef));	
		switch (rParams.m_index) {
			case 0:
				m_handler.Instance = I2C1;
				break;
				
			case 1:
				m_handler.Instance = I2C2;
				break;
				
			default:
				break;
		}
		m_handler.Init.Timing = (uint32_t)rParams.m_speed;
		m_handler.Init.OwnAddress1 = 0;
		m_handler.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		m_handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		m_handler.Init.OwnAddress2 = 0;
		m_handler.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
		m_handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		m_handler.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
		if (HAL_I2C_Init(&m_handler) != HAL_OK) {
			return true;
		}
		// Configure analogue filter.
		if (HAL_I2CEx_ConfigAnalogFilter(&m_handler, I2C_ANALOGFILTER_ENABLE) 
																	  != HAL_OK) {
			return true;
		}
		// Configure digital filter.
		if (HAL_I2CEx_ConfigDigitalFilter(&m_handler, 0) != HAL_OK) {
			return true;		
		}
		// Copying object's initialized handler into global one by bus index.
		memcpy(&s_globalI2cHandler[rParams.m_index], &m_handler,
                                                     sizeof(I2C_HandleTypeDef));        
		s_isI2cInit[rParams.m_index] = true;
	}
	// Take the global copy of I2C instance and copy it if the bus is already.
	else {
		memcpy(&m_handler, &s_globalI2cHandler[rParams.m_index],
                                                     sizeof(I2C_HandleTypeDef));
	}
    m_address = rParams.m_address;
    m_isInit = true;
    
    return false;
}

/**
 * I2C deinitialization. In real, this method make no deinitialization of the
 * bus.
 **/
void Driver::I2c::_deinit(void)
{
    m_isInit = false;
}

/**
 * I2C class destructor.
 **/
Driver::I2c::~I2c(void)
{
    _deinit();
}

/**
 * I2C byte sending.
 * @param[i] uint8_t reg - number of device's register.
 * @param[i] uint8_t value - value that need to be sent.
 * return true - some error was occured during transmission;
 *        false - no error.
 **/
bool Driver::I2c::sendByte(uint8_t reg, uint8_t value)
{
    if (!m_isInit || HAL_I2C_Mem_Write(&m_handler, m_address, reg, 
                     I2C_MEMADD_SIZE_8BIT,  &value, 1, m_kI2cDefaultTimeout)
                                                                  != HAL_OK) {
        return true;
    }

    return false;
}

/**
 * I2C multiple bytes sending.
 * @param[i] uint8_t reg - number of device's register.
 * @param[i] uint8_t* pData - pointer to the byte array to be sent.
 * @param[i] const uint16_t size - amount of bytes to be sent.
 * return true - some error was occured during transmission;
 *        false - no error.
 **/
bool Driver::I2c::sendData(uint8_t reg, uint8_t* pData, const uint16_t size)
{
    if (!m_isInit || HAL_I2C_Mem_Write(&m_handler, m_address, reg,
                     I2C_MEMADD_SIZE_8BIT, pData, size, m_kI2cDefaultTimeout)
                                                                   != HAL_OK) {
        return true;
    }

    return false;
}

/**
 * I2C byte receiving.
 * @param[o] uint8_t reg - number of device's register.
 * @param[i] uint8_t rValue - reference to the variable to be filled by received
 *                            byte.
 * return true - some error was occured during receiving;
 *        false - no error.
 **/
bool Driver::I2c::receiveByte(uint8_t reg, uint8_t& rValue)
{
    if (!m_isInit || HAL_I2C_Mem_Read(&m_handler, m_address, reg,
                     I2C_MEMADD_SIZE_8BIT, &rValue, 1, m_kI2cDefaultTimeout)
                                                                  != HAL_OK) {
        return true;
    }

    return false;
}   

/**
 * I2C multiple bytes receiving.
 * @param[i] uint8_t reg - number of device's register.
 * @param[o] uint8_t* pData - pointer to the byte array to be filled by received
 *                            bytes.
 * @param[i] const uint16_t size - amount of bytes to be received.
 * return true - some error was occured during transmission;
 *        false - no error.
 **/
bool Driver::I2c::receiveData(uint8_t reg, uint8_t* pData, const uint16_t size)
{
    if (!m_isInit || HAL_I2C_Mem_Read(&m_handler, m_address, reg,
                     I2C_MEMADD_SIZE_8BIT, pData, size, m_kI2cDefaultTimeout)
                                                                   != HAL_OK) {
        return true;
    }

    return false;
}