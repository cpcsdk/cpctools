//
// Caprice32 PPI emulator
//

#ifndef _PPI_H_
#define _PPI_H_

class t_PPI
{
public:
   unsigned char control;
   unsigned char portA;
   unsigned char portB;
   unsigned char portC;

public:
	t_PPI();

	void Reset();
};

#endif
