/**
* @class CCPCUnknownDisc
* Classe permettant la gestion d'un disc CPC Unknown (utilisation de la sous-lib DSK)
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#pragma once
#define _CCPCUNKNOWNDISC_HH_

#pragma warning( disable : 4786 )

#include <iostream>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "libdsk.h"
#include <errno.h>
#include <string.h>

#include <vector>

#include "CCPCDisc.h"

using namespace std;

class CCPCUnknownDisc:public CCPCDisc
{
public:
	class CCPCSectorFormat
	{
	public:
		unsigned char				SectorID;
		int							SectorSize;
		int							Head;
		int							Track;
	public:
		CCPCSectorFormat() : SectorID(0), SectorSize(0), Head(-1), Track(-1)	{}
		CCPCSectorFormat(unsigned char sID, int sSize) : SectorID(sID), SectorSize(sSize) {}
	};
	class CCPCTrackFormat
	{
	public:
		unsigned char				Gap3;
		unsigned char				RWGap;
		vector<CCPCSectorFormat>	SectorArray;

	public:
		CCPCTrackFormat() :
		  Gap3(0), RWGap(0), SectorArray() {}

		CCPCTrackFormat(int nbSector, unsigned char secBase, int secSize, bool interlaced = true, int secTrack = -1, int secHead = -1) :
		  Gap3(0), RWGap(0), SectorArray()
		{
			SectorArray.resize(nbSector);
			InitSectors(secBase, secSize, secTrack, secHead, interlaced);
			ComputeGap3();
		}

		void InitSectors(unsigned char secBase, int secSize, int secTrack, int secHead, bool interlaced);
		void ComputeGap3();
	};

	class CFileSector
	{
	public:
		int Track;
		int Side;
		unsigned char SectorID;
		unsigned char SectorSize;
	public:
		CFileSector() :
		  Track(-1), Side(-1), SectorID((unsigned char)-1), SectorSize((unsigned char)-1)
		{}
		CFileSector(int track, int side, unsigned char sectorID, unsigned char sectorSize) :
		  Track(track), Side(side), SectorID(sectorID), SectorSize(sectorSize)
		{}
	};
	typedef vector< CFileSector >	FileSectorList;

public:
	//! Return disc geometry
	DSK_GEOMETRY& DiscGeometry();
	//! Format a given track
	void FormatTrack(int track, int side, const CCPCTrackFormat &format);
	//! Add raw file on disc
	void PutRawFile(const string &filename, int &track, int &side, unsigned char &sectorID, FileSectorList &sectorList, bool useDoubleSide = false);

	/// Lit un bloc sur le disc (buffer deja alloue !)
	virtual void ReadBlock(const unsigned char i_idBlock, void* i_buffer) const;
	/// Ecrit un secteur sur le disc
	virtual void WriteBlock(const unsigned char i_idBlock, const void* i_buffer);

protected:
	/// Renvoie la liste des block libres
	virtual void getEmptyBlock(std::vector<unsigned int> &o_block) const;

	//! Get sector format
	void GetSectorFormat(int track, int side, unsigned char sectorID, DSK_FORMAT &format);
	//! Return next sector
	void NextSector(int &track, int &side, unsigned char &sectorID, bool useDoubleSide);
};
