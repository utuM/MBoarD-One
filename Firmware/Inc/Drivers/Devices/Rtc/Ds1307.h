#ifndef __DRIVERS_DS1307_H
#define __DRIVERS_DS1307_H

#include <stdint.h>

namespace Driver {	
    const uint16_t s_kDefaultStartYear = 2019;
	const uint8_t s_kDs1307Address = 0x68 << 1;

	enum Ds1307Registers {
		kDs1307Seconds = 0x00,
		kDs1307Minutes,
		kDs1307Hours,
		kDs1307WeekDay,
		kDs1307Day,
		kDs1307Month,
		kDs1307Year,
		kDs1307Control
	};
	
	struct RtcDateTime {
		// Date part.
		uint8_t m_year;
		uint8_t m_month;
		uint8_t m_day;
		uint8_t m_weekDay;
		// Time part.
		bool m_is24hFormat;
		uint8_t m_hours;
		uint8_t m_minutes;
		uint8_t m_seconds;
	};

    class Ds1307 {
        private:
			void _init(void);

        public:
            Ds1307(void);
            ~Ds1307(void);
			
			bool getDateTime(RtcDateTime& dateTime);
			bool setDateTime(RtcDateTime& dateTime);
    };
}

#endif // __DRIVERS_DS1307_H