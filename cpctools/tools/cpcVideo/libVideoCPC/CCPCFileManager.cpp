/**
 * Classe permettant la gestion d'un fichier OCP
*/                                                                                                           

#include "CCPCFileManager.h"
#include "CError.h"
#include <fstream>
#include <cstring>

//
// Gestion d'un fichier CPC
// 
/// Charge un fichier
void CCPCFileManager::LoadRawData(const string &i_filename, unsigned char* &o_data, unsigned int &o_size)
{
	ifstream file(i_filename.c_str(),ios::binary);

	TOOLS_ASSERTMSG(file.good(),"Unable to open file " << i_filename);

	file.seekg(0,ios::end);
	int size = file.tellg();
	file.seekg(0,ios::beg);

	unsigned char *data = new unsigned char[size];
	file.read((char*)data,size);

	if (CCPCFileManager::RemoveFileHeader(data,size,o_data,o_size))
	{
		//INFO("Header removed in " << i_filename);
	}
	delete[] data;
}

bool CCPCFileManager::RemoveFileHeader(unsigned char *i_data, unsigned int i_size, unsigned char* &o_data, unsigned int &o_size)
{
	bool findChecksum = false;
	if (i_size > 0x80)
	{
		unsigned int checksum = i_data[0x43]+i_data[0x44]*256;
		findChecksum = (checksum == CCPCFileManager::CheckSum(i_data));
	}

	if (!findChecksum)
	{
		o_data = new unsigned char[i_size];
		o_size = i_size;
		memcpy(o_data,i_data,o_size);
		return false;
	}
	
	o_data = new unsigned char[i_size-0x80];
	o_size = i_size-0x80;
	memcpy(o_data,i_data+0x80,o_size);

	return true;
}

unsigned int CCPCFileManager::CheckSum(const unsigned char *i_data)
{	
	unsigned int checksum=0;
	for (unsigned int i=0;i<0x42;i++)
		checksum+=i_data[i];
	return checksum;
}


//
// Gestion d'un fichier OCP
// 
bool COCPFileManager::LoadColorTable(const string &i_filename, int &o_mode, unsigned int o_colorTable[16])
{
	unsigned char *data;
	unsigned int size;

	CCPCFileManager::LoadRawData(i_filename,data,size);

	bool read = ReadColorTable(data,size,o_mode,o_colorTable);

	delete[] data;
	return read;
}
bool COCPFileManager::LoadScreen(const string &i_filename, unsigned char* &o_data, unsigned int &o_size)
{
	unsigned char *data;
	unsigned int size;

	CCPCFileManager::LoadRawData(i_filename,data,size);

	bool read = ReadScreen(data,size,o_data,o_size);

	delete[] data;
	return read;
}
bool COCPFileManager::LoadWindow(const string &i_filename, unsigned char* &o_data, unsigned int &o_width, unsigned int &o_widtho, unsigned int &o_height)
{
	unsigned char *data;
	unsigned int size;

	CCPCFileManager::LoadRawData(i_filename,data,size);

	bool read = ReadWindow(data,size,o_data,o_width,o_widtho,o_height);

	delete[] data;
	return read;
}

bool COCPFileManager::ReadColorTable(unsigned char *i_data, unsigned int i_size, int &o_mode, unsigned int o_colorTableHard[16])
{
	if (i_size != (3+16*12+44))
		return false;

	int p=0;

	o_mode = i_data[p++];
	p++;
	p++;
	
	for (int i=0;i<16;i++)
	{
		int c = i_data[p++];
		o_colorTableHard[i]=c-64;
		p+= 11;
	}

	return true;
}
bool COCPFileManager::ReadScreen(unsigned char *i_data, unsigned int i_size, unsigned char* &o_data, unsigned int &o_size)
{
	int piData, poData;
	unsigned int iSize,oSize,rSize;

	o_size = 0x4000;
	o_data = new unsigned char[o_size];
	memset(o_data,0,o_size);	

	piData = 0;
	poData = 0;
	iSize = i_size;

	if (ReadCompressedBlock(i_data+piData,iSize,rSize,o_data+poData,oSize))
	{
		piData+=rSize;
		iSize-=rSize;
		poData+=oSize;
		while (iSize > 0)
		{
			if (ReadCompressedBlock(i_data+piData,iSize,rSize,o_data+poData,oSize))
			{
				piData+=rSize;
				iSize-=rSize;
				poData+=oSize;
			}
		}
		return true;
	}

	memcpy(o_data,i_data,(i_size<o_size) ? i_size : o_size);

	return false;
}
bool COCPFileManager::ReadWindow(unsigned char *i_data, unsigned int i_size, unsigned char* &o_data, unsigned int &o_width, unsigned int &o_widtho, unsigned int &o_height)
{
	int piData, poData;
	unsigned int iSize,oSize,rSize;

	unsigned int outSize;

	piData = i_size-4;
	o_width = (int)i_data[piData++];
	o_width += (int)i_data[piData++] * 256;

	o_widtho = ((o_width % 8) == 0) ? (o_width / 8) : ((o_width/8) + 1);

	o_height = (int)i_data[piData++];

	outSize = o_widtho * o_height;
	if (outSize > 0x4000)
	{
		o_data = new unsigned char[i_size];
		memcpy(o_data,i_data,i_size);
		o_width = 1;
		o_widtho = 1;
		o_height = 1;
		return false;
	}
	o_data = new unsigned char[outSize];
	
	memset(o_data,0,outSize);
	poData = 0;
	piData = 0;
	iSize = i_size-5;

	if (ReadCompressedBlock(i_data+piData,iSize,rSize,o_data+poData,oSize))
	{
		piData+=rSize;
		iSize-= (rSize < iSize) ? rSize : iSize;
		poData+=oSize;
		while (iSize > 0)
		{
			if (ReadCompressedBlock(i_data+piData,iSize,rSize,o_data+poData,oSize))
			{
				piData+=rSize;
				iSize-=rSize;
				poData+=oSize;
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	if (i_size-5 != outSize)
	{
		unsigned char *oldData = o_data;
		delete[] oldData;
		o_data = new unsigned char[i_size];
		memcpy(o_data,i_data,i_size);
		o_width = 2*8;
		o_widtho = 2;
		o_height = 8;
		return false;
	}

	memcpy(o_data,i_data,outSize);
	
	return true;
}

bool COCPFileManager::ReadCompressedBlock(unsigned char *i_data, unsigned int i_size, unsigned int &o_readData, unsigned char* o_data,unsigned int &o_size)
{
	int p=0;
	
	if (strncmp("MJH",(char*)i_data,3) == 0)
	{
		p+=3;
		o_size = i_data[p++];
		o_size += i_data[p++]*256;

		unsigned int i = 0;
		while (i<o_size)
		{
			unsigned char m,c,b;
			m = i_data[p++];
			if (m == 1)
			{
				c = i_data[p++];
				b = i_data[p++];
				int nb = ((unsigned int)c == 0) ? 256 : (unsigned int)c;
				while (i<o_size && nb!=0)
				{
					o_data[i++] = b;
					nb--;
				}
			}
			else
			{
				o_data[i++] = m;
			}
		}
		o_readData = p;
		return true;
	}

	return false;
}
