/**
 * @class CCPCBooster
 * Class used to communicate with CPCBooster
 * @author Thierry JOUIN
 * @version 1.1
 * @date 06/02/2006
 */

#include "CCPCBooster.h"
#include <stdio.h>

#include <iostream>

/**
 * @todo Gerer comNumber comme un entier sous windows et une chaine sous linux
 *
 * Pourquoi ne pas avoir utilise les read/write buffer depuis les read/write byte/word ?
 */
CCPCBooster::CCPCBooster(int comNumber) :
_COMPortNumber(comNumber),
_currentState(PortFailed),
_currentError(ErrOK)
{
	OpenPort();
}

CCPCBooster::~CCPCBooster()
{
	ClosePort();
}

bool CCPCBooster::IsOpen() const
{
	return (_currentState == PortOpened);
}

CCPCBoosterState CCPCBooster::GetState() const
{
	return _currentState;
}

void CCPCBooster::OpenPort()
{
	if (_currentState == PortOpened)
	{
		ClosePort();
	}

	_currentState = PortFailed;

	char portName[256];

/**
 * code specifique aux windows
 */
#if _WINDOWS
	sprintf(portName,"COM%d",_COMPortNumber);

    _COMPortHandle = CreateFile(	portName,
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
	sprintf(portName,"/dev/ttyS%d", _COMPortNumber) ;

	_COMPortHandle = open(portName, O_RDWR | O_NOCTTY );
        if (_COMPortHandle < 0)
        {
                _currentError = ErrInvalidHandle;
        }
        else
        {

	struct termios oldtio,newtio;

		tcgetattr(_COMPortHandle,&oldtio); 		/* save current serial port settings */
        	bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
        		
		
		if ( /*fSuccess != TRUE*/ 0) 
		{
			_currentError = ErrGetStateFailed;
		}
		else
		{
 /* 
 *           CRTSCTS : output hardware flow control (only used if the cable has
 *           all necessary lines. See sect. 7 of Serial-HOWTO)
 *           CS8     : 8n1 (8bit,no parity,1 stopbit)
 *           CLOCAL  : local connection, no modem contol
 *          CREAD   : enable receiving characters
 */
   
	      newtio.c_cflag = B115200 | CRTSCTS | CS8 | CLOCAL | CREAD;
         
/*
 *           IGNPAR  : ignore bytes with parity errors
 *           ICRNL   : map CR to NL (otherwise a CR input on the other computer
 *                     will not terminate input)
 *                otherwise make device raw (no other input processing)
 */
         newtio.c_iflag = IGNPAR | ICRNL;
         
/*
 *          Raw output.
 */
         newtio.c_oflag = 0;
         
/*
 *           ICANON  : enable canonical input
 *                     disable all echo functionality, and don't send signals to calling program
 */
         newtio.c_lflag = ICANON;
         
        /* 
 *           initialize all control characters 
 *                     default values can be found in /usr/include/termios.h, and are given
 *                               in the comments, but we don't need them here
 *                                       */
         newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
         newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
         newtio.c_cc[VERASE]   = 0;     /* del */
         newtio.c_cc[VKILL]    = 0;     /* @ */
         newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
         newtio.c_cc[VTIME]    = 10;    // Timeout 
         newtio.c_cc[VMIN]     = 0;     
         newtio.c_cc[VSWTC]    = 0;     /* '\0' */
         newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
         newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
         newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
         newtio.c_cc[VEOL]     = 0;     /* '\0' */
         newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
         newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
         newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
         newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
         newtio.c_cc[VEOL2]    = 0;     /* '\0' */

        /* 
 *           now clean the modem line and activate the settings for the port
 *                   */
         tcflush(_COMPortHandle, TCIFLUSH);
         tcsetattr(_COMPortHandle,TCSANOW,&newtio);

		}
	}
#endif
	
}
void CCPCBooster::ClosePort()
{
	if (_currentState == PortOpened)
	{
#if _WINDOWS
		CloseHandle(_COMPortHandle);
#else
		close(_COMPortHandle);
#endif
	}
	_currentState = PortClosed;
}

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
	unsigned long nbBytesReceived = 0 ;
	while ( nbBytesReceived != 1 )
	{
		nbBytesReceived = read( _COMPortHandle, &val, 1) ;	
	}

	return nbBytesReceived == 1 ;
#endif
}
bool CCPCBooster::ReadByte(unsigned char &val)
{
#if _WINDOWS
	unsigned long nbBytesReceived = 0;
	
	BOOL fSuccess = ReadFile (_COMPortHandle, &val, 1, &nbBytesReceived, NULL);

	return nbBytesReceived == 1 ;
#else
        unsigned long nbBytesReceived = 0 ;
        nbBytesReceived = read( _COMPortHandle, &val, 1) ;  
	return nbBytesReceived == 1;
#endif

}
bool CCPCBooster::WriteByte(const unsigned char val)
{
#if _WINDOWS
	unsigned long nbBytesSend = 0;
    
	BOOL fSuccess = WriteFile(_COMPortHandle, &val, 1, &nbBytesSend, NULL);
	
	return ((nbBytesSend == 1) && fSuccess);
#else
	unsigned long nbBytesSend = 0 ;
	nbBytesSend = write( _COMPortHandle, &val, 1) ;
	return nbBytesSend == 1 ;
#endif
}

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
        unsigned long nbBytesReceived = 0 ;
        while ( nbBytesReceived != 2 )
        {
                nbBytesReceived = read( _COMPortHandle, &val, 2) ;
        }

        return nbBytesReceived == 2 ;

#endif
}

bool CCPCBooster::ReadWord(unsigned short &val)
{
#if _WINDOWS
	unsigned long nbBytesReceived = 0;
	
	BOOL fSuccess = ReadFile (_COMPortHandle, &val, 2, &nbBytesReceived, NULL);

	return ((nbBytesReceived == 2) && fSuccess);
#else
        unsigned long nbBytesReceived = 0 ;
        nbBytesReceived = read( _COMPortHandle, &val, 2) ;
        return nbBytesReceived == 2;

#endif
}
bool CCPCBooster::WriteWord(const unsigned short val)
{
#if _WINDOWS
	unsigned long nbBytesSend = 0;
    
	BOOL fSuccess = WriteFile(_COMPortHandle, &val, 2, &nbBytesSend, NULL);
	
	return ((nbBytesSend == 2) && fSuccess);
#else
        unsigned long nbBytesSend = 0 ;
        nbBytesSend = write( _COMPortHandle, &val, 2) ;
        return nbBytesSend == 2 ;

#endif
}

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
        unsigned long nbBytesReceived = 0 ;
        while ( nbBytesReceived != nbBytes )
        {
                nbBytesReceived = read( _COMPortHandle, buffer, nbBytes) ;
        }

        return nbBytesReceived == nbBytes ;

#endif
}
bool CCPCBooster::ReadBuffer(unsigned char *buffer, const long nbBytes)
{
#if _WINDOWS
	unsigned long nbBytesReceived = 0;
	
	BOOL fSuccess = ReadFile (_COMPortHandle, buffer, nbBytes, &nbBytesReceived, NULL);

	return ((nbBytesReceived == nbBytes) && fSuccess);
#else
        unsigned long nbBytesRead = 0 ;
        nbBytesRead = write( _COMPortHandle, buffer, nbBytes) ;
        return nbBytesRead == nbBytes ;

#endif
}
bool CCPCBooster::WriteBuffer(unsigned char *buffer, const  long nbBytes)
{
#if _WINDOWS
	unsigned long nbBytesSend = 0;
    
	BOOL fSuccess = WriteFile(_COMPortHandle, buffer, nbBytes, &nbBytesSend, NULL);
	

	std::cout << (int)nbBytesSend << " bytes send" << std::endl;
	return ((nbBytesSend == nbBytes) && fSuccess);
#else
        unsigned long nbBytesSend = 0 ;
        nbBytesSend = write( _COMPortHandle, buffer, nbBytes) ;
	std::cout << (int)nbBytesSend << " bytes send" << std::endl;
        return nbBytesSend == nbBytes ;
#endif

}

