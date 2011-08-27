/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010
 */

#include <Application.h>

#include "REmulator.h"

class ReloadedWin;

class ReloadedApp : public BApplication
{
	public:
		ReloadedApp();
		~ReloadedApp() {delete mEmu;}
		
		void RefsReceived(BMessage* message);

	private:
		REmulator* mEmu;
};
