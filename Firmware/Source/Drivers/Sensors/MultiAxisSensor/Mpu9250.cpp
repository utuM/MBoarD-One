/**
  * @file    Mpu9250.cpp
  * @version 0.1.0
  * @author  utuM (Kostyantyn Komarov)
  * @data    25.03.2019 (creation)
  * @brief   MPU9250 driver class implementation.
  **/

#include "Mpu9250.h"

Driver::Mpu9250::Mpu9250(void)
{

}

Driver::Mpu9250::~Mpu9250(void)
{

}

void Driver::Mpu9250::getAccelerator(float& x, float& y, float& z)
{
    uint8_t l_byte[6] = {0};
    // Get accelerometer X axis.
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_ACCEL_XOUT_H, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[0], 1, 1000);
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_ACCEL_XOUT_L, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[1], 1, 1000);
    // Get accelerometer Y axis.
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_ACCEL_YOUT_H, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[2], 1, 1000);
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_ACCEL_YOUT_L, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[3], 1, 1000);
    // Get accelerometer Z axis.
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_ACCEL_ZOUT_H, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[4], 1, 1000);
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_ACCEL_ZOUT_L, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[5], 1, 1000);
    
    x = (float)((uint16_t)l_byte[0] << 8 | l_byte[1]);
    y = (float)((uint16_t)l_byte[2] << 8 | l_byte[3]);
    z = (float)((uint16_t)l_byte[4] << 8 | l_byte[5]);
}

void Driver::Mpu9250::getGyroscope(float& x, float& y, float& z)
{
    uint8_t l_byte[6] = {0};
    // Get gyroscope X axis.
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_GYRO_XOUT_H, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[0], 1, 1000);
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_GYRO_XOUT_L, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[1], 1, 1000);
    // Get gyroscope Y axis.
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_GYRO_YOUT_H, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[2], 1, 1000);
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_GYRO_YOUT_L, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[3], 1, 1000);
    // Get gyroscope Z axis.
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_GYRO_ZOUT_H, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[4], 1, 1000);
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_GYRO_ZOUT_L, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[5], 1, 1000);
    
    x = (float)((uint16_t)l_byte[0] << 8 | l_byte[1]);
    y = (float)((uint16_t)l_byte[2] << 8 | l_byte[3]);
    z = (float)((uint16_t)l_byte[4] << 8 | l_byte[5]);
}

void Driver::Mpu9250::getMagnitometer(float& x, float& y, float& z)
{
    uint8_t l_byte[6] = {0};
    // Get magnitometer X axis.
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250MagnAddress, MPU9250_HXH, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[0], 1, 1000);
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250MagnAddress, MPU9250_HXL, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[1], 1, 1000);
    // Get magnitometer Y axis.
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250MagnAddress, MPU9250_HYH, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[2], 1, 1000);
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250MagnAddress, MPU9250_HYL, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[3], 1, 1000);
    // Get magnitometer Z axis.
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250MagnAddress, MPU9250_HZH, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[4], 1, 1000);
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250MagnAddress, MPU9250_HZL, 
                      I2C_MEMADD_SIZE_8BIT, &l_byte[5], 1, 1000);
    
    x = (float)((uint16_t)l_byte[0] << 8 | l_byte[1]);
    y = (float)((uint16_t)l_byte[2] << 8 | l_byte[3]);
    z = (float)((uint16_t)l_byte[4] << 8 | l_byte[5]);
}

bool Driver::Mpu9250::selfTest(void)
{
    uint8_t l_id = 0;
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250Address, MPU9250_WHO_AM_I, 
                      I2C_MEMADD_SIZE_8BIT, &l_id, 1, 1000);

    return (l_id == 0x73 ? true : false);
}

bool Driver::Mpu9250::magnSelfTest(void)
{
    uint8_t l_id = 0;
    HAL_I2C_Mem_Read(&hi2c1, s_kMpu9250MagnAddress, MPU9250_WIA, 
                      I2C_MEMADD_SIZE_8BIT, &l_id, 1, 1000);

    return (l_id == 0x48 ? true : false);
}