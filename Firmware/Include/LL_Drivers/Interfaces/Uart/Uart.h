/**
  * @file       Uart.h
  * @author     utuM
  * @date       13.06.19 (creation)
  * @version    0.1.0
  * @brief   
  **/

#ifndef __DRV_INTERFACE_USART_H__
#define __DRV_INTERFACE_USART_H__

#include "stm32h743xx.h"

#include "GpioC.h"
#include "Typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define DEFAULT_UART_RXBUF_SIZE 256
      
/**
  * @brief UART handlers could be used for communication.
  *        Useful for STM32H753xx series MCU.
  **/
enm
{
    // UART1 possible pin positions.
    UART1_TXB6_RXB7    = 0x00U,
    UART1_TXB6_RXA10   = 0x01U,
    UART1_TXB6_RXB15   = 0x02U,
    UART1_TXA9_RXB7    = 0x03U,  
    UART1_TXA9_RXA10   = 0x04U,
    UART1_TXA9_RXB15   = 0x05U,
    UART1_TXB14_RXB7   = 0x06U,  
    UART1_TXB14_RXA10  = 0x07U,
    UART1_TXB14_RXB15  = 0x08U,
    // UART2 possible pin positions.
    UART2_TXD5_RXD6    = 0x09U,
    UART2_TXD5_RXA3    = 0x0AU,
    UART2_TXA2_RXD6    = 0x0BU,
    UART2_TXA2_RXA3    = 0x0CU,
    // UART3 possible pin positions.
    UART3_TXC10_RXC11  = 0x0DU,
    UART3_TXC10_RXD9   = 0x0EU,
    UART3_TXC10_RXB11  = 0x0FU,
    UART3_TXD8_RXC11   = 0x10U,
    UART3_TXD8_RXD9    = 0x11U,
    UART3_TXD8_RXB11   = 0x12U,
    UART3_TXB10_RXC11  = 0x13U,
    UART3_TXB10_RXD9   = 0x14U,
    UART3_TXB10_RXB11  = 0x15U,
    // UART4 possible pin positions.
    UART4_TXA12_RXD0   = 0x16U,
    UART4_TXA12_RXC11  = 0x17U,
    UART4_TXA12_RXA11  = 0x18U,
    UART4_TXC10_RXD0   = 0x1BU,
    UART4_TXC10_RXC11  = 0x1CU, 
    UART4_TXC10_RXA11  = 0x1DU,
    UART4_TXD1_RXD0    = 0x20U,
    UART4_TXD1_RXC11   = 0x21U,
    UART4_TXD1_RXA11   = 0x22U,
    // UART5 possible pin positions.
    UART5_TXC12_RXB12  = 0x2AU,
    UART5_TXC12_RXB5   = 0x2BU,
    UART5_TXC12_RXD2   = 0x2CU,
    UART5_TXB6_RXB12   = 0x2EU,
    UART5_TXB6_RXB5    = 0x2FU,
    UART5_TXB6_RXD2    = 0x30U,
    UART5_TXB13_RXB12  = 0x36U,
    UART5_TXB13_RXB5   = 0x37U,
    UART5_TXB13_RXD2   = 0x38U,
    // UART6 possible pin positions.
    UART6_TXG14_RXG9   = 0x3AU,
    UART6_TXG14_RXC7   = 0x3BU,
    UART6_TXC6_RXG9    = 0x3CU,
    UART6_TxC6_RXC7    = 0x3DU,
    // UART7 possible pin positions.
    UART7_TXB4_RXB3    = 0x3EU,
    UART7_TXB4_RXA8    = 0x3FU,
    UART7_TXB4_RXE7    = 0x40U,
    UART7_TXB4_RXF6    = 0x41U,
    UART7_TXA15_RXB3   = 0x42U,
    UART7_TXA15_RXA8   = 0x43U,
    UART7_TXA15_RXE7   = 0x44U,
    UART7_TXA15_RXF6   = 0x45U,
    UART7_TXE8_RXB3    = 0x46U,
    UART7_TXE8_RXA8    = 0x47U,
    UART7_TXE8_RXE7    = 0x48U,
    UART7_TXE8_RXF6    = 0x49U,
    UART7_TXF7_RXB3    = 0x4AU,
    UART7_TXF7_RXA8    = 0x4BU,
    UART7_TXF7_RXE7    = 0x4CU,
    UART7_TXF7_RXF6    = 0x4DU,
    // UART8 possible pin positions.
    UART8_TXE1_RXE0    = 0x4EU,
    // Unknown UART handler.
    UART_UNKNOWN       = 0xFFU
} UART_PinConfig;
    
/**
  * @brief UART baudrates could be configured with for communication.
  **/
enm
{
    UART_1200BPS   = 1200L,   ///< 1200 bits per second.
    UART_2400BPS   = 2400L,   ///< 2400 bits per second.
    UART_4800BPS   = 4800L,   ///< 4800 bits per second.
    UART_9600BPS   = 9600L,   ///< 9600 bits per second.
    UART_19200BPS  = 19200L,  ///< 19200 bits per second.
    UART_38400BPS  = 38400L,  ///< 38400 bits per second.
    UART_57600BPS  = 57600L,  ///< 57600 bits per second.
    UART_115200BPS = 115200L, ///< 115200 bits per second.
    UART_230400BPS = 230400L, ///< 230400 bits per second.
    UART_460800BPS = 460800L, ///< 460800 bits per second.
    UART_921600BPS = 921600L  ///< 921600 bits per second.
} UART_BaudRate;

/**
  * @brief UART handler state.
  **/
enm
{
    UART_IDLE   = 0x00U, ///< UART handler state at the beginning.
    UART_UNLOCK = 0x01U, ///< UART handler is unlocked, could be used by 
                         ///  process. 
    UART_LOCK   = 0x02U  ///< UART handler is locked, busy by some process.
} UART_State;

/**
  * @brief UART return codes.
  **/
enm
{
    UART_NOERROR       = 0x00U,
    UART_NULL_HANDLER  = 0x01U,
    UART_EMPTY_HANDLER = 0x02U
} UART_Error;

/**
  * @brief UART handler.
  **/
stc
{
    flg m_isInitialised;                    ///< Complited initialisation flag.
    USART_TypeDef* m_pInstance;             ///< UART handler instance pointer.
    UART_PinConfig m_config;                ///< UART pin configuration.
    UART_BaudRate m_baudrate;               ///< UART connection baudrate.
    UART_State m_state;                     ///< UART handler state.
    u8 m_rxBuffer[DEFAULT_UART_RXBUF_SIZE]; ///< UART Rx-buffer.
    flg m_isTxDmaInUse;                     ///< UART DMA usage.
} UART_Handler;

UART_Handler* UART_init(UART_PinConfig pinConfig, UART_BaudRate baud, 
                                                              flg isDmaEnabled);

#ifdef __cplusplus
}
#endif

#endif // __DRV_INTERFACE_USART_H__