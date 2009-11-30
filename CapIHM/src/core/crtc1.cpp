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
	{
		return 0; // write only _registers return 0
	}
}

void t_CRTC1::setReg3(unsigned char val)
{
	_registers[3] = val;
	_hsw = val & 0x0f; // isolate horizontal sync width
	_vsw = 16; // isolate vertical sync width (broken on type 1)
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
	return 0 + 0 + (_inVSync << 5);
};

// TODO
// R1 > R0 removes all border
// R8 interleave mode gives a correct 100Hz screen
// R8 bits 4 and 5 does not allow to shift the border
//
// Split border is done with r6, not r8
// No "display enable" bug (same border problem as above ?)
// Offset reg can be changed anytime during first block
// R9 and R4 changes are unbuffered
// R0 minimal value is 0, not 1
