//
// Caprice 32 I/O port emulation
//

#ifndef _IOPORT_H_
#define _IOPORT_H_

#include "cap32type.h"

#include <memory>

using std::shared_ptr;
using std::weak_ptr;

class Emulator;

class IOPort
{
private:
	weak_ptr<Emulator>		emulator;
public:
	IOPort(shared_ptr<Emulator> emulator);

	byte z80_IN_handler(reg_pair port);
	void z80_OUT_handler(reg_pair port, byte val);
};

#endif
