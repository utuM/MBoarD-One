/**
 *  @file       I2c.h
 *  @version    1.1 (header file)
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       10.12.2018 (release)
 *  @brief      I2C interface class.
 *              Represented as a child of 'Interface' basic class. Can be used
 *              as template object for 'Device' class objects.
 **/

#ifndef __INTERFACE_I2C_H
#define __INTERFACE_I2C_H

#include "Interface.h"
#include "stm32f7xx_hal.h"
   
namespace Driver {
    const uint16_t m_kI2cDefaultTimeout = 1000; ///< Default timeout value
                                                ///  after which the data
                                                ///  exchange process must be
                                                ///  finished.
	
    /**
     * I2c possible speed settings.
     **/
    enum I2cSpeed : uint32_t {
        kI2cNormalMode   = 0x00000E12, ///< 100 kHz speed.
        kI2cFastMode     = 0x00000004, ///< 400 kHz speed.
        kI2cFastModePlus = 0x00000000  ///< 1000 kHz speed.
    };
    
    /**
     * I2C parameters structure.
     **/
    struct I2cParameters {
        uint8_t m_index;               ///< I2C bus index.
        uint16_t m_address;            ///< Device's address on current I2C bus.
        I2cSpeed m_speed;              ///< I2C bus speed.
        bool m_useRegisters;           ///< Flag sets to use registers or not.
    };
												
    /**
     * I2C interface class.
     **/
    class I2c : public Interface<I2C_HandleTypeDef, I2cParameters> {
        private:
            bool m_isInit;                     ///< I2C initialization init.
            
            uint16_t m_address;                ///< I2C bus address.
            I2cSpeed m_speed;                  ///< I2C bus speed.
            bool m_useRegisters;               ///< Flag to use registers or
                                               ///  not.
			
            void _deinit();
		
        public:          
            I2C_HandleTypeDef m_handler;       ///< Current I2C handler.
            // Constructors and destructor.
            I2c(void) : m_isInit(false) { /* Leave empty. */ }
            ~I2c(void);

            // Basic methods.
            bool init(I2cParameters& rParameters);
            bool sendByte(uint8_t reg, uint8_t value);
            bool sendData(uint8_t reg, uint8_t* pData, const uint16_t size);
            bool receiveByte(uint8_t reg, uint8_t& rValue);
            bool receiveData(uint8_t reg, uint8_t* pData, const uint16_t size);
    };
}

#endif // __INTERFACE_I2C_H