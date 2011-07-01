/**
 * @file aft
 * Arkos file transfert
 * @author Thierry JOUIN
 * @version 1.1
 * @date 06/02/2006
 */

#include "CAksFileTransfert.h"

#include "CError.h"
#include "COptionParser.h"

#include <iostream>
#include <string>

struct SOption appliOption[]=
{
	{'p',(char *)"comPort",0,1,1,(char *)"Set <$1> as COM port"},
	{'f',(char *)"file",0,1,1,(char *)"Force <$1> to be transfered file"},
	{'d',(char *)"directory",0,1,1,(char *)"Force <$1> to be transfered directory"},
	{'n',(char *)"noLoop",0,1,0,(char *)"Do not loop AFT transfert"},
	{0,NULL,0,0,0,NULL}
};

static const std::string authorName = "Targhan/Ramlaid/Krusty";
static const std::string authorMail = "cpcTools@ramlaid.com";
static const std::string appliName = "aft";
static const std::string appliUsageShort = "";
static const std::string appliUsageLong = "\n\
Arkos File Transfert tool";

int main(int argc, char *argv[])
{
	try
	{
		COptionParser optParser(appliOption, appliName, appliUsageShort, appliUsageLong, authorName, authorMail);
		
		optParser.PrintHeader(std::cout);

		if (!optParser.ParseCommandLine(&argc, &argv, 0, false))
		{
			optParser.PrintError(std::cout);
			optParser.PrintUsage(std::cout);
			return -1;
		}

		bool noLoop = false;
		int COMport = 1;
		std::string filename = "";
		std::string dir = "";

		
		int i=0;
		while (optParser.GetOptionI(i) != 0)
		{
			switch(optParser.GetOptionI(i))
			{
			case 'p':
				{
					COMport = optParser.GetIntOptionI(i);
					break;
				}
			case 'f':
				{
					filename = optParser.GetStringOptionI(i);
					noLoop = true;
					break;
				}
			case 'd':
				{
					dir = optParser.GetStringOptionI(i);
					break;
				}
			case 'n':
				{
					noLoop = true;
					break;
				}
			default:
				{
					break;
				}
			}

			i++;
		}

		std::cout << "COMport " << COMport << endl ;
		CAksFileTransfert transfert(COMport);

		if (!transfert.IsOpen())
		{
			std::cout << "Unable to open port COM " << COMport << std::endl;
			return -1;
		}

		if (filename.size() != 0)
		{
			transfert.SetForceFilename(filename);
		}
		if (dir.size() != 0)
		{
			transfert.SetFilepath(dir);
		}

		while (!noLoop || (noLoop && (transfert.GetNbTransfertDone() != 1)))
		{
			transfert.Run();
		}
	}
	catch(tools::CException &e)
	{
		std::cerr << e << std::endl;
		return -1;
	}

	return 0;
}

