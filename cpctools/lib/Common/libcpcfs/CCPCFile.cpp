/**
* @class CCPCFile
* Classe permettant la gestion d'un fichier CPC
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#include "CCPCFile.h"
#include "CCPCBinaryFile.h"
#include "CCPCAsciiFile.h"

#include "CError.h"

CCPCFile::CCPCFile()
{
}

CCPCFile::~CCPCFile()
{
}

unsigned int CCPCFile::checkSum(const void *i_buffer)
{
	unsigned char *buffer=(unsigned char *)i_buffer;
	
	unsigned int checksum=0;
	for (unsigned int i=0;i<0x42;i++)
		checksum+=buffer[i];
	return checksum;
}

CCPCFile* CCPCFile::openFile(const void *i_buffer, const unsigned int i_size)
{
	CCPCFile *file = NULL;
	
	// !! attention le i_size est surement bcp + grand que le fichier ! (provient surement d'un getFile sur CCPCDisc...)
	if (CCPCBinaryFile::isBinary((unsigned char*)i_buffer, i_size))
    {
		file = new CCPCBinaryFile();
		
		unsigned int realSize = ((unsigned char*)i_buffer)[0x19]*256+((unsigned char*)i_buffer)[0x18];
		
		file->createFile(i_buffer,realSize+0x80,false);
    }
	else
    {
		file = new CCPCAsciiFile();
		// On est arrivé a la fin du buffer, et pas de 0x1a ! on en crée 1 !
		file->createFile(i_buffer,i_size,false);
    }
	return file;
}

std::ostream& operator<<(std::ostream &io_os,const CCPCFile &i_file)
{
	return i_file.printInfo(io_os);
}
