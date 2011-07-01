#!/bin/env python
# -*- coding: utf-8 -*-
"""
@author Krusty/Benediction krusty@cpcscene.com
@date July 2010
@licence GPL
"""

"""CPC booster manipulation from python."""

import serial


STATE_PORT_OPENED = 0
STATE_PORT_FAILED = 1
STATE_PORT_CLOSED = 3

ERROR_OK = 0
ERROR_INVALID_HANDLE = 1
ERROR_GET_STATE_FAILED = 2
ERROR_SET_STATE_FAILED = 3


cdef class CPCBooster(object):
  """Communication class between a pc and a cpc
  through a CPCBooster.
  """


  def __init__(self, file):
    """Open the communication on file."""
  
    print "cpc/ try", file
    self._COMPortNumber = file
    self._currentState = STATE_PORT_FAILED
    self._currentError = ERROR_OK

    self.__OpenPort__()


  def IsOpen(self):
    """Check if current port is open"""
    return self._currentState == STATE_PORT_OPENED

  def GetState(self):
    """Return current CPCBooster state"""
    raise Exception("Not implemented !")

  def ReadWaitByte(self):
    """Read a byte from COM port, waiting"""
    return self.ReadByte()

  def ReadByte(self):
    """Read a byte from COM port, not waiting"""
    byte = self._COMPortHandle.read(1)
    value = ord(byte)
    print "Readed &%X" % value
    return value

  def WriteByte(self, val):
    """Write a byte to COM port"""
    self._COMPortHandle.write(chr(val))

  def ReadWaitWord(self):
    """Read a word from COM port, waiting"""
    return self.ReadWord()

  def ReadWord(self):
    """Read a word from COM port, not waiting"""

    byte1 = self.ReadByte()
    byte2 = self.ReadByte()

    return byte2*256 + byte1

  def WriteWord(self,val):
    """Write a word to COM port"""
    raise Exception("Not implemented !")

  def ReadWaitBuffer(self,buffer, nbBytes):
    """Read N bytes from COM port, waiting"""
    raise Exception("Not implemented !")

  def ReadBuffer(self, nbBytes):
    """Read N bytes from COM port, not waiting"""
    bytes = self._COMPortHandle.read(nbBytes)

    return [ord(b) for b in bytes]

  def WriteBuffer(self,buffer):
    """Write N bytes to COM port"""
    self._COMPortHandle.write(buffer)


  def __OpenPort__(self):
    """Open COM port"""
    
    if self._currentState == STATE_PORT_OPENED:
      self.__ClosePort__()

    self._currentState = STATE_PORT_FAILED

    try:
      self._COMPortHandle = serial.Serial(self._COMPortNumber, \
          115200, \
          bytesize=8, \
          parity='N', \
          stopbits=1, \
          xonxoff=0, \
          rtscts=0, \
          timeout=10)
    except serial.SerialException, err:
      self. _currentError = ERROR_SET_STATE_FAILED
      print err
      return

    self._currentState = STATE_PORT_OPENED

  def __ClosePort__(self):
    """Close COM port"""

