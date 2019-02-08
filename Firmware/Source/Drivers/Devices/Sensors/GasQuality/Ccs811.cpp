/**
 *  @file       Ccs811.cpp (module file)
 *  @version    1.0.0
 *  @author     utuM (Kostyantyn Komarov)
 *  @date       06.02.2019 (implementation)
 *  @brief      CCS811 sensor class.
 **/

#include "Ccs811.h"

Ccs811ErrorCodes Driver::Ccs811::_init(void)
{
	memset(m_buffer, 0x00, sizeof(m_buffer));
}

/**
 * CCS811 class constructor.
 * @param[i] - settings set to be assigned to the sensors registers.
 **/
Driver::Ccs811::Ccs811(void) : Device("Ccs811"),
                               m_isInit(false),
							   m_co2(0),
							   m_tvoc(0),
							   m_error(kCss811NoError)
{
	// Prepare interface for communication.
    I2cParameters l_parameters = {
        0,
        s_kCcs811Address,
        kI2cNormalMode,
        true
    };
    // Initialize the sensor only if used interface was initialized
    // successfully.
    if (!m_interface.init(l_parameters)) {
        _init();
    }
    else {
        m_error = kBmeErrInterfaceIsntInit;
    }
}

/**
 * CCS811 class destructor.
 **/
Driver::Ccs811::~Ccs811(void)
{
	// Turn the sensor into the sleep mode on object destruction.
    if (m_isInit && m_error == kCss811NoError) {
        toggle(false);
        m_isInit = false;
    }
}
                        
Ccs811ErrorCodes Driver::Ccs811::update(uint16_t& rCo2, uint16_t& rTvoc)
{
	
}

Ccs811ErrorCodes Driver::Ccs811::toggle(bool isPowerOn)
{
	
}