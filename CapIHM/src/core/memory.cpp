/*
 *   $Id: memory.cpp 60 2008-11-25 17:08:52Z giot.romain $
 *	 CapriceReloded an Amstrad CPC emulator
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
// Caprice32 GateArray memory emulator
//

#include "memory.h"
#include "cap32.h"
#include "cap32type.h"
#include "config.h"

#include "error.h"

extern dword dwMF2Flags;
extern byte *pbMF2ROM;

char chROMFile[3][14] =
{
	"cpc464.rom",
	"cpc664.rom",
	"cpc6128.rom"
};

t_Memory::t_Memory(t_CPC &cpc) :
CPC(cpc),
ROM_config(0),
RAM_bank(0),
RAM_config(0),
upper_ROM(0),
pbRAM(NULL),
pbROMlo(NULL),
pbROMhi(NULL),
pbExpansionROM(NULL)
{
	int i,j;
	for (i = 0 ; i<8 ; i++)
	{
		for (j = 0 ; j<4 ; j++)
		{
			membank_config[i][j] = NULL;
		}
	}

	for (i = 0 ; i<4 ; i++)
	{
		membank_read[i] = NULL;
		membank_write[i] = NULL;
	}

	for (i = 0 ; i<256 ; i++)
	{
		memmap_ROM[i] = NULL;
	}
}

int t_Memory::Init()
{
	int iErr;

	if ((iErr = RAMInit()) != ERR_OK)
	{
	    printf("Error initializing RAM !\n");
		return iErr;
	}

	if ((iErr = ROMInit()) != ERR_OK)
	{
	    printf("Error initializing ROM !\n");
		return iErr;
	}

	return ERR_OK;
}

void t_Memory::InitMemConfig()
{
	int n;
	for (n = 0; n < 4; n++)
	{
		membank_read[n] = membank_config[0][n];
		membank_write[n] = membank_config[0][n];
	}
	// 'page in' lower ROM
	membank_read[0] = pbROMlo;
	// 'page in' upper ROM
	membank_read[3] = pbROMhi;
}

void t_Memory::Clean()
{
	int iRomNum;

	// loop for ROMs 2-15
	for (iRomNum = 2; iRomNum < 16; iRomNum++)
	{
		// was a ROM assigned to this slot?
		if (memmap_ROM[iRomNum] != NULL)
		{
			// if so, release the associated memory
			delete [] memmap_ROM[iRomNum];
		}
	}

	delete [] pbRAM;
	pbRAM = NULL;
	delete [] pbROMlo;
	pbROMlo = NULL;
}

void t_Memory::Reset(bool MF2Reset)
{
	// memory
	if (MF2Reset)
	{
		// clear only the first 64K of CPC memory
		memset(pbRAM, 0, 64*1024);
	}
	else
	{
		// clear all memory used for CPC RAM
		memset(pbRAM, 0, CPC.ram_size*1024);
		if (pbMF2ROM)
		{
			// clear the MF2's RAM area
			memset(pbMF2ROM+8192, 0, 8192);
		}
	}
}

bool t_Memory::UpdateRAMSize(unsigned int size)
{
	byte *pbTemp;

	pbTemp = new byte [size];
	delete [] pbRAM;
	pbRAM = pbTemp;

	return true;
}

char* t_Memory::GetROMFile(unsigned int idx) const
{
	return (chROMFile[idx]);
}

void t_Memory::SetUpperROM(unsigned char val)
{
	upper_ROM = val;

	SetROMExpansion(memmap_ROM[val]);
	// selected expansion ROM not present?
	if (GetROMExpansion() == NULL)
	{
		// revert to BASIC ROM
		SetROMExpansion(GetROMHigh());
	}
	// upper/expansion ROM is enabled?
	if (!(GetROMConfig() & 0x08))
	{
		// 'page in' upper/expansion ROM
		membank_read[3] = GetROMExpansion();
	}
}

void t_Memory::ga_init_banking (void)
{
	byte *romb0, *romb1, *romb2, *romb3, *romb4, *romb5, *romb6, *romb7;
	byte *pbRAMbank;

	romb0 = pbRAM;
	romb1 = pbRAM + 1*16384;
	romb2 = pbRAM + 2*16384;
	romb3 = pbRAM + 3*16384;

	pbRAMbank = pbRAM + ((RAM_bank + 1) * 65536);
	romb4 = pbRAMbank;
	romb5 = pbRAMbank + 1*16384;
	romb6 = pbRAMbank + 2*16384;
	romb7 = pbRAMbank + 3*16384;

	membank_config[0][0] = romb0;
	membank_config[0][1] = romb1;
	membank_config[0][2] = romb2;
	membank_config[0][3] = romb3;

	membank_config[1][0] = romb0;
	membank_config[1][1] = romb1;
	membank_config[1][2] = romb2;
	membank_config[1][3] = romb7;

	membank_config[2][0] = romb4;
	membank_config[2][1] = romb5;
	membank_config[2][2] = romb6;
	membank_config[2][3] = romb7;

	membank_config[3][0] = romb0;
	membank_config[3][1] = romb3;
	membank_config[3][2] = romb2;
	membank_config[3][3] = romb7;

	membank_config[4][0] = romb0;
	membank_config[4][1] = romb4;
	membank_config[4][2] = romb2;
	membank_config[4][3] = romb3;

	membank_config[5][0] = romb0;
	membank_config[5][1] = romb5;
	membank_config[5][2] = romb2;
	membank_config[5][3] = romb3;

	membank_config[6][0] = romb0;
	membank_config[6][1] = romb6;
	membank_config[6][2] = romb2;
	membank_config[6][3] = romb3;

	membank_config[7][0] = romb0;
	membank_config[7][1] = romb7;
	membank_config[7][2] = romb2;
	membank_config[7][3] = romb3;
}



void t_Memory::ga_memory_manager (void)
{
	dword mem_bank;
	// 64KB of RAM?
	if (CPC.ram_size == 64)
	{
		// no expansion memory
		mem_bank = 0;
		// the only valid configuration is 0
		RAM_config = 0;
	}
	else
	{
		// extract expansion memory bank
		mem_bank = (RAM_config >> 3) & 7;
		// selection is beyond available memory?
		if (((mem_bank+2)*64) > CPC.ram_size)
		{
			// force default mapping
			mem_bank = 0;
		}
	}

	// requested bank is different from the active one?
	if (mem_bank != RAM_bank)
	{
		RAM_bank = mem_bank;
		ga_init_banking();
	}

	// remap active memory banks
	for (int n = 0; n < 4; n++)
	{
		membank_read[n] = membank_config[RAM_config & 7][n];
		membank_write[n] = membank_config[RAM_config & 7][n];
	}

	// lower ROM is enabled?
	if (!(ROM_config & 0x04))
	{
		// is the Multiface 2 paged in?
		if (dwMF2Flags & MF2_ACTIVE)
		{
			membank_read[0] = pbMF2ROM;
			membank_write[0] = pbMF2ROM;
		}
		else
		{
			// 'page in' lower ROM
			membank_read[0] = pbROMlo;
		}
	}

	// upper/expansion ROM is enabled?
	if (!(ROM_config & 0x08))
	{
		// 'page in' upper/expansion ROM
		membank_read[3] = pbExpansionROM;
	}
}

int t_Memory::RAMInit()
{
	// allocate memory for desired amount of RAM
	pbRAM = new byte [CPC.ram_size*1024];
	// allocate memory for 32K of ROM
	pbROMlo = new byte [32*1024];

	if ((!pbRAM) || (!pbROMlo))
	{
		return ERR_OUT_OF_MEMORY;
	}
	pbROMhi = pbExpansionROM = pbROMlo + 16384;

	// clear the expansion ROM map
	memset(memmap_ROM, 0, sizeof(memmap_ROM[0]) * 256);

	return ERR_OK;
}

int t_Memory::ROMInit()
{
	FILE* pfileObject;

	int iErr, iRomNum;
	char chPath[_MAX_PATH + 1];
	char *pchRomData;

	// init the CPC memory banking map
	ga_init_banking();

	if ((iErr = emulator_patch_ROM()))
	{
	    printf("Error patching roms !\n");
		return iErr;
	}

	// loop for ROMs 0-15
	for (iRomNum = 0; iRomNum < 16; iRomNum++)
	{
		// is a ROM image specified for this slot?
		if (CPC.rom_file[iRomNum][0])
		{
			// allocate 16K
			pchRomData = new char [16384];
			// clear memory
			memset(pchRomData, 0, 16384);
			strncpy(chPath, CPC.rom_path, sizeof(chPath)-2);
			strcat(chPath, "/");
			strncat(chPath, CPC.rom_file[iRomNum], sizeof(chPath)-1 - strlen(chPath));

			// attempt to open the ROM image
			if ((pfileObject = fopen(chPath, "rb")) != NULL)
			{
				// read 128 bytes of ROM data
				fread(pchRomData, 128, 1, pfileObject);
				word checksum = 0;
				for (int n = 0; n < 0x43; n++)
				{
					checksum += pchRomData[n];
				}

				// if the checksum matches, we got us an AMSDOS header
				if (checksum == ((pchRomData[0x43] << 8) + pchRomData[0x44]))
				{
					// skip it
					fread(pchRomData, 128, 1, pfileObject);
				}

				// is it a valid CPC ROM image (0 = forground, 1 = background, 2 = extension)?
				if (!(pchRomData[0] & 0xfc))
				{
					// read the rest of the ROM file
					fread(pchRomData+128, 16384-128, 1, pfileObject);
					// update the ROM map
					memmap_ROM[iRomNum] = (byte *)pchRomData;
				}
				else
				{
					// not a valid ROM file
					fprintf(stderr, "ERROR: %s is not a CPC ROM file - clearing ROM slot %d.\n", CPC.rom_file[iRomNum], iRomNum);
					// free memory on error
					delete [] pchRomData;
					CPC.rom_file[iRomNum][0] = 0;
				}
				fclose(pfileObject);
			}
			else
			{
				// file not found
				fprintf(stderr, "ERROR: The %s file is missing - clearing ROM slot %d.\n", CPC.rom_file[iRomNum], iRomNum);
				// free memory on error
				delete [] pchRomData;
				CPC.rom_file[iRomNum][0] = 0;
			}
		}
	}

	return ERR_OK;
}

int t_Memory::emulator_patch_ROM (void)
{
	FILE* pfileObject;

	char chPath[_MAX_PATH + 1];
	byte *pbPtr;

	strncpy(chPath, CPC.rom_path, sizeof(chPath)-2);
	strcat(chPath, "/");

	// determine the ROM image name for the selected model
	strncat(chPath, chROMFile[CPC.model], sizeof(chPath)-1 - strlen(chPath));

	// load CPC OS + Basic
	if ((pfileObject = fopen(chPath, "rb")) != NULL)
	{
		fread(pbROMlo, 2*16384, 1, pfileObject);
		fclose(pfileObject);
	}
	else
	{
	    printf("UNABLE TO FIND SYSTEM ROM ! CHECK CONFIGURATION ! FATAL ERROR ! Should be at %s\n",chPath);
		return ERR_CPC_ROM_MISSING;
	}

    /*
	if (CPC.keyboard) {
		pbPtr = pbROMlo;
		switch(CPC.model)
		{
			// 464
		case 0:
			{
				// location of the keyboard translation table
				pbPtr += 0x1d69;
				break;
			}
			// 664
		case 1:
			// 6128
		case 2:
			{
				// location of the keyboard translation table
				pbPtr += 0x1eef;
				break;
			}
		}
		if (pbPtr != pbROMlo)
		{
			// patch the CPC OS ROM with the chosen keyboard layout
			memcpy(pbPtr, cpc_keytrans[CPC.keyboard-1], 240);
			pbPtr = pbROMlo + 0x3800;
			// add the corresponding character set
			memcpy(pbPtr, cpc_charset[CPC.keyboard-1], 2048);
		}
	}
    */
	return 0;
}
