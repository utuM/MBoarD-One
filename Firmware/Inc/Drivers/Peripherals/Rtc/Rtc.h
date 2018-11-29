/**
 *	@file		Rtc.h
 *	@version	0.3
 *	@author		utuM (Kostyantyn Komarov)
 *	@date		26.11.2018 (implementation)
 *  @brief		Peripheral RTC module class implementation.
 *				This class describes peripheral device of selected 
 *				microcontroller. 
 */

#ifndef __PERIPHERAL_RTC_H
#define __PERIPHERAL_RTC_H

#include <stdint.h>

#include "DriverPeripheral.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_it.h"

namespace Driver {
    const uint16_t m_kRtcYearConstant = 2019;
    /**
     *  RTC possible errors.
     **/
    enum RtcError : uint8_t {
        kRtcNoError = 0x00, 
        kRtcFailedInit
    };
    /**
     *  RTC class implementation.
     **/
    class Rtc : public Peripheral {
        private:
            bool m_isInit;                  ///< Initialization flag.
          
            RTC_HandleTypeDef m_handler;    ///< RTC handler structure.
            RTC_DateTypeDef m_date;         ///< Date structure object field.
            RTC_TimeTypeDef m_time;         ///< Time structure object field.
            
            RtcError m_error;               ///< Current RTC error.
            // Private class methods.
            RtcError _init(void);
            RtcError _uninit(void);
            
        public:
            Rtc(RTC_DateTypeDef* date, RTC_TimeTypeDef* time);
            ~Rtc(void);
            
            uint8_t getError(void) { return (uint8_t)m_error; }
            RtcError getDate(RTC_DateTypeDef& date);
            RtcError getDate(uint8_t& days, uint8_t& months, uint16_t& years);
            RtcError setDate(RTC_DateTypeDef& date);
            RtcError setDate(uint8_t days, uint8_t months, uint16_t years);
            RtcError getTime(RTC_TimeTypeDef& time);
            RtcError getTime(uint8_t& hours, uint8_t& minutes, uint8_t& seconds);
            RtcError setTime(RTC_TimeTypeDef& time);
            RtcError setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
    };
}

#endif // __PERIPHERAL_RTC_H