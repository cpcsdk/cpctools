/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010-2011
 */

#include <Application.h>

#include "REmulator.h"
#include "RVideo.h"
#include "RAudio.h"

class ReloadedApp : public BApplication
{
	public:
		ReloadedApp();
		~ReloadedApp() {delete mEmu;}
		
		void RefsReceived(BMessage* message);
		void ArgvReceived(int32 argc, char** argv);

	private:
		REmulator* mEmu;
		BeAudioPlugin audio;
		RVidPlugin video;
};
