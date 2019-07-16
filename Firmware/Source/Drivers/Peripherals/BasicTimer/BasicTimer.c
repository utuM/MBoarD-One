#include "BasicTimer.h"

#include <string.h>
#include "stm32h7xx_hal.h"

static BasicTimer s_basicTimers[2] = { null, null };

void TIM6_DAC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&s_basicTimers[0].m_handler);
    if (s_basicTimers[0].m_fCallback)
    {
        s_basicTimers[0].m_fCallback();
    }
}

void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&s_basicTimers[1].m_handler);
    if (s_basicTimers[1].m_fCallback)
    {
        s_basicTimers[1].m_fCallback();
    }
}

BasicTimer_Error BasicTimer_Init(BasicTimer_Index handler, u32 milliseconds,
                                                  basicTimerCallback fCallback,
                                                                flg isEnabled)
{
    u8 l_index = (u8)handler;
    if (s_basicTimers[l_index].m_isInitialised)
    {
        return kBTimerAlreadyInit;
    }
    // Prepare variables before usage.
    IRQn_Type l_irq;
    TIM_MasterConfigTypeDef l_masterConfig = {0};
    memset(&s_basicTimers[l_index], null, sizeof(BasicTimer));
    switch (handler)
    {
        case kBasicTimer0:
            s_basicTimers[l_index].m_handler.Instance = TIM6;
            l_irq = TIM6_DAC_IRQn;
            break;
            
        case kBasicTimer1:
            s_basicTimers[l_index].m_handler.Instance = TIM7;
            l_irq = TIM7_IRQn;
            break;
            
        default:
            /* Cannot reach here anyway. */
            break;
    }
    // Calculate prescaler: HPRE clock.
    u32 l_hpreFreq = SystemCoreClock;
    u8 l_bitOffset = RCC->D1CFGR & 0x0000000F;
    if (l_bitOffset >= 0x08)
    {
        l_bitOffset &= 0x07;
        if (l_bitOffset >= 4)
        {
            ++ l_bitOffset;
        }
        l_hpreFreq = SystemCoreClock / (0x01 << (l_bitOffset + 1));
    }
    // Calculate prescaler: D2PPRE1 clock.
    u32 l_d2ppre1Freq = l_hpreFreq;
    l_bitOffset = (RCC->D2CFGR >> 4) & 0x07;
    if (l_bitOffset >= 0x04)
    {
        l_bitOffset &= 0x03;
        l_d2ppre1Freq = l_hpreFreq / (0x01 << (l_bitOffset + 1));
    }
    // Calculate prescaler: final value.
    u32 l_prescaler = l_d2ppre1Freq * 2 / 1000 - 1;
    // Setting up timer parameters.
    s_basicTimers[l_index].m_handler.Init.Prescaler = l_prescaler;
    s_basicTimers[l_index].m_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    s_basicTimers[l_index].m_handler.Init.Period = milliseconds;
    s_basicTimers[l_index].m_handler.Init.AutoReloadPreload =
                                                 TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&s_basicTimers[l_index].m_handler) != HAL_OK)
    {
        return kBTimerCannotInit1;
    }
    l_masterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    l_masterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&s_basicTimers[l_index].m_handler,
                                              &l_masterConfig) != HAL_OK)
    {
        return kBTimerCannotInit2;
    }
    // Enable IRQ handlers.
    NVIC_SetPriority(l_irq, 3U);
    NVIC_EnableIRQ(l_irq);
    // Set time as initialised.
    s_basicTimers[l_index].m_isInitialised = yes;
    // Save timer callback if pointer is not empty here.
    if (fCallback)
    {
        s_basicTimers[l_index].m_fCallback = fCallback;
    }
    // Start the timer if needed.
    if (isEnabled)
    {
        s_basicTimers[l_index].m_isEnabled = yes;
        HAL_TIM_Base_Start_IT(&s_basicTimers[l_index].m_handler);
    }
    
    return kBTimerNoError;
}

BasicTimer_Error BasicTimer_Toggle(BasicTimer_Index index, flg isEnabled)
{
    u8 l_index = (u8)index;
    if (!s_basicTimers[l_index].m_isInitialised)
    {
        return kBTimerIsntInit;
    }
    if (isEnabled && !s_basicTimers[l_index].m_isEnabled)
    {
        s_basicTimers[l_index].m_isEnabled = yes;
        HAL_TIM_Base_Start_IT(&s_basicTimers[l_index].m_handler);
    }
    else if (!isEnabled && s_basicTimers[l_index].m_isEnabled)
    {
        s_basicTimers[l_index].m_isEnabled = no;
        HAL_TIM_Base_Stop(&s_basicTimers[l_index].m_handler);
    }
    
    return kBTimerNoError;
}

BasicTimer_Error BasicTimer_DeInit(BasicTimer_Index index)
{
    u8 l_index = (u8)index;
    if (!s_basicTimers[l_index].m_isInitialised)
    {
        return kBTimerIsntInit;
    }
    HAL_TIM_Base_DeInit(&s_basicTimers[l_index].m_handler);
    s_basicTimers[l_index].m_isInitialised = no;
    
    return kBTimerNoError;
}

flg BasicTimer_GetInitialised(BasicTimer_Index index)
{
    return s_basicTimers[(u8)index].m_isInitialised;
}

flg BasicTimer_GetEnabled(BasicTimer_Index index)
{
    u8 l_index = (u8)index;
    if (!s_basicTimers[l_index].m_isInitialised)
    {
        return no;
    }
    return s_basicTimers[l_index].m_isEnabled;
}