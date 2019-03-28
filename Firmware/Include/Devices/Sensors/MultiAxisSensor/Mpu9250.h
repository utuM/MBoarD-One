/**
  * @file    Mpu9250.h
  * @version 0.1.0
  * @author  utuM (Kostyantyn Komarov)
  * @data    25.03.2019 (creation)
  * @brief   MPU9250 driver class implementation.
  **/

#ifndef __DRIVER_MPU9250_H
#define __DRIVER_MPU9250_H

#include "Device.h"
#include "I2c.h"

namespace Driver {
    const uint8_t s_kMpu9250Address     = 0x68 << 1;
    const uint8_t s_kMpu9250MagnAddress = 0x0C << 1;

    class Mpu9250 : public Device<I2c> {
        public:
            enum Mpu9250ErrorCode : uint8_t {
                kMpu9250NoError              = 0x00,
                kMpu9250ErrInterfaceIsntInit,
                kMpu9250ErrAccelDataReading,
                kMpu9250ErrGyroDataReading,
                kMpu9250ErrMagnDataReading,
            };
      
        private:
            bool m_isInit;
            float m_accelerometer[3];
            float m_gyroscope[3];
            float m_magnetometer[3];
            Mpu9250ErrorCode m_error;
            
            Mpu9250ErrorCode _init(void);

        public:
            /**
              * MPU9250 accelerometer and gyroscope related registers.
              **/
            enum Mpu9250Registers : uint8_t {
                // Gyroscope self-test registers.
                kMpu9250StestXGyro          = 0x00,
                kMpu9250StestYGyro,
                kMpu9250StestZGyro,
                // Accelerometer self-test registers.
                kMpu9250StestXAccel         = 0x0D,
                kMpu9250StestYAccel,
                kMpu9250StestZAccel,
                // Gyroscope offset registers.
                kMpu9250XgOffsetH           = 0x13,
                kMpu9250XgOffsetL,
                kMpu9250YgOffsetH,
                kMpu9250YgOffsetL,
                kMpu9250ZgOffsetH,
                kMpu9250ZgOffsetL,
                // Sample rate divider.
                kMpu9250SampleRateDiv,
                // Configuration registers.
                kMpu9250Config,
                kMpu9250GyroConfig,
                kMpu9250AccelConfig,
                kMpu9250AccelConfig2,
                // Low power accelerometer ODR control.
                kMpu9250LPAccelODR,
                // Wake-on motion threshold.
                kMpu9250WakeOnMotion,
                // FIFO enabling register.
                kMpu9250FifoEn              = 0x23,
                // I2C master control.
                kMpu9250I2cMasterControl,
                // I2C slave 0 control.
                kMpu9250I2cSlave0Addr,
                kMpu9250I2cSlave0Size,
                kMpu9250I2cSlave0Ctrl,
                // I2C slave 1 control.
                kMpu9250I2cSlave1Addr,
                kMpu9250I2cSlave1Size,
                kMpu9250I2cSlave1Ctrl,
                // I2C slave 2 control.
                kMpu9250I2cSlave2Addr,
                kMpu9250I2cSlave2Size,
                kMpu9250I2cSlave2Ctrl,
                // I2C slave 3 control.
                kMpu9250I2cSlave3Addr,
                kMpu9250I2cSlave3Size,
                kMpu9250I2cSlave3Ctrl,
                // I2C slave 4 control.
                kMpu9250I2cSlave4Addr,
                kMpu9250I2cSlave4Size,
                kMpu9250I2cSlave4DO,
                kMpu9250I2cSlave4Ctrl,
                kMpu9250I2cSlave4DI,
                // I2C master status.
                kMpu9250I2cMasterStatus,
                // INT Pin / Bypass enable configuration.
                kMpu9250IntBypassEn,
                // Interrupt enable register.
                kMpu9250IntEnable,
                // Interrupt status register.
                kMpu9250IntStatus           = 0x3A,
                // Accelerometer measurements.
                kMpu9250AccelXOutH,
                kMpu9250AccelXOutL,
                kMpu9250AccelYOutH,
                kMpu9250AccelYOutL,
                kMpu9250AccelZOutH,
                kMpu9250AccelZOutL,
                // Temperature measurements.
                kMpu9250TempOutH,
                kMpu9250TempOutL,
                // Gyroscope measurements.
                kMpu9250GyroXOutH,
                kMpu9250GyroXOutL,
                kMpu9250GyroYOutH,
                kMpu9250GyroYOutL,
                kMpu9250GyroZOutH,
                kMpu9250GyroZOutL,
                // External sensor data.
                kMpu9250ExtSensData00, 
                kMpu9250ExtSensData01,
                kMpu9250ExtSensData02,
                kMpu9250ExtSensData03,
                kMpu9250ExtSensData04,
                kMpu9250ExtSensData05,
                kMpu9250ExtSensData06,
                kMpu9250ExtSensData07,
                kMpu9250ExtSensData08,
                kMpu9250ExtSensData09,
                kMpu9250ExtSensData10,
                kMpu9250ExtSensData11,
                kMpu9250ExtSensData12,
                kMpu9250ExtSensData13,
                kMpu9250ExtSensData14,
                kMpu9250ExtSensData15,
                kMpu9250ExtSensData16,
                kMpu9250ExtSensData17,
                kMpu9250ExtSensData18,
                kMpu9250ExtSensData19,
                kMpu9250ExtSensData20,
                kMpu9250ExtSensData21,
                kMpu9250ExtSensData22,
                kMpu9250ExtSensData23,
                // I2C slaves data out.
                kMpu9250I2cSlave0DO         = 0x63,
                kMpu9250I2cSlave1DO,
                kMpu9250I2cSlave2DO,
                kMpu9250I2cSlave3DO,
                // I2C master delay control.
                kMpu9250I2cMasterDelayCtrl,
                // Signal path reset.
                kMpu9250SignalPathReset,
                // Accelerometer interrupt control.
                kMpu9250MotionDetCtrl,
                // User control.
                kMpu9250UserCtrl,
                // Power management registers.
                kMpu9250PwrManagement1,
                kMpu9250PwrManagement2,
                // FIFO count registers.
                kMpu9250FifoCountH          = 0x72,
                kMpu9250FifoCountL,
                // FIFO read/write register.
                kMpu9250FifoRW,
                // Get sensor ID.
                kMpu9250WhoAmI,
                // Accelerometer offset registers.
                kMpu9250XAOffsetH           = 0x77,
                kMpu9250XAOffsetL,
                kMpu9250YAOffsetH           = 0x7A,
                kMpu9250YAOffsetL,
                kMpu9250ZAOffsetH           = 0x7D,
                kMpu9250ZAOffsetL
            };
            /**
              * Magnitometer registers (to get access to the magnitometer data 
              * need to use AUX-CL and AUX-DA pins).
              **/
            enum Mpu9250MagnRegisters : uint8_t {
                // Device ID.
                kMpu9250MagnWhoAmI    = 0x00,
                // Information register.
                kMpu9250Info,
                // Status 1 register.
                kMpu9250Status1,
                // Magnitometer measurement data.
                kMpu9250MagnXL,
                kMpu9250MagnXH,
                kMpu9250MagnYL,
                kMpu9250MagnYH,
                kMpu9250MagnZL,
                kMpu9250MagnZH,
                // Status 2 register.
                kMpu9250Status2,
                // Control register.
                kMpu9250Control,
                // Reserved!!!
                kMpu9250Reserved,
                // Self-test register.
                kMpu9250SeftTestReg,
                // Test 1 and Test 2.
                kMpu9250Test1,
                kMpu9250Test2,
                // Disable I2C.
                kMpu9250DisableI2c,
                // Axises sensitibility adjustment values.
                kMpu9250AxisSensAdjX, 
                kMpu9250AxisSensAdjY,
                kMpu9250AxisSensAdjZ
            };
          
            // Gyroscope measurement delay values.
            enum Mpu9250GyroDelay : uint8_t {
                kMpu9250Delay0d97ms   = 0x00,
                kMpu9250Delay2d9ms,
                kMpu9250Delay3d9ms,
                kMpu9250Delay5d9ms,
                kMpu9250Delay9d9ms,
                kMpu9250Delay17d85ms,
                kMpu9250Delay33d48ms,
                kMpu9250Delay0d17ms
            };
            
            // Gyrospoce full scale values.
            enum Mpu9250GyroScale : uint8_t {
                kMpu9250s250dps  = 0x00,
                kMpu9250s500dps  = 0x08,
                kMpu9250s1000dps = 0x10,
                kMpu9250s2000dps = 0x18
            };
            
            // Accelerometer full scale values.
            enum Mpu9250AccelScale : uint8_t {
                kMpu9250s2g  = 0x00,
                kMpu9250s4g  = 0x08,
                kMpu9250s8g  = 0x10,
                kMpu9250s16g = 0x18
            };
            
            // Accelerometer measurement delay values.
            enum Mpu9250AccelDelay : uint8_t {
                kMpu9250AccelDelay1d88ms   = 0x01,
                kMpu9250AccelDelay2d88ms,
                kMpu9250AccelDelay4d88ms,
                kMpu9250AccelDelay8d87ms,
                kMpu9250AccelDelay16d83ms,
                kMpu9250AccelDelay32d48ms,
                kMpu9250AccelDelay1d38ms
            };
            
            // Accelerometer low power ODR control.
            enum Mpu9250AccelLPODR : uint8_t {
                kMpu9250Accel0d24Hz   = 0x00,
                kMpu9250Accel0d49Hz,
                kMpu9250Accel0d98Hz,
                kMpu9250Accel1d95Hz,
                kMpu9250Accel3d91Hz,
                kMpu9250Accel7d81Hz,
                kMpu9250Accel15d63Hz,
                kMpu9250Accel31d25Hz,
                kMpu9250Accel62d50Hz,
                kMpu9250Accel125Hz,
                kMpu9250Accel250Hz,
                kMpu9250Accel500Hz
            };
            
            // Possible MPU9250 interrupt statuses.
            enum Mpu9250InterruptStatus : uint8_t {
                kMpu9250WakeOnMotion = 0x40,
                kMpu9250FifoOverflow = 0x10,
                kMpu9250OFsync       = 0x08,
                kMpu9250RawDataReady = 0x01
            };
            
            // User control values.
            enum Mpu9250UserControl : uint8_t {
                kMpu9250EnFifo            = 0x40,
                kMpu9250EnI2cIFMasterMode = 0x20,
                kMpu9250EnSpiMode         = 0x10,
                kMpu9250FifoReset         = 0x04,
                kMpu9250I2cMasterReset    = 0x02,
                kMpu9250ResetAllSignals   = 0x01
            };
            
            // Power management 1 values.
            enum Mpu9250PowerManagement1 : uint8_t {
                kMpu9250ResetRegisters = 0x80,
                kMpu9250SleepMode      = 0x40,
                kMpu9250CycleMode      = 0x20,
                kMpu9250GyroStandby    = 0x10,
                kMpu9250AdcPowerDown   = 0x08
            };
            
            // Clock source values.
            enum Mpu9250ClockSource : uint8_t {
                kMpu9250Autoselect    = 0x01,
                kMpu9250Internal20MHz = 0x06,
                kMpu9250ClockStop
            };
            
            // Power management 2 values.
            enum Mpu9250PowerManagement2 : uint8_t {
                kMpu9250DisableAccelX = 0x20,
                kMpu9250DisableAccelY = 0x10,
                kMpu9250DisableAccelZ = 0x08,
                kMpu9250DisableGyroX  = 0x04,
                kMpu9250DisableGyroY  = 0x02,
                kMpu9250DisableGyroZ  = 0x01
            };
            
            // Magnetometer status 2 values.
            enum Mpu9250MagnStatus2 : uint8_t {
                kMpu9250MagnOverflow    = 0x08,
                kMpu9250Magn16bitOutput = 0x10
            };
            
            // Magnetometer control 1 values.
            enum Mpu9250MagnOperationMode : uint8_t {
                kMpu9250MagnPowerDown     = 0x00,
                kMpu9250MagnSingleMeas,
                kMpu9250MagnContMeas1,
                kMpu9250MagnExtTrigMeas   = 0x04,
                kMpu9250MagnContMeas2     = 0x06,
                kMpu9250MagnSelfTest      = 0x08,
                kMpu9250MagnFuseRomAccess = 0x0F,
                kMpu9250Magn16bitOutput
            };
            
            // Other magnetometer values (use only with properly registers).
            enum Mpu9250MagnOthers : uint8_t {
                kMpu9250CTRL2SoftReset       = 0x01,
                kMpu9250SEFTTESTGenMagnField = 0x40
            };
            
            Mpu9250(void);
            ~Mpu9250(void);

            uint8_t getError(void) { return (uint8_t)m_error; }
            
            Mpu9250ErrorCode updateAccelerator(float& rXValue,
                                               float& rYValue,
                                               float& rZValue);
            Mpu9250ErrorCode updateGyroscope(float& rXValue,
                                             float& rYValue,
                                             float& rZValue);
            Mpu9250ErrorCode updateMagnitometer(float& rXValue,
                                                float& rYValue,
                                                float& rZValue);
            bool selfTest(void);
            bool magnSelfTest(void);   
            Mpu9250ErrorCode toggle(bool isPowerOn);
    };
}

#endif // __DRIVER_MPU9250_H