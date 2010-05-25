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
    if (pbSndBufferCurrent+len<pbSndBufferEnd)
    {
        if (pbSndBufferCurrent+len>pbSndBufferPtr && pbSndBufferCurrent<pbSndBufferPtr)
        {
//            std::cout << "Sound buffer reading overflow ! Available " << ((int)(pbSndBufferCurrent-pbSndBufferPtr)+len) << ", expecting " << len << std::endl;
	    pbSndBufferCurrent -= len; // Better make a big desync than many little clicks
	    // This adds latency (space between reader and writer) but it will max out at the buffer size
	    if(pbSndBufferCurrent < pbSndBuffer) pbSndBufferCurrent = pbSndBuffer;
        }

        memcpy(stream, pbSndBufferCurrent, len);
        pbSndBufferCurrent = pbSndBufferCurrent+len;
    }
    else
    {
        int rest = pbSndBufferEnd - pbSndBufferCurrent;
        memcpy(stream, pbSndBufferCurrent, rest);
        pbSndBufferCurrent = pbSndBuffer;
        memcpy(stream+rest, pbSndBufferCurrent, len-rest);
        pbSndBufferCurrent+=len-rest;

        if (pbSndBufferCurrent>pbSndBufferPtr)
        {
            std::cout << "Sound buffer reading overflow (Cycling) ! Available " << (pbSndBufferCurrent-pbSndBufferPtr)/4 << ", expecting " << len/4 << std::endl;
        }
    }
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
		return 0;
	}

	if (Pa_Initialize() != paNoError) return -1;
	PaStreamParameters psp;

	psp.device = Pa_GetDefaultOutputDevice();
	if (psp.device == paNoDevice) return -2;

	psp.channelCount = 2;
	psp.suggestedLatency = Pa_GetDeviceInfo(psp.device)->defaultLowOutputLatency;
	psp.hostApiSpecificStreamInfo = NULL;
	psp.sampleFormat = paInt16;

	PaStream* stream;
	if (Pa_OpenStream(&stream, NULL /* no input */, &psp, CPC.snd_playback_rate, CPC.snd_playback_rate/50, paClipOff, audio_update, &psg) != paNoError)
	{
		fprintf(stderr, "Could not open audio\n");
		return 1;
	}
	
//	CPC.snd_buffersize = (audio_spec->size/audio_spec->channels)/2; // size in samples : desired number of sample per 20ms
	// The multiplicator here (3) defines the latency. Lower is better. Best should be 1
#define SND_LATENCY 3
#define SAMPLECOUNT CPC.snd_playback_rate*SND_LATENCY/50
	pbSndBuffer = (byte *)malloc(SAMPLECOUNT); // allocate the sound data buffer
	pbSndBufferEnd = pbSndBuffer + SAMPLECOUNT;
	memset(pbSndBuffer, 0, SAMPLECOUNT);
	pbSndBufferPtr = pbSndBuffer+CPC.snd_playback_rate/50; // init write cursor (1VBL latency, will evolve if there are overflows when reading)
	pbSndBufferCurrent = pbSndBuffer;   // init read cursor

    //std::cout << "Audio_spec size : " << audio_spec->size << std::endl;
	
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
