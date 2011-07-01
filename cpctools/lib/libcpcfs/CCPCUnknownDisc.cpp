/**
* @class CCPCUnknownDisc
* Classe permettant la gestion d'un disc CPC Unknown (utilisation de la sous-lib DSK)
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#include "CCPCUnknownDisc.h"

#include "CError.h"

#include <fstream>

using namespace std;

// Track format
void CCPCUnknownDisc::CCPCTrackFormat::InitSectors(unsigned char secBase, int secSize, int secTrack, int secHead, bool interlaced)
{
	int nbSector = SectorArray.size();
	int idxdec = ((nbSector & 1) == 0) ? (nbSector >> 1) : (nbSector >> 1) + 1;
	for (int s=0 ; s<nbSector ; s++)
	{
		SectorArray[s].SectorSize = secSize;

		int idx = interlaced ? (s >> 1) + (((s & 1) == 0) ? 0 : idxdec) : s;
		SectorArray[s].SectorID = secBase + idx;

		SectorArray[s].Track = secTrack;
		SectorArray[s].Head = secHead;
	}
}

void CCPCUnknownDisc::CCPCTrackFormat::ComputeGap3()
{
	if (SectorArray.size() == 0)
		return;

	unsigned int nbSector = SectorArray.size();
	bool allSameSize = true;
	int secsize = SectorArray[0].SectorSize;
	for (unsigned int s=0 ; s<nbSector && allSameSize ; s++)
	{
		allSameSize = allSameSize && (SectorArray[s].SectorSize == secsize);
	}

	if (allSameSize)
	{
		unsigned char psh;
		for (psh = 0; secsize > 128; secsize /= 2)
			psh++;

		Gap3 = CCPCDisc::MaxGAPSize;
		RWGap = CCPCDisc::MaxGAPSize;

		for (unsigned int i=0 ; CCPCDisc::SectorGAPSizeArray[i] != (unsigned int)-1 ; i+=3)
		{
			if (CCPCDisc::SectorGAPSizeArray[i+0] == psh &&
				CCPCDisc::SectorGAPSizeArray[i+1] == nbSector)
			{
				Gap3 = CCPCDisc::SectorGAPSizeArray[i+2];
				RWGap = CCPCDisc::SectorGAPSizeArray[i+2];
			}
		}
	}
	else
	{
		int sectorSizeCount = 0;
		for (unsigned int s=0 ; s<nbSector ; s++)
		{
			sectorSizeCount += (SectorArray[s].SectorSize + CCPCDisc::SectorHeaderSize);
		}

		TOOLS_ASSERTMSG(sectorSizeCount < (int)CCPCDisc::MaxTrackSize, "Unable to compute GAP size " << sectorSizeCount << " bytes on track > " << CCPCDisc::MaxTrackSize);
		int gapSize = CCPCDisc::MaxTrackSize - sectorSizeCount;
		gapSize /= nbSector;
		if (gapSize > (int)CCPCDisc::MaxGAPSize)
		{
			gapSize = (int)CCPCDisc::MaxGAPSize;
		}

		Gap3 = gapSize;
		RWGap = gapSize;
	}
}

// Unknown disc
DSK_GEOMETRY& CCPCUnknownDisc::DiscGeometry()
{
	return _geometry;
}

void CCPCUnknownDisc::FormatTrack(int track, int side, const CCPCTrackFormat &format)
{
	DSK_FORMAT *sectors = new DSK_FORMAT[format.SectorArray.size()];
	for (unsigned int s=0 ; s<format.SectorArray.size() ; s++)
	{
		sectors[s].fmt_cylinder = (format.SectorArray[s].Track != -1) ? format.SectorArray[s].Track : track;
		sectors[s].fmt_head = (format.SectorArray[s].Head != -1) ? format.SectorArray[s].Head : side;
		sectors[s].fmt_sector = format.SectorArray[s].SectorID;
		sectors[s].fmt_secsize = format.SectorArray[s].SectorSize;
	}

	DSK_GEOMETRY tempGeom = _geometry;
	tempGeom.dg_sectors = format.SectorArray.size();
	tempGeom.dg_fmtgap = format.Gap3;
	tempGeom.dg_rwgap = format.RWGap;

	dsk_err_t e = dsk_pformat(_driver, &tempGeom, track, side, sectors, CCPCDisc::FormatByte);
	TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error formatting dsk :" << string(dsk_strerror(e)));

	delete[] sectors;
}

void CCPCUnknownDisc::GetSectorFormat(int track, int side, unsigned char sectorID, DSK_FORMAT &format)
{
	DSK_FORMAT *fmt_track = NULL;
	dsk_err_t e;

	DSK_GEOMETRY geom = _geometry;

	e = dsk_ptrackids(_driver, &geom, track, side, &geom.dg_sectors, &fmt_track);
	TOOLS_ASSERTMSG( (e == DSK_ERR_OK) , "Error reading track IDs :" << string(dsk_strerror(e)));

	for (unsigned int s=0 ; s<geom.dg_sectors ; s++)
	{
		if (fmt_track[s].fmt_sector == sectorID)
		{
			format = fmt_track[s];
			return;
		}
	}
	TOOLS_ERRORMSG("Unable to find sector #" << hex << (int)sectorID << dec << " on track " << track << "[" << side << "]");
}

void CCPCUnknownDisc::NextSector(int &track, int &side, unsigned char &sectorID, bool useDoubleSide)
{
	DSK_FORMAT *fmt_track = NULL;
	dsk_err_t e;

	DSK_GEOMETRY geom = _geometry;

	e = dsk_ptrackids(_driver, &geom, track, side, &geom.dg_sectors, &fmt_track);
	TOOLS_ASSERTMSG( (e == DSK_ERR_OK) , "Error reading track IDs :" << string(dsk_strerror(e)));

	unsigned char nextSector = sectorID+1;

	unsigned int s;
	for (s=0 ; s<geom.dg_sectors ; s++)
	{
		if (fmt_track[s].fmt_sector == nextSector)
		{
			sectorID = nextSector;
			return;
		}
	}

	// Next sector not found, go to next track
	if (_geometry.dg_heads == 2 && useDoubleSide)
	{
		if (side == 0)
			side = 1;
		else
		{
			side = 0;
			track++;
		}
	}
	else
		track++;

	TOOLS_ASSERTMSG(track < (int)_geometry.dg_cylinders, "Number of track execeed");

	e = dsk_ptrackids(_driver, &geom, track, side, &geom.dg_sectors, &fmt_track);
	TOOLS_ASSERTMSG( (e == DSK_ERR_OK) , "Error reading track IDs :" << string(dsk_strerror(e)));

	nextSector = 255;
	for (s=0 ; s<geom.dg_sectors ; s++)
	{
		if (fmt_track[s].fmt_sector < nextSector)
		{
			nextSector = fmt_track[s].fmt_sector;
		}
	}

	TOOLS_ASSERTMSG( nextSector != 255, "Unable to find next sector");

	sectorID = nextSector;
}

void CCPCUnknownDisc::PutRawFile(const string &filename, int &track, int &side, unsigned char &sectorID, FileSectorList &sectorList, bool useDoubleSide)
{
	sectorList.clear();

	ifstream rawFile(filename.c_str(), ios::binary);

	TOOLS_ASSERTMSG(rawFile.good(), "Unable to open " << filename);

	rawFile.seekg(0, ios::end);
	int size = rawFile.tellg();
	rawFile.seekg(0, ios::beg);

	vector<char> rawFileData;
	rawFileData.resize(size + CCPCDisc::MaxTrackSize, 0);

	rawFile.read(&rawFileData[0], size);

	int nbBytes = size;
	int idx = 0;
	while (nbBytes > 0)
	{
		DSK_FORMAT format;
		GetSectorFormat(track, side, sectorID, format);

		WriteSector(track, side, sectorID, (void*)&rawFileData[idx], true);

		idx += format.fmt_secsize;
		nbBytes -= format.fmt_secsize;

		sectorList.push_back(CFileSector(format.fmt_cylinder, format.fmt_head, format.fmt_sector, format.fmt_secsize));

		NextSector(track, side, sectorID, useDoubleSide);
	}
}

void CCPCUnknownDisc::getEmptyBlock(vector<unsigned int> &o_block) const
{
	TOOLS_ERRORMSG("Unable to get empty block on unknown disc");
}

void CCPCUnknownDisc::ReadBlock(const unsigned char i_idBlock, void* o_buffer) const
{
	TOOLS_ERRORMSG("Unable to read block on unknown disc");
}

void CCPCUnknownDisc::WriteBlock(const unsigned char i_idBlock, const void* i_buffer)
{
	TOOLS_ERRORMSG("Unable to write block on unknown disc");
}

