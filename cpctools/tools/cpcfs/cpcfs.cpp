/**
* Petit utilitaire de transfert, mode texte, ligne de commande uniquement
* @author Thierry JOUIN, Adrien Destugues
* @version 1.2
* @date 18/09/2011
*/

#include "CCPCDisc.h"
#include "CDSKFile.h"
#include "CCPCBinaryFile.h"
#include "CCPCAsciiFile.h"
#include "COptionParser.h"

#include <stdio.h>


void GetAmsdosParamFromName(std::string &filename, std::string &filenameams,
		int &user, bool &system, bool &writeProtected,
		int &start, int &exec)
{
	std::string s,f;

	user = 0;
	system = false;
	writeProtected = false;
	start = -1;
	exec = -1;

	f = filename;

	if (f.find(':') != std::string::npos)
	{
		// we've got a user, try to convert to value
		s = filename.substr(0,filename.find(':'));

		user = strtol(s.c_str(), NULL, 0);
		f = f.substr(f.find(':')+1, f.size() - f.find(':'));
	}

	if (f.find(',') != std::string::npos)
	{
		s = f.substr(f.find(',')+1,f.size() - f.find(','));
		f = f.substr(0, f.find(','));

		bool nostart = true;
		bool noexec = true;
		std::string tok;

		while (s.size() > 0)
		{
			if (s.find(',') != std::string::npos)
			{
				tok = s.substr(0, s.find(','));
				s = s.substr(s.find(',')+1,s.size() - s.find(','));
			}
			else
			{
				tok = s;
				s = "";
			}

			if (tok == "S")
			{
				system = true;
			}
			else if (tok == "P")
			{
				writeProtected = true;
			}
			else if (nostart)
			{
				start = strtol(tok.c_str(), NULL, 0);
				nostart = false;
			}
			else if (noexec)
			{
				exec = strtol(tok.c_str(), NULL, 0);
				noexec = false;
			}
		}
	}

	filename = f;
	// TODO make sure we output a valid AMSDOS filename (8.3, space padded,
	// no special chars, etc).
	for (unsigned int i=0;i<f.size();i++)
	{
		f[i] = toupper(f[i]);
	}
	filenameams = f;
}


class Commands
{
	public:
		static void Catalog(void);
		static void Format(void);
		static void PutFile(void);
		static void GetFile(void);
		static void GetAllFiles(void);
		static void RenameFile(void);
		static void DeleteFile(void);
		static void DiskInfo(void);
		static void FileInfo(void);
		static void DumpFile(void);
		static void Unknown(void);

		static std::string discFilename;
		static int side;
		static bool binary;
		static bool headerCreation;
		static bool headerSaving;
		static CCPCDisc::TDisc typeDisc;
		static std::vector<std::string> fileNames;
};

std::string Commands::discFilename;
int Commands::side;
bool Commands::binary;
bool Commands::headerCreation;
bool Commands::headerSaving;
CCPCDisc::TDisc Commands::typeDisc;
std::vector<std::string> Commands::fileNames;

typedef void(*TCommand)(void);


void Commands::Catalog(void)
{
	CCPCDisc *disc = CCPCDisc::OpenDisc(discFilename,side);

	const CCPCDisc::CCPCDirectoryMap &dir = disc->GetDirectoryMap();

	std::cout << "Directory : " << std::endl;

	int totalSize = 0;
	int nbFile = 0;
	for (CCPCDisc::CCPCDirectoryMap::const_iterator i = dir.begin();
			i!=dir.end(); i++)
	{
		std::cout.width(3);
		std::cout << (unsigned int)i->first.User << ":"
			<< std::string(i->first.Name,0,8) << "."
			<< std::string(i->first.Name,8,3) << " ";
		if (i->first.System)
		{
			std::cout << "S";
		}
		else
		{
			std::cout << " ";
		}
		if (i->first.WriteProtected)
		{
			std::cout << "*";
		}
		else
		{
			std::cout << " ";
		}
		std::cout << i->second.Size << "Kb" << std::endl;
		if (i->first.User != CCPCDisc::DeleteUser)
		{
			totalSize += i->second.Size;
			nbFile++;
		}
		std::cout.width(0);
	}

	int capacity = disc->GetDiscCapacity();

	std::cout << nbFile << " Files, " << (capacity-totalSize)
		<< "Kb Free/" << capacity << "Kb" << std::endl;
	disc->Close();
	delete disc;

	TOOLS_ASSERTMSG(capacity >= totalSize, "Disc capacity seems to be invalid, found " << totalSize << " on disc, but capacity only " << (int)disc->GetDiscCapacity());
}


void Commands::Format(void)
{
	CCPCDisc *disc = CCPCDisc::CreateDisc(discFilename,typeDisc,side);

	disc->Format();

	disc->Close();
	delete disc;
}


void Commands::PutFile(void)
{
	CCPCDisc *disc = CCPCDisc::OpenDisc(discFilename,side);

	for (unsigned int i=0;i<fileNames.size();i++)
	{
		std::string filename;
		int user;
		bool sys,pro;
		int start, exec;

		GetAmsdosParamFromName(fileNames[i], filename, user, sys, pro, start, exec);

		CCPCFile* const fileIn = binary ? static_cast<CCPCFile*>(new CCPCBinaryFile()) : static_cast<CCPCFile*>(new CCPCAsciiFile());
		const char* const kind = binary ? "binary":"ascii";

		try
		{
			fileIn->openFile(fileNames[i],headerCreation);
		}
		catch (tools::CException &e)
		{
			std::cerr << e << std::endl;
			TOOLS_ERRORMSG("Error opening " << kind << " file " << filename)
			delete fileIn;
		}

		std::cout << "Putting " << kind << " file <" << filename << "> " << fileIn->getSize() << " bytes";

		if (binary && start != -1)
		{
			((CCPCBinaryFile*)fileIn)->setAdress(start);
			std::cout << " [start address &" << std::hex << start << "]" << std::dec;
		}
		if (binary && exec != -1)
		{
			((CCPCBinaryFile*)fileIn)->setExecute(exec);
			std::cout << " [execute address &" << std::hex << start << "]" << std::dec;
		}

		std::cout << std::endl;

		disc->PutFile(*fileIn, filename, user, sys, pro);
		delete fileIn;
	}

	disc->Close();
	delete disc;
}


void Commands::GetFile(void)
{
	CCPCDisc *disc = CCPCDisc::OpenDisc(discFilename,side);

	TOOLS_ASSERTMSG( (fileNames.size() != 0) , "No file to get !");

	for (unsigned int i=0;i<fileNames.size();i++)
	{
		std::string filename;
		int user;
		bool sys,pro;
		int start, exec;

		GetAmsdosParamFromName(fileNames[i], filename, user, sys, pro, start, exec);

		CCPCFile *file = disc->GetFile(filename, user);
		std::cout << "Getting ";
		if (file->getType() == Binary)
		{
			std::cout << "Bin :";
		}
		else
		{
			std::cout << "Asc :";
		}
		std::cout << "<" << user << ":" << filename << "> \t";
		std::cout << file->getSize() << " bytes \t" << (float)file->getSize()/1024.0f << "Kb " << std::endl;
		file->saveFile(filename,headerSaving);
		delete file;
	}

	disc->Close();
	delete disc;
}


void Commands::GetAllFiles(void)
{
	CCPCDisc *disc = CCPCDisc::OpenDisc(discFilename,side);

	for (unsigned int i=0;i<disc->GetNbFiles();i++)
	{
		int user;
		std::string filename = disc->GetFilename(i, user);
		CCPCFile *file = disc->GetFile(filename, user);
		std::cout << "Getting ";
		if (file->getType() == Binary)
		{
			std::cout << "Bin :";
		}
		else
		{
			std::cout << "Asc :";
		}
		std::cout << "<" << user << ":" << filename << "> \t";
		std::cout << file->getSize() << " bytes \t" << (float)file->getSize()/1024.0f << "Kb " << std::endl;
		if (user != 0)
		{
			char userStr[16];
			sprintf(userStr, "%d_", user);
			filename = userStr + filename;
		}
		file->saveFile(filename,headerSaving);
		delete file;
	}

	disc->Close();
	delete disc;
}


void Commands::RenameFile(void)
{
	TOOLS_ASSERTMSG( (fileNames.size() == 2) , "2 fileNames for rename");
	CCPCDisc *disc = CCPCDisc::OpenDisc(discFilename,side);

	std::string o_filename;
	int o_user;
	bool o_sys, o_pro;
	int o_start, o_exec;

	GetAmsdosParamFromName(fileNames[0], o_filename, o_user, o_sys, o_pro, o_start, o_exec);

	std::string n_filename;
	int n_user;
	bool n_sys, n_pro;
	int n_start, n_exec;

	GetAmsdosParamFromName(fileNames[1], n_filename, n_user, n_sys, n_pro, n_start, n_exec);

	std::cout << "Rename file <" << o_user << ":" << o_filename << "> to <" << n_user << ":" << n_filename << ">" << std::endl;
	disc->RenameFile(	o_filename,n_filename, o_user, n_user,
			o_sys, n_sys, o_pro, n_pro);

	disc->Close();
	delete disc;
}


void Commands::DeleteFile(void)
{
	CCPCDisc *disc = CCPCDisc::OpenDisc(discFilename,side);

	for (unsigned int i=0;i<fileNames.size();i++)
	{
		std::string filename;
		int user;
		bool sys,pro;
		int start, exec;

		GetAmsdosParamFromName(fileNames[i], filename, user, sys, pro, start, exec);

		std::cout << "Delete file <" << user << ":" << filename << ">" << std::endl;
		disc->EraseFile(filename, user);
	}

	disc->Close();

	delete disc;
}


void Commands::DiskInfo(void)
{
	CCPCDisc *disc = CCPCDisc::OpenDisc(discFilename,side);

	disc->PrintInfo(std::cout);

	disc->Close();
	delete disc;
}


void Commands::FileInfo(void)
{
	CCPCDisc *disc = CCPCDisc::OpenDisc(discFilename,side);

	TOOLS_ASSERTMSG( (fileNames.size() != 0) , "No file to print !");

	for (unsigned int i=0;i<fileNames.size();i++)
	{
		std::string filename;
		int user;
		bool sys,pro;
		int start, exec;

		GetAmsdosParamFromName(fileNames[i], filename, user, sys, pro, start, exec);

		CCPCFile *file = disc->GetFile(filename, user);
		std::cout << "File header <" << user << ":" << filename << ">" << std::endl;

		file->printInfo(std::cout);
		std::cout << std::endl;
		file->printHeader(std::cout);

		delete file;
	}

	disc->Close();
	delete disc;
}


void Commands::DumpFile(void)
{
	CCPCDisc *disc = CCPCDisc::OpenDisc(discFilename,side);

	for (unsigned int i=0;i<fileNames.size();i++)
	{
		std::string filename;
		int user;
		bool sys,pro;
		int start, exec;

		GetAmsdosParamFromName(fileNames[i], filename, user, sys, pro, start, exec);

		std::cout << "Dump block file <" << user << ":" << filename << ">" << std::endl;
		disc->DumpBlockFile(filename, user, std::cout);
	}

	disc->Close();

	delete disc;
}


void Commands::Unknown(void)
{
	TOOLS_ERRORMSG( "Unknown command.");
}


static struct SOption appliOption[]=
{
	{'b', "binaryMode"  ,0,1,0,"Use binary mode for transfert"},
	{'e', "createHeader",0,1,0,"Create header when transfering"},
	{'n', "noHeader"    ,0,1,0,"Remove header when transfering"},
	{'s', "side"        ,0,1,0,"Use disk B side"},
	{0,   NULL          ,0,0,0,NULL}
};

static const std::string authorName = "Ramlaid & PulkoMandy";
static const std::string authorMail = "cpcsdk@googlecode.com";
static const std::string appliName = "cpcfs";
static const std::string appliUsageShort = "<dskfile> <command>";
static const std::string appliUsageLong = "<dskfile> <command>\n"
"Manage DSK file.\n"
"Command list :\n"
"\tc : directory\n"
"\tf : format disc\n"
"\tp : put file\n"
"\tg : get file\n"
"\ta : extract all files\n"
"\tr : rename file\n"
"\td : delete file\n"
"\ti : print disc information\n"
"\th : print file header\n"
"When putting or getting file, you can use [<user>:]name.ext[,S][,P][,start[,exec]]";

int main(int argc, char* argv[])
{
	try
	{
		COptionParser optParser(appliOption, appliName, appliUsageShort,
			appliUsageLong, authorName, authorMail);

		if (!optParser.ParseCommandLine(&argc, &argv, 2, true))
		{
			optParser.PrintHeader(std::cout);

			optParser.PrintError(std::cout);
			optParser.PrintUsage(std::cout);
			return -1;
		}

		Commands::discFilename = argv[1];

		std::string c(argv[2]);
		TCommand command = Commands::Unknown;

		switch (c[0])
		{
			case 'c': { command = Commands::Catalog;	break;}
			case 'f': { command = Commands::Format;		break;}
			case 'p': { command = Commands::PutFile;	break;}
			case 'g': { command = Commands::GetFile;	break;}
			case 'a': { command = Commands::GetAllFiles;break;}
			case 'r': { command = Commands::RenameFile;	break;}
			case 'd': { command = Commands::DeleteFile;	break;}
			case 'i': { command = Commands::DiskInfo;	break;}
			case 'h': { command = Commands::FileInfo;	break;}
			case 'b': { command = Commands::DumpFile;	break;}
			default:
			{
				TOOLS_ERRORMSG("Unknown command " << c);
			}
		}

		int i=3;

		Commands::side = optParser.FindOption('s') ? 1 : 0;
		Commands::headerCreation = optParser.FindOption('e');
		Commands::headerSaving = !optParser.FindOption('n');
		Commands::binary = optParser.FindOption('b');
		Commands::typeDisc = CCPCDisc::Data;

		for (i=3;i<argc;i++)
			Commands::fileNames.push_back(std::string(argv[i]));

		(*command)();
    }
	catch (tools::CException &e)
    {
		std::cerr << e << std::endl;
		return -1;
    }

	return 0;
}
