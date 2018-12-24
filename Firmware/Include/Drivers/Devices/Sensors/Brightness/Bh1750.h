/**
 *  @file       Bh1750.h (header file)
 *  @version    1.0
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       11.12.2018 (release)
 *  @brief      BH1750 sensor class.
 *              Sensor provides to receive brightness level, measured in lux.
 *              Before start the communication, connect the 'ADDR' pin of the
 *              sensor to the common GND pin.
 *              To receive a new data from the sensor need to call class object
 *              method 'getLux(float&)' with passing variable by reference. 
 *              Received value needn't to be corvented, it's final and can be
 *              used for further operations and processes.
 **/

#ifndef __SENSORS_BH1750_H
#define __SENSORS_BH1750_H

#include "Device.h"
#include "I2c.h"

namespace Driver {
    const uint8_t s_kBh1750Address = 0x23 << 1; ///< Default BH1750 address on 
                                                ///  I2C bus.

    /**
     * BH1750 opecodes.
     **/
    enum Bh1750Opecodes {
        kBhPowerDown           = 0x00, ///< Sensor sleep mode.
        kBhPowerOn             = 0x01, ///< Sensor active mode.
        kBhReset               = 0x07, ///< Reset data register value (can be
                                       ///  used only in active mode). 
        kBhContHResolution     = 0x10, ///< Start measurement in 1 lux
                                       ///  resolution.
        kBhContHResolution2    = 0x11, ///< Start measurement in 0.5 lux
                                       ///  resolution.
        kBhContLResolution     = 0x13, ///< Start measurement in 4 lux
                                       ///  resolution.
        kBhOneTimeHResolution  = 0x20, ///< Start measurement in 1 lux
                                       ///  resolution, after this will be
                                       ///  turned into sleep mode
                                       ///  automatically.
        kBhOneTimeHResolution2 = 0x21, ///< Start measurement in 0.5 lux
                                       ///  resolution, after this will be
                                       ///  turned into sleep mode
                                       ///  automatically.
        kBhOneTimeLResolution  = 0x23, ///< Start measurement in 4 lux
                                       ///  resolution, after this will be
                                       ///  turned into sleep mode
                                       ///  automatically.
        kBhChangeMeasureTimeH  = 0x40, ///< Change measurement time (the bits 
                                       ///  2, 1, 0 need to be filled by bits 7,
                                       ///  6, 5 of the new time value).
        kBhChangeMeasureTimeL  = 0x60, ///< Change measurement time (the bits 
                                       ///  4, 3, 2, 1, 0 need to be filled by
                                       ///  the same bits of the new time
                                       ///  value).
    };
    
    /**
     * BH1750 possible errors.
     **/
    enum Bh1750ErrorCodes {
        kBhNoError = 0x00,       ///< No error.
        kBhErrInterfaceIsntInit, ///< Some error was occured during interface
                                 ///  initialization process.
        kBhErrIsntInit,          ///< The sensor wasn't properly initialized.
        kBhErrSendingOpcode,     ///< Some error was occured during opcode
                                 ///  sending.
        kBhErrReveingData        ///< Some error was occured during date
                                 ///  receiving.
    };

    class Bh1750 : public Device<I2c> {
        private:
            bool m_isInit;            ///< Successful initialization flag.
            float m_value;            ///< Current brightness in lux units.
            Bh1750ErrorCodes m_error; ///< Current error.
          
            Bh1750ErrorCodes _init(void);

        public:
            // Contructor and destructor.
            Bh1750(void);
            ~Bh1750(void);

            // Getters.
            bool getInit(void) { return m_isInit; }
            uint8_t getError(void) { return (uint8_t)m_error; }
            
            // Basic method.
            Bh1750ErrorCodes getLux(float& rValue);
            Bh1750ErrorCodes toggle(bool isTurnedOn);
    };
}

#endif // __SENSORS_BH1750_H