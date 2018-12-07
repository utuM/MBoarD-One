/**
 *  @file       Timer.cpp (module file)
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

#include "Timer.h"

static uint8_t s_timerEnabling = 0x00;       ///< Only use first 2 bits, shows
                                             ///  what timers are activated.
static bool* s_isTimerUpdated[2];            ///< Timer flags that indicate
                                             ///  incoming timers tick.
static TIM_HandleTypeDef* s_timerHandler[2]; ///< Pointers array to timer
                                             ///  handlers to be used inside the
                                             ///  interrupt handler.

/**
 * Timer DSP initialization.
 * @param[i] TIM_HandleTypeDef* pHandler - pointer to a timer handler object.
 **/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* pHandler)
{
    if (pHandler->Instance == TIM6) {
        __HAL_RCC_TIM6_CLK_ENABLE();
        /* TIM6 interrupt Init */
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
    else if(pHandler->Instance == TIM7) {
        __HAL_RCC_TIM7_CLK_ENABLE();
        /* TIM7 interrupt Init */
        HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
    }
}

/**
 * Timer DSP deinitialization.
 * @param[i] TIM_HandleTypeDef* pHandler - pointer to a timer handler object.
 **/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* pHandler)
{
    if (pHandler->Instance == TIM6) {
        __HAL_RCC_TIM6_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
    }
    else if (pHandler->Instance == TIM7) {
        __HAL_RCC_TIM7_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM7_IRQn);
    }
} 

/**
 * Timer callback function on tick event (can be used only for basic timers).
 * @param[i] TIM_HandleTypeDef* htim - pointer to a timer handler object (isn't
 *                                     using here).
 **/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* pTimer)
{
    if (pTimer->Instance == TIM6) {
        *s_isTimerUpdated[0] = true;
        printf("Triggered\n");
        }
    if (pTimer->Instance == TIM7) {
        *s_isTimerUpdated[1] = true;
    }
}

/**
 * TIM6 basic timer interrupt handler.
 **/
void TIM6_DAC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(s_timerHandler[0]);
}

/**
 * TIM7 basic timer interrupt handler.
 **/
void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(s_timerHandler[1]);
}

/**
 * System tick handler.
 **/
void SysTick_Handler(void)
{
    HAL_IncTick();
}

/**
 * Timer basic initialization.
 * @return kTimerNoError - successful processing;
 *         kTimerFailedInit - some error or exception was occured during timer
 *                            initialization in HAL library.
 **/
Driver::TimerError Driver::Timer::_init(void)
{  
    TIM_MasterConfigTypeDef l_masterConfig = {0};
    memset(&m_handler, 0x00, sizeof(TIM_HandleTypeDef));
    // Basic timer handler initialization.
    switch (m_params.m_index) {
        case 0:
            m_handler.Instance = TIM6;
            s_timerEnabling |= 0x01;
            s_timerHandler[0] = &m_handler;
            break;
            
        case 1:
            m_handler.Instance = TIM7;
            s_timerEnabling |= 0x02;
            s_timerHandler[1] = &m_handler;
            break;
            
        default:
            break;
    }
    s_isTimerUpdated[m_params.m_index] = &m_isUpdated;
    m_handler.Init.Prescaler = (uint32_t)((SystemCoreClock / 1000.0 /
                                           s_kDefaultApb1Divider * 2) - 1);
    m_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    m_handler.Init.Period = m_params.m_period - 1;
    m_handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&m_handler) != HAL_OK) {
        return (m_error = kTimerFailedInit);
    }
    l_masterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    l_masterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&m_handler, &l_masterConfig) != 
                                                                       HAL_OK) {
        return (m_error = kTimerFailedInit);
    }

    m_isInit = true;
    return (m_error = kTimerNoError);
}

/**
 * Timer class basic deinitialization.
 * @return Always returns kTimerNoError.
 **/
Driver::TimerError Driver::Timer::_deinit(void)
{
    // Check if timer was initialized.
    if (!m_isInit) {
        return (m_error = kTimerNoError);
    }
    if (m_isLaunched) {
        toggle(false);
    }
    s_timerEnabling ^= 0x01 << m_params.m_index;
    s_isTimerUpdated[m_params.m_index] = nullptr;
    HAL_TIM_Base_DeInit(&m_handler);
    
    return (m_error = kTimerNoError);
}

/**
 * Timer class constructor.
 * @param[i] TimerParameters& rParameters - reference to timer parameter
 *                                          structure object.
 **/
Driver::Timer::Timer(TimerParameters& rParameters) : m_isInit(false),
                                                     m_isUpdated(false),
                                                     m_isLaunched(false),
                                                     m_error(kTimerNoError)
{
    // Check if input index is valid.
    if (rParameters.m_index <= 1) {
        // Check if timer with input index hasn't been initialized earlier.
        if (!((s_timerEnabling >> rParameters.m_index) & 0x01)) {
            // Check if period is not empty.
            if (rParameters.m_period) {              
                rParameters.m_period = rParameters.m_period > s_kMaxTimerPeriod ?
                                       s_kMaxTimerPeriod : rParameters.m_period;
                memcpy(&m_params, &rParameters, sizeof(TimerParameters));
                _init();
            }
            else {
                m_error = kTimerInvalidParameter;
            }
        }
        else {
            m_error = kTimerAlreadyInit;
        }
    }
    else {
        m_error = kTimerInvalidIndex;
    }
}

/**
 * Timer class destructor.
 **/
Driver::Timer::~Timer(void)
{
    _deinit();
}

/**
 * Turn on or turn off current timer.
 * @param[i] const bool needToEnable - flag that provides timer stop or launch.
 **/
void Driver::Timer::toggle(const bool needToEnable)
{
    if (!m_isLaunched && needToEnable) {
        HAL_TIM_Base_Start(&m_handler);
        HAL_TIM_Base_Start_IT(&m_handler);
    }
    else if (m_isLaunched && !needToEnable) {
        HAL_TIM_Base_Stop_IT(&m_handler);
        HAL_TIM_Base_Stop(&m_handler);
    }
    m_isLaunched = needToEnable;
}