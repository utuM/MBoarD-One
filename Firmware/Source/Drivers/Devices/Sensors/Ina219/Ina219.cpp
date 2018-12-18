/**
 *  @file       Ina219.cpp (module file)
 *  @version    1.0
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

#include "Ina219.h"

#include <math.h>

// Place 16-bit value into 2-celled buffer.
#define PUSH_16BIT(x, y) {     \
    y[0] = (uint8_t)(x >> 8);  \
    y[1] = x;                  \
}                              \

// Take 2-celled buffer into 16-bit value.
#define PULL_16BIT(x, y) {     \
    *x = (uint16_t)y[0] << 8 | \
                   y[1];       \
}                              \

/**
 * INA219 sensor initialization.
 * @return kInaNoError            - successful initialization;
 *         kInaErrCannotReset     - error on sending reset bit;
 *         kInaErrCannotSettingUp - error on sending settings.
 **/
Driver::Ina219ErrorCodes Driver::Ina219::_init(Ina219ModeSettings mode)
{
    uint16_t l_settings = 0;
    uint8_t l_buffer[2] = { 0 };
	
    // Power on device.
    toggle(true);
    // Reset the sensor.
    PUSH_16BIT((uint16_t)kInaReset, l_buffer);
    if (m_interface.sendData(kInaConfiguration, l_buffer, 2)) {
        return (m_error = kInaErrReset);	
    }
    // Setting up device.
    l_settings = kInaGain1 | kInaBus12bit | (uint8_t)mode;
    PUSH_16BIT(l_settings, l_buffer);
    if (m_interface.sendData(kInaConfiguration, l_buffer, 2)) {
        return (m_error = kInaErrSettingUp);	
    }
    // Calibrate the current measurement.
    _calibration();
    
    m_isInit = true;
    return (m_error = kInaNoError);
}

/**
 * Current measurement calibration.
 * @return kInaNoError              - successful calibration;
 *         kInaErrSendRegisterValue - cannot send calibration value.
 **/
Driver::Ina219ErrorCodes Driver::Ina219::_calibration(void)
{
    uint8_t l_buffer[2] = { 0 };
    
    // Calculate new calibration value.
    m_currentLsb = s_kInaMaxExpectedCurrentA / 32768.0;
    uint16_t l_calibration = trunc(s_kInaFixedScale /
                                   (m_currentLsb * s_kInaShuntResistorOhm));
    // Send calculated calibration value to the properly register.
    PUSH_16BIT(l_calibration, l_buffer);
    if (m_interface.sendData(kInaConfiguration, l_buffer, 2)) {
        return (m_error = kInaErrSendRegisterValue);
    }
    
    return (m_error = kInaNoError);
}

/**
 * INA219 class constructor.
 * @param[i] Ina219ModeSettings mode - requested sensor work mode.
 **/
Driver::Ina219::Ina219(Ina219ModeSettings mode) : m_isInit(false),
                                                  m_mode(mode),
                                                  m_voltage(0.0f),
                                                  m_currentLsb(0.0f),
                                                  m_current(0.0f),
                                                  m_error(kInaNoError)
{
    // Init the sensor work mode if current interface is worked.
    I2cParameters l_parameters = {
        0,
        s_kIna219Address,
        kI2cNormalMode,
        true
    };
    // Setting up the sensor only if interface is initialized correctly.
    if (!m_interface.init(l_parameters)) {
        _init(mode);
    }
    else {
        m_error = kInaErrInterfaceIsntInit;
    }
}

/**
 * INA219 class desctructor.
 **/
Driver::Ina219::~Ina219(void)
{
    // Turn the sensor into the sleep mode on object destruction.
    if (m_isInit && m_error == kInaNoError) {
        toggle(false);
        m_isInit = false;
    }
}

/**
 * INA219 update sensor.
 * @param[o] float& rVoltage - reference to the variable to store voltage value.
 * @param[o] float& rCurrent - reference to the variable to store current value.
 * @return 
 **/
Driver::Ina219ErrorCodes Driver::Ina219::update(float& rVoltage, float& rCurrent)
{
    if (!m_isInit) {
        return (m_error = kInaErrIsntInit);
    }
    
    // Get the voltage value.
    uint8_t l_buffer[2] = { 0 };
    uint16_t l_raw;
    if (m_interface.receiveData(kInaBusVoltage, l_buffer, 2)) {
        return (m_error = kInaErrReceiveRegisterValue);
    }
    PULL_16BIT(&l_raw, l_buffer);
    l_raw = l_raw >> 3;
    rVoltage = (l_raw * s_kInaBusVoltageLsb) * 1e-3;    
    // Get the current value.
    if (m_interface.receiveData(kInaCurrent, l_buffer, 2)) {
        return (m_error = kInaErrReceiveRegisterValue);
    }
    PULL_16BIT(&l_raw, l_buffer);
    l_raw = l_raw & 0x7FFF;
    l_raw = (l_buffer[0] & 0x80 ? (-l_raw) : l_raw);
    rCurrent = (l_raw * m_currentLsb) * 1e-3;
    
    return (m_error = kInaNoError);
}

/**
 * INA219 sensor power on/off.
 * @param[i] bool isTurnedOn - flag that decides to turn on/off the sensor.
 * @return kInaNoError                 - successful mode toggling;
 *         kInaErrReceiveRegisterValue - cannot read value from the
 *                                       configuration register;
 *         kInaErrSendRegisterValue    - cannot write value from the
 *                                       configuration register.
 **/
Driver::Ina219ErrorCodes Driver::Ina219::toggle(bool isTurnedOn)
{  
    uint16_t l_value = 0;
    uint8_t l_buffer[2] = { 0 };
    // Read current value from the configuration register.
    if (m_interface.receiveData(kInaConfiguration, l_buffer, 2)) {
        return (m_error = kInaErrReceiveRegisterValue);
    }
    PULL_16BIT(&l_value, l_buffer);
    // Turn sensor into requested mode.
    if (isTurnedOn) {
        l_value |= m_mode;
    }
    else {
        l_value &= 0xFFF8;
    }
    // Send updated configuration value.
    PUSH_16BIT(l_value, l_buffer);
    if (m_interface.sendData(kInaConfiguration, l_buffer, 2)) {
        return (m_error = kInaErrSendRegisterValue);
    }
    
    return (m_error = kInaNoError);
}