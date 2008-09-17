/**
* @class CDSKFile
* Used for analyze DSK file
*/

#include "CDSKFile.h"
#include "CError.h"
#include <fstream>
#include <set>
#include <cstring>

using namespace std;

// Standard DSK disc header
CSDSKDiscHeader::CSDSKDiscHeader() :
NbTrack(0),
NbSide(0),
TrackSize(0)
{
	char h[] = "MV - CPCEMU Disk-File\r\nDisk-Info\r\n";
	char c[] = "CDSKFile";

	strcpy((char*)Header, h);
	strcpy((char*)Creator, c);
	memset(Unused, 0, 204);
}

std::string CSDSKDiscHeader::GetDSKType() const
{
	return std::string((char*)Header, 35);
}
std::string CSDSKDiscHeader::GetCreator() const
{
	return std::string((char*)Creator, 15);
}
int CSDSKDiscHeader::GetNbTrack() const
{
	return NbTrack;
}
int CSDSKDiscHeader::GetTrackSize(int track, int side) const
{
	return TrackSize;
}
int CSDSKDiscHeader::GetNbSide() const
{
	return NbSide;
}

bool CSDSKDiscHeader::Read(std::istream &str)
{
	str.read((char*)&Header, 34);
	str.read((char*)&Creator, 14);
	str.read((char*)&NbTrack, 1);
	str.read((char*)&NbSide, 1);
	str.read((char*)&TrackSize, 2);
	str.read((char*)&Unused, 204);
	return true;
}

bool CSDSKDiscHeader::Write(std::ostream &str) const
{
	str.write((char*)&Header, 34);
	str.write((char*)&Creator, 14);
	str.write((char*)&NbTrack, 1);
	str.write((char*)&NbSide, 1);
	str.write((char*)&TrackSize, 2);
	str.write((char*)&Unused, 204);
	return true;
}

void CSDSKDiscHeader::DisplayInfo(std::ostream &str) const
{
	str << Header;
	str << "\tCreator\t:" << Creator << std::endl;
	str << "\tNbTrack\t:" << (int)NbTrack << std::endl;
	str << "\tNbSide\t:" << (int)NbSide << std::endl;
	str << "\tTrackSize\t:" << (int)TrackSize << std::endl;
}

// Extended DSK disc header class
CEDSKDiscHeader::CEDSKDiscHeader() :
NbTrack(0),
NbSide(0),
Unused(0)
{
	char h[] = "EXTENDED CPC DSK File\r\nDisk-Info\r\n";
	char c[] = "CDSKFile";

	strcpy((char*)Header, h);
	strcpy((char*)Creator, c);
	memset(TrackSizeTable, 0, 204);
}

std::string CEDSKDiscHeader::GetDSKType() const
{
	return std::string((char*)Header, 35);
}
std::string CEDSKDiscHeader::GetCreator() const
{
	return std::string((char*)Creator, 15);
}
int CEDSKDiscHeader::GetNbTrack() const
{
	return NbTrack;
}
int CEDSKDiscHeader::GetTrackSize(int track, int side) const
{
	return TrackSizeTable[(track*NbSide) + side]*256;
}
int CEDSKDiscHeader::GetNbSide() const
{
	return NbSide;
}

bool CEDSKDiscHeader::Read(std::istream &str)
{
	str.read((char*)&Header, 34);
	str.read((char*)&Creator, 14);
	str.read((char*)&NbTrack, 1);
	str.read((char*)&NbSide, 1);
	str.read((char*)&Unused, 2);
	str.read((char*)&TrackSizeTable, 204);
	return true;
}

bool CEDSKDiscHeader::Write(std::ostream &str) const
{
	str.write((char*)&Header, 34);
	str.write((char*)&Creator, 14);
	str.write((char*)&NbTrack, 1);
	str.write((char*)&NbSide, 1);
	str.write((char*)&Unused, 2);
	str.write((char*)&TrackSizeTable, 204);
	return true;
}

void CEDSKDiscHeader::DisplayInfo(std::ostream &str) const
{
	str << Header;
	str << "\tCreator\t:" << Creator << std::endl;
	str << "\tNbTrack\t:" << (int)NbTrack << std::endl;
	str << "\tNbSide\t:" << (int)NbSide << std::endl;
}

// DSK sector data
CDSKSector::CDSKSector() :
Track(0),
Side(0),
SectorID(0),
SectorSize(0),
FDCStatus1(0),
FDCStatus2(0),
DataSize(0),
Data(NULL),
Checksum(0),
Index(0),
CopyOf(-1)
{
}

CDSKSector::~CDSKSector()
{
	if (Data != NULL)
		delete[] Data;
}

bool CDSKSector::ReadHeader(std::istream &str)
{
	str.read((char*)&Track, 1);
	str.read((char*)&Side, 1);
	str.read((char*)&SectorID, 1);
	str.read((char*)&SectorSize, 1);
	str.read((char*)&FDCStatus1, 1);
	str.read((char*)&FDCStatus2, 1);
	str.read((char*)&DataSize, 2);
	
	return true;
}

bool CDSKSector::WriteHeader(std::ostream &str)
{
	str.write((char*)&Track, 1);
	str.write((char*)&Side, 1);
	str.write((char*)&SectorID, 1);
	str.write((char*)&SectorSize, 1);
	str.write((char*)&FDCStatus1, 1);
	str.write((char*)&FDCStatus2, 1);
	str.write((char*)&DataSize, 2);
	
	return true;
}

bool CDSKSector::ReadData(std::istream &str)
{
	Allocate();
	
	int sSize = GetDataSize();
	str.read((char*)Data, sSize);

	Update();

	return true;
}

bool CDSKSector::WriteData(std::ostream &str)
{
	int sSize = GetDataSize();
	str.write((char*)Data, sSize);

	return true;
}

void CDSKSector::Allocate()
{
	if (Data != NULL)
		delete[] Data;
	Data = NULL;

	int sSize = GetDataSize();

	if (sSize != 0)
	{
		Data = new unsigned char[sSize];
	}
}

void CDSKSector::Update()
{
	int sSize = GetDataSize();
	
	Checksum = 0;
	for (int i=0;i<sSize;i++)
	{
		Checksum += Data[i];
	}
}

int CDSKSector::GetDataSize()
{
	int sSize = 0;
	if (DataSize != 0)
	{
		sSize = DataSize;
	}
	else
	{
		sSize = (128 << SectorSize);
	}
	return sSize;
}

void CDSKSector::DisplayInfo(std::ostream &str) const
{
	str << "\tSect ";
	str.setf(std::ios::dec, std::ios::basefield);
	str.fill(' ');
	str.width(3);
	str << Index << ":ID #";
	str.setf(std::ios::hex, std::ios::basefield);
	str.fill('0');
	str.width(2);
	str << (int)SectorID << ",Size #";
	str.setf(std::ios::hex, std::ios::basefield);
	str.fill('0');
	str.width(4);
	str << (int)(128 << SectorSize) << "(" << (int)SectorSize << "),Cyl ";
	str.setf(std::ios::dec, std::ios::basefield);
	str.fill(' ');
	str.width(3);
	str << (int)Track << ",Head ";
	str.fill(' ');
	str.width(3);
	str << (int)Side << ",Chk #";
	str.setf(std::ios::hex, std::ios::basefield);
	str.fill('0');
	str.width(2);
	str << (int)Checksum;

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

	str.setf(std::ios::dec, std::ios::basefield);

	if (DataSize != 0 && DataSize != (128 << SectorSize))
	{
		str << "Real size " << DataSize;
	}
	if (CopyOf != -1)
	{
		str << " (Copy of sector Num " << CopyOf << ")";
	}

	str << std::endl;
}

// DSK track data
CDSKTrack::CDSKTrack() :
TrackNumber(0),
SideNumber(0),
DataRate(0),
RecordingMode(0),
SectorSize(0),
NbSector(0),
GAP3Size(0),
FillByte(0),
Formated(false),
Sectors(NULL)
{
	char h[] = "Track-Info\r\n";

	strcpy((char*)Header, h);
	memset(Unused, 0, 3);
}

CDSKTrack::~CDSKTrack()
{
	if (Sectors != NULL)
		delete[] Sectors;
}

bool CDSKTrack::Read(std::istream &str)
{
	unsigned int trackPos = str.tellg();
	
	str.read((char*)&Header, 13);
	str.read((char*)&Unused, 3);
	str.read((char*)&TrackNumber, 1);
	str.read((char*)&SideNumber, 1);
	str.read((char*)&DataRate, 1);
	str.read((char*)&RecordingMode, 1);
	str.read((char*)&SectorSize, 1);
	str.read((char*)&NbSector, 1);
	str.read((char*)&GAP3Size, 1);
	str.read((char*)&FillByte, 1);

	Allocate();

	int s;
	for (s=0 ; s<NbSector ; s++)
	{
		Sectors[s].ReadHeader(str);
	}

	str.seekg(trackPos + 0x100);
	for (s=0 ; s<NbSector ; s++)
	{
		Sectors[s].ReadData(str);
	}

	Update();

	return true;
}

bool CDSKTrack::Write(std::ostream &str)
{
	unsigned int trackPos = str.tellp();
	
	str.write((char*)&Header, 13);
	str.write((char*)&Unused, 3);
	str.write((char*)&TrackNumber, 1);
	str.write((char*)&SideNumber, 1);
	str.write((char*)&DataRate, 1);
	str.write((char*)&RecordingMode, 1);
	str.write((char*)&SectorSize, 1);
	str.write((char*)&NbSector, 1);
	str.write((char*)&GAP3Size, 1);
	str.write((char*)&FillByte, 1);

	int s;
	for (s=0 ; s<NbSector ; s++)
	{
		Sectors[s].WriteHeader(str);
	}

	int nbBlank = (trackPos+0x100) - str.tellp();
	if (nbBlank != 0)
	{
		unsigned char *buff = new unsigned char[nbBlank];
		memset(buff, 0, nbBlank);
		str.write((char*)buff, nbBlank);
		delete[] buff;
	}

	for (s=0 ; s<NbSector ; s++)
	{
		Sectors[s].WriteData(str);
	}

	return true;
}

void CDSKTrack::Allocate()
{
	if (Sectors != NULL)
		delete[] Sectors;
	Sectors = NULL;
	
	if (NbSector == 0)
		return;

	Sectors = new CDSKSector[NbSector];
}

void CDSKTrack::Update()
{
	for (int s=0 ; s<NbSector ; s++)
	{
		Sectors[s].Index = s;
	}

	for (int s1=0 ; s1<NbSector-1 ; s1++)
	{
		for (int s2=s1+1 ; s2 < NbSector ; s2++)
		{
			if (Sectors[s1].Checksum == Sectors[s2].Checksum && 
				Sectors[s1].SectorID == Sectors[s2].SectorID && 
				Sectors[s1].GetDataSize() == Sectors[s2].GetDataSize())
			{
				if (memcmp(Sectors[s1].Data, Sectors[s2].Data, Sectors[s1].GetDataSize()) == 0)
				{
					Sectors[s2].CopyOf = s1;
				}
			}
		}
	}

	Formated = true;
}

int CDSKTrack::GetDSKTrackSize()
{
	if (!Formated)
		return 0;

	int trackSize = 0x100;
	for (int s=0 ; s<NbSector ; s++)
	{
		trackSize += Sectors[s].GetDataSize();
	}

	return trackSize;
}

void CDSKTrack::DisplayInfo(std::ostream &str) const
{
	if (Formated)
	{
		str << "NbSector\t" << (int)NbSector << std::endl;
		str << "SectorSize\t" << (int)(128 << SectorSize) << " (" << (int)SectorSize << ")" << std::endl;
		str << "DataRate\t";
		switch (DataRate)
		{
		case 1:{str << "Single or double density" << std::endl;break;};
		case 2:{str << "High density" << std::endl;break;};
		case 3:{str << "Extended density" << std::endl;break;};
		default:
		case 0:{str << "Unknown" << std::endl;break;};
		}
		str << "Recording\t";
		switch (RecordingMode)
		{
		case 1:{str << "FM" << std::endl;break;};
		case 2:{str << "MFM" << std::endl;break;};
		default:
		case 0:{str << "Unknown" << std::endl;break;};
		}
		str << "Gap #3\t\t#" << std::hex << (int)GAP3Size << std::dec << " (" << (int)GAP3Size << ")" << std::endl;
		str << "Fill Byte\t#" << std::hex << (int)FillByte << std::dec << std::endl;
		
		for (int s=0 ; s<NbSector ; s++)
		{
			Sectors[s].DisplayInfo(str);
		}
	}
	else
	{
		str << "Not formated" << std::endl;
	}
}

bool CDSKTrack::SameHeader(const CDSKTrack &track) const
{
	bool equal = true;
	equal = equal && (NbSector == track.NbSector);
	equal = equal && (SectorSize == track.SectorSize);
	equal = equal && (DataRate == track.DataRate);
	equal = equal && (RecordingMode == track.RecordingMode);
	equal = equal && (GAP3Size == track.GAP3Size);
	equal = equal && (FillByte == track.FillByte);

	if (!equal || NbSector == 0)
		return equal;

	unsigned char size = Sectors[0].SectorSize;
	set<unsigned char> sectorID;
	set<unsigned char> trackSectorID;
	for (int s=0 ; s<NbSector ; s++)
	{
		if (Sectors[s].SectorSize != size ||
			track.Sectors[s].SectorSize != size)
			return false;

		sectorID.insert(Sectors[s].SectorID);
		trackSectorID.insert(track.Sectors[s].SectorID);
	}
	if (sectorID.size() != trackSectorID.size())
		return false;

	set<unsigned char>::iterator sit = sectorID.begin();
	set<unsigned char>::iterator dit = trackSectorID.begin();
	for ( ; sit != sectorID.end() ; sit++, dit++)
	{
		if ((*sit) != (*dit))
			return false;
	}
	return true;
}

CDSKFile* CDSKFile::OpenDSKFile(const std::string &filename)
{
	TOOLS_ASSERTMSG(	filename != "a:" && filename != "A:" && 
						filename != "b:" && filename != "B:", "Unable to dump real floppy");

	std::ifstream file(filename.c_str(), std::ios::binary);

	CDSKFile *dsk = NULL;

	char header[8];
	file.read(header, 8);
	file.seekg(0, std::ios::beg);

	if (strncmp("MV - CPC", header, 8) == 0)
	{
		dsk = new CStandardDSKFile();
		dsk->readDSK(file);
		file.close();
	}
	else
	{
		if (strncmp("EXTENDED", header, 8) == 0)
		{
			dsk = new CExtendedDSKFile();
			dsk->readDSK(file);
			file.close();
		}
	}
	return dsk;
}

CDSKFile::CDSKFile() : 
_tracks(NULL)
{
}
CDSKFile::CDSKFile(unsigned char nbTrack, unsigned char nbSide) :
_tracks(new CDSKTrack[nbTrack * nbSide])
{
}

CDSKFile::~CDSKFile()
{
	if (_tracks != NULL)
		delete[] _tracks;
}

void CDSKFile::SaveDSKFile(const std::string &filename)
{
	std::ofstream outFile(filename.c_str(), std::ios::binary);

	writeDSK(outFile);
}

CDSKTrack* CDSKFile::GetTrack(unsigned char track, unsigned char side)
{
	if (track < GetHeader().GetNbTrack() && side < GetHeader().GetNbSide() && _tracks != NULL)
	{
		return &_tracks[track * GetHeader().GetNbSide() + side];
	}
	return NULL;
}

// Standard DSK Class
CStandardDSKFile::CStandardDSKFile()
{
}
CStandardDSKFile::CStandardDSKFile(unsigned char nbTrack, unsigned char nbSide)
{
	_header.NbTrack = nbTrack;
	_header.NbSide = nbSide;

	_tracks = new CDSKTrack[_header.NbTrack * _header.NbSide];
}

bool CStandardDSKFile::readDSK(std::istream &str)
{
	_header.Read(str);

	if (_header.NbTrack == 0 && _header.NbSide == 0)
		return false;

	_tracks = new CDSKTrack[_header.NbTrack * _header.NbSide];

	unsigned int trackNum = 0;
	unsigned int trackOff = 0x100;
	for (int t=0 ; t<_header.NbTrack ; t++)
	{
		for (int s=0 ; s<_header.NbSide ; s++)
		{
			str.seekg(trackOff, std::ios::beg);

			_tracks[trackNum].Read(str);

			trackOff += _header.TrackSize;
			trackNum++;
		}
	}

	return true;
}

bool CStandardDSKFile::writeDSK(std::ostream &str)
{
	_header.Write(str);

	unsigned int trackNum = 0;
	unsigned int trackOff = 0x100;
	for (int t=0 ; t<_header.NbTrack ; t++)
	{
		for (int s=0 ; s<_header.NbSide ; s++)
		{
			int nbBlank = trackOff - str.tellp();
			if (nbBlank != 0)
			{
				unsigned char *buff = new unsigned char[nbBlank];
				memset(buff, 0, nbBlank);
				str.write((char*)buff, nbBlank);
				delete[] buff;
			}
			_tracks[trackNum].Write(str);

			trackOff += _header.TrackSize;
			trackNum++;
		}
	}

	return true;
}

void CStandardDSKFile::Update()
{
}

const CDSKDiscHeader& CStandardDSKFile::GetHeader() const
{
	return _header;
}

// Extended DSK class
CExtendedDSKFile::CExtendedDSKFile()
{
}
CExtendedDSKFile::CExtendedDSKFile(unsigned char nbTrack, unsigned char nbSide)
{
	_header.NbTrack = nbTrack;
	_header.NbSide = nbSide;

	_tracks = new CDSKTrack[_header.NbTrack * _header.NbSide];
}

bool CExtendedDSKFile::readDSK(std::istream &str)
{
	_header.Read(str);

	if (_header.NbTrack == 0 && _header.NbSide == 0)
		return false;

	_tracks = new CDSKTrack[_header.NbTrack * _header.NbSide];

	unsigned int trackOff = 0x100;
	for (int t=0 ; t<_header.NbTrack ; t++)
	{
		for (int s=0 ; s<_header.NbSide ; s++)
		{
			if (_header.TrackSizeTable[(t*_header.NbSide) + s] != 0)
			{
				str.seekg(trackOff, std::ios::beg);

				_tracks[(t*_header.NbSide) + s].Read(str);

				trackOff += _header.TrackSizeTable[(t*_header.NbSide) + s] << 8;
			}
		}
	}

	return true;
}

bool CExtendedDSKFile::writeDSK(std::ostream &str)
{
	Update();
	
	_header.Write(str);

	int trackOff = str.tellp();
	for (int t=0 ; t<_header.NbTrack ; t++)
	{
		for (int s=0 ; s<_header.NbSide ; s++)
		{
			if (_header.TrackSizeTable[(t*_header.NbSide) + s] != 0)
			{
				int nbBlank = trackOff - str.tellp();
				if (nbBlank != 0)
				{
					unsigned char *buff = new unsigned char[nbBlank];
					memset(buff, 0, nbBlank);
					str.write((char*)buff, nbBlank);
					delete[] buff;
				}

				_tracks[(t*_header.NbSide) + s].Write(str);

				trackOff += (_header.TrackSizeTable[(t*_header.NbSide) + s]) << 8;
			}
		}
	}

	return true;
}

void CExtendedDSKFile::Update()
{
	for (int t=0 ; t<_header.NbTrack ; t++)
	{
		for (int s=0 ; s<_header.NbSide ; s++)
		{
			int trackSize = _tracks[(t*_header.NbSide) + s].GetDSKTrackSize();
			_header.TrackSizeTable[(t*_header.NbSide) + s] = trackSize >> 8;
		}
	}
}

const CDSKDiscHeader& CExtendedDSKFile::GetHeader() const
{
	return _header;
}
