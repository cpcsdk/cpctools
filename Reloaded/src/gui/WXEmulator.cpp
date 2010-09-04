/*
 *   $Id$
 *	 CapriceReloded an Amstrad CPC emulator
 *   Copyright (C) 2009  cpcsdk crew
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */  



#include "WXEmulator.h"
#include "CapriceWindowImpl.h"
#include <wx/defs.h>
#include <wx/stdpaths.h>

extern byte *pbTapeImage;
extern byte *pbTapeImageEnd;


WXEmulator::WXEmulator()
	: Emulator()
{
	//Get path for configuration

	wxString s = wxStandardPaths::Get().GetUserDataDir();
	wxString s2 = wxStandardPaths::Get().GetDataDir();

	if (!wxDir::Exists(s))
	{
#if defined(__WXMSW__)
		wxMkDir(s.mb_str());
#else
		wxMkDir(s.mb_str(), 0700);
#endif

		wxCopyFile(s2 + wxT("/cap32.cfg"), s+ wxT("/cap32.cfg"), false);
		wxCopyFile(s2 + wxT("/Keymap.cfg"), s+ wxT("/Keymap.cfg"), false);
	}
	strcpy(this->_config_path,s.mb_str());
	// Now we need to reload the config with the correct path...
	_config.loadConfiguration();
}


void WXEmulator::logMessage(const char* message) {
    logSync.lock();
	//wxLogWarning(message);
    if(win)
    {
        win->setStatus(message);
    }
    else // If no window print in stderr
    {
        fprintf(stderr, message);
        fprintf(stderr, "\n");
    }
    logSync.unlock();
}


void WXEmulator::PressKey(uint32_t key, uint32_t mod)
{
	dword cpc_key;

	/*
	// PC SHIFT key held down?
	if (mod & wxMOD_SHIFT)
	{
	// consult the SHIFT table
	cpc_key = _input.keyboard_shift[key];
	}
	// PC CTRL key held down?
	else if (mod & wxMOD_CONTROL)
	{
	// consult the CTRL table
	cpc_key = _input.keyboard_ctrl[key];
	}
	// PC AltGr key held down?
	else if (mod & wxMOD_ALT)
	{
	// consult the AltGr table
	cpc_key = _input.keyboard_mode[key];
	}
	else
	{
	// consult the normal table
	cpc_key = _input.keyboard_normal[key];
	}
	*/

	// Always use the same table. Less flexible mapping but makes handling of Ctrl Shift Esc much easier
	cpc_key = _input.keyboard_normal[key];

	if ((!(cpc_key & MOD_EMU_KEY)) && (!_config.paused) && ((byte)cpc_key != 0xff))
	{
		// key is being held down
		_input.keyboard_matrix[(byte)cpc_key >> 4] &= ~(1 << ((byte)cpc_key & 7));

		// CPC SHIFT key required?
		//if (cpc_key & MOD_CPC_SHIFT)
		if (mod & wxMOD_SHIFT)
		{
			// key needs to be SHIFTed
			_input.keyboard_matrix[0x25 >> 4] &= ~(1 << (0x25 & 7));
		}
		else
		{
			// make sure key is unSHIFTed
			_input.keyboard_matrix[0x25 >> 4] |= (1 << (0x25 & 7));
		}

		// CPC CONTROL key required?
		//if (cpc_key & MOD_CPC_CTRL)
		if (mod & wxMOD_CONTROL)
		{
			// CONTROL key is held down
			_input.keyboard_matrix[0x27 >> 4] &= ~(1 << (0x27 & 7));
		}
		else
		{
			// make sure CONTROL key is released
			_input.keyboard_matrix[0x27 >> 4] |= (1 << (0x27 & 7));
		}
	}
}


void WXEmulator::ReleaseKey(uint32_t key, uint32_t mod)
{
	dword cpc_key;
	/*
	// PC SHIFT key held down?
	if (mod & wxMOD_SHIFT)
	{
	// consult the SHIFT table
	cpc_key = _input.keyboard_shift[key];
	}
	// PC CTRL key held down?
	else if (mod & wxMOD_CONTROL)
	{
		// consult the CTRL table
		cpc_key = _input.keyboard_ctrl[key];
	}
	// PC AltGr key held down?
	else if (mod & wxMOD_ALT)
	{
		// consult the AltGr table
		cpc_key = _input.keyboard_mode[key];
	}
	else
	{
		// consult the normal table
		cpc_key = _input.keyboard_normal[key];
	}
	*/

	cpc_key = _input.keyboard_normal[key];

	// a key of the CPC keyboard?
	if ((!_config.paused) && ((byte)cpc_key != 0xff))
	{
		// key has been released
		_input.keyboard_matrix[(byte)cpc_key >> 4] |= (1 << ((byte)cpc_key & 7));
		// make sure key is unSHIFTed
		_input.keyboard_matrix[0x25 >> 4] |= (1 << (0x25 & 7));
		// make sure CONTROL key is not held down
		_input.keyboard_matrix[0x27 >> 4] |= (1 << (0x27 & 7));
	}
}


void WXEmulator::Pause() {
	  Emulator::Pause();
	  win->Pause();
}


void WXEmulator::fdcLed(bool on) {
	win->fdcLed(on);
}


void WXEmulator::fdcNotifyRead(int side, int track, int sector, int mode) {
	// We store the FDC access list for debuggingin the register window 
	fdcLog l = {side, track, sector, mode};
	fdcAccess.push(l);
}


void WXEmulator::getConfigPath(char* buf) {
#ifdef __WXMSW__
	strcpy(buf,"./");
#else
	wxString s = wxStandardPaths::Get().GetUserDataDir();
	strcpy(buf,s.mb_str());
#endif
}
