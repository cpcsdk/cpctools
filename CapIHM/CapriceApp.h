#ifndef INCLUDED_CAPRICEAPP_H
#define INCLUDED_CAPRICEAPP_H

#include <wx/wxprec.h>
#include <wx/cmdline.h>
#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif
 

#include "CapriceWindowImpl.h"
#include "CapriceRegistersDialogImpl.h"
#include "emulator.h"


class CapriceApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int  OnRun();
	virtual int  OnExit();
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	CapriceWindowImpl 	*frame ;

private:
	Emulator			*emulator ;

	//params
	bool		fullscreen ;
	wxString	drivea ;
	wxString	driveb ;
	wxString	tape ;
	wxString 	snapshot ;	

};
 

static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
     { wxCMD_LINE_SWITCH, wxT("h"), wxT("help"), wxT("displays help on the command line parameters"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
     { wxCMD_LINE_SWITCH, wxT("f"), wxT("fullscreen"), wxT("display Caprice in full screen") },
     { wxCMD_LINE_OPTION, wxT("a"), wxT("drivea"), wxT("specify disk image to insert into drive A"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
 
     { wxCMD_LINE_OPTION, wxT("b"), wxT("driveb"), wxT("specify disk image to insert into drive B"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
     
     { wxCMD_LINE_OPTION, wxT("t"), wxT("tape"), wxT("specify tape image "), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },
     { wxCMD_LINE_OPTION, wxT("s"), wxT("snapshot"), wxT("specify snapshot image "), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL  },

	{ wxCMD_LINE_NONE }
};
 


DECLARE_APP(CapriceApp)
 
#endif // INCLUDED_CAPRICEAPP_H 
