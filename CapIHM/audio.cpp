//
// Caprice32 audio emulator
//

#include "audio.h"
#include <string.h>
#include <SDL_audio.h>
#include "config.h"
#include "psg.h"
#include <SDL.h>

#include <iostream>

byte *pbSndBuffer = NULL;
byte *pbSndBufferEnd = NULL;
byte *pbSndBufferCurrent = NULL;    // current position in the reading sample
byte *pbSndBufferPtr = NULL;    // current position in the writing sample
dword dwSndBufferCopied;

SDL_AudioSpec *audio_spec = NULL;

dword freq_table[MAX_FREQ_ENTRIES] = {
	11025,
	22050,
	44100,
	48000,
	96000
};

void audio_update (void *userdata, Uint8 *stream, int len)
{
    t_PSG *psg = (t_PSG*) userdata;
#ifdef AYEMU
    ayemu_gen_sound ( (ayemu_ay_t*)userdata, stream, len);
    return;
#endif // AYEMU

#ifdef AYLET
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
//    std::cout << "Reading sample of length : " << len/4 << " at " << SDL_GetTicks() << std::endl;
    if (pbSndBufferCurrent+len<pbSndBufferEnd)
    {
//        if (pbSndBufferCurrent+len>pbSndBufferPtr)
//        {
//            std::cout << "Sound buffer reading overflow ! Available " << (pbSndBufferCurrent+len-pbSndBufferPtr)/4 << ", expecting " << len/4 << std::endl;
//        }

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

//        if (pbSndBufferCurrent>pbSndBufferPtr)
//        {
//            std::cout << "Sound buffer reading overflow (Cycling) ! Available " << (pbSndBufferCurrent-pbSndBufferPtr)/4 << ", expecting " << len/4 << std::endl;
//        }
    }
#endif
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
	
	SDL_AudioSpec *desired = (SDL_AudioSpec *)malloc(sizeof(SDL_AudioSpec));
	SDL_AudioSpec *obtained = (SDL_AudioSpec *)malloc(sizeof(SDL_AudioSpec));
	
	desired->freq = 44100;
	desired->format = AUDIO_S16LSB;
	desired->channels = 2;
	desired->samples = audio_align_samples(desired->freq / 50); // desired is 20ms at the given frequency
	desired->callback = audio_update;
	desired->userdata = &psg;
	
	if (SDL_OpenAudio(desired, obtained) < 0)
	{
		fprintf(stderr, "Could not open audio: %s\n", SDL_GetError());
		return 1;
	}

	printf("Audio rate obtained : %d\n",obtained->freq);
	
	free(desired);
	audio_spec = obtained;
	
//	CPC.snd_buffersize = (audio_spec->size/audio_spec->channels)/2; // size in samples : desired number of sample per 20ms
	pbSndBuffer = (byte *)malloc(audio_spec->size*2); // allocate the sound data buffer
	pbSndBufferEnd = pbSndBuffer + audio_spec->size*2;
	memset(pbSndBuffer, audio_spec->silence, audio_spec->size*2);
	pbSndBufferPtr = pbSndBuffer; // init write cursor
	pbSndBufferCurrent = pbSndBuffer;   // init read cursor

    //std::cout << "Audio_spec size : " << audio_spec->size << std::endl;
	
	return 0;
}


void audio_shutdown (void)
{
	SDL_CloseAudio();

	if (audio_spec) {
		free(audio_spec);
	}
	if (pbSndBuffer) {
		free(pbSndBuffer);
	}
}

void audio_pause (t_CPC &CPC)
{
	if (CPC.snd_enabled) {
		SDL_PauseAudio(1);
	}
}

void audio_resume (t_CPC &CPC)
{
	if (CPC.snd_enabled) {
		SDL_PauseAudio(0);
	}
}
