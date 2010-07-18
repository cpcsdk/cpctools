#!/bin/env python
# -*- coding: utf-8 -*-
"""
@author Krusty/Benediction krusty@cpcscene.com
@date July 2010
@licence GPL
"""

"""
Arkos File Transfert implementation in python.
Why doing it from scratch ? Well I was unable to do it
in C++ for Linux, the library receive the data but is unable to send
it ...
I do not know why ...
"""

import sys
import os, os.path
import cpcbooster 
cimport cpcbooster


#List the various commands of AFT
AKS_CMD_UnknownCommand  = 0x00   # 'Unknown command')
AKS_CMD_MinCommand  = 0xF0-1   #   'Min command number')
AKS_CMD_TestComunication = 0xF0   # '#F0 Test Communication (send #80 to CPC to confirm)')
AKS_CMD_Send  = 0xF1   # "#F1 + Word = Asked to send 'word' bytes. 0=#10000")
AKS_CMD_EndTransfert  = 0xF2   #  '#F2 = End Tranfer. Close all file in output and input.')
AKS_CMD_Rewind  = 0xF3   # '#F3 = Rewind. Return to the beginning of the file.')
AKS_CMD_SendFileSize  = 0xF4   # '#F4 = Ask Filesize (send DWord to CPC).')
AKS_CMD_SendFilename  = 0xF5   # '#F5 = Ask Filename (send 12 bytes to CPC).')
AKS_CMD_CreateFile  = 0xF6   # '#F6 = Create file : Get 12 bytes (filename), Send byte (#80=ok autre=echec)')
AKS_CMD_AddData = 0xF7   # '#F7 = Add data : Get 2 bytes (size of data chunck, little endian) + Data')
AKS_CMD_CloseFile = 0xF8   # '#F8 = Close output file.')
AKS_CMD_InitDSK = 0xF9   # '#F9 = Initialise the DSK. Empty it, set some vars. Header creation done when all the tracks are given.'
                 # '   Ask for 2 chars for the Creator (10 for 1.0)   # Nb tracks, nb sides.')
AKS_CMD_WaitTrack = 0xFA   # '#FA = Wait for a track from the CPC, header included. Format described in the function.')
AKS_CMD_NoMoreTrack = 0xFB   # '#FB = Warn that no more track is to be transfered. The DSK can be completed and closed. This command closes the file.')
AKS_CMD_OpenFile  = 0xFC   # '#FC = Open a file in input. Get 12 bytes (filename). Send byte (#80=ok autre=echec)')
AKS_CMD_MaxCommand  = 0xFD   # 'Min command number')


EXPL_CMD = [ \
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
]

#List of the transfet mode
TRANSFERT_MODE_NO_TRANSFERT, \
TRANSFERT_MODE_GET_FILE, \
TRANSFERT_MODE_SEND_FILE, \
TRANSFERT_MODE_GET_DSK, \
TRANSFERT_MODE_SEND_DSK = range(5)

#List of verbosity
VERBOSITY_NO_DISPLAY, \
VERBOSITY_ERROR, \
VERBOSITY_WARNING, \
VERBOSITY_INFO, \
VERBOSITY_DEBUG, \
VERBOSITY_TRANSFERT_DEBUG = range(6)




cdef class AksFileTransfert(cpcbooster.CPCBooster):
  """Implementation of the Arkos File Transfert protocol.
  """

  def __init__(self, file):
    """Initialisation of the objects."""
    print "aks/ try", file
    super(AksFileTransfert, self).__init__( file)
#    CPCBooster.__init__(self, file)
    
    #Initialize various vars
    self._transfert_mode = TRANSFERT_MODE_NO_TRANSFERT
    self._nb_transfert = 0
    self._transfering_data = False
    self._EDSKFile = NULL
    self._inStream = None
    self._outStream = None
    self._amsFilename = ""
    self._filename = ""
    self._filepath = os.getcwd()
   
  cpdef GetCommand(self, wait):
    """Read a command send from the cpc."""
    if not wait:
      print 'Need to code ...'
      exit(-1)

    try:
      command = self.ReadByte()
    except:
      return AKS_CMD_UnknownCommand

    if command > AKS_CMD_MinCommand and command < AKS_CMD_MaxCommand:
      print " Command #%X: %s" %(command, EXPL_CMD[command-(AKS_CMD_MinCommand+1)])
      return command
    else:
      print " Unknown command get ($%x)" % command 
      return AKS_CMD_UnknownCommand


  cpdef Run(self, wait=True):
    """Run one transfert loop"""

    cmd = self.GetCommand(wait)

    cmd_tables = dict()
    cmd_tables[AKS_CMD_TestComunication ] = self.TestComunication
    cmd_tables[AKS_CMD_EndTransfert ] = self.EndTransfert
    cmd_tables[AKS_CMD_Send] = self.Send
    cmd_tables[AKS_CMD_SendFileSize] = self.SendFileSize
    cmd_tables[AKS_CMD_SendFilename] = self.SendFilename
    cmd_tables[AKS_CMD_Rewind] = self.Rewind
    cmd_tables[AKS_CMD_CreateFile] = self.CreateFile
    cmd_tables[AKS_CMD_AddData] = self.AddData
    cmd_tables[AKS_CMD_CloseFile] = self.CloseFile
    cmd_tables[AKS_CMD_InitDSK] = self.InitDSK
    cmd_tables[AKS_CMD_WaitTrack] = self.WaitTrack
    cmd_tables[AKS_CMD_NoMoreTrack] = self.NoMoreTrack
    cmd_tables[AKS_CMD_OpenFile] = self.OpenFile
    cmd_tables[AKS_CMD_UnknownCommand] = self.Unknown

    cmd_tables[cmd]( cmd)

    return cmd

  def Unknown(self, cmd):
    print "Unknown"

  cpdef Acknowledge(self):
    """Send acknowledgement"""
    ack = 0x80
  
    print "Acknowledge"

    self.WriteByte(ack)


  cpdef NotAcknowledge(self):
    """//! Send acknowledgement"""
    ack = 0x00;
  
    print "Not Acknowledge" 

    self.WriteByte(ack);



  cpdef TestComunication(self, cmd):

    if cmd != AKS_CMD_TestComunication:
      return False;

    self.Acknowledge();

    return True; 

  cpdef EndTransfert(self, cmd):
    if cmd != AKS_CMD_EndTransfert:
      return False
    
    if self._EDSKFile is not NULL:
      del_CExtendedDSKFile(self._EDSKFile)
      self._EDSKFile = NULL
    if self._inStream is not None:
      self._inStream = None
    if self._outStream is not None:
      self._outStream = None
    
    if self._transfering_data:
      self._nb_transfert += 1;
      self._transfering_data = False;
    
    return False

    

  cpdef Send(self, cmd):
    if cmd is not AKS_CMD_Send:
      return False
  
    self._transfering_data = True;

    s = self.ReadWaitWord()

    print "Read word %x %d" %(s, s)

    if s!=0:
      size = s
    else:
      size =  0x10000


    if self._inStream is not None:
      buffer = self._inStream.read(size)
      self.WriteBuffer(buffer)
    
      return True
    else:
      return False

  cpdef SendFileSize(self, cmd):
    raise Exception("Not implemented !")
    

  cpdef SendFilename(self, cmd):
    raise Exception("Not implemented !")
    

  cpdef Rewind(self, cmd):
    raise Exception("Not implemented !")
  

  
  cpdef SetForceFilename(self,filename):
    self._filename = filename


  cpdef SetFilepath(self,filepath):
    self._filepath = filepath




  cpdef GetFilename(self):
    """Returns the filename of the file to create."""
    if len(self._filename) != 0:
      return self._filename

    return os.path.join(self._filepath, self._amsFilename)

  cpdef CreateFile(self, cmd):
    if cmd != AKS_CMD_CreateFile:
      return False

    self._amsFilename = "".join([chr(b) for b in self.ReadBuffer(12)])

    print "Filename %s " % self._amsFilename

    if self._outStream is not None:
      self._outStream.close()
      self._outStream = None

    self._outStream = open(self.GetFilename(), 'wb')
    try:
      self._outStream = open(self.GetFilename(), 'wb')
      self.Acknowledge()
    except Exception, e:
      print e
      self.NotAcknowledge()

  cpdef AddData(self, cmd):
    raise Exception("Not implemented !")
    

  cpdef CloseFile(self, cmd):
    raise Exception("Not implemented !")
    

  cpdef InitDSK(self, cmd):
    if cmd is not AKS_CMD_InitDSK:
      return False
    
    self._transfering_data = True

    if self._EDSKFile is not NULL:
      del_CExtendedDSKFile(self._EDSKFile)
      self._EDSKFile = NULL

    if self._outStream is not None:
      self._outStream = None

    
    creator = [self.ReadByte(), self.ReadByte()]
    nbTrack = self.ReadByte()
    nbSide = self.ReadByte()

 #   self._EDSKFile = new_CExtendedDSKFile(nbTrack, nbSide)
    self._EDSKFile = new CExtendedDSKFile(nbTrack, nbSide)

    return True
    

  cpdef WaitTrack(self, cmd):
    """Read a track and store it.
    @bug There is a segmentation fault somewhere in the code ...
    """
    if cmd != AKS_CMD_WaitTrack:
      return False

    self._transfering_data = True

    cdef unsigned char trackNum = self.ReadByte()
    cdef unsigned char sideNum = self.ReadByte()

    cdef CDSKTrack *track = NULL
    
    if self._EDSKFile is not NULL:
      track = self._EDSKFile.GetTrack(trackNum, sideNum)
  

    if track is NULL:
      track = new CDSKTrack()
      print "Fake track reading !"
    

    print "Reading T", trackNum," S", sideNum

    track.TrackNumber = trackNum;
    track.SideNumber = sideNum;

    track.SectorSize = self.ReadByte()
    track.NbSector = self.ReadByte()
    track.GAP3Size = self.ReadByte()
    track.FillByte = self.ReadByte()

    print "track %d %d %x %x" %( track.SectorSize, track.NbSector, track.GAP3Size, track.FillByte)

    track.Allocate()

    cdef CDSKSector sect
    for s in range(track.NbSector):
      print 'Read Sector'
      sect = track.Sectors[s]

      sect.Track = self.ReadByte()
      sect.Side = self.ReadByte()
      sect.SectorID = self.ReadByte()
      sect.SectorSize = self.ReadByte()
      sect.FDCStatus1 = self.ReadByte()
      sect.FDCStatus2 = self.ReadByte()
      sect.DataSize = self.ReadWord()

      print 'Allocate'
      sect.Allocate()

      print 'Read data'
      tmp = self.ReadBuffer(sect.GetDataSize())
      for index, value in enumerate(tmp):
        sect.Data[index] = value

      print 'Update sector'
      sect.Update()

    print 'Update track'
    #track.Update()

#    track->DisplayInfo(std::cout);

    return True;


  cpdef NoMoreTrack(self, cmd):
    cdef fname = self.GetFilename()
    cdef std_string cpp_string = charp_to_stdstring(fname)
    if cmd != AKS_CMD_NoMoreTrack:
      return False
    
    if self._EDSKFile is not NULL:
      self._EDSKFile.SaveDSKFile(cpp_string)
      del self._EDSKFile
      self._EDSKFile = NULL
    
    if self._transferingData:
      self._nbTransfert += 1
      self.transferingData = False

    return False


  cpdef OpenFile(self, cmd):
    if cmd != AKS_CMD_OpenFile:
      return False

    self._amsFilename = "".join([chr(b) for b in self.ReadBuffer(12)]).strip()
    print self._amsFilename


    if self._inStream is not None:
      self._inStream.close()
      self._inStream = None

    self._inStream = open(self.GetFilename())
    self.Acknowledge()

    return True








