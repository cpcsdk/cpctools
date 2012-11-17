/**
* Permet le transfert entre un fichier DAMS et du mode texte (et inversement !)
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#include <fstream>
#include "COptionParser.h"
#include "CError.h"

#define NB_DAMS_MNEMONIQUE 77

struct SOption appliOption[]=
{
	{'n',"noHeader",0,1,0,"Do not check for header (default false)"},
	{0,NULL,0,0,0,NULL}
};
static const std::string authorName = "Ramlaid";
static const std::string authorMail = "cpcTools@ramlaid.com";
static const std::string appliName = "damsConverter";
static const std::string appliUsageShort = "<file.s> <file.z80>";
static const std::string appliUsageLong = "<file.s> <file.z80>\n\
Convert a dams source file to ascii source file";

static const char *damsMnemonique[NB_DAMS_MNEMONIQUE]={
	"LD","INC","DEC","ADD","ADC","SUB","SBC","AND","XOR","OR","CP",
		"PUSH","POP","BIT","RES","SET","RLC","RRC","RL","RR","SLA","SRA",
		"SRL","IN","OUT","RST","DJNZ","EX","IM","JR","CALL","RET","JP",
		"NOP","RLCA","RRCA","RLA","RRA","DAA","CPL","SCF","CCF","HALT",
		"EXX","DI","EI","NEG","RETN","RETI","RRD","RLD","LDI","CPI","INI",
		"OUTI","LDD","CPD","IND","OUTD","LDIR","CPIR","INIR","OTIR","LDDR",
		"CPDR","INDR","OTDR","DB","DW","DM","DS","EQU","ORG","ENT",
		"IF","ELSE","END"
};

bool hasHeader(std::ifstream &inFile)
{
	unsigned char type = 99;
	unsigned int checksum = 0;
	unsigned int checksum2 = 0;

	inFile.seekg(0x00,std::ios::beg);
	for (unsigned int i=0;i<0x42;i++)
	{
		unsigned char c = (unsigned char)inFile.get();
		if (i == 0x12)
			type = c;
		checksum+= (unsigned int)c;
	}

	inFile.seekg(0x43,std::ios::beg);
	checksum2 = (unsigned char)inFile.get();
	checksum2 += (unsigned char)inFile.get() * 256;
	
	inFile.seekg(0x00,std::ios::beg);

	return (checksum == checksum2 && ((type == 2) || (type == 0)));
}

int main(int argc, char* argv[])
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
	
		bool headerCheck = !optParser.FindOption('n');		
		
		std::ifstream inFile;
		inFile.open(argv[1],std::ios::in | std::ios::binary);
		
		TOOLS_ASSERTMSG(inFile.good(),"Unable to open " << argv[1]);
		
		std::ofstream outFile;
		outFile.open(argv[2]);
		
		TOOLS_ASSERTMSG(outFile.good(),"Unable to open " << argv[2]);

		if (headerCheck)
		{
			if (hasHeader(inFile))
			{
				inFile.seekg(0x80,std::ios::beg);
			}
		}

		unsigned char c;
		c = (unsigned char)inFile.get();
		while (!inFile.eof() && c!=0)
		{
			if (c == 0xff)
			{
				// Commentaire ligne
				outFile << ";";
				c = (unsigned char)inFile.get();
				while (c != 0xd)
				{
					outFile << c;
					c = (unsigned char)inFile.get();
				}
				outFile << std::endl;
			}
			else
			{
				if (c >= 0x80 && c!= 0x0d)
				{
					// Mnemonique sans label
					// ENT
					if (c == 0xc9)
						outFile << ";";
					outFile << "\t" << damsMnemonique[c-0x80] << "\t";
					// DS ?,?
					if (c == 0xc6)
					{
						c = (unsigned char)inFile.get();
						// Fin de ligne
						while (c != 0x0d)
						{
							if (c == ',')
							{
								while (c != 0x0d && c != 0xff)
									c = (unsigned char)inFile.get();
							}
							if (c != 0x0d)
							{
								if (c == 0xff)
									outFile << "\t;";
								else		  
									outFile << c;
								c = (unsigned char)inFile.get();
							}
						}
					}
					else
					{
						c = (unsigned char)inFile.get();
						// Fin de ligne
						while (c != 0x0d)
						{
							if (c == 0xff)
								outFile << "\t;";
							else		  
								outFile << c;
							c = (unsigned char)inFile.get();
						}
					}
					outFile << std::endl;

				}
				else
				{
					// Label
					while (c < 0x80 && c != 0x0d)
					{
						outFile << c;
						c = (unsigned char)inFile.get();
					}
					if (c != 0x0d)
					{
						// Mnemonique apres label
						// ENT
						if (c == 0xc9)
							outFile << ";";
						if (c != 0xff)
							outFile << "\t" << damsMnemonique[c-0x80] << "\t";
						else
							outFile << "\t\t\t;";
						// DS ?,?
						if (c == 0xc6)
						{
							c = (unsigned char)inFile.get();
							// Fin de ligne
							while (c != 0x0d)
							{
								if (c == ',')
								{
									while (c != 0x0d && c != 0xff)
										c = (unsigned char)inFile.get();
								}
								if (c != 0x0d)
								{
									if (c == 0xff)
										outFile << "\t;";
									else		  
										outFile << c;
									c = (unsigned char)inFile.get();
								}
							}
						}
						else
						{
							c = (unsigned char)inFile.get();
							// Fin de ligne
							while (c != 0x0d)
							{
								if (c == 0xff)
									outFile << "\t;";
								else		  
									outFile << c;
								c = (unsigned char)inFile.get();
							}
						}
						outFile << std::endl;
					}
					else
					{
						outFile << std::endl;
					}
				}
			}
			if (!inFile.eof())
				c = (unsigned char)inFile.get();
		}
	}
	catch(tools::CException &e)
	{
		std::cerr << e << std::endl;
		return -1;
	}
	return 0;
}
