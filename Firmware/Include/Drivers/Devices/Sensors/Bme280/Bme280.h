/**
 *  @file       Bme280.h (header file)
 *  @version    1.0.2
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       10.12.2018 (release)
 *  @brief      BME280 sensor class.
 *              Sensor provides to receive data about pressure, temperature
 *              and humidity of environment.
 *              To start interactions with sensor just need to initialize the
 *              class object and call 'update(...)' method every time when
 *              needed.
 *  @updates    New in current version:
 *              + Changed all enumeration fields to make them unique in 'Driver'
 *                namespace.
 *              + Moved 'toggle()' method from the private to public area to
 *                make it enable for turn on/off sensor.
 *              + Placed 'toggle()' method calling inside the class desctructor
 *                to turn sensor into sleep mode.
 *              + Added 'm_error' field to detect and log class object state.
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
        kBmePressureMsb  	   = 0xF7,
        kBmePressureLsb  	   = 0xF8,
        kBmePressureXlsb 	   = 0xF9,
        // Current temperature data registers.
        kBmeTemperatureMsb     = 0xFA,
        kBmeTemperatureLsb     = 0xFB,
        kBmeTemperatureXlsb    = 0xFC,
        // Current humidity data registers.
        kBmeHumidityMsb        = 0xFD,
        kBmeHumidityLsb        = 0xFE,  
        // Device ID number register.
        kBmeIdNumber	       = 0xD0,
        // Device reset register.
        kBmeReset     	       = 0xE0,
        // Humidity measurement oversampling register.
        kBmeCtrlHum            = 0xF2,
        // Device status register.
        kBmeStatus    	       = 0xF3,
        // Device configuration.
        kBmeCtrlMeas           = 0xF4,
        kBmeConfig             = 0xF5,
        // Temperature compensation data registers.
        kBmeCompensT1 	       = 0x88,
        kBmeCompensT2 	       = 0x8A,
        kBmeCompensT3 	       = 0x8C,
        // Pressure compensation data registers.
        kBmeCompensP1 	       = 0x8E,
        kBmeCompensP2 	       = 0x90,
        kBmeCompensP3 	       = 0x92,
        kBmeCompensP4 	       = 0x94,
        kBmeCompensP5 	       = 0x96,
        kBmeCompensP6 	       = 0x98,
        kBmeCompensP7 	       = 0x9A,
        kBmeCompensP8          = 0x9C,
        kBmeCompensP9 	       = 0x9E,
        // Humidity compensation data registers.
        kBmeCompensH1 	       = 0xA1,
        kBmeCompensH2 	       = 0xE1,
        kBmeCompensH3 	       = 0xE3,
        kBmeCompensH4 	       = 0xE4,
        kBmeCompensH5 	       = 0xE5,
        kBmeCompensH6 	       = 0xE7
    };
	
    /**
     * BME280 data update rates values.
     **/
    enum Bme280Datarates {
        kBmeDataRate10Ms = 0xC0,
        kBmeDataRate20Ms = 0xE0
    };
	
    /**
     * BME280 humidity data oversampling coefficient values.
     **/
    enum Bme280HumidityCoef {
        kBmeHumidOrsSkipped = 0x00,
        kBmeHumidOrs1       = 0x01,
        kBmeHumidOrs2 	    = 0x02,
        kBmeHumidOrs4 	    = 0x03,
        kBmeHumidOrs8 	    = 0x04,
        kBmeHumidOrs16 	    = 0x05,
    };
	
    /**
     * BME280 pressure data oversampling coefficient values.
     **/
    enum Bme280PressureCoef {
        kBmePressOrsSkipped = 0x00,
        kBmePressOrs1       = 0x04,
        kBmePressOrs2 	    = 0x08,
        kBmePressOrs4 	    = 0x0C,
        kBmePressOrs8 	    = 0x10,
        kBmePressOrs16      = 0x14,
    };
    /**
     * BME280 temperature data oversampling coefficient values.
     **/
    enum Bme280TemperatureCoef {
        kBmeTempOrsSkipped   = 0x00,
        kBmeTempOrs1         = 0x20,
        kBmeTempOrs2 	     = 0x40,
        kBmeTempOrs4 	     = 0x60,
        kBmeTempOrs8 	     = 0x80,
        kBmeTempOrs16 	     = 0xA0,
    };
    /**
     * BME280 work mode.
     **/
    enum Bme280Workmode {
        kBmeSleepMode       = 0x00,
        kBmeForcedMode      = 0x01,
        kBmeForcedModeCopy  = 0x02,
        kBmeNormalMode      = 0x03
    };
	/**
     * BME280 stand-by duration between neighbour data in normal mode
	 * values.
     **/
    enum Bme280Standby {
        kBmeStandby05Ms     = 0x00,
        kBmeStandby625Ms    = 0x20,
        kBmeStandby125Ms    = 0x40,
        kBmeStandby250Ms    = 0x60,
        kBmeStandby500Ms    = 0x80,
        kBmeStandby1000Ms   = 0xA0,
        kBmeStandby10Ms     = 0xC0,
        kBmeStandby20Ms     = 0xE0
    };
    /**
     * BME280 filter coefficient values.
     **/
    enum Bme280Filter {
        kBmeFilterOff       = 0x00,
        kBmeFilter2         = 0x04,
        kBmeFilter4         = 0x08,
        kBmeFilter8         = 0x0C,
        kBmeFilter16        = 0x10
    };
    /**
     * BME280 settings template.
     **/
    enum Bme280Patterns {
        kBmePatternWeather  = 0x01,
        kBmePatternHumidity,
        kBmePatternIndoor,
        kBmePatternGaming
    };
    /**
     * BME280 possible errors.
     **/
    enum Bme280ErrorCodes {
        kBmeNoError            = 0x00, ///< No error.
        kBmeIsntInit,                  ///< The sensor wasn't setting up
                                       ///  correctly.
        kBmeErrInterfaceIsntInit,      ///< The used interface wasn't
                                       ///  initialized correctly.
		kBmeErrCtrlMeasData,           ///< An error during sending settings
                                       ///  to the 'CTRL_MEAS' register.
        kBmeErrCtrlHumData,            ///< An error during sending settings
                                       ///  to the 'CTRL_HUM' register.
        kBmeErrCtrlConfigData,         ///< An error during sending settings
                                       ///  to the 'CTRL_CONFIG' register.
        kBmeErrId,                     ///< Received ID number is not equal
                                       ///  to default one.
        kBmeErrCompensT,               ///< An error during reading
                                       ///  temperature compensation data.
        kBmeErrCompensP,               ///< An error during reading
                                       ///  pressure compensation data.
        kBmeErrCompensH1,              ///< An error during reading
                                       ///  first part of humidity compensation
                                       ///  data.  
        kBmeErrCompensH2,              ///< An error during reading
                                       ///  second part of humidity 
                                       ///  compensation data.  
        kBmeErrReadData,               ///< An error occured on register's 
                                       ///  value reading.
        kBmeErrWriteData               ///< An error occured on register's 
                                       ///  value writing.
    };
    
    class Bme280 : public Device<I2c> {
        private:
            bool m_isInit;                        ///< Successful initialization
                                                  ///  flag.
            uint8_t m_buffer[BME280_BUFFER_SIZE]; ///< TX & RX buffer.
            uint8_t m_bufferSize;                 ///< Size of buffer.
            // Compensation data buffers.
            int16_t m_pressureCompensData[PRESS_COMPENS_SIZE];
            int16_t m_temperCompensData[TEMPR_COMPENS_SIZE];
            int16_t m_humidityCompensData[HUMID_COMPENS_SIZE];
            int32_t m_pressureCode;   ///< Calculated pressure code.
            int32_t m_temperCode;     ///< Calculated temperature code.
            int32_t m_humidityCode;   ///< Calculated humidity code.
            int32_t m_tCompensCode;   ///< Temperature compensation code.
            float m_pressure;         ///< Current pressure value.
            float m_temperature;      ///< Current temperatus value.
            float m_humidity;         ///< Current humidity value.

            Bme280ErrorCodes m_error; ///< Current error.
            
            Bme280ErrorCodes _init(Bme280Patterns pattern);
            Bme280ErrorCodes _readCompensationData(void);
            void _getTemperature(bool isCelsius);
            void _getPressure(bool isMmhg);
            void _getHumidity(void);
            
        public:
            // Contructors and desctructor.
            Bme280(void);
            Bme280(Bme280Patterns pattern);
            ~Bme280(void);
            
            // Getter.
            uint8_t getError(void) { return (uint8_t)m_error; }
            
			// Basic method.
            void update(float& rTemperature, float& hPressure,
                                                             float& rHumidity);
            Bme280ErrorCodes toggle(bool isPowerOn);
    };
}

#endif // __SENSORS_BME280_H