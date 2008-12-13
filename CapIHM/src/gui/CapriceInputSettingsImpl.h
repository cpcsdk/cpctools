#pragma once
#include <map>
#include "CapriceUI.h"

class CapriceInputSettingsImpl : public InputSettings
{
    public:
	CapriceInputSettingsImpl(wxWindow* WinID);
	~CapriceInputSettingsImpl();

    private:
	virtual void onKeyClick(wxCommandEvent& event);
	virtual void onKeyPress(wxKeyEvent& event);
	virtual void onSave(wxCommandEvent& event);
	
	void saveKeymap();
	wxString keyCodeToName(int keycode);


	typedef struct{
	    int stdKeyCode;
	    int shiftKeyCode;
	    int ctrlKeyCode;
	} CPC_Key;

	typedef std::map<uint16_t,CPC_Key> CPC_Keymap;
	CPC_Keymap keymap;
	CPC_Keymap::iterator iter;
};
