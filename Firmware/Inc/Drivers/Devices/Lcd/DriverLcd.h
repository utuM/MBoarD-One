#ifndef __DRIVER_LCD_H
#define __DRIVER_LCD_H

#include "DriverDevice.h"
#include "DriverSpi.h"

namespace Driver {
	class Lcd : public Device<Spi> {
		private:

        public:
            Lcd(void);
            ~Lcd(void);

            bool update(void);
	};
}

#endif // __DRIVER_LCD_H
