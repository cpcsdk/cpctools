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

/* Hitachi HD6845S CRT Controller (CRTC Type 0) emulation
based on the CRTC emulation of WinAPE32 v2.0a5b by Richard Wilson

  Oct 16, 2000 - 23:12 started conversion from assembly to C
  Oct 17, 2000 - 19:25 finished converting main CRTC update loop
  Oct 17, 2000 - 22:04 added framework for mode draw handlers
  Oct 25, 2000 - 22:03 changed all CRTC counters to be only 8 bits wide; fixed address calculation
  Oct 30, 2000 - 19:03 fixed CPC screen address line advancement (test should have been for a _reset_ bit!)
  Mar 20, 2001 - 16:00 added draw_mode2
  Jun 20, 2001 - 23:24 added drawing routines for 32bpp video modes
  Jul 04, 2001 - 22:28 16bpp rendering; updated 8bpp code with VDU visible region limiting
  Sep 26, 2002 - 22:39 moved rendering code to separate files; added line doubling (in software) code
  Oct 07, 2002 - 21:58 removed the CPC.scr_max test in write_video_data; added support for variable line spacing

	May 23, 2004 - 17:38 added some preliminary VDU frame cropping to reduce the amount of data written to the video buffer
	May 24, 2004 - 00:44 moved the write_video_data code into the body of access_video_memory

	  Jul 08, 2004 - 16:58 started implementing Richard's assembly CRTC emulation in C
*/

#include <cmath>
#include <memory.h>
#include <iostream>

#include "crtc.h"
#include "gatearray.h"
#include "vdu.h"

#include "misc/synchro.h"

#include "debug.h"

#ifdef USE_DEBUGGER_CRTC
extern dword dwDebugFlag;
extern FILE *pfoDebug;
#endif

using std::cout;
using std::hex;
using std::endl;

void t_CRTC::dispRegs() {
	for(int i = 0; i < 14; i++)
		cout << "crtc reg" << i << " : " << hex << _registers[i] << "\t";
	cout << endl;
}

t_CRTC::t_CRTC(shared_ptr<t_GateArray> ga, shared_ptr<t_VDU> vdu) :
_vdu(vdu),
_gateArray(ga)
{
	for (int l = 0; l < 0x7400; l++)
	{
		int j = l << 1; // actual address
		_maxLate[l] = (j & 0x7FE) | ((j & 0x6000) << 1);
	}

	Reset();
}

void t_CRTC::Reset()
{
	_regSelect = 0;
	_requestedAddr = 0;
	_nextAddr = 0;
	_addr = 0;
	_nextAddress = 0;
	_scrBase = 0;
	_charCount = 0;
	_lineCount = 0;
	_rasterCount = 0;
	_hsw = 0;
	_hswCount = 0;
	_vsw = 0;
	_vswCount = 0;
	_hadHSync = false;
	_inMonHSync = false;
	_inVSync = false;
	_inVerticalTotalAdjust = false;
	_newScan = false;
	_resChar = false;
	_resFrame = false;
	_resNext = false;
	_resScan = false;
	_resVSync = false;
	_startVerticalTotalAdjust = false;
	_lastHEnd = 0;
	_reg5 = 0;
	_r7Match = 0;
	_r9Match = 0;
	_hStart = 0;
	_hEnd = 0;

	_charInstSL = (void(*)(t_CRTC &CRTC))NoChar;
	_charInstMR = (void(*)(t_CRTC &CRTC))NoChar;

    // register 8 is used for udpdating skew, so set it first !
	SetRegisterValue(8, 0);

	SetRegisterValue(0, 63);
	SetRegisterValue(1, 40);
	SetRegisterValue(2, 46);
	SetRegisterValue(3, 0x8e);
	SetRegisterValue(4, 38);
	SetRegisterValue(5, 0);
	SetRegisterValue(6, 25);
	SetRegisterValue(7, 30);
	SetRegisterValue(9, 7);
	SetRegisterValue(12, 0x30);
	SetRegisterValue(13, 0x00);

	_flags1.monVSYNC = 0;
	_flags1.inHSYNC = 0;
	_flags1.dt.DISPTIMG = 0xff;
	_flags1.dt.HDSPTIMG = 0x03;
	_newDT.NewDISPTIMG = 0xff;
	_newDT.NewHDSPTIMG = 0x03;
}

void t_CRTC::Emulate(int repeat_count)
{
	while (repeat_count) {

		_vdu->Render(_nextAddress, _flags1.combined);

		_nextAddress = _maxLate[(_addr + _charCount) & 0x73ff] | _scrBase; // next address for PreRender
		_flags1.dt.combined = _newDT.combined; // update the DISPTMG flags

#ifdef USE_DEBUGGER_CRTC
		if (dwDebugFlag)
		{
			DisplayDebug();
		}
#endif

		// ----------------------------------------------------------------------------

		// matches horizontal total?
		if (_charCount == _registers[0])
		{
			_lastHEnd = _charCount; // preserve current line length in chars
			_newScan = true; // request starting a new scan line
			_charCount = 0; // reset the horizontal character count
		}
		else
		{
			_charCount++; // update counter
			_charCount &= 255; // limit to 8 bits
		}

		// matches horizontal total?
		if (_charCount == _registers[0])
		{
			// matches maximum raster address?
			if (_rasterCount == _registers[9])
			{
				_resChar = true; // request a line count update
			}
			else
			{
				_resChar = false; // still within the current character line
			}
			// ready to restart frame?
			if (_resNext)
			{
				_resNext = false;
				_resFrame = true; // request a frame restart
			}
			// ready to start vertical total adjust?
			if (_startVerticalTotalAdjust)
			{
				_startVerticalTotalAdjust = false;
				_inVerticalTotalAdjust = true; // entering vertical total adjust
			}

			// in vertical total adjust?
			if (_inVerticalTotalAdjust)
			{
				if ((_rasterCount == _registers[9]) && (_lineCount == _registers[4]))
				{
					_resScan = true; // raster counter only resets once at start of vta
				}
				else
				{
					_resScan = false; // raster counter keeps increasing while in vta
				}
			}
		}

		// matches horizontal displayed?
		if (_charCount == _registers[1])
		{
			// matches maximum raster address?
			if (_rasterCount == _registers[9])
			{
				_nextAddr = _addr + _charCount;
			}
		}

		// not in HSYNC?
		if (!_flags1.inHSYNC)
		{
			// matches horizontal sync position?
			if (_charCount == _registers[2])
			{
				_flags1.inHSYNC = 0xff; // turn HSYNC on
				_hadHSync = true; // prevent GA from processing more than one HSYNC per scan line
				_hswCount = 0; // initialize horizontal sync width counter
				MatchHsw();
			}
		}
		else
		{
			MatchHsw();
		}

		_charInstSL(*this); // if necessary, process vertical total delay
		_charInstMR(*this); // if necessary, process maximum raster count delay

		// scanline change requested?
		if (_newScan)
		{
			_newScan = false;
			_addr = _nextAddr;

			// VSYNC active?
			if (_inVSync)
			{
				_vswCount++; // update counter
				_vswCount &= 31; // limit to 5 bits (to handle "double vbl" case)
				// matches vertical sync width?
				if (_vswCount == _vsw)
				{
					_vswCount = 0; // reset counter
					_resVSync = true; // request VSYNC reset
				}
			}

			// frame restart requested?
			if (_resFrame)
			{
				RestartFrame();
			}
			else
			{
				// raster counter reset requested?
				if (_resScan)
				{
					_resScan = false;
					_rasterCount = 0; // reset counter
					_scrBase = 0;
				}
				else
				{
					_rasterCount++; // update counter
					_rasterCount &= 31; // limit to 5 bits
					// did the counter wrap around?
					if (!_rasterCount)
					{
						MatchLineCount();
					}
					_scrBase = (_scrBase + 0x0800) & 0x3800;
				}
			}

			_charInstSL = (void(*)(t_CRTC &CRTC))CharSL1;

			register dword temp = 0;
			// matches maximum raster address?
			if (_rasterCount == _registers[9])
			{
				temp = 1;
				_resScan = true; // request a raster counter reset
			}
			if (_r9Match != temp)
			{
				_r9Match = temp;
			}
			if (temp)
			{
				_charInstMR = (void(*)(t_CRTC &CRTC))CharMR1;
			}

			// in vertical total adjust?
			if (_inVerticalTotalAdjust)
			{
				// matches vertical total adjust?
				if (_rasterCount == _reg5)
				{
					RestartFrame();
					// maximum raster address is zero?
					if (_registers[9] == 0)
					{
						_resScan = true; // request a raster counter reset
					}
				}
			}

			// line count update requested?
			if (_resChar)
			{
				_lineCount++; // update counter
				_lineCount &= 127; // limit to 7 bits
				ReloadAddr();
			}

			// in VSYNC?
			if (_inVSync)
			{
				// end of VSYNC?
				if (_resVSync)
				{
					_inVSync = false; // turn VSYNC off
					_resVSync = false;

					_vdu->CheckMaxScanlineCount();
				}
				else
				{
					_vdu->CheckMinScanlineCount();
				}
			} else
			{
				_vdu->CheckMaxScanlineCount();
			}
		}

		// leaving border area?
		if (_charCount == _hStart)
		{
			_newDT.NewHDSPTIMG |= 0x01;
		}
		// entering border area?
		if (_charCount == _hEnd)
		{
			_newDT.NewHDSPTIMG &= 0xfe;
		}

		// ----------------------------------------------------------------------------

		repeat_count--;
   }
}

void t_CRTC::RegisterSelect(unsigned char val)
{
	_regSelect = val;
}
unsigned char t_CRTC::GetRegisterSelect() const
{
	return _regSelect;
}

unsigned char t_CRTC::GetRegisterValue(unsigned char reg) const
{
	return _registers[reg];
}

void t_CRTC::setReg6(unsigned char val) {
	_registers[6] = val & 0x7f;
	if (_lineCount == _registers[6]) { // matches vertical displayed?
		_newDT.NewDISPTIMG = 0;
	}
}

void t_CRTC::SetRegisterValue(unsigned char reg, unsigned char val)
{
	if (reg < 16)
	{ // only _registers 0 - 15 can be written to
		switch (reg) {
		case 0: // horizontal total
			_registers[0] = val;
			break;
		case 1: // horizontal displayed
			_registers[1] = val;
			UpdateSkew();
			break;
		case 2: // horizontal sync position
			_registers[2] = val;
			break;
		case 3: // sync width
			setReg3(val);
			break;
		case 4: // vertical total
			setReg4(val);
			break;
		case 5: // vertical total adjust
			_registers[5] = val & 0x1f;
			break;
		case 6: // vertical displayed
			setReg6(val);
			break;
		case 7: // vertical sync position
			_registers[7] = val & 0x7f;
			{
				register dword temp = 0;
				// matches vertical sync position?
				if (_lineCount == _registers[7])
				{
					temp++;
					if (_r7Match != temp)
					{
						_r7Match = temp;
						if (_charCount >= 2)
						{
							_resVSync = false;
							if (!_inVSync)
							{
								_vswCount = 0;
								_inVSync = true;
								_flags1.monVSYNC = 26;
								_gateArray->SetHSCount( 2 ); // GA delays its VSYNC by two CRTC HSYNCs
							}
						}
					}
				}
				else
				{
					_r7Match = 0;
				}
			}
			break;
		case 8: // interlace and skew
			_registers[8] = val;
			UpdateSkew();
			break;
		case 9: // maximum raster count
			setReg9(val);
			break;
		case 10: // cursor start raster
			_registers[10] = val & 0x7f;
			break;
		case 11: // cursor end raster
			_registers[11] = val & 0x1f;
			break;
		case 12: // start address high byte
			setReg12(val);
			break;
		case 13: // start address low byte
			setReg13(val);
			break;
		case 14: // cursor address high byte
			_registers[14] = val & 0x3f;
			break;
		case 15: // cursor address low byte
			_registers[15] = val;
			break;
		}
	}
}

void t_CRTC::setReg9(unsigned char val)
{
	_registers[9] = val & 0x1f;

	register dword temp = 0;
	// matches maximum raster address?
	if (_rasterCount == _registers[9])
	{
		temp = 1;
		_resScan = true; // request a raster counter reset
	}
	if (_r9Match != temp)
	{
		_r9Match = temp;
		if (temp)
		{
			_charInstMR = (void(*)(t_CRTC &CRTC))CharMR1;
		}
	}
	// matches maximum raster address?
	if (_rasterCount == _registers[9])
	{
		if (_charCount == _registers[1])
		{
			_nextAddr = _addr + _charCount;
		}
		// matches horizontal total?
		if (_charCount == _registers[0])
		{
			_resChar = true; // request a line count update
		}
		if (!_startVerticalTotalAdjust)
		{
			_resScan = true;
		}
	}
	else
	{
		// not in vertical total adjust?
		if (!_inVerticalTotalAdjust)
		{
			_resScan = false;
		}
	}
}

void t_CRTC::setReg4(unsigned char val)
{
	_registers[4] = val & 0x7f;
	if (_charInstMR == (void(*)(t_CRTC &CRTC))CharMR2)
	{
		// matches vertical total?
		if (_lineCount == _registers[4])
		{
			// matches maximum raster address?
			if (_rasterCount == _registers[9])
			{
				_startVerticalTotalAdjust = true;
			}
		}
	}
}

void t_CRTC::setReg12(unsigned char val)
{
	_registers[12] = val & 0x3f;
	_requestedAddr = _registers[13] + (_registers[12] << 8);
}

void t_CRTC::setReg13(unsigned char val)
{
	_registers[13] = val;
	_requestedAddr = _registers[13] + (_registers[12] << 8);
}

void t_CRTC::setReg3(unsigned char val)
{
	_registers[3] = val;
	_hsw = val & 0x0f; // isolate horizontal sync width
	_vsw = val >> 4; // isolate vertical sync width
	if(_vsw==0) _vsw = 16;
}

void t_CRTC::WriteData(unsigned char val)
{
	SetRegisterValue(_regSelect, val);
}

unsigned char t_CRTC::ReadData() const
{
	std::cout << "CRTC Get " << (int)_regSelect << ">" << std::hex << (int)_registers[_regSelect] << std::endl;
	if ((_regSelect > 11) && (_regSelect < 18))
	{ // valid range?
		return _registers[_regSelect];
	}
	else
	{
		return 0; // write only _registers return 0
	}
}

void t_CRTC::UpdateSkew()
{
	_newDT.NewHDSPTIMG |= 0x02; // enable horizontal DISPTMG by default
	dword skew = (_registers[8] >> 4) & 3; // isolate the display skew
	if (skew == 3) { // disable output?
		_newDT.NewHDSPTIMG &= 0xfd; // disable horizontal DISPTMG
	} else {
		_hStart = skew; // position at which horizontal display starts
		_hEnd = _hStart + _registers[1]; // position at which it ends
	}
}



void t_CRTC::ChangeMode()
{
	// have we had an HSYNC on this scan line?
	if (_hadHSync)
	{
		_hadHSync = false;
		_gateArray->HSyncChangeMode();
	}
}


void t_CRTC::MatchLineCount()
{
	// matches vertical displayed?
	if (_lineCount == _registers[6])
	{
		_newDT.NewDISPTIMG = 0; // disable vertical DISPTMG
	}
	// matches vertical sync position?
	if (_lineCount == _registers[7])
	{
		// line length was at least 2 chars?
		if (_lastHEnd >= 2)
		{
			if (!_r7Match)
			{
				_resVSync = false;
				// not in VSYNC?
				if (!_inVSync)
				{
					_vswCount = 0; // reset vertical sync width counter
					_inVSync = true; // enter VSYNC
					_flags1.monVSYNC = 26; // enter vertical blanking period for 26 scanlines
					_gateArray->SetHSCount( 2 ); // GA delays its VSYNC by two CRTC HSYNCs
				}
			}
		}
		_r7Match = 1;
	}
	else
	{
		_r7Match = 0;
	}
}



void t_CRTC::ReloadAddr()
{
	if (_lineCount == 0) { // has line count been reset?
		_newDT.NewDISPTIMG = 0xff; // enable vertical DISPTMG
		_addr =	_nextAddr = _requestedAddr; // load with screen start address
	}
	MatchLineCount();
}



void t_CRTC::RestartFrame()
{
	_inVerticalTotalAdjust = false;
	_resFrame = false;
	_resScan = false;
	_resChar = false;
	_rasterCount = 0; // reset raster line counter
	_scrBase = 0;
	_lineCount = 0; // reset character line counter
	ReloadAddr();
}



void t_CRTC::MatchHsw()
{
	// matches horizontal sync width?
	if (_hswCount == _hsw)
	{
		_gateArray->MatchHsw();
		_flags1.inHSYNC = 0; // turn HSYNC off

		// in vertical blanking period?
		if (_flags1.monVSYNC)
		{
			_flags1.monVSYNC--; // update counter
		}
		ChangeMode(); // process possible mode change
		// monitor HSYNC still active?
		if (_inMonHSync)
		{
			_inMonHSync = false;
			_vdu->EndHSync();
		}
	}
	else
	{
		_hswCount++; // update counter
		_hswCount &= 15; // limit to 4 bits

		// ready to start monitor HSYNC?
		if (_hswCount == 3)
		{
			_inMonHSync = true; // enter monitor HSYNC
			_vdu->StartHSync();
		}
		// reached GA HSYNC output cutoff?
		else if (_hswCount == 7)
		{
			ChangeMode();
			_inMonHSync = false;
			_vdu->EndHSync();
		}
	}
}

void t_CRTC::CharSL2(t_CRTC &CRTC)
{
	CRTC._reg5 = CRTC._registers[5];
	CRTC._charInstSL = (void(*)(t_CRTC &CRTC))NoChar;
}



void t_CRTC::CharSL1(t_CRTC &CRTC)
{
	CRTC._charInstSL = (void(*)(t_CRTC &CRTC))CharSL2;
}



void t_CRTC::CharMR2(t_CRTC &CRTC)
{
	// starting vertical total adjust?
	if (CRTC._startVerticalTotalAdjust)
	{
		// matches vertical total?
		if (CRTC._lineCount == CRTC._registers[4])
		{
			// no vertical total adjust?
			if (CRTC._registers[5] == 0)
			{
				CRTC._resNext = true; // request a frame restart
			}
		}
	}
	CRTC._charInstMR = (void(*)(t_CRTC &CRTC))NoChar;
}



void t_CRTC::CharMR1(t_CRTC &CRTC)
{
	if ((CRTC._rasterCount == CRTC._registers[9]) && (CRTC._lineCount == CRTC._registers[4]))
	{
		CRTC._inVerticalTotalAdjust = false;
		CRTC._startVerticalTotalAdjust = true; // request start of vertical total adjust
	}
	else
	{
		CRTC._startVerticalTotalAdjust = false; // not yet at end of frame
	}
	CRTC._charInstMR = (void(*)(t_CRTC &CRTC))CharMR2;
}

void t_CRTC::DisplayDebug() const
{
#ifdef USE_DEBUGGER_CRTC
	char str[16];
	char on[]  = "vhDDHVMa";
	char off[] = "........";

	if (GetFlagInVSync()) {
		str[0] = on[0];
	} else {
		str[0] = off[0];
	}
	if (GetFlag1InHSync()) {
		str[1] = on[1];
	} else {
		str[1] = off[1];
	}
	if (_flags1.dt.HDSPTIMG & 1) {
		str[2] = on[2];
	} else {
		str[2] = off[2];
	}
	if (_flags1.dt.DISPTIMG) {
		str[3] = on[3];
	} else {
		str[3] = off[3];
	}
	if (_newScan) {
		str[4] = on[4];
	} else {
		str[4] = off[4];
	}
	if (_resFrame) {
		str[5] = on[5];
	} else {
		str[5] = off[5];
	}
	if (_resScan) {
		str[6] = on[6];
	} else {
		str[6] = off[6];
	}
	if (_inVerticalTotalAdjust) {
		str[7] = on[7];
	} else {
		str[7] = off[7];
	}
	str[8] = '\0';

	fprintf(pfoDebug, "CRTC : %04X | CC:%2X RC:%2X LC:%2X - HSWC:%2X VSWC:%2X - %s",
		_nextAddress,
		_charCount,
		_rasterCount,
		_lineCount,
		_hswCount,
		_vswCount,
		str);

	_vdu->DisplayDebug();

	fprintf(pfoDebug, "\n");
#endif
}
