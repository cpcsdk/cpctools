/**
 * @class CAksFileTransfert
 * Class used to communicate with CPCBooster using Arkos File Transfert protocol
 * @author Thierry JOUIN
 * @version 1.1
 * @date 06/02/2006
 */

#pragma once

#define _CAKSFILETRANSFERT_H_

#include <fstream>
#include "CCPCBooster.h"
#include "CDSKFile.h"

enum CAksCommand
{
	AksUnknownCommand	= 0x00,		//!< Unknown command
	AksMinCommand		= 0xF0-1,	//!< Min command number
	AksTestComunication = 0xF0,		//!< #F0 Test Communication (send #80 to CPC to confirm).
	AksSend				= 0xF1,		//!< #F1 + Word = Asked to send 'word' bytes. 0=#10000
	AksEndTransfert		= 0xF2,		//!< #F2 = End Tranfer. Close all file in output and input.
	AksRewind			= 0xF3,		//!< #F3 = Rewind. Return to the beginning of the file.
	AksSendFileSize		= 0xF4,		//!< #F4 = Ask Filesize (send DWord to CPC).
	AksSendFilename		= 0xF5,		//!< #F5 = Ask Filename (send 12 bytes to CPC).
	AksCreateFile		= 0xF6,		//!< #F6 = Create file : Get 12 bytes (filename), Send byte (#80=ok autre=echec)
	AksAddData			= 0xF7,		//!< #F7 = Add data : Get 2 bytes (size of data chunck, little endian) + Data
	AksCloseFile		= 0xF8,		//!< #F8 = Close output file.
	AksInitDSK			= 0xF9,		//!< #F9 = Initialise the DSK. Empty it, set some vars. Header creation done when all the tracks are given.
									//   Ask for 2 chars for the Creator (10 for 1.0), Nb tracks, nb sides.
	AksWaitTrack		= 0xFA,		//!< #FA = Wait for a track from the CPC, header included. Format described in the function.
	AksNoMoreTrack		= 0xFB,		//!< #FB = Warn that no more track is to be transfered. The DSK can be completed and closed. This command closes the file.
	AksOpenFile			= 0xFC,		//!< #FC = Open a file in input. Get 12 bytes (filename). Send byte (#80=ok autre=echec)
	AksMaxCommand					//!< Min command number
};

enum CAksTransfertMode
{
	NoTransfert,
	GetFile,
	SendFile,
	GetDSK,
	SendDSK
};

enum CAksVerboseLevel
{
	NoDisplay,
	DisplayError,
	DisplayWarning,
	DisplayInfo,
	DisplayDebug,
	DisplayTransfertDebug
};

class CAksFileTransfert : public CCPCBooster
{
protected:
	//! Command short description
	static char* AksCommandShortDesc[];
	//! Command long description
	static char* AksCommandLongDesc[];

	//! Display message header
	static char* DisplayMessageHeader[];
protected:
	//! Current transfert mode
	CAksTransfertMode	_transfertMode;

	//! Number of global transfert done with CPC
	int					_nbTransfert;
	//! Indicate if we are transfering data
	bool				_transferingData;

	//! Current filename
	std::string			_filename;
	//! Current filename in Amstrad format
	unsigned char		_amsFilename[13];
	//! Current filename path for creating file
	std::string			_filepath;

	//! Current Extended DSK file (create)
	CExtendedDSKFile	*_EDSKFile;

	//! stream used for read
	std::ifstream		*_inStream;
	//! stream used for write
	std::ofstream		*_outStream;
public:
	//! Constructor, opening a COM port
	CAksFileTransfert(int comNumber = 1);
	//! Destructor
	~CAksFileTransfert();

	//! Force filename
	void SetForceFilename(const std::string &filename);
	//! Set filepath
	void SetFilepath(const std::string &filepath);
	//! Return number of transfert done
	int GetNbTransfertDone() const;
	//! Is transfering
	bool IsTransfering() const;

	//! Run one transfert loop
	CAksCommand Run(bool wait = true);

	//! Get filename to be used on PC side
	std::string GetFilename() const;
protected:
	//! Wait and return AFT command
	CAksCommand GetCommand(bool wait = true);

	//! Send acknowledgement
	bool Acknowledge();
	//! Send not acknownledgement
	bool NotAcknowledge();

	bool TestComunication(const CAksCommand &cmd);
	bool Send(const CAksCommand &cmd);
	bool EndTransfert(const CAksCommand &cmd);
	bool Rewind(const CAksCommand &cmd);
	bool SendFileSize(const CAksCommand &cmd);
	bool SendFilename(const CAksCommand &cmd);
	bool CreateFile(const CAksCommand &cmd);
	bool AddData(const CAksCommand &cmd);
	bool CloseFile(const CAksCommand &cmd);
	bool InitDSK(const CAksCommand &cmd);
	bool WaitTrack(const CAksCommand &cmd);
	bool NoMoreTrack(const CAksCommand &cmd);
	bool OpenFile(const CAksCommand &cmd);

	//! Display message
	void Display(CAksVerboseLevel level, const std::string &message);
	//! Display error
	void Error(const std::string &message);
	//! Display warning
	void Warning(const std::string &message);
	//! Display info
	void Info(const std::string &message);
	//! Display debug
	void Debug(const std::string &message);
};