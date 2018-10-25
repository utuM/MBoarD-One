#ifndef __DRIVERS_DS1307_H
#define __DRIVERS_DS1307_H

namespace Driver {	
	const uint8_t s_kDs1307Address = 0x68 << 1;

	struct RtcDateTime {
		// Date part.
		uint16_t m_year;
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