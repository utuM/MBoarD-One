#ifndef __SENSORS_BME280_H
#define __SENSORS_BME280_H

#include "DriverDevice.h"
#include "DriverI2c.h"
#include "i2c.h"
#include <math.h>

// Units convertation macros
#define HPA2MMHG(x)     (x * 0.750062)
#define MMHG2HPA(x)     (x / 0.750062)

#define CELS2KELV(x)    (x + 273.15)
#define KELV2CELS(x)    (x - 273.15)

#define  BME280_BUFFER_SIZE     20

// Known compensation arrays sizes
#define TEMPR_COMPENS_SIZE       3
#define PRESS_COMPENS_SIZE       9
#define HUMID_COMPENS_SIZE       6

// Known registers values
#define BME280_DEFAULT_ID       0x60
#define BME280_DEVICE_RESET     0xB6

namespace Driver {
    enum Bme280Registers {
        // Sensor writing and reading registers when connection is on VDD.
        BME280_VDD_WRITE 	       = 0xEE,
        BME280_VDD_READ  	       = 0xEF,
        // Current pressure data registers.
        BME280_PRESSURE_MSB  	   = 0xF7,
        BME280_PRESSURE_LSB  	   = 0xF8,
        BME280_PRESSURE_XLSB 	   = 0xF9,
        // Current temperature data registers.
        BME280_TEMPERATURE_MSB     = 0xFA,
        BME280_TEMPERATURE_LSB     = 0xFB,
        BME280_TEMPERATURE_XLSB    = 0xFC,
        // Current humidity data registers.
        BME280_HUMIDITY_MSB        = 0xFD,
        BME280_HUMIDITY_LSB        = 0xFE,  
        // Device ID number register.
        BME280_ID_NUMBER	       = 0xD0,
        // Device reset register.
        BME280_RESET     	       = 0xE0,
        // Humidity measurement oversampling register.
        BME280_CTRL_HUM            = 0xF2,
        // Device status register.
        BME280_STATUS    	       = 0xF3,
        // Device configuration.
        BME280_CTRL_MEAS           = 0xF4,
        BME280_CONFIG              = 0xF5,
        // Temperature compensation data registers.
        BME280_COMPENS_T1 	       = 0x88,
        BME280_COMPENS_T2 	       = 0x8A,
        BME280_COMPENS_T3 	       = 0x8C,
        // Pressure compensation data registers.
        BME280_COMPENS_P1 	       = 0x8E,
        BME280_COMPENS_P2 	       = 0x90,
        BME280_COMPENS_P3 	       = 0x92,
        BME280_COMPENS_P4 	       = 0x94,
        BME280_COMPENS_P5 	       = 0x96,
        BME280_COMPENS_P6 	       = 0x98,
        BME280_COMPENS_P7 	       = 0x9A,
        BME280_COMPENS_P8          = 0x9C,
        BME280_COMPENS_P9 	       = 0x9E,
        // Humidity compensation data registers.
        BME280_COMPENS_H1 	       = 0xA1,
        BME280_COMPENS_H2 	       = 0xE1,
        BME280_COMPENS_H3 	       = 0xE3,
        BME280_COMPENS_H4 	       = 0xE4,
        BME280_COMPENS_H5 	       = 0xE5,
        BME280_COMPENS_H6 	       = 0xE7
    };
    // Data update rates.
    enum Bme280Datarates {
        BME280_DATA_RATE_10MS = 0xC0,
        BME280_DATA_RATE_20MS = 0xE0
    };
    // Humidity data oversampling coefficient.
    enum Bme280HumidityCoef {
        BME280_HUMID_ORS_SKIPPED = 0x00,
        BME280_HUMID_ORS_1       = 0x01,
        BME280_HUMID_ORS_2 	     = 0x02,
        BME280_HUMID_ORS_4 	     = 0x03,
        BME280_HUMID_ORS_8 	     = 0x04,
        BME280_HUMID_ORS_16 	 = 0x05,
    };
    // Pressure data oversampling coefficient.
    enum Bme280PressureCoef {
        BME280_PRESS_ORS_SKIPPED = 0x00,
        BME280_PRESS_ORS_1       = 0x04,
        BME280_PRESS_ORS_2 	     = 0x08,
        BME280_PRESS_ORS_4 	     = 0x0C,
        BME280_PRESS_ORS_8 	     = 0x10,
        BME280_PRESS_ORS_16 	 = 0x14,
    };
    // Temperature data oversampling coefficient.
    enum Bme280TemperatureCoef {
        BME280_TEMPR_ORS_SKIPPED = 0x00,
        BME280_TEMPR_ORS_1       = 0x20,
        BME280_TEMPR_ORS_2 	     = 0x40,
        BME280_TEMPR_ORS_4 	     = 0x60,
        BME280_TEMPR_ORS_8 	     = 0x80,
        BME280_TEMPR_ORS_16 	 = 0xA0,
    };
    // Work mode.
    enum Bme280Workmode {
        BME280_SLEEP_MODE       = 0x00,
        BME280_FORCED_MODE      = 0x01,
        BME280_FORCED_MODE_COPY = 0x02,
        BME280_NORMAL_MODE      = 0x03
    };
    // Stand-by duration between neighbour data in normal mode.
    enum Bme280Standby {
        BME280_STANDBY_0_5MS  = 0x00,
        BME280_STANDBY_62_5MS = 0x20,
        BME280_STANDBY_125MS  = 0x40,
        BME280_STANDBY_250MS  = 0x60,
        BME280_STANDBY_500MS  = 0x80,
        BME280_STANDBY_1000MS = 0xA0,
        BME280_STANDBY_10MS   = 0xC0,
        BME280_STANDBY_20MS   = 0xE0
    };
    // Data filter coefficient.
    enum Bme280Filter {
        BME280_FILTER_OFF = 0x00,
        BME280_FILTER_2   = 0x04,
        BME280_FILTER_4   = 0x08,
        BME280_FILTER_8   = 0x0C,
        BME280_FILTER_16  = 0x10
    };
    // Settings templates.
    enum Bme280Patterns {
        BME280_PATTERN_WEATHER = 0x01,
        BME280_PATTERN_HUMIDITY,
        BME280_PATTERN_INDOOR,
        BME280_PATTERN_GAMING
    };
    // Error codes
    enum Bme280ErrorCodes {
        BME280_NOERROR = 0x00,
        BME280_ERR_NULL_HANDLER,
        BME280_ERR_CTRL_MEAS_DATA,
        BME280_ERR_CTRL_HUM_DATA,
        BME280_ERR_CONFIG_DATA,
        BME280_ERR_ID,
        BME280_ERR_COMPENS_T,
        BME280_ERR_COMPENS_P,
        BME280_ERR_COMPENS_H1,
        BME280_ERR_COMPENS_H2,
        BME280_ERR_INVALID_REGS_AMOUNT,
        BME280_ERR_READ_DATA,
        BME280_ERR_WRITE_DATA,
        BME280_NULL_POINTER
    };
    
    class Bme280 : public Device<I2c> {
        private:
            uint8_t m_buffer[BME280_BUFFER_SIZE]; ///< TX & RX buffer.
            uint8_t m_bufferSize;                 ///< Size of buffer.
            // Compensation data buffers.
            int16_t m_pressureCompensData[PRESS_COMPENS_SIZE];
            int16_t m_temperCompensData[TEMPR_COMPENS_SIZE];
            int16_t m_humidityCompensData[HUMID_COMPENS_SIZE];
            int32_t m_pressureCode; ///< Calculated pressure code.
            int32_t m_temperCode;   ///< Calculated temperature code.
            int32_t m_humidityCode; ///< Calculated humidity code.
            int32_t m_tCompensCode; ///< Temperature compensation code.
            float m_pressure;       ///< Current pressure value.
            float m_temperature;    ///< Current temperatus value.
            float m_humidity;       ///< Current humidity value.
            
            Bme280ErrorCodes _init(Bme280Patterns pattern);
            Bme280ErrorCodes _turnPower(bool isPowerOn);
            Bme280ErrorCodes _readCompensationData(void);
            void _getTemperature(bool isCelsius);
            void _getPressure(bool isMmhg);
            void _getHumidity(void);
            
        public:
            Bme280(void);
            Bme280(Bme280Patterns pattern);
            ~Bme280(void);
            
            void update(float& temperature, float& pressure, float& humidity);
    };
}

#endif // __SENSORS_BME280_H