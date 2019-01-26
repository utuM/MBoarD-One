/**
 *  @file       Timer.h (header file)
 *  @version    1.0
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       07.12.2018 (release)
 *  @brief      Timer peripheral class implementation.
 *              This class allows to use two basic counter timers TIM6 and TIM7
 *              according to the reference manual to the STM32F746ZG controller.
 *              Maximal time interval is limited by 's_kMaxTimerPeriod' constant
 *              value and equal to 1 minute.
 *              Also, every single tick calls interrupt function that triggers
 *              object flag. The flag's state can be received by the
 *              'getUpdated()' method.
 */

#ifndef __PERIPHERAL_TIMER_H
#define __PERIPHERAL_TIMER_H

#include <stdint.h>

#include "Peripheral.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_it.h"

namespace Driver {
    const uint16_t s_kMaxTimerPeriod = 60000;   ///< Maximal time interval
                                                ///  between ticks.
    const uint8_t s_kDefaultApb1Divider = 8;    ///< Default APB1 bus prescaler
                                                ///  divider (Notice: it can be
                                                ///  different than defined).
    
    /**
     *  Timer parameters structure.
     **/
    struct TimerParameters {
        uint8_t m_index;        ///< Timer index: 0 - TIM6, 1 - TIM7.
        uint16_t m_period;      ///< Timer period. Must be equal to milliseconds
                                ///  amount and limited by 's_kMaxTimerPeriod'
                                ///  constant value.
    };
    
    /**
     *  Timer errors.
     **/
    enum TimerError : uint8_t {
        kTimerNoError = 0x00,   ///< No error.
        kTimerFailedInit,       ///< Some error or exception was occured during
                                ///< timer initialization in HAL library.
        kTimerInvalidIndex,     ///< Input timer index is invalid (above 1).
        kTimerAlreadyInit,      ///< Timer by input index is already in use.
        kTimerInvalidParameter  ///< One of input object's field is invalid 
                                ///  (equal to 0).
    };
    
    /**
     * Timer class.
     **/
    class Timer : public Peripheral {
        private:
            bool m_isInit;                  ///< Complete initialization flag. 
            TimerParameters m_params;       ///< Timer parameters structure
                                            ///  object.
            TIM_HandleTypeDef m_handler;    ///< Structure object that stores
                                            ///  timer data.
            TimerError m_error;             ///< Current error code.
            bool m_isUpdated;               ///< Timer updating flag.
            bool m_isLaunched;              ///< Timer launching flag.
            // Private class methods.
            TimerError _init(void);
            TimerError _deinit(void);
            
        public:
            // Class constructors and destructor.
            Timer(void) : m_isInit(false) { /* To prevent false init. */ }
            Timer(TimerParameters& parameters);
            ~Timer(void);
            // Getters and setter.
            uint8_t getError(void) { return (uint8_t)m_error; }
            bool getUpdated(void) { return m_isUpdated; }
            void setUpdated(const bool state) { m_isUpdated = state; }
            // Other methods.
            void toggle(const bool needToEnable);
    };
}

#endif // __PERIPHERAL_TIMER_H