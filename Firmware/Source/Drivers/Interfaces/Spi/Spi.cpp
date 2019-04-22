/**
 *  @file       Spi.cpp
 *  @version    0.1 (header file)
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       19.04.2019 (creation)
 *  @brief      SPI interface class implementation.
 **/

#include "Spi_c.h"

#include <string.h>

#if defined(STM32F746xx)
    #define MAXIMAL_SPI_HANDLERS_AMOUNT 6
#else
    #error "SPI handlers amount is 0!"
#endif

// SPI bus initialization flags.
static bool s_isSpiInit[MAXIMAL_SPI_HANDLERS_AMOUNT] = 
{
    no, no, no, no, no, no
};             
                                
// SPI global handlers.
static SPI_HandleTypeDef s_globalSpiHandler[6] =
{
    0, 0, 0, 0, 0, 0
};                                                            

///**
// * SPI MSP initialization.
// * @param[i] pHandler - pointer to the SPI structure object.
// **/
//void HAL_SPI_MspInit(SPI_HandleTypeDef* pHandler)
//{
//    
//}
//
///**
// * SPI MSP deinitialization.
// * @param[i] pHandler - pointer to the SPI structure object.
// **/
//void HAL_SPI_MspDeInit(SPI_HandleTypeDef* pHandler)
//{
//    
//} 

/**
 * SPI initialization. 
 * NOTICE!!! It must to be called to release transmission.
 * @param[i] I2cParameters& rParams - reference to parameters structure object.
 * return true - some error during initialization process;
 *        false - no error.
 **/
bool Driver::Spi::init(SpiParameters& rParams)
{  
	// Check if requered SPI bus by index is not initialized.
	if (!s_isSpiInit[rParams.m_index]) {
		// Basic SPI handler initialization.
		memset(&m_handler, 0x00, sizeof(SPI_HandleTypeDef));	
		switch (rParams.m_index) {
			case 0:
				m_handler.Instance = (SPI_TypeDef*)kSpi1;
				break;
				
			case 1:
				m_handler.Instance = (SPI_TypeDef*)kSpi2;
				break;
                
            case 2:
                m_handler.Instance = (SPI_TypeDef*)kSpi3;
                break;
                
            case 3:
                m_handler.Instance = (SPI_TypeDef*)kSpi4;
                break;
              
            case 4:
                m_handler.Instance = (SPI_TypeDef*)kSpi5;
                break;
              
            case 5:
				m_handler.Instance = (SPI_TypeDef*)kSpi6;
                break;
              
			default:
				break;
		}
        // Setting up other constant parameters.
        m_handler.Init.Mode = SPI_MODE_MASTER;
        m_handler.Init.Direction = SPI_DIRECTION_2LINES;
        m_handler.Init.DataSize = SPI_DATASIZE_8BIT;
        m_handler.Init.CLKPolarity = SPI_POLARITY_LOW;
        m_handler.Init.CLKPhase = SPI_PHASE_1EDGE;
        m_handler.Init.NSS = SPI_NSS_SOFT;
        m_handler.Init.BaudRatePrescaler = rParams.m_speed;
        m_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;
        m_handler.Init.TIMode = SPI_TIMODE_DISABLE;
        m_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        m_handler.Init.CRCPolynomial = 7;
        m_handler.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
        m_handler.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
		if (HAL_SPI_Init(&m_handler) != HAL_OK) {
			return true;
		}
		// Copying object's initialized handler into global one by bus index.
		memcpy(&s_globalSpiHandler[rParams.m_index], &m_handler,
                                                     sizeof(SPI_HandleTypeDef));     
		s_isSpiInit[rParams.m_index] = true;
	}
	// Take the global copy of I2C instance and copy it if the bus is already.
	else {
		memcpy(&m_handler, &s_globalSpiHandler[rParams.m_index],
                                                     sizeof(SPI_HandleTypeDef));
	}

    _m_isInit = true;
    
    return false;
}

/**
 * SPI deinitialization. In real, this method make no deinitialization of the
 * bus.
 **/
void Driver::Spi::_deinit(void)
{
    _m_isInit = false;
}

/**
 * SPI class destructor.
 **/
Driver::Spi::~Spi(void)
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
bool Driver::Spi::sendByte(uint8_t reg, uint8_t value)
{
    if (!_m_isInit) {
        return true;
    }
    // Transmit 1 byte.
    if (HAL_SPI_Transmit(&m_handler, &value, 1,
                                              m_kSpiDefaultTimeout) != HAL_OK) {
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
bool Driver::Spi::sendData(uint8_t reg, uint8_t* pData, const uint16_t size)
{
    if (!_m_isInit) {
        return true;
    }
    // Transmit 'size' amount of byte.
    if (HAL_SPI_Transmit(&m_handler, pData, size,
                                              m_kSpiDefaultTimeout) != HAL_OK) {
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
bool Driver::Spi::receiveByte(uint8_t reg, uint8_t& rValue)
{
    if (!_m_isInit) {
        return true;
    }
    // Transfer 1 byte and then receive 1 byte.
    u8 l_txBuffer[2] = { reg, 0x00 },
       l_rxBuffer[2] = { 0x00, 0x00 };
    if (HAL_SPI_TransmitReceive(&m_handler, l_txBuffer, l_rxBuffer, 2, 
                                              m_kSpiDefaultTimeout) != HAL_OK) {
        rValue = 0;
        return true;                      
    }
    rValue = l_rxBuffer[1];

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
bool Driver::Spi::receiveData(uint8_t reg, uint8_t* pData, const uint16_t size)
{
    if (!_m_isInit) {
        return true;
    }
    // Receive data according to 'm_useRegisters' flag.
    

    return false;
}