#ifndef __INTERFACE_SPI_H
#define __INTERFACE_SPI_H

#include "Interface.h"
#include "spi.h"

namespace Driver {
    struct SpiParameters {
		
	};
	
	class Spi : public Interface {
		private:
		    bool m_isInit;                     ///< SPI initialization init.
		
		    void _deinit();
		
		public:
		    SPI_HandleTypeDef* m_pHandler;
		    // Constructors and destructor.
			Spi(void) : m_isInit(false) { /* Leave empty. */ }
			~Spi(void);

			// Basic methods.
            bool init(SpiParameters& rParameters);
            bool sendByte(uint8_t reg, uint8_t value);
            bool sendData(uint8_t reg, uint8_t* pData, const uint16_t size);
            bool receiveByte(uint8_t reg, uint8_t& rValue);
            bool receiveData(uint8_t reg, uint8_t* pData, const uint16_t size);
	};
}

#endif // __INTERFACE_SPI_H
