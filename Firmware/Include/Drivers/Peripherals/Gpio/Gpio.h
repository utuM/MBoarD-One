/**
  * @file    Gpio.h
  * @version 0.1.0a
  * @author  utuM (Kostyantyn Komarov)
  * @data    19.03.2019 (creation)
  * @brief   -//-
  **/

#ifndef __PERIPHERAL_GPIO_H
#define __PERIPHERAL_GPIO_H

#include "Peripheral.h"
#include "stm32h7xx_hal.h"

#define GPIO_TYPE_INPUT    0x01
#define GPIO_TYPE_OUTPUT   0x02

namespace Driver {
	class Gpio : public Peripheral {
		private:	
			enum GpioError {
				kGpioNoError = 0x00
			};
		
			bool m_isInit;
			GpioError m_error;
			
			// Private class methods.
			GpioError _init();
			GpioError _deinit();
			
		public:			
			enum GpioPort {
                kGpioPortA = GPIOA,
                kGpioPortB = GPIOB,
                kGpioPortC = GPIOC,
                kGpioPortD = GPIOD,
                kGpioPortE = GPIOE,
                kGpioPortF = GPIOF,
                kGpioPortG = GPIOG,
                kGpioPortH = GPIOH
			};
			
			enum GpioPin {
                kGpioPin0  = GPIO_PIN_0,
                kGpioPin1  = GPIO_PIN_1,
                kGpioPin2  = GPIO_PIN_2,
                kGpioPin3  = GPIO_PIN_3,
                kGpioPin4  = GPIO_PIN_4,
                kGpioPin5  = GPIO_PIN_5,
                kGpioPin6  = GPIO_PIN_6,
                kGpioPin7  = GPIO_PIN_7,
                kGpioPin8  = GPIO_PIN_8,
                kGpioPin9  = GPIO_PIN_9,
                kGpioPin10 = GPIO_PIN_10,
                kGpioPin11 = GPIO_PIN_11,
                kGpioPin12 = GPIO_PIN_12,
                kGpioPin13 = GPIO_PIN_13,
                kGpioPin14 = GPIO_PIN_14,
                kGpioPin15 = GPIO_PIN_15
			};
		
			Gpio(void) : m_isInit(false) { /* To prevent false init. */ }
			Gpio(GpioPort port, GpioPin pin);
			~Gpio(void);
			// Getters and setters.
			uint8_t getError(void) { return (uint8_t)m_error; }
	};
}

#endif // __PERIPHERAL_GPIO_H