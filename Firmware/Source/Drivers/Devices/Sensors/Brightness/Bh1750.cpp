/**
 *  @file       Bh1750.cpp (module file)
 *  @version    1.0
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       11.12.2018 (release)
 *  @brief      BH1750 sensor class.
 *              Sensor provides to receive brightness level, measured in lux.
 *              Before start the communication, connect the 'ADDR' pin of the
 *              sensor to the common GND pin.
 *              To receive a new data from the sensor need to call class object
 *              method 'getLux(float&)' with passing variable by reference. 
 *              Received value needn't to be corvented, it's final and can be
 *              used for further operations and processes.
 **/

#include "Bh1750.h"

/**
 * BH1750 sensor initialization.
 * @return kBhNoError     - successful initialization;
 *         kBhErrIsntInit - the sensor wasn't initialized properly.    
 **/
Driver::Bh1750ErrorCodes Driver::Bh1750::_init(void)
{
    // Power on the sensor.
    if (m_interface.sendByte(0x00, kBhPowerOn)) {
        return (m_error = kBhErrIsntInit);
    }
    HAL_Delay(10);
    // Reset sensor's data registers.
    if (m_interface.sendByte(0x00, kBhReset)) {
        return (m_error = kBhErrIsntInit);
    }
    
    m_isInit = true;
    return (m_error = kBhNoError);
}

/**
 * BH1750 class constructor.
 **/
Driver::Bh1750::Bh1750(void) : m_isInit(false),
                               m_value(0.0f),
                               m_error(kBhNoError)
{
    // Init the sensor work mode if current interface is worked.
    I2cParameters l_parameters = {
        0,
        s_kBh1750Address,
        kI2cNormalMode,
        false
    };
    // Setting up the sensor only if interface is initialized correctly.
    if (!m_interface.init(l_parameters)) {
        _init();
    }
    else {
        m_error = kBhErrInterfaceIsntInit;
    }
}

/**
 * BH1750 class desctructor.
 **/
Driver::Bh1750::~Bh1750(void)
{
    // Turn the sensor into the sleep mode on object destruction.
    if (m_isInit && m_error == kBhNoError) {
        toggle(false);
        m_isInit = false;
    }
}

/**
 * Current brightness level receiving.
 * @param[o] float& rValue - reference to the variable that will store the
 *                           current brightness level.
 * @return kBhNoError          - successful updating;
 *         kBhErrIsntInit      - sensor wasn't initialized;
 *         kBhErrSendingOpcode - cannot send data to the sensor;
 *         kBhErrReveingData   - cannot receive data from the sensor.    
 **/
Driver::Bh1750ErrorCodes Driver::Bh1750::getLux(float& rValue)
{
    // Check if the sensor was properly initialized.
    if (!m_isInit) {
        rValue = -1.0f;
        return (m_error = kBhErrIsntInit);
    }
  
    uint8_t l_buffer[2] = { kBhContHResolution, 0x00 };
    uint16_t l_temp; 

    // Set current measurement resolution.
    if (m_interface.sendByte(0x00, l_buffer[0])) {
        rValue = -1.0f;
        return (m_error = kBhErrSendingOpcode);
    }
    HAL_Delay(180);
    // Read current measurement raw data.
    if (m_interface.receiveData(0x00, &l_buffer[0], 2)) {
        rValue = -1.0f;
        return (m_error = kBhErrReveingData);
    }
    
    // Convert raw data into real brightness level.
    l_temp = ((uint16_t)l_buffer[0] << 8) | l_buffer[1];
    m_value = 0.0f;
    for (uint8_t i = 0; i < 16; i ++) {
        m_value += ((l_temp >> i) & 0x0001) * (0x0001 << i);
    }
    m_value /= 1.2f;
    // Assigned input reference by calculated value.
    rValue = m_value;
    
    return (m_error = kBhNoError);
}

/**
 * BH1750 sensor initialization.
 * @param[i] bool isTurnedOn - flag that decides to turn on/off the sensor.
 * @return kBhNoError          - successful mode toggling;
 *         kBhErrIsntInit      - the sensor wasn't initialized properly;
 *         kBhErrSendingOpcode - cannot send opcode via used interface.
 **/
Driver::Bh1750ErrorCodes Driver::Bh1750::toggle(bool isTurnedOn)
{
    // Check if the sensor was properly initialized.
    if (!m_isInit) {
        return (m_error = kBhErrIsntInit);
    }
  
    // Turn sensor into requested mode.
    if (isTurnedOn) {
        if (m_interface.sendByte(0x00, kBhPowerOn)) {
            return (m_error = kBhErrSendingOpcode);
        }      
    }
    else {
        m_interface.sendByte(0x00, kBhPowerDown);
    }
    
    return (m_error = kBhNoError);
}