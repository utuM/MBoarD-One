/**
 *  @file       I2c.h
 *  @version    0.8 (header file)
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       7.12.2018 (implementation)
 *  @brief      I2C interface class.
 *              Represented as a child of 'Interface' basic class. Can be used
 *              as template object for 'Device' class objects.
 **/

#ifndef __INTERFACE_I2C_H
#define __INTERFACE_I2C_H

#include "Interface.h"
#include "i2c.h"

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
     * I2c interface possible errors.
     **/	 
    enum I2cError : uint8_t {
        kI2cNoError = 0x00
	};
												
    /**
     * I2C interface class.
     **/
    class I2c : public Interface {
        private:
            I2C_HandleTypeDef m_handler;       ///< Current I2C handler.
            uint8_t m_address;                 ///< I2C bus address.
            I2cSpeed m_speed;                  ///< I2C bus speed.
			
			I2cError _init(uint8_t index);
			I2cError _deinit();
		
        public:
            // Constructors and destructor.
            I2c(void) { /* Leave empty. */ }
            I2c(uint8_t address, I2cSpeed speed);
            ~I2c(void);
            
            // Setter.
            void setHandler(void* pHandler) { m_handler = (I2C_HandleTypeDef*)handler; }

            /// Other methods.
            bool sendByte(uint8_t reg, uint8_t value);
            bool sendData(uint8_t reg, uint8_t* pData, const uint16_t size);
            bool receiveByte(uint8_t reg, uint8_t value);
            bool receiveData(uint8_t reg, uint8_t* pData, const uint16_t size);
    };
}

#endif // __INTERFACE_I2C_H