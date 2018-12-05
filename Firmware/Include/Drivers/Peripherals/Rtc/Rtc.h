/**
 *	@file		Rtc.h (header file)
 *	@version	1.0 (release)
 *	@author		utuM (Kostyantyn Komarov)
 *	@date		26.11.2018 (implementation),
 *              05.12.2018 (release)
 *  @brief		Peripheral RTC class implementation.
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
    const uint16_t m_kRtcYearConstant = 2019; ///< Start year.
    /**
     *  RTC class possible errors.
     **/
    enum RtcError : uint8_t {
        kRtcNoError = 0x00,     ///< No error.
        kRtcAlreadyInit,        ///< 
        kRtcFailedInit,         ///< Some error or exception was occured during
                                ///  RTC module initialization inside HAL
                                ///  library.
        kRtcIsntInit,           ///< Current object hasn't been initialized
                                ///  properly or class object has been created
                                ///  by a default constructor.
        kRtcTimeSetFailed,      ///< Error was occured during time setting up
                                ///  in the RTC module.
        kRtcTimeGetFailed,      ///< Error was occured during time receiving
                                ///  from the RTC module.
        kRtcDateSetFailed,      ///< Error was occured during date setting up
                                ///  in the RTC module.
        kRtcDateGetFailed,      ///< Error was occured during date receiving
                                ///  from the RTC module.
        kRtcInvalidDateTimePtr  ///< Date or time pointers to objects are both
                                ///  'nullptr' or only one of them.
    };
    /**
     *  RTC class implementation.
     **/
    class Rtc : public Peripheral {
        private:
            bool m_isInit;                  ///< Initialization flag.
            RTC_HandleTypeDef m_handler;    ///< RTC handler structure.            
            RtcError m_error;               ///< Current RTC error.
            // Private class methods.
            RtcError _init(const RTC_DateTypeDef& rDate, 
                           const RTC_TimeTypeDef& rTime);
            RtcError _deinit(void);
            
        public:
            // Constructors and destructor.
            Rtc(void) : m_isInit(false) { /* To prevent false init. */ }
            Rtc(const RTC_DateTypeDef& rDate, const RTC_TimeTypeDef& rTime);
            ~Rtc(void);
            // Setters and getters.
            RtcError getDate(uint8_t& days, uint8_t& months, uint8_t& years);
            RtcError setDate(const uint8_t days, const uint8_t months, 
                                                           const uint8_t years);
            RtcError getTime(uint8_t& hours, uint8_t& minutes, 
                                                              uint8_t& seconds);
            RtcError setTime(const uint8_t hours, const uint8_t minutes,
                                                         const uint8_t seconds);
            uint8_t getError(void) { return (uint8_t)m_error; }
            // Other methods.
            RtcError update(uint8_t* date, uint8_t* time);
    };
}

#endif // __PERIPHERAL_RTC_H