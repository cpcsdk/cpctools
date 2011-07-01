/**
* @class CCPCAsciiFile
* Classe permettant la gestion d'un fichier CPC
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#include "CCPCAsciiFile.h"

CCPCAsciiFile::CCPCAsciiFile()
:_content(NULL),
_size(0)
{
}

CCPCAsciiFile::~CCPCAsciiFile()
{
	if (_content != NULL)
		delete[] _content;
}

TFileType CCPCAsciiFile::getType() const
{
	return Ascii;
}

unsigned int CCPCAsciiFile::getSize() const
{
	return _size;
}

void CCPCAsciiFile::createFile(const void* i_buffer, const unsigned int i_size, bool i_createHeader)
{
	unsigned char *buffer = (unsigned char*)i_buffer;
	
	if (_content != NULL)
		delete[] _content;
	
	_size = i_size;	
	
	_content = new unsigned char[_size];
	memcpy(_content, i_buffer, _size);
}

void CCPCAsciiFile::openFile(const std::string &i_filename,bool i_createHeader)
{
	std::ifstream file(i_filename.c_str());
	
	TOOLS_ASSERTMSG( file.good() , "Unable to open file " << i_filename);
	
	file.seekg(0, std::ios::end);
	std::streampos size = file.tellg();
	file.seekg(0, std::ios::beg);
	
	TOOLS_ASSERTMSG( size < 1024*1024 , "Unable to open file " << i_filename << " file to long (>1Mo)");

	unsigned char *buffer = new unsigned char[size];
	
	file.read((char*)buffer,size);
	
	createFile(buffer,size,i_createHeader);
	
	delete[] buffer;
}

void CCPCAsciiFile::saveFile(const std::string &i_filename,bool i_saveHeader) const
{
	std::ofstream file;
	file.open(i_filename.c_str());
	
	if(file.is_open())
	{
	    for (unsigned int i=0;i<_size;i++)
		file << _content[i];
	    file.close();
	}
	else
	    cout << "Cannot write to output file !" << endl;
}

unsigned int CCPCAsciiFile::getDatasSize() const
{
	return getSize();
}

unsigned char* CCPCAsciiFile::getDatas() const
{
	return _content;
}

std::ostream& CCPCAsciiFile::printInfo(std::ostream& io_os) const
{
	io_os << "Ascii file " << std::endl;
	io_os.setf(std::ios::hex,std::ios::basefield);
	io_os.width(4);
	io_os << "Length : " << getSize() << " ";
	io_os.setf(std::ios::dec,std::ios::basefield);
	return io_os;
}

std::ostream& CCPCAsciiFile::printHeader(std::ostream& io_os) const
{
	io_os << "Header (Ascii File !)" << std::endl;
	return io_os;
}

std::ostream& CCPCAsciiFile::printContent(std::ostream& io_os) const
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
