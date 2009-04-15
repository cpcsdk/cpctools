/*
 *   $Id$
 *	 CapriceReloded an Amstrad CPC emulator
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


/**
 * @todo Clean this totally unreadable file
 */

//
// Caprice32 config manager
//

#include "config.h"
#include <stdio.h>
#include "cap32type.h"
#include <string.h>
#include <stdlib.h>
#include "audio.h"
#include "dsk.h"
#include "render.h"
#include "emulator.h"
#include "c_inifile.h"

#ifdef WIN32
#include <direct.h>
#else
#include <linux/unistd.h>
#endif

#define MIN_SPEED_SETTING 2
#define MAX_SPEED_SETTING 32
#define DEF_SPEED_SETTING 4

extern t_disk_format disk_format[MAX_DISK_FORMAT];

t_CPC::t_CPC()
{
    model=0;
    jumpers=0;
    ram_size=0;
    speed=0;
    limit_speed=0;
    paused=0;
    auto_pause=0;
    keyboard_line=0;
    tape_motor=0;
    tape_play_button=0;
    printer=0;
    mf2=0;
    keyboard=0;
    joysticks=0;
    cycle_count=0;

    snd_enabled=0;
    snd_playback_rate=0;
    snd_bits=0;
    snd_stereo=0;
    snd_volume=0;
    snd_pp_device=0;

    kbd_layout=0;

    max_tracksize=0;

    snap_zip=false;
    drvA_zip=false;
    drvA_format = 0;
    drvB_zip=false;
    drvB_format=0;
    tape_zip=false;

    scr_fps=false;
    scr_tube=Renderer::ColoursMode;
    scr_intensity=0;
    scr_remanency=false;

    vid_w=800;
    vid_h=600;
    vid_bpp=24;
    vid_style=VideoPlugin::DoubleWidth;

}

unsigned int getConfigValueInt (const char* pchFileName, const char* pchSection, const char* pchKey, unsigned int iDefaultValue)
{
	C_Inifile_error err = C_INIFILE_NO_ERROR;
	int val = c_inifile_get_integer(pchSection,pchKey,&err);
	if (err!=C_INIFILE_NO_ERROR)
	{
	    return iDefaultValue;
	}
	else
	{
	    return val;
	}
}

void getConfigValueString (const char* pchFileName, const char* pchSection, const char* pchKey, char* pchValue, const char* pchDefaultValue)
{
    char* val;
    val = c_inifile_get_string(pchSection,pchKey,NULL);
    if(val==NULL)
    {
		strcpy(pchValue,pchDefaultValue);
    }
    else
    {
		strcpy(pchValue,val);
		free(val);
    }
    return;
}



void t_CPC::loadConfiguration (Emulator &emulator)
{
	C_Inifile_error err = C_INIFILE_NO_ERROR ;


	char chFileName[] = DATA_PATH "cap32.cfg" ;

	c_inifile_init( chFileName,&err);

	if(err!=C_INIFILE_NO_ERROR)
	{
	    printf("Error opening configfile !\n");
		exit(-1);
	}
	else
	{

	    // CPC 6128
	    model = getConfigValueInt(chFileName, "system", "model", 2);
	    if (model > 2 || model < 0) 
	    {
			model = 2;
	    }

	    // OEM is Amstrad, video refresh is 50Hz
	    jumpers = getConfigValueInt(chFileName, "system", "jumpers", 0x1e) & 0x1e;

	    // 128KB RAM
	    ram_size = getConfigValueInt(chFileName, "system", "ram_size", 128) & 0x02c0;
	    if (ram_size > 576)
	    {
			ram_size = 576;
	    }
	    else if ((model == 2) && (ram_size < 128))
	    {
			// minimum RAM size for CPC 6128 is 128KB
			ram_size = 128;
	    }

	    // original CPC speed
	    speed = getConfigValueInt(chFileName, "system", "speed", DEF_SPEED_SETTING);
	    if ((speed < MIN_SPEED_SETTING) || (speed > MAX_SPEED_SETTING))
	    {
			speed = DEF_SPEED_SETTING;
	    }
	    limit_speed = 1;

	    auto_pause = getConfigValueInt(chFileName, "system", "auto_pause", 1) & 1;
	    printer = getConfigValueInt(chFileName, "system", "printer", 0) & 1;
	    mf2 = getConfigValueInt(chFileName, "system", "mf2", 0) & 1;

	    keyboard = getConfigValueInt(chFileName, "system", "keyboard", 0);
            // TODO ; load keymap
	    joysticks = getConfigValueInt(chFileName, "system", "joysticks", 0) & 1;

	    vid_w = getConfigValueInt(chFileName, "video", "scr_width", 800);
	    vid_h = getConfigValueInt(chFileName, "video", "scr_height", 600);
	    vid_bpp = getConfigValueInt(chFileName, "video", "scr_bpp", 24);
	    vid_style = (VideoPlugin::VideoType)getConfigValueInt(chFileName, "video", "scr_style", 1);
	    fs = (getConfigValueInt(chFileName, "video", "scr_window", 0) & 1) == 0;
	    oglfilter = getConfigValueInt(chFileName, "video", "scr_oglfilter", 1) & 1;

	    emulator.GetRenderer().SetOpenGLFilter(oglfilter);


	    scr_fps = getConfigValueInt(chFileName, "video", "scr_fps", 0) & 1;
            switch (getConfigValueInt(chFileName, "video", "scr_tube", 0))
            {
                case 0:
                    scr_tube = Renderer::GreenMode;
                    break;
                case 1:
                    scr_tube = Renderer::ColoursMode;
                    break;
                case 2:
                    scr_tube = Renderer::GreyMode;
                    break;
                case 3:
                    scr_tube = Renderer::PersonalMode;
                    break;
                case 4:
                    scr_tube = Renderer::ColoursHiFiMode;
                    break;
                default:
                    scr_tube = Renderer::ColoursHiFiMode;
            }
	    scr_intensity = getConfigValueInt(chFileName, "video", "scr_intensity", 10);
	    scr_remanency = getConfigValueInt(chFileName, "video", "scr_remanency", 0) & 1;
	    if ((scr_intensity < 5) || (scr_intensity > 15))
	    {
			scr_intensity = 10;
	    }
	    //emulator.GetRenderer().SetMonitor(color, intensity, remanency);

	    //renderer.scr_vsync = getConfigValueInt(chFileName, "video", "scr_vsync", 1) & 1;
	    //renderer.scr_led = getConfigValueInt(chFileName, "video", "scr_led", 1) & 1;

	    snd_enabled = getConfigValueInt(chFileName, "sound", "enabled", 1) & 1;
	    snd_playback_rate = getConfigValueInt(chFileName, "sound", "playback_rate", 44100);
	    snd_bits = getConfigValueInt(chFileName, "sound", "bits", 16);
	    snd_stereo = getConfigValueInt(chFileName, "sound", "stereo", 1) & 1;
	    snd_volume = getConfigValueInt(chFileName, "sound", "volume", 100);
	    if (snd_volume > 100)
	    {
			snd_volume = 100;
	    }
	    snd_pp_device = getConfigValueInt(chFileName, "sound", "pp_device", 0) & 1;

	    kbd_layout = getConfigValueInt(chFileName, "control", "kbd_layout", 0);
	    if (kbd_layout > 3)
	    {
			kbd_layout = 0;
	    }

	    max_tracksize = getConfigValueInt(chFileName, "file", "max_track_size", 6144-154);
	    getConfigValueString(chFileName, "file", "snap_path", snap_path, DATA_PATH "snap");
	    if (snap_path[0] == '\0')
	    {
			strcpy(snap_path, DATA_PATH "snap");
	    }
	    getConfigValueString(chFileName, "file", "snap_file", snap_file, "");
	    snap_zip = getConfigValueInt(chFileName, "file", "snap_zip", 0) & 1;

	    getConfigValueString(chFileName, "file", "drvA_path", drvA_path, DATA_PATH "disc");
	    if (drvA_path[0] == '\0')
	    {
			strcpy(drvA_path, DATA_PATH "disc");
	    }
	    getConfigValueString(chFileName, "file", "drvA_file", drvA_file, "");
	    drvA_zip = getConfigValueInt(chFileName, "file", "drvA_zip", 0) & 1;
	    drvA_format = getConfigValueInt(chFileName, "file", "drvA_format", DEFAULT_DISK_FORMAT);
	    getConfigValueString(chFileName, "file", "drvB_path", drvB_path, DATA_PATH "disc");
	    if (drvB_path[0] == '\0')
	    {
			strcpy(drvB_path, DATA_PATH "disc");
	    }
	    getConfigValueString(chFileName, "file", "drvB_file", drvB_file, "");
	    drvB_zip = getConfigValueInt(chFileName, "file", "drvB_zip", 0) & 1;
	    drvB_format = getConfigValueInt(chFileName, "file", "drvB_format", DEFAULT_DISK_FORMAT);
	    
		getConfigValueString(chFileName, "file", "tape_path", tape_path, DATA_PATH "tape");
	    if (tape_path[0] == '\0')
	    {
			strcpy(tape_path, DATA_PATH "tape");
	    }
	    getConfigValueString(chFileName, "file", "tape_file", tape_file, "");
	    tape_zip = getConfigValueInt(chFileName, "file", "tape_zip", 0) & 1;

#ifndef HAVE_LIB765_H
	    int iFmt = FIRST_CUSTOM_DISK_FORMAT;
	    {
		// loop through all user definable disk formats
		for (int i = iFmt; i < MAX_DISK_FORMAT; i++)
		{
		    dword dwVal;
		    char *pchTail;
		    char chFmtId[14];
		    // clear slot
		    disk_format[iFmt].label[0] = 0;
		    // build format ID
		    sprintf(chFmtId, "fmt%02d", i);
		    char chFmtStr[256];
		    getConfigValueString(chFileName, "file", chFmtId, chFmtStr, "");
		    // found format definition for this slot?
		    if (chFmtStr[0] != 0)
		    {
			char chDelimiters[] = ",";
			char *pchToken;
			// format label
			pchToken = strtok(chFmtStr, chDelimiters);
			strncpy((char *)disk_format[iFmt].label, pchToken, sizeof(disk_format[iFmt].label)-1);

			// number of tracks
			pchToken = strtok(NULL, chDelimiters);
			// value missing?
			if (pchToken == NULL)
			{
			    continue;
			}
			dwVal = strtoul(pchToken, &pchTail, 0);
			// invalid value?
			if ((dwVal < 1) || (dwVal > DSK_TRACKMAX))
			{
			    continue;
			}
			disk_format[iFmt].tracks = dwVal;

			// number of sides
			pchToken = strtok(NULL, chDelimiters);
			// value missing?
			if (pchToken == NULL)
			{
			    continue;
			}
			dwVal = strtoul(pchToken, &pchTail, 0);
			// invalid value?
			if ((dwVal < 1) || (dwVal > DSK_SIDEMAX))
			{
			    continue;
			}
			disk_format[iFmt].sides = dwVal;

			// number of sectors
			pchToken = strtok(NULL, chDelimiters);
			// value missing?
			if (pchToken == NULL)
			{
			    continue;
			}
			dwVal = strtoul(pchToken, &pchTail, 0);
			// invalid value?
			if ((dwVal < 1) || (dwVal > DSK_SECTORMAX))
			{
			    continue;
			}
			disk_format[iFmt].sectors = dwVal;

			// sector size as N value
			pchToken = strtok(NULL, chDelimiters);
			// value missing?
			if (pchToken == NULL)
			{
			    continue;
			}
			dwVal = strtoul(pchToken, &pchTail, 0);
			// invalid value?
			if ((dwVal < 1) || (dwVal > 6))
			{
			    continue;
			}
			disk_format[iFmt].sector_size = dwVal;

			// gap#3 length
			pchToken = strtok(NULL, chDelimiters);
			// value missing?
			if (pchToken == NULL)
			{
			    continue;
			}
			dwVal = strtoul(pchToken, &pchTail, 0);
			// invalid value?
			if ((dwVal < 1) || (dwVal > 255))
			{
			    continue;
			}
			disk_format[iFmt].gap3_length = dwVal;

			// filler byte
			pchToken = strtok(NULL, chDelimiters);
			// value missing?
			if (pchToken == NULL)
			{
			    continue;
			}
			dwVal = strtoul(pchToken, &pchTail, 0);
			disk_format[iFmt].filler_byte = (byte)dwVal;

			for (int iSide = 0; iSide < (int)disk_format[iFmt].sides; iSide++)
			{
			    for (int iSector = 0; iSector < (int)disk_format[iFmt].sectors; iSector++)
			    {
				// sector ID
				pchToken = strtok(NULL, chDelimiters);
				// value missing?
				if (pchToken == NULL)
				{
				    dwVal = iSector+1;
				}
				else
				{
				    dwVal = strtoul(pchToken, &pchTail, 0);
				}
				disk_format[iFmt].sector_ids[iSide][iSector] = (byte)dwVal;
			    }
			}
			// entry is valid
			iFmt++;
		    }
		}
	    }
#endif
	    getConfigValueString(chFileName, "file", "printer_file", printer_file, DATA_PATH "printer.dat");
	    if (printer_file[0] == '\0')
	    {
			strcpy(printer_file, DATA_PATH "printer.dat");
	    }

	    getConfigValueString(chFileName, "file", "sdump_file", sdump_file, DATA_PATH "screen.png");
	    if (sdump_file[0] == '\0')
	    {
			strcpy(sdump_file, DATA_PATH "screen.png");
	    }

	    getConfigValueString(chFileName, "rom", "rom_path", rom_path, DATA_PATH "rom");
	    // loop for ROMs 0-15
	    for (int iRomNum = 0; iRomNum < 16; iRomNum++)
	    {
			char chRomId[14];
			sprintf(chRomId, "slot%02d", iRomNum); // build ROM ID
			getConfigValueString(chFileName, "rom", chRomId, rom_file[iRomNum], "");
	    }
	    
		
		// if the path is empty, set it to the default
	    if (rom_path[0] == '\0')
	    {
			strcpy(rom_path, DATA_PATH "rom");
	    }

		FILE *pfileObject;
	    if ((pfileObject = fopen(chFileName, "rt")) == NULL)
	    {
		// insert AMSDOS in slot 7 if the config file does not exist yet
		strcpy(rom_file[7], "amsdos.rom");
	    }
	    else
	    {
		fclose(pfileObject);
	    }
	    getConfigValueString(chFileName, "rom", "rom_mf2", rom_mf2, "");

	    c_inifile_close();
	}
}
