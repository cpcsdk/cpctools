/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015 Teunis van Beelen
*
* Email: teuniz@gmail.com
*
***************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
***************************************************************************
*
* This version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*/

/* Last revision: January 10, 2015 */

/* For more info and how to use this library, visit: http://www.teuniz.net/RS-232/ */


#include "rs232.h"


#ifndef _WIN32 /* Unixen */


int error;


/* returns -1 on error */
int RS232_OpenComport(const char* comport, int baudrate, const char *mode, HANDLE *handle)
{
  int baudr,
      status;

  struct termios new_port_settings;

  switch(baudrate)
  {
    case      50 : baudr = B50;
                   break;
    case      75 : baudr = B75;
                   break;
    case     110 : baudr = B110;
                   break;
    case     134 : baudr = B134;
                   break;
    case     150 : baudr = B150;
                   break;
    case     200 : baudr = B200;
                   break;
    case     300 : baudr = B300;
                   break;
    case     600 : baudr = B600;
                   break;
    case    1200 : baudr = B1200;
                   break;
    case    1800 : baudr = B1800;
                   break;
    case    2400 : baudr = B2400;
                   break;
    case    4800 : baudr = B4800;
                   break;
    case    9600 : baudr = B9600;
                   break;
    case   19200 : baudr = B19200;
                   break;
    case   38400 : baudr = B38400;
                   break;
    case   57600 : baudr = B57600;
                   break;
    case  115200 : baudr = B115200;
                   break;
    case  230400 : baudr = B230400;
                   break;
    case  460800 : baudr = B460800;
                   break;
    case  500000 : baudr = B500000;
                   break;
    case  576000 : baudr = B576000;
                   break;
    case  921600 : baudr = B921600;
                   break;
    case 1000000 : baudr = B1000000;
                   break;
    case 1152000 : baudr = B1152000;
                   break;
    case 1500000 : baudr = B1500000;
                   break;
    case 2000000 : baudr = B2000000;
                   break;
    case 2500000 : baudr = B2500000;
                   break;
    case 3000000 : baudr = B3000000;
                   break;
    case 3500000 : baudr = B3500000;
                   break;
    case 4000000 : baudr = B4000000;
                   break;
    default      : printf("invalid baudrate\n");
                   return -1;
                   break;
  }

  int cbits=CS8,
      cpar=0,
      ipar=IGNPAR,
      bstop=0;

  if(strlen(mode) != 3)
  {
    printf("invalid mode \"%s\"\n", mode);
    return -1;
  }

  switch(mode[0])
  {
    case '8': cbits = CS8;
              break;
    case '7': cbits = CS7;
              break;
    case '6': cbits = CS6;
              break;
    case '5': cbits = CS5;
              break;
    default : printf("invalid number of data-bits '%c'\n", mode[0]);
              return -1;
              break;
  }

  switch(mode[1])
  {
    case 'N':
    case 'n': cpar = 0;
              ipar = IGNPAR;
              break;
    case 'E':
    case 'e': cpar = PARENB;
              ipar = INPCK;
              break;
    case 'O':
    case 'o': cpar = (PARENB | PARODD);
              ipar = INPCK;
              break;
    default : printf("invalid parity '%c'\n", mode[1]);
              return -1;
              break;
  }

  switch(mode[2])
  {
    case '1': bstop = 0;
              break;
    case '2': bstop = CSTOPB;
              break;
    default : printf("invalid number of stop bits '%c'\n", mode[2]);
              return -1;
              break;
  }

/*
http://pubs.opengroup.org/onlinepubs/7908799/xsh/termios.h.html

http://man7.org/linux/man-pages/man3/termios.3.html
*/

  *handle = open(comport, O_RDWR | O_NOCTTY | O_NDELAY);
  if(*handle==-1)
  {
    perror("unable to open comport ");
    return -1;
  }

/*  error = tcgetattr(handle, old_port_settings + comport_number);
  if(error==-1)
  {
    close(handle);
    perror("unable to read portsettings ");
    return -1;
  }
  */
  memset(&new_port_settings, 0, sizeof(new_port_settings));  /* clear the new struct */

  new_port_settings.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD;
  new_port_settings.c_iflag = ipar;
  new_port_settings.c_oflag = 0;
  new_port_settings.c_lflag = 0;
  new_port_settings.c_cc[VMIN] = 0;      /* block untill n bytes are received */
  new_port_settings.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */

  cfsetispeed(&new_port_settings, baudr);
  cfsetospeed(&new_port_settings, baudr);

  error = tcsetattr(*handle, TCSANOW, &new_port_settings);
  if(error==-1)
  {
    close(*handle);
    perror("unable to adjust portsettings ");
    return -1;
  }

  if(ioctl(*handle, TIOCMGET, &status) == -1)
  {
    perror("unable to get portstatus");
    return -1;
  }

  status |= TIOCM_DTR;    /* turn on DTR */
  status |= TIOCM_RTS;    /* turn on RTS */

  if(ioctl(*handle, TIOCMSET, &status) == -1)
  {
    perror("unable to set portstatus");
    return -1;
  }

  return 0;
}


int RS232_PollComport(HANDLE handle, unsigned char *buf, int size)
{
  int n;

  n = read(handle, buf, size);

  return n;
}


int RS232_SendByte(HANDLE handle, unsigned char byte)
{
  int n;

  n = write(handle, &byte, 1);
  return (n<0);
}


int RS232_SendBuf(HANDLE handle, unsigned char *buf, int size)
{
  return (write(handle, buf, size));
}


void RS232_CloseComport(HANDLE handle)
{
  int status;

  if(ioctl(handle, TIOCMGET, &status) == -1)
  {
    perror("unable to get portstatus");
  }

  status &= ~TIOCM_DTR;    /* turn off DTR */
  status &= ~TIOCM_RTS;    /* turn off RTS */

  if(ioctl(handle, TIOCMSET, &status) == -1)
  {
    perror("unable to set portstatus");
  }

  close(handle);
}

/*
Constant  Description
TIOCM_LE        DSR (data set ready/line enable)
TIOCM_DTR       DTR (data terminal ready)
TIOCM_RTS       RTS (request to send)
TIOCM_ST        Secondary TXD (transmit)
TIOCM_SR        Secondary RXD (receive)
TIOCM_CTS       CTS (clear to send)
TIOCM_CAR       DCD (data carrier detect)
TIOCM_CD        see TIOCM_CAR
TIOCM_RNG       RNG (ring)
TIOCM_RI        see TIOCM_RNG
TIOCM_DSR       DSR (data set ready)

http://man7.org/linux/man-pages/man4/tty_ioctl.4.html
*/

int RS232_IsDCDEnabled(HANDLE handle)
{
  int status;

  ioctl(handle, TIOCMGET, &status);

  return (status&TIOCM_CAR);
}

int RS232_IsCTSEnabled(HANDLE handle)
{
  int status;

  ioctl(handle, TIOCMGET, &status);

  return (status&TIOCM_CTS);
}

int RS232_IsDSREnabled(HANDLE handle)
{
  int status;

  ioctl(handle, TIOCMGET, &status);

  return (status&TIOCM_DSR);
}

void RS232_enableDTR(HANDLE handle)
{
  int status;

  if(ioctl(handle, TIOCMGET, &status) == -1)
  {
    perror("unable to get portstatus");
  }

  status |= TIOCM_DTR;    /* turn on DTR */

  if(ioctl(handle, TIOCMSET, &status) == -1)
  {
    perror("unable to set portstatus");
  }
}

void RS232_disableDTR(HANDLE handle)
{
  int status;

  if(ioctl(handle, TIOCMGET, &status) == -1)
  {
    perror("unable to get portstatus");
  }

  status &= ~TIOCM_DTR;    /* turn off DTR */

  if(ioctl(handle, TIOCMSET, &status) == -1)
  {
    perror("unable to set portstatus");
  }
}

void RS232_enableRTS(HANDLE handle)
{
  int status;

  if(ioctl(handle, TIOCMGET, &status) == -1)
  {
    perror("unable to get portstatus");
  }

  status |= TIOCM_RTS;    /* turn on RTS */

  if(ioctl(handle, TIOCMSET, &status) == -1)
  {
    perror("unable to set portstatus");
  }
}

void RS232_disableRTS(HANDLE handle)
{
  int status;

  if(ioctl(handle, TIOCMGET, &status) == -1)
  {
    perror("unable to get portstatus");
  }

  status &= ~TIOCM_RTS;    /* turn off RTS */

  if(ioctl(handle, TIOCMSET, &status) == -1)
  {
    perror("unable to set portstatus");
  }
}

#else         /* windows */

char mode_str[128];

int RS232_OpenComport(const char *comport, int baudrate, const char *mode, HANDLE *handle)
{
  char compath[BUFSIZ] = "\\\\.\\";
  DCB port_settings;
  COMMTIMEOUTS Cptimeouts;
      
  strcat(compath, comport); /* allows com ports above COM4 to work */

  switch(baudrate)
  {
    case     110 : strcpy(mode_str, "baud=110");
                   break;
    case     300 : strcpy(mode_str, "baud=300");
                   break;
    case     600 : strcpy(mode_str, "baud=600");
                   break;
    case    1200 : strcpy(mode_str, "baud=1200");
                   break;
    case    2400 : strcpy(mode_str, "baud=2400");
                   break;
    case    4800 : strcpy(mode_str, "baud=4800");
                   break;
    case    9600 : strcpy(mode_str, "baud=9600");
                   break;
    case   19200 : strcpy(mode_str, "baud=19200");
                   break;
    case   38400 : strcpy(mode_str, "baud=38400");
                   break;
    case   57600 : strcpy(mode_str, "baud=57600");
                   break;
    case  115200 : strcpy(mode_str, "baud=115200");
                   break;
    case  128000 : strcpy(mode_str, "baud=128000");
                   break;
    case  256000 : strcpy(mode_str, "baud=256000");
                   break;
    case  500000 : strcpy(mode_str, "baud=500000");
                   break;
    case 1000000 : strcpy(mode_str, "baud=1000000");
                   break;
    default      : printf("invalid baudrate\n");
                   return -1;
                   break;
  }

  if(strlen(mode) != 3)
  {
    printf("invalid mode \"%s\"\n", mode);
    return -1;
  }

  switch(mode[0])
  {
    case '8': strcat(mode_str, " data=8");
              break;
    case '7': strcat(mode_str, " data=7");
              break;
    case '6': strcat(mode_str, " data=6");
              break;
    case '5': strcat(mode_str, " data=5");
              break;
    default : printf("invalid number of data-bits '%c'\n", mode[0]);
              return -1;
              break;
  }

  switch(mode[1])
  {
    case 'N':
    case 'n': strcat(mode_str, " parity=n");
              break;
    case 'E':
    case 'e': strcat(mode_str, " parity=e");
              break;
    case 'O':
    case 'o': strcat(mode_str, " parity=o");
              break;
    default : printf("invalid parity '%c'\n", mode[1]);
              return -1;
              break;
  }

  switch(mode[2])
  {
    case '1': strcat(mode_str, " stop=1");
              break;
    case '2': strcat(mode_str, " stop=2");
              break;
    default : printf("invalid number of stop bits '%c'\n", mode[2]);
              return -1;
              break;
  }

  strcat(mode_str, " dtr=on rts=on");

/*
http://msdn.microsoft.com/en-us/library/windows/desktop/aa363145%28v=vs.85%29.aspx

http://technet.microsoft.com/en-us/library/cc732236.aspx
*/

  *handle = CreateFileA(compath,
                      GENERIC_READ|GENERIC_WRITE,
                      0,                          /* no share  */
                      NULL,                       /* no security */
                      OPEN_EXISTING,
                      0,                          /* no threads */
                      NULL);                      /* no templates */

  if(*handle==INVALID_HANDLE_VALUE)
  {
    printf("unable to open comport\n");
    return -1;
  }

  memset(&port_settings, 0, sizeof(port_settings));  /* clear the new struct  */
  port_settings.DCBlength = sizeof(port_settings);

  if(!BuildCommDCBA(mode_str, &port_settings))
  {
    printf("unable to set comport dcb settings\n");
    CloseHandle(*handle);
    return -1;
  }

  if(!SetCommState(*handle, &port_settings))
  {
    printf("unable to set comport cfg settings\n");
    CloseHandle(*handle);
    return -1;
  }

  Cptimeouts.ReadIntervalTimeout         = 0;
  Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
  Cptimeouts.ReadTotalTimeoutConstant    = 0;
  Cptimeouts.WriteTotalTimeoutMultiplier = 0;
  Cptimeouts.WriteTotalTimeoutConstant   = 0;

  if(!SetCommTimeouts(*handle, &Cptimeouts))
  {
    printf("unable to set comport time-out settings\n");
    CloseHandle(*handle);
    return -1;
  }

  return 0;
}


int RS232_PollComport(HANDLE handle, unsigned char *buf, int size)
{
  DWORD n;

  ReadFile(handle, buf, size, &n, NULL);

  return (int)n;
}


int RS232_SendByte(HANDLE handle, unsigned char byte)
{
  DWORD n;

  WriteFile(handle, &byte, 1, &n, NULL);

  return (n<0);
}


int RS232_SendBuf(HANDLE handle, unsigned char *buf, int size)
{
  DWORD n;

  if (WriteFile(handle, buf, size, &n, NULL))
  {
    return (int)n;
  }

  return -1;
}


void RS232_CloseComport(HANDLE handle)
{
  CloseHandle(handle);
}

/*
http://msdn.microsoft.com/en-us/library/windows/desktop/aa363258%28v=vs.85%29.aspx
*/

int RS232_IsDCDEnabled(HANDLE handle)
{
  DWORD status;

  GetCommModemStatus(handle, &status);

  return (status&MS_RLSD_ON);
}


int RS232_IsCTSEnabled(HANDLE handle)
{
  DWORD status;

  GetCommModemStatus(handle, &status);

  return (status&MS_CTS_ON);
}


int RS232_IsDSREnabled(HANDLE handle)
{
  DWORD status;

  GetCommModemStatus(handle, &status);

  return (status&MS_DSR_ON);
}


void RS232_enableDTR(HANDLE handle)
{
  EscapeCommFunction(handle, SETDTR);
}


void RS232_disableDTR(HANDLE handle)
{
  EscapeCommFunction(handle, CLRDTR);
}


void RS232_enableRTS(HANDLE handle)
{
  EscapeCommFunction(handle, SETRTS);
}


void RS232_disableRTS(HANDLE handle)
{
  EscapeCommFunction(handle, CLRRTS);
}


#endif


void RS232_cputs(HANDLE handle, const char *text)  /* sends a string to serial port */
{
  while (*text != 0)
  {
    RS232_SendByte(handle, *text++);
  }
}

