/**
* @class CCPCBinaryFile
* Classe permettant la gestion d'un fichier CPC
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#include "CCPCBinaryFile.h"

CCPCBinaryFile::CCPCBinaryFile()
:_file(NULL),
_header(NULL),
_content(NULL)
{
}

CCPCBinaryFile::~CCPCBinaryFile()
{
	if (_file != NULL)
		delete[] _file;
}

TFileType CCPCBinaryFile::getType() const
{
	return Binary;
}

unsigned int CCPCBinaryFile::getSize() const
{
	return (_header[0x18]+_header[0x19]*256);
}
unsigned int CCPCBinaryFile::getAdress() const
{
	return (_header[0x15]+_header[0x16]*256);
}

void CCPCBinaryFile::setAdress(unsigned int i_adr)
{
	_header[0x15] = i_adr % 256;
	_header[0x16] = (i_adr-_header[0x15])/256;
	
	unsigned int checksum = CCPCFile::checkSum(_header);
	_header[0x43]=checksum % 256;
	_header[0x44]=(checksum-_header[0x43])/256;
}

unsigned int CCPCBinaryFile::getExecute() const
{
	return (_header[0x1a]+_header[0x1b]*256);
}

void CCPCBinaryFile::setExecute(unsigned int i_adr)
{
	_header[0x1a] = i_adr % 256;
	_header[0x1b] = (i_adr-_header[0x1a])/256;
	
	unsigned int checksum = CCPCFile::checkSum(_header);
	_header[0x43]=checksum % 256;
	_header[0x44]=(checksum-_header[0x43])/256;
}

bool CCPCBinaryFile::isFileBinary(const std::string &filename)
{
	std::ifstream file;

	file.open(filename.c_str(),std::ios::binary);
	
	TOOLS_ASSERTMSG( file.good() , "Unable to open file " << filename);
	
	file.seekg(0, std::ios::end);
	std::streampos size = file.tellg();
	file.seekg(0, std::ios::beg);
	
	if (size <= 0x80)
		return false;

	unsigned char *buffer = new unsigned char[0x80];
	
	file.read((char*)buffer,0x80);
	
	bool isBinary = CCPCBinaryFile::isBinary(buffer);

	delete[] buffer;

	return isBinary;
}

bool CCPCBinaryFile::isBinary(const unsigned char *i_buffer, int bufferSize)
{
	if (bufferSize <= 0x80 && bufferSize != -1)
		return false;
	
	unsigned char type = i_buffer[0x12];
	unsigned int checksum = i_buffer[0x43]+i_buffer[0x44]*256;
	
	return (checksum == CCPCFile::checkSum(i_buffer) && ((type == 2) || (type == 0)));
}

void CCPCBinaryFile::createBinaryHeader(unsigned char *o_header, const unsigned int i_start, const unsigned int i_size, const unsigned int i_exec, const unsigned char i_type)
{
	for (unsigned int i=0;i<128;i++)
		o_header[i]=0;
	
	o_header[0x12]=i_type;
	
	o_header[0x15]=i_start % 256;
	o_header[0x16]=(i_start - o_header[0x15])/256;
	
	o_header[0x18]=o_header[0x40]=i_size % 256;
	o_header[0x19]=o_header[0x41]=(i_size - o_header[0x18])/256;
	
	o_header[0x1A]=i_exec % 256;
	o_header[0x1B]=(i_exec - o_header[0x1A])/256;
	
	unsigned int checksum = CCPCFile::checkSum(o_header);
	
	o_header[0x43]=checksum % 256;
	o_header[0x44]=(checksum-o_header[0x43])/256;
}

void CCPCBinaryFile::createFile(const void* i_buffer, const unsigned int i_size,bool i_createHeader)
{
	if (_file != NULL)
		delete[] _file;
	
	if (i_createHeader)
    {
		_file = new unsigned char[i_size+0x80];
		_header = _file;
		_content = _file + 0x80;
		CCPCBinaryFile::createBinaryHeader( _header , 0x0 , i_size , 0x0 , 2);
		memcpy(_content, i_buffer, i_size);
    }
	else
    {
		TOOLS_ASSERTMSG( (CCPCBinaryFile::isBinary((unsigned char*)i_buffer, i_size)) , "Not a binary buffer");
		_file = new unsigned char[i_size];
		_header = _file;
		_content = _file + 0x80;
		memcpy(_file, i_buffer, i_size);
    }
}

void CCPCBinaryFile::openFile(const std::string &i_filename,bool i_createHeader)
{
	std::ifstream file;
	file.open(i_filename.c_str(),std::ios::binary);
	TOOLS_ASSERTMSG( file.good() , "Unable to open file " << i_filename);

	file.seekg(0, std::ios::end);
	int size = file.tellg();
	file.seekg(0, std::ios::beg);
	
	TOOLS_ASSERTMSG( size < 1024*1024 , "Unable to open file " << i_filename << " file to long (>1Mo)");

	unsigned char *buffer = new unsigned char[size];
	
	file.read((char*)buffer,size);
	
	createFile(buffer,size,i_createHeader);
	
	delete[] buffer;
}

void CCPCBinaryFile::saveFile(const std::string &i_filename,bool i_saveHeader) const
{
	std::ofstream file;
	file.open(i_filename.c_str(),std::ios::binary);
	
	TOOLS_ASSERTMSG( file.good() , "Unable to save binary file " << i_filename);
	
	if(i_saveHeader)
		for (unsigned int i=0;i<0x80;i++)
			file << _header[i];
		for (unsigned int i=0;i<getSize();i++)
			file << _content[i];
}

unsigned int CCPCBinaryFile::getDatasSize() const
{
	return getSize() + 0x80;
}

unsigned char* CCPCBinaryFile::getDatas() const
{
	return _file;
}

std::ostream& CCPCBinaryFile::printInfo(std::ostream& io_os) const
{
	io_os << "Binary file " << std::endl;
	io_os.setf(std::ios::hex,std::ios::basefield);
	io_os.width(4);
	io_os << "Adress : " << getAdress() << " ";
	io_os.width(4);
	io_os << "Length : " << getSize() << " ";
	io_os.width(4);
	io_os << "Execute : " << getExecute() << std::endl;
	io_os.setf(std::ios::dec,std::ios::basefield);
	return io_os;
}

std::ostream& CCPCBinaryFile::printHeader(std::ostream& io_os) const
{
	io_os << "Header Binary File" << std::endl;
	io_os.setf(std::ios::hex,std::ios::basefield);
	io_os.width(2);
	for (unsigned int i=0;i<(0x80/16);i++)
    {
		unsigned int c;
		io_os.width(4);
		io_os << i*16 << ":";
		for (c=0;c<16;c++)
		{
			io_os.width(2);
			io_os << (unsigned int)_header[i*16+c] << " ";
		}
		for (c=0;c<16;c++)
			io_os << (unsigned char)((_header[i*16+c]<32) ? '.' : (_header[i*16+c]));
		io_os << std::endl;
    }
	io_os.setf(std::ios::dec,std::ios::basefield);
	return io_os;
}

std::ostream& CCPCBinaryFile::printContent(std::ostream& io_os) const
{
	io_os << "Content" << std::endl;
	io_os.setf(std::ios::hex,std::ios::basefield);
	unsigned int nbLignes = (getSize() % 16 == 0) ? getSize()/16 : (getSize()/16+1);
	for (unsigned int i=0;i<nbLignes;i++)
    {
		unsigned int c;
		io_os.width(4);
		io_os << i*16 << ":";
		for (c=0;c<16;c++)
		{
			io_os.width(2);
			if ((i*16+c)<getSize())
				io_os << (unsigned int)_content[i*16+c] << " ";
			else
				io_os << "   ";
		}
		for (c=0;c<16;c++)
			if (i*16+c<getSize())
				io_os << (unsigned char)((_content[i*16+c]<32) ? '.' : (_content[i*16+c]));
			else
				io_os << " ";
			io_os << std::endl;
    }
	io_os.setf(std::ios::dec,std::ios::basefield);
	return io_os;
}
