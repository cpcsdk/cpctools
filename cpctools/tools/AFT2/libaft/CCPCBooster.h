/**
 * @class CCPCBooster
 * Class used to communicate with CPCBooster
 * @author Thierry JOUIN
 * @version 1.1
 * @date 06/02/2006
 */

#pragma once

#define _CCPCBOOSTER_H_

#ifdef _WIN32

#include <windows.h>

#else

#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>

#endif

#include <string>

#include "rs232.h"

#ifndef DWORD
#define DWORD long int
#endif

enum CCPCBoosterState
{
	PortOpened,
	PortFailed,
	PortClosed
};

enum CCPCBoosterError
{
	ErrOK,
	ErrInvalidHandle,
	ErrGetStateFailed,
	ErrSetStateFailed
};

class CCPCBooster
{
protected:
    //! COM port handle
	HANDLE				_COMPortHandle;
	//! Current COM port number
	std::string			_COMPortNumber;


	//! Current port state
	CCPCBoosterState	_currentState;
	//! Current error
	CCPCBoosterError	_currentError;

public:
	//! Constructor, opening a COM port
	CCPCBooster(std::string comport);
	
	//! Destructor, closing opended COM port
	virtual ~CCPCBooster();

	//! Check if current port is open
	bool IsOpen() const;

	//! Return current CPCBooster state
	CCPCBoosterState GetState() const;

	//! Read a byte from COM port, waiting
	void ReadWaitByte(unsigned char &val);
	//! Read a byte from COM port, not waiting
	int ReadByte(unsigned char &val);
	//! Write a byte to COM port
	bool WriteByte(const unsigned char val);
	//! Read a word from COM port, waiting
	void ReadWaitWord(unsigned short &val);
	//! Write a word to COM port
	void WriteWord(const unsigned short val);
	//! Read N bytes from COM port, waiting
	bool ReadWaitBuffer(unsigned char *buffer, const long nbBytes);
	//! Read N bytes from COM port, not waiting
	long ReadBuffer(unsigned char *buffer, const long nbBytes);
	//! Write N bytes to COM port
	bool WriteBuffer(unsigned char *buffer, const long nbBytes);

protected:
	//! Open COM port
	void OpenPort();
	//! Close COM port
	void ClosePort();
};
