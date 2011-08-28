/**
* @class CCPCDisc
* Use to manage CPC disc format
* @author Thierry JOUIN
* @version 2.0
*/

#include "CCPCDisc.h"

#include "CCPCUnknownDisc.h"
#include "CCPCDataDisc.h"
#include "CCPCRomdosD1Disc.h"
#include "CCPCSystemDisc.h"

#include "CDSKFile.h"

#include "CError.h"
#include <algorithm>
#include <set>
#include <memory>

#ifdef WIN32
#define FILE_SEPARATOR '\\'
#else
#define FILE_SEPARATOR '/'
#endif

const char* CCPCDisc::TDiscDesc[] =
{
	"unknown",
	"cpcdata",
	"cpcdata42",
	"cpcsys",
	"cpcsys42",
	"pcw720", // romdos D1
	NULL
};

const CCPCDisc::CDiscFormat CCPCDisc::TDiscFormat[] =
{
	CDiscFormat(0,    0,  0, 0),
	CDiscFormat(1024, 64, 1, 16),
	CDiscFormat(1024, 64, 1, 16),
	CDiscFormat(1024, 64, 1, 16),
	CDiscFormat(1024, 64, 1, 16),
	CDiscFormat(2048, 128,2, 8)
};

/*
une piste fait 6250 octets
12 octets de synchro
3 octets d'id
1 octet = &FE
4 octets d'id
2 octets de controle
22 octets de GAP &4E
12 octets de synchro
3 octets d'id
1 octet -&fb
n octet de données
2 octets de crc
n octet de gap &3e

sectorIDSize = 12+3+1+4+2+22+12+3+1+2 = 62
*/

const unsigned int CCPCDisc::NoBlock = (0-1);
const unsigned int CCPCDisc::DeleteUser = 229;
const unsigned int CCPCDisc::EntrySize = 32;
const unsigned char CCPCDisc::FormatByte = 0xe5;
const unsigned int CCPCDisc::MaxTrackSize = 6250;
const unsigned int CCPCDisc::SectorHeaderSize = 62;
const unsigned int CCPCDisc::MaxSectorPerTrack = 32;
	// will never exceed 19 sectors size 0 per tracks !
const unsigned int CCPCDisc::MaxGAPSize = 0x4e;
const unsigned int CCPCDisc::SectorGAPSizeArray[] =
{
	// sector size, number sector, gap size
    0,28,0x20,
    0,27,0x29,
    0,26,0x32,
    0,25,0x3c,
    0,24,0x47,

    1,19,0x3,
    1,18,0x15,
    1,17,0x29,
    1,16,0x3f,

    2,10,0x24,
	(unsigned int)-1
};

////////////////////////////////////////////////////
// CPC file entry key methods

CCPCDisc::CDiscFileEntryKey::CDiscFileEntryKey() :
User(0),
WriteProtected(false),
System(false)
{
	memset(Name,11,1);
}

CCPCDisc::CDiscFileEntryKey::CDiscFileEntryKey(const string &i_name) :
User(0),
WriteProtected(false),
System(false)
{
	convertStringToFilename(i_name);
}

string CCPCDisc::CDiscFileEntryKey::getFilename() const
{
	int i=0;
	string filename;
	while (i<8 && Name[i] != ' ')
    {
		filename += Name[i++];
    }
	i = 8;
	filename += '.';
	while (i<11 && Name[i] != ' ')
		filename += Name[i++];

	return filename;
}

void CCPCDisc::CDiscFileEntryKey::convertStringToFilename(const string &i_name)
{
	unsigned int i;
	unsigned int p = i_name.find_last_of('.');

	string n = i_name;
	string e;
	if (n.find(FILE_SEPARATOR) != string::npos)
    {
		n = n.substr(n.find_last_of(FILE_SEPARATOR)+1,n.size()
			- n.find_last_of(FILE_SEPARATOR));
    }
	if (n.find('.') != string::npos)
	{
		e = n.substr(n.find('.')+1, n.size() - n.find('.'));
		n = n.substr(0, n.find('.'));
	}
	for (i=0;i<8;i++)
	{
		if (i<n.size())
			Name[i]=toupper(n[i]);
		else
			Name[i]=' ';
	}
	for (i=0;i<3;i++)
	{
		if (i<e.size())
			Name[i+8]=toupper(e[i]);
		else
			Name[i+8]=' ';
	}
}

bool CCPCDisc::CDiscFileEntryKey::operator()(const CDiscFileEntryKey &i_file1,const CDiscFileEntryKey &i_file2) const
{
	return ( (i_file1.User < i_file2.User) || ((i_file1.User == i_file2.User)
		&& (strncmp(i_file1.Name,i_file2.Name,11) < 0)) );
}

////////////////////////////////////////////////////
// CPC file entry methods

CCPCDisc::CDiscFileEntry::CDiscFileEntry() :
Size(0),
NbRecord(0),
Blocks()
{
}

void CCPCDisc::CDiscFileEntry::CleanBlocks()
{
	Blocks.erase(remove(Blocks.begin(), Blocks.end(), CCPCDisc::NoBlock), Blocks.end());
}

////////////////////////////////////////////////////
// CPC disc format methods

CCPCDisc::CDiscFormat::CDiscFormat() :
BlockSize(-1),
NbMaxEntry(-1),
    BlockIDSize(-1),
    NbBlocksPerEntry(-1)
{
}

CCPCDisc::CDiscFormat::CDiscFormat(int blockSize, int maxEntry, int blockIDSize, int blocksPerEntry) :
BlockSize(blockSize),
NbMaxEntry(maxEntry),
    BlockIDSize(blockIDSize),
    NbBlocksPerEntry(blocksPerEntry)
{
}
CCPCDisc::CDiscFormat::CDiscFormat(const CDiscFormat &disc) :
BlockSize(disc.BlockSize),
NbMaxEntry(disc.NbMaxEntry),
    BlockIDSize(disc.BlockIDSize),
    NbBlocksPerEntry(disc.NbBlocksPerEntry)
{
}

int CCPCDisc::CDiscFormat::GetDirectorySize() const
{
	return (NbMaxEntry * CCPCDisc::EntrySize);
}
int CCPCDisc::CDiscFormat::GetRecordSize() const
{
//	return ((NbBlocksPerEntry*BlockSize) >> 7);
    return 128;
}

CCPCDisc::CTrack::CTrack() :
Data(new unsigned char[CCPCDisc::MaxTrackSize]),
Geometry(),
SectorIDArray(new DSK_FORMAT[CCPCDisc::MaxSectorPerTrack]),
FDCStatus1Array(new int[CCPCDisc::MaxSectorPerTrack]),
FDCStatus2Array(new int[CCPCDisc::MaxSectorPerTrack])
{
}

CCPCDisc::CTrack::CTrack(const CTrack &i_track) :
Data(new unsigned char[CCPCDisc::MaxTrackSize]),
Geometry(i_track.Geometry),
SectorIDArray(new DSK_FORMAT[CCPCDisc::MaxSectorPerTrack]),
FDCStatus1Array(new int[CCPCDisc::MaxSectorPerTrack]),
FDCStatus2Array(new int[CCPCDisc::MaxSectorPerTrack])
{
	memcpy(&Data, &i_track.Data, sizeof(unsigned char)*CCPCDisc::MaxTrackSize);
	memcpy(&SectorIDArray, &i_track.SectorIDArray, sizeof(DSK_FORMAT)*CCPCDisc::MaxSectorPerTrack);
	memcpy(&FDCStatus1Array, &i_track.FDCStatus1Array, sizeof(unsigned char)*CCPCDisc::MaxSectorPerTrack);
	memcpy(&FDCStatus2Array, &i_track.FDCStatus2Array, sizeof(unsigned char)*CCPCDisc::MaxSectorPerTrack);
}

CCPCDisc::CTrack::~CTrack()
{
	delete[] Data;
	delete[] SectorIDArray;
	delete[] FDCStatus1Array;
	delete[] FDCStatus2Array;
}

void CCPCDisc::CTrack::ComputeGapValue()
{
	if (SectorIDArray == NULL)
		return;
	if (Geometry.dg_sectors == 0)
		return;

	bool allSameSize = true;
	unsigned int secsize = SectorIDArray[0].fmt_secsize;
	for (unsigned int s=0 ; s<Geometry.dg_sectors && allSameSize ; s++)
	{
		allSameSize = allSameSize && (SectorIDArray[s].fmt_secsize == secsize);
	}

	if (allSameSize)
	{
		unsigned char psh;
		for (psh = 0; secsize > 128; secsize /= 2)
			psh++;

		Geometry.dg_fmtgap = CCPCDisc::MaxGAPSize;
		Geometry.dg_rwgap = CCPCDisc::MaxGAPSize;

		for (int i=0; CCPCDisc::SectorGAPSizeArray[i] != (unsigned int)-1; i+=3)
		{
			if (CCPCDisc::SectorGAPSizeArray[i+0] == psh &&
				CCPCDisc::SectorGAPSizeArray[i+1] == Geometry.dg_sectors)
			{
				Geometry.dg_fmtgap = CCPCDisc::SectorGAPSizeArray[i+2];
				Geometry.dg_rwgap = CCPCDisc::SectorGAPSizeArray[i+2];
			}
		}
	}
	else
	{
		unsigned int sectorSizeCount = 0;
		for (unsigned int s=0 ; s<Geometry.dg_sectors ; s++)
		{
			sectorSizeCount += (SectorIDArray[s].fmt_secsize
				+ CCPCDisc::SectorHeaderSize);
		}

		TOOLS_ASSERTMSG(sectorSizeCount < CCPCDisc::MaxTrackSize,
			"Unable to compute GAP size " << sectorSizeCount
			<< " bytes on track > " << CCPCDisc::MaxTrackSize);
		unsigned int gapSize = CCPCDisc::MaxTrackSize - sectorSizeCount;
		gapSize /= Geometry.dg_sectors;
		if (gapSize > CCPCDisc::MaxGAPSize)
		{
			gapSize = CCPCDisc::MaxGAPSize;
		}

		Geometry.dg_fmtgap = gapSize;
		Geometry.dg_rwgap = gapSize;
	}
}

void CCPCDisc::CTrack::Interlace()
{
	if (Geometry.dg_sectors < 2)
		return;
	bool linear = true;
	unsigned int s;
	for (s=1 ; s<Geometry.dg_sectors ; s++)
	{
		linear = linear && ((SectorIDArray[s].fmt_sector
			- SectorIDArray[s-1].fmt_sector) == 1);
	}
	if (!linear)
		return;

	unsigned int secBase = SectorIDArray[0].fmt_sector;
	unsigned int secSize = SectorIDArray[0].fmt_secsize;
	unsigned int nbSector = Geometry.dg_sectors;

	unsigned char *newData = new unsigned char[CCPCDisc::MaxTrackSize];
	DSK_FORMAT *newSectorIDArray = new DSK_FORMAT[CCPCDisc::MaxSectorPerTrack];
	int *newFDCStatus1Array = new int[CCPCDisc::MaxSectorPerTrack];
	int *newFDCStatus2Array = new int[CCPCDisc::MaxSectorPerTrack];

	int idxdec = ((nbSector & 1) == 0) ? (nbSector >> 1) : (nbSector >> 1) + 1;
	for (s=0 ; s<nbSector ; s++)
	{
		// Compute interlaced index
		int idx = (s >> 1) + (((s & 1) == 0) ? 0 : idxdec);

		memcpy(newData + (s * secSize), Data + (idx * secSize), secSize);
		newSectorIDArray[s] = SectorIDArray[idx];
		newFDCStatus1Array[s] = FDCStatus1Array[idx];
		newFDCStatus2Array[s] = FDCStatus2Array[idx];
	}

	delete[] Data;
	Data = newData;
	delete[] SectorIDArray;
	SectorIDArray = newSectorIDArray;
	delete[] FDCStatus1Array;
	FDCStatus1Array = newFDCStatus1Array;
	delete[] FDCStatus2Array;
	FDCStatus2Array = newFDCStatus2Array;
}

void CCPCDisc::CTrack::DisplayInfo(ostream &str) const
{
	if (Geometry.dg_sectors == 0)
	{
		str << "Not formatted" << endl;
	}
	else
	{
		str << "NbSector\t" << (int)Geometry.dg_sectors << endl;
	}
	str << "Data rate\t";
	switch (Geometry.dg_datarate)
	{
	case RATE_HD: str << "High density" << endl; break;
	case RATE_DD: str << "Double density" << endl; break;
	case RATE_SD: str << "Single density" << endl; break;
	case RATE_ED: str << "Extended density" << endl; break;
	}
	str << "Recording\t" << ((Geometry.dg_fm == 0) ? "MFM" : "FM") << endl;
	str << "GAP #3\t\t#" << hex << (int)Geometry.dg_fmtgap << dec << endl;

	int offset = 0;

	for (unsigned int s=0 ; s<Geometry.dg_sectors ; s++)
	{
		unsigned char checksum = 0;
		for (unsigned int c=0 ; c<SectorIDArray[s].fmt_secsize ; c++)
		{
			checksum += Data[offset++];
		}

		unsigned char FDCStatus1 = FDCStatus1Array[s];
		unsigned char FDCStatus2 = FDCStatus2Array[s];

		int psh = 0;
		int secsize = SectorIDArray[s].fmt_secsize;
		while (secsize != 128)
		{
			secsize = secsize >> 1;
			psh++;
		}

		str << "\tSect ";
		str.setf(ios::dec, ios::basefield);
		str.fill(' ');
		str.width(3);
		str << s << ":ID #";
		str.setf(ios::hex, ios::basefield);
		str.fill('0');
		str.width(2);
		str << (int)SectorIDArray[s].fmt_sector << ",Size #";
		str.setf(ios::hex, ios::basefield);
		str.fill('0');
		str.width(4);
		str << (int)(128 << psh) << "(" << (int)psh << "),Cyl ";
		str.setf(ios::dec, ios::basefield);
		str.fill(' ');
		str.width(3);
		str << (int)SectorIDArray[s].fmt_cylinder << ",Head ";
		str.fill(' ');
		str.width(3);
		str << (int)SectorIDArray[s].fmt_head << ",Chk #";
		str.setf(ios::hex, ios::basefield);
		str.fill('0');
		str.width(2);
		str << (int)checksum;

		if ((FDCStatus1 & 0x80) != 0)
			str << "/EN";
		if ((FDCStatus1 & 0x20) != 0)
			str << "/DE";
		if ((FDCStatus1 & 0x10) != 0)
			str << "/OR";
		if ((FDCStatus1 & 0x04) != 0)
			str << "/ND";
		if ((FDCStatus1 & 0x02) != 0)
			str << "/NW";
		if ((FDCStatus1 & 0x01) != 0)
			str << "/MA";

		if ((FDCStatus2 & 0x40) != 0)
			str << "/CM";
		if ((FDCStatus2 & 0x20) != 0)
			str << "/DD";
		if ((FDCStatus2 & 0x10) != 0)
			str << "/WC";
		if ((FDCStatus2 & 0x08) != 0)
			str << "/SH";
		if ((FDCStatus2 & 0x04) != 0)
			str << "/SN";
		if ((FDCStatus2 & 0x02) != 0)
			str << "/BC";
		if ((FDCStatus2 & 0x01) != 0)
			str << "/MD";

		str.setf(ios::dec, ios::basefield);

		str << endl;
	}
}

bool CCPCDisc::CTrack::IsEqual(const CTrack &track) const
{
	if (Geometry.dg_sectors != track.Geometry.dg_sectors)
		return false;

	int dataPtr = 0;
	for (unsigned int s=0 ; s<Geometry.dg_sectors ; s++)
	{
		if (SectorIDArray[s].fmt_cylinder != track.SectorIDArray[s].fmt_cylinder 
			|| SectorIDArray[s].fmt_head != track.SectorIDArray[s].fmt_head
			|| SectorIDArray[s].fmt_secsize
				!= track.SectorIDArray[s].fmt_secsize
			|| SectorIDArray[s].fmt_sector != track.SectorIDArray[s].fmt_sector)
			return false;

		if (memcmp(Data+dataPtr, track.Data+dataPtr, 	
			SectorIDArray[s].fmt_secsize) != 0)
		{
			int dataPtr2 = 0;
			for (unsigned int s2=0 ; s2<Geometry.dg_sectors ; s2++)
			{
				if (memcmp(Data, track.Data+dataPtr2, 
					SectorIDArray[s].fmt_secsize) == 0)
				{
					cout << s << ":" << SectorIDArray[s].fmt_sector << " = "
						<< s2 << ":" << track.SectorIDArray[s2].fmt_sector
						<< endl;
				}
			}
			dataPtr2 += SectorIDArray[s].fmt_secsize;

			return false;
		}

		dataPtr += SectorIDArray[s].fmt_secsize;
	}

	return true;
}

void CCPCDisc::CTrack::DisplayDifference(const CTrack &track, ostream &str) const
{
}

////////////////////////////////////////////////////
// CPC Disc methods

CCPCDisc::CCPCDisc() :
_filename(),
_driver(NULL),
_format(maxTDisc),
_discFormat(),
_directory(),
_directoryBuffer(NULL),
_isDirectoryChanged(false),
_needAdvancedMode(false),
_interlacedSectors(true)
{
}

CCPCDisc::~CCPCDisc()
{
	if (_directoryBuffer != NULL)
		delete[] _directoryBuffer;
}

void CCPCDisc::readDirectory()
{
	// Read buffer
	readDirectoryBuffer();

	unsigned char *currentEntry = _directoryBuffer;
	// For all directory entry...
	for (int e=0;e<_discFormat.NbMaxEntry;e++)
    {
		CDiscFileEntryKey name;
		CDiscFileEntry nameValues;

		name.User = (unsigned char) currentEntry[0];

		strncpy(name.Name,(char*)currentEntry+1,11);

		// get flags
		name.WriteProtected = ((name.Name[8] & 128) != 0);
		name.System = ((name.Name[9] & 128) != 0);

		bool validName = true;

		// clean name
		for (unsigned int i=0;i<11;i++)
			validName = validName && ((name.Name[i] & 127)>=' ')
				&& (name.Name[i]!=(char)CCPCDisc::DeleteUser);

		if (validName)
		{
			for (unsigned int i=0;i<11;i++)
				name.Name[i]=name.Name[i] & 127;

			CCPCDirectoryMap::iterator pos = _directory.find(name);

			if (pos == _directory.end())
			{
				nameValues.Blocks = vector<unsigned int>(256,CCPCDisc::NoBlock);
				nameValues.Size = 0;
				nameValues.NbRecord = 0;
				_directory[name]=nameValues;
			}

			unsigned char ordreChargement = currentEntry[12];

			// read blocks
			CDiscFileEntry &dirEntry = _directory[name];

			bool addEntry = true;
			for(unsigned int b=16; b<CCPCDisc::EntrySize;
				b+=_discFormat.BlockIDSize)
			{
				if ((_discFormat.BlockIDSize == 1 || currentEntry[b+1]==0)
					&& currentEntry[b] == 0 )
					continue;

				unsigned int blockID = ordreChargement
					* _discFormat.NbBlocksPerEntry+(b-16);
				if (blockID > dirEntry.Blocks.size() ||
					dirEntry.Blocks[blockID] != CCPCDisc::NoBlock)
				{
					// Already have this name, and loaded blocks
					// Can append only on deleted files
					addEntry = false;
					break;
				}

				dirEntry.Blocks[blockID] = currentEntry[b]
					+ (_discFormat.BlockIDSize-1)*256*currentEntry[b+1];
				dirEntry.Size++;
			}
			if (addEntry)
				dirEntry.NbRecord += (unsigned char)currentEntry[0xf];

		}
		currentEntry += CCPCDisc::EntrySize;
    }

	// Clean each entry block
	for (CCPCDirectoryMap::iterator it = _directory.begin() ;
			it != _directory.end() ; it++)
		(*it).second.CleanBlocks();
}

void CCPCDisc::writeDirectory()
{
	if (_isDirectoryChanged)
    {
		unsigned char* pCatBuffer = _directoryBuffer;

		// On vide le directory courant
		for (int i=0;i<_discFormat.GetDirectorySize();i++)
			_directoryBuffer[i]=(char)CCPCDisc::FormatByte;

		// On en recrée un nouveau
		for (CCPCDirectoryMap::const_iterator it=_directory.begin();
			it!=_directory.end();it++)
		{
			int nbEntry = (int)ceil(it->second.Size
				/ (float)_discFormat.NbBlocksPerEntry);
			char name[11];
			char ordreChargement=0;
			unsigned int nbRecord = it->second.NbRecord;
			unsigned int nbRecordMaxPerEntry = (_discFormat.BlockSize
				* _discFormat.NbBlocksPerEntry)/_discFormat.GetRecordSize();

			memcpy(name,it->first.Name,11);
//			cout << "Writing file " << name << endl;
			name[8] = it->first.WriteProtected ? name[8]|128 : name[8];
			name[9] = it->first.System ? name[9]|128 : name[9];
//			cout << name << " is " << nbEntry << " entries " << endl ;
//			cout << "Size in blocks is " << it->second.Size << endl;
			for (int j=0;j<nbEntry;j++)
			{
				for (unsigned int k=0;k<CCPCDisc::EntrySize;k++)
					pCatBuffer[k]=0;

				pCatBuffer[0] = (char)it->first.User;	// Numéro d'USER
				memcpy(pCatBuffer+1,name,11);		// Nom du fichier
				pCatBuffer[12] = ordreChargement;
					// Numéro du bloc dans le fichier
				pCatBuffer[15] = (nbRecord > nbRecordMaxPerEntry) ? 		
					nbRecordMaxPerEntry : nbRecord;
					// Taille du bloc (attention, pour le dernier bloc il faut
					// indiquer la taille restante ?)
				for (unsigned int b=16;b<CCPCDisc::EntrySize;b+=_discFormat.BlockIDSize)
				{
					unsigned int bI = (ordreChargement*_discFormat.NbBlocksPerEntry + (b/_discFormat.BlockIDSize) - _discFormat.NbBlocksPerEntry);
					if (bI < it->second.Size)
					{
//					    cout << name << " block " << b << " is number " 
//							<< it->second.Blocks[bI] << " at offset " << b
//							<<  " and entry number " << (int)ordreChargement
//							<< endl;
					    pCatBuffer[b] = it->second.Blocks[bI] % 256;
					    if(_discFormat.BlockIDSize==2)
					    	pCatBuffer[b+1] = it->second.Blocks[bI] / 256;
					}
					    
				}
				pCatBuffer += CCPCDisc::EntrySize;
				ordreChargement++;
				nbRecord -= nbRecordMaxPerEntry;
			}
		}
		writeDirectoryBuffer();
    }
}

void CCPCDisc::addDirectoryEntry(const CDiscFileEntryKey &i_name, const CDiscFileEntry &i_nameData)
{
    // Récupère le nombre d'entrées nécessaires pour ce fichier, vérifie qu'on a 	// assez de place dans le catalogue
//    cout << i_name.Name << " size is " << i_nameData.Size << endl;
    int nbEntry = getNbEntry(i_nameData.Size);
    TOOLS_ASSERTMSG( (nbEntry < (_discFormat.NbMaxEntry-getNbUsedEntry())) , "Directory Full");

    // Regarde si une entrée avec ce nom existe déjà�
    CCPCDirectoryMap::iterator iNorm = _directory.find(i_name);

    // Si le fichier existe déjà
    if (iNorm != _directory.end())
    {
	// On renomme l'ancien en .BAK
	CDiscFileEntryKey name_bak = i_name;
	name_bak.Name[8]='B';name_bak.Name[9]='A';name_bak.Name[10]='K';

	// On regarde si il n'y a pas déjà un .BAK
	CCPCDirectoryMap::iterator iBak = _directory.find(name_bak);

	if (iBak != _directory.end())
	{
	    // Si oui, on l'efface
	    CDiscFileEntryKey name_bak_del = name_bak;
	    name_bak_del.User=CCPCDisc::DeleteUser;

	    // On met à jour le catalogue
	    _directory[name_bak_del] = _directory[name_bak];
	}
	_directory[name_bak]=_directory[i_name];
    }
    _directory[i_name]=i_nameData;

    _isDirectoryChanged = true;

}

void CCPCDisc::removeDirectoryEntry(const CDiscFileEntryKey &i_name)
{
	CCPCDirectoryMap::iterator iNorm = _directory.find(i_name);

	if (iNorm != _directory.end())
    {
		CDiscFileEntryKey name_del = i_name;
		name_del.User = CCPCDisc::DeleteUser;
		_directory[name_del]=_directory[i_name];
		_directory.erase(iNorm);
    }
	else
    {
		WARNING("File not in the directory !");
    }
	_isDirectoryChanged = true;
}

int CCPCDisc::getNbEntry(const int fileSizeKB) const
{
	int fileSize = fileSizeKB * 1024;
	int nbBlocks = (int)(ceil((float)fileSize / (float)_discFormat.BlockSize));
	int nbEntry = (int)(ceil(float(nbBlocks) / (float)_discFormat.NbBlocksPerEntry));

	return nbEntry;
}

int CCPCDisc::getNbUsedEntry() const
{
	int nbUsed = 0;
	for (CCPCDirectoryMap::const_iterator i=_directory.begin();i!=_directory.end();i++)
    {
		nbUsed+= i->second.Size*1024 / (_discFormat.BlockSize * _discFormat.NbBlocksPerEntry);
    }
	return nbUsed;
}

int CCPCDisc::getDirectoryEntryForBlock(unsigned int i_block) const
{
	int entry = -1;
	int numEntry = 0;
	CCPCDirectoryMap::const_iterator e = _directory.begin();
	while (e!=_directory.end() && entry == -1)
    {
		if (e->first.User != CCPCDisc::DeleteUser)
			for (unsigned int i=0;i<e->second.Blocks.size();i++)
				if (e->second.Blocks[i] == i_block)
					return numEntry;
		e++;
		numEntry++;
    }
	return entry;
}


void CCPCDisc::readDirectoryBuffer()
{
	memset(_directoryBuffer, CCPCDisc::FormatByte, _discFormat.BlockSize*2);
	try
	{
		ReadBlock(0,_directoryBuffer);
		ReadBlock(1,_directoryBuffer+_discFormat.BlockSize);
	}
	catch (tools::CException &e)
    {
		MSG("Error while reading directory " << endl << e);
    }
}

void CCPCDisc::writeDirectoryBuffer()
{
	WriteBlock(0,_directoryBuffer);
	WriteBlock(1,_directoryBuffer+_discFormat.BlockSize);
}

void CCPCDisc::ReadSector(const unsigned char i_cylinder, const unsigned char i_head, const unsigned char i_sectorID, void* o_buffer, bool advanced) const
{
	TOOLS_ASSERTMSG( (i_cylinder < _geometry.dg_cylinders) , "Invalid track ID " << (int)i_cylinder << " for " << (int)_geometry.dg_cylinders);
	TOOLS_ASSERTMSG( (i_head < _geometry.dg_heads) , "Invalid head");
	dsk_err_t e;
	e = dsk_pread(_driver,&_geometry,o_buffer,i_cylinder,i_head,i_sectorID);

	TOOLS_ASSERTMSG( (e == DSK_ERR_OK) , "Error reading sector :" << string(dsk_strerror(e)));
}
void CCPCDisc::WriteSector(const unsigned char i_cylinder, const unsigned char i_head, const unsigned char i_sectorID, const void* i_buffer, bool advanced)
{
	TOOLS_ASSERTMSG( (i_cylinder<_geometry.dg_cylinders) , "Invalid track ID " << i_cylinder << " for " << (int)_geometry.dg_cylinders);
	TOOLS_ASSERTMSG( (i_head < _geometry.dg_heads) , "Invalid head");

	dsk_err_t e;
	if (!advanced)
	{
		e = dsk_pwrite(_driver,&_geometry,i_buffer,i_cylinder,i_head,i_sectorID);
	}
	else
	{
		DSK_FORMAT *fmt_track = NULL;
		DSK_FORMAT sect;
		dsk_psect_t nbSector = 0;

		e = dsk_ptrackids(_driver, &_geometry, i_cylinder, i_head, &nbSector, &fmt_track);
		TOOLS_ASSERTMSG( (e == DSK_ERR_OK) , "Error reading track IDs :" << string(dsk_strerror(e)));

		int s;
		for (s = 0 ; s < nbSector ; s++)
		{
			if (fmt_track[s].fmt_sector == i_sectorID)
			{
				sect = fmt_track[s];
				break;
			}
		}
		TOOLS_ASSERTMSG( (s != nbSector) , "Error finding sector ID " << i_sectorID << " on track " << i_cylinder);

		e = dsk_xwrite(_driver, &_geometry, i_buffer, i_cylinder, i_head, sect.fmt_cylinder, sect.fmt_head, sect.fmt_sector, sect.fmt_secsize, 0);
	}

	TOOLS_ASSERTMSG( (e == DSK_ERR_OK) , "Error writing sector :" << string(dsk_strerror(e)));
}

bool CCPCDisc::ReadTrack(const unsigned char i_cylinder, const unsigned char i_head, CTrack &i_track, bool extended) const
{
	DSK_FORMAT *fmt_track = NULL;
	dsk_err_t e;

	DSK_GEOMETRY &geom = i_track.Geometry;
	geom = _geometry;
	geom.dg_noskip = 1;
	geom.dg_datarate = RATE_SD;

	if (extended)
	{
		// In extended mode
		// First search for a no-existing sector
		// Then find number of sector by secid many times since we have complete a track
		unsigned char noSectorData[CCPCDisc::MaxTrackSize];
		unsigned char checksum[CCPCDisc::MaxSectorPerTrack];
		unsigned int lcount = 0;
		unsigned int nbSector = 0;
		dsk_psect_t noSector = (dsk_psect_t)-1;
		unsigned int totalSize = 0;
		unsigned int gap3Size = CCPCDisc::MaxGAPSize;
		int maxSectorSize = 0;

		DSK_GEOMETRY gtemp;
		gtemp = geom;
		gtemp.dg_secsize = 128;
		gtemp.dg_datarate = RATE_SD;

		do
		{
			e = dsk_pread(_driver,&gtemp,noSectorData,i_cylinder,i_head, ++noSector);
		}
		while (e != DSK_ERR_NOADDR);

		memset(i_track.Data, 0xe5, CCPCDisc::MaxTrackSize);

		unsigned char *buf = i_track.Data;

		geom.dg_datarate = RATE_SD;

		// Found the possible number of sector per track
		e = DSK_ERR_OK;
		while (lcount < CCPCDisc::MaxSectorPerTrack)
		{
			e = dsk_pread(_driver,&gtemp,noSectorData,i_cylinder,i_head, noSector);
			e = DSK_ERR_OK;
			unsigned int s;
			for (s=0 ; s<(lcount+1) && e == DSK_ERR_OK; s++)
			{
				e = dsk_psecid(_driver,&geom,i_cylinder,i_head,&i_track.SectorIDArray[nbSector]);
			}
			if (e != DSK_ERR_OK)
				break;
			int deleted = 0;
			geom.dg_secsize = i_track.SectorIDArray[nbSector].fmt_secsize;
			e = dsk_xread(_driver, &geom, buf, i_cylinder, i_head,
				i_track.SectorIDArray[nbSector].fmt_cylinder,
				i_track.SectorIDArray[nbSector].fmt_head,
				i_track.SectorIDArray[nbSector].fmt_sector,
				i_track.SectorIDArray[nbSector].fmt_secsize, &deleted);

			e = dsk_get_option(_driver, "ST1", &i_track.FDCStatus1Array[nbSector]);
			if (e != DSK_ERR_OK)
				break;
			e = dsk_get_option(_driver, "ST2", &i_track.FDCStatus2Array[nbSector]);
			if (e != DSK_ERR_OK)
				break;

			checksum[nbSector] = 0;
			for (unsigned int d=0;d<i_track.SectorIDArray[nbSector].fmt_secsize;d++)
			{
				checksum[nbSector] += buf[d];
			}

			unsigned char *cmpbuf = i_track.Data;
			bool found = false;
			for (s=0 ; s<nbSector && !found; s++)
			{
				if (checksum[nbSector] == checksum[s] &&
					i_track.SectorIDArray[nbSector].fmt_secsize == i_track.SectorIDArray[s].fmt_secsize &&
					i_track.SectorIDArray[nbSector].fmt_sector == i_track.SectorIDArray[s].fmt_sector)
				{
					found = (memcmp(buf, cmpbuf, i_track.SectorIDArray[nbSector].fmt_secsize) == 0);
				}
				cmpbuf += i_track.SectorIDArray[nbSector].fmt_secsize;
			}

			if (found)
			{
				break;
			}
			else
			{
				maxSectorSize = (maxSectorSize < (int)i_track.SectorIDArray[nbSector].fmt_secsize) ? i_track.SectorIDArray[nbSector].fmt_secsize : maxSectorSize;

				nbSector++;
				// Compute size if we add another sector to see if we must stop here
				totalSize = (maxSectorSize + CCPCDisc::SectorHeaderSize) * (nbSector + 1);
				gap3Size = (CCPCDisc::MaxTrackSize - totalSize) / (nbSector + 1);
				gap3Size = (gap3Size > CCPCDisc::MaxGAPSize) ? CCPCDisc::MaxGAPSize : gap3Size;

				if (totalSize >= CCPCDisc::MaxTrackSize || gap3Size <= 2)
					break;

				buf += i_track.SectorIDArray[nbSector-1].fmt_secsize;
			}
			lcount++;
		}

		i_track.Geometry.dg_secsize = maxSectorSize;
		i_track.Geometry.dg_sectors = nbSector;

		if (!lcount) return false;	/* Track blank */

		i_track.ComputeGapValue();
	}
	else
	{
		e = dsk_ptrackids(_driver, &geom, i_cylinder, i_head, &geom.dg_sectors, &fmt_track);

		if (fmt_track == NULL)
		{
			geom.dg_sectors = 0;
			return false;
		}

		unsigned int s;
		for (s=0 ; s<geom.dg_sectors ; s++)
		{
			i_track.SectorIDArray[s] = fmt_track[s];
		}
		i_track.ComputeGapValue();

		memset(i_track.Data, 0xe5, CCPCDisc::MaxTrackSize);

		unsigned char *buf = i_track.Data;

		geom.dg_secsize = 0;
		for (s=0 ; s<geom.dg_sectors ; s++)
		{
			geom.dg_secsize = (fmt_track[s].fmt_secsize > geom.dg_secsize) ? fmt_track[s].fmt_secsize : geom.dg_secsize;
			i_track.FDCStatus1Array[s] = 0;
			i_track.FDCStatus2Array[s] = 0;

			int deleted = 0;
			e = dsk_xread(_driver, &geom, buf, i_cylinder, i_head,
				fmt_track[s].fmt_cylinder,
				fmt_track[s].fmt_head,
				fmt_track[s].fmt_sector,
				fmt_track[s].fmt_secsize, &deleted);

			TOOLS_ASSERTMSG( (e == DSK_ERR_OK) , "Error reading track :" << string(dsk_strerror(e)));

			dsk_get_option(_driver, "ST1", &i_track.FDCStatus1Array[s]);
			TOOLS_ASSERTMSG( (e == DSK_ERR_OK) , "Error reading track :" << string(dsk_strerror(e)));
			dsk_get_option(_driver, "ST2", &i_track.FDCStatus2Array[s]);
			TOOLS_ASSERTMSG( (e == DSK_ERR_OK) , "Error reading track :" << string(dsk_strerror(e)));

			buf += fmt_track[s].fmt_secsize;
		}
	}

	return true;
}

bool CCPCDisc::WriteTrack(const unsigned char i_cylinder, const unsigned char i_head, const CTrack &i_track, bool extended)
{
	dsk_err_t e;

	const DSK_GEOMETRY &geom = i_track.Geometry;

	if (geom.dg_sectors == 0)
	{
		e = dsk_pformat(_driver, (DSK_GEOMETRY*)&geom, i_cylinder, i_head, i_track.SectorIDArray, CCPCDisc::FormatByte);
		TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error writing track :" << string(dsk_strerror(e)));
	}
	else
	{
		e = dsk_pformat(_driver, (DSK_GEOMETRY*)&geom, i_cylinder, i_head, i_track.SectorIDArray, CCPCDisc::FormatByte);
		TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error writing track :" << string(dsk_strerror(e)));

		if (extended)
		{
			// In extended mode, we first sync to sector using false reading + secid
			unsigned int noSector = (unsigned int)-1;
			unsigned int s;
			do
			{
				noSector++;
				for (s=0 ; s<geom.dg_sectors && i_track.SectorIDArray[s].fmt_sector != noSector; s++);
			}
			while (s != geom.dg_sectors);

			DSK_GEOMETRY gtemp = i_track.Geometry;
			unsigned char noSectorData[CCPCDisc::MaxTrackSize];
			gtemp.dg_secsize = 128;

			unsigned char *buffer = i_track.Data;
			for (s=0 ; s<geom.dg_sectors ; s++)
			{
				DSK_FORMAT &sect = i_track.SectorIDArray[s];
				DSK_FORMAT sectTmp;

				int deleted = ((i_track.FDCStatus1Array[s] & 0x40) != 0) ? 1 : 0;

				e = dsk_pread(_driver,&gtemp,noSectorData,i_cylinder,i_head, noSector);
				e = DSK_ERR_OK;
				for (unsigned int sid=0 ; sid<(s+1) && e == DSK_ERR_OK; sid++)
				{
					e = dsk_psecid(_driver,&geom,i_cylinder,i_head,&sectTmp);
				}

				dsk_set_option(_driver, "ST1", i_track.FDCStatus1Array[s]);
				dsk_set_option(_driver, "ST2", i_track.FDCStatus2Array[s]);

				e = dsk_xwrite(_driver, &geom, buffer, i_cylinder, i_head, sect.fmt_cylinder, sect.fmt_head, sect.fmt_sector, sect.fmt_secsize, deleted);
				TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error writing track :" << string(dsk_strerror(e)));
				buffer += i_track.SectorIDArray[s].fmt_secsize;
			}
		}
		else
		{
			unsigned char *buffer = i_track.Data;
			for (unsigned int s=0 ; s<geom.dg_sectors ; s++)
			{
				DSK_FORMAT &sect = i_track.SectorIDArray[s];

				int deleted = ((i_track.FDCStatus1Array[s] & 0x40) != 0) ? 1 : 0;

				dsk_set_option(_driver, "ST1", i_track.FDCStatus1Array[s]);
				dsk_set_option(_driver, "ST2", i_track.FDCStatus2Array[s]);

				e = dsk_xwrite(_driver, &geom, buffer, i_cylinder, i_head, sect.fmt_cylinder, sect.fmt_head, sect.fmt_sector, sect.fmt_secsize, deleted);
				TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error writing track :" << string(dsk_strerror(e)));
				buffer += i_track.SectorIDArray[s].fmt_secsize;
			}
		}
	}

	return true;
}

bool CCPCDisc::RepairTrack(const unsigned char i_cylinder, const unsigned char i_head, CTrack &i_trackValid, CTrack &i_trackWrong, bool extended) const
{
	dsk_err_t e;

	const DSK_GEOMETRY &geomValid = i_trackValid.Geometry;
	const DSK_GEOMETRY &geomWrong = i_trackWrong.Geometry;

	if (geomValid.dg_sectors == 0)
	{
		TOOLS_ERRORMSG( "Unable to repair unformatted track" );
	}
	else
	{
		bool trackFormat = false;
		if (geomValid.dg_sectors != geomWrong.dg_sectors)
		{
			trackFormat = true;
		}
		else
		{
			for (unsigned int s=0 ; s<geomValid.dg_sectors && !trackFormat ; s++)
			{
				trackFormat =	(i_trackValid.SectorIDArray[s].fmt_sector != i_trackWrong.SectorIDArray[s].fmt_sector) ||
								(i_trackValid.SectorIDArray[s].fmt_secsize != i_trackWrong.SectorIDArray[s].fmt_secsize) ||
								(i_trackValid.SectorIDArray[s].fmt_cylinder != i_trackWrong.SectorIDArray[s].fmt_cylinder);
			}
		}

		if (trackFormat)
		{
			e = dsk_pformat(_driver, (DSK_GEOMETRY*)&geomValid, i_cylinder, i_head, i_trackValid.SectorIDArray, CCPCDisc::FormatByte);
			TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error writing track :" << string(dsk_strerror(e)));
		}

		if (extended)
		{
			// In extended mode, we first sync to sector using false reading + secid
			unsigned int noSector = (unsigned int)-1;
			unsigned int s;
			do
			{
				noSector++;
				for (s=0 ; s<geomValid.dg_sectors && i_trackValid.SectorIDArray[s].fmt_sector != noSector; s++);
			}
			while (s != geomValid.dg_sectors);

			DSK_GEOMETRY gtemp = geomValid;
			unsigned char noSectorData[CCPCDisc::MaxTrackSize];
			gtemp.dg_secsize = 128;

			unsigned char *bufferValid = i_trackValid.Data;
			unsigned char *bufferWrong = i_trackWrong.Data;
			for (s=0 ; s<geomValid.dg_sectors ; s++)
			{
				bool repairSector = true;
				DSK_FORMAT &sectValid = i_trackValid.SectorIDArray[s];

				if (!trackFormat)
				{
					repairSector = (memcmp(bufferValid, bufferWrong, sectValid.fmt_secsize) != 0);
				}

				if (repairSector || trackFormat)
				{
					DSK_FORMAT sectTmp;
					int deleted = ((i_trackValid.FDCStatus1Array[s] & 0x40) != 0) ? 1 : 0;

					e = dsk_pread(_driver,&gtemp,noSectorData,i_cylinder,i_head, noSector);
					e = DSK_ERR_OK;
					for (unsigned int sid=0 ; sid<(s+1) && e == DSK_ERR_OK; sid++)
					{
						e = dsk_psecid(_driver,&geomValid,i_cylinder,i_head,&sectTmp);
					}

					dsk_set_option(_driver, "ST1", i_trackValid.FDCStatus1Array[s]);
					dsk_set_option(_driver, "ST2", i_trackValid.FDCStatus2Array[s]);

					e = dsk_xwrite(_driver, &geomValid, bufferValid, i_cylinder, i_head, sectValid.fmt_cylinder, sectValid.fmt_head, sectValid.fmt_sector, sectValid.fmt_secsize, deleted);
					TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error writing track :" << string(dsk_strerror(e)));
				}
				bufferValid += i_trackValid.SectorIDArray[s].fmt_secsize;
				if (!trackFormat)
				{
					bufferWrong += i_trackValid.SectorIDArray[s].fmt_secsize;
				}
			}
		}
		else
		{
			unsigned char *bufferValid = i_trackValid.Data;
			unsigned char *bufferWrong = i_trackWrong.Data;
			for (unsigned int s=0 ; s<geomValid.dg_sectors ; s++)
			{
				bool repairSector = false;
				DSK_FORMAT &sectValid = i_trackValid.SectorIDArray[s];

				if (!trackFormat)
				{
					repairSector = (memcmp(bufferValid, bufferWrong, sectValid.fmt_secsize) != 0);
				}

				if (repairSector || trackFormat)
				{
					DSK_FORMAT &sectWrong = i_trackValid.SectorIDArray[s];
					int deleted = ((i_trackValid.FDCStatus1Array[s] & 0x40) != 0) ? 1 : 0;

					dsk_set_option(_driver, "ST1", i_trackValid.FDCStatus1Array[s]);
					dsk_set_option(_driver, "ST2", i_trackValid.FDCStatus2Array[s]);

					e = dsk_xwrite(_driver, &geomValid, bufferValid, i_cylinder, i_head, sectValid.fmt_cylinder, sectValid.fmt_head, sectValid.fmt_sector, sectValid.fmt_secsize, deleted);
					TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error writing track :" << string(dsk_strerror(e)));
				}

				bufferValid += i_trackValid.SectorIDArray[s].fmt_secsize;
				if (!trackFormat)
				{
					bufferWrong += i_trackValid.SectorIDArray[s].fmt_secsize;
				}
			}
		}
	}

	return true;
}



unsigned int CCPCDisc::GetNbFiles() const
{
	return _directory.size();
}


string CCPCDisc::GetFilename(const unsigned int i_id, int &user) const
{
	unsigned int i;
	TOOLS_ASSERTMSG( (i_id<_directory.size()), "");
	CCPCDirectoryMap::const_iterator e = _directory.begin();
	for (i=0 ; i<i_id ; i++)
		e++;
	user = e->first.User;
	return (e->first.getFilename());
}


const CCPCDisc::CDiscFileEntryKey& CCPCDisc::GetFileEntry(const unsigned int i_id) const
{
	unsigned int i;
	TOOLS_ASSERTMSG( (i_id<_directory.size()), "");
	CCPCDirectoryMap::const_iterator e = _directory.begin();
	for (i=0 ; i<i_id ; i++)
		e++;
	return e->first;
}


int CCPCDisc::GetFileSize(const unsigned int i_id) const
{
	unsigned int i;
	TOOLS_ASSERTMSG( (i_id<_directory.size()), "");
	CCPCDirectoryMap::const_iterator e = _directory.begin();
	for (i=0 ; i<i_id ; i++)
		e++;
	return e->second.Size;
}

int CCPCDisc::GetDiscCapacity() const
{
	int capacity = _geometry.dg_cylinders * _geometry.dg_secsize * _geometry.dg_sectors * _geometry.dg_heads;

	capacity -= _discFormat.GetDirectorySize();

	capacity = capacity >> 10;

	return capacity;
}


const DSK_GEOMETRY& CCPCDisc::GetDiscGeometry() const
{
	return _geometry;
}

CCPCDisc::TDisc CCPCDisc::GetDiscFormat() const
{
	return _format;
}

string CCPCDisc::GetDiscFormatName() const
{
	string name = "Unknown";
	dsk_format_t dsk_format = getLibDskFormat(TDiscDesc[_format]);
	if (dsk_format != -1)
	{
		dsk_cchar_t dskName;
		dg_stdformat(NULL, dsk_format, &dskName, NULL);
		name = dskName;
	}
	return name;
}

string CCPCDisc::GetDiscFormatDescription() const
{
	string desc = "Unknown";
	dsk_format_t dsk_format = getLibDskFormat(TDiscDesc[_format]);
	if (dsk_format != -1)
	{
		dsk_cchar_t dskDesc;
		dg_stdformat(NULL, dsk_format, NULL, &dskDesc);
		desc = dskDesc;
	}
	return desc;
}

CCPCFile* CCPCDisc::GetFile(const string &i_filename, int user) const
{
	CDiscFileEntryKey key(i_filename);

	key.User = user;

	CCPCDirectoryMap::const_iterator pos = _directory.find(key);

	TOOLS_ASSERTMSG( (pos!= _directory.end()) , "Unknown filename " << i_filename);

	unsigned char *file = new unsigned char[pos->second.Size*_discFormat.BlockSize];
	for (unsigned int i=0;i<pos->second.Size;i++)
		ReadBlock(pos->second.Blocks[i],file+(_discFormat.BlockSize*i));

	CCPCFile *fileOpen = CCPCFile::openFile(file,pos->second.Size*_discFormat.BlockSize);
	delete[] file;
	return fileOpen;
}

void CCPCDisc::PutFile(const CCPCFile &i_file, const string &i_filename, int user, bool system, bool writeProtected)
{
	vector<unsigned int> emptyBlock;
	getEmptyBlock(emptyBlock);

	unsigned int neededBlock = (unsigned int)ceil((double)i_file.getDatasSize() / (double)(_discFormat.BlockSize));
	unsigned int neededRecord = (unsigned int)ceil((double)i_file.getDatasSize() / (double)(_discFormat.GetRecordSize()));

	int emptyBlockSize = emptyBlock.size();
	TOOLS_ASSERTMSG( (neededBlock <= emptyBlock.size()) , "Unable to add file to disc - Disc Full");

	unsigned char *fileBuffer = i_file.getDatas();
	unsigned int fileBufferSize = i_file.getDatasSize();

	unsigned char *block = new unsigned char[_discFormat.BlockSize];

	CDiscFileEntryKey fileEntryKey(i_filename);
	CDiscFileEntry fileEntry;

	fileEntryKey.User = user;
	fileEntryKey.System = system;
	fileEntryKey.WriteProtected = writeProtected;

	fileEntry.Size = neededBlock;
	fileEntry.NbRecord = neededRecord;
	fileEntry.Blocks.clear();

	int remainingBytes = fileBufferSize;
	for (unsigned int j=0;j<neededBlock;j++)
    {
		int copySize = (remainingBytes > _discFormat.BlockSize) ? _discFormat.BlockSize : remainingBytes;

		memset(block,0,_discFormat.BlockSize);

		memcpy(block,fileBuffer+(j*_discFormat.BlockSize),copySize);

		WriteBlock(emptyBlock[j],block);

		fileEntry.Blocks.push_back(emptyBlock[j]);

		remainingBytes -= copySize;
    }


	addDirectoryEntry(fileEntryKey,fileEntry);
	delete[] block;
}

const CCPCDisc::CCPCDirectoryMap& CCPCDisc::GetDirectoryMap() const
{
	return _directory;
}


void CCPCDisc::EraseFile(const string &i_filename, int user)
{
	CDiscFileEntryKey key(i_filename);

	key.User = user;

	removeDirectoryEntry(key);

}

void CCPCDisc::RenameFile(	const string &i_oldFilename, const string &i_newFilename,
							int old_user, int new_user,
							bool old_sys, bool new_sys,
							bool old_pro, bool new_pro)
{
	CDiscFileEntryKey from(i_oldFilename);
	CDiscFileEntryKey to(i_newFilename);

	from.User = old_user;
	from.System = old_sys;
	from.WriteProtected = old_pro;

	to.User = new_user;
	to.System = new_sys;
	to.WriteProtected = new_pro;

	CCPCDirectoryMap::iterator iFrom = _directory.find(from);
	TOOLS_ASSERTMSG( iFrom != _directory.end() , "Unknown file " << i_oldFilename);

	addDirectoryEntry(to,_directory[from]);

	_directory.erase(iFrom);

}

void CCPCDisc::DumpBlockFile(const string &filename, int user, ostream &str) const
{
	CDiscFileEntryKey fentry(filename);

	fentry.User = user;

	CCPCDirectoryMap::const_iterator it = _directory.find(fentry);

	str << "Size " << (*it).second.Size << " kb" << endl;
	str << "Nb record " << (*it).second.NbRecord << endl;
	for (unsigned int i=0 ; i < (*it).second.Blocks.size() ; i++)
	{
		str << (int)(*it).second.Blocks[i] << "\t";
	}
	str << endl;
}

dsk_format_t CCPCDisc::getLibDskFormat(const string &name)
{
	dsk_cchar_t fname;
	dsk_format_t fmt = FMT_180K;
	while (dg_stdformat(NULL, fmt, &fname, NULL) == DSK_ERR_OK)
	{
			if (!strcasecmp(name.c_str(), fname))
				return fmt;
			fmt = (dsk_format_t)(fmt+1);
	}
	return (dsk_format_t)-1;
}

CCPCDisc::TDisc CCPCDisc::guessGeometry(const string &i_filename,
	DSK_PDRIVER driver, DSK_GEOMETRY &geometry, bool &needAdvancedMode,
	bool &interlaced, bool extendedGuess)
{
	TDisc format;

	try
	{
		switch(isFloppy(i_filename))
		{
		    case 1:
				format = guessFloppyGeometry(driver, geometry, needAdvancedMode,
					interlaced, extendedGuess);
			break;
		    case 0:
				format = guessDSKGeometry(i_filename, geometry,
					needAdvancedMode, interlaced, extendedGuess);
			break;
		    case 2:
				format = guessRAWGeometry(i_filename, geometry,
					needAdvancedMode, interlaced, extendedGuess);
			break;
		    default:
	//		format = -1;
			break;
		}

	}
	catch (...)
	{
		format = (TDisc)-1;
	}

	return format;
}

CCPCDisc::TDisc CCPCDisc::guessFloppyGeometry(DSK_PDRIVER self, DSK_GEOMETRY &geometry, bool &needAdvancedMode, bool &interlaced, bool extendedGuess)
{
	needAdvancedMode = false;
	interlaced = true;

	TDisc format = Unknown;

	for (int f=0 ; f < maxTDisc ; f++)
	{
		dsk_format_t dsk_format = getLibDskFormat(TDiscDesc[f]);
		if (dsk_format == -1)
		{
			continue;
		}

		DSK_GEOMETRY candidateGeom;
		DSK_FORMAT sectorId;
		dsk_err_t e;

		e = dg_stdformat(&candidateGeom, dsk_format, NULL, NULL);

		if (e != DSK_ERR_OK)
		{
			continue;
		}

		// First check sector id reading
		e = dsk_psecid(self, &candidateGeom, 0, 0, &sectorId);

		if ( e != DSK_ERR_OK )
		{
			continue;
		}

		if (sectorId.fmt_sector < candidateGeom.dg_secbase ||
			sectorId.fmt_sector >= (candidateGeom.dg_secbase + candidateGeom.dg_sectors))
		{
			continue;
		}

		// We found a right sector, if no candidate already found,
		// we use this format as a start (even if number of track is not good !)
		if (format == Unknown)
		{
			geometry = candidateGeom;
			format = (TDisc)f;
		}

		if (!extendedGuess)
			continue;

		// if found, check last track sector id reading
		e = dsk_psecid(self, &candidateGeom,candidateGeom.dg_cylinders-1, 0, &sectorId);

		if ( e != DSK_ERR_OK )
			continue;

		if (sectorId.fmt_sector < candidateGeom.dg_secbase ||
			sectorId.fmt_sector >= (candidateGeom.dg_secbase + candidateGeom.dg_sectors))
			continue;

		if ((format == Unknown) ||
			((format != Unknown) && (geometry.dg_cylinders < candidateGeom.dg_cylinders)))
		{
			geometry = candidateGeom;
			format = (TDisc)f;
		}
	}

	needAdvancedMode = (format == Unknown);

	return format;
}

CCPCDisc::TDisc CCPCDisc::guessDSKGeometry(const string &i_filename, DSK_GEOMETRY &geometry, bool &needAdvancedMode, bool &interlaced, bool extendedGuess)
{
	needAdvancedMode = false;
	interlaced = true;

	auto_ptr<CDSKFile> dskFile(CDSKFile::OpenDSKFile(i_filename));

	if (dskFile.get() == NULL)
		return (TDisc)-1;

	if (dskFile->GetHeader().GetNbTrack() == 0)
		return (TDisc)-1;

	// Init geometry
	geometry.dg_cylinders = dskFile->GetHeader().GetNbTrack();
	geometry.dg_heads = dskFile->GetHeader().GetNbSide();
	geometry.dg_sidedness = (geometry.dg_heads == 1) ? SIDES_ALT : SIDES_OUTOUT;
	CDSKTrack *firstTrack = dskFile->GetTrack(0,0);
	geometry.dg_sectors = firstTrack->NbSector;
	switch( firstTrack->DataRate )
	{
	case 1:		{ geometry.dg_datarate = RATE_SD; break;}
	case 2:		{ geometry.dg_datarate = RATE_HD; break;}
	case 3:		{ geometry.dg_datarate = RATE_ED; break;}
	default:	{ geometry.dg_datarate = RATE_SD; break;}
	}
	geometry.dg_fmtgap = firstTrack->GAP3Size;
	geometry.dg_rwgap = geometry.dg_fmtgap;
	geometry.dg_fm = (firstTrack->RecordingMode == 1) ? 1 : 0;
	geometry.dg_nomulti = 0;
	geometry.dg_noskip = 0;

	if (firstTrack->NbSector == 0)
	{
		geometry.dg_secsize = 0;
		geometry.dg_secbase = 0;
		return Unknown;
	}

	// Check first track sector

	// Check all same size sectors
	set<unsigned char> sectorID;
	bool sectorsHasSameSize = true;
	int s;
	for (s=0 ; s<firstTrack->NbSector ; s++)
	{
		sectorID.insert(firstTrack->Sectors[s].SectorID);
		sectorsHasSameSize = sectorsHasSameSize &&
			(firstTrack->Sectors[0].SectorSize == firstTrack->Sectors[s].SectorSize);
	}

	geometry.dg_secsize = 128 << firstTrack->Sectors[0].SectorSize;
	geometry.dg_secbase = *sectorID.begin();

	// We need advanced mode if sector has not same size or sector ID are not all different
	needAdvancedMode = !sectorsHasSameSize || (sectorID.size() != firstTrack->NbSector);

	if (needAdvancedMode)
		return Unknown;

	// Check sector order
	bool validSectorOrder = true;
	set<unsigned char>::iterator it;
	unsigned char oldID = (*sectorID.begin()) - 1;
	for (it = sectorID.begin() ; it != sectorID.end() ; it++)
	{
		int diff = (*it) - oldID;
		validSectorOrder = validSectorOrder && (diff == 1);
		oldID = (*it);
	}

	// Now check all track to have same sector header as first one
	bool trackAreSame = true;
	for (unsigned int t=1 ; t<geometry.dg_cylinders ; t++)
	{
		CDSKTrack *track = dskFile->GetTrack(t, 0);
		if (! track->SameHeader(*firstTrack) )
		{
			trackAreSame = false;
			sectorID.clear();
			sectorsHasSameSize = true;
			for (int s=0 ; s<track->NbSector ; s++)
			{
				sectorID.insert(track->Sectors[0].SectorID);
				sectorsHasSameSize = sectorsHasSameSize &&
					(track->Sectors[0].SectorSize == track->Sectors[s].SectorSize);
			}
			if (!sectorsHasSameSize || (sectorID.size() != track->NbSector) )
			{
				needAdvancedMode = true;
				return Unknown;
			}
		}
	}

	if (!trackAreSame || !validSectorOrder || needAdvancedMode)
		return Unknown;

	// We have normal track

	// Just check if sector are interlaced
	bool linear = true;
	for (s=1 ; s<firstTrack->NbSector ; s++)
	{
		linear = linear && ((firstTrack->Sectors[s].SectorID - firstTrack->Sectors[s-1].SectorID) == 1);
	}
	interlaced = !linear;

	TDisc format = Unknown;

	// Now check in standard format if we found something similar
	DSK_GEOMETRY closestGeom;
	for (int f=0 ; f < maxTDisc ; f++)
	{
		dsk_format_t dsk_format = getLibDskFormat(TDiscDesc[f]);
		if (dsk_format == -1)
			continue;

		DSK_GEOMETRY candidateGeom;
		dsk_err_t e;

		e = dg_stdformat(&candidateGeom, dsk_format, NULL, NULL);

		if (e != DSK_ERR_OK)
			continue;

		// Check geometry value
		// We allow that DSK can have more sectors than 'official' one
		// In this case, additionnal sectors won't be take into account
		if (candidateGeom.dg_secbase != geometry.dg_secbase ||
			candidateGeom.dg_sectors > geometry.dg_sectors ||
			candidateGeom.dg_secsize != geometry.dg_secsize)
			continue;

		// We found a right sector, if no candidate already found,
		// we use this format as a start (even if number of track is not good !)
		if (format == Unknown)
		{
			closestGeom = candidateGeom;
			format = (TDisc)f;
		}
		else
		{
			if (abs((int)(candidateGeom.dg_cylinders - geometry.dg_cylinders)) < abs((int)(closestGeom.dg_cylinders - geometry.dg_cylinders)))
			{
				closestGeom = candidateGeom;
				format = (TDisc)f;
			}
		}
	}


	return format;
}

CCPCDisc::TDisc CCPCDisc::guessRAWGeometry(const string &i_filename, DSK_GEOMETRY &geometry, bool &needAdvancedMode, bool &interlaced, bool extendedGuess)
{
    interlaced = false;
    needAdvancedMode = false;

    TDisc format = Unknown;

    // Init geometry
    geometry.dg_cylinders = 80;
    geometry.dg_heads = 1;
    geometry.dg_sidedness = SIDES_ALT;
    geometry.dg_sectors = 9;
    geometry.dg_datarate = RATE_SD;
    geometry.dg_fmtgap = 0x52;
    geometry.dg_rwgap = geometry.dg_fmtgap;
    geometry.dg_fm = 0;
    geometry.dg_nomulti = 0;
    geometry.dg_noskip = 0;
    geometry.dg_secsize = 512;
    geometry.dg_secbase = 1;

    // Now check in standard format if we found something similar
    DSK_GEOMETRY closestGeom;
    for (int f=0 ; f < maxTDisc ; f++)
    {
	dsk_format_t dsk_format = getLibDskFormat(TDiscDesc[f]);
	if (dsk_format == -1)
	{
	    continue;
	}

	DSK_GEOMETRY candidateGeom;
	dsk_err_t e;

	e = dg_stdformat(&candidateGeom, dsk_format, NULL, NULL);

	if (e != DSK_ERR_OK)
	{
	    continue;
	}

	// Check geometry value
	// We allow that DSK can have more sectors than 'official' one
	// In this case, additionnal sectors won't be take into account
	if (candidateGeom.dg_secbase != geometry.dg_secbase ||
		candidateGeom.dg_sectors > geometry.dg_sectors ||
		candidateGeom.dg_secsize != geometry.dg_secsize)
	{
	    continue;
	}

	// We found a right sector, if no candidate already found,
	// we use this format as a start (even if number of track is not good !)
	if (format == Unknown)
	{
	    closestGeom = candidateGeom;
	    format = (TDisc)f;
	}
	else
	{
	    if (abs((int)(candidateGeom.dg_cylinders - geometry.dg_cylinders)) < abs((int)(closestGeom.dg_cylinders - geometry.dg_cylinders)))
	    {
		closestGeom = candidateGeom;
		format = (TDisc)f;
	    }
	}
    }

    return format;
}

int CCPCDisc::isFloppy(const string &filename)
{
	// Haiku has a bug in the STL. Notice the ars to compare are not in the same 
	// order!
	#ifdef __HAIKU__
	if (filename.compare("/dev/disk/ufi", 0, 13) == 0)
	#else
	if (filename.compare(0,7,"/dev/sd") == 0)
	#endif
    {
		return 2; // RAW-access device (USB floppy drive on Linux or Haiku)
    }
    else if (strcasecmp(filename.c_str(), "a:")==0 ||
	    strcasecmp(filename.c_str(), "b:")==0 ||
	    strcasecmp(filename.c_str(), "/dev/fd0")==0 ||
	    strcasecmp(filename.c_str(), "/dev/fd1")==0)
	return 1; // Floppy with sectors/side handling
    else
	return 0; // DSK file
}

bool CCPCDisc::IsDSK(const string &i_filename, int i_inside)
{
	DSK_PDRIVER driver;
	dsk_err_t e;

	bool isDSK = false;
	e = dsk_open(&driver,i_filename.c_str(), NULL, NULL);
	if (e == DSK_ERR_OK)
	{
		if (isFloppy(i_filename)==1)
		{
			e = dsk_set_forcehead(driver, i_inside);
			TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error opening dsk - Cant forcehead :" << string(dsk_strerror(e)));
		}

		TDisc format;
		DSK_GEOMETRY geometry;
		bool needAdvancedMode;
		bool interlaced;

		format = guessGeometry(i_filename, driver, geometry, needAdvancedMode, interlaced, false);

		isDSK = (format != -1);

		dsk_close(&driver);
	}

	return isDSK;
}

CCPCDisc* CCPCDisc::OpenDisc(const string &i_filename, int i_inside)
{
	CCPCDisc* disc = NULL;
	DSK_PDRIVER driver;
	dsk_err_t e;

	e = dsk_open(&driver,i_filename.c_str(), NULL, NULL);
	TOOLS_ASSERTMSG((e==DSK_ERR_OK),
		"Error opening dsk - Cant open :" << string(dsk_strerror(e)));

	if (CCPCDisc::isFloppy(i_filename)==1)
	{
		e = dsk_set_forcehead(driver, i_inside);
		TOOLS_ASSERTMSG((e==DSK_ERR_OK),
			"Error opening dsk - Cant forcehead while opening :"
				<< string(dsk_strerror(e)));
	}

	TDisc format;
	DSK_GEOMETRY geometry;
	bool needAdvancedMode;
	bool interlaced;

	format = guessGeometry(i_filename, driver, geometry, needAdvancedMode,
		interlaced);

	TOOLS_ASSERTMSG( (format != -1) , "Error opening dsk : unknown geometry");

	switch (format)
	{
	    case RomdosD1:
		{
		    disc = new CCPCRomdosD1Disc;
		    disc->Open(i_filename, driver, geometry, format, needAdvancedMode,
		    	interlaced);
		    disc->readDirectory();
		    break;
		}
	    case Data:
	    case Data42:
		{
		    disc = new CCPCDataDisc;
		    disc->Open(i_filename, driver, geometry, format, needAdvancedMode, interlaced);
		    disc->readDirectory();
		    break;
		}
	    case System:
	    case System42:
		{
		    disc = new CCPCSystemDisc;
		    disc->Open(i_filename, driver, geometry, format, needAdvancedMode,
		    	interlaced);
		    disc->readDirectory();
		    break;
		}
	    case Unknown:
		{
		    disc = new CCPCUnknownDisc;
		    disc->Open(i_filename, driver, geometry, format, needAdvancedMode,
		    	interlaced);
		    break;
		}
	    default:
		{
		    TOOLS_ERRORMSG( "Error opening dsk : unknown filesystem");
		}
	}

	return disc;
}

CCPCDisc* CCPCDisc::CreateDisc(const string &i_filename, const TDisc &i_type, int i_inside)
{
    CCPCDisc *d=NULL;
    switch (i_type)
    {
	case Data:
	case Data42:
	    {
		d = new CCPCDataDisc();
		d->Create(i_type, i_filename, i_inside);
		break;
	    }
	case System:
	case System42:
	    {
		d = new CCPCSystemDisc();
		d->Create(i_type, i_filename, i_inside);
		break;
	    }
	case Unknown:
	    {
		d = new CCPCUnknownDisc();
		d->Create(i_type, i_filename, i_inside);
		break;
	    }
    }
    return d;
}

void CCPCDisc::Open(const string &i_filename, DSK_PDRIVER driver, const DSK_GEOMETRY &geometry, const TDisc &format, bool needAdvancedMode, bool interlaced)
{
    _filename = i_filename;
    _driver = driver;
    _geometry = geometry;
    _format = format;
    _needAdvancedMode = needAdvancedMode;
    _interlacedSectors = interlaced;

    _discFormat = TDiscFormat[format];

    int directorySize = _discFormat.NbMaxEntry * CCPCDisc::EntrySize;
    _directoryBuffer = new unsigned char[_discFormat.GetDirectorySize()];
}

void CCPCDisc::Create(TDisc i_type, const string &i_filename, int i_inside)
{
    _filename = i_filename;

    dsk_err_t e;

    TOOLS_ASSERTMSG( (i_type >= 0 && i_type < maxTDisc) , "Error creating dsk : unknown format");

    switch (CCPCDisc::isFloppy(i_filename))
    {
	case 1:
#ifdef WIN32
	    e = dsk_creat(&_driver,i_filename.c_str(), "ntwdm", NULL);
#else
	    e = dsk_creat(&_driver,i_filename.c_str(), "floppy", NULL);
#endif
	    break;
	case 0:
	    e = dsk_creat(&_driver,i_filename.c_str(), "edsk", NULL);
	    break;
	case 2:
	    e = dsk_creat(&_driver,i_filename.c_str(), "raw",NULL);
	    break;
    }
    TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error creating dsk :" << string(dsk_strerror(e)));

    if (isFloppy(i_filename))
    {
	e = dsk_set_option(_driver, "HEAD", i_inside);
	TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error creating dsk :" << string(dsk_strerror(e)));
    }

    dsk_format_t format;
    format = getLibDskFormat(CCPCDisc::TDiscDesc[i_type]);

    if (format != -1)
    {
	_format = i_type;
    }
    else
    {
	format = getLibDskFormat(CCPCDisc::TDiscDesc[Data]);

	TOOLS_ASSERTMSG( (format != -1) , "Error creating dsk :" << string(dsk_strerror(e)));

	_format = Unknown;
    }

    e = dg_stdformat(&_geometry, format, NULL, NULL);
    TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error creating dsk :" << string(dsk_strerror(e)));
}

void CCPCDisc::Close()
{
    if (_isDirectoryChanged)
	writeDirectory();
    dsk_close(&_driver);
}

bool CCPCDisc::IsFloppy() const
{
    return isFloppy(_filename);
}

bool CCPCDisc::NeedAdvancedMode() const
{
    return _needAdvancedMode;
}
bool CCPCDisc::IsInterlaced() const
{
    return _interlacedSectors;
}

void CCPCDisc::Format(bool interlaced)
{
    dsk_err_t e = DSK_ERR_OK;

    DSK_FORMAT *sectors = new DSK_FORMAT[_geometry.dg_sectors];
    int idxdec = ((_geometry.dg_sectors & 1) == 0) ? (_geometry.dg_sectors >> 1) : (_geometry.dg_sectors >> 1) + 1;
    for (unsigned int s=0 ; s<_geometry.dg_sectors ; s++)
    {
	sectors[s].fmt_secsize = _geometry.dg_secsize;

	int idx = interlaced ? (s >> 1) + (((s & 1) == 0) ? 0 : idxdec) : (s >> 1);
	sectors[s].fmt_sector = _geometry.dg_secbase + idx;
    }

    for (unsigned int cyl = 0; cyl < _geometry.dg_cylinders; cyl++)
    {
	for (unsigned int head = 0; head < _geometry.dg_heads; head++)
	{
	    for (unsigned int s=0 ; s<_geometry.dg_sectors ; s++)
	    {
		sectors[s].fmt_cylinder = cyl;
		sectors[s].fmt_head = head;
	    }
	    e = dsk_pformat(_driver, &_geometry, cyl, head, sectors, CCPCDisc::FormatByte);
	    TOOLS_ASSERTMSG( (e==DSK_ERR_OK) , "Error formatting dsk :" << string(dsk_strerror(e)));
	}
    }
    delete[] sectors;
}

void CCPCDisc::PrintInfo(ostream &str) const
{
    str << "DSK\t: " << _filename << endl;
    if (_geometry.dg_heads == 2)
    {
	str << "Double side" << endl;
    }
    str << "NbTracks\t: " << _geometry.dg_cylinders << endl;
    str << "Sector base \t: " << _geometry.dg_secbase << endl;
    str << "NbSectors \t:" << _geometry.dg_sectors << endl;
    str << "Sector size \t:" << _geometry.dg_secsize << endl;
}
