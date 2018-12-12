/**
 *  @file       Ina219.cpp (m file)
 *  @version    0.1
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       12.12.2018 (implementation)
 *  @brief      
 *
 **/

#include "Ina219.h"

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
Driver::Ina219ErrorCodes Driver::Ina219::_init(void)
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
    l_settings = kInaGain1 | kInaBus12bit | kInaBusVContinued;
    PUSH_16BIT(l_settings, l_buffer);
    if (m_interface.sendData(kInaConfiguration, l_buffer, 2)) {
        return (m_error = kInaErrSettingUp);	
    }
    
	m_isInit = true;
    return (m_error = kInaNoError);
}

/**
 * INA219 class constructor.
 * @param[i] Ina219ModeSettings mode - requested sensor work mode.
 **/
Driver::Ina219::Ina219(Ina219ModeSettings mode) : m_isInit(false),
                                                  m_voltage(0.0f),
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
        _init();
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
 **/
Driver::Ina219ErrorCodes Driver::Ina219::update(float& rVoltage, float& rCurrent)
{

}

/**
 * INA219 sensor power on/off.
 * @param[i] bool isTurnedOn - flag that decides to turn on/off the sensor.
 * @return kInaNoError          - successful mode toggling;
 *         kInaErrIsntInit      - the sensor wasn't initialized properly;
 *         kInaErrSendingOpcode - cannot send opcode via used interface.
 **/
Driver::Ina219ErrorCodes Driver::Ina219::toggle(bool isTurnedOn)
{
    // Check if the sensor was properly initialized.
    if (!m_isInit) {
        return (m_error = kInaErrIsntInit);
    }
  
    uint16_t l_value = 0;
    uint8_t l_buffer[2] = { 0 };
	// Read current value from the configuration register.
	if (m_interface.receiveData(kInaConfiguration, l_buffer, 2)) {
        return (m_error = kInaErrReceiveRegisterValue);
	}
    PULL_16BIT(&l_value, l_buffer);
    // Turn sensor into requested mode.
    if (isTurnedOn) {
        l_value |= kInaBusVContinued;
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