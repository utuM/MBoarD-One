#ifndef __SENSORS_MAX30100_H
#define __SENSORS_MAX30100_H

#include "DriverDevice.h"
#include "DriverI2c.h"
#include <math.h>

namespace Device {
	const uint8_t s_kMax30100Address = 0;
	
	class Max30100 : public Device<I2c>	{
		private:
		
		public:
			Max30100(void);
			~Max30100(void);
	};
}

#endif // __SENSORS_MAX30100_H