#ifndef __BASIC_TIMER_H
#define __BASIC_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "stm32h7xx.h"
#include "Typedefs.h"
    
typedef void(*basicTimerCallback)(unk);
    
typedef enum
{
    kBasicTimer0 = 0x00, // TIM6 itself.
    kBasicTimer1         // TIM7 itself.
} BasicTimer_Index;
    
typedef enum
{
    kBTimerNoError = 0x00,
    kBTimerAlreadyInit,
    kBTimerCannotInit1,
    kBTimerCannotInit2,
    kBTimerIsntInit
} BasicTimer_Error;

typedef struct
{
    flg m_isInitialised;
    flg m_isEnabled;
    TIM_HandleTypeDef m_handler;
    basicTimerCallback m_fCallback;
} BasicTimer;

BasicTimer_Error BasicTimer_Init(BasicTimer_Index, u32, basicTimerCallback,
                                                                           flg);
BasicTimer_Error BasicTimer_Toggle(BasicTimer_Index, flg);
BasicTimer_Error BasicTimer_DeInit(BasicTimer_Index);

flg BasicTimer_GetInitialised(BasicTimer_Index);
flg BasicTimer_GetEnabled(BasicTimer_Index);
  
#ifdef __cplusplus
}
#endif

#endif // __BASIC_TIMER_H