#include "crtc1.h"

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
