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
    return (m_error = kVs1053bNoError);
}

Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::_verify(void)
{
    return (m_error = kVs1053bNoError);
}

Driver::Vs1053b::Vs1053b(void) : m_isInit(false),
                                 m_error(kVs1053bNoError)
{

}

Driver::Vs1053b::~Vs1053b(void)
{

}

Driver::Vs1053b::Vs1053bErrorCode Driver::Vs1053b::toggle(bool isTurnedOn)
{
    return (m_error = kVs1053bNoError);
}