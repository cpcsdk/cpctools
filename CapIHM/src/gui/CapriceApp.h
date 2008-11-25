/**
 *   $Id: CapriceApp.h 60 2008-11-25 17:08:52Z giot.romain $
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
