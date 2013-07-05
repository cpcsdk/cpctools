/**
* @class CCPCDisc
* Class used for manipulating generic libdsk structure
* @author Thierry JOUIN
*/

#ifndef _CCPCDISC_HH_
#define _CCPCDISC_HH_

#include <string>
#include <iostream>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "libdsk.h"

#include <map>
#include <vector>

#include <math.h>

#include "CCPCFile.h"

using namespace std;

//! General CPC disc class manager
class CCPCDisc
{
public:
	class CDiscFormat;

public:
	//! Name of all cpc format (use to find geometry in libdsk)
	static const char* TDiscDesc[];
	//! Directory format of all 
	static const CDiscFormat TDiscFormat[];

	//! There is no block
	static const unsigned int NoBlock;
	//! Deleted user value
	static const unsigned int DeleteUser;
	//! Size of each entry in byte
	static const unsigned int EntrySize;
	//! Byte used for formatting
	static const unsigned char FormatByte;
	//! Max track size in byte
	static const unsigned int MaxTrackSize;
	//! Sector header size on disc (exclude gap #3e) (in byte)
	static const unsigned int SectorHeaderSize;
	//! Max number of sector per track
	static const unsigned int MaxSectorPerTrack;
	//! Maximum Gap #3E value
	static const unsigned int MaxGAPSize;
	//! Gap #3E array for each sector size (sector 0 to 5)
	static const unsigned int SectorGAPSizeArray[];

public:
	//! Enum type of 
	enum TDisc
	{
		Unknown = 0,
		Data,
		Data42,
		System,
		System42,
		RomdosD1,
		maxTDisc
	};

	//! Define a key in directory structure
	class CDiscFileEntryKey
	{
	public:
		//! File user
		unsigned char User;
		//! File name
		char Name[11];
		//! File write protected
		bool WriteProtected;
		//! System file
		bool System;
	public:
		//! Default constructor
		CDiscFileEntryKey();
		//! Constructor using a standard name (adjust to Name[11])
		CDiscFileEntryKey(const string &i_name);

		//! Comparaison operator (used for map)
		bool operator<(const CDiscFileEntryKey &other) const;

		//! Return formatted filename for file entry
		string getFilename() const;
	protected:
		//! Convert a string to standard CPC filename
		void convertStringToFilename(const string &i_name);
	};

	//! Define a entry in directory
	class CDiscFileEntry
	{
	public:
		//! Size of the file in kilobyte
		unsigned int Size;
		//! Number of record used by file
		unsigned int NbRecord;
		//! List of block index on disc
		vector<unsigned int> Blocks;

	public:
		//! Default constructor
		CDiscFileEntry();

		//! Clean blocks in File Entry
		void CleanBlocks();
	};


	//! Define generic format for file management
	class CDiscFormat
	{
	public:
		//! Block size in byte
		int BlockSize;
		//! Number max of entry in directory
		int NbMaxEntry;
		//! Number of bytes needed to identify a block (1 for DATA/SYSTEM, but 2 for Romdos!)
		int BlockIDSize;
		//! Number of block per entry in directory
		int NbBlocksPerEntry;
	public:
		//! Default constructor
		CDiscFormat();
		//! Constructor
		CDiscFormat(int blockSize, int maxEntry, int blockIDSize, int blocksPerEntry);
		//! Copy constructor
		CDiscFormat(const CDiscFormat &disc);

		//! Return directory size in byte
		int GetDirectorySize() const;
		//! Return record size in byte (used in directory)
		int GetRecordSize() const;
	};

	//! Define data of a whole track
	class CTrack
	{
	public:
		//! Track data
		unsigned char *Data;
		//! Geometry on track
		DSK_GEOMETRY Geometry;
		//! Track sector ID array
		DSK_FORMAT *SectorIDArray;
		//! FDC Status 1
		int *FDCStatus1Array;
		//! FDC Status 2
		int *FDCStatus2Array;
	public:
		//! Default constructor
		CTrack();
		//! Default constructor
		CTrack(const CTrack &i_track);
		//! Destructor
		~CTrack();

		//! Compute Gap #3e value on current tracks information
		void ComputeGapValue();
		//! Interlace sectors
		void Interlace();

		//! Display track info
		void DisplayInfo(ostream &str) const;
		
		//! Check if track is equal to another track
		bool IsEqual(const CTrack &track) const;
		//! Display difference with another track
		void DisplayDifference(const CTrack &track, ostream &str) const;
	};
	
	//! Define directory map format
	typedef map<CDiscFileEntryKey,CDiscFileEntry> CCPCDirectoryMap;

protected:
	//! Disc filename
	string					_filename;
	//! Driver (dsk, floppy, etc...)
	DSK_PDRIVER				_driver;
	//! Disc geometry
	DSK_GEOMETRY			_geometry;
	//! CPC DSK format
	TDisc					_format;
	//! General disc format information
	CDiscFormat				_discFormat;

	//! Array containing all Directory entry
	CCPCDirectoryMap		_directory;
	//! Directory buffer, contains current disc directory
	unsigned char			*_directoryBuffer;
	//! As directory changed ?
	bool					_isDirectoryChanged;
	
	//! Is DSK need extended mode for transfert ?
	bool					_needAdvancedMode;
	//! Are all sectors interlaced ?
	bool					_interlacedSectors;
protected:
	//! Constructor
	CCPCDisc();
	
	//! Return empty blocks
	virtual void getEmptyBlock(vector<unsigned int> &o_block) const = 0;
	
	//! Read directory buffer from disc
	virtual void readDirectoryBuffer();
	//! Write directory buffer to disc
	virtual void writeDirectoryBuffer();
	
	//! Read directory buffer and fill _directory map
	void readDirectory();
	//! Create directory buffer from _directory map and add to disc
	void writeDirectory();
	
	//! Insert a new entry in directory map
	void addDirectoryEntry(const CDiscFileEntryKey &i_name, const CDiscFileEntry &i_nameData);
	//! Remove an entry from directory map
	void removeDirectoryEntry(const CDiscFileEntryKey &i_name);
	//! Return number of entry used in directory, given a filesize in KB
	int getNbEntry(const int fileSize) const;
	
	//! Return number of entry in current directory
	int getNbUsedEntry() const;
	
	//! Return entry index containing given block ID
	int getDirectoryEntryForBlock(unsigned int i_block) const;

	//! Open existing dsk file (or disc)
	virtual void Open(const string &i_filename, DSK_PDRIVER driver, const DSK_GEOMETRY &geometry, const TDisc &format, bool needAdvancedMode, bool interlacedSector);
	//! Create a new dsk file (or disc)
	virtual void Create(TDisc i_type, const string &i_filename, int i_inside = 0);

public:
	//! Destructor
	virtual ~CCPCDisc();

	//! Close current dsk (or disc)
	virtual void Close();
	//! Return true if file is floppy
	bool IsFloppy() const;
	//! Return true if transfert need advanced mode
	bool NeedAdvancedMode() const;
	//! Return true if sector are all interlaced
	bool IsInterlaced() const;
	
	//! Format current disc
	virtual void Format(bool interlaced = true);
	
	// General reading / writing functions
	
	//! Read a sector from disc in an allocated buffer
	virtual void ReadSector(const unsigned char i_cylinder, const unsigned char i_head, const unsigned char i_sectorID, void* i_buffer, bool advanced = false) const;
	//! Write a sector on disc
	virtual void WriteSector(const unsigned char i_cylinder, const unsigned char i_head, const unsigned char i_sectorID, const void* i_buffer, bool advanced = false);
	//! Read a block from disc in an allocated buffer
	virtual void ReadBlock(const unsigned char i_idBlock, void* i_buffer) const = 0;
	//! Write a block on disc
	virtual void WriteBlock(const unsigned char i_idBlock, const void* i_buffer) = 0;
	//! Read a track from disc in an allocated buffer
	bool ReadTrack(const unsigned char i_cylinder, const unsigned char i_head, CTrack &i_track, bool extended = false) const;
	//! Write a track on disc
	bool WriteTrack(const unsigned char i_cylinder, const unsigned char i_head, const CTrack &i_track, bool extended = false);
	//! Repair a track comparing another given track
	bool RepairTrack(const unsigned char i_cylinder, const unsigned char i_head, CTrack &i_trackValid, CTrack &i_trackWrong, bool extended = false) const;

	//! Return disc capacity
	int GetDiscCapacity() const;
	//! Return disc geometry
	const DSK_GEOMETRY& GetDiscGeometry() const;
	//! Return format type
	CCPCDisc::TDisc GetDiscFormat() const;
	//! Return format name
	string GetDiscFormatName() const;
	//! Return format description
	string GetDiscFormatDescription() const;

	//! Return number of file on disc (exclude 229 user files)
	unsigned int GetNbFiles() const;
	//! Return filename of file given an entry index
	string GetFilename(const unsigned int i_id, int &user) const;
	//! Return full file entry given an entry index
	const CDiscFileEntryKey& GetFileEntry(const unsigned int i_id) const;
	//! Return number of block used by file
	int GetFileSize(const unsigned int i_id) const;
	
	// Functions for getting / putting files

	//! Get a file from disc and put it in a CPCFile structure
	CCPCFile* GetFile(const string &i_filename, int user = 0) const;
	//! Add a new file to the disc
	void PutFile(const CCPCFile &i_file, const string &i_filename, int user = 0, bool system = false, bool writeProtected = false);
	
	// Functions working on directory
	
	//! Return directory map containing directory info
	const CCPCDirectoryMap& GetDirectoryMap() const;

	//! Remove a file from disc
	void EraseFile(const string &i_filename, int user = 0);
	
	//! Rename file on disc
	void RenameFile(const string &i_oldFilename, const string &i_newFilename,
					int old_user = 0, int new_user = 0,
					bool old_sys = false, bool new_sys = false,
					bool old_pro = false, bool new_pro = false);
	
	//! Dump block list of a file
	void DumpBlockFile(const string &filename, int user, ostream &str) const;

	//! Print disc info
	void PrintInfo(ostream &str) const;

protected:
	//! Try to detect geometry of current driver & return format
	static TDisc guessGeometry(const string &i_filename, DSK_PDRIVER driver, DSK_GEOMETRY &geometry, bool &needAdvancedMode, bool &interlaced, bool extendedGuess = true);
	//! Try to detect floppy geometry
	static TDisc guessFloppyGeometry(DSK_PDRIVER self, DSK_GEOMETRY &geometry, bool &needAvancedMode, bool &interlaced, bool extendedGuess = true);
	//! Try to detect DSK file geometry
	static TDisc guessDSKGeometry(const string &i_filename, DSK_GEOMETRY &geometry, bool &needAvancedMode, bool &interlaced, bool extendedGuess = true);
	//! Try to detect RAW file/USB disk geometry
	static TDisc guessRAWGeometry(const string &i_filename, DSK_GEOMETRY &geometry, bool &needAvancedMode, bool &interlaced, bool extendedGuess = true);
	//! Guess data rate and FM mode
	static void guessDataRateRecordingMode(DSK_PDRIVER driver, DSK_GEOMETRY &geometry);

	//! Get back libdsk index of a given format name
	static dsk_format_t getLibDskFormat(const string &name);

	//! Check if name is floppy
	static int isFloppy(const string &filename);

public:
	//! Check if file is a DSK
	static bool IsDSK(const string &i_filename, int i_inside = 0);
	//! Open an existing DSK
	static CCPCDisc* OpenDisc(const string &i_filename, int i_inside = 0);
	//! Create and format a DSK file
	static CCPCDisc* CreateDisc(const string &i_filename, const TDisc &i_type, int i_inside = 0);
};

#endif
