/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010
 */

#ifndef __REMU_H__
#define __REMU_H__

#include "emulator.h"
#include "RVideo.h"

#include "../core/CPCKeyDefs.h"

#include <InterfaceDefs.h>

class REmulator: public Emulator {
	private:
		static volatile bool sInitOnce;

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
		void PressKey(uint32_t key, uint32_t mod);
		void ReleaseKey(uint32_t key, uint32_t mod);

		bool Init();

		void fdcLed(bool on)
		{
			BMessage message;
			message.what='fled';
			message.AddBool("status", on);

			static_cast<RVidPlugin*>(GetRenderer().GetVideoPlugin())->fWindow->PostMessage(&message);
		}
};

#endif
