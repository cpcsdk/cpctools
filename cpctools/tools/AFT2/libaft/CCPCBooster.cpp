/**
 * @class CCPCBooster
 * Class used to communicate with CPCBooster
 * @author Thierry JOUIN
 * @author Adrien DESTUGUES
 * @version 2.0
 * @date 2011-07-03
 */

#include "CCPCBooster.h"
#include <stdio.h>

#include <iostream>

/**
 * @todo use a string to designate the COMPort instead of an int, as it makes more sense.
 */
 
 
// Create a communication with the CPC Booster on the given port number.
// The port number is mapped as follow :
// Windows : COM1 = 0 ; COM2 = 1, and so on, up to COM 16

// Linux : 0 = /dev/ttyS0 .. 15 = /dev/ttyS15
//        16 = /dev/ttyUSB0 .. 22 = /dev/ttyUSB7

// Haiku : 0 = /dev/ports/usb0 .. 3 = /dev/ports/usb3
CCPCBooster::CCPCBooster(std::string comNumber):
_COMPortNumber(comNumber),
_currentState(PortFailed),
_currentError(ErrOK)
{
	OpenPort();
}


// Close and destroy a communication when we're done
CCPCBooster::~CCPCBooster()
{
	ClosePort();
}


// Return true if communication is opened (and false if there was an error)
bool CCPCBooster::IsOpen() const
{
	return (_currentState == PortOpened);
}


// Get status flag
CCPCBoosterState CCPCBooster::GetState() const
{
	return _currentState;
}


// Open the communication (done when you create it)
void CCPCBooster::OpenPort()
{
	if (_currentState == PortOpened)
	{
		ClosePort();
	}

	_currentState = PortFailed;

/**
 * code specifique aux windows
 */
#if _WINDOWS

    _COMPortHandle = CreateFile(_COMPortNumber.c_str(),
						GENERIC_READ | GENERIC_WRITE,
						0,								// must be opened with exclusive-access
						NULL,							// no security attributes
						OPEN_EXISTING,					// must use OPEN_EXISTING
						0,								// not overlapped I/O
						NULL);							// hTemplate must be NULL for comm devices
	
	if (_COMPortHandle == INVALID_HANDLE_VALUE) 
	{
		_currentError = ErrInvalidHandle;
	}
	else
	{	
		// Build on the current configuration, and skip setting the size
		// of the input and output buffers with SetupComm.
		DCB dcb;
		BOOL fSuccess = GetCommState(_COMPortHandle, &dcb);
		
		if (fSuccess != TRUE) 
		{
			_currentError = ErrGetStateFailed;
		}
		else
		{
		// configure COM properties
			// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/wcecoreos5/html/wce50conprogrammingserialconnections.asp
			dcb.BaudRate = CBR_115200;				// set the baud rate
			dcb.ByteSize = 8;						// data size, xmit, and rcv
			dcb.Parity = NOPARITY;					// no parity bit
			dcb.StopBits = ONESTOPBIT;				// one stop bit
			// additionnal properties to fix some USB crappy serial interface
			dcb.fBinary = TRUE;						// Binary mode; no EOF check
			dcb.fOutxCtsFlow = FALSE;				// No CTS output flow control
			dcb.fOutxDsrFlow = FALSE;				// No DSR output flow control 
			dcb.fDtrControl = DTR_CONTROL_DISABLE;
			dcb.fDsrSensitivity = FALSE;			// DSR sensitivity
			dcb.fTXContinueOnXoff = FALSE;			// XOFF continues Tx
			dcb.fOutX = FALSE;						// No XON/XOFF out flow control
			dcb.fInX = FALSE;						// No XON/XOFF in flow control 
			dcb.fNull = FALSE;						// Disable null stripping 
			dcb.fRtsControl = RTS_CONTROL_DISABLE;
			dcb.fAbortOnError = FALSE;				// Do not abort reads/writes on error

			fSuccess = SetCommState(_COMPortHandle, &dcb);
			
			if (fSuccess != TRUE) 
			{
				_currentError = ErrSetStateFailed;
			}
			else
			{
				_currentState = PortOpened;
			}
			
			COMMTIMEOUTS timeOut;
			GetCommTimeouts(_COMPortHandle, &timeOut);

			timeOut.ReadTotalTimeoutMultiplier = 10;

			SetCommTimeouts(_COMPortHandle, &timeOut);


			}
		}

/**
 * Code specifique aux unices
 */
#else
	if (OpenComport(_COMPortNumber.c_str(),115200) == 0)
	{
	    _currentState = PortOpened ;
	    _COMPortHandle = 0;
	}
#endif
	
}


// Close communication (you can reopen it later)
void CCPCBooster::ClosePort()
{
	if (_currentState == PortOpened)
	{
#if _WINDOWS
		CloseHandle(_COMPortHandle);
#else
		CloseComport(_COMPortHandle);
#endif
	}
	_currentState = PortClosed;
}


// Read a byte. If the buffer is empty, wait for a byte to come
bool CCPCBooster::ReadWaitByte(unsigned char &val)
{
#if _WINDOWS
	unsigned long nbBytesReceived = 0;
	BOOL fSuccess = TRUE;
	while (	nbBytesReceived != 1 && fSuccess == TRUE) 
	{
		fSuccess = ReadFile (_COMPortHandle, &val, 1, &nbBytesReceived, NULL);
	}

	return ((nbBytesReceived == 1) && fSuccess);
#else
	int ret;
	do {
		ret = PollComport(_COMPortHandle, &val, 1);
	} while (ret  != 1);
		
	return true ; 
#endif
}


// Non-blocking read of a byte. If the buffer is empty, retunrs false.
bool CCPCBooster::ReadByte(unsigned char &val)
{
#if _WINDOWS
	unsigned long nbBytesReceived = 0;
	
	BOOL fSuccess = ReadFile (_COMPortHandle, &val, 1, &nbBytesReceived, NULL);

	return nbBytesReceived == 1 ;
#else
	return PollComport(_COMPortHandle, &val, 1) == 1;
#endif

}


// Write a byte
bool CCPCBooster::WriteByte(const unsigned char val)
{
#if _WINDOWS
	unsigned long nbBytesSend = 0;
    
	BOOL fSuccess = WriteFile(_COMPortHandle, &val, 1, &nbBytesSend, NULL);
	
	return ((nbBytesSend == 1) && fSuccess);
#else
  return SendByte(_COMPortHandle, val) == 0;
#endif
}


// Read a word in little-endian format.
bool CCPCBooster::ReadWaitWord(unsigned short &val)
{
#if _WINDOWS
	unsigned long nbBytesReceived = 0;
	BOOL fSuccess = TRUE;
	while (	nbBytesReceived != 2 && fSuccess == TRUE) 
	{
		fSuccess = ReadFile (_COMPortHandle, &val, 2, &nbBytesReceived, NULL);
	}

	return ((nbBytesReceived == 2) && fSuccess);
#else
	unsigned char byte1, byte2 ;

	if (
		ReadWaitByte(byte1) &&
		ReadWaitByte(byte2) 
	    )
	{
	    val = byte2 * 256 + byte1 ;
	    return 1 == 1 ;
	}
	else{
	    return 1 == 0 ;
	}

#endif
}


// read a word in little endian format, non blocking
bool CCPCBooster::ReadWord(unsigned short &val)
{
#if _WINDOWS
	unsigned long nbBytesReceived = 0;
	
	BOOL fSuccess = ReadFile (_COMPortHandle, &val, 2, &nbBytesReceived, NULL);

	return ((nbBytesReceived == 2) && fSuccess);
#else
	unsigned char byte1, byte2 ;

	ReadByte(byte1);
	ReadByte(byte2);

	val = byte2 * 256 + byte1 ;
	return 1 == 1 ;


#endif
}


// Write a word in little endian format
bool CCPCBooster::WriteWord(const unsigned short val)
{
#if _WINDOWS
	unsigned long nbBytesSend = 0;
    
	BOOL fSuccess = WriteFile(_COMPortHandle, &val, 2, &nbBytesSend, NULL);
	
	return ((nbBytesSend == 2) && fSuccess);
#else

	unsigned char byte1, byte2 ;

	byte1 = val / 256 ;
	byte2 = val % 256 ;

	WriteByte(byte1);
	WriteByte(byte2) ;

	return true ;
#endif
}


// Read multiple bytes at once, waiting if there isn't enough
bool CCPCBooster::ReadWaitBuffer(unsigned char *buffer, const  long nbBytes)
{
#if _WINDOWS
	unsigned long nbBytesReceived = 0;
	BOOL fSuccess = TRUE;
	while (	nbBytesReceived != nbBytes && 
			nbBytesReceived != 0 &&
			fSuccess == TRUE) 
	{
		fSuccess = ReadFile (_COMPortHandle, buffer, nbBytes, &nbBytesReceived, NULL);
	}

	return ((nbBytesReceived == nbBytes) && fSuccess);
#else
	/*
	for (long i=0 ; i< nbBytes ; i++)
	    buffer[i] =_COMPortHandle.ReadByte( );
	    */

	long bytesLeft = nbBytes;
	while(bytesLeft != 0)
	{
		int i = PollComport(_COMPortHandle, buffer, bytesLeft) ; 
		if (i > 0) {
			bytesLeft -= i;
			buffer += i;
		}
	}

	return true ;
#endif
}


// read multiple bytes, non-blocking.
// TODO : there is no way to know how many bytes were actually read !
bool CCPCBooster::ReadBuffer(unsigned char *buffer, const long nbBytes)
{
#if _WINDOWS
	unsigned long nbBytesReceived = 0;
	
	BOOL fSuccess = ReadFile (_COMPortHandle, buffer, nbBytes, &nbBytesReceived, NULL);

	return ((nbBytesReceived == nbBytes) && fSuccess);
#else
	/*
	for (long i=0 ; i< nbBytes ; i++)
	    buffer[i] =_COMPortHandle.ReadByte( 100 );
	    */
	    
	long bytesLeft = nbBytes;
	while(bytesLeft != 0)
	{
		int i = PollComport(_COMPortHandle, buffer, bytesLeft) ; 
		if (i > 0) {
			bytesLeft -= i;
			buffer += i;
		}
	}
	return true ;

#endif
}


// Write multiple bytes
bool CCPCBooster::WriteBuffer(unsigned char *buffer, const  long nbBytes)
{
#if _WINDOWS
	unsigned long nbBytesSend = 0;
    
	BOOL fSuccess = WriteFile(_COMPortHandle, buffer, nbBytes, &nbBytesSend, NULL);
	
	return ((nbBytesSend == nbBytes) && fSuccess);
#else
	/*
	 for (long i=0 ; i< nbBytes ; i++)
	    _COMPortHandle.WriteByte(buffer[i]);
    */

	long bytesLeft = nbBytes;
	while(bytesLeft != 0)
	{
		int i = SendBuf(_COMPortHandle, buffer, bytesLeft);
		if (i > 0) {
			bytesLeft -= i;
			buffer += i;
		}
	}
	return true ;
#endif

}

