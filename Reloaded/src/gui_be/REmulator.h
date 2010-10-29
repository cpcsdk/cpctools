/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010
 */

#include "emulator.h"

class REmulator: public Emulator {
	private:
		void PressKey(uint32_t, uint32_t) {};
		void ReleaseKey(uint32_t, uint32_t) {};
		static bool sInitOnce;

	public:
		~REmulator() {};

		static inline REmulator* getInstance()
		{
			if(sInitOnce == false) {
				sInitOnce = true;
				instance = new REmulator();
			}
			return (REmulator*)instance;
		}

		void setCfgPath(const char* p) {strcpy(_config_path, p);}
		void waitForEnd() {pthread_join(emuthread, NULL); }

};

