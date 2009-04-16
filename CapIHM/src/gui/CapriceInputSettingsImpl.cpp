#include <iostream>
#include <sstream>
#include <fstream>

#include "CapriceInputSettingsImpl.h"
#include "CapriceWindowImpl.h"
#include "input.h"
#include "CPCKeyDefs.h"

CapriceInputSettingsImpl::CapriceInputSettingsImpl(wxWindow* WinID):
    InputSettings(WinID),
    emulatorInputHandler(static_cast<CapriceWindowImpl*>(WinID)->GetEmulator()->GetInput())
{
    std::ifstream file;
    CPC_Key tmpk;
    uint16_t kid;
    std::string line;
    std::istringstream iss;

    file.open("Keymap.cfg");

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

void CapriceInputSettingsImpl::saveKeymap()
{
    std::cout << "SAVING THE KEYMAP\n";
    std::ofstream file;
    file.open("Keymap.cfg");

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
