/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010
 */

#include "RApp.h"

#include "RWin.h"
#include "RVideo.h"
#include "RAudio.h"
#include "core/snapshot.h"
#include "misc/log.h"

#include <memory.h>

#include <FindDirectory.h>
#include <Path.h>
#include <Entry.h>


int main(void)
{
	new ReloadedApp();
	be_app->Run();
	delete be_app;
	return 0;
}


ReloadedApp::ReloadedApp() : BApplication("application/x-vnd-shinra-reloaded")
{
    auto video = std::make_shared<RVidPlugin>();
    auto audio = std::make_shared<BeAudioPlugin>();
	REmulator::createInstance(video, audio);
	mEmu = REmulator::getInstance();

	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.Append("Reloaded");
	mEmu->setCfgPath(path.Path());

	mEmu->Init();
}


void ReloadedApp::RefsReceived(BMessage* message)
{
	entry_ref theFile;
	
	if (message->FindRef("refs", 0, &theFile) != B_OK)
	{
		WarningLogMessage("[BeReloaded]Empty refs received message !");
		message->PrintToStream();
		return;
	}
	
	BEntry entry(&theFile);
	BPath path;
	entry.GetPath(&path);
	
	InfoLogMessage("[BeReloaded inserting disk in drive A: %s", path.Path());
	
	mEmu->GetFDC()->insertA(path.Path());
}


void ReloadedApp::ArgvReceived(int32 argc, char** argv)
{
	for (int i = 0; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
				case 'a':
					mEmu->GetFDC()->insertA(argv[++i]);
					break;
				case 'b':
					mEmu->GetFDC()->insertB(argv[++i]);
					break;
				case 's':
					snapshot_load(*mEmu, argv[++i]);
					break;
				/*
				case 'f': // Fullscreen
				case 'g': // Green screen
				case 'r': // Set remanency
				case 'i': // Set intensity
				case 't': // Tape
				*/
			}
		}
	}
}
