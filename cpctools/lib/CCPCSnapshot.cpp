/**
* @class CCPCSnapshot
* Classe permettant la gestion d'un snapshot CPC
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#include "CCPCSnapshot.h"

#include <algorithm>
#include <cstring>

#include <stdint.h>
#include <stdio.h>

#include <CCPCBinaryFile.h>

static const int SNAPSHOT_ID = 0x00;
static const int SNAPVERS_ID = 0x10;
static const int MEM_SIZE_ID = 0x6b;

const struct CCPCSnapshot::TTokenID CCPCSnapshot::TokenID[NB_SNAPSHOT_TOKEN] = 
{
	{0x11, 2, "Z80_AF",		"\t\tZ80 register AF"},
	{0x11, 1, "Z80_F",		"\t\tZ80 register F"},
	{0x12, 1, "Z80_A",		"\t\tZ80 register A"},
	{0x13, 2, "Z80_BC",		"\t\tZ80 register BC"},
	{0x13, 1, "Z80_C",		"\t\tZ80 register C"},
	{0x14, 1, "Z80_B",		"\t\tZ80 register B"},
	{0x15, 2, "Z80_DE",		"\t\tZ80 register DE"},
	{0x15, 1, "Z80_E",		"\t\tZ80 register E"},
	{0x16, 1, "Z80_D",		"\t\tZ80 register D"},
	{0x17, 2, "Z80_HL",		"\t\tZ80 register HL"},
	{0x17, 1, "Z80_L",		"\t\tZ80 register L"},
	{0x18, 1, "Z80_H",		"\t\tZ80 register H"},
	{0x19, 1, "Z80_R",		"\t\tZ80 register R"},
	{0x1a, 1, "Z80_I",		"\t\tZ80 register I"},
	{0x1b, 1, "Z80_IFF0",	"\tZ80 interrupt flip-flop IFF0"},
	{0x1c, 1, "Z80_IFF1",	"\tZ80 interrupt flip-flop IFF1"},
	{0x1d, 2, "Z80_IX",		"\t\tZ80 register IX"},
	{0x1d, 1, "Z80_IXL",	"\t\tZ80 register IX (low)"},
	{0x1e, 1, "Z80_IXH",	"\t\tZ80 register IX (high)"},
	{0x1f, 2, "Z80_IY",		"\t\tZ80 register IY"},
	{0x1f, 1, "Z80_IYL",	"\t\tZ80 register IY (low)"},
	{0x20, 1, "Z80_IYH",	"\t\tZ80 register IY (high)"},
	{0x21, 2, "Z80_SP",		"\t\tZ80 register SP"},
	{0x23, 2, "Z80_PC",		"\t\tZ80 register PC"},
	{0x25, 1, "Z80_IM",		"\t\tZ80 interrupt mode (0,1,2)"},
	{0x26, 2, "Z80_AFX",	"\t\tZ80 register AF'"},
	{0x26, 1, "Z80_FX",		"\t\tZ80 register F'"},
	{0x27, 1, "Z80_AX",		"\t\tZ80 register A'"},
	{0x28, 2, "Z80_BCX",	"\t\tZ80 register BC'"},
	{0x28, 1, "Z80_CX",		"\t\tZ80 register C'"},
	{0x29, 1, "Z80_BX",		"\t\tZ80 register B'"},
	{0x2a, 2, "Z80_DEX",	"\t\tZ80 register DE'"},
	{0x2a, 1, "Z80_EX",		"\t\tZ80 register E'"},
	{0x2b, 1, "Z80_DX",		"\t\tZ80 register D'"},
	{0x2c, 2, "Z80_HLX",	"\t\tZ80 register HL'"},
	{0x2c, 1, "Z80_LX",		"\t\tZ80 register L'"},
	{0x2d, 1, "Z80_HX",		"\t\tZ80 register H'"},
	{0x2e, 1, "GA_PEN",		"\t\tGA: index of selected pen"},
	{0x2f, 17,"GA_PAL",		"\t\tGA: current palette (0..16)"},
	{0x40, 1, "GA_ROMCFG",	"\tGA: multi configuration"},
	{0x41, 1, "GA_RAMCFG",	"\tCurrent RAM configuration"},
	{0x42, 1, "CRTC_SEL",	"\tCRTC: index of selected register"},
	{0x43, 18,"CRTC_REG",	"\tCRTC: register data (0..17)"},
	{0x55, 1, "ROM_UP",		"\t\tCurrent ROM selection"},
	{0x56, 1, "PPI_A",		"\t\tPPI: port A"},
	{0x57, 1, "PPI_B",		"\t\tPPI: port B"},
	{0x58, 1, "PPI_C",		"\t\tPPI: port C"},
	{0x59, 1, "PPI_CTL",	"\t\tPPI: control port"},
	{0x5a, 1, "PSG_SEL",	"\t\tPSG: index of selected register"},
	{0x5b, 16,"PSG_REG",	"\t\tPSG: register data (0..15)"},
	{0x6d, 1, "CPC_TYPE",	"\tCPC type: \n\t\t\t0 = CPC464\n\t\t\t1 = CPC664\n\t\t\t2 = CPC6128\n\t\t\t3 = unknown\n\t\t\t4 = 6128 Plus\n\t\t\t5 = 464 Plus\n\t\t\t6 = GX4000"},
	{0x6e, 1, "INT_NUM",	"\tinterrupt number (0..5)"},
	{0x6f, 6, "GA_MULTIMODE",	"\t6 mode bytes (one for each halt)"},
	{0x9c, 1, "FDD_MOTOR",	"\tFDD motor drive state (0=off, 1=on)"},
	{0x9d, 1, "FDD_TRACK",	"\tFDD current physical track"},
	{0xa1, 1, "PRNT_DATA",	"\tPrinter Data/Strobe Register"},
	{0xa4, 1, "CRTC_TYPE",	"\tCRTC type:\n\t\t\t0 = HD6845S/UM6845\n\t\t\t1 = UM6845R\n\t\t\t2 = MC6845\n\t\t\t3 = 6845 in CPC+ ASIC\n\t\t\t4 = 6845 in Pre-ASIC"},
	{0xa9, 1, "CRTC_HCC",	"\tCRTC horizontal character counter register"},
	{0xab, 1, "CRTC_CLC",	"\tCRTC character-line counter register"},
	{0xac, 1, "CRTC_RLC",	"\tCRTC raster-line counter register"},
	{0xad, 1, "CRTC_VAC",	"\tCRTC vertical total adjust counter register"},
	{0xae, 1, "CRTC_VSWC",	"\tCRTC horizontal sync width counter"},
	{0xaf, 1, "CRTC_HSWC",	"\tCRTC vertical sync width counter"},
	{0xb0, 2, "CRTC_STATE",	"\tCRTC state flags. \n\t\t\t0 if '1'/'0' VSYNC active/inactive\n\t\t\t1 if '1'/'0' HSYNC active/inactive\n\t\t\t2-7 reserved\n\t\t\t7 if '1'/'0' Vert Total Adjust active/inactive\n\t\t\t8-15 reserved (0)"},
	{0xb2, 1, "GA_VSC",		"\t\tGA vsync delay counter"},
	{0xb3, 1, "GA_ISC",		"\t\tGA interrupt scanline counter"},
	{0xb4, 1, "INT_REQ",	"\t\tInterrupt request flag\n\t\t\t0=no interrupt requested\n\t\t\t1=interrupt requested"},
};

const char *CCPCSnapshot::TokenSection[NB_SNAPSHOT_TOKEN_SECTION] =
{
	"CPC",
	"Z80",
	"CRTC",
	"GA",
	"ROM",
	"INT",
	"PPI",
	"PSG",
	"FDD",
	"PRNT"
};

unsigned char CCPCSnapshot::DefaultHeader[0x100] =
{
    0x4D, 0x56, 0x20, 0x2D, 0x20, 0x53, 0x4E, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xc0, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x0C, 
    0x8D, 0xC0, 0x00, 0x3F, 0x28, 0x2E, 0x8E, 0x26, 0x00, 0x19, 0x1E, 0x00, 0x07, 0x00, 0x00, 0x30, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x1E, 0x00, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x02, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x08, 0x02, 0x00, 0x04, 0x00, 
    0x01, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
} ;


CCPCSnapshot::CCPCSnapshot()
:_header(),
_memoryDump(NULL),
_nbChunk(0),
_chunkName(NULL),
_chunkSize(NULL),
_chunkData(NULL)
{
	memcpy(_header,CCPCSnapshot::DefaultHeader,0x100);
	_memoryDump = new unsigned char[_header[MEM_SIZE_ID]*1024];
	memset(_memoryDump, _header[MEM_SIZE_ID]*1024, 0);
}

CCPCSnapshot::~CCPCSnapshot()
{
	if (_memoryDump != NULL)
		delete[] _memoryDump;
	if (_nbChunk != 0)
    {
		for (unsigned int i=0;i<_nbChunk;i++)
		{
			delete[] _chunkName[i];
			delete[] _chunkData[i];
		}
		delete[] _chunkName;
		delete[] _chunkSize;
		delete[] _chunkData;
    }
}

void CCPCSnapshot::loadSnapshotIni(const string &filename)
{
	ifstream iniFile(filename.c_str());
	TOOLS_ASSERTMSG( iniFile.good(), "Unable to open ini file " << filename);

	int lineIdx = 0;
	string currentSection = "";
	while (!iniFile.eof())
	{
		lineIdx++;

		char cline[1024];
		iniFile.getline(cline, 1024);
		string line = cline;

		line.erase(remove(line.begin(), line.end(), '\t'), line.end() );
		line.erase(remove(line.begin(), line.end(), ' '), line.end() );
		if (line[0] == ';')
			continue;

		if (line[0] == '[')
		{
			TOOLS_ASSERTMSG(line.find(']') != string::npos, "Wrong ini start section " << cline << " at line " << lineIdx);
			currentSection = line.substr(1, line.find(']')-1);
		}
		else
		{
			if (line.find('=') == string::npos)
				continue;
			
			string tokenName = line.substr(0, line.find('='));
			tokenName = currentSection + "_" + tokenName;
			int tokenIdx = GetTokenIndex(tokenName);
			if (tokenIdx == -1)
				continue;

			const TTokenID &token = TokenID[tokenIdx];
			string value = line.substr(line.find('=')+1, line.size() - line.find('=') - 1);
			if (token.Size == 1)
			{
				int val = strtol(value.c_str(),NULL,0);
				TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
				_header[token.Offset] = (unsigned char)val;
			}
			else if (token.Size == 2)
			{
				int val = strtol(value.c_str(),NULL,0);
				TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
				_header[token.Offset] = (unsigned char)(val % 256);
				_header[token.Offset+1] = (unsigned char)(val >> 8);
			}
			else
			{
				string strVal;
				size_t start = 0;
				size_t pos = value.find(',', start);
				unsigned int idx = 0;
				while (pos != string::npos && idx < token.Size)
				{
					strVal = value.substr(start, pos - start);
					int val = strtol(strVal.c_str(),NULL,0);
					TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
					_header[token.Offset+idx] = (unsigned char)val;
					start = pos+1;
					pos = value.find(',', start);
					idx++;
				}
				if (pos-start > 1 && idx < token.Size)
				{
					strVal = value.substr(start, pos - start);
					int val = strtol(strVal.c_str(),NULL,0);
					TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
					_header[token.Offset+idx] = (unsigned char)val;
				}
			}
		}
	}
}
void CCPCSnapshot::saveSnapshotIni(const string &filename) const
{
	ofstream iniFile(filename.c_str());
	TOOLS_ASSERTMSG( iniFile.good(), "Unable to create ini file " << filename);

	for (int s=0 ; s<NB_SNAPSHOT_TOKEN_SECTION ; s++)
	{
		string sectionName = TokenSection[s];
		iniFile << "[" << sectionName << "]" << endl;
		for (int t=0 ; t<NB_SNAPSHOT_TOKEN ; t++)
		{
			const TTokenID &token = TokenID[t];
			string tokenName = token.ID;
			if (tokenName.find(sectionName) != string::npos)
			{
				tokenName = tokenName.substr(sectionName.size()+1, tokenName.size() - sectionName.size() - 1);

				string desc = token.Description;
				replace(desc.begin(), desc.end(), '\n', ' ');
				desc.erase(remove(desc.begin(), desc.end(), '\t'), desc.end() );

				iniFile << "; " << desc << endl;

				iniFile << tokenName << "\t= ";

				if (token.Size == 1)
				{
					iniFile << "0x";
					iniFile.setf(std::ios::hex, std::ios::basefield);
					iniFile.fill('0');
					iniFile.width(2);
					iniFile << (int)_header[token.Offset];
				}
				else if (token.Size == 2)
				{
					int val = (_header[token.Offset]+_header[token.Offset+1]*256);
					iniFile << "0x";
					iniFile.setf(std::ios::hex, std::ios::basefield);
					iniFile.fill('0');
					iniFile.width(4);
					iniFile << val;
				}
				else
				{
					for (unsigned int v=0 ; v<token.Size ; v++)
					{
						iniFile << "0x";
						iniFile.setf(std::ios::hex, std::ios::basefield);
						iniFile.fill('0');
						iniFile.width(2);
						iniFile << (int)_header[token.Offset+v];
						if (v != token.Size-1)
						{
							iniFile << ", ";
						}
					}
				}
				iniFile << endl;
			}
		}
		iniFile << endl;
	}
}

CCPCSnapshot::TTokenID CCPCSnapshot::GetToken(const char *i_dataID, int &o_index)
{
	std::string token(i_dataID);
	unsigned int p;
	o_index = -1;
	if ((p=token.find(':')) != std::string::npos)
	{
		std::string index = token.substr(p+1,token.size()-(p+1));

		o_index = strtol(index.c_str(),NULL,0);
		TOOLS_ASSERTMSG(errno != ERANGE, "Wrong index value " << index);
		token = token.substr(0,p);
	}
	char cToken[256];
	strcpy(cToken,token.c_str());
#if _WINDOWS
	_strupr(cToken);
#else
	int index=0 ;
	while (index<256 && cToken[index]){
		cToken[index] = toupper(cToken[index]) ;
		index++;
	}
#endif
	token=cToken;

	int tidx = GetTokenIndex(token);

	TOOLS_ASSERTMSG((tidx != -1), "Unknown token " << token);
	TOOLS_ASSERTMSG((o_index < (int)CCPCSnapshot::TokenID[tidx].Size && o_index >= 0) || (o_index == -1),
		"Wrong token array index " << o_index << " [0.." << CCPCSnapshot::TokenID[tidx].Size << "] for token " << token);

	return CCPCSnapshot::TokenID[tidx];
}

int CCPCSnapshot::GetTokenIndex(const string &tokenName)
{
	for (int t=0 ; t<NB_SNAPSHOT_TOKEN ; t++)
	{
		if (tokenName == CCPCSnapshot::TokenID[t].ID)
		{
			return t;
		}
	}
	return -1;
}

int CCPCSnapshot::GetTokenOffset(const char *i_dataID)
{
	int idx;
	CCPCSnapshot::TTokenID t = GetToken(i_dataID,idx);
	if (idx != -1)
		return t.Offset + idx;
	else
		return t.Offset;
}

int CCPCSnapshot::GetTokenSize(const char *i_dataID)
{
	int idx;
	CCPCSnapshot::TTokenID t = GetToken(i_dataID,idx);
	if (idx != -1)
		return 1;
	else
		return t.Size;
}

// Renvoie la byte valeur data
unsigned char CCPCSnapshot::tokenByteValue(const char *i_dataID) const
{
	int o = GetTokenOffset(i_dataID);
	return _header[o];
}
// Renvoie la short valeur data
unsigned short CCPCSnapshot::tokenShortValue(const char *i_dataID) const
{
	int o = GetTokenOffset(i_dataID);
	return (_header[o]+_header[o+1]*256);
}

void CCPCSnapshot::setTokenValue(const char *i_dataID,int val)
{
	int o = GetTokenOffset(i_dataID);
	int s = GetTokenSize(i_dataID);
	if (s == 1)
	{
		_header[o] = (unsigned char)val;
	}
	else
	{
		TOOLS_ASSERT((s == 2));
		_header[o] = (unsigned char)(val % 256);
		_header[o+1] = (unsigned char)(val >> 8);
	}
}

std::ostream& CCPCSnapshot::displayTokenHelp(std::ostream &io_os)
{
	io_os << "Snapshot settable token list :" << std::endl << std::endl;
	io_os << "TOKEN\t\tSize\tDescription" << std::endl << std::endl;
	for (int i=0;i<NB_SNAPSHOT_TOKEN;i++)
	{
		io_os << CCPCSnapshot::TokenID[i].ID << CCPCSnapshot::TokenID[i].Description << std::endl;
	}
	return io_os;
}

/// Renvoie la version du snapshot
unsigned int CCPCSnapshot::getSnapshotVersion() const
{
	unsigned char *h=(unsigned char*)&_header;
	return (unsigned int)h[0x10];
}

/// Efface la memore du snapshot
void CCPCSnapshot::clearMemory()
{
	for (int i=0;i<(_header[MEM_SIZE_ID] * 1024);i++)
    {
		_memoryDump[i] = 0;
    }
}

void CCPCSnapshot::romDeconnect()
{
	// On met les bit 2 et 3 a 1, ca deconnecte les roms 0000-3fff et c000-ffff
	int o = GetTokenOffset("GA_ROMCFG");
	_header[o] = _header[o] | 0xC;
}

void CCPCSnapshot::enableInterrupt()
{
	_header[GetTokenOffset("Z80_IFF0")] = 1;
	_header[GetTokenOffset("Z80_IFF1")] = 1;
}
void CCPCSnapshot::disableInterrupt()
{
	_header[GetTokenOffset("Z80_IFF0")] = 0;
	_header[GetTokenOffset("Z80_IFF1")] = 0;
}

void CCPCSnapshot::loadFromFile(const std::string &i_filename)
{
	std::ifstream file;
	
	if (_memoryDump != NULL)
    {
		delete[] _memoryDump;
		_memoryDump = NULL;
    }
	if (_nbChunk != 0)
    {
		for (unsigned int i=0;i<_nbChunk;i++)
		{
			delete[] _chunkName[i];
			delete[] _chunkData[i];
		}
		delete[] _chunkName;
		delete[] _chunkSize;
		delete[] _chunkData;
		_nbChunk = 0;
		_chunkName = NULL;
		_chunkSize = NULL;
		_chunkData = NULL;
    }
	
	file.open(i_filename.c_str(),std::ios::in | std::ios::binary);
	
	TOOLS_ASSERTMSG(file.good(),"Couldn't open \"" << i_filename << "\" snapshot file");

	file.read((char*) &_header,0x100);
	
	unsigned int size = _header[MEM_SIZE_ID] * 1024;
	
	_memoryDump = new unsigned char[size];
	
	file.read((char*)_memoryDump,size);
	
	if (!file.eof())
    {
		unsigned int nbChunk=0;
		std::streampos startChunk = file.tellg();
		file.seekg(0,std::ios::end);
		std::streampos endChunk = file.tellg();
		file.seekg(startChunk,std::ios::beg);
		// On compte le nombre de chunk
		while (file.tellg() != endChunk)
		{
			unsigned char data[4];
			unsigned int size;
			file.read((char*)data,4);
			file.read((char*)data,4);
			size = data[3]*256*256*256 + data[2]*256*256 + data[1]*256 + data[0];
			file.seekg(size,std::ios::cur);
			nbChunk++;
		}
		_nbChunk = nbChunk;
		_chunkName = new unsigned char*[_nbChunk];
		_chunkSize = new unsigned int[_nbChunk];
		_chunkData = new unsigned char*[_nbChunk];
		file.seekg(startChunk,std::ios::beg);
		for (unsigned int i=0;i<_nbChunk;i++)
		{
			unsigned char data[4];
			_chunkName[i] = new unsigned char[5];
			file.read((char*)_chunkName[i],4);
			_chunkName[i][4] = 0;
			file.read((char*)data,4);
			_chunkSize[i] = data[3]*256*256*256 + data[2]*256*256 + data[1]*256 + data[0];
			_chunkData[i] = new unsigned char[_chunkSize[i]];
			file.read((char*)_chunkData[i],_chunkSize[i]);
		}
    }
}

void CCPCSnapshot::saveToFile(const std::string &i_filename) const
{
	std::ofstream file;
	file.open(i_filename.c_str(),std::ios::out | std::ios::binary);
	
	file.write((char*)&_header,0x100);
	TOOLS_ASSERTMSG(file.good(),"Couldn't save \"" << i_filename << "\" snapshot file");

	unsigned int size = _header[MEM_SIZE_ID] * 1024;
	
	file.write((char*)_memoryDump,size);
	
	for (unsigned int i=0;i<_nbChunk;i++)
    {
		unsigned char data[4];
		file.write((char*)_chunkName[i],4);
		data[0] = (unsigned char)(_chunkSize[i] % 256);
		data[1] = (unsigned char)((_chunkSize[i] >> 8) % 256);
		data[2] = (unsigned char)((_chunkSize[i] >> 16) % 256);
		data[3] = (unsigned char)((_chunkSize[i] >> 24) % 256);
		file.write((char*)data,4);
		file.write((char*)_chunkData[i],_chunkSize[i]);
    }
	file.close();
}

/// Ajoute des donnees dans le snapshot a partir d'un fichier, et d'une adresse
void CCPCSnapshot::loadDataFromFile(const std::string &i_filename,int i_adress)
{
	static const int kAMSDOSHeaderSize = 128;

	std::ifstream file;
	int size;
	file.open(i_filename.c_str(),std::ios::in | std::ios::binary);
	
	TOOLS_ASSERTMSG(file.good(),"Couldn't open \"" << i_filename << "\" data file");

	std::streampos start = file.tellg();
	file.seekg(0,std::ios::end);
	std::streampos end = file.tellg();

	if(i_adress < 0)
	{
		static const int kLoadAddressOffset = 21;
		static const int kExecAddressOffset = 26;
		file.seekg(kLoadAddressOffset, std::ios::beg);
		i_adress = 0;
		file.read((char*)&i_adress, 2); // warning: not endian-safe...

		file.seekg(kExecAddressOffset, std::ios::beg);
		uint16_t entry;
		file.read((char*)&entry, 2); // warning: not endian-safe...
		setTokenValue("Z80_PC", entry);

		start = kAMSDOSHeaderSize; // Skip AMSDOS header
	} else {
		// Check if the file has a valid AMSDOS header, and if that's the case,
		// skip it!
		file.seekg(0, std::ios::beg);
		unsigned char buffer[69];
		file.read((char*)buffer, 69);
		if(CCPCBinaryFile::isBinary(buffer, -1)) {
			start = kAMSDOSHeaderSize; // Skip AMSDOS header
		}
	}

	file.seekg(start,std::ios::beg);
	size = end - start;
	
	TOOLS_ASSERTMSG(((i_adress+size) < ((int)_header[MEM_SIZE_ID] * 1024)), "Data too long, over snapshot size");
	
	
	file.read((char*)(_memoryDump+i_adress) , size);
	
	file.close();
	
}

/// Sauve des donnees dans le fichier a partir du snapshot d'une adresse et d'une taille
void CCPCSnapshot::saveDataToFile(const std::string &i_filename,const int i_adress, const int i_length) const
{
	std::ofstream file;
	
	TOOLS_ASSERTMSG( ((i_adress+i_length) <= ((int)_header[MEM_SIZE_ID] * 1024)) , "Data too long, over snapshot size");
	
	file.open(i_filename.c_str(),std::ios::out | std::ios::binary);
	TOOLS_ASSERTMSG(file.good(),"Couldn't save \"" << i_filename << "\" data file");
	
	file.write((char*)(_memoryDump+i_adress) , i_length);
	
	file.close();
	
}

void CCPCSnapshot::putData(unsigned int adr,unsigned char val)
{
	TOOLS_ASSERTMSG(adr < (_header[MEM_SIZE_ID] * 1024), "Address #" << std::hex << adr << " outside snapshot memory" << std::dec);
	*(_memoryDump+adr) = val;
}
void CCPCSnapshot::fillData(unsigned int adr, unsigned int size, const vector<unsigned char> &datas)
{
	TOOLS_ASSERTMSG(adr < (_header[MEM_SIZE_ID] * 1024), "Start address #" << std::hex << adr << " outside snapshot memory" << std::dec);
	TOOLS_ASSERTMSG((adr+size) < (_header[MEM_SIZE_ID] * 1024), "End address #" << std::hex << adr << " outside snapshot memory" << std::dec);

	unsigned char *destAdr;
	unsigned char *startAdr = _memoryDump + adr;
	unsigned char *endAdr = _memoryDump + adr + size;
	int dataIdx = 0;
	for (destAdr = startAdr ; destAdr < endAdr ; destAdr++)
	{
		(*destAdr) = datas[dataIdx];
		dataIdx = (dataIdx + 1) % datas.size();
	}
}

/// Affiche les infos sur le Snapshot
std::ostream& CCPCSnapshot::printSnapshotInfo(std::ostream &io_os) const
{
	io_os << "SNAPSHOT" << std::endl;
	io_os << "SnapshotID : " << (char*)&(_header[SNAPSHOT_ID]) << std::endl;
	io_os << "Version : " << (unsigned int)_header[SNAPVERS_ID] << std::endl;
	io_os << "Memory Dump Size " << std::hex << (unsigned int)(_header[MEM_SIZE_ID]*1024) << std::dec << " (" << (unsigned int)(_header[MEM_SIZE_ID]) << "Ko)" << std::endl;
	return io_os;
}
/// Affiche les infos sur le Z80
std::ostream& CCPCSnapshot::printZ80Info(std::ostream &io_os) const
{
	io_os << "Z80 State" << std::endl;
	io_os << std::hex << "AF\t#";io_os.fill('0');io_os.width(4);io_os << tokenShortValue("Z80_AF") << "\n";
	io_os << "BC\t#";io_os.fill('0');io_os.width(4);io_os << tokenShortValue("Z80_BC") << "\n";
	io_os << "DE\t#";io_os.fill('0');io_os.width(4);io_os << tokenShortValue("Z80_DE") << "\n";
	io_os << "HL\t#";io_os.fill('0');io_os.width(4);io_os << tokenShortValue("Z80_HL") << "\n";
	io_os << "AF'\t#";io_os.fill('0');io_os.width(4);io_os << tokenShortValue("Z80_AFa") << "\n";
	io_os << "BC'\t#";io_os.fill('0');io_os.width(4);io_os << tokenShortValue("Z80_BCa") << "\n";
	io_os << "DE'\t#";io_os.fill('0');io_os.width(4);io_os << tokenShortValue("Z80_DEa") << "\n";
	io_os << "HL'\t#";io_os.fill('0');io_os.width(4);io_os << tokenShortValue("Z80_HLa") << "\n";
	io_os << "IX\t#";io_os.fill('0');io_os.width(4);io_os << tokenShortValue("Z80_IX") << "\n";
	io_os << "IY\t#";io_os.fill('0');io_os.width(4);io_os << tokenShortValue("Z80_IY") << "\n";
	io_os << "SP\t#";io_os.fill('0');io_os.width(4);io_os << tokenShortValue("Z80_SP") << "\n";
	io_os << "PC\t#";io_os.fill('0');io_os.width(4);io_os << tokenShortValue("Z80_PC") << "\n";
	io_os << "I\t#";io_os.fill('0');io_os.width(2);io_os << (int)tokenByteValue("Z80_I") << "\n";
	io_os << "R\t#";io_os.fill('0');io_os.width(2);io_os << (int)tokenByteValue("Z80_I") << "\n";
	io_os << "Interrupt Mode(" << (int)tokenByteValue("Z80_IM") << "), IFF0(" << (int)tokenByteValue("Z80_IFF0") << "), IFF1(" << (int)tokenByteValue("Z80_IFF1") << ")" << std::endl;
	io_os << std::dec;
	return io_os;
}
/// Affiche les infos sur le GateArray
std::ostream& CCPCSnapshot::printGateArrayInfo(std::ostream &io_os) const
{
	io_os << "GateArray" << std::endl;
	io_os << "Selected Pen " << (int)tokenByteValue("GA_PEN") << std::endl;
	io_os << "Pen\tVal\n";
	int o = GetTokenOffset("GA_PAL");
	for (int i=0;i<17;i++)
	{
		io_os  << i << "\t" << (unsigned int)_header[o+i] << "\n";
	}
	io_os << std::endl;
	return io_os;
}
/// Affiche les infos sur le CRTC
std::ostream& CCPCSnapshot::printCRTCInfo(std::ostream &io_os) const
{
	io_os << "CRTC" << std::endl;
	io_os << "Selected Reg " << (int)tokenByteValue("CRTC_SEL") << std::endl;
	io_os << "Reg\tVal\n";
	int o = GetTokenOffset("CRTC_REG");
	for (int i=0;i<18;i++)
	{
		io_os  << i << "\t" << (unsigned int)_header[o+i] << "\n";
	}
	io_os << std::endl;
	return io_os;
}
/// Affiche les infos sur le PPI
std::ostream& CCPCSnapshot::printPPIInfo(std::ostream &io_os) const
{
	io_os << "PPI" << std::endl;
	io_os << std::hex;
	io_os << "PortA : #" << (int)tokenByteValue("PPI_A") << std::endl;
	io_os << "PortB : #" << (int)tokenByteValue("PPI_B") << std::endl;
	io_os << "PortC : #" << (int)tokenByteValue("PPI_C") << std::endl;
	io_os << "Control : #" << (int)tokenByteValue("PPI_CTL") << std::endl;
	io_os << std::dec;
	return io_os;
}
/// Affiche les infos sur le PSG
std::ostream& CCPCSnapshot::printPSGInfo(std::ostream &io_os) const
{
	io_os << "PSG" << std::endl;
	io_os << "Selected Reg " << (int)tokenByteValue("PSG_SEL") << std::endl;
	io_os << "Reg\tVal\n";
	int o = GetTokenOffset("PSG_REG");
	for (int i=0;i<14;i++)
	{
		io_os  << i << "\t" << (unsigned int)_header[o+i] << "\n";
	}
	io_os << std::endl;
	return io_os;
}
/// Affiche les infos sur la RAM/ROM
std::ostream& CCPCSnapshot::printRAMROMInfo(std::ostream &io_os) const
{
	io_os << "RAM/ROM" << std::endl;
	io_os << std::hex;
	io_os << "RomConfig (" << (int)tokenByteValue("ROM_CFG") << ")" << std::endl;
	io_os << "RamConfig (" << (int)tokenByteValue("RAM_CFG") << ")" << std::endl; 
	io_os << "UpperRom (" << (int)tokenByteValue("ROM_UP") << ")" << std::endl;
	io_os << std::dec;
	return io_os;
}
/// Affiche les infos Chunk
std::ostream& CCPCSnapshot::printChunkInfo(std::ostream &io_os) const
{ 
	io_os << "SNAPSHOT_CHUNK" << std::endl;
	io_os << "NbChunk : " << _nbChunk << std::endl;
	for (unsigned int i=0;i<_nbChunk;i++)
		io_os << "Chunk " << _chunkName[i] << " : " << std::hex << _chunkSize[i] << std::dec << std::endl;
	return io_os;
}

std::ostream& operator<<(std::ostream &io_os,const CCPCSnapshot &i_snap)
{
	i_snap.printSnapshotInfo(io_os);io_os << std::endl;
	i_snap.printZ80Info(io_os);io_os << std::endl;
	i_snap.printGateArrayInfo(io_os);io_os << std::endl;
	i_snap.printCRTCInfo(io_os);io_os << std::endl;
	i_snap.printPPIInfo(io_os);io_os << std::endl;
	i_snap.printPSGInfo(io_os);io_os << std::endl;
	i_snap.printRAMROMInfo(io_os);io_os << std::endl;
	i_snap.printChunkInfo(io_os);
	
	return io_os;
}
