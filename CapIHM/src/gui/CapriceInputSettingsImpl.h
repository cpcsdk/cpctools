#pragma once
#include <map>
#include "CapriceUI.h"

class CapriceInputSettingsImpl : public InputSettings
{
    public:

    CapriceInputSettingsImpl(wxWindow* WinID);

    private:
	virtual void onKeyClick(wxCommandEvent& event);

	wxString keyCodeToName(int keycode);

	typedef struct{
	    uint16_t CPC_KeyId;
	    wxString KeyName;
	    int stdKeyCode;
	    int shiftKeyCode;
	    int ctrlKeyCode;
	} CPC_Key;

	typedef std::map<int,CPC_Key> CPC_Keymap;
	CPC_Keymap keymap;

};
