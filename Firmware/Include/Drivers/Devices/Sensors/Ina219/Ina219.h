/**
 *  @file       Ina219.h (header file)
 *  @version    0.1
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       12.12.2018 (implementation)
 *  @brief      
 *
 **/

#ifndef __SENSORS_INA219_H
#define __SENSORS_INA219_H

#include <math.h>
#include "Device.h"
#include "I2c.h"

namespace Driver {
    const uint8_t s_kIna219Address = 0xEC; ///< Default INA219 address on 
                                           ///  I2C bus.

    /**
     * INA219 sensor registers.
     **/
    enum Ina219Registers : uint8_t {

    };
	
    /**
     * INA219 possible errors.
     **/
    enum Ina219ErrorCodes {
        kInaNoError            = 0x00  ///< No error.
    };
    
    class Ina219 : public Device<I2c> {
        private:
            bool m_isInit;                        ///< Successful initialization
                                                  ///  flag.

            Ina219ErrorCodes m_error;             ///< Current error.
            
            Ina219ErrorCodes _init(Bme280Patterns pattern);
            
        public:
            // Contructors and desctructor.
            Ina219(void);
            // Ina219( );
            ~Ina219(void);
            
            // Getter.
            uint8_t getError(void) { return (uint8_t)m_error; }
            
            Ina219ErrorCodes toggle(bool isPowerOn);
    };
}

#endif // __SENSORS_INA219_H