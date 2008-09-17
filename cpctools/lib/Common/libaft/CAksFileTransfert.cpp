/**
 * @class CAksFileTransfert
 * Class used to communicate with CPCBooster using Arkos File Transfert protocol
 * @author Thierry JOUIN
 * @version 1.1
 * @date 06/02/2006
 */

#include "CAksFileTransfert.h"

#include <iostream>
#include <cstring>

char* CAksFileTransfert::AksCommandShortDesc[] = 
{
	"TestComunication",
	"Send",
	"EndTransfert",
	"Rewind",
	"SendFileSize",
	"SendFilename",
	"CreateFile",
	"AddData",
	"CloseFile",
	"InitDSK",
	"WaitTrack",
	"NoMoreTrack",
	"OpenFile"
};

char* CAksFileTransfert::AksCommandLongDesc[] = 
{
	"Test Communication (send #80 to CPC to confirm).",
	"Asked to send 'word' bytes. 0=#10000",
	"End Tranfer. Close all file in output and input.",
	"Rewind. Return to the beginning of the file.",
	"Ask Filesize (send DWord to CPC).",
	"Ask Filename (send 12 bytes to CPC).",
	"Create file : Get 12 bytes (filename), Send byte (#80=ok autre=echec)",
	"Add data : Get 2 bytes (size of data chunck, little endian) + Data",
	"Close output file.",
	"Initialise the DSK. Empty it, set some vars. Header creation done when all the tracks are given. Ask for 2 chars for the Creator (10 for 1.0), Nb tracks, nb sides.",
	"Wait for a track from the CPC, header included. Format described in the function.",
	"Warn that no more track is to be transfered. The DSK can be completed and closed. This command closes the file.",
	"Open a file in input. Get 12 bytes (filename). Send byte (#80=ok autre=echec)"
};

char* CAksFileTransfert::DisplayMessageHeader[] =
{
	"",
	"Err: ",
	"Warn: ",
	"Info: ",
	"Debug: ",
	"Debug: "
};

CAksFileTransfert::CAksFileTransfert(int comNumber) :
CCPCBooster(comNumber),
_transfertMode(NoTransfert),
_nbTransfert(0),
_transferingData(false),
_EDSKFile(NULL),
_inStream(NULL),
_outStream(NULL)
{
	memset(_amsFilename, 0, 13);
}
CAksFileTransfert::~CAksFileTransfert()
{
	if (_EDSKFile != NULL)
		delete _EDSKFile;
	if (_inStream != NULL)
		delete _inStream;
	if (_outStream != NULL)
		delete _outStream;
}

void CAksFileTransfert::SetForceFilename(const std::string &filename)
{
	_filename = filename;
}

void CAksFileTransfert::SetFilepath(const std::string &filepath)
{
	_filepath = filepath;
}

std::string CAksFileTransfert::GetFilename() const
{
	if (_filename.size() != 0)
		return _filename;
	return (_filepath+std::string("\\")+std::string((char*)_amsFilename));
}

int CAksFileTransfert::GetNbTransfertDone() const
{
	return _nbTransfert;
}
bool CAksFileTransfert::IsTransfering() const
{
	return _transferingData;
}

CAksCommand CAksFileTransfert::Run(bool wait)
{
	CAksCommand cmd = GetCommand(wait);

	switch(cmd)
	{
	case AksTestComunication :	{ TestComunication(cmd);	break; }
	case AksEndTransfert :		{ EndTransfert(cmd);		break; }
	case AksSend:				{ Send(cmd);				break; }
	case AksSendFileSize:		{ SendFileSize(cmd);		break; }
	case AksSendFilename:		{ SendFilename(cmd);		break; }
	case AksRewind:				{ Rewind(cmd);				break; }
	case AksCreateFile:			{ CreateFile(cmd);			break; }
	case AksAddData:			{ AddData(cmd);				break; }
	case AksCloseFile:			{ CloseFile(cmd);			break; }
	case AksInitDSK:			{ InitDSK(cmd);				break; }
	case AksWaitTrack:			{ WaitTrack(cmd);			break; }
	case AksNoMoreTrack:		{ NoMoreTrack(cmd);			break; }
	case AksOpenFile:			{ OpenFile(cmd);			break; }
	}

	return cmd;
}

CAksCommand CAksFileTransfert::GetCommand(bool wait)
{
	unsigned char command;
	bool readed = false;
	if (wait)
	{
		readed = ReadWaitByte(command);
	}
	else
	{
		readed = ReadByte(command);
	}
	
	if (readed)
	{
		if (command > AksMinCommand && command < AksMaxCommand)
		{
			std::cout << "Command #" << std::hex << (int)command << std::dec << " : " << AksCommandShortDesc[command-(AksMinCommand+1)] << std::endl;

			return ((CAksCommand)command);
		}
		else
		{
			std::cout << "Unknwon command get" << std::endl;

			return AksUnknownCommand;
		}
	}
	else
	{
		std::cout << "Unknwon command not wait" << std::endl;

		return AksUnknownCommand;
	}
}

bool CAksFileTransfert::Acknowledge()
{
	unsigned char ack = 0x80;
	
	std::cout << "Acknowledge" << std::endl;

	return WriteByte(ack);
}

bool CAksFileTransfert::NotAcknowledge()
{
	unsigned char ack = 0x00;
	
	std::cout << "Not Acknowledge" << std::endl;

	return WriteByte(ack);
}

bool CAksFileTransfert::TestComunication(const CAksCommand &cmd)
{
	if (cmd != AksTestComunication)
		return false;

	Acknowledge();

	return true;
}
bool CAksFileTransfert::Send(const CAksCommand &cmd)
{
	if (cmd != AksSend)
		return false;
	
	_transferingData = true;

	// Read a word, send word bytes
	unsigned short s;
	ReadWaitWord(s);

	std::cout << "Read word " << std::hex << (int)s << std::dec << std::endl;

	int size = (s != 0) ? s : 0x10000;

	if (_inStream != NULL)
	{
		unsigned char *buffer = new unsigned char [size];
	
		_inStream->read((char*)buffer, size);

		WriteBuffer(buffer, size);
		
		delete[] buffer;

		return true;
	}
	
	return false;
}
bool CAksFileTransfert::EndTransfert(const CAksCommand &cmd)
{
	if (cmd != AksEndTransfert)
		return false;

	if (_EDSKFile != NULL)
		delete _EDSKFile;
	_EDSKFile = NULL;
	if (_inStream != NULL)
		delete _inStream;
	_inStream = NULL;
	if (_outStream != NULL)
		delete _outStream;
	_outStream = NULL;

	if (_transferingData)
	{
		_nbTransfert++;
		_transferingData = false;
	}

	return true;
}
bool CAksFileTransfert::Rewind(const CAksCommand &cmd)
{
	if (cmd != AksRewind)
		return false;

	if (_inStream != NULL)
	{
		_inStream->seekg(0,std::ios::beg);
	}

	return true;
}
bool CAksFileTransfert::SendFileSize(const CAksCommand &cmd)
{
	if (cmd != AksSendFileSize)
		return false;

	int fileSize = 0;
	
	if (_inStream != NULL)
	{
		std::ios::pos_type pos = _inStream->tellg();
		
		_inStream->seekg(0,std::ios::end);
		fileSize = _inStream->tellg();
		_inStream->seekg(pos);
	}

	unsigned char c;
	c = (fileSize >> 0) & 0xFF;
	WriteByte(c);
	c = (fileSize >> 8) & 0xFF;
	WriteByte(c);
	c = (fileSize >> 16) & 0xFF;
	WriteByte(c);
	c = (fileSize >> 24) & 0xFF;
	WriteByte(c);

	return true;
}
bool CAksFileTransfert::SendFilename(const CAksCommand &cmd)
{
	if (cmd != AksSendFilename)
		return false;

	WriteBuffer(_amsFilename, 12);

	return true;
}
bool CAksFileTransfert::CreateFile(const CAksCommand &cmd)
{
	if (cmd != AksCreateFile)
		return false;

	ReadBuffer(_amsFilename, 12);
	_amsFilename[12] = 0;

	std::cout << "Filename " << _amsFilename << std::endl;

	if (_outStream != NULL)
		delete _outStream;

	_outStream = new std::ofstream(GetFilename().c_str(), std::ios::binary);

	if (_outStream->good())
	{
		Acknowledge();
		return true;
	}
	else
	{
		NotAcknowledge();
		return false;
	}
}
bool CAksFileTransfert::AddData(const CAksCommand &cmd)
{
	if (cmd != AksAddData)
		return false;

	_transferingData = true;

	unsigned short size;

	ReadWord(size);
	
	std::cout << "Read word " << std::hex << (int)size << std::dec << std::endl;

	unsigned char *buffer = new unsigned char[size];

	ReadBuffer(buffer, size);

	if (_outStream != NULL)
	{
		_outStream->write((char*)buffer, size);
	}

	delete[] buffer;

	return true;
}
bool CAksFileTransfert::CloseFile(const CAksCommand &cmd)
{
	if (cmd != AksCloseFile)
		return false;

	if (_EDSKFile != NULL)
		delete _EDSKFile;
	_EDSKFile = NULL;
	if (_inStream != NULL)
		delete _inStream;
	_inStream = NULL;
	if (_outStream != NULL)
		delete _outStream;
	_outStream = NULL;

	if (_transferingData)
	{
		_nbTransfert ++;
		_transferingData = false;
	}

	return true;
}
bool CAksFileTransfert::InitDSK(const CAksCommand &cmd)
{
	if (cmd != AksInitDSK)
		return false;

	_transferingData = true;

	if (_EDSKFile != NULL)
		delete _EDSKFile;

	if (_outStream != NULL)
		delete _outStream;
	_outStream = NULL;

	unsigned char nbTrack;
	unsigned char nbSide;
	unsigned char creator[2];
	
	ReadByte(creator[0]);
	ReadByte(creator[1]);
	ReadByte(nbTrack);
	ReadByte(nbSide);

	_EDSKFile = new CExtendedDSKFile(nbTrack, nbSide);

	return true;
}
bool CAksFileTransfert::WaitTrack(const CAksCommand &cmd)
{
	if (cmd != AksWaitTrack)
		return false;

	_transferingData = true;

	unsigned char trackNum;
	unsigned char sideNum;

	ReadByte(trackNum);
	ReadByte(sideNum);

	CDSKTrack fakeTrack;
	CDSKTrack *track = NULL;
	
	if (_EDSKFile != NULL)
	{
		track = _EDSKFile->GetTrack(trackNum, sideNum);
	}

	if (track == NULL)
	{
		track = &fakeTrack;
		std::cout << "Fake track reading !" << std::endl;
	}

	std::cout << "Reading T" << (int)trackNum << " S" << (int)sideNum << std::endl;

	track->TrackNumber = trackNum;
	track->SideNumber = sideNum;

	ReadByte(track->SectorSize);
	ReadByte(track->NbSector);
	ReadByte(track->GAP3Size);
	ReadByte(track->FillByte);

	std::cout << "track " << (int)track->SectorSize << " " << (int)track->NbSector << " " << (int)track->GAP3Size << " " << (int)track->FillByte << std::endl;

	track->Allocate();

	for (int s = 0; s < track->NbSector ; s++)
	{
		CDSKSector &sect = track->Sectors[s];

		ReadByte(sect.Track);
		ReadByte(sect.Side);
		ReadByte(sect.SectorID);
		ReadByte(sect.SectorSize);
		ReadByte(sect.FDCStatus1);
		ReadByte(sect.FDCStatus2);
		ReadWord(sect.DataSize);

		sect.DisplayInfo(std::cout);

		sect.Allocate();

		ReadBuffer(sect.Data, sect.GetDataSize());

		sect.Update();
	}

	track->Update();

	track->DisplayInfo(std::cout);

	return true;
}
bool CAksFileTransfert::NoMoreTrack(const CAksCommand &cmd)
{
	if (cmd != AksNoMoreTrack)
		return false;

	// Release EDSK file
	if (_EDSKFile != NULL)
	{
		_EDSKFile->SaveDSKFile(GetFilename());

		delete _EDSKFile;
		_EDSKFile = NULL;
	}

	if (_transferingData)
	{
		_nbTransfert ++;
		_transferingData = false;
	}

	return true;
}
bool CAksFileTransfert::OpenFile(const CAksCommand &cmd)
{
	if (cmd != AksOpenFile)
		return false;

	ReadBuffer(_amsFilename, 12);
	_amsFilename[12] = 0;

	if (_inStream != NULL)
		delete _inStream;

	_inStream = new std::ifstream(GetFilename().c_str(), std::ios::binary);

	if (_inStream->good())
	{
		Acknowledge();
		
		return true;
	}
	else
	{
		NotAcknowledge();

		return false;
	}
}

void CAksFileTransfert::Display(CAksVerboseLevel level, const std::string &message)
{
	if (level <= DisplayTransfertDebug && level > NoDisplay)
	{
		std::cout << DisplayMessageHeader[level] << message << std::endl;
	}
}
void CAksFileTransfert::Error(const std::string &message)
{
	Display(DisplayError, message);
}
void CAksFileTransfert::Warning(const std::string &message)
{
	Display(DisplayWarning, message);
}
void CAksFileTransfert::Info(const std::string &message)
{
	Display(DisplayInfo, message);
}
void CAksFileTransfert::Debug(const std::string &message)
{
	Display(DisplayDebug, message);
}
