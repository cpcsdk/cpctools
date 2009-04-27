#include <iostream>
#include <sstream>
#include <fstream>

#include "CapriceInputSettingsImpl.h"
#include "CapriceWindowImpl.h"
#include "input.h"
#include "CPCKeyDefs.h"

#include <wx/filename.h>
#include <wx/dir.h>

CapriceInputSettingsImpl::CapriceInputSettingsImpl(wxWindow* WinID):
    InputSettings(WinID),
    emulatorInputHandler(static_cast<CapriceWindowImpl*>(WinID)->GetEmulator()->GetInput())
{
    std::ifstream file;
    CPC_Key tmpk;
    uint16_t kid;
    std::string line;
    std::istringstream iss;

    file.open(getKeymapFileNameLoad());

    while(getline(file,line))
    {
	iss.str(line);
	iss >> kid;
	iss >> tmpk.stdKeyCode;
	iss >> tmpk.shiftKeyCode;
	iss >> tmpk.ctrlKeyCode;

	iss.clear();
	
	keymap.insert(std::pair<int, CPC_Key>(kid, tmpk));
    }

}

const char * CapriceInputSettingsImpl::getKeymapFileNameLoad()
{

  return DATA_PATH "Keymap.cfg" ;
}

const char * CapriceInputSettingsImpl::getKeymapFileNameSave()
{
  const char * a = ( const char * ) wxStandardPaths::Get().GetDocumentsDir().c_str() ;
  const char * b = ( const char * ) wxStandardPaths::Get().GetUserConfigDir().c_str() ;
  const char * c = ( const char * ) wxStandardPaths::Get().GetExecutablePath().c_str() ;
  const char * d = ( const char * ) wxStandardPaths::Get().GetConfigDir().c_str() ;

    wxStandardPathsBase &stdPth = wxStandardPaths::Get();
    wxString sPath = stdPth.GetExecutablePath();

//  std::cout << a << endl
//            << b << endl
//            << c << endl
//            << d << endl
//            << (wxChar *)sPath.c_str() << endl ;

  std::cout << wxDir::Exists(wxStandardPaths::Get().GetUserDataDir()) << endl ;

  wxFileName name = wxFileName(wxStandardPaths::Get().GetExecutablePath());
  //name.RemoveLastDir();

  cout << name.DirExists() << endl ;

  name.SetName(wxT( "Keymap.cfg"));
  cout << name.FileExists() << endl ;

  return DATA_PATH "Keymap.cfg" ;
}



CapriceInputSettingsImpl::~CapriceInputSettingsImpl()
{
    saveKeymap();
}

void CapriceInputSettingsImpl::onKeyClick(wxCommandEvent& event)
{
    m_keyName->SetLabel(dynamic_cast<wxButton*>(event.GetEventObject())->GetLabel());

    iter = keymap.find(event.GetId());
    if( iter != keymap.end() )
    {
	m_regularKey->SetValue(keyCodeToName(iter->second.stdKeyCode));
    }
    else
    {
	std::cout << "Warning : key not found in keymap ! Adding it ..." << std::endl;
	CPC_Key tmpk;

	tmpk.stdKeyCode = 0;
	tmpk.shiftKeyCode = 0;
	tmpk.ctrlKeyCode = 0;

	keymap.insert(std::pair<int,CPC_Key>(event.GetId(),tmpk));
	iter = keymap.find(event.GetId()); // iter pointe sur l'élément cliqué, on va pouvoir le modifier 
					   // dans key_pressed
    }
}

void CapriceInputSettingsImpl::onKeyPress(wxKeyEvent& event)
{
    m_regularKey->SetValue(event.GetUnicodeKey());
    iter->second.stdKeyCode = event.GetKeyCode();
}

void CapriceInputSettingsImpl::onSave(wxCommandEvent& event)
{
    saveKeymap();
}

void CapriceInputSettingsImpl::applySettings()
{
    for(CPC_Keymap::iterator iter=keymap.begin();iter!=keymap.end();iter++)
    {
	 emulatorInputHandler.setupKey(iter->first,iter->second.stdKeyCode,iter->second.shiftKeyCode,iter->second.ctrlKeyCode);
    }
}

/**
 * @TODO save the keymap at the right place (not in share, but user home)
 */
void CapriceInputSettingsImpl::saveKeymap()
{
    std::cout << "SAVING THE KEYMAP\n";
    std::ofstream file;
    file.open( getKeymapFileNameSave());

    for(CPC_Keymap::iterator iter=keymap.begin();iter!=keymap.end();iter++)
    {
	file << iter->first << " " << iter->second.stdKeyCode << " " << iter->second.shiftKeyCode 
			    << " " << iter->second.ctrlKeyCode << std::endl;
    }

    file.close();
}

wxString CapriceInputSettingsImpl::keyCodeToName(int keycode)
{
    switch(keycode)
    {
	case WXK_ESCAPE:
	    return _T("ESCAPE");
	default:
	    return (wchar_t)keycode;
    }
}
