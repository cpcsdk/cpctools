//
// Caprice32 GateArray emulator
//

#include "gatearray.h"
#include "cap32type.h"

#include <AntTweakBar.h>

#ifdef Z80_H
extern t_z80regs z80;
#endif

/*
	Soft -> Hard color table
0 Black				20	&54 
1 Blue				4	&44 
2 Bright Blue		21	&55 
3 Red				28	&5C 
4 Magenta			24	&58 
5 Mauve				29	&5D 
6 Bright Red		12	&4C 
7 Purple			5	&45 
8 Bright Magenta	13	&4D 
9 Green				22	&56 
10 Cyan				6	&46 
11 Sky Blue			23	&57 
12 Yellow			30	&5E 
13 White			0	&40 
14 Pastel Blue		31	&5F 
15 Orange			14	&4E 
16 Pink				7	&47 
17 Pastel Magenta	15	&4F 
18 Bright Green		18	&52 
19 Sea Green		2	&42 
20 Bright Cyan		19	&53 
21 Lime				26	&5A 
22 Pastel Green		25	&59 
23 Pastel Cyan		27	&5B 
24 Bright Yellow	10	&4A 
25 Pastel Yellow	3	&43 
26 Bright White		11	&4B
*/

// Table de conversion Hard->Soft couleur
unsigned int t_GateArray::HardToSoftTable[32] = 
{
	13,		// White			0	&40 
	13,		// White			1	&41
	19,		// Sea Green		2	&42 
	25,		// Pastel Yellow	3	&43 
	1,		// Blue				4	&44 
	7,		// Purple			5	&45 
	10,		// Cyan				6	&46 
	16,		// Pink				7	&47 
	7,		// Purple			8	&48 
	25,		// Pastel Yellow	9	&49 
	24,		// Bright Yellow	10	&4A 
	26,		// Bright White		11	&4B 
	6,		// Bright Red		12	&4C 
	8,		// Bright Magenta	13	&4D 
	15,		// Orange			14	&4E 
	17,		// Pastel Magenta	15	&4F 
	1,		// Blue				16	&50
	19,		// Sea Green		17	&51 
	18,		// Bright Green		18	&52 
	20,		// Bright Cyan		19	&53 
	0,		// Black			20	&54 
	2,		// Bright Blue		21	&55 
	9,		// Green			22	&56 
	11,		// Sky Blue			23	&57 
	4,		// Magenta			24	&58 
	22,		// Pastel Green		25	&59 
	21,		// Lime				26	&5A 
	23,		// Pastel Cyan		27	&5B 
	3,		// Red				28	&5C 
	5,		// Mauve			29	&5D 
	12,		// Yellow			30	&5E 
	14		// Pastel Blue		31	&5F 
};

// Table de conversion Hard->Soft couleur
unsigned int t_GateArray::SoftToHardTable[27] = 
{
	20,4,21,28,24,29,12,5,13,22,6,23,30,0,31,14,7,15,18,2,19,26,25,27,10,3,11
};

t_GateArray::t_GateArray(Renderer &render) :
_renderer(render)
{
}

void t_GateArray::Reset(int mode)
{
	requested_scr_mode = mode;
	scr_mode = mode;

	pen = 0;
	for (int i=0 ; i<17 ; i++)
	{
		SetInk(i, SoftToHardTable[0]);
	}

	hs_count = 0;
	sl_count = 0;
	int_delay = 0;

	_renderer.SetMode(scr_mode);
}

void t_GateArray::SetColour(unsigned char colour)
{
	ink_values[pen] = colour;
	
	_renderer.SetPalette(pen, colour);
	if (pen < 2) 
	{
		_renderer.SetAntiAliasingColour(ink_values[0], ink_values[1]);
	}
}

void t_GateArray::SetInk(unsigned int i, unsigned char v)
{
	unsigned char val = v & 63;
	ink_values[i] = val;
	
	_renderer.SetPalette(i, val);
	if (i < 2) 
	{
		_renderer.SetAntiAliasingColour(ink_values[0], ink_values[1]);
	}
}

void t_GateArray::MatchHsw()
{
	sl_count++; // update GA scan line counter
	if (sl_count == 52) { // trigger interrupt?
#ifdef Z80_H
		z80.int_pending = 1; // queue Z80 interrupt
#endif
		sl_count = 0; // clear counter
	}
	if (hs_count) { // delaying on VSYNC?
		hs_count--;
		if (!hs_count) {
			if (sl_count >= 32) { // counter above save margin?
#ifdef Z80_H
				z80.int_pending = 1; // queue interrupt
#endif
			}
			sl_count = 0; // clear counter
		}
	}
}

void t_GateArray::HSyncChangeMode()
{
	if (scr_mode == requested_scr_mode)
		return;

	scr_mode = requested_scr_mode; // execute mode change
	
	_renderer.SetMode(scr_mode);
}
