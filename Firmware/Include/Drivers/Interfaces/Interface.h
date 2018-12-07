/**
 *  @file       Interface.h
 *  @version    1.0
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       7.12.2018 (release)
 *  @brief      Interface class implementation.
 *              This class was developed for unifiing communication functions
 *              calling to be used in device template-classes. 
 **/

#ifndef __DRIVER_INTERFACE_H
#define __DRIVER_INTERFACE_H

#include <cstdint>

namespace Driver {
    /**
     *	Interface class declaration.
     */
    class Interface {
        private:
		
        public:
            // Class constructor and desctructor.
            Interface(void) : m_error(InterfaceNoError) { /* Leave empty. */ }
            ~Interface(void) { }
            // Class setters.
            virtual void setHandler(void* handler) = 0;
			
            // Basic interface methods.
            virtual bool sendByte(uint8_t reg, uint8_t value) = 0;
            virtual bool sendData(uint8_t reg, uint8_t* data, uint16_t size) = 0;
            virtual bool recvByte(uint8_t reg, uint8_t* value) = 0;
            virtual bool recvData(uint8_t reg, uint8_t* data, uint16_t size) = 0;
	};
}

#endif // __DRIVER_INTERFACE_H
