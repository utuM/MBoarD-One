#ifndef __DRIVER_DEVICE_H
#define __DRIVER_DEVICE_H

/**
 *	@file		DriverDevice.h
 *	@version	1.0
 *	@author 	utuM (Kostyantyn Komarov)
 *	@date		20.11.2018 (release)
 *	@brief		Basic external devices or sensors class description.
 *				This class represents device or a sensor using fields
 * 				that give all basic information you need. 
 **/

#include <string>
#include "DriverInterface.h"

namespace Driver {
	enum DeviceError {
		DeviceNoError = 0x00
	};

	/**
	 *	Template class description.
	 **/
	template <typename T>
	class Device {
		private:
			char m_name[16];		///< Device name.
			uint32_t m_id;			///< Device ID.
			DeviceError m_error;	///< Current error during runtime.
		
		public:
            T m_interface;			///< Device's communication interface.

			// Class constructors.
			Device(void) : m_error(DeviceNoError) { }
			Device(std::string name, uint8_t address) : m_interface(address),
														m_error(DeviceNoError)
			{
				name = (name.length() >= 16 ? name.substr(0, 15), name);
				sprintf(m_name, "%s", name.c_str());
			}
			// Class destructor.
			~Device(void) { }
			// Class setter.
			void setError(DeviceError error) { m_error = error; }
			// Class getters.
			std::string getName(void) { return std::string(m_name); }
			uint32_t getId(void) { return m_id; }
			uint8_t getError(void) { return (uint8_t)m_error; }
	};
}

#endif // __DRIVER_DEVICE_H
