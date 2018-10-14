#ifndef __SENSORS_MPU9250_H
#define __SENSORS_MPU9250_H

#include "DriverDevice.h"
#include "DriverI2c.h"
#include "i2c.h"

namespace Driver {
    const uint8_t s_kMpu9250Address = 0x68;
    const uint8_t s_kMpu9250MagnAddress = 0x0C;
    /**
     *      Major MPU9250 registers.
     **/
    enum Mpu9250Registers {
        // Gyroscope self-test registers.
        MPU9250_STEST_X_GYRO = 0x00,
        MPU9250_STEST_Y_GYRO = 0x01,
        MPU9250_STEST_Z_GYRO = 0x02,
        // Accelerometer self-test registers.
        MPU9250_STEST_X_ACCEL = 0x0D,
        MPU9250_STEST_Y_ACCEL = 0x0E,
        MPU9250_STEST_Z_ACCEL = 0x0F,
        // Gyroscope offset registers.
        MPU9250_XG_OFFSET_H = 0x13,
        MPU9250_XG_OFFSET_L = 0x14,
        MPU9250_YG_OFFSET_H = 0x15,
        MPU9250_YG_OFFSET_L = 0x16,
        MPU9250_ZG_OFFSET_H = 0x17,
        MPU9250_ZG_OFFSET_L = 0x18,
        // Sample rate divider.
        MPU9250_SMPLRT_DIV = 0x19,
        // Configuration registers.
        MPU9250_CONFIG = 0x1A,
        MPU9250_GYRO_CONFIG = 0x1B,
        MPU9250_ACCEL_CONFIG = 0x1C,
        MPU9250_ACCEL_CONFIG_2 = 0x1D,
        // Low power accelerometer ODR control.
        MPU9250_LP_ACCEL_ODR = 0x1E,
        // Wake-on motion threshold.
        MPU9250_WOM_THR = 0x1F,
        // FIFO enabling register.
        MPU9250_FIFO_EN = 0x23,
        // I2C master control.
        MPU9250_I2C_MST_CTRL = 0x24,
        // I2C slave 0 control.
        MPU9250_I2C_SLV0_ADDR = 0x25,
        MPU9250_I2C_SLV0_SIZE = 0x26,
        MPU9250_I2C_SLV0_CTRL = 0x27,
        // I2C slave 1 control.
        MPU9250_I2C_SLV1_ADDR = 0x28,
        MPU9250_I2C_SLV1_SIZE = 0x29,
        MPU9250_I2C_SLV1_CTRL = 0x2A,
        // I2C slave 2 control.
        MPU9250_I2C_SLV2_ADDR = 0x2B,
        MPU9250_I2C_SLV2_SIZE = 0x2C,
        MPU9250_I2C_SLV2_CTRL = 0x2D,
        // I2C slave 3 control.
        MPU9250_I2C_SLV3_ADDR = 0x2E,
        MPU9250_I2C_SLV3_SIZE = 0x2F,
        MPU9250_I2C_SLV3_CTRL = 0x30,
        // I2C slave 4 control.
        MPU9250_I2C_SLV4_ADDR = 0x31,
        MPU9250_I2C_SLV4_SIZE = 0x32,
        MPU9250_I2C_SLV4_DO = 0x33,
        MPU9250_I2C_SLV4_CTRL = 0x34,
        MPU9250_I2C_SLV4_DI = 0x35,
        // I2C master status.
        MPU9250_MST_STATUS = 0x36,
        // INT Pin / Bypass enable configuration.
        MPU9250_INT_PIN_CFG = 0x37,
        // Interrupt enable register.
        MPU9250_INT_ENABLE = 0x38,
        // Interrupt status register.
        MPU9250_INT_STATUS = 0x3A,
        // Accelerometer measurements.
        MPU9250_ACCEL_XOUT_H = 0x3B,
        MPU9250_ACCEL_XOUT_L = 0x3C,
        MPU9250_ACCEL_YOUT_H = 0x3D,
        MPU9250_ACCEL_YOUT_L = 0x3E,
        MPU9250_ACCEL_ZOUT_H = 0x3F,
        MPU9250_ACCEL_ZOUT_L = 0x40,
        // Temperature measurements.
        MPU9250_TEMP_OUT_H = 0x41,
        MPU9250_TEMP_OUT_L = 0x42,
        // Gyroscope measurements.
        MPU9250_GYRO_XOUT_H = 0x43,
        MPU9250_GYRO_XOUT_L = 0x44,
        MPU9250_GYRO_YOUT_H = 0x45,
        MPU9250_GYRO_YOUT_L = 0x46,
        MPU9250_GYRO_ZOUT_H = 0x47,
        MPU9250_GYRO_ZOUT_L = 0x48,
        // External sensor data.
        MPU9250_EXT_SENS_DATA_00 = 0x49,
        MPU9250_EXT_SENS_DATA_01 = 0x4A,
        MPU9250_EXT_SENS_DATA_02 = 0x4B,
        MPU9250_EXT_SENS_DATA_03 = 0x4C,
        MPU9250_EXT_SENS_DATA_04 = 0x4D,
        MPU9250_EXT_SENS_DATA_05 = 0x4E,
        MPU9250_EXT_SENS_DATA_06 = 0x4F,
        MPU9250_EXT_SENS_DATA_07 = 0x50,
        MPU9250_EXT_SENS_DATA_08 = 0x51,
        MPU9250_EXT_SENS_DATA_09 = 0x52,
        MPU9250_EXT_SENS_DATA_10 = 0x53,
        MPU9250_EXT_SENS_DATA_11 = 0x54,
        MPU9250_EXT_SENS_DATA_12 = 0x55,
        MPU9250_EXT_SENS_DATA_13 = 0x56,
        MPU9250_EXT_SENS_DATA_14 = 0x57,
        MPU9250_EXT_SENS_DATA_15 = 0x58,
        MPU9250_EXT_SENS_DATA_16 = 0x59,
        MPU9250_EXT_SENS_DATA_17 = 0x5A,
        MPU9250_EXT_SENS_DATA_18 = 0x5B,
        MPU9250_EXT_SENS_DATA_19 = 0x5C,
        MPU9250_EXT_SENS_DATA_20 = 0x5D,
        MPU9250_EXT_SENS_DATA_21 = 0x5E,
        MPU9250_EXT_SENS_DATA_22 = 0x5F,
        MPU9250_EXT_SENS_DATA_23 = 0x60,
        // I2C slaves data out.
        MPU9250_I2C_SLV0_DO = 0x63,
        MPU9250_I2C_SLV1_DO = 0x64,
        MPU9250_I2C_SLV2_DO = 0x65,
        MPU9250_I2C_SLV3_DO = 0x66,
        // I2C master delay control.
        MPU9250_I2C_MST_DELAY_CTRL = 0x67,
        // Signal path reset.
        MPU9250_SIGNAL_PATH_RESET = 0x68,
        // Accelerometer interrupt control.
        MPU9250_MOT_DETECT_CTRL = 0x69,
        // User control.
        MPU9250_USER_CTRL = 0x6A,
        // Power management registers.
        MPU9250_PWR_MGMT_1 = 0x6B,
        MPU9250_PWR_MGMT_2 = 0x6C,
        // FIFO count registers.
        MPU9250_FIFO_COUNTH = 0x72,
        MPU9250_FIFO_COUNTL = 0x73,
        // FIFO read/write register.
        MPU9250_FIFO_R_W = 0x74,
        // Get sensor ID.
        MPU9250_WHO_AM_I = 0x75,
        // Accelerometer offset registers.
        MPU9250_XA_OFFSET_H = 0x77,
        MPU9250_XA_OFFSET_L = 0x77,
        MPU9250_YA_OFFSET_H = 0x7A,
        MPU9250_YA_OFFSET_L = 0x7B,
        MPU9250_ZA_OFFSET_H = 0x7D,
        MPU9250_ZA_OFFSET_L = 0x7E
    };
    /**
     *      Magnitometer registers.
     **/
    enum Mpu9250MagnRegisters {
        // Device ID.
        MPU9250_WIA = 0x00,
        // Information register.
        MPU9250_INFO = 0x01,
        // Status 1 register.
        MPU9250_ST1 = 0x02,
        // Magnitometer measurement data.
        MPU9250_HXL = 0x03,
        MPU9250_HXH = 0x04,
        MPU9250_HYL = 0x05,
        MPU9250_HYH = 0x06,
        MPU9250_HZL = 0x07,
        MPU9250_HZH = 0x08,
        // Status 2 register.
        MPU9250_ST2 = 0x09,
        // Control register.
        MPU9250_CNTL = 0x0A,
        // Reserved!!!
        MPU9250_RSV = 0x0B,
        // Self-test register.
        MPU9250_ASTC = 0x0C,
        // Test 1 and Test 2.
        MPU9250_TS1 = 0x0D,
        MPU9250_TS2 = 0x0E,
        // Disable I2C.
        MPU9250_I2CDIS = 0x0F,
        // Axises sensitibility adjustment values.
        MPU9250_ASAX = 0x10,
        MPU9250_ASAY = 0x11,
        MPU9250_ASAZ = 0x12
    };

    class Mpu9250 {
        private:
            

        public:
            Mpu9250(void);
            ~Mpu9250(void);
    };
}

#endif // __SENSORS_MPU9250_H