cimport cpcbooster

cdef extern from "string":
     ctypedef struct std_string "std::string":
         pass
     cdef std_string charp_to_stdstring "std::string"(char*) 

cdef extern from "CDSKFile.h":
  cdef cppclass CDSKSector:
    unsigned char Track
    unsigned char Side
    unsigned char SectorID
    unsigned char SectorSize
    unsigned char*  Data
    unsigned char FDCStatus1
    unsigned char FDCStatus2
    unsigned short  DataSize
    void Allocate()
    void Update()
    int GetDataSize()
 
  
  cdef cppclass CDSKTrack:
      unsigned char TrackNumber, SideNumber
      unsigned char SectorSize, NbSector, GAP3Size, FillByte 
      CDSKSector*    Sectors
      void Allocate()
      void Update()

  cdef cppclass CExtendedDSKFile:
    CExtendedDSKFile(char, char)
    CDSKTrack *GetTrack(char, char)
    void SaveDSKFile(std_string)

    
  CExtendedDSKFile *new_CExtendedDSKFile "new CExtendedDSKFile" (char, char)
  void del_CExtendedDSKFile "delete" (CExtendedDSKFile *dsk)


cdef class AksFileTransfert(cpcbooster.CPCBooster):
 
  cdef CExtendedDSKFile *_EDSKFile
  cdef int _transfert_mode
  cdef int _nb_transfert
  cdef bool _transfering_data
  cdef str     _filename

  #! Current filename in Amstrad format
  cdef  str  _amsFilename

  #Current filename path for creating file
  cdef str _filepath

  #stream used for read
  cdef _inStream
  #stream used for write
  cdef _outStream
 

  cpdef GetCommand(self, wait)
  cpdef Run(self, wait=*)
  cpdef Acknowledge(self)
  cpdef NotAcknowledge(self)
  cpdef TestComunication(self, cmd)
  cpdef EndTransfert(self, cmd)
  cpdef Send(self, cmd)
  cpdef SendFileSize(self, cmd)
  cpdef SendFilename(self, cmd)
  cpdef Rewind(self, cmd)
  cpdef SetForceFilename(self,filename)
  cpdef SetFilepath(self,filepath)
  cpdef GetFilename(self)
  cpdef CreateFile(self, cmd)
  cpdef AddData(self, cmd)
  cpdef CloseFile(self, cmd)
  cpdef InitDSK(self, cmd)
  cpdef WaitTrack(self, cmd)
  cpdef NoMoreTrack(self, cmd)
  cpdef OpenFile(self, cmd)
