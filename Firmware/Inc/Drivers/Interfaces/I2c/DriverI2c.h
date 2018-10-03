#ifndef __DRIVER_I2C_H
#define __DRIVER_I2C_H

#include "DriverInterface.h"
#include "i2c.h"

namespace Driver {
	class I2c : public Interface {
		private:
			I2C_HandleTypeDef* m_handler;
            uint8_t m_address;
		
		public:
            I2c(void);
            I2c(uint8_t address);
			~I2c(void);
            
            void setHandler(void* handler) { m_handler = (I2C_HandleTypeDef*)handler; }

			/// TODO: develop properly and functional methods
			uint8_t sendByte(uint8_t reg, uint8_t value);
			uint8_t sendData(uint8_t reg, uint8_t* data, uint16_t size);
			uint8_t recvByte(uint8_t reg, uint8_t* value);
			uint8_t recvData(uint8_t reg, uint8_t* data, uint16_t size);
	};
}

#endif // __DRIVER_I2C_H
