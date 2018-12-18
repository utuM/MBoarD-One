#ifndef __SENSORS_MAX30100_H
#define __SENSORS_MAX30100_H

#include <stdint.h>

namespace Driver {
	const uint8_t s_kMax30100Address = 0x57 << 1;
    const uint8_t s_kMaxFifoSize = 8;
    const float s_kTempFractCoef = 0.0625f;
	
    enum Max30100Register {
        kInterruptStatus = 0x00,
        kInterruptEnable,
        kFifoWritePtr,
        kOverFlowCounter,
        kFifoReadPtr,
        kFifoDataRegister,
        kModeConfig,
        kSpo2Config,
        kLedConfig = 0x09,
        kTempInteger = 0x16,
        kTempFraction,
        kRevisionId = 0xFE,
        kPartId
    };
    
    enum Max30100ModeControl {
        kPowerDownMode = 0x80,
        kReset = 0x40,
        kTempIsEnabled = 0x08,
        kHrEnabled = 0x02,
        kSpo2Enabled
    };
    
    enum Max30100InterruptBits {
        kFullFifo = 0x80,
        kTempIsReady = 0x40,
        kHrIsReady = 0x20,
        kSpo2IsReady = 0x10
    };
    
    enum Max30100Spo2SampleRate {
        k50Samples = 0x00,
        k100Samples = 0x04,
        k167Samples = 0x08,
        k200Samples = 0x0C,
        k400Samples = 0x10,
        k600Samples = 0x14,
        k800Samples = 0x18,
        k1000Samples = 0x1C
    };
    
    enum Max30100LedPulseWidth {
        k200us13bits = 0x00,
        k400us14bits,
        k800us15bits,
        k1600us16bits
    };
    
    enum Max30100LedCurrent {
        k0p0mA = 0x00,
        k4p4mA,
        k7p6mA,
        k11p0mA,
        k14p0mA,
        k17p4mA,
        k20p8mA,
        k24p0mA,
        k27p1mA,
        k30p6mA,
        k33p8mA,
        k37p0mA,
        k40p2mA,
        k43p6mA,
        k46p8mA,
        k50p0mA
    };
    
	class Max30100 {
		private:
            uint8_t m_currSample;
            uint8_t m_availSamples;
            uint16_t m_irData[s_kMaxFifoSize];
            uint16_t m_redData[s_kMaxFifoSize];
        
            void _init(void);
		
		public:
			Max30100(void);
			~Max30100(void);
            
            bool checkId(uint8_t& revision, uint8_t& part);
            bool getHeartRate(void);
            bool getTemperature(float& value);
	};
}

#endif // __SENSORS_MAX30100_H