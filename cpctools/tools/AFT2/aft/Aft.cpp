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
	{'p', "comPort",   0,1,1,"Set <$1> as COM port (eg /dev/ttyS* or COM*)"},
	{'b', "badrate",   0,1,1,"Set <$1> as baudrate (default 115200 baud)"},
	{'f', "file",      0,1,1,"Force <$1> to be transfered file"},
	{'d', "directory", 0,1,1,"Force <$1> to be transfered directory"},
	{'n', "noLoop",    0,1,0,"Do not loop AFT transfert"},
	{0,   NULL,        0,0,0,NULL}
};

static const std::string authorName = "Targhan/Ramlaid/Krusty/PulkoMandy/pelrun";
static const std::string authorMail = "cpcsdk@googlegroups.com";
static const std::string appliName = "aft";
static const std::string appliUsageShort = "";
static const std::string appliUsageLong = "\nArkos File Transfer tool";

static const std::string devicePrefixes[] =
{
#if defined(__HAIKU__)
	"/dev/ports/usb0",
	"/dev/ports/usb",
	"/dev/ports/",
#elif defined(__linux__)
	"/dev/ttyUSB0",
	"/dev/ttyUSB",
	"/dev/tty",
	"/dev/",
#else
	"COM1",
	"COM",
#endif

	""
};

static inline const std::string defaultComPort()
{
	return devicePrefixes[0];
}

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
		std::string COMport = defaultComPort();
		std::string filename = "";
		std::string dir = "";
		int baudrate = 115200;

		
		int i=0;
		while (optParser.GetOptionI(i) != 0)
		{
			switch(optParser.GetOptionI(i))
			{
			case 'p':
				{
					COMport = optParser.GetStringOptionI(i);
					break;
				}
			case 'b':
				{
					baudrate = optParser.GetIntOptionI(i);
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

		for(unsigned int i = 0; i < sizeof(devicePrefixes) / sizeof(devicePrefixes[0]); i++)
		{
			std::string device = devicePrefixes[i] + COMport;
			CAksFileTransfert transfert(device, baudrate);

			if (!transfert.IsOpen())
				continue;
			else
				std::cout << "AFT now listening on " << device << std::endl;

			if (filename.size() != 0)
				transfert.SetForceFilename(filename);

			if (dir.size() != 0)
				transfert.SetFilepath(dir);

			while (!noLoop || (noLoop && (transfert.GetNbTransfertDone() != 1)))
				transfert.Run();

			return 0;
		}

		std::cout << "Unable to open COM port " << COMport << std::endl;
		return -1;
	}
	catch(tools::CException &e)
	{
		std::cerr << e << std::endl;
		return -1;
	}
}

