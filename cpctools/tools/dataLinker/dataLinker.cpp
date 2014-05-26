/*
 Generateur de table de DB a partir d'un fichier binaire
 */

#include <stdlib.h>
#include <iostream>
#include <fstream>


#include <vector>
#include <algorithm>

#include "CDataLink.h"

#include "CError.h"
#include "COptionParser.h"

struct SDataLinker
{
	CDataLink data;
	int adr;

};

bool operator<(const SDataLinker &i_s, const SDataLinker &i_d)
{
	return (i_s.data.getMask() < i_d.data.getMask());
}

struct SDataZone
{
	std::string name;
	unsigned int start;
	unsigned int end;
	unsigned int bank;

	std::vector< SDataLinker > datas;

};

std::vector< SDataLinker >	linkerDataTable;
std::vector< SDataZone >	linkerZoneTable;
std::ostream				*linkerOutASM = &std::cout;

void linkData()
{
	int nbFilePlaced = 0;
	int i;
	std::vector<bool>	filePlaced(linkerDataTable.size(),false);
	std::vector<int>	zoneUsed(linkerZoneTable.size(),0); 
	
	for (i=0;i<zoneUsed.size();i++)
	{
		zoneUsed[i] = linkerZoneTable[i].start;
		linkerZoneTable[i].datas.clear();
	}

	std::sort(linkerDataTable.begin(),linkerDataTable.end());
	
	i=0;
	while (nbFilePlaced != linkerDataTable.size())
	{
		bool filePlaced = false;
		
		for (int j=0; j<linkerZoneTable.size() && !filePlaced; j++)
		{
			unsigned int start = zoneUsed[j];
			if (linkerDataTable[i].data.getSize() != 0)
			{
				start += 0xffff - linkerDataTable[i].data.getMask();
				start &= linkerDataTable[i].data.getMask();
				if (start + linkerDataTable[i].data.getSize() < linkerZoneTable[j].end)
				{
					filePlaced = true;
					linkerDataTable[i].adr = start;
					zoneUsed[j]= start + linkerDataTable[i].data.getSize();
					linkerZoneTable[j].datas.push_back(linkerDataTable[i]);
					nbFilePlaced++;
					i++;
				}
			}
			else
			{
				filePlaced = true;
				linkerDataTable[i].adr = start;
				linkerZoneTable[j].datas.push_back(linkerDataTable[i]);
				nbFilePlaced++;
				i++;
			}
		}
		if (!filePlaced)
		{
			WARNING("Unable to place " << linkerDataTable[i].data.getLabel());
			nbFilePlaced++;
		}
	}
}

void saveFile()
{
	for (int i=0; i<linkerZoneTable.size(); i++)
	{
		if (linkerZoneTable[i].datas.size() != 0)
		{
			std::ofstream data(linkerZoneTable[i].name.c_str(),std::ios::binary);
			(*linkerOutASM) << "; Bank #"<< std::hex << linkerZoneTable[i].bank << std::dec << " : Data file " << linkerZoneTable[i].name.c_str() << std::endl;
			int start = linkerZoneTable[i].start;
			for (int j=0;j<linkerZoneTable[i].datas.size();j++)
			{
				while (start < linkerZoneTable[i].datas[j].adr)
				{
					data << (char)0;
					start++;
				}
				data.write((char*)linkerZoneTable[i].datas[j].data.getDatas(),linkerZoneTable[i].datas[j].data.getSize());
				start+=linkerZoneTable[i].datas[j].data.getSize();
				(*linkerOutASM) << linkerZoneTable[i].datas[j].data.getLabel() << "\t\tEQU\t#" << std::hex << linkerZoneTable[i].datas[j].adr << std::endl;
			}
			(*linkerOutASM) << "; End data file " << linkerZoneTable[i].name.c_str() << " : #" << std::hex << start << std::endl;
		}
	}
	if (linkerOutASM != &(std::cout))
	{
		delete linkerOutASM;
	}
}

struct SOption appliOption[]=
{
	{'z',(char *)"zone",1,0,4,(char *)"Define a zone named $1 in bank $2 from $3 to $4"},
	{'d',(char *)"dataFilename",1,0,3,(char *)"Load a data file $1 with label $2 and mask $3 to be linked"},
	{'o',(char *)"output",0,1,1,(char *)"Z80 output filename"},
	{'c',(char *)"configFile",0,1,1,(char *)"Configuration file"},
	{0,NULL,0,0,0,NULL}
};

static const std::string authorName = "Ramlaid";
static const std::string authorMail = "cpcTools@ramlaid.com";
static const std::string appliName = "dataLinker";
static const std::string appliUsageShort = "";
static const std::string appliUsageLong = "\n\
Link data and create a .z80 file";

int main (int argc, char **argv)
{
	try
	{
		int i;
		COptionParser optParser(appliOption, appliName, appliUsageShort, appliUsageLong, authorName, authorMail);
		
		optParser.PrintHeader(std::cout);

		if (!optParser.ParseCommandLine(&argc, &argv, 0, true))
		{
			optParser.PrintError(std::cout);
			optParser.PrintUsage(std::cout);
			return -1;
		}
	
		i=0;
		while (optParser.GetOptionI(i) != 0)
		{
			switch(optParser.GetOptionI(i))
			{
			case 'd' :
				{
					SDataLinker file;
					std::string name = optParser.GetStringOptionI(i, 0);
					std::string label = optParser.GetStringOptionI(i, 1);
					unsigned int mask = optParser.GetIntOptionI(i, 2);
					file.data = CDataLink(name, label, mask);
					file.data.loadFile();
					file.adr = -1;
					linkerDataTable.push_back(file);
					break;
				}
			case 'z':
				{
					SDataZone zone;
					zone.name = optParser.GetStringOptionI(i, 0);
					zone.bank = optParser.GetIntOptionI(i, 1);
					zone.start = optParser.GetIntOptionI(i, 2);
					zone.end = optParser.GetIntOptionI(i, 3);
					linkerZoneTable.push_back(zone);
					break;
				}
			case 'o' : 
				{
					linkerOutASM = new std::ofstream(optParser.GetStringOptionI(i, 0).c_str());
					break;
				}
			case 'c' : {break;}
			default :
				{
					std::cerr << "Unknow option ! " << optParser.GetOptionI(i) << std::endl;
					return -1;
				}
			}
			i++;
		}
	
		linkData();
	
		saveFile();
	}
	catch(tools::CException &e)
	{
		std::cerr << e << std::endl;
		return -1;
	}

	return 0;
}
