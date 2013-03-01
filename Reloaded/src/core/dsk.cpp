/*
 *   $Id: dsk.cpp 60 2008-11-25 17:08:52Z giot.romain $
 *	 Reloded an Amstrad CPC emulator
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


// Caprice32 DSK manager
//


#include "cap32type.h"
#include "error.h"
#include "dsk.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>

using std::cout;

#ifndef HAVE_LIB765_H
extern byte *pbGPBuffer;

FILE *pDSKfileObject;

t_disk_format disk_format[MAX_DISK_FORMAT] = {
	{ "178K Data Format", 40, 1, 9, 2, 0x52, 0xe5, {{ 0xc1, 0xc6, 0xc2, 0xc7, 0xc3, 0xc8, 0xc4, 0xc9, 0xc5 }} },
	{ "169K Vendor Format", 40, 1, 9, 2, 0x52, 0xe5, {{ 0x41, 0x46, 0x42, 0x47, 0x43, 0x48, 0x44, 0x49, 0x45 }} }
};

void dsk_eject (t_drive *drive)
{
	dword track, side;

	for (track = 0; track < DSK_TRACKMAX; track++) { // loop for all tracks
		for (side = 0; side < DSK_SIDEMAX; side++) { // loop for all sides
			if (drive->track[track][side].data) { // track is formatted?
				free(drive->track[track][side].data); // release memory allocated for this track
			}
		}
	}
	dword dwTemp = drive->current_track; // save the drive head position
	memset(drive, 0, sizeof(t_drive)); // clear drive info structure
	drive->current_track = dwTemp;
}



int dsk_load (const char *pchFileName, t_drive *drive, char /*chID*/)
{
	int iRetCode;
	dword dwTrackSize, track, side, sector, dwSectorSize, dwSectors;
    byte *pbPtr, *pbDataPtr, *pbTempPtr, *pbTrackSizeTable;

    byte* tmpBuffer = (byte*)malloc(0x200);

	iRetCode = 0;
	dsk_eject(drive);
	if ((pDSKfileObject = fopen(pchFileName, "rb")) != NULL) {
        fread(tmpBuffer, 0x100, 1, pDSKfileObject); // read DSK header
        pbPtr = tmpBuffer;

		if (memcmp(pbPtr, "MV - CPC", 8) == 0) { // normal DSK image?
			drive->tracks = *(pbPtr + 0x30); // grab number of tracks
			if (drive->tracks > DSK_TRACKMAX) { // compare against upper limit
				drive->tracks = DSK_TRACKMAX; // limit to maximum
			}
			drive->sides = *(pbPtr + 0x31); // grab number of sides
			if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
				iRetCode = ERR_DSK_SIDES;
				goto exit;
			}
			dwTrackSize = (*(pbPtr + 0x32) + (*(pbPtr + 0x33) << 8)) - 0x100; // determine track size in bytes, minus track header
			drive->sides--; // zero base number of sides
			for (track = 0; track < drive->tracks; track++) { // loop for all tracks
				for (side = 0; side <= drive->sides; side++) { // loop for all sides
                    fread(tmpBuffer+0x100, 0x100, 1, pDSKfileObject); // read track header
                    pbPtr = tmpBuffer + 0x100;
					if (memcmp(pbPtr, "Track-Info", 10) != 0) { // abort if ID does not match
						iRetCode = ERR_DSK_INVALID;
						cout << "Track info not found\n";
						goto exit;
					}
					dwSectorSize = 0x80 << *(pbPtr + 0x14); // determine sector size in bytes
					dwSectors = *(pbPtr + 0x15); // grab number of sectors
					if (dwSectors > DSK_SECTORMAX) { // abort if sector count greater than maximum
						iRetCode = ERR_DSK_SECTORS;
						goto exit;
					}
					drive->track[track][side].sectors = dwSectors; // store sector count
					drive->track[track][side].size = dwTrackSize; // store track size
					drive->track[track][side].data = (byte *)malloc(dwTrackSize); // attempt to allocate the required memory
					if (drive->track[track][side].data == NULL) { // abort if not enough
						iRetCode = ERR_OUT_OF_MEMORY;
						goto exit;
                    }
                    drive->track[track][side].gap3 = *(pbPtr + 0x16);
					pbDataPtr = drive->track[track][side].data; // pointer to start of memory buffer
					pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
					for (sector = 0; sector < dwSectors; sector++) { // loop for all sectors
						memcpy(drive->track[track][side].sector[sector].CHRN, (pbPtr + 0x18), 4); // copy CHRN
						memcpy(drive->track[track][side].sector[sector].flags, (pbPtr + 0x1c), 2); // copy ST1 & ST2
						drive->track[track][side].sector[sector].size = dwSectorSize;
						drive->track[track][side].sector[sector].declared_size = dwSectorSize;
						drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data
						pbDataPtr += dwSectorSize;
						pbPtr += 8;
					}
					if (!fread(pbTempPtr, dwTrackSize, 1, pDSKfileObject)) { // read entire track data in one go
						iRetCode = ERR_DSK_INVALID;
						cout << "Track not complete or wrong size\n";
						goto exit;
					}
				}
			}
			drive->altered = 0; // disk is as yet unmodified
		} else {
			if (memcmp(pbPtr, "EXTENDED", 8) == 0) { // extended DSK image?
				drive->tracks = *(pbPtr + 0x30); // number of tracks
				if (drive->tracks > DSK_TRACKMAX) {  // limit to maximum possible
					drive->tracks = DSK_TRACKMAX;
				}
				drive->random_DEs = *(pbPtr + 0x31) & 0x80; // simulate random Data Errors?
				drive->sides = *(pbPtr + 0x31) & 3; // number of sides (0..3)
				if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
					iRetCode = ERR_DSK_SIDES;
					goto exit;
				}
				pbTrackSizeTable = pbPtr + 0x34; // pointer to track size table in DSK header
				drive->sides--; // zero base number of sides
				for (track = 0; track < drive->tracks; track++) { // loop for all tracks
					for (side = 0; side <= drive->sides; side++) { // loop for all sides
						dwTrackSize = (*pbTrackSizeTable++ << 8); // track size in bytes
						if (dwTrackSize != 0) { // only process if track contains data
							dwTrackSize -= 0x100; // compensate for track header
                            fread(tmpBuffer+0x100, 0x100, 1, pDSKfileObject); // read track header
                            pbPtr = tmpBuffer + 0x100;
							if (memcmp(pbPtr, "Track-Info", 10) != 0) { // valid track header?
								cout << "Track info not found\n";
								iRetCode = ERR_DSK_INVALID;
								goto exit;
							}
							dwSectors = *(pbPtr + 0x15); // number of sectors for this track
							if (dwSectors > DSK_SECTORMAX) { // abort if sector count greater than maximum
								iRetCode = ERR_DSK_SECTORS;
								goto exit;
							}
							drive->track[track][side].sectors = dwSectors; // store sector count
							drive->track[track][side].size = dwTrackSize; // store track size
							drive->track[track][side].data = (byte *)malloc(dwTrackSize); // attempt to allocate the required memory
							if (drive->track[track][side].data == NULL) { // abort if not enough
								iRetCode = ERR_OUT_OF_MEMORY;
								goto exit;
							}
							pbDataPtr = drive->track[track][side].data; // pointer to start of memory buffer
							pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
							for (sector = 0; sector < dwSectors; sector++) { // loop for all sectors
								memcpy(drive->track[track][side].sector[sector].CHRN, (pbPtr + 0x18), 4); // copy CHRN
								memcpy(drive->track[track][side].sector[sector].flags, (pbPtr + 0x1c), 2); // copy ST1 & ST2
								dwSectorSize = *(pbPtr + 0x1e) + (*(pbPtr + 0x1f) << 8);
									// sector size in bytes
								drive->track[track][side].sector[sector].size = dwSectorSize;
								unsigned char dwSectorType = *(pbPtr + 0x1B);
								drive->track[track][side].sector[sector].declared_size = 128 << (dwSectorType & 7 );
								drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data
								pbDataPtr += dwSectorSize;
								pbPtr += 8;
							}
							if (dwTrackSize != 0 && !fread(pbTempPtr, dwTrackSize, 1, pDSKfileObject)) { // read entire track data in one go
								cout << "EXT : Track " << track << " not complete or wrong size (" << dwTrackSize << ")\n";
								iRetCode = ERR_DSK_INVALID;
								goto exit;
							}
						} else {
							memset(&drive->track[track][side], 0, sizeof(t_track)); // track not formatted
						}
					}
				}
				drive->altered = 0; // disk is as yet unmodified
			} else {
				iRetCode = ERR_DSK_INVALID; // file could not be identified as a valid DSK
			}
		}
		/*    {
		char *pchTmpBuffer = new char[MAX_LINE_LEN];
		LoadString(hAppInstance, MSG_DSK_LOAD, chMsgBuffer, sizeof(chMsgBuffer));
		snprintf(pchTmpBuffer, _MAX_PATH-1, chMsgBuffer, chID, chID == 'A' ? CPC.drvA_file : CPC.drvB_file);
		add_message(pchTmpBuffer);
		delete [] pchTmpBuffer;
	  } */
exit:
        fclose(pDSKfileObject);
        free(tmpBuffer);
   } else {
	   iRetCode = ERR_FILE_NOT_FOUND;
   }

   if (iRetCode != 0) { // on error, 'eject' disk from drive
	   dsk_eject(drive);
   }
   return iRetCode;
}



int dsk_save (const char *pchFileName, t_drive *drive)
{
	t_DSK_header dh;
	t_track_header th;
	dword track, side, pos, sector;

	if ((pDSKfileObject = fopen(pchFileName, "wb")) != NULL) {
		memset(&dh, 0, sizeof(dh));
		strcpy(dh.id, "EXTENDED CPC DSK File\r\nDisk-Info\r\n");
		strcpy(dh.unused1, "Reloaded!\r\n");
		dh.tracks = drive->tracks;
		dh.sides = (drive->sides+1) | (drive->random_DEs); // correct side count and indicate random DEs, if necessary
		pos = 0;
		for (track = 0; track < drive->tracks; track++) { // loop for all tracks
			for (side = 0; side <= drive->sides; side++) { // loop for all sides
				if (drive->track[track][side].size) { // track is formatted?
					dh.track_size[pos] = (drive->track[track][side].size + 0x100) >> 8; // track size + header in bytes
				}
				pos++;
			}
		}
		if (!fwrite(&dh, sizeof(dh), 1, pDSKfileObject)) { // write header to file
			fclose(pDSKfileObject);
			return ERR_DSK_WRITE;
		}

		memset(&th, 0, sizeof(th));
		strcpy(th.id, "Track-Info\r\n");
		for (track = 0; track < drive->tracks; track++) { // loop for all tracks
			for (side = 0; side <= drive->sides; side++) { // loop for all sides
				if (drive->track[track][side].size) { // track is formatted?
					th.track = track;
					th.side = side;
					th.bps = 2;
					th.sectors = drive->track[track][side].sectors;
					th.gap3 = 0x4e;
					th.filler = 0xe5;
					for (sector = 0; sector < th.sectors; sector++) {
						memcpy(&th.sector[sector][0], drive->track[track][side].sector[sector].CHRN, 4); // copy CHRN
						memcpy(&th.sector[sector][4], drive->track[track][side].sector[sector].flags, 2); // copy ST1 & ST2
						th.sector[sector][6] = drive->track[track][side].sector[sector].size & 0xff;
						th.sector[sector][7] = (drive->track[track][side].sector[sector].size >> 8) & 0xff; // sector size in bytes
					}
					if (!fwrite(&th, sizeof(th), 1, pDSKfileObject)) { // write track header
						fclose(pDSKfileObject);
						return ERR_DSK_WRITE;
					}
					if (!fwrite(drive->track[track][side].data, drive->track[track][side].size, 1, pDSKfileObject)) { // write track data
						fclose(pDSKfileObject);
						return ERR_DSK_WRITE;
					}
				}
			}
		}
		fclose(pDSKfileObject);
	} else {
		return ERR_DSK_WRITE; // write attempt failed
	}

	/* char *pchTmpBuffer = new char[MAX_LINE_LEN];
	LoadString(hAppInstance, MSG_DSK_SAVE, chMsgBuffer, sizeof(chMsgBuffer));
	snprintf(pchTmpBuffer, _MAX_PATH-1, chMsgBuffer, chID, chID == 'A' ? CPC.drvA_file : CPC.drvB_file);
	add_message(pchTmpBuffer);
	delete [] pchTmpBuffer; */
	return 0;
}



int dsk_format (t_drive *drive, int iFormat)
{
	int iRetCode = 0;
	drive->tracks = disk_format[iFormat].tracks;
	if (drive->tracks > DSK_TRACKMAX) { // compare against upper limit
		drive->tracks = DSK_TRACKMAX; // limit to maximum
	}
	drive->sides = disk_format[iFormat].sides;
	if (drive->sides > DSK_SIDEMAX) { // abort if more than maximum
		iRetCode = ERR_DSK_SIDES;
		goto exit;
	}
	{
		drive->sides--; // zero base number of sides
		for (dword track = 0; track < drive->tracks; track++) { // loop for all tracks
			for (dword side = 0; side <= drive->sides; side++) { // loop for all sides
				dword dwSectorSize = 0x80 << disk_format[iFormat].sector_size; // determine sector size in bytes
				dword dwSectors = disk_format[iFormat].sectors;
				if (dwSectors > DSK_SECTORMAX) { // abort if sector count greater than maximum
					iRetCode = ERR_DSK_SECTORS;
					goto exit;
				}
				dword dwTrackSize = dwSectorSize * dwSectors; // determine track size in bytes, minus track header
				drive->track[track][side].sectors = dwSectors; // store sector count
				drive->track[track][side].size = dwTrackSize; // store track size
				drive->track[track][side].data = (byte *)malloc(dwTrackSize); // attempt to allocate the required memory
				if (drive->track[track][side].data == NULL) { // abort if not enough
					iRetCode = ERR_OUT_OF_MEMORY;
					goto exit;
				}
				byte *pbDataPtr = drive->track[track][side].data; // pointer to start of memory buffer
				byte *pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
				byte CHRN[4];
				CHRN[0] = (byte)track;
				CHRN[1] = (byte)side;
				CHRN[3] = (byte)disk_format[iFormat].sector_size;
				for (dword sector = 0; sector < dwSectors; sector++) { // loop for all sectors
					CHRN[2] = disk_format[iFormat].sector_ids[side][sector];
					memcpy(drive->track[track][side].sector[sector].CHRN, CHRN, 4); // copy CHRN
					drive->track[track][side].sector[sector].size = dwSectorSize;
					drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data
					pbDataPtr += dwSectorSize;
				}
				memset(pbTempPtr, disk_format[iFormat].filler_byte, dwTrackSize);
			}
		}
		drive->altered = 1; // flag disk as having been modified
	}
exit:
	if (iRetCode != 0) { // on error, 'eject' disk from drive
		dsk_eject(drive);
	}
	return iRetCode;
}

#endif
