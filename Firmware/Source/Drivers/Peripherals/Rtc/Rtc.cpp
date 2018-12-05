/**
 *	@file		Rtc.cpp (module file)
 *	@version	1.0 (release)
 *	@author		utuM (Kostyantyn Komarov)
 *	@date		26.11.2018 (implementation),
 *              05.12.2018 (release)
 *  @brief		Peripheral RTC class implementation.
 *				This class describes peripheral device of selected 
 *				microcontroller. 
 */

#include "Rtc.h"

static bool s_isRtcInit = false;    ///< Global RTC module initialization flag.

/**
 * Overrided RTC MSP initialization.
 * @param[i] RTC_HandleTypeDef* pHandler - current RTC module handler object.
 **/
void HAL_RTC_MspInit(RTC_HandleTypeDef* pHandler)
{
    if (pHandler->Instance == RTC) {
        __HAL_RCC_RTC_ENABLE();
    }
}

/**
 * Overrided RTC MSP deinitialization.
 * @param[i] RTC_HandleTypeDef* pHandler - current RTC module handler object.
 **/
void HAL_RTC_MspDeInit(RTC_HandleTypeDef* pHandler)
{
    if (pHandler->Instance == RTC) {
        __HAL_RCC_RTC_DISABLE();
    }
} 

/**
 * RTC module and object basic initialization method.
 * @param[i] const RTC_DateTypeDef& rDate - reference to RTC date structure 
 *                                          object.
 * @param[i] const RTC_TimeTypeDef& rTime - reference to RTC time structure 
 *                                          object.
 * @return kRtcNoError - successful processing;
 *         kRtcFailedInit - some error occurs during RTC module initialization
 *                          by HAL library.
 **/
Driver::RtcError Driver::Rtc::_init(const RTC_DateTypeDef& rDate,
                                    const RTC_TimeTypeDef& rTime)
{
    // Check, if RTC module has been initialized already.
    if (s_isRtcInit) {
        return (m_error = kRtcAlreadyInit);
    } 
    // RTC structure initialization.      
    memset(&m_handler, 0x00, sizeof(RTC_HandleTypeDef));
    m_handler.Instance = RTC;
    m_handler.Init.HourFormat = RTC_HOURFORMAT_24;
    m_handler.Init.AsynchPrediv = 127;
    m_handler.Init.SynchPrediv = 255;
    m_handler.Init.OutPut = RTC_OUTPUT_DISABLE;
    if (HAL_RTC_Init(&m_handler) != HAL_OK) {
        return (m_error = kRtcFailedInit);
    }
    m_isInit = s_isRtcInit = true;
    // Set defined time and date.
    setTime(rTime.Hours, rTime.Minutes, rTime.Seconds);
    setDate(rDate.Date, rDate.Month, rDate.Year);
    
    return (m_error = kRtcNoError);
}

/**
 * RTC module and object basic deinitialization method.
 * @return kRtcNoError - successful processing;
 *         kRtcIsntInit - current RTC class object hasn't been initialized yet.
 **/
Driver::RtcError Driver::Rtc::_deinit(void)
{
    if (!m_isInit) {
        return (m_error = kRtcIsntInit);
    } 
  
    HAL_RTC_DeInit(&m_handler);
    m_isInit = s_isRtcInit = false;
    return (m_error = kRtcNoError);
}

/**
 * RTC module initialization/class construction.
 * @param[i] RTC_DateTypeDef& rDate - reference to RTC date structure object.
 * @param[i] RTC_TimeTypeDef& rTime - reference to RTC time structure object.
 **/
Driver::Rtc::Rtc(const RTC_DateTypeDef& rDate, const RTC_TimeTypeDef& rTime) : 
                                                           m_isInit(false),
                                                           m_error(kRtcNoError)
{
    // Initialize the RTC module.    
    _init(rDate, rTime);
}

/**
 * RTC module and object deinitialization.
 **/
Driver::Rtc::~Rtc(void)
{
    _deinit();
}

/**
 * Get current time.
 * @param[o] uint8_t& rDays   - reference to hours variable.
 * @param[o] uint8_t& rMonths - reference to minutes variable.
 * @param[o] uint8_t& rYears  - reference to seconds variable.
 * @return kRtcNoError - successful processing;
 *         kRtcIsntInit - current RTC class object hasn't been initialized yet;
 *         kRtcTimeGetFailed - current time values cannot be received now.
 **/
Driver::RtcError Driver::Rtc::getTime(uint8_t& rHours, uint8_t& rMinutes,
                                                              uint8_t& rSeconds)
{
    if (!m_isInit) {
        return (m_error = kRtcIsntInit);
    }
    // Receive time from the RTC module.
    RTC_TimeTypeDef l_time;
    if (HAL_RTC_GetTime(&m_handler, &l_time, RTC_FORMAT_BIN) != HAL_OK) {
        return (m_error = kRtcTimeGetFailed);
    }
    // Assign received values to the object ones.
    rHours = l_time.Hours;
    rMinutes = l_time.Minutes;
    rSeconds = l_time.Seconds;
    
    return (m_error = kRtcNoError);
}

/**
 * Set new time by input values.
 * @param[i] uint8_t hours   - hours value.
 * @param[i] uint8_t minutes - minutes value.
 * @param[i] uint8_t seconds - seconds value.
 * @return kRtcNoError - successful processing;
 *         kRtcIsntInit - current RTC class object hasn't been initialized yet;
 *         kRtcTimeSetFailed - new time values cannot be applied now.
 **/
Driver::RtcError Driver::Rtc::setTime(uint8_t hours, uint8_t minutes, 
                                                                uint8_t seconds)
{
    if (!m_isInit) {
        return (m_error = kRtcIsntInit);
    }
    // Assign input values to the class time fields.
    RTC_TimeTypeDef l_time;
    l_time.StoreOperation = RTC_STOREOPERATION_SET;
    l_time.Hours = hours;
    l_time.Minutes = minutes;
    l_time.Seconds = seconds;
    // Send time to the RTC module.
    if ( HAL_RTC_SetTime(&m_handler, &l_time, RTC_FORMAT_BIN) != HAL_OK) {
        return (m_error = kRtcTimeSetFailed);
    }
  
    return (m_error = kRtcNoError);
}

/**
 * Get current date.
 * @param[o] uint8_t& rDays   - reference to days variable.
 * @param[o] uint8_t& rMonths - reference to month variable.
 * @param[o] uint8_t& rYears  - reference to years variable.
 * @return kRtcNoError - successful processing;
 *         kRtcIsntInit - current RTC class object hasn't been initialized yet;
 *         kRtcDateGetFailed - current date values cannot be received now.
 **/
Driver::RtcError Driver::Rtc::getDate(uint8_t& rDays, uint8_t& rMonths, 
                                                                uint8_t& rYears)
{
    if (!m_isInit) {
        return (m_error = kRtcIsntInit);
    }
    // Receive date from the RTC module.
    RTC_DateTypeDef l_date;
    if (HAL_RTC_GetDate(&m_handler, &l_date, RTC_FORMAT_BIN) != HAL_OK) {
        return (m_error = kRtcDateGetFailed);
    }
    rDays = l_date.Date;
    rMonths = l_date.Month;
    rYears = l_date.Year;
  
    return (m_error = kRtcNoError);
}

/**
 * Set new date by input values.
 * @param[i] uint8_t days   - days value.
 * @param[i] uint8_t months - months value.
 * @param[i] uint8_t years  - years value from 0 to 99 (Note: don't forget to 
 *                            add 'm_kRtcYearConstant' to the received one).
 * @return kRtcNoError - successful processing;
 *         kRtcIsntInit - current RTC class object hasn't been initialized yet;
 *         kRtcDateSetFailed - new date values cannot be applied now.
 **/
Driver::RtcError Driver::Rtc::setDate(uint8_t days, uint8_t months, 
                                                                  uint8_t years)
{
    if (!m_isInit) {
        return (m_error = kRtcIsntInit);
    }
    // Prepare date and send it to the RTC module.
    RTC_DateTypeDef l_date;
    l_date.Date = days;
    l_date.Month = months;
    l_date.Year = years < 100 ? years : (years % 100);
    if (HAL_RTC_SetDate(&m_handler, &l_date, RTC_FORMAT_BIN) != HAL_OK) {
        return (m_error = kRtcDateSetFailed);
    }
    
    return (m_error = kRtcNoError);
}

/**
 * Receive both date and time data from the RTC module.
 * @param[o] uint8_t* pDate - output date from the RTC module.
 * @param[o] uint8_t* pTime - output time from the RTC module.
 * @return kRtcNoError - successful processing;
 *         kRtcInvalidDateTimePtr - input date and time pointers are
 *                                  empty both or one of them.
 **/
Driver::RtcError Driver::Rtc::update(uint8_t* pDate, uint8_t* pTime)
{
    // Check if input pointers aren't 'nullptr'.
    if (pDate == nullptr || pTime == nullptr) {
        return (m_error = kRtcInvalidDateTimePtr);
    }
    // Get time and date from the RTC module.
    getTime(pTime[0], pTime[1], pTime[2]);
    getDate(pDate[0], pDate[1], pDate[2]);
  
    return (m_error = kRtcNoError);
}