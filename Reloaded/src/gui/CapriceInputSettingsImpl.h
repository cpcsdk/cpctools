#pragma once
#include <map>
#include "CapriceUI.h"
#include <wx/stdpaths.h>
#include <wx/string.h>

#include "core/config_manager.h"

class Emulator;

class CapriceInputSettingsImpl : public Settings
{
	public:
		CapriceInputSettingsImpl(wxWindow* WinID);
		~CapriceInputSettingsImpl();
		void applySettings();

	private:
		void onKeyClick(wxCommandEvent& event);
		void onKeyPress(wxKeyEvent& event);
		void onSave(wxCommandEvent& event);
		void restoreSettings(wxCommandEvent& event);
		void applySettings(wxCommandEvent& event) { applySettings(); }
		void changeCRTCType( wxSpinEvent& event );
		void changeColorPalette( wxCommandEvent& event );
		void RomChanged( wxCommandEvent& event );
		void SelectManufacturer( wxCommandEvent& event );
		void Select50HZ( wxCommandEvent& event );
		void Select60HZ( wxCommandEvent& event );
		void LoadPreset( wxCommandEvent& event );
		void Select64K( wxCommandEvent& event );
		void Select128K( wxCommandEvent& event );
		void Select576K( wxCommandEvent& event );

		void saveKeymap();

		wxString keyCodeToName(int keycode);

		/**
		 * Returns the name of the keyboard configuration file
		 * @TODO Find the real file to use between share directory or user
		 */
		const char * getKeymapFileNameLoad();

		/**
		 * Returns the name of the keyboard configuration file
		 * @TODO Find the real file to use between directory or user
		 */
		const char * getKeymapFileNameSave();


		Emulator& emulator;

		typedef struct{
			int stdKeyCode;
			int shiftKeyCode;
			int ctrlKeyCode;
		} CPC_Key;

		//typedef std::map<uint16_t, CPC_Key> CPC_Keymap;
		typedef std::map<unsigned short, CPC_Key> CPC_Keymap;
		CPC_Keymap keymap;
		CPC_Keymap::iterator iter;

		wxButton* lastClickedButton;

		t_CPC old_cfg;
};
