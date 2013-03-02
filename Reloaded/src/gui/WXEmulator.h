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

#include <memory>
#include <queue>

#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/log.h>

#include "core/emulator.h"

#include "misc/synchro.h"
#include "misc/log.h"

using std::shared_ptr;

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
    SysSync logSync;


  protected:
	WXEmulator(shared_ptr<VideoPlugin> video, shared_ptr<AudioPlugin> audio);

  public:
	// TODO : accessors for this instead
	struct fdcLog {int s; int t; t_sector* val;};
	std::queue<fdcLog> fdcAccess;

	static inline WXEmulator* createInstance(shared_ptr<VideoPlugin> video,
		shared_ptr<AudioPlugin> audio)
	{
		instance = new WXEmulator(video, audio);
		return static_cast<WXEmulator*>(instance);
	}

	static inline WXEmulator* getInstance()
	{
		return (WXEmulator*)instance;
	}

	void setWindow(CapriceWindowImpl* w) { win = w; }

	/**
	* Specific code with this GUI
	*/
	virtual void PressKey(uint32_t key, uint32_t mod);
	virtual void ReleaseKey(uint32_t key, uint32_t mod);

	void logMessage(const char* message);

	// FDC Led status
	void fdcLed(bool on);
	void fdcNotifyRead(int side, int track, t_sector* sector);
	// Emulator->Emulate() (?)
	// "Pause"/"Breakpoint" menu (de)activation, screen or pause-image display
	void Pause();

	//Return folder where config files should be stored
	void getConfigPath(char* buf);

	~WXEmulator() { InfoLogMessage("Exit and destruct WXEmulator (0x%x)", this);}
} ;

#endif
