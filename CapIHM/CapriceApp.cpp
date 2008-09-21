// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"
 
#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif
 
#include "CapriceApp.h"
#include "CapriceWindowImpl.h"
 
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
	wxImage::AddHandler(new wxPNGHandler);
	emulator = new Emulator();


	frame = new CapriceWindowImpl();
	frame->Show(true);

	SetTopWindow(frame);



	return true ; 
}


int CapriceApp::OnRun() {
    // initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE) < 0){ 
        std::cerr << "unable to init SDL: " << SDL_GetError() << '\n';
        
        return -1;
    }
	
	if(emulator->Init()){
		frame->SetEmulator(emulator);
	}
    // generate an initial idle event to start things
    wxIdleEvent event;
    //event.SetEventObject(&frame->getPanel());
    //frame->getPanel().AddPendingEvent(event);

    // start the main loop
    return wxApp::OnRun();
}
