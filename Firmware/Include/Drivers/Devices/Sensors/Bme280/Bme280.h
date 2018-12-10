/**
 *  @file       Bme280.h (header file)
 *  @version    1.0
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       10.12.2018 (release)
 *  @brief      BME280 sensor class.
 *              Sensor provides to receive data about pressure, temperature
 *              and humidity of environment.
 *              To start interactions with sensor just need to initialize the
 *              class object and call 'update(...)' method every time when
 *              needed.
 **/

#ifndef __SENSORS_BME280_H
#define __SENSORS_BME280_H

#include <math.h>
#include "Device.h"
#include "I2c.h"

// Units convertation macroses.
#define HPA2MMHG(x)      (x * 0.750062)
#define MMHG2HPA(x)      (x / 0.750062)

#define CELS2KELV(x)     (x + 273.15)
#define KELV2CELS(x)     (x - 273.15)

#define BME280_BUFFER_SIZE     20

// Known compensation arrays sizes.
#define TEMPR_COMPENS_SIZE       3
#define PRESS_COMPENS_SIZE       9
#define HUMID_COMPENS_SIZE       6

// Known registers values.
#define BME280_DEFAULT_ID       0x60
#define BME280_DEVICE_RESET     0xB6

namespace Driver {
    const uint8_t s_kBme280Address = 0xEC; ///< Default BME280 address on 
                                           ///  I2C bus.

    /**
     * BME280 sensor registers.
     **/
    enum Bme280Registers : uint8_t {
        // Current pressure data registers.
        kPressureMsb       = 0xF7,
        kPressureLsb       = 0xF8,
        kPressureXlsb      = 0xF9,
        // Current temperature data registers.
        kTemperatureMsb    = 0xFA,
        kTemperatureLsb    = 0xFB,
        kTemperatureXlsb   = 0xFC,
        // Current humidity data registers.
        kHumidityMsb       = 0xFD,
        kHumidityLsb       = 0xFE,  
        // Device ID number register.
        kIdNumber          = 0xD0,
        // Device reset register.
        kReset             = 0xE0,
        // Humidity measurement oversampling register.
        kCtrlHum           = 0xF2,
        // Device status register.
        kStatus            = 0xF3,
        // Device configuration.
        kCtrlMeas          = 0xF4,
        kConfig            = 0xF5,
        // Temperature compensation data registers.
        kCompensT1         = 0x88,
        kCompensT2         = 0x8A,
        kCompensT3         = 0x8C,
        // Pressure compensation data registers.
        kCompensP1         = 0x8E,
        kCompensP2         = 0x90,
        kCompensP3         = 0x92,
        kCompensP4         = 0x94,
        kCompensP5         = 0x96,
        kCompensP6         = 0x98,
        kCompensP7         = 0x9A,
        kCompensP8         = 0x9C,
        kCompensP9         = 0x9E,
        // Humidity compensation data registers.
        kCompensH1         = 0xA1,
        kCompensH2         = 0xE1,
        kCompensH3         = 0xE3,
        kCompensH4         = 0xE4,
        kCompensH5         = 0xE5,
        kCompensH6         = 0xE7
    };
	
    /**
     * BME280 data update rates values.
     **/
    enum Bme280Datarates {
        kDataRate10Ms = 0xC0,
        kDataRate20Ms = 0xE0
    };
	
    /**
     * BME280 humidity data oversampling coefficient values.
     **/
    enum Bme280HumidityCoef {
        kHumidOrsSkipped = 0x00,
        kHumidOrs1       = 0x01,
        kHumidOrs2       = 0x02,
        kHumidOrs4       = 0x03,
        kHumidOrs8       = 0x04,
        kHumidOrs16      = 0x05,
    };
	
    /**
     * BME280 pressure data oversampling coefficient values.
     **/
    enum Bme280PressureCoef {
        kPressOrsSkipped = 0x00,
        kPressOrs1       = 0x04,
        kPressOrs2       = 0x08,
        kPressOrs4       = 0x0C,
        kPressOrs8       = 0x10,
        kPressOrs16      = 0x14,
    };
    /**
     * BME280 temperature data oversampling coefficient values.
     **/
    enum Bme280TemperatureCoef {
        kTempOrsSkipped  = 0x00,
        kTempOrs1        = 0x20,
        kTempOrs2        = 0x40,
        kTempOrs4        = 0x60,
        kTempOrs8        = 0x80,
        kTempOrs16       = 0xA0,
    };
    /**
     * BME280 work mode.
     **/
    enum Bme280Workmode {
        kSleepMode       = 0x00,
        kForcedMode      = 0x01,
        kForcedModeCopy  = 0x02,
        kNormalMode      = 0x03
    };
	/**
     * BME280 stand-by duration between neighbour data in normal mode
	 * values.
     **/
    enum Bme280Standby {
        kStandby05Ms     = 0x00,
        kStandby625Ms    = 0x20,
        kStandby125Ms    = 0x40,
        kStandby250Ms    = 0x60,
        kStandby500Ms    = 0x80,
        kStandby1000Ms   = 0xA0,
        kStandby10Ms     = 0xC0,
        kStandby20Ms     = 0xE0
    };
    /**
     * BME280 filter coefficient values.
     **/
    enum Bme280Filter {
        kFilterOff       = 0x00,
        kFilter2         = 0x04,
        kFilter4         = 0x08,
        kFilter8         = 0x0C,
        kFilter16        = 0x10
    };
    /**
     * BME280 settings template.
     **/
    enum Bme280Patterns {
        kPatternWeather  = 0x01,
        kPatternHumidity,
        kPatternIndoor,
        kPatternGaming
    };
    /**
     * BME280 possible errors.
     **/
    enum Bme280ErrorCodes {
        kNoError               = 0x00, ///< No error.
        kErrCtrlMeasData               ///< An error during sending settings
                                       ///  to the 'CTRL_MEAS' register.
        kErrCtrlHumData,               ///< An error during sending settings
                                       ///  to the 'CTRL_HUM' register.
        kErrCtrlConfigData,            ///< An error during sending settings
                                       ///  to the 'CTRL_CONFIG' register.
        kErrId,                        ///< Received ID number is not equal
                                       ///  to default one.
        kErrCompensT,                  ///< An error during reading
                                       ///  temperature compensation data.
        kErrCompensP,                  ///< An error during reading
                                       ///  pressure compensation data.
        kErrCompensH1,                 ///< An error during reading
                                       ///  first part of humidity compensation
                                       ///  data.  
        kErrCompensH2,                 ///< An error during reading
                                       ///  second part of humidity 
                                       ///  compensation data.  
        kErrReadData,                  ///< An error occured on register's 
                                       ///  value reading.
        kErrWriteData                  ///< An error occured on register's 
                                       ///  value writing.
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
            Bme280ErrorCodes _toggle(bool isPowerOn);
            Bme280ErrorCodes _readCompensationData(void);
            void _getTemperature(bool isCelsius);
            void _getPressure(bool isMmhg);
            void _getHumidity(void);
            
        public:
            // Contructors and desctructor.
            Bme280(void);
            Bme280(Bme280Patterns pattern);
            ~Bme280(void) { /* Leave clean. */ }
            
			// Basic method.
            void update(float& rTemperature, float& hPressure,
                                                             float& rHumidity);
    };
}

#endif // __SENSORS_BME280_H