/**
  *  @file       Vs1053b.cpp (module file)
  *  @version    0.1.0
  *  @author     utuM (Kostyantyn Komarov)
  *  @date       19.04.2019 (creation)
  *  @brief      VS1053B decoder class implementation.
  **/

#include "Vs1053b.h"

#include "spi.h"

#define DEFAULT_TIMEOUT_TICKS    1000
#define DEFAULT_DATA_SAMPLE_SIZE   32
#define MAXIMAL_SAMPLE_SIZE       448

static bool s_isSciMode = true;

static void reinitSpi(bool isSciMode)
{
    if (s_isSciMode == isSciMode) {
        return;
    }
  
    HAL_SPI_DeInit(&hspi1);
    
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    if (!isSciMode) {
        hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    }
    
    HAL_SPI_Init(&hspi1);
}

/**
  *
  **/
Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::_sciRead(u8 reg, u16 rValue)
{    
    // Prepare data before send/receive.
    u8 l_txData[4] = { kVs1053bRead, reg, 0x00, 0x00 };
    u8 l_rxData[4] = { 0x00 };
    // Command the device and wait response.
    _m_pPinDc->setPinState(Driver::OutputPin::kStateSet);
    _m_pPinSs->setPinState(Driver::OutputPin::kStateReset);
    if (!_waitForDeviceReady()) {
        _m_pPinSs->setPinState(Driver::OutputPin::kStateSet);
        return (_m_error = kVs1053bErrDeviceIsntReady);
    }
    // Send command and receive data.
    // TODO: implement other methods with SPI class.
    rValue = 0;
    reinitSpi(true);
    if (HAL_SPI_TransmitReceive(&hspi1, l_txData, l_rxData, 4,
                                DEFAULT_TIMEOUT_TICKS) != HAL_OK) {
        _m_error = kVs1053bErrReadRegister;
    }
    else {
        _m_error = kVs1053bNoError;
        rValue = ((u16)l_rxData[2] << 8) | l_rxData[3];
    }
    _m_pPinSs->setPinState(Driver::OutputPin::kStateSet);
    
    return _m_error;
}

/**
  *
  **/
Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::_sciWrite(const u8 reg,
                                                             u16 value)
{   
    // Prepare data before send.
    u8 l_txData[4] = { kVs1053bWrite, reg, (u8)(value >> 8), (u8)value };
    // Command the device and wait response.
    _m_pPinDc->setPinState(Driver::OutputPin::kStateSet);
    _m_pPinSs->setPinState(Driver::OutputPin::kStateReset);
    if (!_waitForDeviceReady()) {
        _m_pPinSs->setPinState(Driver::OutputPin::kStateSet);
        return (_m_error = kVs1053bErrDeviceIsntReady);
    }
    // Send command and receive data.
    // TODO: implement other methods with SPI class.
    reinitSpi(true);
    if (HAL_SPI_Transmit(&hspi1, l_txData, 4,
                         DEFAULT_TIMEOUT_TICKS) != HAL_OK) {
        _m_error = kVs1053bErrWriteRegister;           
    }
    else {
        _m_error = kVs1053bNoError;
    }
    _m_pPinSs->setPinState(Driver::OutputPin::kStateSet);
    
    return _m_error;
}

/**
  *
  **/
Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::_sdiWrite(u8* pData,
                                                             u16 size)
{ 
    // Firstly, check if input buffer isn't 0.
    if (pData == nullptr) {
        return (_m_error = kVs1053bErrNullInputBuffer);
    }
    // TODO: check if more than 32 bytes buffer could be used for transmitting.
    if (size > DEFAULT_DATA_SAMPLE_SIZE) {
        size = DEFAULT_DATA_SAMPLE_SIZE;
    }
    // Prepare pins for data transaction.
    _m_pPinSs->setPinState(Driver::OutputPin::kStateSet);
    _m_pPinDc->setPinState(Driver::OutputPin::kStateReset);
    if (!_waitForDeviceReady()) {
        _m_pPinDc->setPinState(Driver::OutputPin::kStateSet);
        return (_m_error = kVs1053bErrDeviceIsntReady);
    }
    // Send data.
    // TODO: implement other methods with SPI class.
    reinitSpi(false);
    if (HAL_SPI_Transmit(&hspi1, pData, size,
                         DEFAULT_TIMEOUT_TICKS) != HAL_OK) {
        _m_error = kVs1053bErrSendData;           
    }
    else {
        _m_error = kVs1053bNoError;
    }
    _m_pPinDc->setPinState(Driver::OutputPin::kStateSet);
    
    return _m_error;
}

/**
  *
  **/
flag Driver::Vs1053b::_waitForDeviceReady(void)
{
    u16 l_timeout = DEFAULT_TIMEOUT_TICKS;
    flag l_state = yes;
    // Wait until 'DREQ' state is 0 or timeout is 0.
    while ((l_state = _m_pPinDreq->getPinState()) || (-- l_timeout));
    if (!l_state) {
        return true;
    }
    
    return false;
}

/**
  *
  **/
Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::_init(void)
{
    // Init every additional pin to communicate properly with the device.
    _m_pPinDreq = new Driver::InputPin("CODEC_PIN_DREQ",
                                       Driver::Gpio::kGpioPortC,
                                       Driver::Gpio::kGpioPin0,
                                       Driver::Gpio::kGpioSpeedHigh,
                                       Driver::InputPin::kInputNoPull);
    _m_pPinSs  = new Driver::OutputPin("CODEC_PIN_SS",
                                       Driver::Gpio::kGpioPortC,
                                       Driver::Gpio::kGpioPin1,
                                       Driver::Gpio::kGpioSpeedHigh,
                                       Driver::OutputPin::kStateSet);
    _m_pPinDc  = new Driver::OutputPin("CODEC_PIN_DC",
                                       Driver::Gpio::kGpioPortC,
                                       Driver::Gpio::kGpioPin2,
                                       Driver::Gpio::kGpioSpeedHigh,
                                       Driver::OutputPin::kStateReset);
    _m_pPinRst = new Driver::OutputPin("CODEC_PIN_RESET",
                                       Driver::Gpio::kGpioPortC,
                                       Driver::Gpio::kGpioPin3,
                                       Driver::Gpio::kGpioSpeedHigh,
                                       Driver::OutputPin::kStateSet);

    // Verify device.
    if (_verify() != kVs1053bNoError) {
        return (_m_error = kVs1053bErrIsntVerified);
    }
    // Setting up the device registers.
    _sciWrite(kVs1053bMode, kVs1053bNativeSpiMode);
    _sciWrite(kVs1053bBass, 0x7A00);
    _sciWrite(kVs1053bClockF, 0xC000);
    _sciWrite(kVs1053bVol, 0x5050);
    
    _m_isInit = true;
    
    return (_m_error = kVs1053bNoError);
}

/**
  *
  **/
Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::_runSineTest(u16 frequency,
                                                                u16 duration)
{
    if (!_m_isInit) {
        return (_m_error = kVs1053bNoError);
    }
    
    // Prepare all parameters and data before initialize the sine generation.
    // TODO: make the frequency parameter dynamically according to input value.
    u8 l_frequency = 0xE0 | 0x2E; // 1200 Hz.
    u8 l_startFreq[] = { 0x53, 0xEF, 0x6E, l_frequency,
                         0x00, 0x00, 0x00, 0x00 };
    u8 l_endFreq[] = { 0x45, 0x78, 0x69, 0x74, 0x00, 0x00, 0x00, 0x00 };
    // Change SPI mode and run before duration is over.
    if (_sdiWrite(l_startFreq, 8) != kVs1053bNoError) {
        return _m_error;
    }
    HAL_Delay(duration);
    if (_sdiWrite(l_endFreq, 8) != kVs1053bNoError) {
        return _m_error;
    }
    reinitSpi(true);

    return (_m_error = kVs1053bNoError);
}

/**
  *
  **/
bool Driver::Vs1053b::_verify(void)
{
    // Verify device by model version.
    u16 l_value = 0x0000;
    _sciRead(kVs1053bStatus, l_value);
    // For VS1053B, version need to be '4'.
    if (_m_error != kVs1053bNoError &&
        ((0x000F & (l_value >> 4)) != 4)) {
        return false;
    }

    return true;
}

/**
  *
  **/
Driver::Vs1053b::Vs1053b(void) : _m_isInit(false),
                                 _m_error(kVs1053bNoError)
{
    // TODO: need to place initialization of 'm_interface' field here.

    // Initialize the device only if used interface was initialized
    // successfully.
    //if (!m_interface.init(l_parameters)) {
        _init();
    /*}
    else {
        _m_error = kVs1053bErrInterfaceIsntInit;
    }*/
}

/**
  *
  **/
Driver::Vs1053b::~Vs1053b(void)
{
    // Pull the device's 'RESET' pin into active level.
    if (_m_isInit && _m_error == kVs1053bNoError) {
        toggle(false);
        _m_isInit = false;
    }
}

/**
  *
  **/
Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::setVolume(const u8 value)
{
    if (!_m_isInit) {
        return (_m_error = kVs1053bErrDeviceIsntInit);
    }
    
    // Prepare the volume value and command the device.
    u8 l_invValue = 255 - value;
    u16 l_volume = ((u16)l_invValue << 8) | l_invValue;
    reinitSpi(true);
    _sciWrite(kVs1053bVol, l_volume);
  
    return _m_error;
}

/**
  *
  **/
Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::getVolume(u8& rValue)
{
    if (!_m_isInit) {
        return (_m_error = kVs1053bErrDeviceIsntInit);
    }
    
    // Read current volume value.
    u16 l_volume = 0xFFFF; // Indicates 0 volume level.
    reinitSpi(true);
    _sciRead(kVs1053bVol, l_volume);
    if (_m_error != kVs1053bNoError) {
        rValue = 0x00;
        return _m_error;
    }
    rValue = (u8)l_volume;
  
    return _m_error;
}

/**
  *
  **/
Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::feedAudioSample(u8* pData,
                                                                   u16 size)
{
    if (!_m_isInit) {
        return (_m_error = kVs1053bErrDeviceIsntInit);
    }
    if (pData != nullptr || !size) {
        return (_m_error = kVs1053bErrInvalidParams);
    }
    
    if (size > MAXIMAL_SAMPLE_SIZE) {
        size = MAXIMAL_SAMPLE_SIZE;
    }
    // Prepare audio buffer and send portions of data to the device.
    u16 l_index = 0;            
    while (size) {
        if (size >= DEFAULT_DATA_SAMPLE_SIZE) {
            _sdiWrite(&pData[l_index], DEFAULT_DATA_SAMPLE_SIZE);
            size -= DEFAULT_DATA_SAMPLE_SIZE;
            l_index += DEFAULT_DATA_SAMPLE_SIZE;
        }
        else {
            _sdiWrite(&pData[l_index], size);
          
            break;
        }
    }
    
  
    return (_m_error = kVs1053bNoError);
}

/**
  *
  **/
Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::toggle(bool isTurnedOn)
{
    if (!_m_isInit) {
        return (_m_error = kVs1053bErrDeviceIsntInit);
    }

    // Change the pin state to toggle device power in hardware way.
    if (isTurnedOn) {
        _m_pPinRst->setPinState(Driver::OutputPin::kStateSet);
    }
    else {
        _m_pPinRst->setPinState(Driver::OutputPin::kStateReset);
    }
    
    return (_m_error = kVs1053bNoError);
}