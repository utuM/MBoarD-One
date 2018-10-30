#include "Ds1307.h"
#include "i2c.h"

extern I2C_HandleTypeDef hi2c1;

void Driver::Ds1307::_init(void)
{	
	// Setting up device to 1Hz frequency.
	uint8_t l_settings = 0x10;
	HAL_I2C_Mem_Write(&hi2c1, s_kDs1307Address, kDs1307Control,
					  I2C_MEMADD_SIZE_8BIT, &l_settings, 1, 1000);
    // Enable clock oscillator
    l_settings = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, s_kDs1307Address, kDs1307Seconds,
					  I2C_MEMADD_SIZE_8BIT, &l_settings, 1, 1000);
}

Driver::Ds1307::Ds1307(void)
{
	
}

Driver::Ds1307::~Ds1307(void)
{
	
}

bool Driver::Ds1307::getDateTime(RtcDateTime& dateTime)
{
	uint8_t l_buffer[7] = {0};
	
	// Read registers step by step.
	for (uint8_t i = 0; i < 7; i ++) {
		if (HAL_I2C_Mem_Read(&hi2c1, s_kDs1307Address, kDs1307Seconds + i,
							 I2C_MEMADD_SIZE_8BIT, &l_buffer[i], 1, 1000) != HAL_OK) {
			return true;					  
		}
	}
	// Parse seconds.
	dateTime.m_seconds = ((l_buffer[0] >> 4) & 0x07) * 10 + 
						   (l_buffer[0] & 0x0F);
	// Parse minutes.
	dateTime.m_minutes = (l_buffer[1] >> 4) * 10 + 
						  (l_buffer[1] & 0x0F);
	// Parse hour format bit.
	l_buffer[2] & 0x40 ? dateTime.m_is24hFormat = false :
                         dateTime.m_is24hFormat = true;
	// Parse hours.
	dateTime.m_hours = ((l_buffer[2] >> 4) & 0x03) * 10 + 
						 (l_buffer[2] & 0x0F);
	// Parse week day.
	dateTime.m_weekDay = l_buffer[3];
	// Parse month day.
	dateTime.m_day = (l_buffer[4] >> 4) * 10 + 
				      (l_buffer[4] & 0x0F);
	// Parse month.
	dateTime.m_month = (l_buffer[5] >> 4) * 10 + 
						(l_buffer[5] & 0x0F);
	// Parse year.
	dateTime.m_year = (l_buffer[6] >> 4) * 10 + 
				       (l_buffer[6] & 0x0F);
	
	
	return false;
}

bool Driver::Ds1307::setDateTime(RtcDateTime& dateTime)
{
	uint8_t l_buffer[7] = {0};
    
    // Prepare time and date values.
    l_buffer[0] = ((dateTime.m_seconds / 10) << 4) | (dateTime.m_seconds % 10);
    l_buffer[1] = ((dateTime.m_minutes / 10) << 4) | (dateTime.m_minutes % 10);
    dateTime.m_is24hFormat = true ? l_buffer[2] = 0x00 : l_buffer[2] = 0x40;
    l_buffer[2] |= ((dateTime.m_hours / 10) << 4) | (dateTime.m_hours % 10);
    l_buffer[3] = dateTime.m_weekDay;
    l_buffer[4] = ((dateTime.m_day / 10) << 4) | (dateTime.m_day % 10);
    l_buffer[5] = ((dateTime.m_month / 10) << 4) | (dateTime.m_minutes % 10);
    l_buffer[6] = ((dateTime.m_year / 10) << 4) | (dateTime.m_minutes % 10);
	// Send new date and time.
    for (uint8_t i = 0; i < 7; i ++) {
		if (HAL_I2C_Mem_Write(&hi2c1, s_kDs1307Address, kDs1307Seconds + i,
							  I2C_MEMADD_SIZE_8BIT, &l_buffer[i], 1, 1000) != HAL_OK) {
			return true;					  
		}
	}
    
    
	return false;
}