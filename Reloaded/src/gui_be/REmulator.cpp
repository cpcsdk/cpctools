#include "REmulator.h"

#include "../core/input.h"

bool REmulator::Init()
{
	if (!Emulator::Init())
		return false;
	// Line 0
	_input->keyboard_normal[87] = 0; // cursor up
	_input->keyboard_normal[99] = 1; // cursor right
	_input->keyboard_normal[98] = 2; // cursor down
	_input->keyboard_normal[57] = 3; // F9
	_input->keyboard_normal[77] = 4; // F6
	_input->keyboard_normal[90] = 5; // F3
	_input->keyboard_normal[37] = 6; // enter
	_input->keyboard_normal[101] = 7; // F.

	// Line 1
	_input->keyboard_normal[97] = 16; // cursor left
	_input->keyboard_normal[93] = 17; // copy
	_input->keyboard_normal[55] = 18; // f7
	_input->keyboard_normal[56] = 19; // f8
	_input->keyboard_normal[76] = 20; // f5
	_input->keyboard_normal[88] = 21; // f1
	_input->keyboard_normal[89] = 22; // F2
	_input->keyboard_normal[100] = 23; // F0

	// Line 2 (32)
	_input->keyboard_normal[52] = 32; // clr
	_input->keyboard_normal[50] = 33; // [{
	_input->keyboard_normal[71] = 34; // return
	_input->keyboard_normal[51] = 35; // ]}
	// f4
	_input->keyboard_normal[75] = 36; // shift
	_input->keyboard_normal[86] = 37; // shift
	_input->keyboard_normal[92] = 38; // ctrl

	// Line 3 (48)
	_input->keyboard_normal[29] = 48; // ^£
	_input->keyboard_normal[28] = 49; // -=
	_input->keyboard_normal[49] = 50; // @|
	_input->keyboard_normal[48] = 51; // p
	_input->keyboard_normal[70] = 52; // +
	_input->keyboard_normal[69] = 53; // *
	_input->keyboard_normal[85] = 54; // ?
	_input->keyboard_normal[84] = 55; // >

	// Line 4
	_input->keyboard_normal[17] = 64; // 0
	_input->keyboard_normal[26] = 65; // 9
	_input->keyboard_normal[47] = 66; // o
	_input->keyboard_normal[46] = 67; // i
	_input->keyboard_normal[68] = 68; // l
	_input->keyboard_normal[67] = 69; // k
	_input->keyboard_normal[82] = 70; // m
	_input->keyboard_normal[83] = 71; // <

	// Line 5
	_input->keyboard_normal[25] = 80; // 8
	_input->keyboard_normal[24] = 81; // 7
	_input->keyboard_normal[45] = 82; // u
	_input->keyboard_normal[44] = 83; // y
	_input->keyboard_normal[65] = 84; // h
	_input->keyboard_normal[66] = 85; // j
	_input->keyboard_normal[81] = 86; // n
	_input->keyboard_normal[94] = 87; // space

	// Line 6
	_input->keyboard_normal[23] = 96; // 6
	_input->keyboard_normal[22] = 97; // 5
	_input->keyboard_normal[42] = 98; // r
	_input->keyboard_normal[43] = 99; // t
	_input->keyboard_normal[64] = 100; // g
	_input->keyboard_normal[63] = 101; // f
	_input->keyboard_normal[80] = 102; // b
	_input->keyboard_normal[79] = 103; // v

	// Line 7
	_input->keyboard_normal[21] = 120; // 3
	_input->keyboard_normal[20] = 121; // 4
	_input->keyboard_normal[41] = 122; // e
	_input->keyboard_normal[40] = 123; // w
	_input->keyboard_normal[61] = 124; // s
	_input->keyboard_normal[62] = 125; // d
	_input->keyboard_normal[78] = 126; // c
	_input->keyboard_normal[77] = 127; // x

	// Line 8
	_input->keyboard_normal[18] = 136; // 1
	_input->keyboard_normal[19] = 137; // 2
	_input->keyboard_normal[1] = 138; // esc
	_input->keyboard_normal[39] = 139; // q
	_input->keyboard_normal[38] = 140; // tab 
	_input->keyboard_normal[60] = 141; // a
	_input->keyboard_normal[59] = 142; // caps
	_input->keyboard_normal[76] = 143; // z

	// Line 9 (152)
	// joy.......
	_input->keyboard_normal[30] = 159; // del

	return true;
}

void REmulator::PressKey(uint32_t key, uint32_t mod)
{
	dword cpc_key;
	cpc_key = _input->keyboard_normal[key];

	if ((!(cpc_key & MOD_EMU_KEY)) && (!_config->paused)
			&& ((byte)cpc_key != 0xff))
	{
		// key is being held down
		_input->keyboard_matrix[(byte)cpc_key >> 4] &= ~(1 << ((byte)cpc_key & 7));

		// CPC SHIFT key required?
		if (mod & B_SHIFT_KEY)
		{
			// key needs to be SHIFTed
			_input->keyboard_matrix[0x25 >> 4] &= ~(1 << (0x25 & 7));
		}
		else
		{
			// make sure key is unSHIFTed
			_input->keyboard_matrix[0x25 >> 4] |= (1 << (0x25 & 7));
		}

		// CPC CONTROL key required?
		if (mod & B_CONTROL_KEY)
		{
			// CONTROL key is held down
			_input->keyboard_matrix[0x27 >> 4] &= ~(1 << (0x27 & 7));
		}
		else
		{
			// make sure CONTROL key is released
			_input->keyboard_matrix[0x27 >> 4] |= (1 << (0x27 & 7));
		}
	}
}

void REmulator::ReleaseKey(uint32_t key, uint32_t mod)
{
	dword cpc_key;
	cpc_key = _input->keyboard_normal[key];

	// a key of the CPC keyboard?
	if ((!_config->paused) && ((byte)cpc_key != 0xff))
	{
		// key has been released
		_input->keyboard_matrix[(byte)cpc_key >> 4] |= (1 << ((byte)cpc_key & 7));
		// make sure key is unSHIFTed
		_input->keyboard_matrix[0x25 >> 4] |= (1 << (0x25 & 7));
		// make sure CONTROL key is not held down
		_input->keyboard_matrix[0x27 >> 4] |= (1 << (0x27 & 7));
	}
}

