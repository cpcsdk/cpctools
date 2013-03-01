#include "crtc1.h"
#include "vdu.h"

unsigned char t_CRTC1::ReadData() const
{
	// Reg 12 and 13 are not readable on type 1 and 2, you get 0.
	if ((_regSelect > 13) && (_regSelect < 18))
	{ // valid range?
		return _registers[_regSelect];
	} else if (_regSelect == 31)
		return 255; // Hi-Z state! (should be random, but well...)
	else
		return 0; // write only _registers return 0
}

void t_CRTC1::setReg3(unsigned char val)
{
	_registers[3] = val;
	_hsw = val & 0x0f; // isolate horizontal sync width
	_vsw = 16; // isolate vertical sync width (broken on type 1)
}

void t_CRTC1::setReg4(unsigned char val)
{
	_registers[4] = val & 0x7f;
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


void t_CRTC1::setReg6(unsigned char val) {
	_registers[6] = val & 0x7f;
	if (_lineCount >= _registers[6]) { // matches vertical displayed?
		_newDT.NewDISPTIMG = 0;
	} else
		_newDT.NewDISPTIMG = 0xFF;
}

void t_CRTC1::setReg9(unsigned char val)
{
	_registers[9] = val & 0x1f;

	register dword temp = 0;
	// matches maximum raster address?
	// (also allow changes when in first line)
	if (_rasterCount == _registers[9] || _rasterCount == 0)
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
	if (_rasterCount == _registers[9] || _rasterCount == 0)
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


void t_CRTC1::Emulate(int repeat_count)
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

		// ready to start vertical total adjust?
		if (_startVerticalTotalAdjust)
		{
			_startVerticalTotalAdjust = false;
			_inVerticalTotalAdjust = true; // entering vertical total adjust
		}
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

unsigned char t_CRTC1::ReadStatus() const {
	// Bit 7 = Update Ready (TODO)
	// 1 on update strobe
	// 0 on R31 access
	//
	// Bit 6 = Lightpen (TODO)
	// 1 on LPEN strobe
	// 0 on R16/17 access
	//
	// Bit 5 = VBL
	// 1 = VBL is running now
	// 0 = no VBL is running
	return (1<<7) + 0 + (_inVSync << 5);
};

void t_CRTC1::setReg12(unsigned char val)
{
			_registers[12] = val & 0x3f;
			_requestedAddr = _registers[13] + (_registers[12] << 8);
			if(_lineCount == 0)
			{
				_nextAddr = _requestedAddr; // load with screen start address
				MatchLineCount();
			}
}

void t_CRTC1::setReg13(unsigned char val)
{
			_registers[13] = val;
			_requestedAddr = _registers[13] + (_registers[12] << 8);
			if(_lineCount == 0)
			{
				_nextAddr = _requestedAddr; // load with screen start address
				MatchLineCount();
			}
}

// TODO
// R1 > R0 removes all border
// R8 interleave mode gives a correct 100Hz screen
// R8 bits 4 and 5 does not allow to shift the border
//
// Split border is done with r6, not r8
// R9 and R4 changes are unbuffered
// R0 minimal value is 0, not 1
