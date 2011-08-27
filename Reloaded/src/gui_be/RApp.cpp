/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010
 */

#include "RApp.h"

#include "RVideo.h"
#include "RAudio.h"
#include "RWin.h"

#include <FindDirectory.h>
#include <Path.h>


int main(int argc, char* argv[])
{
	ReloadedApp app;
	app.Run();

	return 0;
}


ReloadedApp::ReloadedApp() : BApplication("application/x-vnd-shinra-reloaded")
{
	mEmu = REmulator::getInstance();
	mEmu->setVideoPlugin(&RVidPlugin::Create);
	mEmu->setAudioPlugin(new BeAudioPlugin());

	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.Append("Reloaded");
	mEmu->setCfgPath(path.Path());

	mEmu->Init();

}
