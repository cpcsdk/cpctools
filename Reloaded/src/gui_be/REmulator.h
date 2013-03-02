/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010
 */

#ifndef __REMU_H__
#define __REMU_H__

#include <memory>

#include "core/emulator.h"
#include "RVideo.h"

#include "core/CPCKeyDefs.h"

#include <InterfaceDefs.h>

using std::shared_ptr;

class REmulator: public Emulator {
	private:
		REmulator(shared_ptr<VideoPlugin> video, shared_ptr<AudioPlugin> audio)
			: Emulator(video, audio)
		{}

	public:
		~REmulator() {};

		static inline REmulator* getInstance()
		{
			// Assumes createInstance was called before
			return (REmulator*)instance;
		}

		static inline void createInstance(shared_ptr<VideoPlugin> video, shared_ptr<AudioPlugin> audio)
		{
			instance = new REmulator(video, audio);
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

			shared_ptr<RVidPlugin> video = std::static_pointer_cast<RVidPlugin>(
				GetRenderer().GetVideoPlugin());
			video->fWindow->PostMessage(&message);
		}
};

#endif
