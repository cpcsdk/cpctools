/**
* @class CCPCSystemDisc
* Classe permettant la gestion d'un disc CPC System (utilisation de la sous-lib DSK)
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#include "CCPCSystemDisc.h"

#include "CError.h"
/// Indique que le bloc correspond au boot CPM
unsigned int CCPCSystemDisc::boot =(0-1);

/// Renvoie la liste des block libres
void CCPCSystemDisc::getEmptyBlock(std::vector<unsigned int> &o_block) const
{
	for (unsigned int c=0;c<_geometry.dg_cylinders;c++)
		for (unsigned int s=0;s<_geometry.dg_sectors;s++)
		{
			unsigned int b = (c*_geometry.dg_sectors + s) >> 1;
			b = (b < 9) ? CCPCSystemDisc::boot : (b-9);
			int entry = getDirectoryEntryForBlock(b);
			if (((c*_geometry.dg_sectors + s) % 2) != 1)
				if (b!=0 && b!=1 && b!=CCPCSystemDisc::boot && entry == -1)
					o_block.push_back(b);
		}
}

void CCPCSystemDisc::ReadBlock(const unsigned char i_idBlock, void* o_buffer) const
{
	unsigned int bloc = i_idBlock+9;
	
	unsigned char track1 = (bloc*2 - (bloc*2 % _geometry.dg_sectors)) / _geometry.dg_sectors;
	unsigned char sector1 = _geometry.dg_secbase+(bloc*2 % _geometry.dg_sectors);
	unsigned char track2 = ((bloc*2+1) - ((bloc*2+1) % _geometry.dg_sectors)) / _geometry.dg_sectors;
	unsigned char sector2 = _geometry.dg_secbase+((bloc*2+1) % _geometry.dg_sectors);
	
	ReadSector(track1,0,sector1,o_buffer);
	ReadSector(track2,0,sector2,(char*)o_buffer+_geometry.dg_secsize);
	
}

void CCPCSystemDisc::WriteBlock(const unsigned char i_idBlock, const void* i_buffer)
{
	unsigned int bloc = i_idBlock+9;
	
	unsigned char track1 = (bloc*2 - (bloc*2 % _geometry.dg_sectors)) / _geometry.dg_sectors;
	unsigned char sector1 = _geometry.dg_secbase+(bloc*2 % _geometry.dg_sectors);
	unsigned char track2 = ((bloc*2+1) - ((bloc*2+1) % _geometry.dg_sectors)) / _geometry.dg_sectors;
	unsigned char sector2 = _geometry.dg_secbase+((bloc*2+1) % _geometry.dg_sectors);
	
	WriteSector(track1,0,sector1,i_buffer);
	WriteSector(track2,0,sector2,(char*)i_buffer+_geometry.dg_secsize);
	
}

