/**
 *   $Id: CapriceApp.cpp 60 2008-11-25 17:08:52Z giot.romain $
 *	 Reloded an Amstrad CPC emulator
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
#include "CapriceInputSettingsImpl.h"
#include <pthread.h>

#include <wx/splash.h> 

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
 


#if CLI
extern "C" {
#include "cparser.h"
#include "cparser_priv.h"
#include "cparser_token.h"
#include "cparser_tree.h"
  }

  cparser_t parser;
  extern cparser_node_t cparser_root;
  wxFrame *frameClone;
  Emulator *emulatorClone;
#endif

#if WITH_ASM
#include "../asm/CapASM.h"
CapASM *capAsm ;
#endif

#include "WXVideo.cpp"


#ifdef __WXMAC__ 
#include <ApplicationServices/ApplicationServices.h>
#endif

//TODO destroy emulator when finishing

IMPLEMENT_APP(CapriceApp)

	wxSplashScreen* splash ;

// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool CapriceApp::OnInit()
{
    // call default behaviour (mandatory)
    if (!wxApp::OnInit())
        return false;

    SetAppName(wxT("reloaded"));
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
	emulator = WXEmulator::getInstance();
	emulator->setVideoPlugin(&WXDoubleLinePlugin::Create);
	Emulator::getInstance();
	
	#if WITH_ASM
	capAsm = new CapASM(emulator);
	#endif

	printf("Launch window\n");
	frame = new CapriceWindowImpl(emulator);
	frame->Show(true);
	frame->Raise();

  #if CLI
    frameClone = frame ;
  #endif
	printf("Initialization done\n");
	return true ; 
}

/**
 * @TODO write the code
 */
int CapriceApp::OnExit()
{
	delete emulator;
	emulator = NULL;
	//delete frame;
  	cout << "Quit" << endl ;
#if CLI
  	if (cli)
  	{
    	cout << "Quit parser" << endl ;
    	cparser_quit(&parser) ;
  	}
#endif
	return 0;
}


int CapriceApp::OnRun()
{
#ifdef __WXMAC__ 
	ProcessSerialNumber PSN;
	GetCurrentProcess(&PSN);
	TransformProcessType(&PSN,kProcessTransformToForegroundApplication);
#endif

    //Set command line options
    if (greenscreen) emulator->GetConfig().scr_tube = Renderer::GreenMode;
    if (intensity!=-1) emulator->GetConfig().scr_intensity = intensity;
    if (remanency) emulator->GetConfig().scr_remanency = true;

    if(emulator->Init())
    {
        #ifdef USE_DEBUGGER
        InitDebug();
        #endif

        //do the initialisations
        if (!drivea.IsEmpty()) emulator->GetFDC().insertA( (const char *) drivea.mb_str());
        if (!driveb.IsEmpty()) emulator->GetFDC().insertB( (const char *) driveb.mb_str());

        if (!snapshot.IsEmpty()) snapshot_load( *emulator,  (const char *)snapshot.c_str()) ;

        if (!tape.IsEmpty()) emulator->GetTape().tape_insert((const char *)tape.c_str()) ; 

        if (fullscreen) emulator->GetRenderer().ToggleFullScreen();		

		// Initializing the keymap is done in the input settings window...
		CapriceInputSettingsImpl* InputSettingsWindow = new CapriceInputSettingsImpl(frame);
		InputSettingsWindow -> applySettings();
		delete InputSettingsWindow;
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

#if CLI
 /**
 * CLI in a thread
 * @todo use locks and co
 */
void* cliRout(void* args)
{ 

    parser.cfg.root = &cparser_root;
    parser.cfg.ch_complete = '\t' ;
    parser.cfg.ch_erase ='\b' ;
    parser.cfg.ch_del = 127 ;
    parser.cfg.ch_help = '?' ;
    parser.cfg.flags =  0 ;
    strcpy(parser.cfg.prompt, "caprice>> ");
    //parser.cfg.fd = STDOUT_FILENO ;
    cparser_io_config(&parser);

    //Load cli interface
    if (CPARSER_OK != cparser_init(&parser.cfg, &parser))
    {
      std::cout << "Fail to initialize parser." << endl;
      return false;
    }
    cparser_run(&parser) ;

    //TODO quit application
    frameClone->Close();

	return args;
}
#endif


bool CapriceApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	intensity = -1 ;
	greenscreen = false ;
	remanency = false ;
  cli = false;

    fullscreen = parser.Found(wxT("f"));
	greenscreen = parser.Found(wxT("g"));
	remanency = parser.Found(wxT("r"));
	parser.Found(wxT("i"), &intensity);

	parser.Found(wxT("a"), &drivea);
	parser.Found(wxT("b"), &driveb);
	parser.Found(wxT("t"), &tape);
	parser.Found(wxT("s"), &snapshot);

#if CLI
  if (parser.Found(wxT("c")))
  {
    cli = true ;
    pthread_t threadcli;
    if (pthread_create (&threadcli, NULL, cliRout, NULL) < 0)
    {
      std::cout << "Fail to create cli thread." << endl ;
      return false ;
    }
  }

#endif

    return true;
}


