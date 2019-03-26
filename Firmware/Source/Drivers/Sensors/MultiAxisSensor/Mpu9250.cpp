/**
  * @file    Mpu9250.cpp
  * @version 0.1.0
  * @author  utuM (Kostyantyn Komarov)
  * @data    25.03.2019 (creation)
  * @brief   MPU9250 driver class implementation.
  **/

#include "Mpu9250.h"

#include <string.h>

Driver::Mpu9250::Mpu9250ErrorCode Driver::Mpu9250::_init(void)
{
    // Clear up the data buffers.
    memset(m_accelerometer, 0x00, sizeof(float) * 3);
    memset(m_gyroscope, 0x00, sizeof(float) * 3);
    memset(m_magnetometer, 0x00, sizeof(float) * 3);
    
    // Setting up the sensor.
    
}

Driver::Mpu9250::Mpu9250(void) : Device("MPU9250"),
                                 m_isInit(false),
                                 m_error(kMpu9250NoError)
{
    // Prepare interface for communication.
    I2cParameters l_parameters = {
        0,
        s_kMpu9250Address,
        kI2cNormalMode,
        true
    };
    // Initialize the sensor only if used interface was initialized
    // successfully.
    if (!m_interface.init(l_parameters)) {
        _init();
    }
    else {
        m_error = kMpu9250ErrInterfaceIsntInit;
    }
}

Driver::Mpu9250::~Mpu9250(void)
{
    // Turn the sensor into the sleep mode on object destruction.
    if (m_isInit && m_error == kMpu9250NoError) {
        toggle(false);
        m_isInit = false;
    }
}

Driver::Mpu9250::Mpu9250ErrorCode
Driver::Mpu9250::updateAccelerator(float& rXValue,
                                   float& rYValue,
                                   float& rZValue)
{
    uint8_t l_rxBuffer[6] = {0};
    // Read all accelerometer axises data once.
    if (m_interface.receiveData(kMpu9250AccelXOutH, &l_rxBuffer[0], 6)) {
        return (m_error = kMpu9250ErrAccelDataReading);
    }
    
    x = (float)((uint16_t)l_rxBuffer[0] << 8 | l_rxBuffer[1]);
    y = (float)((uint16_t)l_rxBuffer[2] << 8 | l_rxBuffer[3]);
    z = (float)((uint16_t)l_rxBuffer[4] << 8 | l_rxBuffer[5]);
}

Driver::Mpu9250::Mpu9250ErrorCode
Driver::Mpu9250::updateGyroscope(float& rXValue,
                                 float& rYValue,
                                 float& rZValue)
{
    uint8_t l_rxBuffer[6] = {0};
    // Read all gyroscope axises data once.
    if (m_interface.receiveData(kMpu9250GyroXOutH, &l_rxBuffer[0], 6)) {
        return (m_error = kMpu9250ErrGyroDataReading);
    }
    
    x = (float)((uint16_t)l_rxBuffer[0] << 8 | l_rxBuffer[1]);
    y = (float)((uint16_t)l_rxBuffer[2] << 8 | l_rxBuffer[3]);
    z = (float)((uint16_t)l_rxBuffer[4] << 8 | l_rxBuffer[5]);
}

Driver::Mpu9250::Mpu9250ErrorCode
Driver::Mpu9250::updateMagnitometer(float& rXValue,
                                    float& rYValue,
                                    float& rZValue)
{
    uint8_t l_rxBuffer[6] = {0};
    // Get magnitometer X axis.
    if (m_interface.receiveData(kMpu9250MagnXL, &l_rxBuffer[0], 6)) {
        return (m_error = kMpu9250ErrMagnDataReading);
    }
    
    x = (float)((uint16_t)l_rxBuffer[1] << 8 | l_rxBuffer[0]);
    y = (float)((uint16_t)l_rxBuffer[3] << 8 | l_rxBuffer[2]);
    z = (float)((uint16_t)l_rxBuffer[5] << 8 | l_rxBuffer[4]);
}

bool Driver::Mpu9250::selfTest(void)
{
    uint8_t l_id = 0;
    // Self-test request.
    if (m_interface.receiveData(kMpu9250WhoAmI, &l_id, 1)) {
        m_error = kMpu9250ErrMagnDataReading;
        return false;
    }

    return (l_id == 0x73 ? true : false);
}

bool Driver::Mpu9250::magnSelfTest(void)
{
    uint8_t l_id = 0;
    // Magnetometer self-test request.
    if (m_interface.receiveData(kMpu9250MagnWhoAmI, &l_id, 1)) {
        m_error = kMpu9250ErrMagnDataReading;
        return false;
    }

    return (l_id == 0x48 ? true : false);
}

Driver::Mpu9250::Mpu9250ErrorCode Driver::Mpu9250::toggle(bool isPowerOn)
{

}