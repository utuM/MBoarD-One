#ifndef __CORE_DEVICE_H
#define __CORE_DEVICE_H

namespace Core {
    class Device {
        private:
            static bool m_isInit;
			class Memory {
				private:
				
				public: 
					Memory(void) { }
					~Memory(void) { }
			};
			static Memory m_memory;
        
        public:
            Device(void) { }
			~Device(void) { }
			static void init(void);
    };
}

#endif // __CORE_DEVICE_H