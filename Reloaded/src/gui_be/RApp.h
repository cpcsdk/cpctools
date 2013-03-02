/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010-2011
 */

#include <memory>

#include <Application.h>

#include "REmulator.h"

class BeAudioPlugin;
class RVidPlugin;

class ReloadedApp : public BApplication
{
	public:
		ReloadedApp();
		~ReloadedApp() {delete mEmu;}
		
		void RefsReceived(BMessage* message);
		void ArgvReceived(int32 argc, char** argv);

	private:
		REmulator* mEmu;
		shared_ptr<BeAudioPlugin> audio;
		shared_ptr<RVidPlugin> video;
};
