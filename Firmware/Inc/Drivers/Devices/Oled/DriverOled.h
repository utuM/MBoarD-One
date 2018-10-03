#ifndef __DRIVER_OLED_H
#define __DRIVER_OLED_H

#include "DriverDevice.h"
#include "DriverI2c.h"
#include <math.h>

namespace Driver {
    class Oled : public Device<I2c> {
        private: 
            uint8_t m_width;
            uint8_t m_height;
            bool m_needToUpdate;

            bool init(void);

        public:
            Oled(uint8_t width, uint8_t height);
            ~Oled(void);
    };
}

#endif // __DRIVER_OLED_H
