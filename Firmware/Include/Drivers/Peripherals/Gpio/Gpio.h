/**
  * @file    Gpio.h
  * @version 1.0.1
  * @author  utuM (Kostyantyn Komarov)
  * @data    19.03.2019 (creation)
  * @data    23.03.2019 (release)
  * @brief   GPIO driver class implementation.
  *          Current driver can initializes every input and output pins are
  *          required. Every pin is represented as a 'InputPin' or 'OutputPin'
  *          class object. Every time on pin initialization, binded port will
  *          be enabling using RCC staff.
  *          For input pins, there is only 'getPinState()'. For output pins,
  *          there are 'setPinState()' and 'togglePin()'.
  *          On object desctructure, current pin will be de-initialized.
  **/

#ifndef __PERIPHERAL_GPIO_H
#define __PERIPHERAL_GPIO_H

#include "Peripheral.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"

namespace Driver {
    /**
      * Basic GPIO class implementation.
      **/
    class Gpio : public Peripheral {
        public:
		    /**
              * Possible GPIO pin positions for one port.
              **/
            enum GpioPin : uint16_t {
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
      
        private:            
            std::string m_name;    ///< Short purpose of pin.
            GPIO_TypeDef* m_pPort; ///< Pointer to global port structure.
            GpioPin m_pin;         ///< Pin position on port.
          
        public:          
		    /**
              * Possible set of GPIO ports (can be different), represented as
			  * addresses to the general port structure object.
              **/
            enum GpioPort : uint32_t {
#if defined(STM32H743xx)
                kGpioPortA = (uint32_t)GPIOA,
                kGpioPortB = (uint32_t)GPIOB,
                kGpioPortC = (uint32_t)GPIOC,
                kGpioPortD = (uint32_t)GPIOD,
                kGpioPortE = (uint32_t)GPIOE,
                kGpioPortF = (uint32_t)GPIOF,
                kGpioPortG = (uint32_t)GPIOG,
                kGpioPortH = (uint32_t)GPIOH
#elif
                error("ERROR! List of ports is not defined!");
#endif
            };
            
            /**
              * Possible GPIO level changing speed.
              **/
            enum GpioSpeed : uint32_t {
                kGpioSpeedLow      = GPIO_SPEED_FREQ_LOW,
                kGpioSpeedMedium   = GPIO_SPEED_FREQ_MEDIUM,
                kGpioSpeedHigh     = GPIO_SPEED_FREQ_HIGH,
                kGpioSpeedVeryHigh = GPIO_SPEED_FREQ_VERY_HIGH
            };
            
            // Constructors and desctructor.
            Gpio(void)  { /* Leave empty for prevent false init. */ }
            Gpio(std::string name) { m_name = name; }
            ~Gpio(void) { /* Leave empty for prevent false init. */ } 
            
            // Setters and getters.
            void setPort(GpioPort port) { m_port = (GPIO_TypeDef*)port; }
            void setPin(GpioPin pin) { m_pin = pin; }
            std::string getPinName(void) { return m_name; }
            GPIO_TypeDef* getPort(void) { return m_port; }
            GpioPin getPin(void) { return m_pin; }
            
            // Basic methods.
            void enablePort(GpioPort port);
            void disablePort(GpioPort port);
    };
    
    /**
      * Child GPIO class implementation for input signals pin.
      **/
    class InputPin : public Gpio {
        private:
            /* Let it be empty. */
          
        public:
		    /**
              * Possible input pins pulls.
              **/
            enum Pull : uint32_t {
                kInputNoPull   = GPIO_NOPULL,
                kInputPullUp   = GPIO_PULLUP,
                kInputPullDown = GPIO_PULLDOWN,
            };
         
            // Constructor and destructor.
            InputPin(std::string name,
                     GpioPort port,
                     GpioPin pin,
                     GpioSpeed speed,
                     Pull pull);
            ~InputPin(void);
            
            // Basic method.
            bool getPinState(void);
    };

    /**
      * Child GPIO class implementation for output signals pin.
      **/
    class OutputPin : public Gpio {
        private:
            /* Let it be empty. */
          
        public:
            /**
              * Possible output levels.
              **/
            enum State : uint8_t {
                kStateReset = 0x00, // Down to 0V level.
                kStateSet           // Up to 3.3V level.
            };

            // Constructor and destructor.
            OutputPin(std::string name,
                      GpioPort port,
                      GpioPin pin,
                      GpioSpeed speed,
                      State state);
            ~OutputPin(void);
            
            // Basic methods.
            void setPinState(State state);
            void togglePin(void);
    };
}

#endif // __PERIPHERAL_GPIO_H