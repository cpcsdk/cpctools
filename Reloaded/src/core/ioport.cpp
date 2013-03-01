/*
 *   $Id: ioport.cpp 60 2008-11-25 17:08:52Z giot.romain $
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
// Caprice 32 I/O port emulation
//

#include "ioport.h"
#include "cap32type.h"

#include "emulator.h"
#include "z80.h"
#include "crtc.h"
#include "ppi.h"
#include "psg.h"
#include "fdc.h"
#include "gatearray.h"
#include "memory.h"
#include "tape.h"
#include "input.h"

#include "debug.h"

#ifdef USE_DEBUGGER
extern dword dwDebugFlag;
extern FILE *pfoDebug;
#endif

extern byte *pbMF2ROM;

extern FILE *pfoPrinter;

extern dword dwMF2Flags;

IOPort::IOPort(shared_ptr<Emulator> emulator) :
emulator(emulator)
{
}

byte IOPort::z80_IN_handler(reg_pair port)
{
    auto emulator = this->emulator.lock();
    auto CPC = emulator->GetConfig();
    auto CRTC = emulator->GetCRTC();
    auto FDC = emulator->GetFDC();
    auto PPI = emulator->GetPPI();
	byte ret_val;
	
	// default return value
	ret_val = 0xff;
	
	// CRTC -----------------------------------------------------------------------
	// CRTC chip select?
	if (!(port.b.h & 0x40)) 
	{
		// read CRTC register?
		if ((port.b.h & 3) == 3) 
		{
			ret_val = CRTC->ReadData();
		} else if ((port.b.h & 3) == 2)
			ret_val = CRTC->ReadStatus();
	}
	
	// PPI ------------------------------------------------------------------------
	// PPI chip select?
	else if (!(port.b.h & 0x08)) 
	{
		byte ppi_port = port.b.h & 3;
		switch (ppi_port) 
		{
			// read from port A?
		case 0:
			{
				// port A set to input?
				if (PPI->control & 0x10) 
				{
                    auto Input = emulator->GetInput();
                    auto PSG = emulator->GetPSG();
					// PSG control set to read?
					if ((PSG->GetControl() & 0xc0) == 0x40) 
					{
						// within valid range?
						if (PSG->GetRegSelect() < 16) 
						{
							// PSG port A?
							if (PSG->GetRegSelect() == 14) 
							{
								// port A in input mode?
								if (!(PSG->GetAYRegister(7) & 0x40)) 
								{
									// read keyboard matrix node status
									ret_val = Input->keyboard_matrix[CPC->keyboard_line & 0x0f];
								} 
								else 
								{
									// return last value w/ logic AND of input
									ret_val = PSG->GetAYRegister(14) & (Input->keyboard_matrix[CPC->keyboard_line & 0x0f]);
								}
							} 
							// PSG port B?
							else if (PSG->GetRegSelect() == 15) 
							{
								// port B in output mode?
								if ((PSG->GetAYRegister(7) & 0x80)) 
								{
									// return stored value
									ret_val = PSG->GetAYRegister(15);
								}
							} 
							else 
							{
								// read PSG register
								ret_val = PSG->GetAYRegister(PSG->GetRegSelect());
							}
						}
					}
				} 
				else 
				{
					// return last programmed value
					ret_val = PPI->portA;
				}
				break;
			}
			
			// read from port B?
		case 1:
			{
				// port B set to input?
				if (PPI->control & 2)
				{
                    auto Tape = emulator->GetTape();
					// tape level when reading
					ret_val = Tape->GetTapeLevel() |
						// ready line of connected printer
						(CPC->printer ? 0 : 0x40) |
						// manufacturer + 50Hz
						(CPC->jumpers & 0x7f) |
						// VSYNC status
						(CRTC->GetFlagInVSync() ? 1 : 0);
					
				} 
				else 
				{
					// return last programmed value
					ret_val = PPI->portB;
				}
				break;
			}
			
			// read from port C?
		case 2:
			{
				// isolate port C directions
				byte direction = PPI->control & 9;
				// default to last programmed value
				ret_val = PPI->portC;
				// either half set to input?
				if (direction) 
				{
					// upper half set to input?
					if (direction & 8) 
					{
						// blank out upper half
						ret_val &= 0x0f;
						// isolate PSG control bits
						byte val = PPI->portC & 0xc0;
						// PSG specify register?
						if (val == 0xc0) 
						{
							// change to PSG write register
							val = 0x80;
						}
						// casette write data is always set
						ret_val |= val | 0x20;
						if (CPC->tape_motor) 
						{
							// set the bit if the tape motor is running
							ret_val |= 0x10;
						}
					}
					if (!(direction & 1)) { // lower half set to output?
						ret_val |= 0x0f; // invalid - set all bits
					}
				}
				break;
			}
		}
	}
	// ----------------------------------------------------------------------------
	// external peripheral?
	else if (!(port.b.h & 0x04)) 
	{
		// FDC?
		if ((port.b.h == 0xfb) && (!(port.b.l & 0x80))) 
		{
			
			// FDC status register?
			if (!(port.b.l & 0x01)) 
			{
			
				ret_val = FDC->fdc_read_status();
			} 
			else 
				// FDC data register
			{
				ret_val = FDC->fdc_read_data();
			}

		}else if( (port.b.h == 0xfa) && (port.b.l == 0xfe ))
		{


			ret_val = FDC->fdc_read_status();
		}
	}
	return ret_val;
}

void IOPort::z80_OUT_handler(reg_pair port, byte val)
{
    auto emulator = this->emulator.lock();
    auto CPC = emulator->GetConfig();
    auto CRTC = emulator->GetCRTC();
    auto FDC = emulator->GetFDC();
    auto GateArray = emulator->GetGateArray();
    auto Memory = emulator->GetMemory();
    auto PPI = emulator->GetPPI();
    auto PSG = emulator->GetPSG();

	// Gate Array -----------------------------------------------------------------
	// Memory PAL
	if ((port.b.h & 0x80) == 0)
	{
		// set memory configuration
		if(val>>6 == 3)
		{
#ifdef USE_DEBUGGER_GA
			if (dwDebugFlag) 
			{
				fprintf(pfoDebug, "IOGA : mem 0x%02x\r\n", val);
			}
#endif
			Memory->SetRAMConfig( val );
			Memory->ga_memory_manager();

			// MF2 enabled?
			if (CPC->mf2) 
			{
				*(pbMF2ROM + 0x03fff) = val;
			}
		}
	}

	// GA chip select?
	if ((port.b.h & 0xc0) == 0x40) 
	{
		switch (val >> 6) 
		{
			// select pen
		case 0:
			{
#ifdef USE_DEBUGGER_GA
				if (dwDebugFlag) 
				{
					fprintf(pfoDebug, "IOGA : pen 0x%02x\r\n", val);
				}
#endif
				// if bit 5 is set, pen indexes the border colour
				GateArray->SelectPen( val & 0x10 ? 0x10 : val & 0x0f );
				// MF2 enabled?
				if (CPC->mf2) 
				{
					*(pbMF2ROM + 0x03fcf) = val;
				}
				break;
			}
			// set colour
		case 1:
			{
#ifdef USE_DEBUGGER_GA
				if (dwDebugFlag) 
				{
					fprintf(pfoDebug, "IOGA : clr 0x%02x\r\n", val);
				}
#endif
				{
					// isolate colour value
					byte colour = val & 0x1f;
					
					GateArray->SetColour( colour );
				}
				// MF2 enabled?
				if (CPC->mf2) 
				{
					int iPen = *(pbMF2ROM + 0x03fcf);
					*(pbMF2ROM + (0x03f90 | ((iPen & 0x10) << 2) | (iPen & 0x0f))) = val;
				}
				break;
			}
			// set mode
		case 2:
			{
#ifdef USE_DEBUGGER_GA
				if (dwDebugFlag) 
				{
					fprintf(pfoDebug, "IOGA : rom 0x%02x\r\n", val);
				}
#endif
				Memory->SetROMConfig( val ) ;
				// request a new CPC screen mode
				GateArray->SetMode( val & 0x03 );
				Memory->ga_memory_manager();
				// delay Z80 interrupt?
				if (val & 0x10) 
				{
					// clear pending interrupts
					(&emulator->GetZ80())->int_pending = 0;
					// reset GA scanline counter
					GateArray->SetSLCount( 0 );
				}
				// MF2 enabled?
				if (CPC->mf2) 
				{
					*(pbMF2ROM + 0x03fef) = val;
				}
				break;
			}
		}
	}
	
	// CRTC -----------------------------------------------------------------------
	// CRTC chip select?
	if (!(port.b.h & 0x40)) 
	{
		byte crtc_port = port.b.h & 3;
		// CRTC register select?
		if (crtc_port == 0) 
		{
			CRTC->RegisterSelect(val);
			// MF2 enabled?
			if (CPC->mf2) 
			{
				*(pbMF2ROM + 0x03cff) = val;
			}
		}
		// CRTC write data?
		else if (crtc_port == 1) 
		{
			CRTC->WriteData(val);
			// MF2 enabled?
			if (CPC->mf2) 
			{
				*(pbMF2ROM + (0x03db0 | (*(pbMF2ROM + 0x03cff) & 0x0f))) = val;
			}
#ifdef USE_DEBUGGER_CRTC
			if (dwDebugFlag) 
			{
				fprintf(pfoDebug, "IOCRTC : %02x = %02x\r\n", CRTC.GetRegisterSelect(), val);
			}
#endif
		}
	}
	
	// ROM select -----------------------------------------------------------------
	// ROM select?
	if (!(port.b.h & 0x20)) 
	{
		Memory->SetUpperROM( val );

		// MF2 enabled?
		if (CPC->mf2) 
		{
			*(pbMF2ROM + 0x03aac) = val;
		}
	}
	
	// printer port ---------------------------------------------------------------
	// printer port?
	if (!(port.b.h & 0x10)) 
	{
		// invert bit 7
		CPC->printer_port = val ^ 0x80;
		if (pfoPrinter) 
		{
			// only grab data bytes; ignore the strobe signal
			if (!(CPC->printer_port & 0x80)) 
			{
				// capture printer output to file
				fputc(CPC->printer_port, pfoPrinter);
			}
		}
	}
	
	// PPI ------------------------------------------------------------------------
	// PPI chip select?
	if (!(port.b.h & 0x08)) 
	{
		switch (port.b.h & 3) 
		{
			// write to port A?
		case 0:
			{
				PPI->portA = val;
				// port A set to output?
				if (!(PPI->control & 0x10)) 
				{
					PSG->psg_write(val);
				}
				break;
			}
			// write to port B?
		case 1:
			{
				PPI->portB = val;
				break;
			}
			// write to port C?
		case 2:
			{
				PPI->portC = val;
				// output lower half?
				if (!(PPI->control & 1)) 
				{
					CPC->keyboard_line = val;
				}
				// output upper half?
				if (!(PPI->control & 8)) 
				{
					// update tape motor control
					CPC->tape_motor = val & 0x10;
					// change PSG control
					PSG->SetControl(val);
					PSG->psg_write(PPI->portA);
				}
				break;
			}
			// modify PPI control
		case 3:
			{
				// change PPI configuration
				if (val & 0x80) 
				{
					// update control byte
					PPI->control = val;
					// clear data for all ports
					PPI->portA = 0;
					PPI->portB = 0;
					PPI->portC = 0;
				} 
				// bit manipulation of port C data
				else 
				{
					// set bit?
					if (val & 1) 
					{
						// isolate bit to set
						byte bit = (val >> 1) & 7;
						// set requested bit
						PPI->portC |= (1 << bit);
						// output lower half?
						if (!(PPI->control & 1)) 
						{
							CPC->keyboard_line = PPI->portC;
						}
						// output upper half?
						if (!(PPI->control & 8)) 
						{
							CPC->tape_motor = PPI->portC & 0x10;
							// change PSG control
							PSG->SetControl(PPI->portC);
							PSG->psg_write(PPI->portA);
						}
					}
					else 
					{
						// isolate bit to reset
						byte bit = (val >> 1) & 7;
						// reset requested bit
						PPI->portC &= ~(1 << bit);
						// output lower half?
						if (!(PPI->control & 1))
						{
							CPC->keyboard_line = PPI->portC;
						}
						// output upper half?
						if (!(PPI->control & 8))
						{
							CPC->tape_motor = PPI->portC & 0x10;
							// change PSG control
							PSG->SetControl(PPI->portC);
							PSG->psg_write(PPI->portA);
						}
					}
				}
				// MF2 enabled?
				if (CPC->mf2) 
				{
					*(pbMF2ROM + 0x037ff) = val;
				}
				break;
			}
		}
	}

	// ----------------------------------------------------------------------------
	// floppy motor control?
	if ((port.b.h == 0xfa) && (!(port.b.l & 0x80))) 
	{
		FDC->SetMotor(val & 0x01);
#ifdef USE_DEBUGGER_FDC
		if (dwDebugFlag)
		{
			fputs(FDC->GetMotor() ? "IOFDC : --- motor on\n" : "IOFDC : --- motor off\n", pfoDebug);
		}
#endif

#ifndef HAVE_LIB765_H
		FDC->SetFlags(FDC->GetFlags() | STATUSDRVA_flag | STATUSDRVB_flag);
#endif
	}
	// FDC data register?
	else if ((port.b.h == 0xfb) && (!(port.b.l & 0x80))) 
	{
		FDC->fdc_write_data(val);

	}
	// Multiface 2?
	else if ((CPC->mf2) && (port.b.h == 0xfe))
	{
		// page in MF2 ROM?
		if ((port.b.l == 0xe8) && (!(dwMF2Flags & MF2_INVISIBLE))) 
		{
			dwMF2Flags |= MF2_ACTIVE;
			Memory->ga_memory_manager();
		}
		// page out MF2 ROM?
		else if (port.b.l == 0xea) 
		{
			dwMF2Flags &= ~MF2_ACTIVE;
			Memory->ga_memory_manager();
		}
	}
}
