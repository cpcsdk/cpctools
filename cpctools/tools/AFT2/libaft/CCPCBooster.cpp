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
 
 
// Create a communication with the CPC Booster on the given port.
// The port number can be a full name (COM17, /dev/ttyUSB0, ...), or just a
// number, or a substring of the port name (USB0, ACM0, ...).
CCPCBooster::CCPCBooster(std::string comNumber, int baudrate):
_COMPortNumber(comNumber),
_baudrate(baudrate),
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

	if (RS232_OpenComport(_COMPortNumber.c_str(),_baudrate,"8N1",&_COMPortHandle) == 0)
	{
	    _currentState = PortOpened ;
	}
}


// Close communication (you can reopen it later)
void CCPCBooster::ClosePort()
{
	if (_currentState == PortOpened)
	{
		RS232_CloseComport(_COMPortHandle);
	}
	_currentState = PortClosed;
}


// Read a byte. If the buffer is empty, wait for a byte to come
void CCPCBooster::ReadWaitByte(unsigned char &val)
{
	int ret;
	do {
		ret = RS232_PollComport(_COMPortHandle, &val, 1);
	} while (ret  != 1);
}


// Non-blocking read of a byte. Returns the number of bytes read.
int CCPCBooster::ReadByte(unsigned char &val)
{
	return RS232_PollComport(_COMPortHandle, &val, 1);
}


// Write a byte
bool CCPCBooster::WriteByte(const unsigned char val)
{
  return RS232_SendByte(_COMPortHandle, val) == 0;
}


// Read a word in little-endian format.
void CCPCBooster::ReadWaitWord(unsigned short &val)
{
	unsigned char byte1, byte2 ;

	ReadWaitByte(byte1);
	ReadWaitByte(byte2);
	val = byte2 * 256 + byte1 ;
}


// Write a word in little endian format
void CCPCBooster::WriteWord(const unsigned short val)
{
	WriteByte(val&0xFF);
	WriteByte(val>>8);
}


// Read multiple bytes at once, waiting if there isn't enough
bool CCPCBooster::ReadWaitBuffer(unsigned char *buffer, const  long nbBytes)
{
	long bytesLeft = nbBytes;

	while(bytesLeft != 0)
	{
		int i = RS232_PollComport(_COMPortHandle, buffer, bytesLeft);
		if (i >= 0) {
			bytesLeft -= i;
			buffer += i;
		} else {
			// error!
			return false;
		}
	}
	return true;
}


// read multiple bytes, non-blocking.
long CCPCBooster::ReadBuffer(unsigned char *buffer, const long nbBytes)
{
	return RS232_PollComport(_COMPortHandle, buffer, nbBytes);
}


// Write multiple bytes
bool CCPCBooster::WriteBuffer(unsigned char *buffer, const  long nbBytes)
{
	long bytesLeft = nbBytes;

	while(bytesLeft != 0)
	{
		int i = RS232_SendBuf(_COMPortHandle, buffer, bytesLeft);
		if (i >= 0) {
			bytesLeft -= i;
			buffer += i;
		} else {
			// error!
			return false;
		}
	}
	return true ;
}

