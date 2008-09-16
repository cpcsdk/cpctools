//
// Threads used for transfering to A:
//

#include "StdAfx.h"

#include "transfertThread.h"
#include "ProgressDialog.h"
#include "CatalogueDlg.h"
#include "fileTools.h"

#include "CCPCBinaryFile.h"
#include "CCPCAsciiFile.h"
#include "CError.h"

#include <algorithm>

using namespace std;

CCPCDisc* TransfertThread::OpenDSK(const string &dskFilename, int dskSide, bool create, CCPCDisc::TDisc dskFormat, CProgressDialog *progress)
{
	CCPCDisc *disc = NULL;
	try
	{
		if (progress != NULL)
		{
			string filename = dskFilename;
			if (filename.find_last_of('\\') != string::npos)
			{
				filename = filename.substr(filename.find_last_of('\\')+1, filename.size()-filename.find_last_of('\\')-1);
			}
			CString msg;
			msg.Format("Checking DSK %s",filename.c_str());
			progress->SetProgressText(msg);

			if (progress->HasAbort())
			{
				return NULL;
			}
		}
		if (CCPCDisc::IsDSK(dskFilename, dskSide))
		{
			if (progress != NULL)
			{
				string filename = dskFilename;
				if (filename.find_last_of('\\') != string::npos)
				{
					filename = filename.substr(filename.find_last_of('\\')+1, filename.size()-filename.find_last_of('\\')-1);
				}
				CString msg;
				msg.Format("Opening DSK %s",filename.c_str());
				progress->SetProgressText(msg);
				if (progress->HasAbort())
					return NULL;
			}
			disc = CCPCDisc::OpenDisc(dskFilename, dskSide);
			if (progress != NULL)
			{
				if (progress->HasAbort())
				{
					disc->Close();
					delete disc;
					return NULL;
				}
			}
		}
		else
		{
			TOOLS_ASSERT(!FileTool::IsFileExist(dskFilename) && create);

			if (progress != NULL)
			{
				string filename = dskFilename;
				if (filename.find_last_of('\\') != string::npos)
				{
					filename = filename.substr(filename.find_last_of('\\')+1, filename.size()-filename.find_last_of('\\')-1);
				}
				CString msg;
				msg.Format("Creating and formating DSK %s",filename.c_str());
				progress->SetProgressText(msg);
			}
			disc = CCPCDisc::CreateDisc(dskFilename, dskFormat, dskSide);
			disc->Format();
			disc->Close();
			delete disc;
			if (progress != NULL)
			{
				string filename = dskFilename;
				if (filename.find_last_of('\\') != string::npos)
				{
					filename = filename.substr(filename.find_last_of('\\')+1, filename.size()-filename.find_last_of('\\')-1);
				}
				CString msg;
				msg.Format("Opening DSK %s",filename.c_str());
				progress->SetProgressText(msg);
			}
			disc = CCPCDisc::OpenDisc(dskFilename, dskSide);
		}
	}
	catch (...)
	{
		if (disc != NULL)
			disc->Close();
		delete disc;
		disc = NULL;

		TOOLS_ASSERTMSG( disc != NULL, "Unable to open DSK " << dskFilename);
	}
	
	return disc;
}

void TransfertThread::CatalogueDSK(const string &dskFilename, int dskSide)
{
	CatalogueDSKThread *catalogue = new CatalogueDSKThread(dskFilename, dskSide);

	catalogue->Execute(IsFloppy(dskFilename));
}

void TransfertThread::ExtractFile(const string &dskFilename, int dskSide, const string &directoryPath)
{
	ExtractFileThread *extract = new ExtractFileThread(dskFilename, dskSide, directoryPath);

	extract->Execute(IsFloppy(dskFilename));
}

void TransfertThread::FileCopy(const string &dskFilename, int dskSide, const vector<string> &fileList)
{
	FileCopyThread *copy = new FileCopyThread(dskFilename, dskSide, fileList);

	copy->Execute(IsFloppy(dskFilename));
}

void TransfertThread::DSKCopy(const string &srcName, int srcSide, 
							 const string &dstName, int dstSide, 
							 int startTrack, int endTrack, int nbSide, 
							 bool extended, bool interlace)
{
	DSKCopyThread *copy = new DSKCopyThread(srcName, srcSide, 
											dstName, dstSide, 
											startTrack, endTrack, nbSide, extended, interlace);

	copy->Execute(true);
}

void TransfertThread::FormatFloppy(const string &dskFilename, int dskSide, int nbSide, CCPCDisc::TDisc dskFormat)
{
	FormatFloppyThread *format = new FormatFloppyThread(dskFilename, dskSide, nbSide, dskFormat);

	format->Execute(true);
}

UINT TransfertThread::ThreadFunction( LPVOID pParam )
{
	TransfertThread *threadCommand = static_cast<TransfertThread*>(pParam);
	try
	{
		if (threadCommand != NULL)
		{
			threadCommand->Run();

			delete threadCommand;
		}
	}	
	catch (tools::CException &e)
	{
		delete threadCommand;
		string errorMessage = e.what();
		MessageBox ( NULL, errorMessage.c_str(), "CPC File System error", MB_ICONWARNING );
	}
	catch (...)
	{
		delete threadCommand;
		MessageBox ( NULL, "Unknown error", "CPC File System error", MB_ICONWARNING );
	}

	AfxEndThread(0);
	return 0;
}

bool TransfertThread::IsFloppy(const string &dskFilename)
{
	string name = dskFilename;
	transform( name.begin(), name.end(), name.begin(), toupper );
	return (name.find("A:") == 0 ||
			name.find("B:") == 0);
}

TransfertThread::TransfertThread() :
_progress(NULL)
{
}
TransfertThread::~TransfertThread()
{
	CloseProgress();
}

void TransfertThread::Execute(bool createProgress)
{
	if (createProgress)
	{
		_progress = new CProgressDialogThread( RUNTIME_CLASS(CProgressDialog), IDD_PROGRESS);

		_progress->CreateThread();
	}
	else
	{
		_progress = NULL;
	}
	
	AfxBeginThread(TransfertThread::ThreadFunction, this);
}

void TransfertThread::CloseProgress()
{
	if (_progress != NULL)
	{
		_progress->PostThreadMessage(WM_QUIT, 0, 0);

		//delete _progress;
		_progress = NULL;
	}
}

// Catalogue thread
CatalogueDSKThread::CatalogueDSKThread(const string &dskFilename, int dskSide) :
_dskFilename(dskFilename),
_dskSide(dskSide)
{
}

CatalogueDSKThread::~CatalogueDSKThread()
{
}

void CatalogueDSKThread::Run()
{
	CProgressDialog *progress = NULL;
	if (_progress != NULL)
	{
		progress = _progress->GetProgressDialog();
	}

	CCPCDisc *disc = NULL;
	try
	{
		disc = OpenDSK(_dskFilename, _dskSide, false, CCPCDisc::Data42, progress);
		if (disc == NULL)
			return;

		CloseProgress();

		CCatalogueDlg catDlg;

		catDlg.SetDSK(disc);

		catDlg.DoModal();

		disc->Close();
		delete disc;
	}
	catch (tools::CException &e)
	{
		if (disc != NULL)
			disc->Close();
		delete disc;

		throw e;
	}
}

// DSK Copy thread object
DSKCopyThread::DSKCopyThread(	const string &srcName, int srcSide, 
					const string &dstName, int dstSide, 
					int startTrack, int endTrack, int nbSide, bool extended, bool interlace):
_srcName(srcName),_srcSide(srcSide),
_dstName(dstName),_dstSide(dstSide),
_startTrack(startTrack),_endTrack(endTrack),_nbSide(nbSide),
_extended(extended), _interlace(interlace)
{
}

DSKCopyThread::~DSKCopyThread()
{
}

void DSKCopyThread::Run()
{
	CProgressDialog *progress = NULL;
	if (_progress != NULL)
	{
		progress = _progress->GetProgressDialog();
	}
	
	CCPCDisc *src = NULL;
	CCPCDisc *dst = NULL;

	CCPCDisc::CTrack *tracks = NULL;
	try
	{
		src = CCPCDisc::OpenDisc(_srcName, _srcSide);
		dst = CCPCDisc::CreateDisc(_dstName, src->GetDiscFormat(), _dstSide);

		if (src == NULL || dst == NULL)
			goto AbortAndClose;			

		bool srcFloppy = src->IsFloppy();
		bool dstFloppy = dst->IsFloppy();

		string dskName;
		if (srcFloppy)
		{
			dskName = _dstName.substr(_dstName.find_last_of('\\')+1, _dstName.size() - _dstName.find_last_of('\\')-1);
		}
		else
		{
			dskName = _srcName.substr(_srcName.find_last_of('\\')+1, _srcName.size() - _srcName.find_last_of('\\')-1);
		}

		tracks = new CCPCDisc::CTrack[((_endTrack - _startTrack)+1) * _nbSide];

		int t,s, tIdx;

		tIdx = 0;
		for (t=_startTrack ; t<=_endTrack ; t++)
		{
			for (s=_srcSide ; s < (_srcSide + _nbSide) ; s++)
			{
				src->ReadTrack(t, s, tracks[tIdx], _extended);
				if (_interlace && !_extended)
				{
					tracks[tIdx].Interlace();
				}
				if (srcFloppy && progress != NULL)
				{
					CString title;
					title.Format("%d/%d Read track", t, _endTrack);
					progress->SetWindowText(title);

					CString msg;
					msg.Format("Read \"%s\" track %d Side %d", dskName.c_str(), t, s);
					progress->SetProgressText(msg);

					ostrstream longMsg;
					tracks[tIdx].DisplayInfo(longMsg);
					longMsg << ends;
					progress->SetLongProgressText(longMsg.str());

					if (progress->HasAbort())
						goto AbortAndClose;
				}
				tIdx ++;
			}
		}

		tIdx = 0;
		for (t=_startTrack ; t<=_endTrack ; t++)
		{
			for (s=_dstSide ; s < (_dstSide + _nbSide) ; s++)
			{
				if (dstFloppy && progress != NULL)
				{
					CString title;
					title.Format("%d/%d Write track", t, _endTrack);
					progress->SetWindowText(title);

					CString msg;
					msg.Format("Writing \"%s\" track %d Side %d", dskName.c_str(), t, s);
					progress->SetProgressText(msg);

					ostrstream longMsg;
					tracks[tIdx].DisplayInfo(longMsg);
					longMsg << ends;
					progress->SetLongProgressText(longMsg.str());

					if (progress->HasAbort())
						goto AbortAndClose;
				}
				dst->WriteTrack(t, s, tracks[tIdx], _extended);
				tIdx ++;
			}
		}
	}
	catch (tools::CException &e)
	{
		if (src != NULL)
		{
			src->Close();
		}
		delete src;
		if (dst != NULL)
		{
			dst->Close();
		}
		delete dst;

		delete[] tracks;

		throw e;
	}

AbortAndClose:
	if (src != NULL)
		src->Close();
	delete src;
	if (dst != NULL)
		dst->Close();
	delete dst;

	delete[] tracks;
}

// File copy thread
FileCopyThread::FileCopyThread( const string &dskFilename, int dskSide, const vector<string> &fileList ) :
_dskFilename(dskFilename),
_dskSide(dskSide),
_fileList(fileList)
{
}

FileCopyThread::~FileCopyThread()
{
}

void FileCopyThread::Run()
{
	unsigned int nbFiles = _fileList.size();

	if (nbFiles == 0)
		return;

	CProgressDialog *progress = NULL;
	if (_progress != NULL)
	{
		progress = _progress->GetProgressDialog();
		if( progress->HasAbort() )
			return;
	}

	string dskName = _dskFilename.substr(_dskFilename.find_last_of('\\')+1, _dskFilename.size() - _dskFilename.find_last_of('\\')-1);

	if (progress != NULL)
	{
		CString msg;
		msg.Format("Copy to %s", dskName.c_str());
		progress->SetWindowText(msg);
	}

	CCPCDisc *disc = OpenDSK(_dskFilename, _dskSide, true, CCPCDisc::Data42, progress);
	if (disc == NULL)
		return;

	string unableToPutFile;
	string errorStr;

	for (unsigned int i=0;i<nbFiles;i++)
	{
		try
		{
			auto_ptr<CCPCFile> fileIn;
			if (CCPCBinaryFile::isFileBinary(_fileList[i]))
			{
				fileIn = auto_ptr<CCPCFile>(new CCPCBinaryFile);
			}
			else
			{
				fileIn = auto_ptr<CCPCFile>(new CCPCAsciiFile);
			}
			fileIn->openFile(_fileList[i],false);

			if (_progress != NULL)
			{
				string filename = _fileList[i];
				if (filename.find_last_of('\\') != string::npos)
				{
					filename = filename.substr(filename.find_last_of('\\')+1, filename.size()-filename.find_last_of('\\')-1);
				}
				CString msg;
				msg.Format("Putting %s [%d kb]",filename.c_str(), (int)ceil(fileIn->getSize()/1024.0f));
				progress->SetProgressText(msg);

				ostrstream detail;
				CCPCBinaryFile *binFile = dynamic_cast<CCPCBinaryFile*>(fileIn.get());
				if (binFile != NULL)
				{
					detail << "Binary file\n";
					detail << "\tStart address\t: #" << std::hex << binFile->getAdress() << std::dec << "\n";
					detail << "\tExec address\t: #" << std::hex << binFile->getExecute() << std::dec << "\n";
					detail << "\tSize\t\t: #" << std::hex << binFile->getSize() << std::dec << "\n";
					detail << ends;
				}
				else
				{
					detail << "Ascii file\n";
					detail << "\tSize\t: #" << std::hex << fileIn->getSize() << std::dec << "\n";
					detail << ends;
				}
				progress->SetLongProgressText(detail.str());

				if (progress->HasAbort())
					goto AbortAndClose;
			}
			
			disc->PutFile(*fileIn,_fileList[i]);
		}
		catch (tools::CException &e )
		{
			string filename = _fileList[i].substr(_fileList[i].find_last_of("\\")+1, _fileList[i].size() - _fileList[i].find_last_of("\\") - 1);
			unableToPutFile += '\"'+filename+'\"';
			unableToPutFile += "\n";

			if (errorStr.empty())
			{
				errorStr = e.what();
			}
		}
		catch (...)
		{
			string filename = _fileList[i].substr(_fileList[i].find_last_of("\\")+1, _fileList[i].size() - _fileList[i].find_last_of("\\") - 1);
			unableToPutFile += '\"'+filename+'\"';
			unableToPutFile += "\n";

			if (errorStr.empty())
			{
				errorStr = "Unknown error";
			}
		}
	}

AbortAndClose:
	disc->Close();
	delete disc;

	if (unableToPutFile.size() != 0)
	{
		TOOLS_ERRORMSG("Unable to put files\n" << unableToPutFile << "\nError : " << errorStr);
	}
}

// Extract all file thread
ExtractFileThread::ExtractFileThread( const string &dskFilename, int dskSide, const string &directoryPath ) :
_dskFilename(dskFilename),
_dskSide(dskSide),
_directoryPath(directoryPath)
{
}

ExtractFileThread::~ExtractFileThread()
{
}
void ExtractFileThread::Run()
{
	if (!FileTool::IsDirectoryExist(_directoryPath))
	{
		if (CreateDirectory(_directoryPath.c_str(), NULL) == 0)
		{
			TOOLS_ERRORMSG("Unable to create directory\n" << _directoryPath);
		}
	}

	CProgressDialog *progress = NULL;
	if (_progress != NULL)
	{
		progress = _progress->GetProgressDialog();
		if( progress->HasAbort() )
			return;
	}

	CCPCDisc *disc = OpenDSK(_dskFilename, _dskSide, false, CCPCDisc::Data42, NULL);
	if (disc == NULL)
		return;
	
	string unableToGetFile;
	string errorStr;

	unsigned int nbFiles = disc->GetNbFiles();
	if (nbFiles != 0)
	{
		for (unsigned int i=0;i<nbFiles;i++)
		{
			string filename;
			try
			{
				int user;
				filename = disc->GetFilename(i, user);
			
				if (user == 229)
					continue;

				if (progress != NULL)
				{
					string fname = filename;
					if (fname.find_last_of('\\') != string::npos)
					{
						fname = fname.substr(fname.find_last_of('\\')+1, fname.size()-fname.find_last_of('\\')-1);
					}
					CString msg;
					msg.Format("Extracting %s",fname.c_str());
					progress->SetProgressText(msg);
					if (progress->HasAbort())
					{
						disc->Close();
						delete disc;
					}
				}	
				CCPCFile *file = disc->GetFile(filename, user);

				if (user != 0)
				{
					char userStr[16];
					sprintf(userStr, "%d_", user);
					filename = userStr + filename;
				}
				
				filename = _directoryPath + '\\' + filename;
				file->saveFile(filename,true);
				delete file;
			}
			catch (tools::CException &e)
			{
				unableToGetFile += '\"'+filename + string("\"\n");
				if (errorStr.empty())
				{
					errorStr = e.what();
				}
			}
			catch (...)
			{
				unableToGetFile += '\"'+filename+'\"';
				if (errorStr.empty())
				{
					errorStr = "Unknown error";
				}
			}
		}
	}
	
	disc->Close();
	delete disc;

	if (unableToGetFile.size() != 0)
	{
		TOOLS_ERRORMSG("Unable to get files\n" << unableToGetFile << "\nError : " << errorStr);
	}
}

// Format floppy thread
FormatFloppyThread::FormatFloppyThread( const string &dskFilename, int dskSide, int nbSide, CCPCDisc::TDisc format ) :
_dskFilename(dskFilename),
_dskSide(dskSide),
_dskNbSide(nbSide),
_dskFormat(format)
{
}

FormatFloppyThread::~FormatFloppyThread()
{
}
void FormatFloppyThread::Run()
{
	CProgressDialog *progress = NULL;
	if (_progress != NULL)
	{
		progress = _progress->GetProgressDialog();
		if( progress->HasAbort() )
			return;
	}

	CCPCDisc *disc = OpenDSK(_dskFilename, _dskSide, true, _dskFormat, progress);

	disc->Format();

	if (disc != NULL)
		disc->Close();
	delete disc;
}