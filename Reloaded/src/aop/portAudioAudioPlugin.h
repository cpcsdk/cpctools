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

#ifndef _PORTAUDIOAUDIOPLUGIN_H_
#define _PORTAUDIOAUDIOPLUGIN_H_

#include "../core/audioPlugin.h"

class PortAudioAudioPlugin : public AudioPlugin
{
    public:
        PortAudioAudioPlugin();
        int init(shared_ptr<t_CPC> cpc, shared_ptr<t_PSG> psg);
        void shutdown();
        inline int update();
        void pause();
        void resume();
        inline uint8_t* getBuffer() {return pbSndBufferPtr;}
    private:
        uint8_t *pbSndBuffer;
        uint8_t *pbSndBufferPtr;
};

#endif /* #ifndef _PORTAUDIOAUDIOPLUGIN_H_ */
