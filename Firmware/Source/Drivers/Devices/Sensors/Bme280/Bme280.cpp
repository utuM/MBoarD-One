/**
 *  @file       Bme280.cpp (module file)
 *  @version    1.0
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       10.12.2018 (release)
 *  @brief      BME280 sensor class.
 *              Sensor provides to receive data about pressure, temperature
 *              and humidity of environment. 
 **/

#include "Bme280.h"

Driver::Bme280ErrorCodes Driver::Bme280::_init(Bme280Patterns pattern)
{
    // Initialized array by input device work pattern.
    uint8_t l_settings[6] = {0};
    switch (pattern) {
        // "Weather" pattern.
        case BME280_PATTERN_WEATHER: {
            l_settings[0] = (uint8_t)BME280_NORMAL_MODE;
            l_settings[1] = (uint8_t)BME280_STANDBY_62_5MS;
            l_settings[2] = (uint8_t)BME280_PRESS_ORS_1;
            l_settings[3] = (uint8_t)BME280_TEMPR_ORS_1;
            l_settings[4] = (uint8_t)BME280_HUMID_ORS_1;
            l_settings[5] = (uint8_t)BME280_FILTER_OFF;
        } break;
        // "Humidity" pattern.
        case BME280_PATTERN_HUMIDITY: {
            l_settings[0] = (uint8_t)BME280_NORMAL_MODE;
            l_settings[1] = (uint8_t)BME280_STANDBY_500MS;
            l_settings[2] = (uint8_t)BME280_PRESS_ORS_SKIPPED;
            l_settings[3] = (uint8_t)BME280_TEMPR_ORS_1;
            l_settings[4] = (uint8_t)BME280_HUMID_ORS_1;
            l_settings[5] = (uint8_t)BME280_FILTER_OFF;
        } break;
        // "Indoor" pattern.
        case BME280_PATTERN_INDOOR: {
            l_settings[0] = (uint8_t)BME280_NORMAL_MODE;
            l_settings[1] = (uint8_t)BME280_STANDBY_500MS;
            l_settings[2] = (uint8_t)BME280_PRESS_ORS_16;
            l_settings[3] = (uint8_t)BME280_TEMPR_ORS_2;
            l_settings[4] = (uint8_t)BME280_HUMID_ORS_1;
            l_settings[5] = (uint8_t)BME280_FILTER_16;
        } break;
        // "Gaming" pattern.
        case BME280_PATTERN_GAMING: {
            l_settings[0] = (uint8_t)BME280_NORMAL_MODE;
            l_settings[1] = (uint8_t)BME280_STANDBY_62_5MS;
            l_settings[2] = (uint8_t)BME280_PRESS_ORS_4;
            l_settings[3] = (uint8_t)BME280_TEMPR_ORS_1;
            l_settings[4] = (uint8_t)BME280_HUMID_ORS_SKIPPED;
            l_settings[5] = (uint8_t)BME280_FILTER_16;
        } break;
        
        default: {

        } break;
    }
    
    // Setting up 'BME280_CONFIG' register.
    _turnPower(false);
    m_buffer[0] = l_settings[1] | l_settings[5];
    if (m_interface.sendByte(BME280_CONFIG, m_buffer[0])) {
        return BME280_ERR_CONFIG_DATA;
    }
    _turnPower(true);
    // Setting up 'BME280_CTRL_HUM' register.
    m_buffer[0] = l_settings[4];
    if (m_interface.sendByte(BME280_CTRL_HUM, m_buffer[0])) {
        return BME280_ERR_CTRL_HUM_DATA;
    }
    // Setting up 'BME280_CTRL_MEAS' register.
    m_buffer[0] = l_settings[0] | l_settings[2] | l_settings[3];
    m_bufferSize = BME280_BUFFER_SIZE;
    if (m_interface.sendByte(BME280_CTRL_MEAS, m_buffer[0])) {
        return BME280_ERR_CTRL_MEAS_DATA;
    }
    // Read componensation data registers.
    _readCompensationData();
    
    return BME280_NOERROR;
}

Driver::Bme280ErrorCodes Driver::Bme280::_turnPower(bool isPowerOn)
{
    uint8_t l_regValue;
    // Read 'CTRL_MEAS' register before change.
    if (m_interface.receiveByte(BME280_CTRL_MEAS, l_regValue)) {
        return BME280_ERR_READ_DATA;
    }
    // Change register work mode bit according to input state.
    isPowerOn == true ? l_regValue |= 0x03 : l_regValue &= 0xFFFC;
    if (m_interface.sendByte(BME280_CTRL_MEAS, l_regValue)) {
        return BME280_ERR_WRITE_DATA;
    }

    return BME280_NOERROR;
}

Driver::Bme280ErrorCodes Driver::Bme280::_readCompensationData(void)
{    
    // Read temperature compensation registers first.
    memset(&m_buffer[0], 0x00, TEMPR_COMPENS_SIZE * 2);
    if (m_interface.receiveData(BME280_COMPENS_T1, &m_buffer[0], 
                                TEMPR_COMPENS_SIZE * 2)) {
        return BME280_ERR_COMPENS_T;
    }
    m_temperCompensData[0] = m_buffer[0] | (int16_t)m_buffer[1] << 8;
    m_temperCompensData[1] = m_buffer[2] | (int16_t)m_buffer[3] << 8;
    m_temperCompensData[2] = m_buffer[4] | (int16_t)m_buffer[5] << 8;
    // Read pressure compensation registers.
    memset(&m_buffer[0], 0x00, PRESS_COMPENS_SIZE * 2);
    if (m_interface.receiveData(BME280_COMPENS_P1, &m_buffer[0], 
                                PRESS_COMPENS_SIZE * 2)) {
        return BME280_ERR_COMPENS_P;
    }
    m_pressureCompensData[0] = m_buffer[0]  | (int16_t)m_buffer[1]  << 8;
    m_pressureCompensData[1] = m_buffer[2]  | (int16_t)m_buffer[3]  << 8;
    m_pressureCompensData[2] = m_buffer[4]  | (int16_t)m_buffer[5]  << 8;
    m_pressureCompensData[3] = m_buffer[6]  | (int16_t)m_buffer[7]  << 8;
    m_pressureCompensData[4] = m_buffer[8]  | (int16_t)m_buffer[9]  << 8;
    m_pressureCompensData[5] = m_buffer[10] | (int16_t)m_buffer[11] << 8;
    m_pressureCompensData[6] = m_buffer[12] | (int16_t)m_buffer[13] << 8;
    m_pressureCompensData[7] = m_buffer[14] | (int16_t)m_buffer[15] << 8;
    m_pressureCompensData[8] = m_buffer[16] | (int16_t)m_buffer[17] << 8;
    // Read humidity compensation registers.
    if (m_interface.receiveByte(BME280_COMPENS_H1, m_buffer[0])) {
        return BME280_ERR_COMPENS_H1;
    }
    m_humidityCompensData[0] = m_buffer[0];
    memset(&m_buffer[0], 0x00, 7);
    if (m_interface.receiveData(BME280_COMPENS_H2, &m_buffer[0], 7)) {
        return BME280_ERR_COMPENS_H2;
    }
    m_humidityCompensData[1] = (int16_t)m_buffer[1] << 8 | m_buffer[0];
    m_humidityCompensData[2] = m_buffer[2];
    m_humidityCompensData[3] = (int16_t)m_buffer[3] << 4 | (m_buffer[4] & 0x0F);
    m_humidityCompensData[4] = (int16_t)m_buffer[5] << 4 | 
                                                    ((m_buffer[4] & 0xF0) >> 4);
    m_humidityCompensData[5] = m_buffer[6];
    
    return BME280_NOERROR;
}

void Driver::Bme280::_getTemperature(bool isKelvin)
{
    // Read data from temperature registers.
    memset(&m_buffer[0], 0x00, 3);
    if (m_interface.receiveData(BME280_TEMPERATURE_MSB, &m_buffer[0], 3)) {
        return;
    }
    m_temperCode = (int)m_buffer[0] << 12 | (int)m_buffer[1] << 4 |
                                            (int)m_buffer[2] >> 4;
    // Calculater temperature code.
    int l_value1, l_value2, l_tValue;
    float l_value = 0;
    
    l_value1 = ((((m_temperCode >> 3) - ((int)m_temperCompensData[0] << 1))) *
               ((int)m_temperCompensData[1])) >> 11;
    l_value2 = (((((m_temperCode >> 4) - ((int)m_temperCompensData[0])) *
               ((m_temperCode >> 4) - ((int)m_temperCompensData[0]))) >> 12) *
               ((int)m_temperCompensData[2])) >> 14;
    m_tCompensCode = l_value1 + l_value2;
    l_tValue = (m_tCompensCode * 5 + 128) >> 8;
    
    // Calculate final value.
    l_value = l_tValue * 0.01;
    // Convert value if needed to Kelvins.
    if (isKelvin) {
        l_value = CELS2KELV(l_value);
    }
    m_temperature = l_value;
}

void Driver::Bme280::_getPressure(bool isMmhg)
{
    // Read data from pressure registers.
    if (m_interface.receiveData(BME280_PRESSURE_MSB, &m_buffer[0], 3)) {
        return;
    }
    m_pressureCode = (int)m_buffer[0] << 12 | (int)m_buffer[1] << 4 |
                                              (int)m_buffer[2] >> 4;
    // Calculate pressure code.
    int64_t l_value1 = 0,
            l_value2 = 0,
            l_result = 0;
    
    l_value1 = (int64_t)m_tCompensCode - 128000;
    l_value2 = l_value1 * l_value1 * (int64_t)m_pressureCompensData[5];
    l_value2 = l_value2 + ((l_value1 * (int64_t)m_pressureCompensData[4]) << 17);
    l_value2 = l_value2 + (((int64_t)m_pressureCompensData[3]) << 35);
    l_value1 = ((l_value1 * l_value1 * (int64_t)m_pressureCompensData[2]) >> 8) +
               ((l_value1 * (int64_t)m_pressureCompensData[1]) << 12);
    l_value1 = (((((int64_t)1) << 47) + l_value1)) *
               ((int64_t)((uint16_t)m_pressureCompensData[0])) >> 33;
    if (l_value1 == 0) {
        l_result = 0;
    }
    else {
        l_result = 1048576 - m_pressureCode;
        l_result = (((l_result << 31) - l_value2) * 3125) / l_value1;
        l_value1 = (((int64_t)m_pressureCompensData[8]) * (l_result >> 13) *
                   (l_result >> 13)) >> 25;
        l_value2 = (((int64_t)m_pressureCompensData[7]) * l_result) >> 19;
        l_result = ((l_result + l_value1 + l_value2) >> 8) + 
                   (((int64_t)m_pressureCompensData[6]) << 4);
    }
    // Calculate final value.
    if (l_result != 0) {
        m_pressure = (uint32_t)l_result / (256.0 * 100);
    }
    else {
        m_pressure = (uint32_t)l_result;
    }
    // Convert value if needed into mmHg.
    if (isMmhg) {
        m_pressure = HPA2MMHG(m_pressure);
    }
}

void Driver::Bme280::_getHumidity(void)
{
    // Read data from humidity registers.
    if (m_interface.receiveData(BME280_HUMIDITY_MSB, &m_buffer[0], 2)) {
        return;
    }
    m_humidityCode = (int)m_buffer[0] << 8 | m_buffer[1];
    // Calculate humidity code.
    int l_value = m_tCompensCode - (int)76800;
    l_value = (((((m_humidityCode << 14) -
              (((int)m_humidityCompensData[3] << 20) -
              ((int)m_humidityCompensData[4]) * l_value)) +
              ((int)16384)) >> 15) *
              (((((((l_value * ((int)m_humidityCompensData[5])) >> 10) *
              (((l_value * ((int)m_humidityCompensData[2])) >> 11) +
              ((int)32768))) >> 10) + ((int)2097152)) *
              ((int)m_humidityCompensData[1]) + 8192) >> 14));
    l_value = (l_value - (((((l_value >> 15) * (l_value >> 15)) >> 7) *
              ((int)m_humidityCompensData[0])) >> 4));
    l_value = (l_value < 0 ? 0 : l_value);
    l_value = (l_value > 419430400 ? 419430400 : l_value);
    m_humidity = ((uint32_t)l_value >> 12) / 1024.0;
}

Driver::Bme280::Bme280(void) : Device("BME280")
{
    // Zeroing all basic object fields.
    m_pressure = m_temperature = m_humidity = 0.0;
    m_tCompensCode = 0;
    // Prepare interface for communication.
    I2cParameters l_parameters = {
        0,
        s_kBme280Address,
        kI2cNormalMode
    };
    m_interface.init(l_parameters);
    
    _init(BME280_PATTERN_INDOOR);
}

Driver::Bme280::Bme280(Bme280Patterns pattern) : Device("BME280")
{
    // Zeroing all basic object fields.
    m_pressure = m_temperature = m_humidity = 0.0;
    m_tCompensCode = 0;
    // Prepare interface for communication.
    I2cParameters l_parameters = {
        0,
        s_kBme280Address,
        kI2cNormalMode
    };
    m_interface.init(l_parameters);

    _init(pattern);
}

Driver::Bme280::~Bme280(void)
{

}

void Driver::Bme280::update(float& temperature, float& pressure, float& humidity)
{
    // Get values.
    _getTemperature(false);
    _getPressure(true);
    _getHumidity();

    temperature = m_temperature;
    pressure = m_pressure;
    humidity = m_humidity;
}