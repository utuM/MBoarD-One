/**
 *  @file       Spi.h
 *  @version    0.1 (header file)
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       19.04.2019 (creation)
 *  @brief      SPI interface class implementation.
 **/

#ifndef __INTERFACE_SPI_H
#define __INTERFACE_SPI_H

#include "Interface.h"
#include "stm32f7xx_hal.h"
#include "Typedefs.h"

namespace Driver
{  
    const u16 m_kSpiDefaultTimeout = 1000; ///< Default timeout value after
                                           ///  which the data exchange process
                                           ///  must be finished.
  
    /**
      * SPI instance.
      **/
    enum SpiInstance : u32
    {
#if defined(STM32F746xx)
        kSpi1 = SPI1_BASE,
        kSpi2 = SPI2_BASE,
        kSpi3 = SPI3_BASE,
        kSpi4 = SPI4_BASE,
        kSpi5 = SPI5_BASE,
        kSpi6 = SPI6_BASE
#else
    #error "No SPI instance is defined!"
#endif
    };
    
    /**
      * SPI interface baudrate prescalers.
      **/
    enum SpiSpeed : u32
    {
        kSpiPrescaler2   = 0x00000000,
        kSpiPrescaler4   = 0x00000008,
        kSpiPrescaler8   = 0x00000010,
        kSpiPrescaler16  = 0x00000018,
        kSpiPrescaler32  = 0x00000020,
        kSpiPrescaler64  = 0x00000028,
        kSpiPrescaler128 = 0x00000030,
        kSpiPrescaler256 = 0x00000038
    };
  
    /**
      * Global SPI parameters for SPI class objects.
      **/
    struct SpiParameters
    {
        u8 m_index;       ///< Index of SPI bus to be in use.
        SpiSpeed m_speed; ///< Interface prescaler.
        
        flag m_useDma;    ///< DMA usage flag (reserved for future).
    };  
    
    /**
      * SPI template class.
      **/
    class Spi : public Interface<SPI_HandleTypeDef, SpiParameters>
    {
        public:
            SPI_HandleTypeDef m_handler;
            
            // Constructor and destructor.
            Spi(void) : _m_isInit(no), _m_isDmaOn(no)
            {
                /* Leave empty. */
            }
            ~Spi(void);
            
            // Basic methods.
            flag init(SpiParameters& rParameters);
            flag sendByte(uint8_t reg, uint8_t value);
            flag sendData(uint8_t reg, uint8_t* pData, const uint16_t size);
            flag receiveByte(uint8_t reg, uint8_t& rValue);
            flag receiveData(uint8_t reg, uint8_t* pData, const uint16_t size);
      
        private:
            flag _m_isInit;  ///< Successful initialization flag.
            flag _m_isDmaOn; ///< DMA usage flag (reserved for future usage).

            // Private basic methods.
            void _deinit(void);
    };
}

#endif // __INTERFACE_SPI_H