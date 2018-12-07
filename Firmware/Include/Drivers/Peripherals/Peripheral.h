/**
 *	@file       Peripheral.h (header file only)
 *	@version    1.0
 *	@author     utuM (Kostyantyn Komarov)
 *	@date       7.12.2018 (release)
 *  @brief      Peripheral controller module class implementation.
 *              This class describes peripheral device of selected 
 *              microcontroller. 
 */

#ifndef __DRIVER_PERIPHERAL_H
#define __DRIVER_PERIPHERAL_H

#include <iostream>

namespace Driver {
    /**
     *	Peripheral class implementation.
     **/
    class Peripheral {
        private:
            char m_name[16];	///< Name of peripheral device.
		
		public:
            // Class constructors and desctructor.
            Peripheral(void) { /* Leave empty for default one. */ }
            /**
             * Peripheral class constructor.
             * @param[i] const std::string name - name of current peripheral
                                                  device.
             **/
            Peripheral(const std::string name)
            {
                setName(name);
            }
            ~Peripheral(void) { }
			
            // Class setter and getter.
            /**
             * Set peripheral device name.
             * @param[i] std::string name - new name to be setting up.
             **/
            void setName(std::string name)
            {
                name = (name.length() >= 16 ? name.substr(0, 15) : name);
                sprintf(m_name, "%s", name.c_str());
            }
			
            /**
             * Get peripheral device name.
             **/
            std::string getName(void)
            {
                return std::string(m_name);
            }
    };
}

#endif // __DRIVER_PERIPHERAL_H
