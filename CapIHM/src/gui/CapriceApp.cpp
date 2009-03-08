/**
 *   $Id: CapriceApp.cpp 60 2008-11-25 17:08:52Z giot.romain $
 *	 CapriceReloded an Amstrad CPC emulator
 *   Copyright (C) 2008  cpcsdk crew
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */  


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

	wxSplashScreen* splash ;

// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool CapriceApp::OnInit()
{
    // call default behaviour (mandatory)
    if (!wxApp::OnInit())
        return false;

	wxImage::AddHandler(new wxPNGHandler);

	//Splash screen managment
	wxBitmap bitmap;
	if (bitmap.LoadFile( wxT( DATA_PATH "logo.png"), wxBITMAP_TYPE_PNG))
    {
		splash = new wxSplashScreen(bitmap,
		                wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
		                6000, NULL, -1, wxDefaultPosition, wxDefaultSize,
		                wxSIMPLE_BORDER|wxSTAY_ON_TOP);
    }
	wxYield();


	//Create emulator and IHM
	emulator = new Emulator();

	frame = new CapriceWindowImpl();
	frame->Show(true);


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
	

	//Set command line options
	if (greenscreen)	emulator->GetConfig().scr_tube = false ;
	if (intensity!=-1)	emulator->GetConfig().scr_intensity = intensity ;
	if (remanency)		emulator->GetConfig().scr_remanency = true ;
	
	if(emulator->Init()){
		frame->SetEmulator(emulator);

		//do the initialisations
		if (!drivea.IsEmpty()) emulator->GetFDC().insertA( (const char *) drivea.char_str());
		if (!driveb.IsEmpty()) emulator->GetFDC().insertB( (const char *) driveb.char_str());

		if (!snapshot.IsEmpty()) snapshot_load( *emulator,  snapshot.char_str()) ;

		if (!tape.IsEmpty()) emulator->GetTape().tape_insert(tape.char_str()) ; 
	
		if (fullscreen) emulator->GetRenderer().ToggleFullScreen();		

	}

	delete splash;

	SetTopWindow(frame);
    
	
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
	intensity = -1 ;
	greenscreen = false ;
	remanency = false ;

    fullscreen = parser.Found(wxT("f"));
	greenscreen = parser.Found(wxT("g"));
	remanency = parser.Found(wxT("r"));
	parser.Found(wxT("i"), &intensity);

	parser.Found(wxT("a"), &drivea);
	parser.Found(wxT("b"), &driveb);
	parser.Found(wxT("t"), &tape);
	parser.Found(wxT("s"), &snapshot);


    return true;
}
