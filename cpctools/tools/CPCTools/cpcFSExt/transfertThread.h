//
// Threads used for transfering to A:
//

#pragma once
#define _TRANSFERT_THREAD_H_

#include <string>
#include <vector>

using namespace std;

class CProgressDialog;
class CProgressDialogThread;

#include "CCPCDisc.h"

class TransfertThread
{
protected:
	CProgressDialogThread		*_progress;
protected:
	//! Create a new dsk
	static CCPCDisc* OpenDSK(const string &dskFilename, int dskSide, bool create=false, CCPCDisc::TDisc dskFormat = CCPCDisc::Data42, CProgressDialog *progress = NULL);

	//! Thread function
	static UINT ThreadFunction( LPVOID pParam );
public:

	//! Catalogue a DSK
	static void CatalogueDSK(const string &dskFilename, int dskSide);
	//! Extract all .dsk file to directory
	static void ExtractFile(const string &dskFilename, int dskSide, const string &directoryPath);
	//! Add all file to a .dsk
	static void FileCopy(const string &dskFilename, int dskSide, const vector<string> &fileList);
	//! Make copy of DSK file
	static void DSKCopy(const string &srcName, int srcSide, const string &dstName, int dstSide, int startTrack, int endTrack, int nbSide, bool extended, bool interlace);
	//! Format Floppy
	static void FormatFloppy(const string &dskFilename, int dskSide, int nbSide, CCPCDisc::TDisc dskFormat);

protected:
	TransfertThread();
	virtual ~TransfertThread();
	virtual void Run() = 0;

	static bool IsFloppy(const string &dskFilename);

public:
	void Execute(bool createProgress = false);

protected:
	void CloseProgress();
};

class CatalogueDSKThread : public TransfertThread
{
private:
	string		_dskFilename;
	int			_dskSide;
public:
	CatalogueDSKThread(const string &dskFilename, int dskSide);

protected:
	~CatalogueDSKThread();
	virtual void Run();
};

class DSKCopyThread : public TransfertThread
{
private:
	string			_srcName;
	int				_srcSide;
	string			_dstName;
	int				_dstSide;
	int				_startTrack;
	int				_endTrack;
	int				_nbSide;
	bool			_extended;
	bool			_interlace;
public:
	DSKCopyThread(	const string &srcName, int srcSide, 
					const string &dstName, int dstSide, 
					int startTrack, int endTrack, int nbSide, bool extended, bool interlace);
protected:
	virtual ~DSKCopyThread();
	virtual void Run();
};

class FileCopyThread : public TransfertThread
{
private:
	string			_dskFilename;
	int				_dskSide;
	vector<string>	_fileList;
public:
	FileCopyThread( const string &dskFilename, int dskSide, const vector<string> &fileList );

protected:
	virtual ~FileCopyThread();
	virtual void Run();
};

class ExtractFileThread : public TransfertThread
{
private:
	string			_dskFilename;
	int				_dskSide;
	string			_directoryPath;
public:
	ExtractFileThread( const string &dskFilename, int dskSide, const string &directoryPath );

protected:
	virtual ~ExtractFileThread();
	virtual void Run();
};

class FormatFloppyThread : public TransfertThread
{
private:
	string			_dskFilename;
	int				_dskSide;
	int				_dskNbSide;
	CCPCDisc::TDisc	_dskFormat;
public:
	FormatFloppyThread( const string &dskFilename, int dskSide, int nbSide, CCPCDisc::TDisc format );

protected:
	virtual ~FormatFloppyThread();
	virtual void Run();
};