/**
 *  @file       Uart.h (header file)
 *  @version    1.0.1
 *  @author     utuM aka Kostyantyn Komarov
 *  @date       30.12.2018 (release)
 *  @date       26.01.2018 (update, version 1.0.1)
 *  @brief      UART interface class.
 *              Represented as a child of 'Interface' basic class. Can be used
 *              as template object for 'Device' class objects.
 *              Receiving operating is released with NVIC and DMA (1.0.1).
 *  @update     New in version 1.0.1:
 *              + added RX receiving through DMA;
 *              + minor changes in global things;
 *              + minor changes in class structure.
 **/

#ifndef __INTERFACE_UART_H
#define __INTERFACE_UART_H

#include "Interface.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_def.h"

namespace Driver {
    const uint16_t s_kUartBufferSize = 512;      ///< Default buffer size for
                                                 ///  TX/RX operations.
    const uint16_t s_kUartDefaultTimeout = 1000; ///< Default timeout for UART
                                                 ///  transceiving operations.
  
    /**
      * UART instance.
      **/
    enum UartInstance : uint8_t {
        kUart1 = 0x00, ///< UART1 instance.
        kUart2         ///< UART2 instance.
    };
  
    /**
      * UART baud rate value.
      **/
    enum UartBaudrate : uint32_t {
        kUart9600   = 9600,
        kUart19200  = 19200,
        kUart38400  = 38400,
        kUart57600  = 57600,
        kUart115200 = 115200,
        kUart230400 = 230400
    };
  
    /**
      * UART data size.
      **/
    enum UartDataSize : uint32_t {
        kUart7Bits = UART_WORDLENGTH_7B, ///< 7 data bits.
        kUart8Bits = UART_WORDLENGTH_8B, ///< 8 data bits.
        kUart9Bits = UART_WORDLENGTH_9B  ///< 9 data bits.
    };
    
    /**
      * UART stop bits.
      **/
    enum UartStopBits : uint32_t {
        kUart0_5Bits = UART_STOPBITS_0_5, ///< 0.5 stop bits.
        kUart1_0Bits = UART_STOPBITS_1,   ///< 1 stop bits.
        kUart1_5Bits = UART_STOPBITS_1_5, ///< 1.5 stop bits.
        kUart2_0Bits = UART_STOPBITS_2    ///< 2 stop bits.
    };
    
    /**
      * UART parity control.
      **/
    enum UartParity : uint32_t {
        kUartNoneParity = UART_PARITY_NONE, ///< None parity. 
        kUartOdd        = UART_PARITY_ODD,  ///< Odd parity.
        kUartEven       = UART_PARITY_EVEN  ///< Even parity.
    };
    
    /**
      * UART parameters structure.
      **/  
    struct UartParameters {
        UartInstance m_index;    ///< Index of UART instance.
        UartBaudrate m_baud;     ///< Baud rate.
        UartDataSize m_dataSize; ///< Data size in bits.
        UartStopBits m_stopBits; ///< Stop bits.
        UartParity m_parity;     ///< Parity control.
        uint32_t m_timeout;      ///< Timeout value in blocked mode (ingored in
                                 ///  non-blocked mode).
    };
  
    /**
     * UART interface class.
     **/  
    class Uart : public Interface<UartParameters> {
        private:
            bool m_isInit;                         ///< UART initialization
                                                   ///  init.
            UART_HandleTypeDef* m_pHandler;        ///< Current UART handler.
            DMA_HandleTypeDef*  m_pDma;            ///< Current DMA handler.
            UartInstance m_instance;               ///< Current UART instance.
            uint8_t* m_pRxBuffer;                  ///< UART RX-buffer.
            uint16_t m_startIndex;                 ///< Start index from where
                                                   ///  byte need to be read.
            uint32_t m_timeout;                    ///< Timeout value in blocked
                                                   ///  mode.
            
            void _deinit(void);
      
        public:
            // Constructor and destructor.
            Uart(void) : m_isInit(false),
                         m_startIndex(0) { /* Leave empty. */ }
            ~Uart(void);
            
            // Basic methods.
            bool init(UartParameters& rParams);
            bool sendByte(uint8_t reg, uint8_t value);
            bool sendData(uint8_t reg, uint8_t* pData, const uint16_t size);
            bool receiveByte(uint8_t reg, uint8_t& rValue);
            bool receiveData(uint8_t reg, uint8_t* pData, const uint16_t size);
            uint16_t updateRxCounter(void);
            void resetRxCounter(void);
            friend void dmaProcessing(Uart* pObject);
    };

    void dmaProcessing(Uart* pObject);
}

#endif // __INTERFACE_UART_H