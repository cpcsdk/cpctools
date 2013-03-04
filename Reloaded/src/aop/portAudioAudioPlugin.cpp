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

#include "core/emulator.h"

#include "core/psg.h"
#include "misc/log.h"

//#define BLOCK_IO 1

static int CallbackWrapper(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	PortAudioAudioPlugin * obj = reinterpret_cast<PortAudioAudioPlugin*>(userData);
	return obj->pa_callback(input, output, frameCount, timeInfo, statusFlags);
}

int PortAudioAudioPlugin::pa_callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
	uint32_t * out = (uint32_t*)output;

	buffer_lock.lock();

	if(frameCount > buffer.size())
	{
		InfoLogMessage("[PortAudio Plugin] Buffer underrun");
#if 0
		// TODO: A more correct buffer underrun fallback
		int size = buffer.size();
		for(int i = 0; i < size; i++)
		{
			*out++ = buffer.front();
			buffer.pop();
		}
		for(int j = 0; j < (frameCount - size); j++)
		{
			*out++ = *(((uint32_t*)output) + j);
		}
#endif
	}
	else
	{
		for(int i = 0; i < frameCount; i++)
		{
			*out++ = buffer.front();
			buffer.pop();
		}
	}

	buffer_lock.unlock();

	return paContinue;
}

PortAudioAudioPlugin::PortAudioAudioPlugin() :
	audioStream(NULL),
	sndBuffer(NULL),
	sndBufferPtr(NULL)
{
}

#if BLOCK_IO
inline int PortAudioAudioPlugin::update()
{
#if 0
	return Pa_WriteStream(audioStream, sndBuffer, 1) == paNoError ? 0 : -1;
#else
	sndBufferPtr += sample_size;
	
	if((sndBufferPtr - sndBuffer) > sample_count*sample_size)
	{
		sndBufferPtr = sndBuffer;
		return Pa_WriteStream(audioStream, sndBuffer, sample_count) == paNoError ? 0 : -1;
	}

	return 0;
#endif
}
#else
inline int PortAudioAudioPlugin::update()
{
	sndBufferPtr += sample_size;
	if((sndBufferPtr - sndBuffer) > sample_count*sample_size)
	{
		sndBufferPtr = sndBuffer;
	
		buffer_lock.lock();
		for(int i = 0; i < sample_count; i++)
		{
			buffer.push(*((uint32_t*)sndBuffer + i));
		}
		int size = buffer.size();
		buffer_lock.unlock();
		return size > 2*sample_count ? 1 : 0;
	}

	return buffer.size() > 2*sample_count ? 1 : 0;
}
#endif

int PortAudioAudioPlugin::init(shared_ptr<t_CPC> cpc, shared_ptr<t_PSG> psg)
{
	PaError err;

	if (!cpc->snd_enabled) {
		InfoLogMessage("[PortAudio Plugin] Not opening audio because disabled in the config");
		return 0;
	}

	if((err = Pa_Initialize()) != paNoError) {
		ErrorLogMessage("[PortAudio Plugin] Failed to initialize. (PortAudio Message: %s)", Pa_GetErrorText(err));
		return -1;
	}

	PaStreamParameters hostApiOutputParameters;
	PaStreamParameters *hostApiOutputParametersPtr;
	this->cpc = cpc;
	this->psg = psg;

	sample_count = cpc->snd_playback_rate / 50;

	unsigned int rate = cpc->snd_playback_rate;
	int channels = cpc->snd_stereo ? 2 : 1;

	PaSampleFormat format = paInt16;
	switch(cpc->snd_bits)
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
			WarningLogMessage("[PortAudio Plugin] Warning, %d bits format unknown, fallback to %s.", cpc->snd_bits, format);
			cpc->snd_bits = 16;
	}

	sample_size = (cpc->snd_bits * channels) / 8;

	hostApiOutputParameters.device = Pa_GetDefaultOutputDevice();
	hostApiOutputParameters.channelCount = channels;
	hostApiOutputParameters.sampleFormat = format;
//	hostApiOutputParameters.suggestedLatency = Pa_GetDeviceInfo(hostApiOutputParameters.device)->defaultHighOutputLatency;
	hostApiOutputParametersPtr = &hostApiOutputParameters;

#ifndef NDEBUG
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

#if BLOCK_IO
	if((err = Pa_OpenDefaultStream(&audioStream, 0/*input*/, 2/*channels*/, paInt16, rate, 1, NULL, NULL)) != paNoError)
#else
	if((err = Pa_OpenDefaultStream(&audioStream, 0/*input*/, 2/*channels*/, paInt16, rate, sample_count, &CallbackWrapper, this)) != paNoError)
#endif
//    if((err = Pa_OpenStream(&audioStream, NULL, hostApiOutputParametersPtr, rate, 1, paNoFlag, NULL, NULL)) != paNoError)
	{
		ErrorLogMessage("[PortAudio Plugin] Could not open audio. (PortAudio Message: %s)", Pa_GetErrorText(err));
		return 1;
	}

	sndBuffer = new byte[sample_size*sample_count]; // allocate the sound data buffer
	if(!sndBuffer)
	{
		ErrorLogMessage("[PortAudio Plugin] pbSndBuffer allocation error.");
		return 1;
	}
//	pbSndBufferEnd = pbSndBuffer + SAMPLECOUNT;
//	memset(pbSndBuffer, 0, SAMPLECOUNT);
	sndBufferPtr = sndBuffer; // init write cursor (1VBL latency, will evolve if there are overflows when reading)
//	pbSndBufferCurrent = pbSndBuffer + SAMPLECOUNT/2;   // init read cursor

	if((err = Pa_StartStream(audioStream)) != paNoError)
	{
		ErrorLogMessage("[PortAudio Plugin] Could not start stream. (PortAudio Message: %s)", Pa_GetErrorText(err));
		return 1;
	}

	PaStreamInfo const * info = Pa_GetStreamInfo(audioStream);
	InfoLogMessage("[PortAudio Plugin] Reported Output Latency: %f", info->outputLatency);
	InfoLogMessage("[PortAudio Plugin] Reported real SampleRate: %f", info->sampleRate);

	InfoLogMessage("[PortAudio Plugin] Device open.");

	return 0;
}


void PortAudioAudioPlugin::shutdown()
{
    Pa_StopStream(audioStream);
    Pa_Terminate();

	delete [] sndBuffer;
	sndBuffer = NULL;
}

void PortAudioAudioPlugin::pause()
{
	/// \TODO ...
	if (cpc.lock()->snd_enabled) {
	}
}

void PortAudioAudioPlugin::resume()
{
	/// \TODO ...
	if (cpc.lock()->snd_enabled) {
	}
}
