//
// Caprice32 PPI emulator
//

#include "ppi.h"

t_PPI::t_PPI() :
control(0),
portA(0),
portB(0),
portC(0)
{
}

void t_PPI::Reset()
{
	control = 0;
	portA = 0;
	portB = 0;
	portC = 0;
}
