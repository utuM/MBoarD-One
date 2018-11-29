#include "Rtc.h"

Driver::RtcError Driver::Rtc::_init(void)
{
    // RTC structure initialization.    
    m_handler.Instance = RTC;
    m_handler.Init.AsynchPrediv = 127;
    m_handler.Init.SynchPrediv = 255;
    m_handler.Init.OutPut = RTC_OUTPUT_DISABLE;
    if (HAL_RTC_Init(&m_handler) != HAL_OK) {
        return (m_error = kRtcFailedInit);
    }
    // Change flag into 'true' state on successful init.
    m_isInit = true;
    
    return (m_error = kRtcNoError);
}

Driver::RtcError Driver::Rtc::_uninit(void)
{
    if (m_isInit) {
        HAL_RTC_DeInit(&m_handler);
    } 
  
    return (m_error = kRtcNoError);
}

Driver::Rtc::Rtc(RTC_DateTypeDef* date, RTC_TimeTypeDef* time) : 
                                                           m_isInit(false),
                                                           m_error(kRtcNoError)
{
    // Check if input date is empty.
    if (date != nullptr) {
        memcpy(&m_date, date, sizeof(RTC_DateTypeDef));
    }
    // Check uf input time is empty.
    if (time != nullptr) {
        memcpy(&m_time, time, sizeof(RTC_TimeTypeDef));
    }
    
    _init();
}

Driver::Rtc::~Rtc(void)
{
    _uninit();
}

Driver::RtcError Driver::Rtc::getTime(RTC_TimeTypeDef& time)
{
    if (!m_isInit) {
        return (m_error = kRtcFailedInit);
    }
    // Receive time from the RTC module.
    HAL_RTC_GetTime(&m_handler, &m_time, RTC_HOURFORMAT_24);
    // Copy received data to the input structure.
    memcpy(&time, &m_time, sizeof(RTC_TimeTypeDef));
    
    return (m_error = kRtcNoError);
}

Driver::RtcError Driver::Rtc::getTime(uint8_t& hours, uint8_t& minutes,
                                                               uint8_t& seconds)
{
    if (!m_isInit) {
        return (m_error = kRtcFailedInit);
    }
    // Receive time from the RTC module.
    HAL_RTC_GetTime(&m_handler, &m_time, RTC_HOURFORMAT_24);
    // Assign received values to the object ones.
    hours = m_time.Hours;
    minutes = m_time.Minutes;
    seconds = m_time.Seconds;
  
    return (m_error = kRtcNoError);
}

Driver::RtcError Driver::Rtc::setTime(RTC_TimeTypeDef& time)
{
    if (!m_isInit) {
        return (m_error = kRtcFailedInit);
    }
    memcpy(&m_time, &time, sizeof(RTC_TimeTypeDef));
  
    return (m_error = kRtcNoError);
}

Driver::RtcError Driver::Rtc::setTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    if (!m_isInit) {
        return (m_error = kRtcFailedInit);
    }
    m_time.Hours = hours;
    m_time.Minutes = minutes;
    m_time.Seconds = seconds;
    HAL_RTC_SetTime(&m_handler, &m_time, RTC_HOURFORMAT_24);
  
    return (m_error = kRtcNoError);
}

Driver::RtcError Driver::Rtc::getDate(RTC_DateTypeDef& date)
{
    if (!m_isInit) {
        return (m_error = kRtcFailedInit);
    }
    memcpy(&date, &m_date, sizeof(RTC_DateTypeDef));
  
    return (m_error = kRtcNoError);
}

Driver::RtcError Driver::Rtc::getDate(uint8_t& days, uint8_t& months, uint16_t& years)
{
    if (!m_isInit) {
        return (m_error = kRtcFailedInit);
    }
    days = m_date.Date;
    months = m_date.Month;
    years = m_date.Year + m_kRtcYearConstant;
  
    return (m_error = kRtcNoError);
}

Driver::RtcError Driver::Rtc::setDate(RTC_DateTypeDef& date)
{
    if (!m_isInit) {
        return (m_error = kRtcFailedInit);
    }
    memcpy(&m_date, &date, sizeof(RTC_DateTypeDef));
  
    return (m_error = kRtcNoError);
}

Driver::RtcError Driver::Rtc::setDate(uint8_t days, uint8_t months, uint16_t years)
{
    if (!m_isInit) {
        return (m_error = kRtcFailedInit);
    }
    m_date.Date = days;
    m_date.Month = months;
    m_date.Year = years < 100 ? years : (years - m_kRtcYearConstant);
  
    return (m_error = kRtcNoError);
}