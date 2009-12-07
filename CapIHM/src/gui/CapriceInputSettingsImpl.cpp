#include <iostream>
#include <sstream>
#include <fstream>

#include "CapriceInputSettingsImpl.h"
#include "CapriceWindowImpl.h"
#include "input.h"
#include "CPCKeyDefs.h"

#include <wx/filename.h>
#include <wx/dir.h>

/******************************************************************************
* COMMON BASE 
******************************************************************************/

CapriceInputSettingsImpl::CapriceInputSettingsImpl(wxWindow* WinID):
	InputSettings(WinID),
	emulator(*(static_cast<CapriceWindowImpl*>(WinID)->GetEmulator())),
	lastClickedButton(NULL)
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
	choice_colorPalette->SetSelection((int)(emulator.GetConfig().scr_tube));

	// roms
	wxString path(emulator.GetConfig().rom_path, wxConvUTF8);
	path += wxString("/",wxConvUTF8);
	cout << "rom0 " << path.char_str() << endl;
	ROM0file->SetPath(path+wxString(emulator.GetConfig().rom_file[0],wxConvUTF8));
	ROM1file->SetPath(path+wxString(emulator.GetConfig().rom_file[1],wxConvUTF8));
	ROM2file->SetPath(path+wxString(emulator.GetConfig().rom_file[2],wxConvUTF8));
	ROM3file->SetPath(path+wxString(emulator.GetConfig().rom_file[3],wxConvUTF8));
	ROM4file->SetPath(path+wxString(emulator.GetConfig().rom_file[4],wxConvUTF8));
	ROM5file->SetPath(path+wxString(emulator.GetConfig().rom_file[5],wxConvUTF8));
	ROM6file->SetPath(path+wxString(emulator.GetConfig().rom_file[6],wxConvUTF8));
	ROM7file->SetPath(path+wxString(emulator.GetConfig().rom_file[7],wxConvUTF8));
}

CapriceInputSettingsImpl::~CapriceInputSettingsImpl()
{
	saveKeymap();
}

void CapriceInputSettingsImpl::onSave(wxCommandEvent& event)
{
	saveKeymap();
}

void CapriceInputSettingsImpl::applySettings()
{
	for (CPC_Keymap::iterator iter = keymap.begin();iter!=keymap.end();iter++)
	{
		emulator.GetInput().setupKey(iter->first, iter->second.stdKeyCode,
			iter->second.shiftKeyCode, iter->second.ctrlKeyCode);
	}
}

/******************************************************************************
* INPUT 
******************************************************************************/

const char * CapriceInputSettingsImpl::getKeymapFileNameLoad()
{

	return DATA_PATH "Keymap.cfg" ;
}

const char * CapriceInputSettingsImpl::getKeymapFileNameSave()
{
	/*	
		const char * a = ( const char * ) wxStandardPaths::Get().GetDocumentsDir().c_str() ;
		const char * b = ( const char * ) wxStandardPaths::Get().GetUserConfigDir().c_str() ;
		const char * c = ( const char * ) wxStandardPaths::Get().GetExecutablePath().c_str() ;
		const char * d = ( const char * ) wxStandardPaths::Get().GetConfigDir().c_str() ;


		std::cout << a << endl
		<< b << endl
		<< c << endl
		<< d << endl
		<< (wxChar *)sPath.c_str() << endl ;
		*/

	wxStandardPathsBase &stdPth = wxStandardPaths::Get();
	wxString sPath = stdPth.GetExecutablePath();

	std::cout << wxDir::Exists(wxStandardPaths::Get().GetUserDataDir()) << endl;

	wxFileName name = wxFileName(wxStandardPaths::Get().GetExecutablePath());
	//name.RemoveLastDir();

	cout << name.DirExists() << endl ;

	name.SetName(wxT( "Keymap.cfg"));
	cout << name.FileExists() << endl ;

	return DATA_PATH "Keymap.cfg" ;
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
		std::cout << "Warning : key not found in keymap ! Adding it ..." << std::endl;
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
	m_regularKey->SetValue(wxChar(event.GetKeyCode()));
	iter->second.stdKeyCode = event.GetKeyCode();
}

/**
 * @TODO save the keymap at the right place (not in share, but user home)
 */
void CapriceInputSettingsImpl::saveKeymap()
{
	std::cout << "SAVING THE KEYMAP " << getKeymapFileNameSave() << std::endl;
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
	// TODO
	event.Skip();
}

void CapriceInputSettingsImpl::changeColorPalette( wxCommandEvent& event )
{
	t_CPC _config = emulator.GetConfig();
	_config.scr_tube = (Renderer::MonitorMode)(event.GetInt());
	emulator.GetRenderer().SetMonitor(_config.scr_tube, _config.scr_intensity, _config.scr_remanency);
	emulator.GetRenderer().Init();
}

/***************
 * ROMS
 ***************/
void CapriceInputSettingsImpl::RomChange( wxFileDirPickerEvent& event )
{
	strcpy(emulator.GetConfig().rom_file[0], basename(ROM0file->GetPath().mb_str()));
	strcpy(emulator.GetConfig().rom_file[1], basename(ROM1file->GetPath().mb_str()));
	strcpy(emulator.GetConfig().rom_file[2], basename(ROM2file->GetPath().mb_str()));
	strcpy(emulator.GetConfig().rom_file[3], basename(ROM3file->GetPath().mb_str()));
	strcpy(emulator.GetConfig().rom_file[4], basename(ROM4file->GetPath().mb_str()));
	strcpy(emulator.GetConfig().rom_file[5], basename(ROM5file->GetPath().mb_str()));
	strcpy(emulator.GetConfig().rom_file[6], basename(ROM6file->GetPath().mb_str()));
	strcpy(emulator.GetConfig().rom_file[7], basename(ROM7file->GetPath().mb_str()));

}


