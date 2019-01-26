/**
 *  @file       Interface.h (header only)
 *  @version    1.0.1
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       12.12.2018 (release)
 *  @brief      Interface class implementation.
 *              This class was developed for unifiing communication functions
 *              calling to be used in device template-classes. 
 **/

#ifndef __DRIVER_INTERFACE_H
#define __DRIVER_INTERFACE_H

#include <stdint.h>
   
namespace Driver {
    /**
     *	Interface class declaration.
     */
    template <typename H, typename P>
    class Interface {
        private:
            /* Leave empty. */
		
        public:			
            Interface(void) { /* Leave empty. */ }
            ~Interface(void) { /* Leave empty. */ }
          
            // Basic interface methods.
            virtual bool init(P& parameters) = 0;
            virtual bool sendByte(uint8_t reg, uint8_t value) = 0;
            virtual bool sendData(uint8_t reg, uint8_t* pData, 
                                                       const uint16_t size) = 0;
            virtual bool receiveByte(uint8_t reg, uint8_t& value) = 0;
            virtual bool receiveData(uint8_t reg, uint8_t* pData, 
                                                       const uint16_t size) = 0;
    };
}

#endif // __DRIVER_INTERFACE_H
