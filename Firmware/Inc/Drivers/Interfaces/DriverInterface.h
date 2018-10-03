#ifndef __DRIVER_INTERFACE_H
#define __DRIVER_INTERFACE_H

#include <cstdint>

namespace Driver {
	enum InterfaceError {
		InterfaceNoError = 0x00
	};

	class Interface {
		private:
			InterfaceError m_error;
		
		public:
			Interface(void) : m_error(InterfaceNoError) { }
			~Interface(void) { }
			
            virtual void setHandler(void* handler) = 0;
			void setError(const InterfaceError code) { m_error = code; }
			uint8_t getError(void) { return (uint8_t)m_error; }
			
			virtual uint8_t sendByte(uint8_t reg, uint8_t value) = 0;
			virtual uint8_t sendData(uint8_t reg, uint8_t* data, 
                                                             uint16_t size) = 0;
			virtual uint8_t recvByte(uint8_t reg, uint8_t* value) = 0;
			virtual uint8_t recvData(uint8_t reg, uint8_t* data, 
                                                             uint16_t size) = 0;
	};
}

#endif // __DRIVER_INTERFACE_H
