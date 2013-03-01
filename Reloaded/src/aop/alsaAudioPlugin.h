/**
 * Reloaded, an Amstrad CPC emulator
 * Copyright (C) 2011  cpcsdk crew
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

#ifndef _ALSAAUDIOPLUGIN_H_
#define _ALSAAUDIOPLUGIN_H_

#include "audioPlugin.h"
#include <alsa/asoundlib.h>
#include <pthread.h>

typedef struct _thread_param
{
    uint8_t* sndBuffer;
    uint8_t** sndBufferPtr;
    uint32_t* qte;
    snd_pcm_t *pcm_handle;          
} thread_param;

class AlsaAudioPlugin : public AudioPlugin
{
    public:
        AlsaAudioPlugin();
        int init(shared_ptr<t_CPC> cpc, shared_ptr<t_PSG> psg);
        void shutdown() {}
        int update();
        void pause() {}
        void resume() {}
        uint8_t* getBuffer();
		void lock();
		void unlock();
    private:
        int periodsize;
        snd_pcm_t *pcm_handle;          
        uint8_t *sndBuffer;
        uint8_t *sndBufferPtr;
        uint32_t qte;
        pthread_t* thread;
        thread_param param_thread;
};

#endif /* #ifndef _ALSAAUDIOPLUGIN_H_ */
