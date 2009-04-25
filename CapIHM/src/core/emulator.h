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


	void Pause() { GetConfig().paused = 1; }
	void Run() { GetConfig().paused = 0; }

	void emulator_reset(bool bolMF2Reset);

	inline	t_CPC&			GetConfig()					{ return _config;		}
	inline	Renderer&		GetRenderer()				{ return _renderer;		}
	inline	t_Input&		GetInput()					{ return _input;		}

	inline  t_z80regs&		GetZ80()					{ return *_z80;			}
	inline  t_GateArray&	GetGateArray()				{ return *_gateArray;	}
	inline  t_CRTC&			GetCRTC()					{ return *_crtc;		}
	inline  t_VDU&			GetVDU()					{ return *_vdu;			}
	inline  t_PSG&			GetPSG()					{ return *_psg;			}
	inline  t_Memory&		GetMemory()					{ return *_cpcMemory;	}
	inline  t_FDC&			GetFDC()					{ return *_fdc;			}
	inline  t_PPI&			GetPPI()					{ return *_ppi;			}
	inline  t_Tape&			GetTape()					{ return *_tape;		}

	inline t_drive& GetDriveA() {return GetFDC().GetDriveA();}
	inline t_drive& GetDriveB() {return GetFDC().GetDriveB();}

private:
	bool MF2Init();

	int emulator_init();

	void emulator_shutdown();

	int printer_start();
	void printer_stop();

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
