#include "Ds1307.h"

extern I2C_HandleTypeDef hi2c1;

void Driver::Ds1307::_init(void)
{	
	// Setting up device to 1Hz frequency.
	uint8_t l_settings = 0x10;
	HAL_I2C_Mem_Write(&hi2c1, s_kDs1307Address, kDs1307Control,
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
	dateTime->m_seconds = ((l_buffer[0] >> 4) & 0x07) * 10 + 
						   (l_buffer[0] & 0x0F);
	// Parse minutes.
	dateTime->m_minutes = (l_buffer[1] >> 4) * 10 + 
						  (l_buffer[1] & 0x0F);
	// Parse hour format bit.
	l_buffer[2] & 0x40 ? dateTime->m_is24hFormat = true : dateTime->m_is24hFormat = false;
	// Parse hours.
	dateTime->m_hours = ((l_buffer[2] >> 4) & 0x03) * 10 + 
						 (l_buffer[2] & 0x0F);
	// Parse week day.
	dateTime->m_weekDay = l_buffer[3];
	// Parse month day.
	dateTime->m_day = (l_buffer[4] >> 4) * 10 + 
				      (l_buffer[4] & 0x0F);
	// Parse month.
	dateTime->m_month = (l_buffer[5] >> 4) * 10 + 
						(l_buffer[5] & 0x0F);
	// Parse year.
	dateTime->m_year = (l_buffer[6] >> 4) * 10 + 
				       (l_buffer[6] & 0x0F);
	
	
	return false;
}

bool Driver::Ds1307::setDateTime(RtcDateTime& dateTime)
{
	
	
	return false;
}