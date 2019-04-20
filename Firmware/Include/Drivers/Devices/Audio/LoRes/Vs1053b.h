/**
  *  @file       Vs1053b.h (header file)
  *  @version    0.1.0
  *  @author     utuM (Kostyantyn Komarov)
  *  @date       19.04.2019 (release)
  *  @brief      VS1053B decoder class implementation.
  **/

#ifndef __DEVICE_VS1053B_H
#define __DEVICE_VS1053B_H

#include "Device.h"
#include "Gpio_c.h"
#include "Typedefs.h"

namespace Driver
{
    /**
      * VS1053B class.
      **/
    class Vs1053b
    {
        public:
            /**
              * VS1053B possible operations.
              **/
            enum Vs1053bOperation : u8
            {
                kVs1053bWrite = 0x02, ///< For write into register operations.
                kVs1053bRead          ///< For read from register operations.
            };
            
            /**
              * VS1053B available registers.
              **/
            enum Vs1053bRegisters : u8
            {
                kVs1053bMode       = 0x00, ///< Mode control.
                kVs1053bStatus,            ///< Status of the device.
                kVs1053bBass,              ///< Built-in bass/treble control.
                kVs1053bClockF,            ///< Clock freq + multiplier.
                kVs1053bDecodeTime,        ///< Decode time in seconds.
                kVs1053bAuData,            ///< Misc. audio data.
                kVs1053bWRam,              ///< RAM write/read.
                kVs1053bWRamAddr,          ///< Base address for RAM write/read.
                kVs1053bHDat0,             ///< Stream header data 0.
                kVs1053bHDat1,             ///< Stream header data 1.
                kVs1053bAiAddr,            ///< Start address of application.
                kVs1053bVol,               ///< Volume control.
                kVs1053bAiCtrl0,           ///< Application control register 0.
                kVs1053bAiCtrl1,           ///< Application control register 1.
                kVs1053bAiCtrl2,           ///< Application control register 2.
                kVs1053bAiCtrl3            ///< Application control register 3.
            };
            
            /**
              * VS1053B 'kVs1053bMode' bit values, for R/W.
              **/
            enum Vs1053bModeRegister : u16
            {
                kVs1053bDifferential  = 0x0001, ///< Left channel inverted.
                kVs1053bAllowMpeg1_2  = 0x0002, ///< Allow MPEG layers 1 and 2.
                kVs1053bSoftReset     = 0x0004, ///< Provides soft reset.
                kVs1053bCancelCurDec  = 0x0008, ///< Cancels current decoding.
                kVs1053bSpeakerLowSet = 0x0010, ///< Enables low settings.
                kVs1053bSdiTests      = 0x0020, ///< Allows SDI tests.
                kVs1053bStreamMode    = 0x0040, ///< Enables stream mode. 
                kVs1053bSpeakerHiSet  = 0x0080, ///< Enables high settings.
                kVs1053bDClkActEdge   = 0x0100, ///< Falling edge is active for
                                                ///  clock.
                kVs1053bSdiBitOrder   = 0x0200, ///< Bit order is LSB.
                kVs1053bShareCSelect  = 0x0400, ///< Shares SPI CS-pin.
                kVs1053bNativeSpiMode = 0x0800, ///< Uses native VS1002 modes.
                kVs1053bAdpcmActRec   = 0x1000, ///< Active ADPCM recording. 
                kVs1053bLine1Select   = 0x4000, ///< Select the LINE1.
                kVs1053bInput24_26MHz = 0x8000  ///< Input clock range is 
                                                ///  24..26MHz.
            };
          
            /**
              * VS1053B 'kVs1053bStatus' bit values, for R/W.
              **/
            enum Vs1053bStatusRegister : u16
            {
                kVs1053bReference  = 0x0001, ///< Reference voltage is 1.65V,
                                             ///  else - 1.23V.
                kVs1053bAdClock    = 0x0002, ///< AD clock select is 3MHz,
                                             ///  else - 6MHz.
                kVs1053bApDown1    = 0x0004, ///< Analog internal powerdown.
                kVs1053bApDown2    = 0x0008, ///< Analog driver powerdown.
                kVs1053bVcmDisable = 0x0400, ///< GBUF overload detection.
                kVs1053bVcmOverld  = 0x0800, ///< GBUF overload indicator.
                kVs1053bDoNotJump  = 0x8000, ///< Header in decode.
            };
            
            /**
              * VS1053B possible swing values.
              **/ 
            enum Vs1053bSwing : u16
            {
                kVs1053bVolume0_5dB = 0x1000, ///< +0.5 dB.
                kVs1053bVolume1_0dB = 0x2000  ///< +1.0 dB.
                // Other values, up to +3.0dB, do not work, according to
                // datasheet. 
            };
            
            /**
              * VS1053B possible error codes.
              **/
            enum Vs1053bErrorCode : u8
            {
                kVs1053bNoError = 0x00,       ///< No error was occured.
                kVs1053bErrInterfaceIsntInit, ///< The SPI interface hasn't been
                                              ///  initialized correctly.
                
            };
          
            // Constructor and destructor.
            Vs1053b(void);
            ~Vs1053b(void);
            
            // Setter and getters.
            Vs1053bErrorCode setVolume(const u8 value);
            u8 getVolume(u8 value);
            flag getInit(void) { return _m_isInit; }
            u8 getError(void) { return (u8)_m_error; }
            
            // Basic methods.
            Vs1053bErrorCode playAudioSample(u8* pData, u16 size);
            Vs1053bErrorCode toggle(bool isTurnedOn);
      
        private:
            flag _m_isInit;            ///< Properly initialization flag.
            InputPin* _m_pPinDreq;     ///< 'DREQ' pin object, input, no-pull.
            OutputPin* _m_pPinSs;      ///< 'SS' pin object, output.
            OutputPin* _m_pPinDc;      ///< 'D/C' pin object, output.
            OutputPin* _m_pPinRst;     ///< 'RESET' pin object, output.
            Vs1053bErrorCode _m_error; ///< Current error.
            
            // Private basic methods.
            Vs1053bErrorCode _init(void);
            Vs1053bErrorCode _runSineTest(u16 frequency,
                                          u16 duration);
            Vs1053bErrorCode _verify(void);
    };
}

#endif // __DEVICE_VS1053B_H