/**
  * @file       Uart.c
  * @author     utuM
  * @date       13.06.19 (creation)
  * @version    0.1.0
  * @brief   
  **/

#include "Uart.h"

#include <string.h>

// UART enabling flags.
#define UART1_ENABLED 0
#define UART2_ENABLED 0
#define UART3_ENABLED 0
#define UART4_ENABLED 0
#define UART5_ENABLED 0
#define UART6_ENABLED 0
#define UART7_ENABLED 0
#define UART8_ENABLED 0
// Total UART handlers amount.
#define TOTAL_UART_AMOUNT 8

UART_Handler s_uart[TOTAL_UART_AMOUNT] = {null};

/**
  * @brief UART initialisation function.
  * @param[i] pinConfig    - UART pin configuration.
  * @param[i] baud         - UART baud rate.
  * @param[i] isDmaEnabled - DMA enabling flag.
  * @return
  **/
UART_Handler* UART_init(UART_PinConfig pinConfig, UART_BaudRate baud, 
                                                               flg isDmaEnabled)
{
    USART_TypeDef* l_pInstance = null;
    u8 l_index;
    // Configure direct UART handler.
    switch (pinConfig) 
    {
        // UART1 handler configuration.
        case UART1_TXB6_RXB7:  case UART1_TXB6_RXA10:  case UART1_TXB6_RXB15:
        case UART1_TXA9_RXB7:  case UART1_TXA9_RXA10:  case UART1_TXA9_RXB15:
        case UART1_TXB14_RXB7: case UART1_TXB14_RXA10: case UART1_TXB14_RXB15:
#if (UART1_ENABLED == 0)
            RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
            memset(&s_uart[0], null, sizeof(UART_Handler));
            #undef  UART1_ENABLED
            #define UART1_ENABLED 1
            switch (pinConfig)
            {
                case UART1_TXB6_RXB7:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN6, GPIO_UART1_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN7, GPIO_UART1_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART1_TXB6_RXA10:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN6, GPIO_UART1_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN10, GPIO_UART1_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART1_TXB6_RXB15:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN6, GPIO_UART1_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN15, GPIO_UART1_AF4,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART1_TXA9_RXB7:
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN9, GPIO_UART1_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN7, GPIO_UART1_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART1_TXA9_RXA10:
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN9, GPIO_UART1_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN10, GPIO_UART1_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART1_TXA9_RXB15:
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN9, GPIO_UART1_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN15, GPIO_UART1_AF4,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART1_TXB14_RXB7:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN14, GPIO_UART1_AF4,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN7, GPIO_UART1_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART1_TXB14_RXA10:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN14, GPIO_UART1_AF4,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN10, GPIO_UART1_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART1_TXB14_RXB15:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN14, GPIO_UART1_AF4,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN15, GPIO_UART1_AF4,
                                                                GPIO_LOW_SPEED);
                    break;
            }
            l_index = 0;
            l_pInstance = USART1;            
#endif // (UART1_ENABLED == 0)
            break;
        // UART2 handler configuration.
        case UART2_TXD5_RXD6: case UART2_TXD5_RXA3: case UART2_TXA2_RXD6:
        case UART2_TXA2_RXA3:
#if (UART2_ENABLED == 0)
            RCC->APB1LENR |= RCC_APB1LENR_USART2EN;
            memset(&s_uart[1], null, sizeof(UART_Handler));
            #undef  UART2_ENABLED
            #define UART2_ENABLED 1
            switch (pinConfig)
            {
                case UART2_TXD5_RXD6:
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN5, GPIO_UART2_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN6, GPIO_UART2_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART2_TXD5_RXA3:
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN5, GPIO_UART2_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN3, GPIO_UART2_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART2_TXA2_RXD6:
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN2, GPIO_UART2_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN6, GPIO_UART2_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART2_TXA2_RXA3:
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN2, GPIO_UART2_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN3, GPIO_UART2_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
            }
            l_index = 1;
            l_pInstance = USART2;
#endif // (UART2_ENABLED == 0)
            break;
        // UART3 handler configuration. 
        case UART3_TXC10_RXC11: case UART3_TXC10_RXD9: case UART3_TXC10_RXB11:
        case UART3_TXD8_RXC11:  case UART3_TXD8_RXD9:  case UART3_TXD8_RXB11:
        case UART3_TXB10_RXC11: case UART3_TXB10_RXD9: case UART3_TXB10_RXB11:
#if (UART3_ENABLED == 0)
            RCC->APB1LENR |= RCC_APB1LENR_USART3EN;
            memset(&s_uart[2], null, sizeof(UART_Handler));
            #undef  UART3_ENABLED
            #define UART3_ENABLED 1
            switch (pinConfig)
            {
                case UART3_TXC10_RXC11:
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN10, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN11, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART3_TXC10_RXD9:
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN10, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN9, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART3_TXC10_RXB11:
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN10, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN11, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART3_TXD8_RXC11:
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN8, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN11, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART3_TXD8_RXD9:
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN8, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN9, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART3_TXD8_RXB11:
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN8, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN11, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART3_TXB10_RXC11:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN10, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN11, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART3_TXB10_RXD9:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN10, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN9, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART3_TXB10_RXB11:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN10, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN11, GPIO_UART3_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
            }
            l_index = 2;
            l_pInstance = USART3;
#endif // (UART3_ENABLED == 0)
            break;
        // UART4 handler configuration.
        case UART4_TXA12_RXD0: case UART4_TXA12_RXC11: case UART4_TXA12_RXA11:
        case UART4_TXC10_RXD0: case UART4_TXC10_RXC11: case UART4_TXC10_RXA11:
        case UART4_TXD1_RXD0:  case UART4_TXD1_RXC11:  case UART4_TXD1_RXA11:
#if (UART4_ENABLED == 0)
            RCC->APB1LENR |= RCC_APB1LENR_UART4EN;
            memset(&s_uart[3], null, sizeof(UART_Handler));
            #undef  UART4_ENABLED
            #define UART4_ENABLED 1
            switch (pinConfig)
            {
                case UART4_TXA12_RXD0:
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN12, GPIO_UART4_AF6,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN0, GPIO_UART4_AF8,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART4_TXA12_RXC11:
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN12, GPIO_UART4_AF6,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN11, GPIO_UART4_AF8,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART4_TXA12_RXA11:
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN12, GPIO_UART4_AF6,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN11, GPIO_UART4_AF6,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART4_TXC10_RXD0:
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN10, GPIO_UART4_AF8,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN0, GPIO_UART4_AF8,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART4_TXC10_RXC11:
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN10, GPIO_UART4_AF8,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN11, GPIO_UART4_AF8,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART4_TXC10_RXA11:
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN10, GPIO_UART4_AF8,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN11, GPIO_UART4_AF6,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART4_TXD1_RXD0:
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN1, GPIO_UART4_AF8,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN0, GPIO_UART4_AF8,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART4_TXD1_RXC11:
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN1, GPIO_UART4_AF8,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN11, GPIO_UART4_AF6,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART4_TXD1_RXA11:
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN1, GPIO_UART4_AF8,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN11, GPIO_UART4_AF6,
                                                                GPIO_LOW_SPEED);
                    break;
            }
            l_index = 3;
            l_pInstance = UART4;
#endif // (UART4_ENABLED == 0)
            break;
        // UART5 handler configuration.
        case UART5_TXC12_RXB12: case UART5_TXC12_RXB5: case UART5_TXC12_RXD2:
        case UART5_TXB6_RXB12:  case UART5_TXB6_RXB5:  case UART5_TXB6_RXD2:
        case UART5_TXB13_RXB12: case UART5_TXB13_RXB5: case UART5_TXB13_RXD2:
#if (UART5_ENABLED == 0)
            RCC->APB1LENR |= RCC_APB1LENR_UART5EN;
            memset(&s_uart[4], null, sizeof(UART_Handler));
            #undef  UART5_ENABLED
            #define UART5_ENABLED 1
            switch (pinConfig)
            {
                case UART5_TXC12_RXB12:
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN12, GPIO_UART5_AF8,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN12, GPIO_UART5_AF14,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART5_TXC12_RXB5:
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN12, GPIO_UART5_AF8,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN5, GPIO_UART5_AF14,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART5_TXC12_RXD2:
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN12, GPIO_UART5_AF8,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN2, GPIO_UART5_AF8,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART5_TXB6_RXB12:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN6, GPIO_UART5_AF14,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN12, GPIO_UART5_AF14,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART5_TXB6_RXB5:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN6, GPIO_UART5_AF14,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN5, GPIO_UART5_AF14,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART5_TXB6_RXD2:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN6, GPIO_UART5_AF14,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN2, GPIO_UART5_AF8,
                                                                GPIO_LOW_SPEED);
                    break;
                
                case UART5_TXB13_RXB12:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN13, GPIO_UART5_AF14,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN12, GPIO_UART5_AF14,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART5_TXB13_RXB5:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN13, GPIO_UART5_AF14,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN5, GPIO_UART5_AF14,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART5_TXB13_RXD2:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN13, GPIO_UART5_AF14,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOD_PORT, GPIO_PIN2, GPIO_UART5_AF8,
                                                                GPIO_LOW_SPEED);
                    break;
            }
            l_index = 4;
            l_pInstance = UART5;
#endif // (UART5_ENABLED == 0)
            break;
        // UART6 handler configuration.
        case UART6_TXG14_RXG9: case UART6_TXG14_RXC7: case UART6_TXC6_RXG9:
        case UART6_TxC6_RXC7:
#if (UART6_ENABLED == 0)
            RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
            memset(&s_uart[5], null, sizeof(UART_Handler));
            #undef  UART6_ENABLED
            #define UART6_ENABLED 1
            switch (pinConfig)
            {
                case UART6_TXG14_RXG9:
                    GPIO_InitAltFunc(GPIOG_PORT, GPIO_PIN14, GPIO_UART6_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOG_PORT, GPIO_PIN9, GPIO_UART6_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART6_TXG14_RXC7:
                    GPIO_InitAltFunc(GPIOG_PORT, GPIO_PIN14, GPIO_UART6_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN7, GPIO_UART6_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART6_TXC6_RXG9:
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN6, GPIO_UART6_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOG_PORT, GPIO_PIN9, GPIO_UART6_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART6_TxC6_RXC7:
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN6, GPIO_UART6_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOC_PORT, GPIO_PIN7, GPIO_UART6_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
            }
            l_index = 5;
            l_pInstance = USART6;
#endif // (UART6_ENABLED == 0)
            break;
        // UART7 handler configuration.
        case UART7_TXB4_RXB3:  case UART7_TXB4_RXA8:  case UART7_TXB4_RXE7:
        case UART7_TXB4_RXF6:  case UART7_TXA15_RXB3: case UART7_TXA15_RXA8:
        case UART7_TXA15_RXE7: case UART7_TXA15_RXF6: case UART7_TXE8_RXB3:
        case UART7_TXE8_RXA8:  case UART7_TXE8_RXE7:  case UART7_TXE8_RXF6:
        case UART7_TXF7_RXB3:  case UART7_TXF7_RXA8:  case UART7_TXF7_RXE7:
        case UART7_TXF7_RXF6:
#if (UART7_ENABLED == 0)
            RCC->APB1LENR |= RCC_APB1LENR_UART7EN;
            memset(&s_uart[6], null, sizeof(UART_Handler));
            #undef  UART7_ENABLED
            #define UART7_ENABLED 1
            switch (pinConfig)
            {
                case UART7_TXB4_RXB3:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN4, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN3, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXB4_RXA8:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN4, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN8, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXB4_RXE7:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN4, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOE_PORT, GPIO_PIN7, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXB4_RXF6:
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN4, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOF_PORT, GPIO_PIN6, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXA15_RXB3:
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN15, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN3, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXA15_RXA8:
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN15, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN8, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXA15_RXE7:
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN15, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOE_PORT, GPIO_PIN7, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXA15_RXF6:
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN15, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOF_PORT, GPIO_PIN6, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXE8_RXB3:
                    GPIO_InitAltFunc(GPIOE_PORT, GPIO_PIN8, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN3, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXE8_RXA8:
                    GPIO_InitAltFunc(GPIOE_PORT, GPIO_PIN8, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN8, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXE8_RXE7:
                    GPIO_InitAltFunc(GPIOE_PORT, GPIO_PIN8, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOE_PORT, GPIO_PIN7, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXE8_RXF6:
                    GPIO_InitAltFunc(GPIOE_PORT, GPIO_PIN8, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOF_PORT, GPIO_PIN6, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXF7_RXB3:
                    GPIO_InitAltFunc(GPIOF_PORT, GPIO_PIN7, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOB_PORT, GPIO_PIN3, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXF7_RXA8:
                    GPIO_InitAltFunc(GPIOF_PORT, GPIO_PIN7, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOA_PORT, GPIO_PIN8, GPIO_UART7_AF11,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXF7_RXE7:
                    GPIO_InitAltFunc(GPIOF_PORT, GPIO_PIN7, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOE_PORT, GPIO_PIN7, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
                    
                case UART7_TXF7_RXF6:
                    GPIO_InitAltFunc(GPIOF_PORT, GPIO_PIN7, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    GPIO_InitAltFunc(GPIOF_PORT, GPIO_PIN6, GPIO_UART7_AF7,
                                                                GPIO_LOW_SPEED);
                    break;
            }
            l_index = 6;
            l_pInstance = UART7;
#endif // (UART7_ENABLED == 0)
            break;
        // UART8 handler configuration.
        case UART8_TXE1_RXE0:
#if (UART8_ENABLED == 0)
            RCC->APB1LENR |= RCC_APB1LENR_UART8EN;
            memset(&s_uart[7], null, sizeof(UART_Handler));
            #undef  UART8_ENABLED
            #define UART8_ENABLED 1
            GPIO_InitAltFunc(GPIOE_PORT, GPIO_PIN1, GPIO_UART8_AF8,
                                                                GPIO_LOW_SPEED);
            GPIO_InitAltFunc(GPIOE_PORT, GPIO_PIN0, GPIO_UART8_AF8,
                                                                GPIO_LOW_SPEED);
            l_index = 7;
            l_pInstance = UART8;
#endif // (UART8_ENABLED == 0)
            break;
        // Magic error is occurred.
        default:
            return null;
            break;
    }
    // Prepare and configure 'UART_Handler' structure by input parameter values.
    memset(&s_uart[l_index], null, sizeof(UART_Handler));
    
    return &s_uart[l_index];
}