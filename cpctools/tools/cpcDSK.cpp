/*
 * cpcDSK : manage DSK reading / writing
 *
 * (c) Ramlaid 2006
 */

#pragma warning( disable : 4786 )

#if _WINDOWS
#include <conio.h>
#else
#include <cstdlib>
#endif
#include "CError.h"
#include "COptionParser.h"
#include "CCPCDisc.h"

static struct SOption appliOption[]=
{
	{'c',"configFile",0,1,1,"Configuration file"},
	{'s',"sourceSide",0,1,1,"Select source side 0 / 1"},
	{'d',"destSide",0,1,1,"Select destination side 0 / 1"},
	{'n',"numSide",0,1,1,"Select number of side 1 / 2"},
	{'e',"extended",0,1,0,"Select extended mode"},
	{'t',"nbTrack",0,1,1,"Number of track to read"},
	{0,NULL,0,0,0,NULL}
};

static const std::string authorName = "Ramlaid";
static const std::string authorMail = "cpcsdk@googlegroups.com";
static const std::string appliName = "cpcDSK";
static const std::string appliUsageShort = "<source> <destination>";
static const std::string appliUsageLong = "<source> <destination>\n\
Read a DSK from source and put it on destination";

int main (int argc, char **argv)
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

		int nbTrack = optParser.GetIntOption('t', 42);
		int nbSide = optParser.GetIntOption('n' , 1);
		std::string srcName = argv[1];
		std::string dstName = argv[2];
		int srcSide = optParser.GetIntOption('s' , 0);
		int dstSide = optParser.GetIntOption('d' , 0);
		bool extended = optParser.FindOption('e');
		bool checkFloppy = true;

		TOOLS_ASSERTMSG(srcSide == 0 || srcSide == 1, "Wrong source side value " << srcSide);
		TOOLS_ASSERTMSG(dstSide == 0 || dstSide == 1, "Wrong destination side value " << dstSide);
		TOOLS_ASSERTMSG(nbSide == 1 || nbSide == 2, "Wrong number of side value " << nbSide);

		if (!CCPCDisc::IsDSK(srcName, srcSide))
		{
			std::cerr << "Source not seems to be a valid DSK" << std::endl;
			return -1;
		}

		CCPCDisc *src = CCPCDisc::OpenDisc(srcName, srcSide);
		CCPCDisc *dst = CCPCDisc::CreateDisc(dstName, src->GetDiscFormat(), dstSide);

		bool srcFloppy = src->IsFloppy();
		bool dstFloppy = dst->IsFloppy();

		CCPCDisc::CTrack *tracks = new CCPCDisc::CTrack[nbTrack * nbSide];

		int t,s, tIdx;

		if (!srcFloppy)
			std::cout << "Reading DSK " << srcName << std::endl;

		tIdx = 0;
		for (t=0 ; t<nbTrack ; t++)
		{
			for (s=srcSide ; s < (srcSide + nbSide) ; s++)
			{
				if (srcFloppy)
					std::cout << "Reading track " << t << " S[" << s << "]" << std::endl;
				src->ReadTrack(t, s, tracks[tIdx], extended);
				if (srcFloppy)
					tracks[tIdx].DisplayInfo(std::cout);
				tIdx ++;
			}
		}

		if (!dstFloppy)
			std::cout << "Writing DSK " << dstName << std::endl;

		if (srcFloppy && dstFloppy && srcName == dstName)
		{
			// Make a pause
			std::cout << "Please change disc..." << std::endl;
#if _WINDOWS
			_getch();
#else
			getchar();
#endif

		}

		tIdx = 0;
		for (t=0 ; t<nbTrack ; t++)
		{
			for (s=dstSide ; s < (dstSide + nbSide) ; s++)
			{
				if (dstFloppy)
					std::cout << "Writing track " << t << " S[" << s << "]" << std::endl;
				if (dstFloppy)
					tracks[tIdx].DisplayInfo(std::cout);
				dst->WriteTrack(t, s, tracks[tIdx], extended);
				
				if (dstFloppy && checkFloppy)
				{
					CCPCDisc::CTrack trackTest;
					
					std::cout << "Checking track " << t << " S[" << s << "]" << std::endl;
					dst->ReadTrack(t, s, trackTest, extended);

					int nbTry = 0;
					while (! trackTest.IsEqual(tracks[tIdx]) && nbTry < 5)
					{
						std::cout << "Repair track " << t << " S[" << s << "]" << std::endl;
						dst->RepairTrack(t, s, tracks[tIdx], trackTest, extended);
						std::cout << "Checking track " << t << " S[" << s << "]" << std::endl;
						dst->ReadTrack(t, s, trackTest, extended);

						nbTry ++;
					}

					TOOLS_ASSERTMSG(trackTest.IsEqual(tracks[tIdx]), "Unable to write track " << t << " side " << s);
				}

				tIdx ++;
			}
		}

		delete[] tracks;
	
	}
	catch(tools::CException &e)
	{
		std::cerr << e << std::endl;
		return -1;
	}

	return 0;
}
