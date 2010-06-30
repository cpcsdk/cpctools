/**
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
// Caprice32 audio emulator
//

#include "audio.h"
#include <string.h>
#include "config.h"
#include "psg.h"
#include "emulator.h"
#include "log.h"
#include <iostream>

#if SOUND_OUTPUT == SOUND_OUTPUT_PortAudio
#include <portaudio.h>

PaStream* audioStream = NULL;
#endif

byte *pbSndBuffer = NULL;
// byte *pbSndBufferEnd = NULL;
// byte *pbSndBufferCurrent = NULL;    // current position in the reading sample
byte *pbSndBufferPtr = NULL;    // current position in the writing sample
// dword dwSndBufferCopied;


#if 0
int audio_update (const void* inbuf, void* outbuf, unsigned long len, const PaStreamCallbackTimeInfo* sci, PaStreamCallbackFlags scf, void *userdata)
{
	int16_t* stream = (int16_t*)outbuf;
#ifdef AYEMU
    ayemu_gen_sound ( (ayemu_ay_t*)userdata, stream, len);
    return;
#endif // AYEMU

#ifdef AYLET
    t_PSG *psg = (t_PSG*) userdata;
    if (audio_spec->channels==1)
    {
        psg->fillSample(len/(2*1));
    }
    else if (audio_spec->channels==2)
    {
        psg->fillSample(len/(2*2));
    }
    else
    {
        std::cerr << "Unknwon channel number ! No sound" << std::endl;
        return;
    }
#endif

#ifdef CLASSIC_SOUND
    memcpy(stream, pbSndBuffer, len);
    dwSndBufferCopied = 1;
#else
	// StSound
	// len unit is 'frame'. A frame is 2 channels * 2 bytes = 4 bytes.
	// hence the 4*len in this copy (because our buffers are byte-sized)
// Dummy way
	// memcpy(stream, pbSndBuffer, 4*len);
// A bit more clever
#if 0
	if(pbSndBufferPtr>pbSndBuffer+4*len)
	{
		// StSound generated more than expected !
		// Copy the excedental data to the beginning of buffer
		memcpy(pbSndBuffer, pbSndBuffer+4*len, pbSndBufferPtr - pbSndBuffer+4*len);
		pbSndBufferPtr -= 4*len;	
	} else {
		// StSound is late ! align it back for the next round
		pbSndBufferPtr = pbSndBuffer;
	}
#else
//The best one
	// TODO : the code below should be more efficient (avoids memcpying), but it makes the sound crackle and pop a lot more...
    if (pbSndBufferCurrent+4*len<pbSndBufferEnd)
    {
		// The data we need is between pbSndBufferCurrent and +4*len, fine
        if (pbSndBufferCurrent+4*len>pbSndBufferPtr && pbSndBufferCurrent<pbSndBufferPtr)
        {
			// A read overflow occured : the emu was too slow and didn't generate enough samples
	    	pbSndBufferCurrent -= 4*len; // Better make a big desync than many little clicks
	    	// This adds latency (space between reader and writer) but it will max out at the buffer size
	    	if(pbSndBufferCurrent < pbSndBuffer) pbSndBufferCurrent = pbSndBuffer;
        }
        memcpy(stream, pbSndBufferCurrent, 4*len);
        pbSndBufferCurrent = pbSndBufferCurrent+4*len;
    }
    else
    {
		// We are looping the buffer
        int rest = pbSndBufferEnd - pbSndBufferCurrent;
        memcpy(stream, pbSndBufferCurrent, rest);
        pbSndBufferCurrent = pbSndBuffer;
        memcpy(stream+rest, pbSndBufferCurrent, 4*len-rest);
        pbSndBufferCurrent+=4*len-rest;
	}
#endif
#endif
	return paContinue;
}

int audio_align_samples (int given)
{
	int actual = 1;
	while (actual < given) {
		actual <<= 1;
	}
	return actual; // return the closest match as 2^n
}
#endif

int audio_init (t_CPC &CPC, t_PSG* psg)
{
#if SOUND_OUTPUT == SOUND_OUTPUT_PortAudio
	if (!CPC.snd_enabled) {
		InfoLogMessage("Not opening audio because it is disabled in the config");
		return 0;
	}

	if (Pa_Initialize() != paNoError) {
		ErrorLogMessage("Failed to initialize portaudio");
		return -1;
	}
#define SAMPLECOUNT 1 /*CPC.snd_playback_rate * 2*/
//	if (Pa_OpenDefaultStream(&audioStream, 0/*input*/, 2/*channels*/, paInt16, CPC.snd_playback_rate, SAMPLECOUNT/8, audio_update, NULL) != paNoError)
    if (Pa_OpenDefaultStream(&audioStream, 0/*input*/, 2/*channels*/, paInt16, CPC.snd_playback_rate, 1, NULL, NULL) != paNoError)
	{
		ErrorLogMessage("Could not open audio");
		return 1;
	}
	
	pbSndBuffer = (byte *)malloc(SAMPLECOUNT); // allocate the sound data buffer
//	pbSndBufferEnd = pbSndBuffer + SAMPLECOUNT;
//	memset(pbSndBuffer, 0, SAMPLECOUNT);
	pbSndBufferPtr = pbSndBuffer; // init write cursor (1VBL latency, will evolve if there are overflows when reading)
//	pbSndBufferCurrent = pbSndBuffer + SAMPLECOUNT/2;   // init read cursor

        if(Pa_StartStream(audioStream) != paNoError) {
			ErrorLogMessage("Could not start stream");
		return 1;
	}
	
	return 0;
#else
    InfoLogMessage("Not opening audio because compile with no sound library output");
    return 0;
#endif
}


void audio_shutdown (void)
{
#if SOUND_OUTPUT == SOUND_OUTPUT_PortAudio
    Pa_StopStream(audioStream);
    Pa_Terminate();
#endif

    free(pbSndBuffer);
}

void audio_pause (t_CPC &CPC)
{
	// TODO ...
	if (CPC.snd_enabled) {
		//SDL_PauseAudio(1);
	}
}

void audio_resume (t_CPC &CPC)
{
	// TODO ...
	if (CPC.snd_enabled) {
		//SDL_PauseAudio(0);
	}
}
