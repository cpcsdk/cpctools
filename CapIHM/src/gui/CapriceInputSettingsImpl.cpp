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
	emulatorInputHandler(static_cast<CapriceWindowImpl*>(WinID)->GetEmulator()->GetInput()),
	lastClickedButton(NULL)
{
	std::ifstream file;
	CPC_Key tmpk;
	uint16_t kid;
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

}

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



CapriceInputSettingsImpl::~CapriceInputSettingsImpl()
{
	saveKeymap();
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

void CapriceInputSettingsImpl::onSave(wxCommandEvent& event)
{
	saveKeymap();
}

void CapriceInputSettingsImpl::applySettings()
{
	for (CPC_Keymap::iterator iter = keymap.begin();iter!=keymap.end();iter++)
	{
		emulatorInputHandler.setupKey(iter->first, iter->second.stdKeyCode,
			iter->second.shiftKeyCode, iter->second.ctrlKeyCode);
	}
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
