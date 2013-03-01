/*
 *   $Id: vdu.cpp 60 2008-11-25 17:08:52Z giot.romain $
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
// Caprice32 VDU emulator
//

#include <cmath>

#include "vdu.h"
#include "render.h"

#include "debug.h"

#ifdef USE_DEBUGGER_CRTC
extern dword dwDebugFlag;
extern FILE *pfoDebug;
#endif

t_VDU::t_VDU(Renderer &render) :
_renderer(render)
{
	Reset();
}

void t_VDU::Reset()
{
	_renderer.Reset();

	iMonHSPeakPos = 0;
	iMonHSStartPos = 0;
	iMonHSEndPos = 0;
	iMonHSPeakToStart = 0;
	iMonHSStartToPeak = 0;
	iMonHSEndToPeak = 0;
	iMonHSPeakToEnd = 0;
	HadP = 0;

	scrln = 0;
	scanline = 0;
	frame_completed = 0;

	flag_drawing = 1;

	HSyncDuration = 0xA00;
	MinHSync = 0x4000 - HSyncDuration - 257;
	MaxHSync = 0x4000 - HSyncDuration + 257;
	MonHSYNC = 0x4000 - HSyncDuration;
	MonFreeSync = MonHSYNC;

	MinVSync = MID_VHOLD;
	MaxVSync = MinVSync + MIN_VHOLD_RANGE + (int)ceil((float)((MinVSync - MIN_VHOLD) *
		(MAX_VHOLD_RANGE - MIN_VHOLD_RANGE) / (MAX_VHOLD - MIN_VHOLD)));
}

void t_VDU::Render(unsigned int memAddr, dword flags)
{
	if (flag_drawing) { // are we within the rendering area?
		_renderer.Render(memAddr, flags);
	}

	iMonHSStartPos += 0x100;
	iMonHSEndPos += 0x100;
	iMonHSPeakPos += 0x100;

	int horzPos = _renderer.UpdateHorzPos();

	if (horzPos >= MonHSYNC)
	{
		HadP = 1;
		iMonHSPeakPos = horzPos - MonHSYNC;
		iMonHSStartToPeak = iMonHSStartPos - iMonHSPeakPos;
		iMonHSEndToPeak = iMonHSEndPos - iMonHSPeakPos;

		horzPos = iMonHSPeakPos - HSyncDuration;

		_renderer.HSyncCycle(horzPos, flag_drawing);

		scrln++;
		scanline++;
		if ((dword)scrln >= MAX_DRAWN) {
			flag_drawing = 0;
		} else {
			flag_drawing = 1;
		}
	}
}

void t_VDU::CheckMaxScanlineCount()
{
	if (scanline == MaxVSync) { // above maximum scanline count?
		frame_finished();
	}
}

void t_VDU::CheckMinScanlineCount()
{
	if (scanline > MinVSync) { // above minimum scanline count?
		frame_finished();
	}
}


void t_VDU::StartHSync()
{
	iMonHSStartPos = 0;
	iMonHSPeakToStart = iMonHSPeakPos;
}

void t_VDU::EndHSync()
{
	int temp;

	iMonHSPeakToEnd = iMonHSPeakPos;
	if (HadP) {
		HadP = 0;
		if (iMonHSPeakPos >= iMonHSStartPos) {
			temp = iMonHSEndPos - HSyncDuration;
			if (temp < MonFreeSync) {
				if (MonFreeSync != MinHSync) {
					MonFreeSync--;
				}
			} else if (temp > MonFreeSync) {
				if (MonFreeSync != MaxHSync) {
					MonFreeSync++;
				}
			}
			temp = iMonHSPeakToEnd - iMonHSEndToPeak;
			if (temp < 0) {
				temp = -temp;
				if (temp > iMonHSStartPos) {
					temp = iMonHSStartPos;
				}
				temp >>= 3;
				if (!temp) {
					temp++;
				}
				if (temp > MAX_SYNC_INC) {
					temp = MAX_SYNC_INC;
				}
				MonHSYNC = MonFreeSync + temp;
				if (MonHSYNC > MaxHSync) {
					MonHSYNC = MaxHSync;
				}
			} else {
				if (temp > iMonHSStartPos) {
					temp = iMonHSStartPos;
				}
				temp >>= 3;
				if (!temp) {
					temp++;
				}
				if (temp > MAX_SYNC_DEC) {
					temp = MAX_SYNC_DEC;
				}
				MonHSYNC = MonFreeSync - temp;
				if (MonHSYNC < MinHSync) {
					MonHSYNC = MinHSync;
				}
			}
		} else {
			temp = iMonHSStartToPeak - iMonHSPeakToEnd;
			if (!temp) {
				MonHSYNC = MonFreeSync;
			} else if (temp < 0) {
				temp = -temp;
				if (temp > iMonHSStartPos) {
					temp = iMonHSStartPos;
				}
				temp >>= 3;
				if (!temp) {
					temp++;
				}
				if (temp > MAX_SYNC_INC) {
					temp = MAX_SYNC_INC;
				}
				MonHSYNC = MonFreeSync + temp;
				if (MonHSYNC > MaxHSync) {
					MonHSYNC = MaxHSync;
				}
			} else if (temp > 0) {
				if (temp > iMonHSStartPos) {
					temp = iMonHSStartPos;
				}
				temp >>= 3;
				if (!temp) {
					temp++;
				}
				if (temp > MAX_SYNC_DEC) {
					temp = MAX_SYNC_DEC;
				}
				MonHSYNC = MonFreeSync - temp;
				if (MonHSYNC < MinHSync) {
					MonHSYNC = MinHSync;
				}
			}
		}
	}
	iMonHSEndPos = 0;
}

void t_VDU::frame_finished(void)
{
	frame_completed = 1; // emulation loop exit condition met
	scrln = -(((scanline - MIN_VHOLD) + 1) >> 1);
	scanline = 0;
	flag_drawing = 0;
}

bool t_VDU::IsFrameFinished()
{
	if (!frame_completed)
		return false;

	frame_completed = 0;
	return true;
}

void t_VDU::SetScanline(int scln)
{
	scanline = scln;
	if (scanline > MaxVSync) {
		scanline = MaxVSync; // limit to max value
	}
}

void t_VDU::DisplayDebug()
{
#ifdef USE_DEBUGGER_CRTC
/*	fprintf(pfoDebug, "mhs%04X mfs%04X  s%04X p%04X e%04X p%04X c%d",

		MonHSYNC,
		MonFreeSync,
		iMonHSStartPos,
		iMonHSPeakPos,
		iMonHSEndPos,
		HorzPos,
		HorzChar);
*/
fprintf(pfoDebug, "mhs%04X mfs%04X  s%04X p%04X e%04X ",
		MonHSYNC,
		MonFreeSync,
		iMonHSStartPos,
		iMonHSPeakPos,
		iMonHSEndPos);
#endif
}
