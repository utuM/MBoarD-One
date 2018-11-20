#ifndef __DRIVER_PERIPHERAL_H
#define __DRIVER_PERIPHERAL_H

/**
 *	@file		DriverPeripheral.h
 *	@version	0.1
 *	@author		utuM (Kostyantyn Komarov)
 *	@date		20.11.2018 (implementation)
 *  @brief		Peripheral controller module class implementation.
 *				This class describes peripheral device of selected 
 *				microcontroller. 
 */

namespace Driver {
	/**
	 *	Peripheral class implementation.
	 **/
	class Peripheral {
		private:
			char m_name[16];	///< Name of peripheral device.
		
		public:
			// Class constructors.
			Peripheral(void);
			Peripheral(std::string name)
			{
				name = (name.length() >= 16 ? name.substr(0, 15), name);
				sprintf(m_name, "%s", name.c_str());
			}
	};
}

#endif // __DRIVER_PERIPHERAL_H
