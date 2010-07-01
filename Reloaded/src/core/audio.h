//
// Caprice32 audio emulator
//

#ifndef _AUDIO_H_
#define _AUDIO_H_

#define MAX_FREQ_ENTRIES 5

#include "cap32type.h"

class t_CPC;
class t_PSG;

//extern dword dwSndBufferCopied;
extern byte *pbSndBuffer;
// extern byte *pbSndBufferEnd;
extern byte *pbSndBufferPtr;

// int audio_update (const void* inbuf, void* outbuf, unsigned long len, const PaStreamCallbackTimeInfo* sci, PaStreamCallbackFlags scf, void *userdata);
// int audio_align_samples (int given);
int audio_init (t_CPC &CPC, t_PSG* psg);
void audio_shutdown (void);
void audio_pause (t_CPC &CPC);
void audio_resume (t_CPC &CPC);

#endif
