/**
 *	@file		Timer.h
 *	@version	1.0
 *	@author		utuM (Kostyantyn Komarov)
 *	@date		21.11.2018 (release)
 *  @brief		Timer peripheral class implementation.
 *				This class allows to use two basic counter timers TIM6 and TIM7
 *              according to the reference manual to the STM32F746ZG controller.
 *              Maximal time interval is limited by 's_kMaxTimerPeriod' constant
 *              value and equal to 1 minute.
 *              Also, every single tick calls interrupt function that triggers
 *              object flag. The flag's state can be received by the
 *              'getUpdated()' method.
 *  @todo       Automatically calculate prescaler value according to controller
 *              clocks frequencies to make period value equal to the
 *              milliseconds.
 */

#ifndef __DRIVER_TIMER_H
#define __DRIVER_TIMER_H

#include <stdint.h>

#include "DriverPeripheral.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_it.h"

namespace Driver {
    const uint16_t s_kMaxTimerPeriod = 60000; ///< Maximal time interval between
                                              ///< ticks.
    
    /**
     *  Timer parameters structure.
     **/
    struct TimerParameters {
        uint8_t m_index;        ///< Timer index: 0 - TIM6, 1 - TIM7.
        uint16_t m_period;      ///< Timer period. Must be equal to milliseconds
                                ///  amount and limited by 's_kMaxTimerPeriod'
                                ///  constant value.
        uint16_t m_prescaler;   ///< Prescaler value.
    };
    
    /**
     *  Timer errors.
     **/
    enum TimerError : uint8_t {
        kTimerNoError = 0x00,   ///< No error.
        kTimerInvalidIndex,     ///< Input timer index is invalid (above 1).
        kTimerAlreadyInit,      ///< Timer by input index is already in use.
        kTimerInvalidParameter  ///< One of input object's field is invalid 
                                ///  (equal to 0).
    };
    
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
            TimerError _uninit(void);
            
        public:
            Timer(void) : m_isInit(false) { }
            Timer(TimerParameters& parameters);
            ~Timer(void);
            
            uint8_t getError(void) { return (uint8_t)m_error; }
            bool getUpdated(void) { return m_isUpdated; }
            void setUpdated(bool state) { m_isUpdated = state; }
            void toggle(bool needToEnable);
    };
}

#endif // __DRIVER_TIMER_H