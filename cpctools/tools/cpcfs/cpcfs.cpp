/**
* Petit utilitaire de transfert, mode texte, ligne de commande uniquement
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#include "CCPCDisc.h"
#include "CDSKFile.h"
#include "CCPCBinaryFile.h"
#include "CCPCAsciiFile.h"
#include "COptionParser.h"

#include <stdio.h>

enum TCommand
{
    Catalogue,
		Format,
		PutFile,
		GetFile,
		GetAllFile,
		RenameFile,
		DeleteFile,
		PrintDisc,
		PrintFile,
		DumpBlockFile,
		Unknown
};

void GetAmsdosParamFromName(const std::string &filename, std::string &filenameams,
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

	for (unsigned int i=0;i<f.size();i++)
	{
		f[i] = toupper(f[i]);
	}

	if (f.find(':') != std::string::npos)
	{
		s = filename.substr(0,filename.find(':'));

		if (s.size() == 1)
		{
			if (s[0] >= '0' && s[0] < '9')
			{
				// we've got a user, try to convert to value
				user = strtol(s.c_str(), NULL, 0);
				f = f.substr(f.find(':')+1, f.size() - f.find(':'));
			}
		}
		else
		{
			// we've got a user, try to convert to value
			user = strtol(s.c_str(), NULL, 0);
			f = f.substr(f.find(':')+1, f.size() - f.find(':'));
		}
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
	filenameams = f;
}

static struct SOption appliOption[]=
{
	{'c',(char *)"configFile",0,1,1,(char *)"Configuration file"},
	{'b',(char *)"binaryMode",0,1,0,(char *)"Use binary mode for transfert"},
	{'e',(char *)"createHeader",0,1,0,(char *)"Create header when transfering"},
	{'n',(char *)"noHeader",0,1,0,(char *)"Remove header when transfering"},
	{0,NULL,0,0,0,NULL}
};

static const std::string authorName = "Ramlaid";
static const std::string authorMail = "cpcTools@ramlaid.com";
static const std::string appliName = "cpcfs";
static const std::string appliUsageShort = "<dskfile> <command>";
static const std::string appliUsageLong = "<dskfile> <command>\n"
"Manage DSK file.\n"
"Command list :\n"
"\tc : directory\n"
"\tf : disc format\n"
"\tp : put file\n"
"\tg : get file\n"
"\ta : get all file\n"
"\tr : rename file\n"
"\td : delete file\n"
"\ti : print disc info\n"
"\th : print file header\n"
"When putting or getting file, you can use [<user>:]name.ext[,S][,P][,start][,exec]";

int main(int argc, char* argv[])
{
	try
	{
		COptionParser optParser(appliOption, appliName, appliUsageShort, appliUsageLong, authorName, authorMail);

		optParser.PrintHeader(std::cout);

		if (!optParser.ParseCommandLine(&argc, &argv, 2, true))
		{
			optParser.PrintError(std::cout);
			optParser.PrintUsage(std::cout);
			return -1;
		}

		std::string discFilename(argv[1]);
		std::string c(argv[2]);
		TCommand command = Unknown;

		switch (c[0])
		{
		case 'c': { command = Catalogue; break;}
		case 'f': { command = Format; break;}
		case 'p': { command = PutFile; break;}
		case 'g': { command = GetFile; break;}
		case 'a': { command = GetAllFile; break;}
		case 'r': { command = RenameFile; break;}
		case 'd': { command = DeleteFile; break;}
		case 'i': { command = PrintDisc; break;}
		case 'h': { command = PrintFile; break;}
		case 'b': { command = DumpBlockFile; break;}
		default:
			{
				TOOLS_ERRORMSG("Unknown command " << c);
			}
		}

		int i=3;

		int	side= optParser.FindOption('s') ? 1 : 0;
		bool headerCreation = optParser.FindOption('e');
		bool headerSaving = !optParser.FindOption('n');
		bool binary = optParser.FindOption('b');
		CCPCDisc::TDisc typeDisc = CCPCDisc::Data;

		std::vector<std::string> fileNames;
		for (i=3;i<argc;i++)
			fileNames.push_back(std::string(argv[i]));

		switch (command)
		{
		case Catalogue :
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

				break;
			}
		case Format :
			{
				CCPCDisc *disc = CCPCDisc::CreateDisc(discFilename,typeDisc,side);

				disc->Format();

				disc->Close();
				delete disc;
				break;
			}
		case PutFile :
			{
				CCPCDisc *disc = CCPCDisc::OpenDisc(discFilename,side);

				for (unsigned int i=0;i<fileNames.size();i++)
				{
					std::string filename;
					int user;
					bool sys,pro;
					int start, exec;

					GetAmsdosParamFromName(fileNames[i], filename, user, sys, pro, start, exec);

					if (binary)
					{
						CCPCBinaryFile fileIn;
						try
						{
							fileIn.openFile(filename,headerCreation);
						}
						catch (tools::CException &e)
						{
							std::cerr << e << std::endl;
							TOOLS_ERRORMSG("Error opening binary file " << filename)
						}
						std::cout << "Putting binary file <" << filename << "> " << fileIn.getSize() << " bytes";
						if (start != -1)
						{
							fileIn.setAdress(start);
							std::cout << " [start address &" << std::hex << start << "]" << std::dec;
						}
						if (exec != -1)
						{
							fileIn.setExecute(exec);
							std::cout << " [execute address &" << std::hex << start << "]" << std::dec;
						}
						std::cout << std::endl;
						disc->PutFile(fileIn,filename, user, sys, pro);
					}
					else
					{
						CCPCAsciiFile fileIn;
						try
						{
							fileIn.openFile(filename,headerCreation);
						}
						catch (tools::CException &e)
						{
							std::cerr << e << std::endl;
							TOOLS_ERRORMSG("Error opening ascii file " << filename)
						}
						std::cout << "Putting ascii file <" << filename << "> " << fileIn.getSize() << " bytes" << std::endl;
						disc->PutFile(fileIn,filename, user, sys, pro);
					}
		  }

		  disc->Close();
		  delete disc;
		  break;
	  }
	case GetFile :
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
			break;
		}
	case GetAllFile :
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
			break;
		}
	case RenameFile :
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
			break;
		}
	case DeleteFile :
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
			break;
		}
	case PrintDisc :
		{
			CCPCDisc *disc = CCPCDisc::OpenDisc(discFilename,side);

			disc->PrintInfo(std::cout);

			disc->Close();
			delete disc;
			break;
		}
	case PrintFile :
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

				std::cout << *file << std::endl;
				file->printHeader(std::cout);

				delete file;
			}

			disc->Close();
			delete disc;
			break;
		}
	case DumpBlockFile :
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
			break;
		}
	default :
		{
			TOOLS_ERRORMSG( "Unknown command : " << c);
		}
	}
    }
	catch (tools::CException &e)
    {
		std::cerr << e << std::endl;
		return -1;
    }

	return 0;
}
