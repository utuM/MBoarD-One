#ifndef __DRIVER_DEVICE_H
#define __DRIVER_DEVICE_H

#include <string>
#include "DriverInterface.h"

namespace Driver {
	enum DeviceError {
		DeviceNoError = 0x00
	};

	template <typename T>
	class Device {
		private:
			char m_description[16];
			uint32_t m_id;
			DeviceError m_error;
		
		public:
            T m_interface;

			Device(void) : m_error(DeviceNoError) { }
            Device(uint8_t address) : m_interface(address), m_error(DeviceNoError) { }
			~Device(void) { }
			
			void setError(DeviceError error) { m_error = error; }
			std::string getDescription(void) { return std::string(m_description); }
			uint32_t getId(void) { return m_id; }
			uint8_t getError(void) { return (uint8_t)m_error; }
	};
}

#endif // __DRIVER_DEVICE_H
