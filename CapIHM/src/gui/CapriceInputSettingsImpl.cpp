#include "CapriceInputSettingsImpl.h"
#include "input.h"

CapriceInputSettingsImpl::CapriceInputSettingsImpl(wxWindow* WinID):
    InputSettings(WinID)
{
    // TODO : Init the keymap from the config file and a static table of wxID > CPC_id

    CPC_Key tmpk;

    tmpk.CPC_KeyId = CPC_ESC;
    tmpk.KeyName = _T("ESC");
    tmpk.stdKeyCode = WXK_ESCAPE;
    tmpk.shiftKeyCode = WXK_ESCAPE;
    tmpk.ctrlKeyCode = WXK_ESCAPE;
    keymap.insert(std::pair<int, CPC_Key>(wxID_ESC, tmpk));

    tmpk.CPC_KeyId = CPC_1;
    tmpk.KeyName = _T("1&&");
    tmpk.stdKeyCode = '1';
    tmpk.shiftKeyCode = '1';
    tmpk.ctrlKeyCode = '1';
    keymap.insert(std::pair<int, CPC_Key>(wxID_1, tmpk));
}

void CapriceInputSettingsImpl::onKeyClick(wxCommandEvent& event)
{

    CPC_Keymap::iterator iter = keymap.find(event.GetId());
    if( iter != keymap.end() )
    {
    	m_keyName->SetLabel(iter->second.KeyName);
	m_regularKey->SetValue(keyCodeToName(iter->second.stdKeyCode));
    }
    else
	std::cout << "Warning : key not found in keymap !" << std::endl;
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
