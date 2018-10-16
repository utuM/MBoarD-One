#ifndef __SENSORS_BH1750_H
#define __SENSORS_BH1750_H

#include "DriverDevice.h"
#include "DriverI2c.h"
#include "gpio.h"
#include "i2c.h"

namespace Driver {
    const uint8_t s_kBh1750Address = 0x23 << 1;

    enum Bh1750ErrorCodes {
        NoError = 0x00
    };

    class Bh1750 {
        private:
            Bh1750ErrorCodes _init(void);

        public:
            Bh1750(void);
            ~Bh1750(void);

            void getLux(float& value);
    };
}

#endif // __SENSORS_BH1750_H