/**
  *  @file       Vs1053b.cpp (module file)
  *  @version    0.1.0
  *  @author     utuM (Kostyantyn Komarov)
  *  @date       19.04.2019 (release)
  *  @brief      VS1053B decoder class implementation.
  **/

#include "Vs1053b.h"

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

    // Setting up the device registers.
    

    return (_m_error = kVs1053bNoError);
}

Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::_runSineTest(u16 frequency,
                                                                u16 duration)
{


    return (_m_error = kVs1053bNoError);
}

Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::_verify(void)
{


    return (_m_error = kVs1053bNoError);
}

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

Driver::Vs1053b::~Vs1053b(void)
{
    // Pull the device's 'RESET' pin into active level.
    if (_m_isInit && _m_error == kVs1053bNoError) {
        toggle(false);
        _m_isInit = false;
    }
}

Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::toggle(bool isTurnedOn)
{


    return (_m_error = kVs1053bNoError);
}