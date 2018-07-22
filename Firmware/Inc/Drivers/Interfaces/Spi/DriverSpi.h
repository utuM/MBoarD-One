#ifndef __DRIVER_SPI_H
#define __DRIVER_SPI_H

#include "DriverInterface.h"
#include "spi.h"

namespace Driver {
	class Spi : public Interface {
		private:
			SPI_HandleTypeDef* m_handler;
		
		public:
			Spi(void);
			~Spi(void);
			/// TODO: develop properly and functional methods
			uint8_t sendByte(uint8_t byte);
			uint8_t sendData(uint8_t* data, uint16_t size);
			uint8_t recvByte(uint8_t* byte);
			uint8_t recvData(uint8_t* data, uint16_t size);
	};
}

#endif // __DRIVER_SPI_H
