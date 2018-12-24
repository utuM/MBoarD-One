/**
 *  @file       Ina219.h (header file)
 *  @version    1.0.1
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       18.12.2018 (release)
 *  @brief      INA219 sensor class.
 *              Class provides to read current and voltage values from the 
 *              INA219 sensor. Also, driver provides communication between
 *              target MCU and the sensor via I2C. 
 *              To start communication with the sensor, need to define class
 *              object with input measurement mode value.
 *              To get the values just need to call the 'update(..)' method
 *              using variable references as input parameters.
 **/

#ifndef __SENSORS_INA219_H
#define __SENSORS_INA219_H

#include <math.h>
#include "Device.h"
#include "I2c.h"

namespace Driver {
    const uint8_t s_kIna219Address = 0x40 << 1;  ///< Default INA219 address on 
                                                 ///  I2C bus.
    const uint8_t s_kInaBusVoltageLsb = 4;       ///< Amount of mV that is 
                                                 ///  carried by LSB.
    const uint16_t s_kInaShuntResistorOhm = 100; ///< Shunt resistor value in
                                                 ///  Ohms. 
    const float s_kInaMaxExpectedCurrentA = 2;   ///< Maximal value of current
                                                 ///  expected in the circuit in
                                                 ///  Ampers.
    const float s_kInaFixedScale = 0.04096f;     ///< An internal fixed value
                                                 ///  used to ensure scaling is
                                                 ///  maintained properly.
    const uint16_t s_kInaMaxCurrentCode = 32768; ///< Maximal possible code raw
                                                 ///  code value for current
                                                 ///  measurement.

    /**
     * INA219 sensor registers.
     **/
    enum Ina219Registers : uint8_t {
        kInaConfiguration      = 0x00, ///< Sensor configuration.
        kInaShuntVoltage       = 0x01, ///< Shunt voltage value.
        kInaBusVoltage         = 0x02, ///< Bus voltage value.
        kInaPower              = 0x03, ///< Power value.
        kInaCurrent            = 0x04, ///< Current value.
        kInaCalibration        = 0x05  ///< Calibration value.
    };

    /**
     * INA219 PGA settings (use for sending to 'Configuration' register).
     **/
    enum Ina219ConfigureValues : uint16_t {
        kInaReset              = 0x8000, ///< Reset configuration.
        kInaBusVRange          = 0x2000, ///< Bus voltage range is 32V.
        kInaGain1              = 0x0000, ///< Voltage range is +- 40mV.
        kInaGain2              = 0x0800, ///< Voltage range is +- 80mV.
        kInaGain4              = 0x1000, ///< Voltage range is +- 160mV.
        kInaGain8              = 0x1800  ///< Voltage range is +- 320mV.
	};
	
    /**
     * INA219 bus ADC resolution (use for sending to 'Configuration' register).
     **/
    enum Ina219BusAdcResolution : uint16_t {
        kInaBus9bit            = 0x0000, ///< 9-bit ADC resolution.
        kInaBus10bit           = 0x0080, ///< 10-bit ADC resolution.
        kInaBus11bit           = 0x0100, ///< 11-bit ADC resolution.
        kInaBus12bit           = 0x0180, ///< 12-bit ADC resolution.
        kInaBus2Samples        = 0x0480, ///< 2 samples.
        kInaBus4Samples        = 0x0500, ///< 4 samples.
        kInaBus8Samples        = 0x0580, ///< 8 samples.
        kInaBus16Samples       = 0x0600, ///< 16 samples.
        kInaBus32Samples       = 0x0680, ///< 32 samples.
        kInaBus64Samples       = 0x0700, ///< 64 samples.
        kInaBus128Samples      = 0x0780  ///< 128 samples.
	};
	
    /**
     * INA219 shunt ADC resolution (use for sending to 'Configuration' register).
     **/
    enum Ina219ShuntAdcResolution : uint16_t {
        kInaShunt9bit          = 0x0000, ///< 9-bit ADC resolution.
        kInaShunt10bit         = 0x0008, ///< 10-bit ADC resolution.
        kInaShunt11bit         = 0x0010, ///< 11-bit ADC resolution.
        kInaShunt12bit         = 0x0018, ///< 12-bit ADC resolution.
        kInaShunt2Samples      = 0x0048, ///< 2 samples.
        kInaShunt4Samples      = 0x0050, ///< 4 samples.
        kInaShunt8Samples      = 0x0058, ///< 8 samples.
        kInaShunt16Samples     = 0x0060, ///< 16 samples.
        kInaShunt32Samples     = 0x0068, ///< 32 samples.
        kInaShunt64Samples     = 0x0070, ///< 64 samples.
        kInaShunt128Samples    = 0x0078  ///< 128 samples.
	};

    /**
     * INA219 measurement modes (use for sending to 'Configuration' register).
     **/
    enum Ina219ModeSettings : uint16_t {
        kInaPowerDown          = 0x0000, ///< Power-down mode.
        kInaShuntVTriggered    = 0x0001, ///< Shunt voltage, triggered.
        kInaBusVTriggered      = 0x0002, ///< Bus voltage, triggered.
        kInaShuntBusVTriggered = 0x0003, ///< Shunt/bus voltage, triggered.
        kInaAdcDisabled        = 0x0004, ///< ADC off (disabled).
        kInaShuntVContinued    = 0x0005, ///< Shunt voltage, continuous.
        kInaBusVContinued      = 0x0006, ///< Bus voltage, continuous.
        kInaShuntBusVContinued = 0x0007  ///< Shunt/bus voltage, continuous.
    };

    /**
     * INA219 possible errors.
     **/
    enum Ina219ErrorCodes : uint8_t {
        kInaNoError = 0x00,           ///< No error.
        kInaErrInterfaceIsntInit,     ///< Some error was occured during
                                      ///  interface initialization process.
        kInaErrIsntInit,              ///< The sensor wasn't properly
                                      ///  initialized.
        kInaErrReset,                 ///< Cannot send reset bit to the sensor.
        kInaErrSettingUp,             ///< Cannot send settings value to the
                                      ///  sensor.
        kInaErrReceiveRegisterValue,  ///< Cannot receive a value from the
                                      ///  requested register.
        kInaErrSendRegisterValue      ///< Cannot send a value into the 
                                      ///  requested register.
    };
    
    class Ina219 : public Device<I2c> {
        private:
            bool m_isInit;             ///< Successful initialization flag.
            Ina219ModeSettings m_mode; ///< Current measurement mode.
            float m_voltage;           ///< Voltage value.
            float m_currentLsb;        ///< Calculated current LSB value.
            float m_current;           ///< Current value.
            Ina219ErrorCodes m_error;  ///< Current error.
            
            Ina219ErrorCodes _init(Ina219ModeSettings mode);
            Ina219ErrorCodes _calibration(void);
            
        public:
            // Contructors and desctructor.
            Ina219(void) : m_isInit(false) { /* To prevent false init. */ }
            Ina219(Ina219ModeSettings mode);
            ~Ina219(void);
            // Getter.
            uint8_t getError(void) { return (uint8_t)m_error; }
            // Basic methods.
            Ina219ErrorCodes update(float& rVoltage, float& rCurrent);
            Ina219ErrorCodes toggle(bool isPowerOn);
    };
}

#endif // __SENSORS_INA219_H