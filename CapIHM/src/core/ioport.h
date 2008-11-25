//
// Caprice 32 I/O port emulation
//

#ifndef _IOPORT_H_
#define _IOPORT_H_

#include "cap32type.h"

class t_z80regs;
class t_CRTC;
class t_GateArray;
class t_Memory;
class t_FDC;
class t_PSG;
class t_PPI;
class t_CPC;
class t_Tape;
class t_Input;
class Emulator;

class IOPort
{
private:
	t_z80regs		*_z80;
	t_CRTC			&CRTC;
	t_GateArray		&GateArray;
	t_Memory		&Memory;
	t_FDC			&FDC;
	t_PSG			&PSG;
	t_PPI			&PPI;
	t_CPC			&CPC;
	t_Tape			&Tape;
	t_Input			&Input;
public:
	IOPort(Emulator &emulator);

	inline void SetZ80(t_z80regs *z80)					{ _z80 = z80;		}

	byte z80_IN_handler(reg_pair port);
	void z80_OUT_handler(reg_pair port, byte val);
};

#endif
