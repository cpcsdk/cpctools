#include <iostream>
#include <fstream>

#include "CapriceInputSettingsImpl.h"
#include "input.h"

CapriceInputSettingsImpl::CapriceInputSettingsImpl(wxWindow* WinID):
    InputSettings(WinID)
{
    // TODO : Init the keymap from the config file and a static table of wxID > CPC_id

    CPC_Key tmpk;

    tmpk.CPC_KeyId = CPC_ESC;
    tmpk.stdKeyCode = WXK_ESCAPE;
    tmpk.shiftKeyCode = WXK_ESCAPE;
    tmpk.ctrlKeyCode = WXK_ESCAPE;
    keymap.insert(std::pair<int, CPC_Key>(wxID_ESC, tmpk));

    tmpk.CPC_KeyId = CPC_1;
    tmpk.stdKeyCode = '1';
    tmpk.shiftKeyCode = '1';
    tmpk.ctrlKeyCode = '1';
    keymap.insert(std::pair<int, CPC_Key>(wxID_1, tmpk));
}

CapriceInputSettingsImpl::~CapriceInputSettingsImpl()
{
    saveKeymap();
}

void CapriceInputSettingsImpl::onKeyClick(wxCommandEvent& event)
{
    m_keyName->SetLabel(dynamic_cast<wxButton*>(event.GetEventObject())->GetLabel());

    CPC_Keymap::iterator iter = keymap.find(event.GetId());
    if( iter != keymap.end() )
    {
	m_regularKey->SetValue(keyCodeToName(iter->second.stdKeyCode));
    }
    else
    {
	std::cout << "Warning : key not found in keymap ! Adding it ..." << std::endl;
	CPC_Key tmpk;

	tmpk.CPC_KeyId = 9999 ; // Valeur bidon, pour bien faire comprendre a l'utilisateur que ça peut pas marcher
	tmpk.stdKeyCode = 0;
	tmpk.shiftKeyCode = 0;
	tmpk.ctrlKeyCode = 0;

	keymap.insert(std::pair<int,CPC_Key>(event.GetId(),tmpk));

    }
}

void CapriceInputSettingsImpl::onSave(wxCommandEvent& event)
{
    saveKeymap();
}

void CapriceInputSettingsImpl::saveKeymap()
{
    std::cout << "SAVING THE KEYMAP\n";
    std::ofstream file;
    file.open("Keymap.cfg");

    for(CPC_Keymap::iterator iter=keymap.begin();iter!=keymap.end();iter++)
    {
	file << iter->first << "," << iter->second.CPC_KeyId << "," << iter->second.stdKeyCode << "\n";
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
