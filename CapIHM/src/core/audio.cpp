/**
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
// Caprice32 audio emulator
//

#include "audio.h"
#include <string.h>
#include "config.h"
#include "psg.h"

#include <portaudio.h>
#include <iostream>

byte *pbSndBuffer = NULL;
byte *pbSndBufferEnd = NULL;
byte *pbSndBufferCurrent = NULL;    // current position in the reading sample
byte *pbSndBufferPtr = NULL;    // current position in the writing sample
dword dwSndBufferCopied;

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
    memcpy(stream, pbSndBuffer, 4*len);

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

	// TODO : the code below should be more efficient (avoids memcpying), but it makes the sound crackle and pop a lot more...
	/*
    if (pbSndBufferCurrent+4*len<pbSndBufferEnd)
    {
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
        int rest = pbSndBufferEnd - pbSndBufferCurrent;
        memcpy(stream, pbSndBufferCurrent, rest);
        pbSndBufferCurrent = pbSndBuffer;
        memcpy(stream+rest, pbSndBufferCurrent, 4*len-rest);
        pbSndBufferCurrent+=4*len-rest;
	}
	*/
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


int audio_init (t_CPC &CPC, t_PSG* psg)
{
	if (!CPC.snd_enabled) {
		fprintf(stderr, "Not opening audio because it is disabled in the config\n");
		return 0;
	}

	if (Pa_Initialize() != paNoError) {
		fprintf(stderr, "Failed to initialize portaudio\n");
		return -1;
	}
	PaStream* stream;
	if (Pa_OpenDefaultStream(&stream, 0/*input*/, 2/*channels*/, paInt16, CPC.snd_playback_rate, 0, audio_update, &psg) != paNoError)
	{
		fprintf(stderr, "Could not open audio\n");
		return 1;
	}
	
	// The multiplicator here (3) defines the latency. Lower is better. Best should be 1
#define SAMPLECOUNT CPC.snd_playback_rate * 2
	pbSndBuffer = (byte *)malloc(SAMPLECOUNT); // allocate the sound data buffer
	pbSndBufferEnd = pbSndBuffer + SAMPLECOUNT;
	memset(pbSndBuffer, 0, SAMPLECOUNT);
	pbSndBufferPtr = pbSndBuffer+CPC.snd_playback_rate/50; // init write cursor (1VBL latency, will evolve if there are overflows when reading)
	pbSndBufferCurrent = pbSndBuffer;   // init read cursor

	if(Pa_StartStream(stream) != paNoError) {
		fprintf(stderr, "Could not start stream\n");
		return 1;
	}
	
	fprintf(stderr, "Audio init ok..\n");
	return 0;
}


void audio_shutdown (void)
{
	Pa_Terminate();

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
