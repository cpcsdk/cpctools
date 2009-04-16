#pragma once
#include <map>
#include "CapriceUI.h"

class t_Input;

class CapriceInputSettingsImpl : public InputSettings
{
    public:
	CapriceInputSettingsImpl(wxWindow* WinID);
	~CapriceInputSettingsImpl();
	void applySettings();

    private:
	virtual void onKeyClick(wxCommandEvent& event);
	virtual void onKeyPress(wxKeyEvent& event);
	virtual void onSave(wxCommandEvent& event);
	virtual void applySettings(wxCommandEvent& event) { applySettings(); }
	
	void saveKeymap();
	wxString keyCodeToName(int keycode);

        t_Input& emulatorInputHandler;

	typedef struct{
	    int stdKeyCode;
	    int shiftKeyCode;
	    int ctrlKeyCode;
	} CPC_Key;

	typedef std::map<uint16_t,CPC_Key> CPC_Keymap;
	CPC_Keymap keymap;
	CPC_Keymap::iterator iter;
};
