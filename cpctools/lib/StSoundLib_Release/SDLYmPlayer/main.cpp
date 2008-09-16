/* SDLYmPlayer
Copyright (C) 1995-1999 Arnaud Carre ( http://leonard.oxg.free.fr )
Copyright (C) 2007 Bertrand Jouin (SDL port)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <iostream>
#include <iomanip>
#include "SDL.h"
#include "../StSoundLibrary/StSoundLibrary.h"

static void audio_update (void *userdata, Uint8 *stream, int len)
{
	if (userdata)
	{
		int nbSample = len / sizeof(ymsample);
		ymMusicCompute(userdata,(ymsample*)stream,nbSample);
	}
}

int main (int argc, char **argv)
{
	// initialize SDL
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0)
	{
		std::cerr << "SDL_Init() failed: " << SDL_GetError() << std::endl;
		return -1;
	}

	//--------------------------------------------------------------------------
	// Checks args.
	//--------------------------------------------------------------------------
	std::cout << "SDLYmPlayer.\n"
                 "Using ST-Sound Library, under GPL license\n"
                 "Copyright (C) 1995-1999 Arnaud Carre ( http://leonard.oxg.free.fr )\n"
                 "Copyright (C) 2007 Bertrand Jouin (SDL port)\n" << std::endl;

	if (argc!=2)
	{
		std::cout << "Usage: SDLYmPlayer <ym music file>" << std::endl;
		return -1;
	}

	//--------------------------------------------------------------------------
	// Load YM music and creates WAV file
	//--------------------------------------------------------------------------
	std::cout << "Loading music \"" << argv[1] << "\"..." << std::endl;
	YMMUSIC *pMusic = ymMusicCreate();

	if (ymMusicLoad(pMusic,argv[1]))
	{
        SDL_AudioSpec *desired = (SDL_AudioSpec *)malloc(sizeof(SDL_AudioSpec));
        SDL_AudioSpec *obtained = (SDL_AudioSpec *)malloc(sizeof(SDL_AudioSpec));

        desired->freq = 44100;
        desired->format = AUDIO_S16LSB;
        desired->channels = 1;
        desired->samples = desired->freq;
        desired->callback = audio_update;
        desired->userdata = pMusic;

        if (SDL_OpenAudio(desired, obtained) < 0)
        {
            std::cerr << "Could not open audio: " << SDL_GetError() << std::endl;
            return -1;
        }

        free(desired);

        ymMusicInfo_t info;
        ymMusicGetInfo(pMusic,&info);
        std::cout << "Name.....: " << info.pSongName << std::endl;
        std::cout << "Author...: " << info.pSongAuthor << std::endl;
        std::cout << "Comment..: " << info.pSongComment << std::endl;
        std::cout << "Duration.: " << info.musicTimeInSec/60 << std::setw(2) << info.musicTimeInSec%60 << std::endl;

        std::cout << "\nPlaying music...(Ctrl C to abort)" << std::endl;

        ymMusicSetLoopMode(pMusic,YMTRUE);
        ymMusicPlay(pMusic);

        int oldSec = -1;
        SDL_Event event;
        bool running = true;

        SDL_PauseAudio(0);

        while(running)
        {
            while(SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_QUIT:
                        running = false;
                        break;
                }
            }

            if (pMusic)
            {
                int sec = ymMusicGetPos(pMusic) / 1000;
                if (sec != oldSec)
                {
                    std::cout << "Time: " << sec/60 << ':' << std::setw(2) << std::setfill('0') << sec%60 << '\r' << std::flush;
                    oldSec = sec;
                }
            }
            SDL_Delay(50);
        }

        SDL_PauseAudio(1);

        std::cout << std::endl;

        // Switch off replayer
        ymMusicStop(pMusic);
	}
	else
	{
        std::cout << "Error in loading file " << argv[1] << ":" << ymMusicGetLastError(pMusic) << std::endl;
	}

	ymMusicDestroy(pMusic);

    SDL_Quit();
    return 0;
}
