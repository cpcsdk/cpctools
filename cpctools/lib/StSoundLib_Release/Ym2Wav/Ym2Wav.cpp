/*-----------------------------------------------------------------------------

	ST-Sound ( YM files player library )

	Copyright (C) 1995-1999 Arnaud Carre ( http://leonard.oxg.free.fr )

	This is a sample program: it's an YM to WAV converter.

-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------

	This file is part of ST-Sound

	ST-Sound is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	ST-Sound is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with ST-Sound; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

-----------------------------------------------------------------------------*/

#include	<stdlib.h>
#include	<stdio.h>
#include	"../StSoundLibrary/StSoundLibrary.h"

//---------------------------------------------------------------------
//---------------------------------------------------------------------
#define	NBSAMPLEPERBUFFER		1024
static	ymsample	convertBuffer[NBSAMPLEPERBUFFER];	// Sound buffer to create WAV file.


//---------------------------------------------------------------------
// To produce a WAV file.
//---------------------------------------------------------------------
#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746D66
#define ID_DATA 0x61746164
typedef struct
{
    ymu32		   RIFFMagic;
    ymu32   FileLength;
    ymu32   FileType;
    ymu32   FormMagic;
    ymu32   FormLength;
    ymu16  SampleFormat;
    ymu16  NumChannels;
    ymu32   PlayRate;
    ymu32   BytesPerSec;
    ymu16  Pad;
    ymu16  BitsPerSample;
    ymu32   DataMagic;
    ymu32   DataLength;
} WAVHeader;



int main(int argc, char* argv[])
{

	//--------------------------------------------------------------------------
	// Checks args.
	//--------------------------------------------------------------------------
	printf(	"YM2WAV YM to WAV converter.\n"
			"Using ST-Sound Library, under GPL license\n"
			"Copyright (C) 1995-1999 Arnaud Carre ( http://leonard.oxg.free.fr )\n");

	if (argc!=3)
	{
		printf("Usage: YM2WAV <ym music file> <wav file>\n\n");
		return -1;
	}


	//--------------------------------------------------------------------------
	// Load YM music and creates WAV file
	//--------------------------------------------------------------------------
	YMMUSIC *pMusic = ymMusicCreate();

	if (ymMusicLoad(pMusic,argv[1]))
	{

		// Get info about the current music.
		ymMusicInfo_t info;
		ymMusicGetInfo(pMusic,&info);

		printf("Generating wav file from \"%s\"\n",argv[1]);
		printf("%s\n%s\n(%s)\n",info.pSongName,info.pSongAuthor,info.pSongComment);
		printf("Total music time: %d seconds.\n",info.musicTimeInSec);

		FILE *out = fopen(argv[2],"wb");
		if (!out)
		{
			printf("Unable to create %s file.\n",argv[2]);
			return -1;
		}
		// Reserve space to write the header.
		WAVHeader head;
		fwrite(&head,1,sizeof(WAVHeader),out);		// write non initialized dummy data to reserve space


		//--------------------------------------------------------------------------
		// Main loop: render each music frame and store to the WAV file.
		//--------------------------------------------------------------------------
		ymMusicSetLoopMode(pMusic,YMFALSE);			// Be sure there is no loop (to avoid a BIG wav file :-) )
		ymu32 totalNbSample = 0;

		ymu32 nbTotal = info.musicTimeInSec * 44100;
		int oldRatio = -1;

		while (ymMusicCompute(pMusic,convertBuffer,NBSAMPLEPERBUFFER))
		{
			fwrite((void*)convertBuffer,sizeof(ymsample),NBSAMPLEPERBUFFER,out);
			totalNbSample += NBSAMPLEPERBUFFER;

			const int ratio = (totalNbSample * 100) / nbTotal;
			if (ratio != oldRatio)
			{
				printf("Rendering... (%d%%)\r",ratio);
				oldRatio = ratio;
			}
		}
		printf("\n");
	

		//--------------------------------------------------------------------------
		// Write the WAV file header and close the file.
		//--------------------------------------------------------------------------
		fseek(out,0,SEEK_SET);
		head.RIFFMagic = ID_RIFF;
		head.FileType  = ID_WAVE;
		head.FormMagic = ID_FMT;
		head.DataMagic = ID_DATA;
		head.FormLength = 0x10;
		head.SampleFormat = 1;
		head.NumChannels = 1;
		head.PlayRate = 44100;
		head.BitsPerSample = 16;
		head.BytesPerSec = 44100*(16/8);
		head.Pad = (16/8);
		head.DataLength = totalNbSample*(16/8);
		head.FileLength = head.DataLength + sizeof(WAVHeader) - 8;
		fwrite(&head,1,sizeof(WAVHeader),out);
		fseek(out,0,SEEK_END);
		fclose(out);
		printf("%d samples written (%.02f Mb).\n",totalNbSample,(float)(totalNbSample*sizeof(ymsample))/(1024*1024));
	}
	else
	{	// Error in loading music.
		printf("Error in loading file %s:\n%s\n",argv[1],ymMusicGetLastError(pMusic));
		return -1;
	}

	ymMusicDestroy(pMusic);

	return 0;
}

