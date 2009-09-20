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



#include "WXEmulator.h"
#include <wx/defs.h>

byte *pbTapeImage = NULL;
byte *pbTapeImageEnd = NULL;


void WXEmulator::PressKey(uint32_t key, uint32_t mod)
{
	dword cpc_key;

	/*
	// PC SHIFT key held down?
	if (mod & wxMOD_SHIFT)
	{
		// consult the SHIFT table
		cpc_key = _input.keyboard_shift[key];
	}
	// PC CTRL key held down?
	else if (mod & wxMOD_CONTROL)
	{
		// consult the CTRL table
		cpc_key = _input.keyboard_ctrl[key];
	}
	// PC AltGr key held down?
	else if (mod & wxMOD_ALT)
	{
		// consult the AltGr table
		cpc_key = _input.keyboard_mode[key];
	}
	else
	{
		// consult the normal table
		cpc_key = _input.keyboard_normal[key];
	}
	*/

	// Always use the same table. Less flexible mapping but makes handling of Ctrl Shift Esc much easier
	cpc_key = _input.keyboard_normal[key];

	if ((!(cpc_key & MOD_EMU_KEY)) && (!_config.paused) && ((byte)cpc_key != 0xff))
	{
		// key is being held down
		_input.keyboard_matrix[(byte)cpc_key >> 4] &= ~(1 << ((byte)cpc_key & 7));

		// CPC SHIFT key required?
		//if (cpc_key & MOD_CPC_SHIFT)
		if (mod & wxMOD_SHIFT)
		{
			// key needs to be SHIFTed
			_input.keyboard_matrix[0x25 >> 4] &= ~(1 << (0x25 & 7));
		}
		else
		{
			// make sure key is unSHIFTed
			_input.keyboard_matrix[0x25 >> 4] |= (1 << (0x25 & 7));
		}

		// CPC CONTROL key required?
		//if (cpc_key & MOD_CPC_CTRL)
		if (mod & wxMOD_CONTROL)
		{
			// CONTROL key is held down
			_input.keyboard_matrix[0x27 >> 4] &= ~(1 << (0x27 & 7));
		}
		else
		{
			// make sure CONTROL key is released
			_input.keyboard_matrix[0x27 >> 4] |= (1 << (0x27 & 7));
		}
	}
}


void WXEmulator::ReleaseKey(uint32_t key, uint32_t mod)
{
	dword cpc_key;
	/*
	// PC SHIFT key held down?
	if (mod & wxMOD_SHIFT)
	{
		// consult the SHIFT table
		cpc_key = _input.keyboard_shift[key];
	}
	// PC CTRL key held down?
	else if (mod & wxMOD_CONTROL)
	{
		// consult the CTRL table
		cpc_key = _input.keyboard_ctrl[key];
	}
	// PC AltGr key held down?
	else if (mod & wxMOD_ALT)
	{
		// consult the AltGr table
		cpc_key = _input.keyboard_mode[key];
	}
	else
	{
		// consult the normal table
		cpc_key = _input.keyboard_normal[key];
	}
	*/

	cpc_key = _input.keyboard_normal[key];

	// a key of the CPC keyboard?
	if (!(cpc_key & MOD_EMU_KEY))
	{
		if ((!_config.paused) && ((byte)cpc_key != 0xff))
		{
			// key has been released
			_input.keyboard_matrix[(byte)cpc_key >> 4] |= (1 << ((byte)cpc_key & 7));
			// make sure key is unSHIFTed
			_input.keyboard_matrix[0x25 >> 4] |= (1 << (0x25 & 7));
			// make sure CONTROL key is not held down
			_input.keyboard_matrix[0x27 >> 4] |= (1 << (0x27 & 7));
		}
	}
	// process emulator specific keys
	else
	{
		switch (cpc_key)
		{
			case CAP32_FULLSCRN:
				{
					audio_pause(_config);
					SDL_Delay(20);

					if (!_renderer.ToggleFullScreen())
					{
						fprintf(stderr, "video_init() failed. Aborting.\n");
						exit(-1);
					}

					audio_resume(_config);
					break;
				}

			case CAP32_TAPEPLAY:
				{
					if (pbTapeImage)
					{
						if (_config.tape_play_button)
						{
							_config.tape_play_button = 0;
						}
						else
						{
							_config.tape_play_button = 0x10;
						}
					}
					break;
				}

			case CAP32_RESET:
				{
					emulator_reset(false);
					break;
				}

			case CAP32_JOY:
				{
					_config.joysticks = _config.joysticks ? 0 : 1;
					_input.input_swap_joy(_config);
					break;
				}

			case CAP32_EXIT:
				{
					exitRequested = true;
					break;
				}

			case CAP32_FPS:
				{
					FPSDisplay = !FPSDisplay;
					break;
				}

			case CAP32_SPEED:
				{
					_config.limit_speed = _config.limit_speed ? 0 : 1;
					//fprintf(stderr, "Toggle limit speed\n");
					break;
				}

#ifdef USE_DEBUGGER
			case CAP32_DEBUG:
				{
					dwDebugFlag = dwDebugFlag ? 0 : 1;
#ifdef USE_DEBUGGER_CRTC
					if (!dwDebugFlag)
						break;

					for (int n = 0; n < 14; n++)
					{
						fprintf(pfoDebug, "CRTC Reg %02x = %02x\r\n", n, _crtc->GetRegisterValue( n ));
					}
#endif
					break;
				}
#endif
		}
	}
}


