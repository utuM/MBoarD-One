/**
 *	@file		DriverInterface.h
 *	@version	0.8
 *	@author		utuM (Kostyantyn Komarov)
 *	@date		20.11.2018 (pre-release)
 *  @brief		Interface class implementation.
 *				This class was developed for unifiing communication functions
 *				calling to be used in device template-classes. 
 **/

#ifndef __DRIVER_INTERFACE_H
#define __DRIVER_INTERFACE_H

#include <cstdint>

namespace Driver {
	/**
	 *	Interface errors description.
	 */
	enum InterfaceError {
		InterfaceNoError = 0x00,	///< No error was occured during process.
		InterfaceNullHandler,		///< Empty interface handler or non-assigned.
		InterfaceSendError,			///< Error during byte/bytes transmiting.
		InterfaceReceiveError		///< Error during byte/bytes receiving.
	};
	/**
	 *	Interface class declaration.
	 */
	class Interface {
		private:
			InterfaceError m_error; ///< Current interface error.
		
		public:
			// Class constructors.
			Interface(void) : m_error(InterfaceNoError) { }
			~Interface(void) { }
			// Class setters.
            virtual void setHandler(void* handler) = 0;
			void setError(const InterfaceError code) { m_error = code; }
			// Class getter.
			uint8_t getError(void) { return (uint8_t)m_error; }
			
			// Class other public methods.
			virtual uint8_t sendByte(uint8_t reg, uint8_t value) = 0;
			virtual uint8_t sendData(uint8_t reg, uint8_t* data, 
                                                             uint16_t size) = 0;
			virtual uint8_t recvByte(uint8_t reg, uint8_t* value) = 0;
			virtual uint8_t recvData(uint8_t reg, uint8_t* data, 
                                                             uint16_t size) = 0;
	};
}

#endif // __DRIVER_INTERFACE_H
