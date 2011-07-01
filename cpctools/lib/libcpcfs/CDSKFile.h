/**
* @class CDSKFile
* Used for analyze DSK file
*/

#pragma warning( disable : 4786 )

#include <string>
#include <fstream>

#pragma once
#define _CDSKFILE_H_

//! DSK DiscHeader
class CDSKDiscHeader
{
public:
	//! Get format description
	virtual std::string GetDSKType() const = 0;
	//! Get Creator
	virtual std::string GetCreator() const = 0;
	//! Get Nb Track
	virtual int GetNbTrack() const = 0;
	//! Get Track size
	virtual int GetTrackSize(int track, int side = 0) const = 0;
	//! Get Nb Side
	virtual int GetNbSide() const = 0;

	//! Read header from stream
	virtual bool Read(std::istream &str) = 0;
	//! Write track to stream
	virtual bool Write(std::ostream &str) const = 0;

	//! Display standard DSK disc header information
	virtual void DisplayInfo(std::ostream &str) const = 0;
};

//! DSK Disc header
class CSDSKDiscHeader : public CDSKDiscHeader
{
public:
	unsigned char	Header[35];
	unsigned char	Creator[15];
	unsigned char	NbTrack;
	unsigned char	NbSide;
	unsigned short	TrackSize;
	unsigned char	Unused[204];
public:
	//! Default constructor
	CSDSKDiscHeader();

	//! Get format description
	virtual std::string GetDSKType() const;
	//! Get Creator
	virtual std::string GetCreator() const;
	//! Get Nb Track
	virtual int GetNbTrack() const;
	//! Get Track size
	virtual int GetTrackSize(int track, int side = 0) const;
	//! Get Nb Side
	virtual int GetNbSide() const;

	//! Read header from stream
	virtual bool Read(std::istream &str);
	//! Write track to stream
	virtual bool Write(std::ostream &str) const;

	//! Display standard DSK disc header information
	virtual void DisplayInfo(std::ostream &str) const;
};

//! Extended DSK Disc header
class CEDSKDiscHeader : public CDSKDiscHeader
{
public:
	unsigned char	Header[35];
	unsigned char	Creator[15];
	unsigned char	NbTrack;
	unsigned char	NbSide;
	unsigned short	Unused;
	unsigned char	TrackSizeTable[204];
public:
	//! Default constructor
	CEDSKDiscHeader();

	//! Get format description
	virtual std::string GetDSKType() const;
	//! Get Creator
	virtual std::string GetCreator() const;
	//! Get Nb Track
	virtual int GetNbTrack() const;
	//! Get Track size
	virtual int GetTrackSize(int track, int side = 0) const;
	//! Get Nb Side
	virtual int GetNbSide() const;

	//! Read header from stream
	virtual bool Read(std::istream &str);
	//! Write track to stream
	virtual bool Write(std::ostream &str) const;

	//! Display extended DSK disc header information
	virtual void DisplayInfo(std::ostream &str) const;
};

//! DSK sector data
class CDSKSector
{
public:
	unsigned char	Track;
	unsigned char	Side;
	unsigned char	SectorID;
	unsigned char	SectorSize;
	unsigned char	FDCStatus1;
	unsigned char	FDCStatus2;
	unsigned short	DataSize;

	unsigned char*	Data;
	unsigned char	Checksum;
	int				Index;
	int				CopyOf;
public:
	//! Default constructor
	CDSKSector();
	//! Destructor
	~CDSKSector();

	//! Read sector header from stream
	bool ReadHeader(std::istream &str);
	//! Write sector header to stream
	bool WriteHeader(std::ostream &str);
	//! Read sector data from stream
	bool ReadData(std::istream &str);
	//! Write sector data to stream
	bool WriteData(std::ostream &str);

	//! Write track to stream
	bool Write(std::ostream &str);

	//! Allocate data buffer
	void Allocate();
	//! Update data info on sector
	void Update();

	//! Return actual sector size
	int GetDataSize();

	//! Display DSK sector information
	void DisplayInfo(std::ostream &str) const;
};

//! DSK track data
class CDSKTrack
{
public:
	unsigned char	Header[14];
	unsigned char	Unused[3];
	unsigned char	TrackNumber;
	unsigned char	SideNumber;
	unsigned char	DataRate;
	unsigned char	RecordingMode;
	unsigned char	SectorSize;
	unsigned char	NbSector;
	unsigned char	GAP3Size;
	unsigned char	FillByte;

	bool			Formated;

	//! Actual sector on tracks
	CDSKSector*		Sectors;
public:
	//! Default constructor
	CDSKTrack();
	//! Destructor
	~CDSKTrack();

	//! Read track from stream
	bool Read(std::istream &str);
	//! Write track to stream
	bool Write(std::ostream &str);

	//! Allocate memory for sector
	void Allocate();
	//! Set 'CopyOf' flags
	void Update();

	//! Return track size in DSK
	int GetDSKTrackSize();

	//! Display DSK track information
	void DisplayInfo(std::ostream &str) const;

	//! Return true if 2 tracks have same header
	bool SameHeader(const CDSKTrack &track) const;
};

//! Generic DSK file
class CDSKFile
{
protected:
	//! Tracks data
	CDSKTrack*		_tracks;

public:
	//! Destructor
	virtual ~CDSKFile();

	//! Open DSK file depending on header
	static CDSKFile* OpenDSKFile(const std::string &filename);

	//! Save DSK file
	void SaveDSKFile(const std::string &filename);

	//! Update DSK structure
	virtual void Update() = 0;

	//! Return track, NULL if not found
	CDSKTrack* GetTrack(unsigned char track, unsigned char side);

	//! Return DSK header
	virtual const CDSKDiscHeader& GetHeader() const = 0;
protected:
	//! Open DSK file
	CDSKFile();
	//! Create DSK with nbTrack, nbSide
	CDSKFile(unsigned char nbTrack, unsigned char nbSide);

	//! Read dsk file from stream
	virtual bool readDSK(std::istream &str) = 0;
	//! Write DSK to stream
	virtual bool writeDSK(std::ostream &str) = 0;
};

//! Standard DSK File
class CStandardDSKFile : public CDSKFile
{
	friend class CDSKFile;

protected:
	//! Disc header
	CSDSKDiscHeader	_header;

public:
	//! Constructor using nbTrack and nbSide
	CStandardDSKFile(unsigned char nbTrack, unsigned char nbSide);

	//! Update DSK structure
	virtual void Update();

	//! Return DSK header
	virtual const CDSKDiscHeader& GetHeader() const;
protected:
	//! Default constructor
	CStandardDSKFile();

	//! Read dsk file from stream
	virtual bool readDSK(std::istream &str);
	//! Write DSK to stream
	virtual bool writeDSK(std::ostream &str);
};

//! Extended DSK File
class CExtendedDSKFile : public CDSKFile
{
	friend class CDSKFile;

protected:
	//! Disc header
	CEDSKDiscHeader		_header;

public:
	//! Constructor using nbTrack and nbSide
	CExtendedDSKFile(unsigned char nbTrack, unsigned char nbSide);

	//! Update DSK structure
	virtual void Update();

	//! Return DSK header
	virtual const CDSKDiscHeader& GetHeader() const;
protected:
	//! Default constructor
	CExtendedDSKFile();

	//! Read dsk file from stream
	virtual bool readDSK(std::istream &str);
	//! Write DSK to stream
	virtual bool writeDSK(std::ostream &str);
};
