/**
 *  @file       Ccs811.h (header file)
 *  @version    1.0.0
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       06.02.2019 (implementation)
 *  @brief      CCS811 sensor class.
 **/

#ifndef __SENSORS_CCS811_H
#define __SENSORS_CCS811_H

#include "Device.h"
#include "I2c.h"

namespace Driver {
    const uint8_t s_kCcs811Address = 0xB4; ///< Default CCS811 address on
                                           ///  I2C bus.
    
	enum Ccs811Registers : uint8_t {
		kCcs811Status        = 0x00,
		kCcs811MeasMode,
		kCcs811AlgResultData,
		kCcs811RawData,
		kCcs811EnvData       = 0x05,
		kCcs811Ntc,
		kCcs811Thresholds    = 0x10,
		kCcs811Baseline,
		kCcs811HwId          = 0x20,
		kCcs811HwVersion,
		kCcs811FwBootVersion = 0x23,
		kCcs811FwAppVersion,
		kCcs811ErrorId       = 0xE0,
		kCcs811SwReset       = 0xFF
	};
	
	enum kCcs811StatusBits : uint8_t {
		kCcs811Error     = 0x01,
		kCcs811DataReady = 0x08
	};
	
	enum kCcs811MeasModeBits : uint8_t {
		kCcs811PowerSave       = 0x00,
		kCcs811IntThresholds   = 0x04,
		kCcs811IntDataReady    = 0x08,
		kCcs811ConstPower1s    = 0x10,
		kCcs811PulseHeat10s    = 0x20,
		kCcs811LowPower60s     = 0x30,
		kCcs811ConstPower250ms = 0x40
	};
	
    class Ccs811 : public Device<I2c> {
        private:
            bool m_isInit;                        ///< Successful initialization
                                                  ///  flag.
			uint8_t m_buffer[8];                  ///< Algorithm results data 
			                                      ///  buffer.
	        uint16_t m_co2;                       ///< Level of CO2 in the air.
			uint16_t m_tvoc;                      ///< Level of TVOC in the air.

            Ccs811ErrorCodes m_error; ///< Current error.
            
            Ccs811ErrorCodes _init(void);
            
        public:
            // Contructors and desctructor.
            Ccs811(void);
            ~Ccs811(void);
            
            // Getter.
            uint8_t getError(void) { return (uint8_t)m_error; }
            
            // Basic method.
			Ccs811ErrorCodes update(uint16_t& rCo2, uint16_t& rTvoc);
            Ccs811ErrorCodes toggle(bool isPowerOn);
    };
}

#endif // __SENSORS_CCS811_H