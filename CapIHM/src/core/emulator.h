/*
 *   $Id$
 *	 CapriceReloded an Amstrad CPC emulator
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

#include "cap32type.h"
#include "render.h"
#include "config.h"
#include "input.h"
#include "fdc.h"

class t_z80regs;
class t_VDU;
class t_GateArray;
class t_CRTC;
class t_PSG;
class t_PPI;
class t_Tape;
class t_Memory;

#ifdef DEBUG
extern dword dwDebugFlag;
extern FILE *pfoDebug;
void InitDebug();
#endif

class Emulator
{
private:
	t_CPC					_config;
	Renderer				_renderer;
	t_Input					_input;
	t_z80regs				*_z80;
	t_VDU					*_vdu;
	t_GateArray				*_gateArray;
	t_CRTC					*_crtc;
	t_PSG					*_psg;
	t_FDC					*_fdc;
	t_PPI					*_ppi;
	t_Tape					*_tape;
	t_Memory				*_cpcMemory;
	unsigned int			_cycleCount;

public:
	Emulator();
	~Emulator();

	bool Init();
	void Loop();
	void Emulate();


  /**
   * Pause the emulator
   */
	void Pause() { GetConfig().paused = 1; }
  /**
   * Run the emulator
   */
	void Run() { GetConfig().paused = 0; }

  /**
   * Reset computer
   */
	void emulator_reset(bool bolMF2Reset);

	inline	t_CPC&			GetConfig()					{ return _config;		}
  /**
   * Get the renderer of the emulator
   */
	inline	Renderer&		GetRenderer()				{ return _renderer;		}
  /**
   * Get the input of the emulator
   */
	inline	t_Input&		GetInput()					{ return _input;		}

  /**
   * Get the processor
   */
	inline  t_z80regs&		GetZ80()					{ return *_z80;			}
  /**
   * Get the gate array
   */
	inline  t_GateArray&	GetGateArray()				{ return *_gateArray;	}
  /**
   * Get the CRTC
   */
	inline  t_CRTC&			GetCRTC()					{ return *_crtc;		}
  /**
   * Get the Screen
   */
	inline  t_VDU&			GetVDU()					{ return *_vdu;			}
  /**
   * Get the PSG
   */
	inline  t_PSG&			GetPSG()					{ return *_psg;			}
  /**
   * Get the Memory
   */
	inline  t_Memory&		GetMemory()					{ return *_cpcMemory;	}
  /**
   * Get the FDC
   */
	inline  t_FDC&			GetFDC()					{ return *_fdc;			}
  /**
   * Get the ppi
   */
	inline  t_PPI&			GetPPI()					{ return *_ppi;			}
  /**
   * Get the tape reader
   */
	inline  t_Tape&			GetTape()					{ return *_tape;		}
  /**
   * Get drive a
   */
	inline t_drive& GetDriveA() {return GetFDC().GetDriveA();}
  /**
   * Get drive b
   */
	inline t_drive& GetDriveB() {return GetFDC().GetDriveB();}

private:
	bool MF2Init();

	int emulator_init();

  /**
   * Shutdown the emu
   */
	void emulator_shutdown();

	int printer_start();
	void printer_stop();

  /**
   * Operate the keyboard emulation
   */
	bool KeyboardEmulation();
	bool FPSDisplay;

	
	dword dwTicks ;
	dword dwFPS ;
	dword dwFrameCount ;

	dword dwTicksOffset ;
	dword dwTicksTarget ;
	dword dwTicksTargetFPS ;

};

#endif
