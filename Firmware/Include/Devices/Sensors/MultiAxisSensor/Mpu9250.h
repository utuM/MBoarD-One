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
        private:
            

        public:
           /**
             * MPU9250 accelerometer and gyroscope related registers.
             **/
            enum Mpu9250Registers {
                // Gyroscope self-test registers.
                kMpu9250StestXGyro  = 0x00,
                kMpu9250StestYGyro,
                kMpu9250StestZGyro,
                // Accelerometer self-test registers.
                kMpu9250StestXAccel = 0x0D,
                kMpu9250StestYAccel,
                kMpu9250StestZAccel,
                // Gyroscope offset registers.
                kMpu9250XgOffsetH   = 0x13,
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
                kMpu9250FifoEn      = 0x23,
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
                kMpu9250IntStatus   = 0x3A,
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
                kMpu9250I2cSlave0DO = 0x63,
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
                kMpu9250FifoCountH  = 0x72,
                kMpu9250FifoCountL,
                // FIFO read/write register.
                kMpu9250FifoRW,
                // Get sensor ID.
                kMpu9250WhoAmI,
                // Accelerometer offset registers.
                kMpu9250XAOffsetH   = 0x77,
                kMpu9250XAOffsetL,
                kMpu9250YAOffsetH   = 0x7A,
                kMpu9250YAOffsetL,
                kMpu9250ZAOffsetH   = 0x7D,
                kMpu9250ZAOffsetL
            };
            /**
              * Magnitometer registers (to get access to the magnitometer data 
              * need to use AUX-CL and AUX-DA pins).
              **/
            enum Mpu9250MagnRegisters {
                // Device ID.
                kMpu9250MagnWhoAmI  = 0x00,
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
          
            Mpu9250(void);
            ~Mpu9250(void);

            void getAccelerator(float& x, float& y, float& z);
            void getGyroscope(float& x, float& y, float& z);
            void getMagnitometer(float& x, float& y, float& z);
    
            bool selfTest(void);
            bool magnSelfTest(void);
    };
}

#endif // __DRIVER_MPU9250_H