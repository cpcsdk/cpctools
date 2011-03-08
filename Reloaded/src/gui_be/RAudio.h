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

#pragma once

#include "audioPlugin.h"

#include <PushGameSound.h>
#include <GameSoundDefs.h>

class BeAudioPlugin : public AudioPlugin
{
    public:
        BeAudioPlugin() {}
        int init(t_CPC& cpc, t_PSG& psg)
		{
			gs_audio_format format;
			format.frame_rate = 44100; // TOOD : use config
			format.channel_count = 2;
			format.format = gs_audio_format::B_GS_F; // float
			format.buffer_size = 0; // auto choose
			soundOutput = new BPushGameSound(1, &format);

			if (soundOutput->InitCheck() != B_OK)
			{
				printf("BPushGameSound init fail\n");
				return -1;
			}

			status_t sta = soundOutput->LockForCyclic((void**)&outBuf, &outSize);

			if (sta == BPushGameSound::lock_failed) {
				printf("unable to lock sound buffer\n");
				return -2;
			}
			return 0;
		}

        void shutdown()
		{
			delete soundOutput;
			soundOutput = NULL;
		}
		
        int update() {return 0;}
        void pause() {}
        void resume() {}
        uint8_t* getBuffer() {return outBuf;}
	private:
		BPushGameSound* soundOutput;
		uint8_t* outBuf;
		size_t outSize;
};
