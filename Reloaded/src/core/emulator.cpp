/*
 *   $Id$
 *   Reloded an Amstrad CPC emulator
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
// Caprice32 Emulator class
//

#include "emulator.h"

#include "crtc.h"
#include "crtc1.h"
#include "tape.h"
#include "video.h"
#include "z80.h"
#include "psg.h"
#include "gatearray.h"
#include "memory.h"
#include "ppi.h"
#include "vdu.h"
#include "render.h"

#include "audioPlugin.h"

#include "dsk.h"
#include "snapshot.h"
#include "input.h"

#include "error.h"

#include "misc/log.h"

//#include "filetools.h"

#include "IMG_savepng.h"

#include <memory>
#include <iostream>

#include <cassert>

#ifndef _WIN32
#include <unistd.h>
#endif

using std::make_shared;

struct null_deleter
{
    void operator()(void const *) const
    { //do nothing!
    }
};

#define VERSION_STRING "v5.0.0"

dword dwMF2ExitAddr;
dword dwMF2Flags = 0;
byte *pbMF2ROMbackup = NULL;
byte *pbMF2ROM = NULL;

byte *pbGPBuffer = NULL;

FILE *pfoPrinter;

#ifdef USE_DEBUGGER
dword dwDebugFlag = 0;
FILE *pfoDebug;
#endif

Emulator* Emulator::instance = NULL;
bool Emulator::sInitOnce = false;


void InitDebug()
{
#ifdef USE_DEBUGGER
	pfoDebug = fopen("./debugger.txt", "w");
#endif
}

void Emulator::setCRTC(shared_ptr<t_CRTC> newCRTC) {
    this->Pause();
    _crtc = newCRTC;
    _crtc->Reset();
    this->Run();
}

void Emulator::emulator_reset(bool bolMF2Reset)
{
	// Z80
	GetZ80().reset();

	// CPC
	_config->cycle_count = CYCLE_COUNT_INIT;
	_config->tape_motor = 0;
	_config->tape_play_button = 0;
	_config->printer_port = 0xff;

	_input->Reset();

	// VDU
	//_vdu->Reset();

	// CRTC
	//_crtc->Reset();

	_gateArray->Reset();
	_cpcMemory->ga_init_banking();

	_ppi->Reset();
	_psg->Reset();
	_fdc->Reset();
	_cpcMemory->Reset(bolMF2Reset);
	_cpcMemory->InitMemConfig();

	// Multiface 2
	dwMF2Flags = 0;
	// clear MF2 return address
	dwMF2ExitAddr = 0xffffffff;
	if ((pbMF2ROM) && (pbMF2ROMbackup))
	{
		// copy the MF2 ROM to its proper place
		memcpy(pbMF2ROM, pbMF2ROMbackup, 8192);
	}

	goToAddress = -1 ;
}

bool Emulator::MF2Init()
{
	FILE *pfileObject;

	char chPath[_MAX_PATH + 1];

	// Multiface 2 enabled?
	if (_config->mf2)
	{
		if (!pbMF2ROM)
		{
			// allocate the space needed for the Multiface 2: 8K ROM + 8K RAM
			pbMF2ROM = new byte [16384];
			// allocate the space needed for the backup of the MF2 ROM
			pbMF2ROMbackup = new byte [8192];
			if ((!pbMF2ROM) || (!pbMF2ROMbackup))
			{
			    delete [] pbMF2ROM;
			    pbMF2ROM = NULL;
			    delete [] pbMF2ROMbackup;
			    pbMF2ROMbackup = NULL;
			    ErrorLogMessage("Emulator::MF2Init(): Out of memory !");
				return false;
			}

			// clear memory
			memset(pbMF2ROM, 0, 16384);
			strncpy(chPath, _config->rom_path, sizeof(chPath)-2);
			strcat(chPath, "/");
			// combine path and file name
			strncat(chPath, _config->rom_mf2, sizeof(chPath)-1 - strlen(chPath));

			// attempt to open the ROM image
			if ((pfileObject = fopen(chPath, "rb")) != NULL)
			{
				fread(pbMF2ROMbackup, 8192, 1, pfileObject);

				// does it have the required signature?
				if (memcmp(pbMF2ROMbackup+0x0d32, "MULTIFACE 2", 11) != 0)
				{
					ErrorLogMessage("ERROR: The file selected as the MF2 ROM is either corrupt or invalid.");
					delete [] pbMF2ROMbackup;
					pbMF2ROMbackup = NULL;
					delete [] pbMF2ROM;
					pbMF2ROM = NULL;
					_config->rom_mf2[0] = 0;
					// disable MF2 support
					_config->mf2 = 0;
				}
				fclose(pfileObject);
			}
			else
			{
				// error opening file
				ErrorLogMessage("ERROR: The file selected as the MF2 ROM is either corrupt or invalid.");
				delete [] pbMF2ROMbackup;
				pbMF2ROMbackup = NULL;
				delete [] pbMF2ROM;
				pbMF2ROM = NULL;
				_config->rom_mf2[0] = 0;
				// disable MF2 support
				_config->mf2 = 0;
			}
		}
	}

	return true;
}

// Non Thread Safe
void Emulator::emulator_shutdown()
{
	delete [] pbMF2ROMbackup;
	pbMF2ROMbackup = NULL;
	delete [] pbMF2ROM;
	pbMF2ROM = NULL;

	delete [] pbGPBuffer;
	pbGPBuffer = NULL;

	_cpcMemory->Clean();
	_crtc = shared_ptr<t_CRTC>();
}

int Emulator::printer_start()
{
	if (!pfoPrinter)
	{
		if(!(pfoPrinter = fopen(_config->printer_file, "wb")))
		{
			// failed to open/create file
			return 0;
		}
	}
	// ready to capture printer output
	return 1;
}

void Emulator::printer_stop()
{
	if (pfoPrinter)
	{
		fclose(pfoPrinter);
	}
	pfoPrinter = NULL;
}

Emulator::Emulator(shared_ptr<VideoPlugin> video, shared_ptr<AudioPlugin> audio):
    this_(this, null_deleter()),
	_config(make_shared<t_CPC>(this_)),
	_renderer(_config->vid_bpp),
    _input(make_shared<t_Input>()),
	_vdu(make_shared<t_VDU>(_renderer)),
	_crtc(NULL),
	_fdc(make_shared<t_FDC>(_config)),
    _ppi(make_shared<t_PPI>()),
	_tape(make_shared<t_Tape>(_config)),
	_psg(make_shared<t_PSG>(_config)),
	_cpcMemory(make_shared<t_Memory>(_config)),
	_z80(make_shared<t_z80regs>(this_)),
	_gateArray(make_shared<t_GateArray>(_renderer, _z80)),
    _audioPlugin(audio),
	FPSDisplay(false),
	exitRequested(false),
	_isInit(false),
	_videoPlugin(video)
{
    emuSync.lock();

	setCRTC(make_shared<t_CRTC>(_gateArray, _vdu));

    emuSync.unlock();

	sem_init(&breakpointLock, 0, 0);
}

Emulator::~Emulator()
{
    exitRequested=1;
    emuSync.lock();
#ifdef USE_PTHREAD
    pthread_join(emuthread, NULL);
#endif

	printer_stop();
	//TODO use lib765 for ejecting floppies
	//#ifndef HAVE_LIB765_H
	//	dsk_eject(&_driveA);
	//	dsk_eject(&_driveB);
	//#endif
	_tape->tape_eject();

	emulator_shutdown();

    _audioPlugin->shutdown();

#ifdef USE_DEBUGGER
	if (pfoDebug) {
		fclose(pfoDebug);
		pfoDebug = NULL;
	}
#endif
    emuSync.unlock();
}


#ifdef USE_PTHREAD
void* runEmulation(void* theEmu)
{
    DebugLogMessage("runEmulation");
    Emulator* theRealEmu = (Emulator*)theEmu;
	assert(theRealEmu);
    DebugLogMessage("runEmulation lock");
    theRealEmu->emuSync.lock();
    DebugLogMessage("runEmulation Emulate");
    theRealEmu->Emulate();
    DebugLogMessage("runEmulation unlock");
    theRealEmu->emuSync.unlock();
    return NULL;
}
#endif

bool Emulator::Init()
{
	DebugLogMessage("Emulator::Init()");
    emuSync.lock();
	timer.start();

	// attempt to allocate the general purpose buffer
	pbGPBuffer = new byte [128*1024];

	if (_config->printer)
	{
		// start capturing printer output, if enabled
		if (!printer_start())
		{
			_config->printer = 0;
		}
	}

	if (_input->input_init(*_config))
	{
		CriticalLogMessage("input_init() failed. Aborting.");
		return false;
	}

	_videoPlugin->Init(_config->vid_w, _config->vid_h, _config->vid_bpp, false);
	_renderer.SetMonitor(_config->scr_tube, _config->scr_intensity, _config->scr_remanency);
	int status = _renderer.Init(_videoPlugin);
	if (status != 0) {
		return false;
	}

	_cpcMemory->Init();

	if (!MF2Init())
	{
		return false;
	}

	_renderer.SetMemory(_cpcMemory->GetRAM());
	if(_config->crtc == 1)
		_crtc = make_shared<t_CRTC1>(_gateArray, _vdu);
	else
		_crtc = make_shared<t_CRTC>(_gateArray, _vdu);

	// init Z80 emulation
	GetZ80().z80_init_tables();

    if(_audioPlugin)
    {
        if (_audioPlugin->init(_config, _psg))
        {
            ErrorLogMessage("AudioPlugin init() failed. Disabling sound.");
            // disable sound emulation
    		_config->snd_enabled = 0;
        }
    }
    else
    {
        InfoLogMessage("No audio plugin. Disabling sound.");
        // disable sound emulation
        _config->snd_enabled = 0;
    }

	_psg->Init(_config->snd_enabled);

	emulator_reset(false);
	_config->paused &= ~1;

    if(_audioPlugin)
        _audioPlugin->resume();

	dwTicks = 0;
	dwFPS = 0;
	dwFrameCount = 0;

	dwTicksOffset = 80 / _config->speed;
	dwTicksTarget = timer.getTime();
	dwTicksTargetFPS = dwTicksTarget;

	goToAddress = -1 ;

#ifdef USE_PTHREAD
	// Spawn a thread for emulating (this way we do not freeze the window)
	pthread_create(&emuthread,NULL,runEmulation,this);
#endif

	_isInit = true;

    emuSync.unlock();

	return true;
}

void Emulator::Emulate()
{
	int iExitCondition;

	iExitCondition = EC_FRAME_COMPLETE;

#ifdef USE_PTHREAD
	for(;;)
#else
	for (int i = 0; i<100000; i++)
#endif
	{

        if (exitRequested)
        {
#ifdef USE_PTHREAD
            return;
#else
            break;
#endif
        }

		//Do the goto if needed
		if ( goToAddress != -1 )
		{
			ExecGoTo();
		}

		dwTicks = timer.getTime();
		// update FPS counter?
		if(dwTicks >= dwTicksTargetFPS)
		{
			dwFPS = dwFrameCount;
			dwFrameCount = 0;
			// prep counter for the next run
			dwTicksTargetFPS = dwTicks + 1000;
		}

        // limit speed !
        if(_config->limit_speed)
        {
            if (dwTicks < dwTicksTarget)
            {
                // delay emulation
                if((dwTicksTarget - dwTicks) > 5) // if next frame in more than 5ms use passive wait
                {
			#ifdef _WIN32
				Sleep(dwTicksTarget - dwTicks);
			#else
                    		usleep((dwTicksTarget - dwTicks)*900);
			#endif
                }
                do
                {
                } while(timer.getTime() < dwTicksTarget);
            }
            else
            {
//                DebugLogMessage("Delay after target: %d ms",dwTicks - dwTicksTarget);
            }
            // prep counter for the next run
            dwTicksTarget = dwTicks + dwTicksOffset;
        }
		/*
		// limit to original CPC speed?
		if (_config->limit_speed)
		{
		if (_config->snd_enabled)
		{
		if (iExitCondition == EC_SOUND_BUFFER)
		{
		// limit speed?
		if (!dwSndBufferCopied)
		{
		// delay emulation
		continue;
		}
		dwSndBufferCopied = 0;
		}
		}
		else if (iExitCondition == EC_CYCLE_COUNT)
		{
		dwTicks = timer.getTime();
		// limit speed?
		if (dwTicks < dwTicksTarget)
		{
		// delay emulation
		continue;
		}
		// prep counter for the next run
		dwTicksTarget = dwTicks + dwTicksOffset;
		}
		}
		*/
        if (!_renderer.BeginDisplay(_vdu->GetScrLn()))
        {
//            continue;
        }

        //active if necessary trace mode
        if (GetConfig()->breakpoint)
        {
            GetZ80().trace = 1 ;
        }
        // run the emulation until an exit condition is met
        iExitCondition = GetZ80().z80_execute();

        //We have meet a breakpoint
        if (iExitCondition == EC_BREAKPOINT || iExitCondition == EC_TRACE)
        {
			if (iExitCondition == EC_BREAKPOINT) {
				char msg[19] = "Breakpoint at NNNN";
				sprintf(msg+14, "%.4X", GetZ80()._rPC);
				logMessage(msg);
			}
			Breakpoint();
#ifndef USE_PTHREAD
            return;
#endif
        }

        // emulation finished rendering a complete frame?
        if (iExitCondition == EC_FRAME_COMPLETE)
        {
            dwFrameCount++;

            if (FPSDisplay)
            {
                char chStr[15];
                sprintf(chStr, "%3dFPS %3d%%", (int)dwFPS, (int)dwFPS * 100 / 50);
                _renderer.AddTextLocate(0, 0, chStr);
            }

            //loopcon=0;

            _renderer.EndDisplay(true);
#ifndef USE_PTHREAD
            return;
#endif
        }
		/*
		else
		{
			char x[20];
			sprintf(x,"%d",iExitCondition);
			logMessage(x);
			loopcon++;
			if(loopcon>64*1024) {
				_renderer.EndDisplay(false);
				loopcon=0;
			}
		}
	*/
    }
}

#if 0
void Emulator::Loop()
{
	int iExitCondition;
	bool bolDone;

	dword dwTicks = 0;
	dword dwFPS = 0;
	dword dwFrameCount = 0;

	dword dwTicksOffset = (int)(20.0 / (double)((_config->speed * 25) / 100.0));
	dword dwTicksTarget = timer.getTime();
	dword dwTicksTargetFPS = dwTicksTarget;
	dwTicksTarget += dwTicksOffset;

	audio_resume(_config);

	iExitCondition = EC_FRAME_COMPLETE;
	bolDone = false;

	while (!bolDone)
	{
		bool exit = KeyboardEmulation();

		// run the emulation, as long as the user doesn't pause it
		if (!_config->paused)
		{
			dwTicks = timer.getTime();
			// update FPS counter?
			if (dwTicks >= dwTicksTargetFPS)
			{
				dwFPS = dwFrameCount;
				dwFrameCount = 0;
				// prep counter for the next run
				dwTicksTargetFPS = dwTicks + 1000;
			}

			// limit speed !
			if (dwTicks < dwTicksTarget)
			{
				// delay emulation
				continue;
			}
			// prep counter for the next run
			dwTicksTarget = dwTicksTarget + dwTicksOffset;
			/*
			// limit to original CPC speed?
			if (_config->limit_speed)
			{
			if (_config->snd_enabled)
			{
			if (iExitCondition == EC_SOUND_BUFFER)
			{
			// limit speed?
			if (!dwSndBufferCopied)
			{
			// delay emulation
			continue;
			}
			dwSndBufferCopied = 0;
			}
			}
			else if (iExitCondition == EC_CYCLE_COUNT)
			{
			dwTicks = timer.getTime();
			// limit speed?
			if (dwTicks < dwTicksTarget)
			{
			// delay emulation
			continue;
			}
			// prep counter for the next run
			dwTicksTarget = dwTicks + dwTicksOffset;
			}
			}
			*/
			if (! _renderer.BeginDisplay(_vdu->GetScrLn()))
			{
				continue;
			}

			// run the emulation until an exit condition is met
			iExitCondition = GetZ80()->z80_execute();

			//We have meet a breakpoint
			if (iExitCondition == EC_BREAKPOINT)
			{
				Pause();
				std::cout << "==================================" << endl ;
				continue;
			}


			// emulation finished rendering a complete frame?
			if (iExitCondition == EC_FRAME_COMPLETE)
			{
				dwFrameCount++;

				if (FPSDisplay)
				{
					char chStr[15];
					sprintf(chStr, "%3dFPS %3d%%", (int)dwFPS, (int)dwFPS * 100 / 50);
					_renderer.AddTextLocate(0, 0, chStr);
				}

				_renderer.EndDisplay(true);
			}
			else
			{
				_renderer.EndDisplay(false);
			}
		}

		if (exit)
		{
			bolDone = true;
		}
	}
}
#endif

// TODO: Autoconf: Make conditional on libpng presence, remove SDL dependency
void Emulator::SaveScreenshot(string filename)
{
	GetRenderer().GetVideoPlugin()->Screenshot(filename);
}

Emulator* Emulator::getInstance()
{
	return instance;
}
