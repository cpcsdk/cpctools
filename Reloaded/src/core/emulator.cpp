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
#include "config.h"

#include "dsk.h"
#include "snapshot.h"
#include "input.h"

#include "error.h"

#include "log.h"

//#include "filetools.h"

#include <IMG_savepng.h>

#include <iostream>
#include <unistd.h>

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


void InitDebug()
{
#ifdef USE_DEBUGGER
	pfoDebug = fopen("./debugger.txt", "wt");
#endif
}

void Emulator::emulator_reset(bool bolMF2Reset)
{
	// Z80
	_z80->reset();

	// CPC
	_config.cycle_count = CYCLE_COUNT_INIT;
	_config.tape_motor = 0;
	_config.tape_play_button = 0;
	_config.printer_port = 0xff;

	_input.Reset();

	// VDU
	_vdu->Reset();

	// CRTC
	_crtc->Reset();

	// Gate Array
	_gateArray->Reset();
	_cpcMemory->ga_init_banking();

	// PPI
	// clear PPI data structure
	_ppi->Reset();

	// PSG
	_psg->Reset();

	// FDC
	// clear FDC data structure
	_fdc->Reset();

	_cpcMemory->Reset(bolMF2Reset);

	// initialize active read/write bank configuration
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
	if (_config.mf2)
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
			strncpy(chPath, _config.rom_path, sizeof(chPath)-2);
			strcat(chPath, "/");
			// combine path and file name
			strncat(chPath, _config.rom_mf2, sizeof(chPath)-1 - strlen(chPath));

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
					_config.rom_mf2[0] = 0;
					// disable MF2 support
					_config.mf2 = 0;
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
				_config.rom_mf2[0] = 0;
				// disable MF2 support
				_config.mf2 = 0;
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
	delete _cpcMemory;
	_cpcMemory = NULL;
	delete _gateArray;
	_gateArray = NULL;
	delete _vdu;
	_vdu = NULL;
	delete _crtc;
	_crtc = NULL;
	delete _tape;
	_tape = NULL;
	delete _psg;
	_psg = NULL;
	delete _ppi;
	_ppi = NULL;
	delete _fdc;
	_fdc = NULL;
	delete _z80;
	_z80 = NULL;
}

int Emulator::printer_start()
{
	if (!pfoPrinter)
	{
		if(!(pfoPrinter = fopen(_config.printer_file, "wb")))
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

Emulator::Emulator():
	_renderer(this),
	_config(this),
	FPSDisplay(true)
{
    emuSync.lock();
	// retrieve the emulator configuration
	_config.loadConfiguration();
    emuSync.unlock();
}

Emulator::~Emulator()
{
    emuSync.lock();
#ifdef USE_PTHREAD
    exitRequested=1;
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

	audio_shutdown();

#ifdef USE_DEBUGGER
	fclose(pfoDebug);
#endif
    DebugLogMessage("Destruct Emulator");
    emuSync.unlock();
}


#ifdef USE_PTHREAD
void* runEmulation(void* theEmu)
{
    Emulator* theRealEmu = (Emulator*)theEmu;
    theRealEmu->emuSync.lock();
    theRealEmu->Emulate();
    theRealEmu->emuSync.unlock();
    return NULL;
}
#endif

bool Emulator::Init()
{
    emuSync.lock();
	timer.start();
	
	// attempt to allocate the general purpose buffer
	pbGPBuffer = new byte [128*1024];

	if (_config.printer)
	{
		// start capturing printer output, if enabled
		if (!printer_start())
		{
			_config.printer = 0;
		}
	}

	// init Z80 emulation
	_z80->z80_init_tables();

	if (_input.input_init(_config))
	{
		CriticalLogMessage("input_init() failed. Aborting.");
		return false;
	}

#if 0
#ifdef USE_OPENGL
	_renderer.SetVideoMode(VideoPlugin::OpenGLScale/*_config.vid_style*/, _config.vid_w, _config.vid_h, _config.vid_bpp, false);
	_renderer.SetOpenGLFilter(true);
#else
	_renderer.SetVideoMode(VideoPlugin::DoubleSize/*_config.vid_style*/, _config.vid_w, _config.vid_h, _config.vid_bpp, false);
#endif
#endif

	_renderer.SetVideoMode(_videoPlugin, _config.vid_w, _config.vid_h, _config.vid_bpp, false);

	_renderer.SetMonitor(_config.scr_tube, _config.scr_intensity, _config.scr_remanency);

	if (!_renderer.Init()) {
        CriticalLogMessage("video_init() failed. Aborting.");
		return false;
	}

	_cpcMemory = new t_Memory(_config);

	_cpcMemory->Init();

	if (!MF2Init())
	{
		return false;
	}

	_renderer.SetMemory(_cpcMemory->GetRAM());

	_gateArray = new t_GateArray(_renderer);
	_vdu = new t_VDU(_renderer);
//	_crtc = new t_CRTC1(*_gateArray, *_vdu);
	_crtc = new t_CRTC(*_gateArray, *_vdu);

	_tape = new t_Tape(_config);

	_psg = new t_PSG(_config, *_tape);

	if (audio_init(_config, _psg))
	{
        ErrorLogMessage("audio_init() failed. Disabling sound.");
		// disable sound emulation
		_config.snd_enabled = 0;
	}

	_psg->Init(_config.snd_enabled);

	_ppi = new t_PPI();

	_fdc = new t_FDC(_config) ;

	_z80 = new t_z80regs(*this);

	_gateArray->SetZ80(_z80);

	emulator_reset(false);
	_config.paused &= ~1;

	audio_resume(_config);

	dwTicks = 0;
	dwFPS = 0;
	dwFrameCount = 0;

	dwTicksOffset = 80 / _config.speed;
	dwTicksTarget = timer.getTime();
	dwTicksTargetFPS = dwTicksTarget;


	goToAddress = -1 ;

#ifdef USE_PTHREAD
	// Spawn a thread for emulating (this way we do not freeze the window)
	pthread_create(&emuthread,NULL,runEmulation,this);
#endif

    emuSync.unlock();
	return true;
}

void Emulator::Emulate()
{
	int iExitCondition;
	bool bolDone;

	iExitCondition = EC_FRAME_COMPLETE;
	bolDone = false;

#ifdef USE_PTHREAD
	while(1)
#else
	for (int i = 0; i<100000; i++)
#endif
	{
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
        if(_config.limit_speed)
        {
            if (dwTicks < dwTicksTarget)
            {
                // delay emulation
                if((dwTicksTarget - dwTicks) > 5) // if next frame in more than 5ms use passive wait
                {
                    usleep((dwTicksTarget - dwTicks)*900);
//                    DebugLogMessage("usleep");
                    //continue;
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
		if (_config.limit_speed)
		{
		if (_config.snd_enabled)
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
        if (GetConfig().breakpoint)
        {
            _z80->trace = 1 ;
        }
        // run the emulation until an exit condition is met
        iExitCondition = _z80->z80_execute();

        //We have meet a breakpoint
        if (iExitCondition == EC_BREAKPOINT || iExitCondition == EC_TRACE)
        {
            this->Breakpoint();
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

        if (exitRequested)
        {
#ifdef USE_PTHREAD
            return;
#else
            break;
#endif
        }
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

	dword dwTicksOffset = (int)(20.0 / (double)((_config.speed * 25) / 100.0));
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
		if (!_config.paused)
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
			if (_config.limit_speed)
			{
			if (_config.snd_enabled)
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
			iExitCondition = _z80->z80_execute();

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
	/*
	DebugLogMessage("Save screenshot in %s",filename.c_str());
	IMG_SavePNG( 
			filename.c_str(),
			GetRenderer().GetVideoPlugin()->_publicVideo,
			9);*/
}

Emulator* Emulator::getInstance()
{
	return instance;
}
