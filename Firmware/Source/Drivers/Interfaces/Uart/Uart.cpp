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

#include "Uart.h"

#include <stdio.h>
#include <string.h>

static bool s_isUartInit[2] = { false, false }; ///< UART initialization flags.
static Driver::Uart* s_pGlobalUart[2] = {       ///< Global UARTs handlers.
   nullptr, nullptr
};

/**
  * UART deinitialization.
  **/
void Driver::Uart::_deinit(void)
{
    m_isInit = false;
    HAL_UART_AbortReceive(m_pHandler);
    HAL_UART_DeInit(m_pHandler);
    // Separated deinitialization according to binded UART instance.
    switch (m_instance) {
        case kUart1 : {
            s_pGlobalUart[0] = nullptr;
            __HAL_RCC_USART1_CLK_DISABLE();
            HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
        
            break;
        }

        case kUart2 : {
            s_pGlobalUart[1] = nullptr;
            __HAL_RCC_USART2_CLK_DISABLE();
            HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);
        
            break;
        }

        default : {
            break;
        }
    }
    HAL_DMA_DeInit(m_pDma);

    delete[] m_pRxBuffer;
    delete m_pHandler;
    delete m_pDma;
}

/**
  * UART class destructor.
  **/
Driver::Uart::~Uart(void)
{
    _deinit();
}

/**
  * UART initialization. 
  * @param[i] UartParameters& rParams - reference to parameters structure
  *           object.
  * @return true  - some error during initialization process;
  *         false - no error.
  **/
bool Driver::Uart::init(UartParameters& rParams)
{
    // Check if UART instance hasn't been initialized yet.
    if (!s_isUartInit[rParams.m_index]) {
        m_pHandler = new UART_HandleTypeDef;
        switch (rParams.m_index) {
            case kUart1 :
                __HAL_RCC_USART1_CLK_ENABLE();
                m_pHandler->Instance = USART1;
                break;
                
            case kUart2 :
                __HAL_RCC_USART2_CLK_ENABLE();
                m_pHandler->Instance = USART2;
                break;
                
            default :
                break;
        }
        m_pHandler->Init.BaudRate = (uint32_t)rParams.m_baud;
        m_pHandler->Init.WordLength = rParams.m_dataSize;
        m_pHandler->Init.StopBits = rParams.m_stopBits;
        m_pHandler->Init.Parity = rParams.m_parity;
        m_pHandler->Init.Mode = UART_MODE_TX_RX;
        m_pHandler->Init.HwFlowCtl = UART_HWCONTROL_NONE;
        m_pHandler->Init.OverSampling = UART_OVERSAMPLING_16;
        m_pHandler->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        m_pHandler->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init(m_pHandler) != HAL_OK) {
            return true;
        }
        m_pRxBuffer = new uint8_t[s_kUartBufferSize]{};
        
        s_isUartInit[rParams.m_index] = true;  
        s_pGlobalUart[rParams.m_index] = this;

        // Clocks, GPIOs and DMA initialization section.
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        switch (m_instance) {
            case kUart1 : {
                // USART1 clock enable.
                __HAL_RCC_GPIOA_CLK_ENABLE();
                /**
                  * USART1 GPIO Configuration    
                  * PA9     ------> USART1_TX
                  * PA10    ------> USART1_RX 
                  **/
                GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
                GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
                GPIO_InitStruct.Pull = GPIO_NOPULL;
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
                GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

                // USART1 DMA initialization.
                __HAL_RCC_DMA1_CLK_ENABLE();
                HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

                m_pDma = new DMA_HandleTypeDef;
                m_pDma->Instance = DMA1_Channel5;
                m_pDma->Init.Request = DMA_REQUEST_2;
                m_pDma->Init.Direction = DMA_PERIPH_TO_MEMORY;
                m_pDma->Init.PeriphInc = DMA_PINC_DISABLE;
                m_pDma->Init.MemInc = DMA_MINC_ENABLE;
                m_pDma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
                m_pDma->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
                m_pDma->Init.Mode = DMA_CIRCULAR;
                m_pDma->Init.Priority = DMA_PRIORITY_LOW;
                if (HAL_DMA_Init(m_pDma) != HAL_OK) {
                    return true;
                }
                __HAL_LINKDMA(m_pHandler, hdmarx, *m_pDma);
                
                break;
            }

            case kUart2 : {
                // USART2 clock enable.
                __HAL_RCC_GPIOA_CLK_ENABLE();
                /**
                  * USART2 GPIO Configuration    
                  * PA2     ------> USART2_TX
                  * PA3     ------> USART2_RX 
                  **/
                GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
                GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
                GPIO_InitStruct.Pull = GPIO_NOPULL;
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
                GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

                // USART2 DMA initialization.
                __HAL_RCC_DMA1_CLK_ENABLE();
                HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

                m_pDma = new DMA_HandleTypeDef;
                m_pDma->Instance = DMA1_Channel6;
                m_pDma->Init.Request = DMA_REQUEST_2;
                m_pDma->Init.Direction = DMA_PERIPH_TO_MEMORY;
                m_pDma->Init.PeriphInc = DMA_PINC_DISABLE;
                m_pDma->Init.MemInc = DMA_MINC_ENABLE;
                m_pDma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
                m_pDma->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
                m_pDma->Init.Mode = DMA_CIRCULAR;
                m_pDma->Init.Priority = DMA_PRIORITY_LOW;
                if (HAL_DMA_Init(m_pDma) != HAL_OK) {
                    return true;
                }
                __HAL_LINKDMA(m_pHandler, hdmarx, *m_pDma);

                break;
            }

            default : {
                break;
            }
        }

        m_isInit = true;
    }
    // Otherwise: copy global UART handler into current one.
    else {
        m_pHandler = s_pGlobalUart[rParams.m_index]->m_pHandler;
    }
    // Finish initialization.
    m_instance = rParams.m_index;
    m_timeout = rParams.m_timeout;
    HAL_UART_Receive_DMA(m_pHandler, m_pRxBuffer, s_kUartBufferSize);
    
    return false;
}

/**
  * Byte sending process.
  * @param[i] reg   - can be ignored.
  * @param[i] value - byte that need to be sent.
  * @return true  - object hasn't been initialized properly or failed sending;
  *         false - successful sending.
  **/
bool Driver::Uart::sendByte(uint8_t reg, uint8_t value)
{
    // Check if current object was initialized correctly.
    if (!m_isInit) {
        return true;
    }
    // Transmit 1 byte.
    if (HAL_UART_Transmit(m_pHandler, &reg, 1, s_kUartDefaultTimeout) != 
                                                                       HAL_OK) {
        return true;
    }
    
    return false;
}

/**
  * Data array sending process.
  * @param[i] reg   - can be ignored.
  * @param[i] pData - pointer to data array which need to be sent.
  * @param[i] size  - amount of bytes to be sent.
  * @return true  - object hasn't been initialized properly, input size is above
  *                 the size of RX-buffer, input pointer to array is null or
  *                 failed sending;
  *         false - successful sending.
  **/
bool Driver::Uart::sendData(uint8_t reg, uint8_t* pData, const uint16_t size)
{
    // Check if current object was initialized correctly and input amount of
    // bytes to send is above or equal to buffer size.
    if (!m_isInit || size > s_kUartBufferSize || pData == nullptr) {
        return true;
    }
    // Transmit required amount of bytes (total is limited by buffer size).
    if (HAL_UART_Transmit(m_pHandler, pData, size, s_kUartDefaultTimeout) !=
                                                                       HAL_OK) {
        return true;
    }
    
    return false;
}

/**
  * Byte reading process (reading from internal object RX-buffer).
  * @param[i] reg    - can be ignored.
  * @param[i] rValue - reference to outer byte variable need to be filled.
  * @param[i] size   - amount of bytes to be sent.
  * @return true  - object hasn't been initialized properly or failed sending;
  *         false - successful reading.
  **/
bool Driver::Uart::receiveByte(uint8_t reg, uint8_t& rValue)
{
    // Check if current object was initialized correctly.
    if (!m_isInit) {
        return true;
    }
    // Try to read 1 byte.
    if (updateRxCounter() > 0) {
        rValue = m_pRxBuffer[m_startIndex];
        if (m_startIndex >= (s_kUartBufferSize - 1)) {
            m_startIndex = 0;
        }
        else {
            ++ m_startIndex;
        }

        return false;
    }
    
    return true;
}

/**
  * Data array reading process (reading from internal object RX-buffer).
  * @param[i] reg    - can be ignored.
  * @param[i] pData  - pointer to data array which need to be filled.
  * @param[i] size   - amount of bytes to be filled.
  * @return true  - object hasn't been initialized properly, input size is above
  *                 the size of RX-buffer, input pointer to array is null or
  *                 failed reading;
  *         false - successful reading.
  **/
bool Driver::Uart::receiveData(uint8_t reg, uint8_t* pData, const uint16_t size)
{
    // Check if current object was initialized correctly.
    if (!m_isInit || size > s_kUartBufferSize || pData == nullptr) {
        return true;
    }
    // Try to read all bytes that is available now.
    uint16_t l_length = 0;
    uint8_t l_byte;
    while (l_length < size && updateRxCounter() > 0) {
        if (receiveByte(0, l_byte)) {
            break;
        }
        pData[l_length ++] = l_byte;
    }
    
    return false;
}

/**
  * Update available amount of bytes that can be read.
  * @return 0       - object has been initialized properly or no bytes left to
  *                   read;
  *         above 0 - available amount of bytes to be read. 
  **/
uint16_t Driver::Uart::updateRxCounter(void)
{
    if (!m_isInit) {
        return 0;
    }
    // Calculate amount of bytes available to be read from RX-buffer.
    uint16_t l_writeIndex = s_kUartBufferSize - 
                            m_pHandler->hdmarx->Instance->CNDTR;
    if (l_writeIndex < m_startIndex) {
        return (s_kUartBufferSize - m_startIndex + l_writeIndex);
    }
    else {
        return (l_writeIndex - m_startIndex);
    }
}

/**
  * Reseting reading start index.
  **/
void Driver::Uart::resetRxCounter(void)
{
    m_startIndex = s_kUartBufferSize - m_pHandler->hdmarx->Instance->CNDTR;
}

/**
  * Friendly method to override DMA interrupt.
  * @param[i] pObject - pointer to Driver::Uart class object from where
  *                     DMA handler need to be used. 
  **/
void Driver::dmaProcessing(Driver::Uart* pObject)
{
    if (pObject == nullptr) {
        return;
    }

    HAL_DMA_IRQHandler(pObject->m_pDma);
}

/**
  * Handles DMA global interrupt on USART1 RX-event.
  **/
extern "C" void DMA1_Channel5_IRQHandler(void)
{
    dmaProcessing(s_pGlobalUart[0]);
}

/**
  * Handles DMA global interrupt on USART2 RX-event.
  **/
extern "C" void DMA1_Channel6_IRQHandler(void)
{
    dmaProcessing(s_pGlobalUart[1]);
}
