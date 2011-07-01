/**
 * @class CCPCBooster
 * Class used to communicate with CPCBooster
 * @author Thierry JOUIN
 * @version 1.1
 * @date 06/02/2006
 */

#pragma once

#define _CCPCBOOSTER_H_

#if _WINDOWS
#include <windows.h>
#else
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>

#include <string>
#include <SerialPort.h>
#endif


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
#if _WINDOWS
    //! COM port handle
	HANDLE				_COMPortHandle;
	//! Current COM port number
	int					_COMPortNumber;

#else
	SerialStream				_COMPortHandle;
	//! Current COM port number
	std::string					_COMPortNumber;

#endif

	//! Current port state
	CCPCBoosterState	_currentState;
	//! Current error
	CCPCBoosterError	_currentError;

public:
	//! Constructor, opening a COM port
#if _WINDOWS
	CCPCBooster(int comNumber = 1);
#else 
	CCPCBooster(std::string comNumber = "/dev/ttyUSB0");
#endif
	//! Destructor, closing opended COM port
	virtual ~CCPCBooster();

	//! Check if current port is open
	bool IsOpen() const;

	//! Return current CPCBooster state
	CCPCBoosterState GetState() const;

	//! Read a byte from COM port, waiting
	bool ReadWaitByte(unsigned char &val);
	//! Read a byte from COM port, not waiting
	bool ReadByte(unsigned char &val);
	//! Write a byte to COM port
	bool WriteByte(const unsigned char val);
	//! Read a word from COM port, waiting
	bool ReadWaitWord(unsigned short &val);
	//! Read a word from COM port, not waiting
	bool ReadWord(unsigned short &val);
	//! Write a word to COM port
	bool WriteWord(const unsigned short val);
	//! Read N bytes from COM port, waiting
	bool ReadWaitBuffer(unsigned char *buffer, const DWORD nbBytes);
	//! Read N bytes from COM port, not waiting
	bool ReadBuffer(unsigned char *buffer, const DWORD nbBytes);
	//! Write N bytes to COM port
	bool WriteBuffer(unsigned char *buffer, const DWORD nbBytes);

protected:
	//! Open COM port
	void OpenPort();
	//! Close COM port
	void ClosePort();
};
