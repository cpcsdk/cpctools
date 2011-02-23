/**
 * Reloaded, an Amstrad CPC emulator
 * Copyright (C) 2010,2011  cpcsdk crew
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "portAudioAudioPlugin.h"
#include <portaudio.h>

#include "emulator.h"

#include "log.h"

PaStream* audioStream = NULL;

//uint8_t *pbSndBuffer = NULL;
//*pbSndBuffer = NULL;
// byte *pbSndBufferEnd = NULL;
// byte *pbSndBufferCurrent = NULL;    // current position in the reading sample
//uint8_t *pbSndBufferPtr = NULL;    // current position in the writing sample
//*pbSndBufferPtr = NULL;    // current position in the writing sample
// dword dwSndBufferCopied;

PortAudioAudioPlugin::PortAudioAudioPlugin() :
    pbSndBuffer(NULL),
    pbSndBufferPtr(NULL)
{
}

inline int PortAudioAudioPlugin::update()
{
    return Pa_WriteStream(audioStream,pbSndBuffer,1) == paNoError ? 0 : -1;
}

int PortAudioAudioPlugin::init(t_CPC& cpc, t_PSG& psg)
{
    this->cpc = &cpc;
    this->psg = &psg;

	if (!cpc.snd_enabled) {
		InfoLogMessage("Not opening audio because it is disabled in the config");
		return 0;
	}

	if (Pa_Initialize() != paNoError) {
		ErrorLogMessage("Failed to initialize portaudio");
		return -1;
	}
#define SAMPLECOUNT 1 /*CPC.snd_playback_rate * 2*/
    if (Pa_OpenDefaultStream(&audioStream, 0/*input*/, 2/*channels*/, paInt16, cpc.snd_playback_rate, 1, NULL, NULL) != paNoError)
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
}


void PortAudioAudioPlugin::shutdown()
{
    Pa_StopStream(audioStream);
    Pa_Terminate();

    free(pbSndBuffer);
}

void PortAudioAudioPlugin::pause()
{
	/// \TODO ...
	if (cpc->snd_enabled) {
		//SDL_PauseAudio(1);
	}
}

void PortAudioAudioPlugin::resume()
{
	/// \TODO ...
	if (cpc->snd_enabled) {
		//SDL_PauseAudio(0);
	}
}
