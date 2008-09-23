// For compilers that don't support precompilation, include "wx/wx.h"
 
#include "CapriceApp.h"
#include "CapriceWindowImpl.h"

#include <wx/splash.h> 
#include <zlib.h>
#include <SDL.h>

#include "cap32.h"
#include "config.h"
#include "debug.h"


#include "fdc.h"

#include "dsk.h"
#include "snapshot.h"
#include "tape.h"

#include "filetools.h"
#include "configBis.h"
#include <iostream>

//TODO destroy emulator when finishing

IMPLEMENT_APP(CapriceApp)
 
// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool CapriceApp::OnInit()
{
    // call default behaviour (mandatory)
    if (!wxApp::OnInit())
        return false;

	wxImage::AddHandler(new wxPNGHandler);

	//Splash screen managment
	wxBitmap bitmap;
	if (bitmap.LoadFile( wxT("logo.png"), wxBITMAP_TYPE_PNG))
	{
	 	wxSplashScreen* splash = new wxSplashScreen(bitmap,
		wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
		6000, NULL, -1, wxDefaultPosition, wxDefaultSize,
		wxSIMPLE_BORDER|wxSTAY_ON_TOP);
	}
	wxYield();


	//Create emulator and IHM
	emulator = new Emulator();

	frame = new CapriceWindowImpl();
	frame->Show(true);

	SetTopWindow(frame);

	return true ; 
}

/**
 * @TODO write the code
 */
int CapriceApp::OnExit()
{
	return 0;
}


int CapriceApp::OnRun() {
    // initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE) < 0){ 
        std::cerr << "unable to init SDL: " << SDL_GetError() << '\n';
        
        return -1;
    }
	
	if(emulator->Init()){
		frame->SetEmulator(emulator);

		//do the initialisations
		if (!drivea.IsEmpty()) emulator->GetFDC().insertA( (const char *) drivea.char_str());
		if (!driveb.IsEmpty()) emulator->GetFDC().insertB( (const char *) driveb.char_str());

		if (!snapshot.IsEmpty()) snapshot_load( *emulator,  snapshot.char_str()) ;

		if (!tape.IsEmpty()) emulator->GetTape().tape_insert(tape.char_str()) ; 
	
		if (fullscreen) emulator->GetRenderer().ToggleFullScreen();		

	}
    // generate an initial idle event to start things
    wxIdleEvent event;
    //event.SetEventObject(&frame->getPanel());
    //frame->getPanel().AddPendingEvent(event);

    // start the main loop
    return wxApp::OnRun();
}






void CapriceApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc (g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars (wxT("-"));
}
 
bool CapriceApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    fullscreen = parser.Found(wxT("f"));
	parser.Found(wxT("a"), &drivea);
	parser.Found(wxT("b"), &driveb);
	parser.Found(wxT("t"), &tape);
	parser.Found(wxT("s"), &snapshot);


    return true;
}
