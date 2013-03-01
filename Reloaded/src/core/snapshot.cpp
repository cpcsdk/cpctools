/*
 *   $Id: snapshot.cpp 60 2008-11-25 17:08:52Z giot.romain $
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
// Caprice32 Snapshot manager
//

#include "snapshot.h"
#include "cap32.h"
#include "cap32type.h"
#include "error.h"
#include "z80.h"
#include "psg.h"
#include "fdc.h"
#include "crtc.h"
#include "gatearray.h"
#include "memory.h"
#include "ppi.h"
#include "vdu.h"

#include "emulator.h"

#define __rA        z80.AF.b.h
#define __rF        z80.AF.b.l
#define __rAF       z80.AF.w.l
#define __rAFdword  z80.AF.d
#define __rB        z80.BC.b.h
#define __rC        z80.BC.b.l
#define __rBC       z80.BC.w.l
#define __rBCdword  z80.BC.d
#define __rD        z80.DE.b.h
#define __rE        z80.DE.b.l
#define __rDE       z80.DE.w.l
#define __rDEdword  z80.DE.d
#define __rH        z80.HL.b.h
#define __rL        z80.HL.b.l
#define __rHL       z80.HL.w.l
#define __rHLdword  z80.HL.d
#define __rPC       z80.PC.w.l
#define __rPCdword  z80.PC.d
#define __rSP       z80.SP.w.l

#define __rIXh      z80.IX.b.h
#define __rIXl      z80.IX.b.l
#define __rIX       z80.IX.w.l
#define __rIXdword  z80.IX.d
#define __rIYh      z80.IY.b.h
#define __rIYl      z80.IY.b.l
#define __rIY       z80.IY.w.l
#define __rIYdword  z80.IY.d

#define __rI        z80.I
#define __rR        z80.R
#define __rRb7      z80.Rb7
#define __rIFF1     z80.IFF1
#define __rIFF2     z80.IFF2
#define __rIM       z80.IM
#define __rHALT     z80.HALT

FILE *pSNAfileObject;
FILE *pMemoryfileObject;

int snapshot_load (Emulator &emulator, const char *pchFileName)
{
	auto GateArray = emulator.GetGateArray();
	auto CRTC = emulator.GetCRTC();
	auto VDU = emulator.GetVDU();
	t_z80regs& z80 = emulator.GetZ80();
	auto Memory = emulator.GetMemory();
    auto CPC = emulator.GetConfig();
	auto FDC = emulator.GetFDC();
	auto PSG = emulator.GetPSG();
	auto PPI = emulator.GetPPI();

	int n;
	dword dwSnapSize, dwModel, dwFlags;
	char chPath[_MAX_PATH + 1];
	byte val;
	reg_pair port;
	t_SNA_header sh;
	
	memset(&sh, 0, sizeof(sh));
	if ((pSNAfileObject = fopen(pchFileName, "rb")) != NULL) 
	{
		// read snapshot header
		fread(&sh, sizeof(sh), 1, pSNAfileObject);
		
		// valid SNApshot image?
		if (memcmp(sh.id, "MV - SNA", 8) != 0) 
		{
			fclose(pSNAfileObject);
			return ERR_SNA_INVALID;
		}

		// memory dump size
		dwSnapSize = sh.ram_size[0] + (sh.ram_size[1] * 256);
		// limit to multiples of 64
		dwSnapSize &= ~0x3f;
		if (dwSnapSize == 0)
			dwSnapSize = 64; // Compatibility with CPCE
		
		// memory dump size differs from current RAM size?
		if (dwSnapSize > CPC->ram_size) 
		{
			if (Memory->UpdateRAMSize(dwSnapSize*1024)) 
			{
				CPC->ram_size = dwSnapSize;
			} 
			else 
			{
				fclose(pSNAfileObject);
				return ERR_OUT_OF_MEMORY;
			}
		}

		//emulator.emulator_reset(false);
		emulator.Pause();
		
		// read memory dump into CPC RAM
		n = fread(Memory->GetRAM(), dwSnapSize*1024, 1, pSNAfileObject);
		fclose(pSNAfileObject);
		if (!n) 
		{
			emulator.emulator_reset(false);
			return ERR_SNA_INVALID;
		}
		
		// Z80
		__rA = sh.AF[1];
		__rF = sh.AF[0];
		__rB = sh.BC[1];
		__rC = sh.BC[0];
		__rD = sh.DE[1];
		__rE = sh.DE[0];
		__rH = sh.HL[1];
		__rL = sh.HL[0];
		__rR = sh.R & 0x7f;
		__rRb7 = sh.R & 0x80; // bit 7 of R
		__rI = sh.I;
		if (sh.IFF0)
			__rIFF1 = Pflag;
		if (sh.IFF1)
			__rIFF2 = Pflag;
		__rIXh = sh.IX[1];
		__rIXl = sh.IX[0];
		__rIYh = sh.IY[1];
		__rIYl = sh.IY[0];
		z80.SP.b.h = sh.SP[1];
		z80.SP.b.l = sh.SP[0];
		z80.PC.b.h = sh.PC[1];
		z80.PC.b.l = sh.PC[0];
		__rIM = sh.IM; // interrupt mode
		z80.AFx.b.h = sh.AFx[1];
		z80.AFx.b.l = sh.AFx[0];
		z80.BCx.b.h = sh.BCx[1];
		z80.BCx.b.l = sh.BCx[0];
		z80.DEx.b.h = sh.DEx[1];
		z80.DEx.b.l = sh.DEx[0];
		z80.HLx.b.h = sh.HLx[1];
		z80.HLx.b.l = sh.HLx[0];
		// Gate Array
		port.b.h = 0x7f;
		
		// loop for all colours + border
		for (n = 0; n < 17; n++) 
		{
			GateArray->SelectPen( n );
			
			// GA palette entry
			val = sh.ga_ink_values[n];
			z80.z80_OUT_handler(port, val | (unsigned char)(1<<6));
		}

		// GA pen
		val = sh.ga_pen;
		z80.z80_OUT_handler(port, (val & 0x3f));

		// GA ROM configuration
		val = sh.ga_ROM_config;
		z80.z80_OUT_handler(port, (val & 0x3f) | (unsigned char)(2<<6));

		// GA RAM configuration
		val = sh.ga_RAM_config;
		z80.z80_OUT_handler(port, (val & 0x3f) | (unsigned char)(3<<6));


		// CRTC
		port.b.h = 0xbd;
		// loop for all CRTC registers
		for (n = 0; n < 18; n++) 
		{
			val = sh.crtc_registers[n];
			CRTC->RegisterSelect(n);
			z80.z80_OUT_handler(port, val);
		}
		port.b.h = 0xbc;

		// CRTC register select
		val = sh.crtc_reg_select;
		z80.z80_OUT_handler(port, val);
		
		// ROM select
		port.b.h = 0xdf;
		// upper ROM number
		val = sh.upper_ROM;
		z80.z80_OUT_handler(port, val);
		
		// PPI
		// port A
		port.b.h = 0xf4;
		z80.z80_OUT_handler(port, sh.ppi_A);
		// port B
		port.b.h = 0xf5;
		z80.z80_OUT_handler(port, sh.ppi_B);
		// port C
		port.b.h = 0xf6;
		z80.z80_OUT_handler(port, sh.ppi_C);
		// control
		port.b.h = 0xf7;
		z80.z80_OUT_handler(port, sh.ppi_control);
		
		// PSG
		PSG->SetControl(PPI->portC);
		PSG->SetRegSelect(sh.psg_reg_select);
		
		// loop for all PSG registers
		for (n = 0; n < 16; n++) 
		{
			PSG->SetAYRegister(n, sh.psg_registers[n]);
		}
		
		/*
		char msg[19];
		sprintf(msg,"Snapshot version %d", sh.version);
		emulator.logMessage(msg);
		*/
		// does the snapshot have version 2 data?
		if (sh.version > 1) 
		{
			// determine the model it was saved for
			dwModel = sh.cpc_model;
			
			// different from what we're currently running?
			if (dwModel != CPC->model) 
			{
				// not one of the known models?
				if (dwModel > 2) 
				{
					emulator.emulator_reset(false);
					return ERR_SNA_CPC_TYPE;
				}
				strncpy(chPath, CPC->rom_path, sizeof(chPath)-2);
				strcat(chPath, "/");

				// path to the required ROM image
				strncat(chPath, Memory->GetROMFile(dwModel), sizeof(chPath)-1 - strlen(chPath));
				if ((pSNAfileObject = fopen(chPath, "rb")) != NULL) 
				{
					n = fread(Memory->GetROMLow(), 2*16384, 1, pSNAfileObject);
					fclose(pSNAfileObject);
					if (!n) 
					{
						emulator.emulator_reset(false);
						return ERR_CPC_ROM_MISSING;
					}
					CPC->model = dwModel;
				} 
				else 
				{
					// ROM image load failed
					emulator.emulator_reset(false);
					return ERR_CPC_ROM_MISSING;
				}
			}
		}
		
		// does the snapshot have version 3 data?
		if (sh.version > 2) 
		{
			FDC->SetMotor(sh.fdc_motor);
			FDC->GetDriveA().current_track = sh.drvA_current_track;
			FDC->GetDriveB().current_track = sh.drvB_current_track;

			// invert bit 7 again
			CPC->printer_port = sh.printer_data ^ 0x80;
			// multiply by 2 to bring it into the 0 - 30 range

			PSG->SetAmplitudeEnv(sh.psg_env_step << 1);
			PSG->SetEnvDirection(sh.psg_env_direction);

			CRTC->SetAddr( sh.crtc_addr[0] + (sh.crtc_addr[1] * 256) );
			VDU->SetScanline( sh.crtc_scanline[0] + (sh.crtc_scanline[1] * 256) );
			CRTC->SetCharCount( sh.crtc_char_count[0] );
			CRTC->SetLineCount( sh.crtc_line_count & 127 );
			CRTC->SetRasterCount( sh.crtc_raster_count & 31 );
			CRTC->SetHswCount( sh.crtc_hsw_count & 15 );
			CRTC->SetVswCount( sh.crtc_vsw_count & 15 );
			dwFlags = sh.crtc_flags[0] + (sh.crtc_flags[1] * 256);
			// vsync active?
			CRTC->SetFlagInVSync( dwFlags & 1 ? 1 : 0 );
			
			// hsync active?
			if (dwFlags & 2)
			{
				CRTC->SetFlag1InHSync(0xff);
				CRTC->SetFlagHadHSync( 1 );
				if ((CRTC->GetHswCount() >= 3) && (CRTC->GetHswCount() < 7)) 
				{
					CRTC->SetFlagInMonHSync( 1 );
				}
			}

			// in vertical total adjust?
			CRTC->SetFlagInVta( dwFlags & 0x80 ? 1 : 0 );
			GateArray->SetHSCount( sh.ga_int_delay & 3 );
			GateArray->SetSLCount( sh.ga_sl_count );
			z80.int_pending = sh.z80_int_pending;
		}

		emulator.Run();
   } 
   else 
   {
	   return ERR_FILE_NOT_FOUND;
   }

   return 0;
}



int snapshot_save (Emulator &emulator, const char *pchFileName)
{
	auto GateArray = emulator.GetGateArray();
	auto CRTC = emulator.GetCRTC();
	auto VDU = emulator.GetVDU();
	t_z80regs& z80 = emulator.GetZ80();
	auto Memory = emulator.GetMemory();
	auto CPC = emulator.GetConfig();
	auto FDC = emulator.GetFDC();
	auto PSG = emulator.GetPSG();
	auto PPI = emulator.GetPPI();

	t_SNA_header sh;
	int n;
	dword dwFlags;
	
	memset(&sh, 0, sizeof(sh));
	strcpy(sh.id, "MV - SNA");
	sh.version = 3;
	// Z80
	sh.AF[1] = __rA;
	sh.AF[0] = __rF;
	sh.BC[1] = __rB;
	sh.BC[0] = __rC;
	sh.DE[1] = __rD;
	sh.DE[0] = __rE;
	sh.HL[1] = __rH;
	sh.HL[0] = __rL;
	sh.R = (__rR & 0x7f) | (__rRb7 & 0x80);
	sh.I = __rI;
	if (__rIFF1)
		sh.IFF0 = 1;
	if (__rIFF2)
		sh.IFF1 = 1;
	sh.IX[1] = __rIXh;
	sh.IX[0] = __rIXl;
	sh.IY[1] = __rIYh;
	sh.IY[0] = __rIYl;
	sh.SP[1] = z80.SP.b.h;
	sh.SP[0] = z80.SP.b.l;
	sh.PC[1] = z80.PC.b.h;
	sh.PC[0] = z80.PC.b.l;
	sh.IM = __rIM;
	sh.AFx[1] = z80.AFx.b.h;
	sh.AFx[0] = z80.AFx.b.l;
	sh.BCx[1] = z80.BCx.b.h;
	sh.BCx[0] = z80.BCx.b.l;
	sh.DEx[1] = z80.DEx.b.h;
	sh.DEx[0] = z80.DEx.b.l;
	sh.HLx[1] = z80.HLx.b.h;
	sh.HLx[0] = z80.HLx.b.l;
	// Gate Array
	sh.ga_pen = GateArray->GetPen();
	// loop for all colours + border
	for (n = 0; n < 17; n++) 
	{
		sh.ga_ink_values[n] = GateArray->GetInk( n );
	}
	sh.ga_ROM_config = Memory->GetROMConfig();
	sh.ga_RAM_config = Memory->GetRAMConfig();
	// CRTC
	sh.crtc_reg_select = CRTC->GetRegisterSelect();
	// loop for all CRTC registers
	for (n = 0; n < 18; n++) 
	{
		sh.crtc_registers[n] = CRTC->GetRegisterValue(n);
	}
	// ROM select
	sh.upper_ROM = Memory->GetUpperROM();
	// PPI
	sh.ppi_A = PPI->portA;
	sh.ppi_B = PPI->portB;
	sh.ppi_C = PPI->portC;
	sh.ppi_control = PPI->control;
	// PSG
	sh.psg_reg_select = PSG->GetRegSelect();
	// loop for all PSG registers
	for (n = 0; n < 16; n++) 
	{
		sh.psg_registers[n] = PSG->GetAYRegister(n);
	}
	
	sh.ram_size[0] = CPC->ram_size & 0xff;
	sh.ram_size[1] = (CPC->ram_size >> 8) & 0xff;
	// version 2 info
	sh.cpc_model = CPC->model;
	// version 3 info
	sh.fdc_motor = FDC->GetMotor();
	sh.drvA_current_track = FDC->GetDriveA().current_track;
	sh.drvB_current_track = FDC->GetDriveB().current_track;
	// invert bit 7 again
	sh.printer_data = CPC->printer_port ^ 0x80;
	// divide by 2 to bring it into the 0 - 15 range
	sh.psg_env_step = PSG->GetAmplitudeEnv() >> 1;
	sh.psg_env_direction = PSG->GetEnvDirection();

	sh.crtc_addr[0] = CRTC->GetAddr() & 0xff;
	sh.crtc_addr[1] = (CRTC->GetAddr() >> 8) & 0xff;
	sh.crtc_scanline[0] = VDU->GetScanline() & 0xff;
	sh.crtc_scanline[1] = (VDU->GetScanline() >> 8) & 0xff;
	sh.crtc_char_count[0] = CRTC->GetCharCount();
	sh.crtc_line_count = CRTC->GetLineCount();
	sh.crtc_raster_count = CRTC->GetRasterCount();
	sh.crtc_hsw_count = CRTC->GetHswCount();
	sh.crtc_vsw_count = CRTC->GetVswCount();
	dwFlags = 0;
	// vsync active?
	if (CRTC->GetFlagInVSync()) 
	{
		dwFlags |= 1;
	}
	// hsync active?
	if (CRTC->GetFlag1InHSync()) 
	{
		dwFlags |= 2;
	}
	// in vertical total adjust?
	if (CRTC->GetFlagInVta()) 
	{
		dwFlags |= 0x80;
	}
	sh.crtc_flags[0] = dwFlags & 0xff;
	sh.crtc_flags[1] = (dwFlags >> 8) & 0xff;
	sh.ga_int_delay = GateArray->GetHSCount();
	sh.ga_sl_count = GateArray->GetSLCount();
	sh.z80_int_pending = z80.int_pending;
	
	if ((pSNAfileObject = fopen(pchFileName, "wb")) != NULL) 
	{
		// write snapshot header
		if (fwrite(&sh, sizeof(sh), 1, pSNAfileObject) != 1) 
		{
			fclose(pSNAfileObject);
			return ERR_SNA_WRITE;
		}
		// write memory contents to snapshot file
		if (fwrite(Memory->GetRAM(), CPC->ram_size*1024, 1, pSNAfileObject) != 1) 
		{
			fclose(pSNAfileObject);
			return ERR_SNA_WRITE;
		}
		fclose(pSNAfileObject);
                if((pMemoryfileObject = fopen("memory.txt", "wt")) != NULL)
                {
                    for(int i = 0; i < 0xFFFF; i+=0x10)
                    {
                        fprintf(pMemoryfileObject,"%04x    ",i);
                        for(int j = 0; j < 0x10; j++)
                        {
                            fprintf(pMemoryfileObject,"%02x ",*(Memory->GetRAM() + i + j));
                        }
                        fprintf(pMemoryfileObject,"   ");
                        for(int j = 0; j < 0x10; j++)
                        {
                            if(*(Memory->GetRAM() + i + j) >= 0x20)
                            {
                                fprintf(pMemoryfileObject,"%c",*(Memory->GetRAM() + i + j));
                            }
                            else
                            {
                                fprintf(pMemoryfileObject,".");
                            }
                        }
                        fprintf(pMemoryfileObject,"\n");
                    }
                }
                fclose(pMemoryfileObject);
	} 
	else 
	{
		return ERR_SNA_WRITE;
	}

	return 0;
}
