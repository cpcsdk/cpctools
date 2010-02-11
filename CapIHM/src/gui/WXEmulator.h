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

#ifndef __EMULATOR_WX__
#define __EMULATOR_WX__

#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/filefn.h>

#include "emulator.h"
class CapriceWindowImpl;

/**
 * Emulator class specific for the wxWidget Implementation.
 * Maybe it would be better to use adaptor pattern instead of this heritage
 * (cf. difference between fullscreen in SDL and window in wxwidget).
 * Or maybe it would be a good idea to have an instance of SDL, and an instance of wxwidget
 * and swap between them on demand.
 */
class WXEmulator : public Emulator {

  private:
	  CapriceWindowImpl* win;

  public:
    WXEmulator()
    {
      //Get path for configuration

      wxString s = wxStandardPaths::Get().GetUserDataDir();
      wxString s2 = wxStandardPaths::Get().GetDataDir();

      if (!wxDir::Exists(s))
      {
        printf("Copy configuration files ");
        wxMkDir(s.mb_str(), 0700);

        wxCopyFile(s2 + wxT("/cap32.cfg"), s+ wxT("/cap32.cfg"), false);
        wxCopyFile(s2 + wxT("/Keymap.cfg"), s+ wxT("/Keymap.cfg"), false);
      }
	    strcpy(this->_config_path,s.mb_str());
    }

	  static inline WXEmulator* getInstance()
	  {
		  if(!instance) instance = new WXEmulator();
		  std::cout << "[DEBUG] Get WXEmulator at " << instance << endl;
		  return (WXEmulator*)instance;
	  }

	  void setWindow(CapriceWindowImpl* w) { win = w; }

	  /**
	   * Specific code with this GUI
	   */
	  virtual void PressKey(uint32_t key, uint32_t mod);
	  virtual void ReleaseKey(uint32_t key, uint32_t mod);

	  // FDC Led status
	  void fdcLed(bool on);
	  // Emulator->Emulate() (?)
	  // "Pause"/"Breakpoint" menu (de)activation, screen or pause-image display
	  void Pause();

	  // Return folder where config files should be stored
	//  const char * getConfigPath();
} ;

#endif
