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
// Caprice32 input manager
//

#include "input.h"
#include "cap32type.h"
#include "config.h"
#include "CPCKeyDefs.h"
#include <iostream>


dword t_Input::cpc_kbd[149] =
{
		0x40,						// CPC_0
		0x80,						// CPC_1
		0x81,						// CPC_2
		0x71,						// CPC_3
		0x70,						// CPC_4
		0x61,						// CPC_5
		0x60,						// CPC_6
		0x51,						// CPC_7
		0x50,						// CPC_8
		0x41,						// CPC_9
		0x85,						// CPC_a
		0x66,						// CPC_b
		0x76,						// CPC_c
		0x75,						// CPC_d
		0x72,						// CPC_e
		0x65,						// CPC_f
		0x64,						// CPC_g
		0x54,						// CPC_h
		0x43,						// CPC_i
		0x55,						// CPC_j
		0x45,						// CPC_k
		0x44,						// CPC_l
		0x46,						// CPC_m
		0x56,						// CPC_n
		0x42,						// CPC_o
		0x33,						// CPC_p
		0x83,						// CPC_q
		0x62,						// CPC_r
		0x74,						// CPC_s
		0x63,						// CPC_t
		0x52,						// CPC_u
		0x67,						// CPC_v
		0x73,						// CPC_w
		0x77,						// CPC_x
		0x53,						// CPC_y
		0x87,						// CPC_z
		0x35 | MOD_CPC_SHIFT,		// CPC_ASTERISK
		0x32,						// CPC_AT
		0x26,						// CPC_BACKSLASH
		0x86,						// CPC_CAPSLOCK
		0x20,						// CPC_CLR
		0x35,						// CPC_COLON
		0x47,						// CPC_COMMA
		0x27,						// CPC_CONTROL
		0x11,						// CPC_COPY
		0x02,						// CPC_CUR_DOWN
		0x10,						// CPC_CUR_LEFT
		0x01,						// CPC_CUR_RIGHT
		0x00,						// CPC_CUR_UP
		0x81 | MOD_CPC_SHIFT,		// CPC_DBLQUOTE
		0x97,						// CPC_DEL
		0x70 | MOD_CPC_SHIFT,		// CPC_DOLLAR
		0x06,						// CPC_ENTER
		0x31 | MOD_CPC_SHIFT,		// CPC_EQUAL
		0x82,						// CPC_ESC
		0x80 | MOD_CPC_SHIFT,		// CPC_EXCLAMATN
		0x17,						// CPC_F0
		0x15,						// CPC_F1
		0x16,						// CPC_F2
		0x05,						// CPC_F3
		0x24,						// CPC_F4
		0x14,						// CPC_F5
		0x04,						// CPC_F6
		0x12,						// CPC_F7
		0x13,						// CPC_F8
		0x03,						// CPC_F9
		0x07,						// CPC_FPERIOD
		0x37 | MOD_CPC_SHIFT,		// CPC_GREATER
		0x71 | MOD_CPC_SHIFT,		// CPC_HASH
		0x21,						// CPC_LBRACKET
		0x21 | MOD_CPC_SHIFT,		// CPC_LCBRACE
		0x50 | MOD_CPC_SHIFT,		// CPC_LEFTPAREN
		0x47 | MOD_CPC_SHIFT,		// CPC_LESS
		0x25,						// CPC_LSHIFT
		0x31,						// CPC_MINUS
		0x61 | MOD_CPC_SHIFT,		// CPC_PERCENT
		0x37,						// CPC_PERIOD
		0x32 | MOD_CPC_SHIFT,		// CPC_PIPE
		0x34 | MOD_CPC_SHIFT,		// CPC_PLUS
		0x30 | MOD_CPC_SHIFT,		// CPC_POUND
		0x30,						// CPC_POWER
		0x36 | MOD_CPC_SHIFT,		// CPC_QUESTION
		0x51 | MOD_CPC_SHIFT,		// CPC_QUOTE
		0x23,						// CPC_RBRACKET
		0x23 | MOD_CPC_SHIFT,		// CPC_RCBRACE
		0x22,						// CPC_RETURN
		0x41 | MOD_CPC_SHIFT,		// CPC_RIGHTPAREN
		0x25,						// CPC_RSHIFT
		0x34,						// CPC_SEMICOLON
		0x36,						// CPC_SLASH
		0x57,						// CPC_SPACE
		0x84,						// CPC_TAB
		0x40 | MOD_CPC_SHIFT,		// CPC_UNDERSCORE
		0x90,						// CPC_J0_UP
		0x91,						// CPC_J0_DOWN
		0x92,						// CPC_J0_LEFT
		0x93,						// CPC_J0_RIGHT
		0x94,						// CPC_J0_FIRE1
		0x95,						// CPC_J0_FIRE2
		0x60,						// CPC_J1_UP
		0x61,						// CPC_J1_DOWN
		0x62,						// CPC_J1_LEFT
		0x63,						// CPC_J1_RIGHT
		0x64,						// CPC_J1_FIRE1
		0x65,						// CPC_J1_FIRE2
};

int t_Input::joy_layout[12][2] =
{
	{ CPC_J0_UP,		SDLK_UP },
	{ CPC_J0_DOWN,		SDLK_DOWN },
	{ CPC_J0_LEFT,		SDLK_LEFT },
	{ CPC_J0_RIGHT,		SDLK_RIGHT },
	{ CPC_J0_FIRE1,		SDLK_z },
	{ CPC_J0_FIRE2,		SDLK_x },
	{ CPC_J1_UP,		0 },
	{ CPC_J1_DOWN,		0 },
	{ CPC_J1_LEFT,		0 },
	{ CPC_J1_RIGHT,		0 },
	{ CPC_J1_FIRE1,		0 },
	{ CPC_J1_FIRE2,		0 }
};

void t_Input::Reset()
{
	// clear CPC keyboard matrix
	memset(keyboard_matrix, 0xff, sizeof(keyboard_matrix));
}

void t_Input::input_swap_joy (t_CPC &CPC)
{
	dword n, pc_idx, val;

	for (n = 0; n < 6; n++)
	{
		// get the PC key to change the assignment for
		pc_idx = joy_layout[n][1];
		if (pc_idx)
		{
			// keep old value
			val = keyboard_normal[pc_idx];
			// assign new function
			keyboard_normal[pc_idx] = cpc_kbd[joy_layout[n][0]];
			// store old value
			cpc_kbd[joy_layout[n][0]] = val;
		}
	}
}


void t_Input::setupKey(int cpc_code, int pc_stdcode, int pc_shiftcode  , int pc_ctrlcode , int pc_altcode )
{
    keyboard_normal[pc_stdcode]=cpc_kbd[cpc_code];
    keyboard_shift[pc_shiftcode>0?pc_shiftcode:pc_stdcode]=cpc_kbd[cpc_code]|MOD_CPC_SHIFT;
    keyboard_ctrl[pc_ctrlcode>0?pc_ctrlcode:pc_stdcode]=cpc_kbd[cpc_code]|MOD_CPC_CTRL;
    keyboard_mode[pc_altcode>0?pc_altcode:pc_stdcode]=cpc_kbd[cpc_code];
}


int t_Input::input_init (t_CPC &CPC)
{
	keyboard_normal.clear();
	keyboard_shift.clear();
	keyboard_ctrl.clear();
	keyboard_mode.clear();

	// enable keyboard joystick emulation?
	if (CPC.joysticks)
	{
		input_swap_joy(CPC);
	}

	return 0;
}
