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
	PaError err;

	if (!cpc.snd_enabled) {
		InfoLogMessage("[PortAudio Plugin] Not opening audio because disabled in the config");
		return 0;
	}

	if((err = Pa_Initialize()) != paNoError) {
		ErrorLogMessage("[PortAudio Plugin] Failed to initialize. (PortAudio Message: %s)", Pa_GetErrorText(err));
		return -1;
	}

	PaStreamParameters hostApiOutputParameters;
	PaStreamParameters *hostApiOutputParametersPtr;
    this->cpc = &cpc;
    this->psg = &psg;

    unsigned int rate = cpc.snd_playback_rate;
	int channels = cpc.snd_stereo ? 2 : 1;

	PaSampleFormat format = paInt16;
	switch(cpc.snd_bits)
	{
		case 8:
			format = paUInt8;
			break;
		case 16:
			format = paInt16;
			break;
		case 24:
			format = paInt24;
			break;
		case 32:
			format = paInt32;
			break;
		default:
			WarningLogMessage("[PortAudio Plugin] Warning, %d bits format unknown, fallback to %s.", cpc.snd_bits, format);
	}

	hostApiOutputParameters.device = Pa_GetDefaultOutputDevice();
	hostApiOutputParameters.channelCount = channels;
	hostApiOutputParameters.sampleFormat = format;
//	hostApiOutputParameters.suggestedLatency = Pa_GetDeviceInfo(hostApiOutputParameters.device)->defaultHighOutputLatency;
	hostApiOutputParametersPtr = &hostApiOutputParameters;

#if DEBUG
	if(Pa_GetHostApiCount() > 0)
	{
		DebugLogMessage("[PortAudio Plugin] Number of available API: %d", Pa_GetHostApiCount());
		for(PaHostApiIndex i = 0; i < Pa_GetHostApiCount(); i++)
		{
			const PaHostApiInfo *info = Pa_GetHostApiInfo(i);
			DebugLogMessage("[PortAudio Plugin] API: %d Name: %s.", i, info->name);
		}
	}
	if(Pa_GetDeviceCount() > 0)
	{
		DebugLogMessage("[PortAudio Plugin] Number of available device: %d", Pa_GetDeviceCount());
		for(PaDeviceIndex i = 0; i < Pa_GetDeviceCount(); i++)
		{
			const PaDeviceInfo *info = Pa_GetDeviceInfo(i);
			DebugLogMessage("[PortAudio Plugin] Device: %d Name: %s.", i, info->name);
		}
	}
	DebugLogMessage("[PortAudio Plugin] Default Device: %d.", Pa_GetDefaultOutputDevice());
//	const PaDeviceInfo *info = Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice());
//	DebugLogMessage("[PortAudio Plugin] Default Device Name: %s.", info->name);
#endif

#define SAMPLECOUNT 1 /*CPC.snd_playback_rate * 2*/
    if((err = Pa_OpenDefaultStream(&audioStream, 0/*input*/, 2/*channels*/, paInt16, cpc.snd_playback_rate, 1, NULL, NULL)) != paNoError)
//    if((err = Pa_OpenStream(&audioStream, NULL, hostApiOutputParametersPtr, rate, 1, paNoFlag, NULL, NULL)) != paNoError)
	{
		ErrorLogMessage("[PortAudio Plugin] Could not open audio. (PortAudio Message: %s)", Pa_GetErrorText(err));
		return 1;
	}

	pbSndBuffer = new byte[SAMPLECOUNT]; // allocate the sound data buffer
	if(!pbSndBuffer)
	{
		ErrorLogMessage("[PortAudio Plugin] pbSndBuffer allocation error.");
		return 1;
	}
//	pbSndBufferEnd = pbSndBuffer + SAMPLECOUNT;
//	memset(pbSndBuffer, 0, SAMPLECOUNT);
	pbSndBufferPtr = pbSndBuffer; // init write cursor (1VBL latency, will evolve if there are overflows when reading)
//	pbSndBufferCurrent = pbSndBuffer + SAMPLECOUNT/2;   // init read cursor

        if((err = Pa_StartStream(audioStream)) != paNoError) {
			ErrorLogMessage("[PortAudio Plugin] Could not start stream. (PortAudio Message: %s)", Pa_GetErrorText(err));
		return 1;
	}

	InfoLogMessage("[PortAudio Plugin] Device open.");

	return 0;
}


void PortAudioAudioPlugin::shutdown()
{
    Pa_StopStream(audioStream);
    Pa_Terminate();

	delete [] pbSndBuffer;
	pbSndBuffer = NULL;
}

void PortAudioAudioPlugin::pause()
{
	/// \TODO ...
	if (cpc->snd_enabled) {
	}
}

void PortAudioAudioPlugin::resume()
{
	/// \TODO ...
	if (cpc->snd_enabled) {
	}
}
