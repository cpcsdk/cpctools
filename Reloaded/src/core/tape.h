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

#ifndef _TAPE_H_
#define _TAPE_H_

#include <memory>
#include "cap32type.h"

using std::shared_ptr;

class t_CPC;

#define TAPE_LEVEL_LOW 0
#define TAPE_LEVEL_HIGH 0x80

#define VOC_THRESHOLD 128

#define TAPE_PILOT_STAGE 1
#define TAPE_SYNC_STAGE 2
#define TAPE_DATA_STAGE 3
#define TAPE_SAMPLE_DATA_STAGE 4
#define TAPE_PAUSE_STAGE 5
#define TAPE_END 6

class t_Tape
{
private:
	shared_ptr<t_CPC>		CPC;

	byte bTapeLevel;
	byte bTapeData;
	byte *pbTapeBlock;
	byte *pbTapeBlockData;
	word *pwTapePulseTable;
	word *pwTapePulseTableEnd;
	word *pwTapePulseTablePtr;
	word wCycleTable[2];
	int iTapeCycleCount;
	dword dwTapePulseCycles;
	dword dwTapeZeroPulseCycles;
	dword dwTapeOnePulseCycles;
	dword dwTapeStage;
	dword dwTapePulseCount;
	dword dwTapeDataCount;
	dword dwTapeBitsToShift;

public:
	t_Tape(shared_ptr<t_CPC> cpc);

	void Emulate(int iCycleCount);

	void tape_eject (void);
	int tape_insert (const char *pchFileName);
	int tape_insert_voc (const char *pchFileName);

	void Tape_UpdateLevel(void);
	void Tape_Rewind(void);

	inline unsigned char			GetTapeLevel() const		{ return bTapeLevel;	}

private:
	void Tape_GetCycleCount(void);
	void Tape_SwitchLevel(void);
	int Tape_ReadDataBit(void);
	int Tape_ReadSampleDataBit(void);
	int Tape_GetNextBlock(void);
	void Tape_BlockDone(void);
};

#endif
