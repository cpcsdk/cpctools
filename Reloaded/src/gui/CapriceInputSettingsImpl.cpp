#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>

#include "CapriceInputSettingsImpl.h"
#include "CapriceWindowImpl.h"
#include "core/input.h"
#include "core/CPCKeyDefs.h"
#include "core/crtc1.h"
#include "core/filetools.h"

#include "core/log.h"

#include <wx/event.h>
#include <wx/filename.h>
#include <wx/dir.h>

using std::make_shared;

/******************************************************************************
* COMMON BASE 
******************************************************************************/

CapriceInputSettingsImpl::CapriceInputSettingsImpl(wxWindow* WinID):
	Settings(WinID),
	emulator(*(static_cast<CapriceWindowImpl*>(WinID)->GetEmulator())),
	lastClickedButton(NULL),
	old_cfg(*emulator.GetConfig())
{
	// input
	std::ifstream file;
	CPC_Key tmpk;
	//uint16_t kid;
	unsigned short kid;
	std::string line;
	std::istringstream iss;

	file.open(getKeymapFileNameLoad());

	while (getline(file, line))
	{
		iss.str(line);
		iss >> kid;
		iss >> tmpk.stdKeyCode;
		iss >> tmpk.shiftKeyCode;
		iss >> tmpk.ctrlKeyCode;

		iss.clear();

		keymap.insert(std::pair<int, CPC_Key>(kid, tmpk));
	}

	// video
	choice_colorPalette->SetSelection((int)(emulator.GetConfig()->scr_tube));

	// roms
	t_CPC _config = *emulator.GetConfig();

	// TODO - the code bellow takes a lot of time to browse the disk. Can we improve it ?
	wxArrayString sysromlist;
	wxArrayString uromlist;
	wxArrayString realromlist;
	char thepath[1024];
	emulator.getConfigPath(thepath);

	wxString userpath(wxString(thepath, wxConvUTF8));
	userpath.append(wxT("/roms/"));
	wxDir::GetAllFiles(userpath,&uromlist);
	if (uromlist.GetCount() < 1)
	{
		ErrorLogMessage("There is no user ROM !!");
		ErrorLogMessage("Search path = %s", userpath.c_str());
 	}
	else
	{
		for (unsigned int i=0;i<uromlist.GetCount(); i++) {
			realromlist.Add(wxFileName(uromlist[i]).GetFullName());
		}
	}

	realromlist.Add(wxEmptyString); // Acts both as separator between user and sys and the "no rom" item
	wxDir::GetAllFiles(wxString(_config.rom_path, wxConvUTF8),&sysromlist);
	if (sysromlist.GetCount() <1)
	{
		ErrorLogMessage("There is no system ROM !!");
		ErrorLogMessage("Search path = %s", userpath.c_str());
	}
	else
	{
		for (unsigned int i=0;i<sysromlist.GetCount(); i++) {
			realromlist.Add(wxFileName(sysromlist[i]).GetFullName());
		}
	}

	ROM0file->Append(realromlist);
	ROM1file->Append(realromlist);
	ROM2file->Append(realromlist);
	ROM3file->Append(realromlist);
	ROM4file->Append(realromlist);
	ROM5file->Append(realromlist);
	ROM6file->Append(realromlist);
	ROM7file->Append(realromlist);

	ROM0file->SetStringSelection(wxString(emulator.GetConfig()->rom_file[0],wxConvUTF8));
	ROM1file->SetStringSelection(wxString(emulator.GetConfig()->rom_file[1],wxConvUTF8));
	ROM2file->SetStringSelection(wxString(emulator.GetConfig()->rom_file[2],wxConvUTF8));
	ROM3file->SetStringSelection(wxString(emulator.GetConfig()->rom_file[3],wxConvUTF8));
	ROM4file->SetStringSelection(wxString(emulator.GetConfig()->rom_file[4],wxConvUTF8));
	ROM5file->SetStringSelection(wxString(emulator.GetConfig()->rom_file[5],wxConvUTF8));
	ROM6file->SetStringSelection(wxString(emulator.GetConfig()->rom_file[6],wxConvUTF8));
	ROM7file->SetStringSelection(wxString(emulator.GetConfig()->rom_file[7],wxConvUTF8));

	manufacturerName->SetSelection((~emulator.GetConfig()->jumpers>>1) & 7);
	if (emulator.GetConfig()->jumpers & 0x10)
		radio50->SetValue(true);
	else
		radio60->SetValue(true);

	switch (emulator.GetConfig()->ram_size) {
		case 576:
			RAMSize576->SetValue(true);
			break;
		case 128:
		default:
			RAMSize128->SetValue(true);
			break;
		case 64:
			RAMSize64->SetValue(true);
			break;
	}

	choice_colorPalette->SetSelection(emulator.GetConfig()->scr_tube);
	spin_CRTC->SetValue(emulator.GetConfig()->crtc);
}

CapriceInputSettingsImpl::~CapriceInputSettingsImpl()
{
}

void CapriceInputSettingsImpl::onSave(wxCommandEvent& event)
{
	saveKeymap();
	emulator.GetConfig()->saveConfiguration();
}


void CapriceInputSettingsImpl::restoreSettings(wxCommandEvent& event)
{
    emulator.SetConfig(old_cfg);
}


void CapriceInputSettingsImpl::applySettings()
{
	for (CPC_Keymap::iterator iter = keymap.begin();iter!=keymap.end();iter++)
	{
		emulator.GetInput()->setupKey(iter->first, iter->second.stdKeyCode,
			iter->second.shiftKeyCode, iter->second.ctrlKeyCode);
	}
}

/******************************************************************************
* INPUT 
******************************************************************************/

const char * CapriceInputSettingsImpl::getKeymapFileNameLoad()
{
	static char str[1024];
	emulator.getConfigPath(str);
	strcat(str,"/Keymap.cfg");
	return str;
}

const char * CapriceInputSettingsImpl::getKeymapFileNameSave()
{
	static char str[1024];
	emulator.getConfigPath(str);
	strcat(str,"/Keymap.cfg");
	return str;
}


void CapriceInputSettingsImpl::onKeyClick(wxCommandEvent& event)
{
	if (lastClickedButton != NULL)
		lastClickedButton->SetBackgroundColour(
			wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	lastClickedButton = dynamic_cast<wxButton*>(event.GetEventObject());

	m_keyName->SetLabel(lastClickedButton->GetLabel());
	lastClickedButton->SetBackgroundColour(
		wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));

	iter = keymap.find(event.GetId());
	if ( iter != keymap.end() )
	{
		m_regularKey->SetValue(keyCodeToName(iter->second.stdKeyCode));
	}
	else
	{
		WarningLogMessage("Warning : key not found in keymap ! Adding it ...");
		CPC_Key tmpk;

		tmpk.stdKeyCode = 0;
		tmpk.shiftKeyCode = 0;
		tmpk.ctrlKeyCode = 0;

		keymap.insert(std::pair<int, CPC_Key>(event.GetId(),tmpk));
		iter = keymap.find(event.GetId()); // iter pointe sur l'élément cliqué,
		// on va pouvoir le modifier dans key_pressed
	}
}

void CapriceInputSettingsImpl::onKeyPress(wxKeyEvent& event)
{
	m_regularKey->SetValue(keyCodeToName(event.GetKeyCode()));
	iter->second.stdKeyCode = event.GetKeyCode();
}


void CapriceInputSettingsImpl::saveKeymap()
{
	std::ofstream file;
	file.open(getKeymapFileNameSave());

	//TODO add check to see if file is really open !

	for (CPC_Keymap::iterator iter = keymap.begin();iter!=keymap.end();iter++)
	{
		file << iter->first << " " << iter->second.stdKeyCode << " "
			<< iter->second.shiftKeyCode << " " << iter->second.ctrlKeyCode
			<< std::endl;
	}

	file.close();
}

wxString CapriceInputSettingsImpl::keyCodeToName(int keycode)
{
	std::cout << keycode;
	switch(keycode)
	{
		case WXK_BACK:
			return _T("BACKSPACE");
		case WXK_TAB:
			return _T("TAB");
		case WXK_SPACE:
			return _T("SPACE");
		case WXK_DELETE:
			return _T("DELETE");
		case WXK_RETURN:
			return _T("RETURN");
		case WXK_ESCAPE:
			return _T("ESCAPE");
		case WXK_SHIFT:
			return _T("SHIFT");
		case WXK_ALT:
			return _T("ALT");
		case WXK_CONTROL:
			return _T("CONTROL");
		case WXK_MENU:
			return _T("MENU");
		case WXK_PAUSE:
			return _T("PAUSE");
		case WXK_CAPITAL:
			return _T("CAPS");
		case WXK_HOME:
			return _T("HOME");
		case WXK_END:
			return _T("END");
		case WXK_LEFT:
			return _T("LEFT");
		case WXK_UP:
			return _T("UP");
		case WXK_RIGHT:
			return _T("RIGHT");
		case WXK_DOWN:
			return _T("DOWN");
		case WXK_INSERT:
			return _T("INSERT");
		case WXK_NUMPAD0:
			return _T("Numpad 0");
		case WXK_NUMPAD1:
			return _T("Numpad 1");
		case WXK_NUMPAD2:
			return _T("Numpad 2");
		case WXK_NUMPAD3:
			return _T("Numpad 3");
		case WXK_NUMPAD4:
			return _T("Numpad 4");
		case WXK_NUMPAD5:
			return _T("Numpad 5");
		case WXK_NUMPAD6:
			return _T("Numpad 6");
		case WXK_NUMPAD7:
			return _T("Numpad 7");
		case WXK_NUMPAD8:
			return _T("Numpad 8");
		case WXK_NUMPAD9:
			return _T("Numpad 9");
		case WXK_SCROLL:
			return _T("Scroll Lock");
		case WXK_PAGEUP:
			return _T("Page Up");
		case WXK_PAGEDOWN:
			return _T("Page Down");
		default:
			return wxChar(keycode);
	}
}

/******************************************************************************
* VIDEO 
******************************************************************************/

void CapriceInputSettingsImpl::changeCRTCType( wxSpinEvent& event )
{
	shared_ptr<t_CRTC> newCRTC;

	switch(event.GetPosition()) {
		case 0:
		default:
			newCRTC = make_shared<t_CRTC>(emulator.GetGateArray(), emulator.GetVDU());
			break;
		case 1:
			newCRTC = make_shared<t_CRTC1>(emulator.GetGateArray(), emulator.GetVDU());
			break;
	}
	emulator.setCRTC(newCRTC);
}

void CapriceInputSettingsImpl::changeColorPalette( wxCommandEvent& event )
{
    auto _config = emulator.GetConfig();
	_config->scr_tube = (Renderer::MonitorMode)(event.GetInt());
	emulator.GetRenderer().SetMonitor(_config->scr_tube, _config->scr_intensity, _config->scr_remanency);
	emulator.GetRenderer().InitPalette();
}

/***************
 * ROMS
 ***************/
void CapriceInputSettingsImpl::RomChanged( wxCommandEvent& event )
{
	wxString fn;
	fn = ROM0file->GetStringSelection();
	strcpy(emulator.GetConfig()->rom_file[0],fn.mb_str());
	fn = ROM1file->GetStringSelection();
	strcpy(emulator.GetConfig()->rom_file[1],fn.mb_str());
	fn = ROM2file->GetStringSelection();
	strcpy(emulator.GetConfig()->rom_file[2],fn.mb_str());
	fn = ROM3file->GetStringSelection();
	strcpy(emulator.GetConfig()->rom_file[3],fn.mb_str());
	fn = ROM4file->GetStringSelection();
	strcpy(emulator.GetConfig()->rom_file[4],fn.mb_str());
	fn = ROM5file->GetStringSelection();
	strcpy(emulator.GetConfig()->rom_file[5],fn.mb_str());
	fn = ROM6file->GetStringSelection();
	strcpy(emulator.GetConfig()->rom_file[6],fn.mb_str());
	fn = ROM7file->GetStringSelection();
	strcpy(emulator.GetConfig()->rom_file[7],fn.mb_str());
}


void CapriceInputSettingsImpl::SelectManufacturer(wxCommandEvent& event)
{
	emulator.GetConfig()->jumpers = (emulator.GetConfig()->jumpers & 0xF1) 
		| (~event.GetSelection() <<1);
}


void CapriceInputSettingsImpl::Select50HZ(wxCommandEvent& event)
{
	emulator.GetConfig()->jumpers |= 0x10;
}


void CapriceInputSettingsImpl::Select60HZ(wxCommandEvent& event)
{
	emulator.GetConfig()->jumpers &= 0xEF;
}


void CapriceInputSettingsImpl::LoadPreset( wxCommandEvent& event ) {
	switch(event.GetSelection()) {
		case 0: // 464
			emulator.GetConfig()->ram_size = 64; 
			emulator.GetConfig()->rom_file[7][0] = '\0';
			RAMSize64->SetValue(true);
			break;

		case 1: // 664
			emulator.GetConfig()->ram_size = 64; 
			strcpy(emulator.GetConfig()->rom_file[7],"amsdos.rom");
			RAMSize64->SetValue(true);
			break;

		case 2: // 6128
		default:
			emulator.GetConfig()->ram_size = 128; 
			strcpy(emulator.GetConfig()->rom_file[7],"amsdos.rom");
			RAMSize128->SetValue(true);
			break;
	}

	emulator.GetConfig()->model = event.GetSelection();

	ROM7file->SetStringSelection(wxString(emulator.GetConfig()->rom_file[7],wxConvUTF8));
}


void CapriceInputSettingsImpl::Select64K( wxCommandEvent& event ) {
	emulator.GetConfig()->ram_size = 64; 
}


void CapriceInputSettingsImpl::Select128K( wxCommandEvent& event ) {
	emulator.GetConfig()->ram_size = 128; 
}


void CapriceInputSettingsImpl::Select576K( wxCommandEvent& event ) {
	emulator.GetConfig()->ram_size = 576; 
}



