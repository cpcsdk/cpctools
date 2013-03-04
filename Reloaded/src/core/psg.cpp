/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2004 Ulrich Doewich

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/* General Instruments AY-3-8912 Programable Sound Generator

   converted from the AY Emulator project by Sergey Vladimirovich Bulba
   based on Version 2.7 beta 3

   Jul 07, 2001 - 16:07 basic functionality of sound_fill_buffer, converted from asm code
   Jul 20, 2001 - 18:49 sound emulation works properly for the first time; tone and noise implemented
   Jul 28, 2001 - 15:23 switched to the AY emulation from MAME
   Jul 28, 2001 - 16:09 fixed the envelope problem: the amp_table was still being referenced as byte!
   Aug 01, 2001 - 17:28 added code to support mono output
   Aug 12, 2001 - 21:42 moved psg_reg_write() from cap32.cpp
   Dec 08, 2002 - 10:43 properly integrated the tape noise into the mixing buffer
   Apr 29, 2003 - 21:48 started to convert AyEmul source code from Pascal to C
   Apr 30, 2003 - 23:39 continued with conversion
   May 04, 2003 - 00:13 modified Synthesizer_Stereo16 for Caprice32 - new AY code works for the first time!
   May 04, 2003 - 16:09 fixed the problem with the envelope period: the struct in TRegisterAY wasn't accessed properly
   May 05, 2003 - 18:46 added alternate synthesizers for mono/stereo + 8/16 bits; some code cleanup
   May 06, 2003 - 15:18 enabled 'tape noise' again
   May 13, 2003 - 15:57 moved counter init to InitAYCounterVars; corrected the 'divide by zero' possibility in the
                        synthesizer routines: LoopCountInit calculation needs to take CPC speed into account
   May 15, 2003 - 00:08 replaced the rand() call with Sergey's noise algorithm
   May 23, 2003 - 14:58 added code to supress sound if the tone frequency is set to a value below 5, and no
                        envelope assigned to the channel
   Jun 04, 2003 - 19:37 added support for Digiblaster/Soundplayer on the printer port
*/

#include <memory>
#include <cmath>

#include "cap32.h"
#include "psg.h"
#include "tape.h"
#include "audioPlugin.h"
#include "emulator.h"

#include "misc/log.h"

#include <iostream>

#ifdef AYLET
#include <sound.h>
#endif

#define TAPE_VOLUME 32

#ifdef ST_SOUND
#include "YmProfiles.h"
#endif

using std::shared_ptr;
using std::make_shared;

t_PSG::t_PSG(shared_ptr<Emulator> emulator) :
    _emulator(emulator)
{
#ifdef AYLET
    sound_init();
    sound_ay_reset_cpc();
#endif

#ifdef AYEMU
    ayemu_init(&m_ayemu);
    ayemu_set_chip_freq(&m_ayemu, 1000000);
    ayemu_set_chip_type(&m_ayemu, AYEMU_AY_LOG, NULL);
    if(_emulator.lock()->GetConfig()->enableSound)
        ayemu_set_sound_format(&m_ayemu, audio_spec->freq , audio_spec->channels, audio_spec->format==AUDIO_S16LSB?16:8);
    else
        ayemu_set_sound_format(&m_ayemu,44100,2,16); // No audio_spec if sound disabled, so use default
    ayemu_set_stereo(&m_ayemu, AYEMU_ABC, NULL);
#endif

#ifdef ST_SOUND
    m_Ym2149 = unique_ptr<CYm2149Ex>(new CYm2149Ex(profileCPC, _emulator.lock()->GetConfig()->snd_playback_rate));
    m_Ym2149->reset();
#endif

    InitAYCounterVars();
}

t_PSG::~t_PSG()
{
}

void t_PSG::Reset()
{
    control = 0;

#ifdef AYLET
#endif

#ifdef AYEMU
    ayemu_reset(&m_ayemu);
#endif

#ifdef ST_SOUND
    m_Ym2149->reset();
#endif

    buffer_full = 0;
}

void t_PSG::Emulate(int iCycleCount)
{
    if(auto emu = _emulator.lock())
    {
        if(auto ap = emu->GetAudioPlugin())
        {
#ifdef ST_SOUND
            cycle_count += iCycleCount;

            if (cycle_count >= snd_cycle_count)
            {
                uint8_t* bufferPtr = ap->getBuffer();
                if(bufferPtr == NULL) return;

                cycle_count -= snd_cycle_count;

                m_Ym2149->updateStereo((ymsample*)bufferPtr, (ymint)1);
                for(unsigned int k = 0; k<sizeof(ymsample)*2; k++)
                {
                    // Add Tape Sound
                    // TODO: Overflow verification
                    // TODO: Configurable
                    *(bufferPtr+k) += emu->GetTape()->GetTapeLevel()/32;
                }

                buffer_full = ap->update();
            }
#endif
        }
    }
}

void t_PSG::fillSample(int nbSample)
{
#ifdef AYLET
    sound_frameExt(nbSample, 1);
    cycle_count = 0;
#endif
}

void t_PSG::SetAYRegister(int Num, byte Value)
{
#ifdef ST_SOUND
    m_Ym2149->writeRegister(Num, Value);
#endif

#ifdef AYLET
    sound_ay_write(Num, Value, cycle_count/(2<<16));
#endif

#ifdef AYEMU
    m_ayemu_reg_frame[Num] = Value ;
    ayemu_set_regs(&m_ayemu, m_ayemu_reg_frame);
#endif
}

void t_PSG::SetEnvDirection(unsigned char dir)
{
    // TODO : add env direction
    //	FirstPeriod = false;

    // up
    if (dir == 0x01) 
    {
	/*		switch (RegisterAY.EnvType)
			{
			case 4:
			case 5:
			case 6:
			case 7:
			case 13:
			case 14:
			case 15:
			{
			FirstPeriod = true;
			break;
			}
			}
	 */
    }
    // down
    else if (dir == 0xff) 
    {
	/*
	   switch (RegisterAY.EnvType)
	   {
	   case 0:
	   case 1:
	   case 2:
	   case 3:
	   case 9:
	   case 10:
	   case 11:
	   {
	   FirstPeriod = true;
	   break;
	   }
	   }
	 */
    }
}

unsigned char t_PSG::GetEnvDirection() const
{
    // TODO
    /*
       if (FirstPeriod)
       {
       switch (RegisterAY.EnvType)
       {
       case 0:
       case 1:
       case 2:
       case 3:
       case 8:
       case 9:
       case 10:
		case 11:
			{
				return 0xff; // down
				break;
			}
		case 4:
		case 5:
		case 6:
		case 7:
		case 12:
		case 13:
		case 14:
		case 15:
			{
				return 0x01; // up
				break;
			}
		}
	} 
	else 
	{
		switch (RegisterAY.EnvType)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 9:
		case 11:
		case 13:
		case 15:
			{
				return 0x00; // hold
				break;
			}
		case 8:
		case 14:
			{
				return 0xff; // down
				break;
			}
		case 10:
		case 12:
			{
				return 0x01; // up
				break;
			}
		}
	}
*/
	return 0x00;
}

void t_PSG::InitAYCounterVars()
{
    auto CPC = _emulator.lock()->GetConfig();
    cycle_count = 0;
    snd_cycle_count = (4000000.0/(double)(CPC->snd_playback_rate)); // number of Z80 cycles per sample
    // std::cout << "Audio cycle count : " << snd_cycle_count << std::endl;
}
