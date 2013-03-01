/*
 *   $Id: gatearray.cpp 60 2008-11-25 17:08:52Z giot.romain $
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
// Caprice32 GateArray emulator
//

#include "gatearray.h"
#include "cap32type.h"
#include "z80.h"
#include "render.h"

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

t_GateArray::t_GateArray(Renderer &render, shared_ptr<t_z80regs> z80) :
_z80(z80),
_renderer(render)
{
}

void t_GateArray::Reset(int mode)
{
	requested_scr_mode = mode;
	scr_mode = mode;

	pen = 0;

	ink_values[0] = 0;  // to validate !
	ink_values[1] = 0;

	for (int i=0 ; i<17 ; i++)
	{
		SetInk(i, 20); // Set all color to Bright Cyan on Reset
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
    auto z80 = _z80.lock();
	sl_count++; // update GA scan line counter
	if (sl_count == 52) { // trigger interrupt?
#ifndef NO_Z80
		z80->int_pending = 1; // queue Z80 interrupt
#endif
		sl_count = 0; // clear counter
	}
	if (hs_count) { // delaying on VSYNC?
		hs_count--;
		if (!hs_count) {
			if (sl_count >= 32) { // counter above save margin?
#ifndef NO_Z80
				z80->int_pending = 1; // queue interrupt
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
