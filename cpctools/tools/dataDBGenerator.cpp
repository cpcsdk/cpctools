/*
 Generateur de table de DB a partir d'un fichier binaire
 */

#include <iostream>
#include <fstream>
#include "COptionParser.h"
#include "CError.h"

struct SOption appliOption[]=
{
	{'n',(char *)"noHeader",0,1,0,(char *)"Do not add header"},
	{'s',(char *)"lineSize",0,1,1,(char *)"Number of bytes per line (default 8)"},
	{'d',(char *)"decimal",0,1,0,(char *)"Convert in decimal values"},
	{'b',(char *)"binary",0,1,0,(char *)"Convert in bynary values"},
	{'p',(char *)"prefixChar",0,1,1,(char *)"Prefix character before value (default : '#' for hex, '%' for bin)"},
	{0,NULL,0,0,0,NULL}
};
static const std::string authorName = "Ramlaid";
static const std::string authorMail = "cpcTools@ramlaid.com";
static const std::string appliName = "dataDBGenerator";
static const std::string appliUsageShort = "<file.bin> <file.z80>";
static const std::string appliUsageLong = "<file.bin> <file.z80>\n\
Create a source file with defb given a binary file";

enum ValueMode
{
	Hexadecimal,
	Decimal,
	Binary
};

int main (int argc, char **argv)
{
	try
	{
		COptionParser optParser(appliOption, appliName, appliUsageShort, appliUsageLong, authorName, authorMail);
		
		optParser.PrintHeader(std::cout);

		if (!optParser.ParseCommandLine(&argc, &argv, 2, false))
		{
			optParser.PrintError(std::cout);
			optParser.PrintUsage(std::cout);
			return -1;
		}
	
		bool header = !optParser.FindOption('n');
		int nbValPerLine = optParser.GetIntOption('s',8);
		ValueMode valMode = Hexadecimal;
		valMode = optParser.FindOption('b') ? Binary : valMode;
		valMode = optParser.FindOption('d') ? Decimal : valMode;
		char prefix = (valMode == Hexadecimal) ? '#' : ((valMode == Binary) ? '%' : ' ');
		std::string dPrefix;
		dPrefix = prefix;
		prefix = optParser.GetStringOption('p', dPrefix)[0];

		std::ifstream in(argv[1],std::ios::binary);
		
		TOOLS_ASSERTMSG(in.good(), "Unable to open file " << argv[1]);

		std::ofstream out(argv[2]);

		int nbVal = 0;
		if (header)
		{
			out << "; Original datas from " << argv[1] << "";
		}
		while (!in.eof())
		{
			unsigned char c;
			in.read((char*)&c,1);
			if (!in.eof())
			{
				if (nbVal % nbValPerLine == 0)
					out << "\n\tdb\t";
				else
					out << ",";

				switch (valMode)
				{
				case Binary :
					{
						out << prefix;
						out.setf(std::ios::dec, std::ios::basefield);
						out.fill(' ');
						out.width(0);
						for (unsigned char mask = 0x80 ; mask > 0 ; mask = mask >> 1)
							out << (((c & mask) == 0) ? '0' : '1');
						break;
					}
				case Decimal :
					{
						out.setf(std::ios::dec, std::ios::basefield);
						out.fill(' ');
						out.width(3);
						out << (unsigned int)c;
						break;
					}
				case Hexadecimal :
				default : 
					{
						out << prefix;
						out.setf(std::ios::hex, std::ios::basefield);
						out.fill('0');
						out.width(2);
						out << (unsigned int)c;
						break;
					}
				}

				nbVal ++;
			}
		}
		out << std::endl;

		std::cout << "\"" << argv[1] << "\" converted to \"" << argv[2] << "\"" << std::endl;
	}
	catch(tools::CException &e)
	{
		std::cerr << e << std::endl;
		return -1;
	}
	

	return 0;
}
