/**
* @class CCPCRomdosD1Disc
* Classe permettant la gestion d'un disc CPC Romdos D1 (utilisation de la sous-lib DSK)
* @author PulkoMandy
* @version 1.0
* @date 19/09/08
*/

#include "CCPCRomdosD1Disc.h"

#include "CError.h"

void CCPCRomdosD1Disc::getEmptyBlock(std::vector<unsigned int> &o_block) const
{
    for(unsigned int f=0;f<_geometry.dg_heads;f++)
	for (unsigned int c=0;c<_geometry.dg_cylinders;c++)
		for (unsigned int s=0;s<_geometry.dg_sectors;s++)
		{
			unsigned int b = (f*_geometry.dg_cylinders*_geometry.dg_sectors + c*_geometry.dg_sectors + s) >> 2;
			int entry = getDirectoryEntryForBlock(b);
			if (((f*_geometry.dg_cylinders*_geometry.dg_sectors + c*_geometry.dg_sectors + s) % 4) == 0)
				if (b < 4 && entry == -1)
					o_block.push_back(b);
		}
}

void CCPCRomdosD1Disc::ReadBlock(const unsigned char i_idBlock, void* o_buffer) const
{
	unsigned char track1 = (i_idBlock*4 - (i_idBlock*4 % _geometry.dg_sectors)) / _geometry.dg_sectors;
	unsigned char sector1 = _geometry.dg_secbase + (i_idBlock*4 % _geometry.dg_sectors);
	unsigned char track2 = ((i_idBlock*4+1) - ((i_idBlock*4+1) % _geometry.dg_sectors)) / _geometry.dg_sectors;
	unsigned char sector2 = _geometry.dg_secbase+((i_idBlock*4+1) % _geometry.dg_sectors);
	unsigned char track3 = ((i_idBlock*4+2) - ((i_idBlock*4+2) % _geometry.dg_sectors)) / _geometry.dg_sectors;
	unsigned char sector3 = _geometry.dg_secbase+((i_idBlock*4+2) % _geometry.dg_sectors);
	unsigned char track4 = ((i_idBlock*4+3) - ((i_idBlock*4+3) % _geometry.dg_sectors)) / _geometry.dg_sectors;
	unsigned char sector4 = _geometry.dg_secbase+((i_idBlock*4+3) % _geometry.dg_sectors);
	unsigned char side=0;
	if (track1 > 80){track1-=80;side=1;}
	if (track2 > 80){track2-=80;}
	if (track3 > 80){track3-=80;}
	if (track4 > 80){track4-=80;}
	
	ReadSector(track1,side,sector1,o_buffer);
	ReadSector(track2,side,sector2,(char*)o_buffer+_geometry.dg_secsize);
	ReadSector(track3,side,sector3,(char*)o_buffer+_geometry.dg_secsize*2);
	ReadSector(track4,side,sector4,(char*)o_buffer+_geometry.dg_secsize*3);
	
}

void CCPCRomdosD1Disc::WriteBlock(const unsigned char i_idBlock, const void* i_buffer)
{
	unsigned char track1 = (i_idBlock*2 - (i_idBlock*2 % _geometry.dg_sectors)) / _geometry.dg_sectors;
	unsigned char sector1 = _geometry.dg_secbase+(i_idBlock*2 % _geometry.dg_sectors);
	unsigned char track2 = ((i_idBlock*2+1) - ((i_idBlock*2+1) % _geometry.dg_sectors)) / _geometry.dg_sectors;
	unsigned char sector2 = _geometry.dg_secbase+((i_idBlock*2+1) % _geometry.dg_sectors);
	unsigned char track3 = ((i_idBlock*4+2) - ((i_idBlock*4+2) % _geometry.dg_sectors)) / _geometry.dg_sectors;
	unsigned char sector3 = _geometry.dg_secbase+((i_idBlock*4+2) % _geometry.dg_sectors);
	unsigned char track4 = ((i_idBlock*4+3) - ((i_idBlock*4+3) % _geometry.dg_sectors)) / _geometry.dg_sectors;
	unsigned char sector4 = _geometry.dg_secbase+((i_idBlock*4+3) % _geometry.dg_sectors);
	unsigned char side=0;
	if (track1 > 80){track1-=80;side=1;}
	if (track2 > 80){track2-=80;}
	if (track3 > 80){track3-=80;}
	if (track4 > 80){track4-=80;}
	
	WriteSector(track1,0,sector1,i_buffer);
	WriteSector(track2,0,sector2,(char*)i_buffer+_geometry.dg_secsize);
	WriteSector(track3,0,sector3,(char*)i_buffer+_geometry.dg_secsize*2);
	WriteSector(track4,0,sector4,(char*)i_buffer+_geometry.dg_secsize*3);
	
}

