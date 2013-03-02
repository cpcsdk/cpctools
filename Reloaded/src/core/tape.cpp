/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2004 Ulrich Doewich

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

/* CDT tape image emulation
   (c) Copyright 2002,2003 Ulrich Doewich

   Nov 05, 2002 - 22:17
   Nov 06, 2002 - 21:31 first test run - nada, zip, zilch - nothing happens...
   Nov 07, 2002 - 11:52 doh! data bits were missing one half of their wave; OHMUMMY.CDT loads, but does not run yet
   Nov 07, 2002 - 22:04 tapes with block id 0x11 seem to load now - enabling tape control through the motor bit fixed it: go figure!
   Nov 07, 2002 - 23:26 added processing of blocks 0x10, 0x12, 0x13, 0x14
   Nov 14, 2002 - 23:35 modified the pause to properly finish the previous pulse, if necessary
   Dec 07, 2002 - 23:00 changed the points where Tape_SwitchLevel is called; level changes should now occur at the correct times
   Jan 19, 2003 - 12:09 added support for the direct recording block (0x15)
   Jan 28, 2003 - 22:33 VOC "tapes" work - problems with SpeedLock loaders must therefore be due to incorrect CDT emulation
   Jan 29, 2003 - 19:14 fixed CDT loading: the pause-after-block value was read from the wrong location!
*/

#include <new>

#include "cap32.h"
#include "tape.h"
#include "config_manager.h"
#include "error.h"

#include "misc/file/filetools.h"

#include "debug.h"

#define CYCLE_SCALE ((40 << 16) / 35)
#define CYCLE_ADJUST(p) (((dword)(p) * CYCLE_SCALE) >> 16)
#define MS_TO_CYCLES(p) ((dword)(p) * 4000)

byte *pbTapeImage = NULL;
byte *pbTapeImageEnd = NULL;

extern byte *pbGPBuffer;

#ifdef USE_DEBUGGER_TAPE
extern FILE *pfoDebug;
extern dword dwDebugFlag;
#endif

using std::bad_alloc;

t_Tape::t_Tape(shared_ptr<t_CPC> cpc) :
CPC(cpc),
bTapeLevel(0),
bTapeData(0),
pbTapeBlock(NULL),
pbTapeBlockData(NULL),
pwTapePulseTable(NULL),
pwTapePulseTableEnd(NULL),
pwTapePulseTablePtr(NULL),
iTapeCycleCount(0),
dwTapePulseCycles(0),
dwTapeZeroPulseCycles(0),
dwTapeOnePulseCycles(0),
dwTapeStage(TAPE_END),
dwTapePulseCount(0),
dwTapeDataCount(0),
dwTapeBitsToShift(0)
{
	wCycleTable[0] = 0;
	wCycleTable[1] = 0;
}

void t_Tape::Emulate(int iCycleCount)
{
	iTapeCycleCount -= iCycleCount;
	if (iTapeCycleCount <= 0)
	{
		Tape_UpdateLevel();
	}
}

void t_Tape::tape_eject (void)
{
	free(pbTapeImage);
	pbTapeImage = NULL;
}


int t_Tape::tape_insert (const char *pchFileName)
{
    long lFileSize;
    int iBlockLength;
    byte bID;
    byte *pbPtr, *pbBlock;
    FILE *pfileObject = NULL;

    tape_eject();
    if ((pfileObject = fopen(pchFileName, "rb")) == NULL)
    {
        return ERR_FILE_NOT_FOUND;
    }
    fread(pbGPBuffer, 10, 1, pfileObject); // read CDT header
    pbPtr = pbGPBuffer;
    if (memcmp(pbPtr, "ZXTape!\032", 8) != 0) // valid CDT file?
    {
        fclose(pfileObject);
        return ERR_TAP_INVALID;
    }
    if (*(pbPtr + 0x08) != 1) // major version must be 1
    {
        fclose(pfileObject);
        return ERR_TAP_INVALID;
    }
    lFileSize = file_size(fileno(pfileObject)) - 0x0a;
    if (lFileSize <= 0) // the tape image should have at least one block...
    {
        fclose(pfileObject);
        return ERR_TAP_INVALID;
    }
    try
    {
        pbTapeImage = new byte[lFileSize+6];
    }
    catch (bad_alloc&)
    {
        fclose(pfileObject);
        return ERR_OUT_OF_MEMORY;
    }
    *pbTapeImage = 0x20; // start off with a pause block
    *(word *)(pbTapeImage+1) = 2000; // set the length to 2 seconds
    fread(pbTapeImage+3, lFileSize, 1, pfileObject); // append the entire CDT file
    fclose(pfileObject);
    *(pbTapeImage+lFileSize+3) = 0x20; // end with a pause block
    *(word *)(pbTapeImage+lFileSize+3+1) = 2000; // set the length to 2 seconds
	
#ifdef USE_DEBUGGER_TAPE
    if (dwDebugFlag) fputs("--- New Tape\r\n", pfoDebug);
#endif
    pbTapeImageEnd = pbTapeImage + lFileSize+6;
    pbBlock = pbTapeImage;
    bool bolGotDataBlock = false;
    while (pbBlock < pbTapeImageEnd)
    {
        bID = *pbBlock++;
        switch(bID)
        {
            case 0x10: // standard speed data block
                iBlockLength = *(word *)(pbBlock+2) + 4;
                bolGotDataBlock = true;
                break;
            case 0x11: // turbo loading data block
                iBlockLength = (*(dword *)(pbBlock+0x0f) & 0x00ffffff) + 0x12;
                bolGotDataBlock = true;
                break;
            case 0x12: // pure tone
                iBlockLength = 4;
                bolGotDataBlock = true;
                break;
            case 0x13: // sequence of pulses of different length
                iBlockLength = *pbBlock * 2 + 1;
                bolGotDataBlock = true;
                break;
            case 0x14: // pure data block
                iBlockLength = (*(dword *)(pbBlock+0x07) & 0x00ffffff) + 0x0a;
                bolGotDataBlock = true;
                break;
            case 0x15: // direct recording
                iBlockLength = (*(dword *)(pbBlock+0x05) & 0x00ffffff) + 0x08;
                bolGotDataBlock = true;
                break;
            case 0x20: // pause
                if ((!bolGotDataBlock) && (pbBlock != pbTapeImage+1))
                {
                    *(word *)pbBlock = 0; // remove any pauses (execept ours) before the data starts
                }
                iBlockLength = 2;
                break;
            case 0x21: // group start
                iBlockLength = *pbBlock + 1;
                break;
            case 0x22: // group end
                iBlockLength = 0;
                break;
            case 0x23: // jump to block
                return ERR_TAP_UNSUPPORTED;
                iBlockLength = 2;
                break;
            case 0x24: // loop start
                return ERR_TAP_UNSUPPORTED;
                iBlockLength = 2;
                break;
            case 0x25: // loop end
                return ERR_TAP_UNSUPPORTED;
                iBlockLength = 0;
                break;
            case 0x26: // call sequence
                return ERR_TAP_UNSUPPORTED;
                iBlockLength = (*(word *)pbBlock * 2) + 2;
                break;
            case 0x27: // return from sequence
                return ERR_TAP_UNSUPPORTED;
                iBlockLength = 0;
                break;
            case 0x28: // select block
                return ERR_TAP_UNSUPPORTED;
                iBlockLength = *(word *)pbBlock + 2;
                break;
            case 0x30: // text description
                iBlockLength = *pbBlock + 1;
                break;
            case 0x31: // message block
                iBlockLength = *(pbBlock+1) + 2;
                break;
            case 0x32: // archive info
                iBlockLength = *(word *)pbBlock + 2;
                break;
            case 0x33: // hardware type
                iBlockLength = (*pbBlock * 3) + 1;
                break;
            case 0x34: // emulation info
                iBlockLength = 8;
                break;
            case 0x35: // custom info block
                iBlockLength = *(dword *)(pbBlock+0x10) + 0x14;
                break;
            case 0x40: // snapshot block
                iBlockLength = (*(dword *)(pbBlock+0x01) & 0x00ffffff) + 0x04;
                break;
            case 0x5A: // another tzx/cdt file
                iBlockLength = 9;
                break;		
           default: // "extension rule"
                iBlockLength = *(dword *)pbBlock + 4;
        }

#ifdef USE_DEBUGGER_TAPE
        if (dwDebugFlag) fprintf(pfoDebug, "%02x %d\r\n", bID, iBlockLength);
#endif

        pbBlock += iBlockLength;
    }

    if (pbBlock != pbTapeImageEnd)
    {
        tape_eject();
        return ERR_TAP_INVALID;
    }

    Tape_Rewind();

    /* char *pchTmpBuffer = new char[MAX_LINE_LEN];
    LoadString(hAppInstance, MSG_TAP_INSERT, chMsgBuffer, sizeof(chMsgBuffer));
    snprintf(pchTmpBuffer, _MAX_PATH-1, chMsgBuffer, CPC.tape_file);
    add_message(pchTmpBuffer);
    delete [] pchTmpBuffer; */
    return 0;
}



int t_Tape::tape_insert_voc (const char *pchFileName)
{
    long lFileSize, lOffset, lInitialOffset, lSampleLength;
    int iBlockLength;
    byte *pbPtr, *pbTapeImagePtr, *pbVocDataBlock, *pbVocDataBlockPtr;
    bool bolDone;

    FILE *pfileObject = NULL;

    tape_eject();
    if ((pfileObject = fopen(pchFileName, "rb")) == NULL)
    {
        return ERR_FILE_NOT_FOUND;
    }
    fread(pbGPBuffer, 26, 1, pfileObject); // read VOC header
    pbPtr = pbGPBuffer;
    if (memcmp(pbPtr, "Creative Voice File\032", 20) != 0) // valid VOC file?
    {
        fclose(pfileObject);
        return ERR_TAP_BAD_VOC;
    }
    lOffset = lInitialOffset = *(word *)(pbPtr + 0x14);
    lFileSize = file_size(fileno(pfileObject));
    if ((lFileSize-26) <= 0) // should have at least one block...
    {
        fclose(pfileObject);
        return ERR_TAP_BAD_VOC;
    }

#ifdef USE_DEBUGGER_TAPE
    if (dwDebugFlag) fputs("--- New Tape\r\n", pfoDebug);
#endif
    iBlockLength = 0;
    lSampleLength = 0;
    byte bSampleRate = 0;
    bolDone = false;
    while ((!bolDone) && (lOffset < lFileSize))
    {
        fseek(pfileObject, lOffset, SEEK_SET);
        fread(pbPtr, 16, 1, pfileObject); // read block ID + size
#ifdef USE_DEBUGGER_TAPE
        if (dwDebugFlag) fprintf(pfoDebug, "%02x %d\r\n", *pbPtr, *(dword *)(pbPtr+0x01) & 0x00ffffff);
#endif
        switch(*pbPtr)
        {
            case 0x0: // terminator
                bolDone = true;
                break;
            case 0x1: // sound data
                iBlockLength = (*(dword *)(pbPtr+0x01) & 0x00ffffff) + 4;
                lSampleLength += iBlockLength - 6;
                if ((bSampleRate) && (bSampleRate != *(pbPtr+0x04))) // no change in sample rate allowed
                {
                    fclose(pfileObject);
                    return ERR_TAP_BAD_VOC;
                }
                bSampleRate = *(pbPtr+0x04);
                if (*(pbPtr+0x05) != 0) // must be 8 bits wide
                {
                    fclose(pfileObject);
                    return ERR_TAP_BAD_VOC;
                }
                break;
            case 0x2: // sound continue
                iBlockLength = (*(dword *)(pbPtr+0x01) & 0x00ffffff) + 4;
                lSampleLength += iBlockLength - 4;
                break;
            case 0x3: // silence
                iBlockLength = 4;
                lSampleLength += *(word *)(pbPtr+0x01) + 1;
                if ((bSampleRate) && (bSampleRate != *(pbPtr+0x03))) // no change in sample rate allowed
                {
                    fclose(pfileObject);
                    return ERR_TAP_BAD_VOC;
                }
                bSampleRate = *(pbPtr+0x03);
                break;
            case 0x4: // marker
                iBlockLength = 3;
                break;
            case 0x5: // ascii
                iBlockLength = (*(dword *)(pbPtr+0x01) & 0x00ffffff) + 4;
                break;
            default:
                fclose(pfileObject);
                return ERR_TAP_BAD_VOC;
        }
        lOffset += iBlockLength;
    }
#ifdef USE_DEBUGGER_TAPE
    if (dwDebugFlag) fprintf(pfoDebug, "--- %ld bytes\r\n", lSampleLength);
#endif

    dword dwTapePulseCycles = 3500000L / (1000000L / (256 - bSampleRate)); // length of one pulse in ZX Spectrum T states
    dword dwCompressedSize = lSampleLength >> 3; // 8x data reduction
    if (dwCompressedSize > 0x00ffffff) // we only support one direct recording block right now
    {
        fclose(pfileObject);
        return ERR_TAP_BAD_VOC;
    }
    try
    {
        pbTapeImage = new byte[dwCompressedSize+1+8+6];
    }
    catch (bad_alloc&)
    {
        fclose(pfileObject);
        return ERR_OUT_OF_MEMORY;
    }
    *pbTapeImage = 0x20; // start off with a pause block
    *(word *)(pbTapeImage+1) = 2000; // set the length to 2 seconds

    *(pbTapeImage+3) = 0x15; // direct recording block
    *(word *)(pbTapeImage+4) = (word)dwTapePulseCycles; // number of T states per sample
    *(word *)(pbTapeImage+6) = 0; // pause after block
    *(pbTapeImage+8) = lSampleLength & 7 ? lSampleLength & 7 : 8; // bits used in last byte
    *(dword *)(pbTapeImage+9) = dwCompressedSize & 0x00ffffff; // data length
    pbTapeImagePtr = pbTapeImage + 12;

    lOffset = lInitialOffset;
    bolDone = false;
    dword dwBit = 8;
    byte bByte = 0;
    while ((!bolDone) && (lOffset < lFileSize))
    {
        fseek(pfileObject, lOffset, SEEK_SET);
        fread(pbPtr, 1, 1, pfileObject); // read block ID
        switch(*pbPtr)
        {
            case 0x0: // terminator
                bolDone = true;
                break;
            case 0x1: // sound data
                {
                    fread(pbPtr, 3+2, 1, pfileObject); // get block size and sound info
                    iBlockLength = (*(dword *)(pbPtr) & 0x00ffffff) + 4;
                    lSampleLength = iBlockLength - 6;
                    try
                    {
                        pbVocDataBlock = new byte[lSampleLength];
                    }
                    catch (bad_alloc&)
                    {
                        fclose(pfileObject);
                        tape_eject();
                        return ERR_OUT_OF_MEMORY;
                    }
                    fread(pbVocDataBlock, lSampleLength, 1, pfileObject);
                    pbVocDataBlockPtr = pbVocDataBlock;
                    for (int iBytePos = 0; iBytePos < lSampleLength; iBytePos++)
                    {
                        byte bVocSample = *pbVocDataBlockPtr++;
                        dwBit--;
                        if (bVocSample > VOC_THRESHOLD)
                        {
                            bByte |= (1 << dwBit);
                        }
                        if (!dwBit) // got all 8 bits?
                        {
                            *pbTapeImagePtr++ = bByte;
                            dwBit = 8;
                            bByte = 0;
                        }
                    }
                    delete[] pbVocDataBlock;
                    break;
                }
            case 0x2: // sound continue
                {
                    fread(pbPtr, 3, 1, pfileObject); // get block size
                    iBlockLength = (*(dword *)(pbPtr) & 0x00ffffff) + 4;
                    lSampleLength = iBlockLength - 4;
                    try
                    {
                        pbVocDataBlock = new byte[lSampleLength];
                    }
                    catch (bad_alloc&)
                    {
                        fclose(pfileObject);
                        tape_eject();
                        return ERR_OUT_OF_MEMORY;
                    }
                    fread(pbVocDataBlock, lSampleLength, 1, pfileObject);
                    pbVocDataBlockPtr = pbVocDataBlock;
                    for (int iBytePos = 0; iBytePos < lSampleLength; iBytePos++)
                    {
                        byte bVocSample = *pbVocDataBlockPtr++;
                        dwBit--;
                        if (bVocSample > VOC_THRESHOLD)
                        {
                            bByte |= (1 << dwBit);
                        }
                        if (!dwBit) // got all 8 bits?
                        {
                            *pbTapeImagePtr++ = bByte;
                            dwBit = 8;
                            bByte = 0;
                        }
                    }
                    delete[] pbVocDataBlock;
                    break;
                }
            case 0x3: // silence
                {
                    iBlockLength = 4;
                    lSampleLength = *(word *)(pbPtr) + 1;
                    for (int iBytePos = 0; iBytePos < lSampleLength; iBytePos++)
                    {
                        dwBit--;
                        if (!dwBit) // got all 8 bits?
                        {
                            *pbTapeImagePtr++ = bByte;
                            dwBit = 8;
                            bByte = 0;
                        }
                    }
                }
                break;
            case 0x4: // marker
                iBlockLength = 3;
                break;
            case 0x5: // ascii
                iBlockLength = (*(dword *)(pbPtr) & 0x00ffffff) + 4;
                break;
        }
        lOffset += iBlockLength;
    }
    fclose(pfileObject);

    *pbTapeImagePtr = 0x20; // end with a pause block
    *(word *)(pbTapeImagePtr+1) = 2000; // set the length to 2 seconds

    pbTapeImageEnd = pbTapeImagePtr + 3;
    /*
    #ifdef USE_DEBUGGER_TAPE
    if ((pfileObject = fopen("./test.cdt", "wb")) != NULL) {
    fwrite(pbTapeImage, (int)((pbTapeImagePtr+3)-pbTapeImage), 1, pfileObject);
    fclose(pfileObject);
    }
    #endif
    */
    Tape_Rewind();

    /* char *pchTmpBuffer = new char[MAX_LINE_LEN];
    LoadString(hAppInstance, MSG_TAP_INSERT, chMsgBuffer, sizeof(chMsgBuffer));
    snprintf(pchTmpBuffer, _MAX_PATH-1, chMsgBuffer, CPC.tape_file);
    add_message(pchTmpBuffer);
    delete [] pchTmpBuffer; */
    return 0;
}

void t_Tape::Tape_GetCycleCount(void)
{
    dwTapePulseCycles = CYCLE_ADJUST(*pwTapePulseTablePtr++);
    if (pwTapePulseTablePtr >= pwTapePulseTableEnd)
    {
    pwTapePulseTablePtr = pwTapePulseTable;
    }
}



void t_Tape::Tape_SwitchLevel(void)
{
    if (bTapeLevel == TAPE_LEVEL_LOW)
    {
        bTapeLevel = TAPE_LEVEL_HIGH; // reverse the level
    }
    else
    {
        bTapeLevel = TAPE_LEVEL_LOW; // reverse the level
    }
}



int t_Tape::Tape_ReadDataBit(void)
{
	if (dwTapeDataCount) {
		if (!dwTapeBitsToShift) {
			bTapeData = *pbTapeBlockData; // get the next data byte
			pbTapeBlockData++;
			dwTapeBitsToShift = 8;
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fprintf(pfoDebug, ">>> 0x%02x\r\n", (byte)bTapeData);
#endif
		}
		byte bBit = bTapeData & 0x80;
		bTapeData <<= 1;
		dwTapeBitsToShift--;
		dwTapeDataCount--;
		if (bBit) {
			dwTapePulseCycles = dwTapeOnePulseCycles;
		}
		else {
			dwTapePulseCycles = dwTapeZeroPulseCycles;
		}
		dwTapePulseCount = 2; // two pulses = one bit
		return 1;
	}
	return 0; // no more data
}



int t_Tape::Tape_ReadSampleDataBit(void)
{
	if (dwTapeDataCount) {
		if (!dwTapeBitsToShift) {
			bTapeData = *pbTapeBlockData; // get the next data byte
			pbTapeBlockData++;
			dwTapeBitsToShift = 8;
		}
		byte bBit = bTapeData & 0x80;
		bTapeData <<= 1;
		dwTapeBitsToShift--;
		dwTapeDataCount--;
		if (bBit) {
			bTapeLevel = TAPE_LEVEL_HIGH; // set high level
		}
		else {
			bTapeLevel = TAPE_LEVEL_LOW; // set low level
		}
		iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
		if (dwDebugFlag)
			fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
		return 1;
	}
	return 0; // no more data
}



int t_Tape::Tape_GetNextBlock(void)
{
	while (pbTapeBlock < pbTapeImageEnd) { // loop until a valid block is found
#ifdef USE_DEBUGGER_TAPE
		if (dwDebugFlag)
			fprintf(pfoDebug, "--- New Block\r\n%02x\r\n", *pbTapeBlock);
#endif
		
		switch (*pbTapeBlock) {
			
		case 0x10: // standard speed data block
            dwTapeStage = TAPE_PILOT_STAGE; // block starts with a pilot tone
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fputs("--- PILOT\r\n", pfoDebug);
#endif
            dwTapePulseCycles = CYCLE_ADJUST(2168);
            iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
            dwTapePulseCount = 3220;
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
            return 1;
			
		case 0x11: // turbo loading data block
            dwTapeStage = TAPE_PILOT_STAGE; // block starts with a pilot tone
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fputs("--- PILOT\r\n", pfoDebug);
#endif
            dwTapePulseCycles = CYCLE_ADJUST(*(word *)(pbTapeBlock+0x01));
            iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
            dwTapePulseCount = *(word *)(pbTapeBlock+0x01+0x0a);
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
            return 1;
			
		case 0x12: // pure tone
            dwTapeStage = TAPE_PILOT_STAGE; // block starts with a pilot tone
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fputs("--- TONE\r\n", pfoDebug);
#endif
            dwTapePulseCycles = CYCLE_ADJUST(*(word *)(pbTapeBlock+0x01));
            iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
            dwTapePulseCount = *(word *)(pbTapeBlock+0x01+0x02);
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
            return 1;
			
		case 0x13: // sequence of pulses of different length
            dwTapeStage = TAPE_SYNC_STAGE;
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fputs("--- PULSE SEQ\r\n", pfoDebug);
#endif
            dwTapePulseCount = *(pbTapeBlock+0x01);
            pwTapePulseTable =
				pwTapePulseTablePtr = (word *)(pbTapeBlock+0x01+0x01);
            pwTapePulseTableEnd = pwTapePulseTable + dwTapePulseCount;
            Tape_GetCycleCount();
            iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
            return 1;
			
		case 0x14: // pure data block
            dwTapeStage = TAPE_DATA_STAGE;
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fputs("--- DATA\r\n", pfoDebug);
#endif
            dwTapeZeroPulseCycles = CYCLE_ADJUST(*(word *)(pbTapeBlock+0x01)); // pulse length for a zero bit
            dwTapeOnePulseCycles = CYCLE_ADJUST(*(word *)(pbTapeBlock+0x01+0x02)); // pulse length for a one bit
            dwTapeDataCount = ((*(dword *)(pbTapeBlock+0x01+0x07) & 0x00ffffff) - 1) << 3; // (byte count - 1) * 8 bits
            dwTapeDataCount += *(pbTapeBlock+0x01+0x04); // add the number of bits in the last data byte
            pbTapeBlockData = pbTapeBlock+0x01+0x0a; // pointer to the tape data
            dwTapeBitsToShift = 0;
            Tape_ReadDataBit(); // get the first bit of the first data byte
            iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
            return 1;
			
		case 0x15: // direct recording
            dwTapeStage = TAPE_SAMPLE_DATA_STAGE;
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fputs("--- SAMPLE DATA\r\n", pfoDebug);
#endif
            dwTapePulseCycles = CYCLE_ADJUST(*(word *)(pbTapeBlock+0x01)); // number of T states per sample
            dwTapeDataCount = ((*(dword *)(pbTapeBlock+0x01+0x05) & 0x00ffffff) - 1) << 3; // (byte count - 1) * 8 bits
            dwTapeDataCount += *(pbTapeBlock+0x01+0x04); // add the number of bits in the last data byte
            pbTapeBlockData = pbTapeBlock+0x01+0x08; // pointer to the tape data
            dwTapeBitsToShift = 0;
            Tape_ReadSampleDataBit(); // get the first bit of the first data byte
            return 1;
			
		case 0x20: // pause
            if (*(word *)(pbTapeBlock+0x01)) { // was a pause requested?
				dwTapeStage = TAPE_PAUSE_STAGE;
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fputs("--- PAUSE\r\n", pfoDebug);
#endif
				dwTapePulseCycles = MS_TO_CYCLES(1); // start with a 1ms level opposite to the one last played
				iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
				dwTapePulseCycles = MS_TO_CYCLES(*(word *)(pbTapeBlock+0x01) - 1); // get the actual pause length
				dwTapePulseCount = 2; // just one pulse
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
				return 1;
            }
            else {
				pbTapeBlock += 2 + 1; // skip block if pause length is 0
            }
            break;
			
		case 0x21: // group start
            pbTapeBlock += *(pbTapeBlock+0x01) + 1 + 1; // nothing to do, skip the block
            break;
			
		case 0x22: // group end
            pbTapeBlock += 1; // nothing to do, skip the block
            break;
			
		case 0x30: // text description
            pbTapeBlock += *(pbTapeBlock+0x01) + 1 + 1; // nothing to do, skip the block
            break;
			
		case 0x31: // message block
            pbTapeBlock += *(pbTapeBlock+0x01+0x01) + 2 + 1; // nothing to do, skip the block
            break;
			
		case 0x32: // archive info
            pbTapeBlock += *(word *)(pbTapeBlock+0x01) + 2 + 1; // nothing to do, skip the block
            break;
			
		case 0x33: // hardware type
            pbTapeBlock += (*(pbTapeBlock+0x01) * 3) + 1 + 1; // nothing to do, skip the block
            break;
			
		case 0x34: // emulation info
            pbTapeBlock += 8 + 1; // nothing to do, skip the block
            break;
			
		case 0x35: // custom info block
            pbTapeBlock += *(dword *)(pbTapeBlock+0x01+0x10) + 0x14 + 1; // nothing to do, skip the block
            break;
			
		case 0x40: // snapshot block
            pbTapeBlock += (*(dword *)(pbTapeBlock+0x01+0x01) & 0x00ffffff) + 0x04 + 1; // nothing to do, skip the block
            break;
			
		case 0x5A: // another tzx/cdt file
            pbTapeBlock += 9 + 1; // nothing to do, skip the block
            break;
			
		default: // "extension rule"
            pbTapeBlock += *(dword *)(pbTapeBlock+0x01) + 4 + 1; // nothing to do, skip the block
      }
   }
   
   return 0; // we've reached the end of the image
}



void t_Tape::Tape_BlockDone(void)
{
	if (pbTapeBlock < pbTapeImageEnd) {
		switch (*pbTapeBlock) {
			
		case 0x10: // standard speed data block
            pbTapeBlock += *(word *)(pbTapeBlock+0x01+0x02) + 0x04 + 1;
            break;
			
		case 0x11: // turbo loading data block
            pbTapeBlock += (*(dword *)(pbTapeBlock+0x01+0x0f) & 0x00ffffff) + 0x12 + 1;
            break;
			
		case 0x12: // pure tone
            pbTapeBlock += 4 + 1;
            break;
			
		case 0x13: // sequence of pulses of different length
            pbTapeBlock += *(pbTapeBlock+0x01) * 2 + 1 + 1;
            break;
			
		case 0x14: // pure data block
            pbTapeBlock += (*(dword *)(pbTapeBlock+0x01+0x07) & 0x00ffffff) + 0x0a + 1;
            break;
			
		case 0x15: // direct recording
            pbTapeBlock += (*(dword *)(pbTapeBlock+0x01+0x05) & 0x00ffffff) + 0x08 + 1;
            break;
			
		case 0x20: // pause
            pbTapeBlock += 2 + 1;
            break;
		}
		
		if (!Tape_GetNextBlock()) {
			dwTapeStage = TAPE_END;
			CPC->tape_play_button = 0;
		}
	}
}



void t_Tape::Tape_UpdateLevel(void)
{
	switch (dwTapeStage) {
		
	case TAPE_PILOT_STAGE:
		Tape_SwitchLevel();
		dwTapePulseCount--;
		if (dwTapePulseCount > 0) { // is the pilot tone still playing?
            iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
		}
		else { // finished with the pilot tone
            switch (*pbTapeBlock) {
				
			case 0x10: // standard speed data block
				dwTapeStage = TAPE_SYNC_STAGE;
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fputs("--- SYNC\r\n", pfoDebug);
#endif
				wCycleTable[0] = 667;
				wCycleTable[1] = 735;
				pwTapePulseTable =
					pwTapePulseTablePtr = &wCycleTable[0];
				pwTapePulseTableEnd = &wCycleTable[2];
				Tape_GetCycleCount();
				iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
				dwTapePulseCount = 2;
				break;
				
			case 0x11: // turbo loading data block
				dwTapeStage = TAPE_SYNC_STAGE;
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fputs("--- SYNC\r\n", pfoDebug);
#endif
				pwTapePulseTable =
					pwTapePulseTablePtr = (word *)(pbTapeBlock+0x01+0x02);
				pwTapePulseTableEnd = (word *)(pbTapeBlock+0x01+0x06);
				Tape_GetCycleCount();
				iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
				dwTapePulseCount = 2;
				break;
				
			case 0x12: // pure tone
				Tape_BlockDone();
				break;
            }
		}
		break;
		
	case TAPE_SYNC_STAGE:
		Tape_SwitchLevel();
		dwTapePulseCount--;
		if (dwTapePulseCount > 0) {
            Tape_GetCycleCount();
            iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
		}
		else {
            switch (*pbTapeBlock) {
				
			case 0x10: // standard speed data block
				dwTapeStage = TAPE_DATA_STAGE;
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fputs("--- DATA\r\n", pfoDebug);
#endif
				dwTapeZeroPulseCycles = CYCLE_ADJUST(855); // pulse length for a zero bit
				dwTapeOnePulseCycles = CYCLE_ADJUST(1710); // pulse length for a one bit
				dwTapeDataCount = *(word *)(pbTapeBlock+0x01+0x02) << 3; // byte count * 8 bits;
				pbTapeBlockData = pbTapeBlock+0x01+0x04; // pointer to the tape data
				dwTapeBitsToShift = 0;
				Tape_ReadDataBit();
				iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
				break;
				
			case 0x11: // turbo loading data block
				dwTapeStage = TAPE_DATA_STAGE;
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fputs("--- DATA\r\n", pfoDebug);
#endif
				dwTapeZeroPulseCycles = CYCLE_ADJUST(*(word *)(pbTapeBlock+0x01+0x06)); // pulse length for a zero bit
				dwTapeOnePulseCycles = CYCLE_ADJUST(*(word *)(pbTapeBlock+0x01+0x08)); // pulse length for a one bit
				dwTapeDataCount = ((*(dword *)(pbTapeBlock+0x01+0x0f) & 0x00ffffff) - 1) << 3; // (byte count - 1) * 8 bits;
				dwTapeDataCount += *(pbTapeBlock+0x01+0x0c); // add the number of bits in the last data byte
				pbTapeBlockData = pbTapeBlock+0x01+0x12; // pointer to the tape data
				dwTapeBitsToShift = 0;
				Tape_ReadDataBit();
				iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
				break;
				
			case 0x13: // sequence of pulses of different length
				Tape_BlockDone();
				break;
            }
		}
		break;
		
	case TAPE_DATA_STAGE:
		Tape_SwitchLevel();
		dwTapePulseCount--;
		if (dwTapePulseCount > 0) {
            iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
		}
		else {
            if (Tape_ReadDataBit()) {
				iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
            }
            else {
				switch (*pbTapeBlock) {
					
				case 0x10: // standard speed data block
					if (*(word *)(pbTapeBlock+0x01)) { // was a pause requested?
                        dwTapeStage = TAPE_PAUSE_STAGE;
#ifdef USE_DEBUGGER_TAPE
						if (dwDebugFlag)
							fputs("--- PAUSE\r\n", pfoDebug);
#endif
                        dwTapePulseCycles = MS_TO_CYCLES(1); // start with a 1ms level opposite to the one last played
                        iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
						if (dwDebugFlag)
							fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
                        dwTapePulseCycles = MS_TO_CYCLES(*(word *)(pbTapeBlock+0x01) - 1); // pause in ms
                        dwTapePulseCount = 2; // just one pulse
					}
					else {
                        Tape_BlockDone();
					}
					break;
					
				case 0x11: // turbo loading data block
					if (*(word *)(pbTapeBlock+0x01+0x0d)) { // was a pause requested?
                        dwTapeStage = TAPE_PAUSE_STAGE;
#ifdef USE_DEBUGGER_TAPE
						if (dwDebugFlag)
							fputs("--- PAUSE\r\n", pfoDebug);
#endif
                        dwTapePulseCycles = MS_TO_CYCLES(1); // start with a 1ms level opposite to the one last played
                        iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
						if (dwDebugFlag)
							fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
                        dwTapePulseCycles = MS_TO_CYCLES(*(word *)(pbTapeBlock+0x01+0x0d) - 1); // pause in ms
                        dwTapePulseCount = 2; // just one pulse
					}
					else {
                        Tape_BlockDone();
					}
					break;
					
				case 0x14: // pure data block
					if (*(word *)(pbTapeBlock+0x01+0x05)) { // was a pause requested?
                        dwTapeStage = TAPE_PAUSE_STAGE;
#ifdef USE_DEBUGGER_TAPE
						if (dwDebugFlag)
							fputs("--- PAUSE\r\n", pfoDebug);
#endif
                        dwTapePulseCycles = MS_TO_CYCLES(1); // start with a 1ms level opposite to the one last played
                        iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
						if (dwDebugFlag)
							fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
                        dwTapePulseCycles = MS_TO_CYCLES(*(word *)(pbTapeBlock+0x01+0x05) - 1); // pause in ms
                        dwTapePulseCount = 2; // just one pulse
					}
					else {
                        Tape_BlockDone();
					}
					break;
					
				default:
					Tape_BlockDone();
				}
            }
		}
		break;
		
	case TAPE_SAMPLE_DATA_STAGE:
		if (!Tape_ReadSampleDataBit()) {
            if (*(word *)(pbTapeBlock+0x01+0x02)) { // was a pause requested?
				dwTapeStage = TAPE_PAUSE_STAGE;
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fputs("--- PAUSE\r\n", pfoDebug);
#endif
				dwTapePulseCycles = MS_TO_CYCLES(1); // start with a 1ms level opposite to the one last played
				iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
				if (dwDebugFlag)
					fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
				dwTapePulseCycles = MS_TO_CYCLES(*(word *)(pbTapeBlock+0x01+0x02) - 1); // pause in ms
				dwTapePulseCount = 2; // just one pulse
            }
            else {
				Tape_BlockDone();
            }
		}
		break;
		
	case TAPE_PAUSE_STAGE:
		bTapeLevel = TAPE_LEVEL_LOW;
		dwTapePulseCount--;
		if (dwTapePulseCount > 0) {
            iTapeCycleCount += (int)dwTapePulseCycles; // set cycle count for current level
#ifdef USE_DEBUGGER_TAPE
			if (dwDebugFlag)
				fprintf(pfoDebug, "%c %d\r\n",(bTapeLevel == TAPE_LEVEL_HIGH ? 'H':'L'), iTapeCycleCount);
#endif
		}
		else {
            Tape_BlockDone();
		}
		break;
		
	case TAPE_END:
		CPC->tape_play_button = 0;
		break;
   }
}



void t_Tape::Tape_Rewind(void)
{
	pbTapeBlock = pbTapeImage;
	bTapeLevel = TAPE_LEVEL_LOW;
	iTapeCycleCount = 0;
	CPC->tape_play_button = 0;
	Tape_GetNextBlock();
}
