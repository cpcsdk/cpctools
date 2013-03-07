/*
 *   $Id: zip.cpp 60 2008-11-25 17:08:52Z giot.romain $
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



//
// Caprice32 ZIP manager
//

#include "zip.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "error.h"

FILE *pZIPFile;

extern byte *pbGPBuffer;

int zip_dir (t_zip_info *zi)
{
	int n, iFileCount;
	long lFilePosition;
	dword dwCentralDirPosition, dwNextEntry;
	word wCentralDirEntries, wCentralDirSize, wFilenameLength;
	byte *pbPtr;
	char *pchStrPtr;
	dword dwOffset;

	iFileCount = 0;
	if ((pZIPFile = fopen(zi->pchZipFile, "rb")) == NULL) {
		return ERR_FILE_NOT_FOUND;
	}

	wCentralDirEntries = 0;
	wCentralDirSize = 0;
	dwCentralDirPosition = 0;
	lFilePosition = -256;
	do {
		fseek(pZIPFile, lFilePosition, SEEK_END);
		if (fread(pbGPBuffer, 256, 1, pZIPFile) == 0) {
			fclose(pZIPFile);
			return ERR_FILE_BAD_ZIP; // exit if loading of data chunck failed
		}
		pbPtr = pbGPBuffer + (256 - 22); // pointer to end of central directory (under ideal conditions)
		while (pbPtr != (byte *)pbGPBuffer) {
			if (*(dword *)pbPtr == 0x06054b50) { // check for end of central directory signature
				wCentralDirEntries = *(word *)(pbPtr + 10);
				wCentralDirSize = *(word *)(pbPtr + 12);
				dwCentralDirPosition = *(dword *)(pbPtr + 16);
				break;
			}
			pbPtr--; // move backwards through buffer
		}
		lFilePosition -= 256; // move backwards through ZIP file
	} while (wCentralDirEntries == 0);
	if (wCentralDirSize == 0) {
		fclose(pZIPFile);
		return ERR_FILE_BAD_ZIP; // exit if no central directory was found
	}
	fseek(pZIPFile, dwCentralDirPosition, SEEK_SET);
	if (fread(pbGPBuffer, wCentralDirSize, 1, pZIPFile) == 0) {
		fclose(pZIPFile);
		return ERR_FILE_BAD_ZIP; // exit if loading of data chunck failed
	}

	pbPtr = pbGPBuffer;
	if (zi->pchFileNames) {
		free(zi->pchFileNames); // dealloc old string table
	}
	zi->pchFileNames = (char *)malloc(wCentralDirSize); // approximate space needed by using the central directory size
	pchStrPtr = zi->pchFileNames;

	for (n = wCentralDirEntries; n; n--) {
		wFilenameLength = *(word *)(pbPtr + 28);
		dwOffset = *(dword *)(pbPtr + 42);
		dwNextEntry = wFilenameLength + *(word *)(pbPtr + 30) + *(word *)(pbPtr + 32);
		pbPtr += 46;
		char *pchThisExtension = zi->pchExtension;
		while (*pchThisExtension != '\0') { // loop for all extensions to be checked
			if (strncmp((char *)pbPtr + (wFilenameLength - 4), pchThisExtension, 4) == 0) {
				strncpy(pchStrPtr, (char *)pbPtr, wFilenameLength); // copy filename from zip directory
				pchStrPtr[wFilenameLength] = 0; // zero terminate string
				pchStrPtr += wFilenameLength+1;
				*(dword *)pchStrPtr = dwOffset; // associate offset with string
				pchStrPtr += 4;
				iFileCount++;
				break;
			}
			pchThisExtension += 4; // advance to next extension
		}
		pbPtr += dwNextEntry;
	}
	fclose(pZIPFile);

	if (iFileCount == 0) { // no files found?
		return ERR_FILE_EMPTY_ZIP;
	}

	zi->iFiles = iFileCount;
	return 0; // operation completed successfully
}



int zip_extract (char */*pchZipFile*/, char */*pchFileName*/, dword /*dwOffset*/)
{
/*
int iStatus, iCount;
dword dwSize;
byte *pbInputBuffer, *pbOutputBuffer;
FILE *pfileOut, *pfileIn;
z_stream z;

  tmpnam(pchFileName); // generate a unique (temporary) file name for the decompression process
  if (!(pfileOut = fopen(pchFileName, "wb"))) {
  return ERR_FILE_UNZIP_FAILED; // couldn't create output file
  }
  pfileIn = fopen(pchZipFile, "rb"); // open ZIP file for reading
  fseek(pfileIn, dwOffset, SEEK_SET); // move file pointer to beginning of data block
  fread(pbGPBuffer, 30, 1, pfileIn); // read local header
  dwSize = *(dword *)(pbGPBuffer + 18); // length of compressed data
  dwOffset += 30 + *(word *)(pbGPBuffer + 26) + *(word *)(pbGPBuffer + 28);
  fseek(pfileIn, dwOffset, SEEK_SET); // move file pointer to start of compressed data

	pbInputBuffer = pbGPBuffer; // space for compressed data chunck
	pbOutputBuffer = pbInputBuffer + 16384; // space for uncompressed data chunck
	z.zalloc = (alloc_func)0;
	z.zfree = (free_func)0;
	z.opaque = (voidpf)0;
	iStatus = inflateInit2(&z, -MAX_WBITS); // init zlib stream (no header)
	do {
	z.next_in = pbInputBuffer;
	if (dwSize > 16384) { // limit input size to max 16K or remaining bytes
	z.avail_in = 16384;
	} else {
	z.avail_in = dwSize;
	}
	z.avail_in = fread(pbInputBuffer, 1, z.avail_in, pfileIn); // load compressed data chunck from ZIP file
	while ((z.avail_in) && (iStatus == Z_OK)) { // loop until all data has been processed
	z.next_out = pbOutputBuffer;
	z.avail_out = 16384;
	iStatus = inflate(&z, Z_NO_FLUSH); // decompress data
	iCount = 16384 - z.avail_out;
	if (iCount) { // save data to file if output buffer is full
	fwrite(pbOutputBuffer, 1, iCount, pfileOut);
	}
	}
	dwSize -= 16384; // advance to next chunck
	} while ((dwSize > 0) && (iStatus == Z_OK)) ; // loop until done
	if (iStatus != Z_STREAM_END) {
	return ERR_FILE_UNZIP_FAILED; // abort on error
	}
	iStatus = inflateEnd(&z); // clean up
	fclose(pfileIn);
	fclose(pfileOut);
	*/

	return 0; // data was successfully decompressed
}
