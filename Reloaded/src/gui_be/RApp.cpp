/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010
 */

#include "RApp.h"

#include "RVideo.h"
#include "RAudio.h"
#include "RWin.h"


int main(int argc, char* argv[])
{
	ReloadedApp app;
	app.Run();

	return 0;
}


ReloadedApp::ReloadedApp() : BApplication("application/x-vnd-shinra-reloaded")
{
	mWin = new ReloadedWin();

	mWin->Show();
	
	mEmu = REmulator::getInstance();
	RVidPlugin::sView = mWin;
	mEmu->setVideoPlugin(&RVidPlugin::Create);
	mEmu->setAudioPlugin(new BeAudioPlugin());

	mEmu->setCfgPath("/Donnees/Dev/cpcsdk/reloaded");

	mEmu->Init();

}
