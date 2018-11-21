#include "Timer.h"

extern TIM_HandleTypeDef* htim6;
static uint8_t s_timerEnabling = 0x00;
static bool* s_isTimerUpdated[2];

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM6) {
        __HAL_RCC_TIM6_CLK_ENABLE();
        /* TIM6 interrupt Init */
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
    else if(tim_baseHandle->Instance == TIM7) {
        __HAL_RCC_TIM7_CLK_ENABLE();
        /* TIM7 interrupt Init */
        HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM6) {
        __HAL_RCC_TIM6_CLK_DISABLE();
        /* TIM6 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
    }
    else if (tim_baseHandle->Instance == TIM7) {
        __HAL_RCC_TIM7_CLK_DISABLE();
        /* TIM7 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM7_IRQn);
    }
} 

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (TIM6->CNT) {
        *s_isTimerUpdated[0] = true;
    }
    if (TIM7->CNT) {
        *s_isTimerUpdated[1] = true;
    }
}

Driver::TimerError Driver::Timer::_init(void)
{
    // Basic timer handler initialization.
    TIM_MasterConfigTypeDef sMasterConfig;
    switch (m_params.m_index) {
        case 0:
            m_handler.Instance = TIM6;
            htim6 = &m_handler;
            s_timerEnabling |= 0x01;
            break;
            
        case 1:
            m_handler.Instance = TIM7;
            s_timerEnabling |= 0x02;
            break;
            
        default:
            break;
    }
    s_isTimerUpdated[m_params.m_index] = &m_isUpdated;
    m_handler.Init.Prescaler = m_params.m_prescaler - 1;
    m_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    m_handler.Init.Period = m_params.m_period - 1;
    m_handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&m_handler) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&m_handler, &sMasterConfig) != 
                                                                       HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }
}

Driver::TimerError Driver::Timer::_uninit(void)
{
    if (m_isLaunched) {
        toggle(false);
    }
    s_timerEnabling ^= 0x01 << m_params.m_index;
    s_isTimerUpdated[m_params.m_index] = nullptr;
    HAL_TIM_Base_DeInit(&m_handler);
}

Driver::Timer::Timer(TimerParameters& parameters) : m_isInit(false),
                                                    m_error(kTimerNoError)
{
    // Check if input index is valid.
    if (parameters.m_index <= 1) {
        // Check if timer with input index hasn't been initialized earlier.
        if (!((s_timerEnabling >> parameters.m_index) & 0x01)) {
            // Check if period and prescaler values are not empty.
            if (parameters.m_period && parameters.m_prescaler) {
                parameters.m_period = parameters.m_period > s_kMaxTimerPeriod ?
                                      s_kMaxTimerPeriod : parameters.m_period;
                memcpy(&m_params, &parameters, sizeof(TimerParameters));
                _init();
                m_isInit = true;
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

Driver::Timer::~Timer(void)
{
    _uninit();
}

void Driver::Timer::toggle(bool needToEnable)
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