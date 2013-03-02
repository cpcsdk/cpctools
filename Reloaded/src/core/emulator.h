/*
 *   $Id$
 *	 Reloded an Amstrad CPC emulator
 *   Copyright (C) 2008  cpcsdk crew
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

//
// Caprice32 Emulator class
//

#ifndef _EMULATOR_H_
#define _EMULATOR_H_

#include <memory>
#include <iostream>

#include "cap32type.h"
#include "config_manager.h"
#include "fdc.h"
#include "z80.h"

#include "misc/timer.h"
#include "misc/synchro.h"
#include <semaphore.h>

#ifdef USE_PTHREAD
#include <pthread.h>
#endif

class t_CRTC;
class t_GateArray;
class t_Input;
class t_Memory;
class t_PPI;
class t_PSG;
class t_Tape;
class t_VDU;
class t_z80regs;

class AudioPlugin;
class VideoPlugin;

#ifdef WITH_ASM
#include "../asm/CapASM.h"
#endif

#ifdef USE_DEBUGGER
extern dword dwDebugFlag;
extern FILE *pfoDebug;
void InitDebug();
#endif

using std::shared_ptr;
using std::weak_ptr;
using std::enable_shared_from_this;

/**
 * Emulator class.
 * This class is now abstract in order to be easily adaptable
 * with other GUI.
 */
class Emulator : public enable_shared_from_this<Emulator>
{
private:
    shared_ptr<Emulator>    this_;
protected:
	shared_ptr<t_CPC>					_config;
	Renderer				_renderer;
	shared_ptr<t_Input>					_input;

	shared_ptr<t_VDU>					_vdu;
	shared_ptr<t_CRTC>		_crtc;
	shared_ptr<t_FDC>					_fdc;
	shared_ptr<t_PPI>					_ppi;
	shared_ptr<t_Tape>					_tape;
	shared_ptr<t_PSG>					_psg;
	shared_ptr<t_Memory>				_cpcMemory;
	shared_ptr<t_z80regs>				_z80;
	shared_ptr<t_GateArray>				_gateArray;

	unsigned int			_cycleCount;
	shared_ptr<AudioPlugin>			_audioPlugin;
	shared_ptr<VideoPlugin>			_videoPlugin;

#ifdef USE_PTHREAD
	pthread_t emuthread;
#endif

	/**
	 * Address to jump if required
	 */
	int goToAddress ;

	Emulator(shared_ptr<VideoPlugin> video, shared_ptr<AudioPlugin> audio);
	virtual ~Emulator();
	static Emulator* instance;

public:
	static Emulator* getInstance();

    inline weak_ptr<Emulator> get_weak_ptr() {return this_;}

	void setCRTC(shared_ptr<t_CRTC> newCRTC);

	bool Init();
	void Loop();
	void Emulate();

	bool isInit() const {return _isInit;}

	//## These are events sent from the GUI to the emulator
	/**
	 * Operate the keyboard emulation
   	 * Specific of the GUI.
	 */
	virtual void PressKey(uint32_t key, uint32_t mod) = 0;
	virtual void ReleaseKey(uint32_t key, uint32_t mod) = 0;


	//## These are events sent from the emulator to the GUI
	// This way the GUI does not have to poll the emu to know these things

	// screen update every 50Hz (do we want that ?)

	// fdc led status
	virtual void fdcLed(bool on) {};
	virtual void fdcNotifyRead(int side, int track, t_sector* sector) {};
		// mode > 0 : read sector
		// mode < 0 : read track
		// mode = 0 : sector not found


	//## These are "internal" methods.
	// They still may be overriden from the gui to do some things more.

	virtual void getConfigPath(char* buf) {
      strcpy(buf,_config_path);
  	}


	virtual void logMessage(const char* message) {
		printf("%s\n", message);
	}


	/**
	 * Pause the emulator
	 */
	virtual inline void Pause()
	{
		GetConfig()->paused = 1;
		timer.pause();
	}

	/**
	 * Pause the emulator due to a breakpoint
	 */
	inline void Breakpoint()
	{
		Pause();
		sem_wait(&breakpointLock);
	}

	/**
	 * Run the emulator
	 */
	inline void Run()
	{
		GetConfig()->paused = 0;
		GetConfig()->breakpoint = 0;
		timer.start();
		sem_post(&breakpointLock);
	}

	/**
	 * Run the emulator step by step
	 */
	inline void Step()
	{
		GetConfig()->paused = 0 ;
		GetConfig()->breakpoint = 1 ;
		timer.start();
		sem_post(&breakpointLock);
	}


	/**
	 * Set PC at memory
	 * @param memory adress where to jump
	 */
	inline void GoTo(int memory)
	{
		goToAddress = memory ;
	}


	/**
	 * Reset computer
	 */
	void emulator_reset(bool bolMF2Reset);

	//inline	t_CPC&			GetConfig()					{ return *_config;		}
	inline	shared_ptr<t_CPC>			GetConfig()					{ return _config;		}
	inline void SetConfig(t_CPC& cfg) {*_config = cfg;}
	/**
	 * Get the renderer of the emulator
	 */
	inline	Renderer&		GetRenderer()				{ return _renderer;		}
	/**
	 * Get the input of the emulator
	 */
	inline	shared_ptr<t_Input>		GetInput()					{ return _input;		}

	void SaveScreenshot(string filename);

	inline t_z80regs& GetZ80() { return *_z80; }
	inline shared_ptr<t_GateArray>	GetGateArray() { return _gateArray;	}
	inline shared_ptr<t_CRTC> GetCRTC() { return _crtc; }
	inline shared_ptr<t_VDU> GetVDU() { return _vdu; }
	inline shared_ptr<t_PSG> GetPSG() { return _psg; }
	inline shared_ptr<t_Memory> GetMemory() { return _cpcMemory; }
	inline shared_ptr<t_FDC> GetFDC() { return _fdc; }
	inline shared_ptr<t_PPI> GetPPI() { return _ppi; }
	inline shared_ptr<t_Tape> GetTape() { return _tape; }
	/**
	 * Get drive a
	 */
	inline t_drive& GetDriveA() {return GetFDC()->GetDriveA();}
	/**
	 * Get drive b
	 */
	inline t_drive& GetDriveB() {return GetFDC()->GetDriveB();}

    inline shared_ptr<AudioPlugin> GetAudioPlugin() {return _audioPlugin;}

	 char _config_path[1024];
protected:
	bool MF2Init();

	int emulator_init();

	/**
	 * Shutdown the emu
	 */
	void emulator_shutdown();

	int printer_start();
	void printer_stop();

	/**
	 * Set PC at goToAddress
	 */
	inline void ExecGoTo()
	{
		std::cout << "Launch " << std::hex << goToAddress << std::endl ;

		//TODO check if in amsdos execution mode
		//Select bank
		GetMemory()->SetROMConfig(0);
		GetMemory()->ga_memory_manager();

		//Jump
		GetZ80().PC.w.l = goToAddress & 0xffff ;

		//Reset
		goToAddress = -1 ;
	}


	bool FPSDisplay;
	bool exitRequested;
	static bool sInitOnce;

	Timer timer;

	dword dwTicks ;
	dword dwFPS ;
	dword dwFrameCount ;

	dword dwTicksOffset ;
	dword dwTicksTarget ;
	dword dwTicksTargetFPS ;

	bool _isInit;

	//int loopcon;
public:
    SysSync emuSync; // Global sync on Emulator object
private:
	sem_t breakpointLock;
};

#endif
