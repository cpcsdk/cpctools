/*
 * cpcDSK : manage DSK files
 *
 * (c) Ramlaid 2006
 */

#pragma warning( disable : 4786 )

//#include <conio.h>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <ctype.h>

#include "CError.h"
#include "COptionParser.h"
#include "CDSKFile.h"
#include "CCPCDisc.h"
#include "CCPCUnknownDisc.h"
#include "CCPCDataDisc.h"

using namespace std;

typedef vector<CCPCUnknownDisc::CCPCTrackFormat> DSKFormat;

static struct SOption appliOption[]=
{
	{'f',"formatType",0,1,1,"Set format type (default cpcdata42)"},
	{'t',"nbTrack",0,1,1,"Number of track"},
	{'s',"nbSide",0,1,1,"Number of side"},
	{'n',"nbSector",0,1,1,"Number of sector per track"},
	{'b',"sectorBase",0,1,1,"Sector base"},
	{'e',"sectorSize",0,1,1,"Sector size"},
	{'j',"notInterlaced",0,1,0,"Not interlaced sector ID"},
	{'g',"gap3",0,1,1,"GAP3 size"},
	{'r',"rwgap",0,1,1,"Read/Write GAP size"},
	{'i',"formatFile",0,1,1,"Format description file"},
	{'a',"startTrack",0,1,1,"Start track when putting"},
	{'p',"startSide",0,1,1,"Start side when putting"},
	{'o',"startSector",0,1,1,"Start sector ID when putting"},
	{'d',"useDoubleSide",0,1,0,"Use double side when putting"},
	{'m',"prefixOutputZ80",0,1,1,"Add $1 before filename output"},
	{'z',"outputZ80",0,1,1,"Output put raw file info to file $1"},
	{'x',"dumpDirectory",0,1,0,"WIP ! Dump directory information"},
	{0,NULL,0,0,0,NULL}
};

static const string authorName = "Ramlaid";
static const string authorMail = "cpcTools@ramlaid.com";
static const string appliName = "cpcDSKManager";
static const string appliUsageShort = "<DSK> <command>";
static const string appliUsageLong = "<DSK> <command>\n\
Manage DSK file\n\
Command list :\n\
\td : dump\n\
\tf : disc format\n\
\tp : put file\n\
Available format : cpcdata, cpcdata42, cpcsys, cpcsys42, parados80";

static const string outputHeaderMask = 
"; %FILE% (%SIZE% sectors) starts on track %TRACK%, sector #%SECTOR%, side %HEAD%\n\
%PREFIX%%FILE%_track\tEQU\t%TRACK%\n\
%PREFIX%%FILE%_sector\tEQU\t#%SECTOR%\n\
%PREFIX%%FILE%_head\tEQU\t%HEAD%\n\
%PREFIX%%FILE%_size\tEQU\t%SIZE%\n\n";

// These char will be replaced by '_'
static const string filenameFilterList = " .&ι\"'(θηΰ)=+°^$*ω!:;,?/§\\~#{[|`^@]}";

void DumpDSKFile(const string &filename)
{
	auto_ptr<CDSKFile> dsk(CDSKFile::OpenDSKFile(filename));

	if (dsk.get() != NULL)
	{
		dsk->GetHeader().DisplayInfo(cout);
		
		cout << endl;

		int nbSide = dsk->GetHeader().GetNbSide();
		int nbTrack = dsk->GetHeader().GetNbTrack();

		for (int s=0 ; s<nbSide ; s++)
			for (int t=0 ; t<nbTrack ; t++)
			{
				cout << "Track " << t << " [Side " << s << "] (" << dsk->GetHeader().GetTrackSize(t, s) << " bytes)" << endl;
				CDSKTrack* track = dsk->GetTrack(t, s);
				if (track != NULL)
				{
					track->DisplayInfo(cout);
					cout << "..." << endl;
//TODO verifier si la mofification fonctionne egalement avec windows !
//DEBUT modif					
	//				while (!_kbhit());
	//				_getch();
					getchar();
//FIN modif
					cout << endl;
				}
			}

	}
}

void GetTrackFormat(const CCPCDisc::TDisc &format, int &nbTrack, CCPCUnknownDisc::CCPCTrackFormat &trackFormat)
{
	switch (format)
	{
	case CCPCDisc::Data : 
		{
			trackFormat = CCPCUnknownDisc::CCPCTrackFormat(9, 0xc1, 512, true);
			nbTrack = 40;
			break;
		}
	case CCPCDisc::Data42 : 
		{
			trackFormat = CCPCUnknownDisc::CCPCTrackFormat(9, 0xc1, 512, true);
			nbTrack = 42;
			break;
		}
	case CCPCDisc::System : 
		{
			trackFormat = CCPCUnknownDisc::CCPCTrackFormat(9, 0x41, 512, true);
			nbTrack = 40;
			break;
		}
	case CCPCDisc::System42 : 
		{
			trackFormat = CCPCUnknownDisc::CCPCTrackFormat(9, 0x41, 512, true);
			nbTrack = 42;
			break;
		}
	case CCPCDisc::Parados80 :
		{
			trackFormat = CCPCUnknownDisc::CCPCTrackFormat(10, 0xc1, 512, true);
			nbTrack = 80;
			break;
		}
	}
}

void ReadIntValuesFromString(const string &str, vector<int> &values)
{
	values.clear();
	if (str.find('-') != string::npos)
	{
		string strVal = str.substr(0, str.find('-'));
		int start = strtol(strVal.c_str(),NULL,0);
		TOOLS_ASSERTMSG(errno != ERANGE, "Wrong start value " << strVal);
		strVal = str.substr(str.find('-')+1 , str.size() - str.find('-') - 1);
		int end = strtol(strVal.c_str(),NULL,0);
		TOOLS_ASSERTMSG(errno != ERANGE, "Wrong end value " << strVal);
		TOOLS_ASSERTMSG(end > start, "Wrong interval " << start << "-" << end);

		for (int v=start ; v<end ; v++)
			values.push_back(v);
	}
	else
	{
		string strVal;
		size_t start = 0;
		size_t pos = str.find(',', start);
		while (pos != string::npos)
		{
			strVal = str.substr(start, pos - start);
			int val = strtol(strVal.c_str(),NULL,0);
			TOOLS_ASSERTMSG(errno != ERANGE, "Wrong value " << strVal);
			values.push_back(val);
			start = pos+1;
			pos = str.find(',', start);
		}
		if (pos-start > 1)
		{
			strVal = str.substr(start, pos - start);
			int val = strtol(strVal.c_str(),NULL,0);
			TOOLS_ASSERTMSG(errno != ERANGE, "Wrong value " << strVal);
			values.push_back(val);
		}
	}
}

void FillTrack(DSKFormat &tracks, const vector<int> &tracksID, 
			   int nbSector, unsigned char secBase, int secSize, int secTrack, int secHead,
			   bool interlaced,
			   vector<int> &sectorID, vector<int> &sectorSize, vector<int> &sectorTrack, vector<int> &sectorHead,
			   unsigned char gap3, unsigned char rwgap)
{
	if (tracksID.empty())
		return;

	CCPCUnknownDisc::CCPCTrackFormat trackFormat;
	if (sectorID.empty())
	{
		trackFormat = CCPCUnknownDisc::CCPCTrackFormat(nbSector, secBase, secSize, interlaced, secTrack, secHead);
	}
	else
	{
		TOOLS_ASSERTMSG(sectorID.size() == sectorSize.size(), "Error in init custom track");
		TOOLS_ASSERTMSG(sectorID.size() == sectorTrack.size(), "Error in init custom track");
		TOOLS_ASSERTMSG(sectorID.size() == sectorHead.size(), "Error in init custom track");

		trackFormat.SectorArray.resize(sectorID.size());
		for (int s=0 ; s<sectorID.size() ; s++)
		{
			trackFormat.SectorArray[s].SectorID = sectorID[s];
			trackFormat.SectorArray[s].SectorSize = sectorSize[s];
			trackFormat.SectorArray[s].Track = sectorTrack[s];
			trackFormat.SectorArray[s].Head = sectorHead[s];
		}
		trackFormat.ComputeGap3();
	}
	if (gap3 != (unsigned char)-1)
	{
		trackFormat.Gap3 = gap3;
		trackFormat.RWGap = rwgap;
	}

	for (int t=0 ; t<tracksID.size() ; t++)
	{
		if (tracksID[t] < 0 || tracksID[t] >= tracks.size())
			continue;
		tracks[tracksID[t]] = trackFormat;
	}
}

void InitFormat(const string &formatFilename, DSKFormat &tracks, int &nbSide)
{
	ifstream iniFile(formatFilename.c_str());

	TOOLS_ASSERTMSG(iniFile.good(), "Unable to read " << formatFilename);

	tracks.clear();

	int nbTrack = 42;
	nbSide = 1;
	vector<int> tracksID;
	int nbSector = 9;
	unsigned char secBase = 0xc0;
	int secSize = 512;
	int secTrack = -1;
	int secHead = -1;
	bool interlaced = true;
	int gap3 = -1;
	int rwgap = -1;
	vector<int> sectorID;
	vector<int> sectorSize;
	vector<int> sectorTrack;
	vector<int> sectorHead;

	while (!iniFile.eof())
	{
		char lineStr[1024];
		iniFile.getline(lineStr, 1024);
		string line = lineStr;
		line.erase( remove(line.begin(), line.end(), ' '), line.end());
		line.erase( remove(line.begin(), line.end(), '\t'), line.end());
		transform(line.begin(), line.end(), line.begin(), ptr_fun(toupper) );

		if (line.empty() || line[0] == ';')
			continue;

		if (line[0] == '[')
		{
			TOOLS_ASSERTMSG(line[line.size()-1] == ']', "Wrong header " << line);
			string subLine = line.substr(1, line.size()-2);
			
			if (subLine.find("TRACK") == 0)
			{
				if (tracks.empty())
					tracks.resize(nbTrack * nbSide);
				
				FillTrack(	tracks, tracksID, nbSector, secBase, secSize, secTrack, secHead, interlaced, 
							sectorID, sectorSize, sectorTrack, sectorHead,
							gap3, rwgap);
				
				tracksID.clear();
				nbSector = 9;
				secBase = 0xc0;
				secSize = 512;
				secTrack = -1;
				secHead = -1;
				gap3 = -1;
				rwgap = -1;
				sectorID.clear();
				sectorSize.clear();
				sectorTrack.clear();
				sectorHead.clear();

				int side = -1;
				if (subLine.find("TRACK-A") == 0)
				{
					side = 0;
				}
				if (subLine.find("TRACK-B") == 0)
				{
					TOOLS_ASSERTMSG(nbSide == 2, "Unable to set TRACK-B, not double sided");
					side = 1;
				}

				vector<int> tracksIDList;
				if (subLine.find(':') == string::npos)
				{
					tracksIDList.resize(nbTrack);
					for (int t=0 ; t<nbTrack ; t++)
						tracksIDList[t] = t;
				}
				else
				{
					subLine = subLine.substr(subLine.find(':')+1, subLine.size() - subLine.find(':') - 1);

					ReadIntValuesFromString(subLine, tracksIDList);
				}

				if (side == -1)
				{
					for (int t=0 ; t<= tracksIDList.size() ; t++)
						for (int s=0 ; s < nbSide ; s++)
							tracksID.push_back(tracksIDList[t] * nbSide + s);
				}
				else
				{
					for (int t=0 ; t<= tracksIDList.size() ; t++)
						tracksID.push_back(tracksIDList[t] * nbSide + side);
				}
			}
		}
		else
		{
			string value = line.substr(line.find('=')+1, line.size() - line.find('=') - 1);
			if (line.find("NBSIDE") == 0)
			{
				int val = strtol(value.c_str(),NULL,0);
				TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
				nbSide = val;
			}
			else if (line.find("NBTRACK") == 0)
			{
				int val = strtol(value.c_str(),NULL,0);
				TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
				nbTrack = val;
			}
			else if (line.find("NBSECTOR") == 0)
			{
				int val = strtol(value.c_str(),NULL,0);
				TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
				nbSector = val;
			}
			else if (line.find("SECTORSIZE") == 0)
			{
				int val = strtol(value.c_str(),NULL,0);
				TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
				secSize = val;
			}
			else if (line.find("SECTORBASE") == 0)
			{
				int val = strtol(value.c_str(),NULL,0);
				TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
				secBase = val;
			}
			else if (line.find("SECTORTRACK") == 0)
			{
				int val = strtol(value.c_str(),NULL,0);
				TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
				secTrack = val;
			}
			else if (line.find("SECTORHEAD") == 0)
			{
				int val = strtol(value.c_str(),NULL,0);
				TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
				secHead = val;
			}
			else if (line.find("GAP3") == 0)
			{
				int val = strtol(value.c_str(),NULL,0);
				TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
				gap3 = val;
				if (rwgap == -1)
					rwgap = gap3;
			}
			else if (line.find("RWGAP") == 0)
			{
				int val = strtol(value.c_str(),NULL,0);
				TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
				rwgap = val;
			}
			else if (line.find("SECTORIDSIZE") == 0)
			{
				ReadIntValuesFromString(value, sectorSize);
			}
			else if (line.find("SECTORIDTRACK") == 0)
			{
				ReadIntValuesFromString(value, sectorTrack);
			}
			else if (line.find("SECTORIDHEAD") == 0)
			{
				ReadIntValuesFromString(value, sectorHead);
			}
			else if (line.find("SECTORID") == 0)
			{
				ReadIntValuesFromString(value, sectorID);
				if (sectorSize.size() != sectorID.size())
				{
					sectorSize.resize(sectorID.size());
					fill(sectorSize.begin(), sectorSize.end(), secSize);
				}
				if (sectorTrack.size() != sectorID.size())
				{
					sectorTrack.resize(sectorID.size());
					fill(sectorTrack.begin(), sectorTrack.end(), secTrack);
				}
				if (sectorHead.size() != sectorID.size())
				{
					sectorHead.resize(sectorID.size());
					fill(sectorHead.begin(), sectorHead.end(), secHead);
				}
			}
			else if (line.find("INTERLACED") == 0)
			{
				int val = strtol(value.c_str(),NULL,0);
				TOOLS_ASSERTMSG(errno != ERANGE, "Wrong token value " << value);
				interlaced = (val == 1);
			}
		}
	}

	FillTrack(	tracks, tracksID, nbSector, secBase, secSize, secTrack, secHead, interlaced,
				sectorID, sectorSize, sectorTrack, sectorHead,
				gap3, rwgap);
}

void InitFormat(int nbTrack, int nbSide, const CCPCUnknownDisc::CCPCTrackFormat &format, DSKFormat &tracks)
{
	tracks.resize(nbTrack * nbSide);
	fill(tracks.begin(), tracks.end(), format);
}

void FormatDSK(const string &filename, int nbSide, const DSKFormat &tracks)
{
	if (tracks.empty())
		return;

	auto_ptr<CCPCUnknownDisc> disc(dynamic_cast<CCPCUnknownDisc*>(CCPCDisc::CreateDisc(filename, CCPCDisc::Unknown, 0)) );
	
	if (disc.get() == NULL)
		return;

	int nbTrack = tracks.size() / nbSide;
	disc->DiscGeometry().dg_cylinders = nbTrack;
	disc->DiscGeometry().dg_fmtgap = tracks[0].Gap3;
	disc->DiscGeometry().dg_rwgap = tracks[0].RWGap;
	disc->DiscGeometry().dg_heads = nbSide;
	if (! tracks[0].SectorArray.empty())
	{
		disc->DiscGeometry().dg_secbase = tracks[0].SectorArray[0].SectorID;
		disc->DiscGeometry().dg_secsize = tracks[0].SectorArray[0].SectorSize;
	}
	for (int t=0 ; t<nbTrack ; t++)
	{
		for (int s=0 ; s<nbSide ; s++)
		{
			const CCPCUnknownDisc::CCPCTrackFormat &trackFormat = tracks[t * nbSide + s];
			disc->FormatTrack(t, s, trackFormat);
			cout << "Format track " << t << "[" << s << "], NbSect " << trackFormat.SectorArray.size() << ", Gap3 #" << hex << (int)trackFormat.Gap3 << dec;
			if (trackFormat.SectorArray.size() == 0)
			{
				cout << endl << endl;
				continue;
			}
			bool allSameSize = true;
			bool allSameTrack = true;
			bool allSameHead = true;
			for (int i=1 ; i< trackFormat.SectorArray.size() ; i++)
			{
				allSameSize = allSameSize && (trackFormat.SectorArray[0].SectorSize == trackFormat.SectorArray[i].SectorSize);
				allSameTrack = allSameTrack && (trackFormat.SectorArray[0].Track == trackFormat.SectorArray[i].Track);
				allSameHead = allSameHead && (trackFormat.SectorArray[0].Head == trackFormat.SectorArray[i].Head);
			}

			if (allSameSize)
			{
				cout << " SectSize " << trackFormat.SectorArray[0].SectorSize;
			}
			if (allSameTrack)
			{
				if (trackFormat.SectorArray[0].Track != -1)
					cout << " SecTrack " << trackFormat.SectorArray[0].Track;
				else
					cout << " SecTrack " << t;
			}
			if (allSameHead)
			{
				if (trackFormat.SectorArray[0].Head != -1)
					cout << " SecHead " << trackFormat.SectorArray[0].Head;
				else
					cout << " SecHead " << s;
			}
			cout << endl << "\t";
			for (i=0 ; i< trackFormat.SectorArray.size() ; i++)
			{
				cout << hex << "#" << (int)trackFormat.SectorArray[i].SectorID << dec;
				if ( !(allSameSize && allSameTrack && allSameHead) )
				{
					cout << " [";
				}
				if (!allSameSize)
				{
					cout << "S:" << trackFormat.SectorArray[i].SectorSize << " ";
				}
				if (!allSameTrack)
				{
					cout << "T:" << trackFormat.SectorArray[i].Track << " ";
				}
				if (!allSameHead)
				{
					cout << "H:" << trackFormat.SectorArray[i].Head << " ";
				}
				if ( !(allSameSize && allSameTrack && allSameHead) )
				{
					cout << "]";
				}
				if (i < trackFormat.SectorArray.size()-1)
				{
					cout << ",";
				}
			}
			cout << endl << endl;
		}
	}

	disc->Close();
}

string StringFindAllReplace(const string &str, const string &from, const string &to)
{
	int pos;
	if ((pos = str.find(from)) == string::npos)
		return str;

	string finalEnd = str.substr(pos + from.size(), str.size()-(pos + from.size()));
	
	string final = str.substr(0,pos) + to + StringFindAllReplace(finalEnd, from, to);

	return final;
}

void PutDSKFilesOutput(ostream &outStr, const string &filename, CCPCUnknownDisc::FileSectorList &fileSector, const string &prefix, const string &headerMask)
{
	string file = filename;
	int pos;
	if ((pos = file.find_last_of('\\')) != string::npos)
	{
		file = file.substr(pos+1, file.size() - pos - 1);
	}
	else if ((pos = file.find_last_of('/')) != string::npos)
	{
		file = file.substr(pos+1, file.size() - pos - 1);
	}

	int nbFilter = filenameFilterList.size();
	for (int i=0 ; i<nbFilter ; i++)
	{
		string str;
		str += (char)filenameFilterList[i];
		file = StringFindAllReplace(file, str, "_");
	}

	string header = StringFindAllReplace(headerMask, "%FILE%", file);

	header = StringFindAllReplace(header, "%PREFIX%", prefix);

	{
		char str[32];
		ostrstream ostr(str,32);
		ostr << fileSector[0].Track << ends;
		header = StringFindAllReplace(header, "%TRACK%", ostr.str());
	}

	{
		char str[32];
		ostrstream ostr(str,32);
		ostr << hex << (int)fileSector[0].SectorID << dec << ends;
		header = StringFindAllReplace(header, "%SECTOR%", ostr.str());
	}

	{
		char str[32];
		ostrstream ostr(str,32);
		ostr << fileSector[0].Side << ends;
		header = StringFindAllReplace(header, "%HEAD%", ostr.str());
	}

	{
		char str[32];
		ostrstream ostr(str,32);
		ostr << fileSector.size() << ends;
		header = StringFindAllReplace(header, "%SIZE%", ostr.str());
	}

	outStr << header;
}

void PutDSKFiles(const string &filename, 
				 const vector<string> &fileList, 
				 vector<CCPCUnknownDisc::FileSectorList> &fileSectorList,
				 int startTrack, int side, unsigned char startSectorID, bool useDoubleSide)
{
	if (fileList.empty())
		return;
	
	auto_ptr<CCPCUnknownDisc> disc(dynamic_cast<CCPCUnknownDisc*>(CCPCDisc::OpenDisc(filename, 0)) );
	
	if (disc.get() == NULL)
		return;

	int track = startTrack;
	unsigned char sectorID = (startSectorID == 255) ? disc->GetDiscGeometry().dg_secbase : startSectorID;

	fileSectorList.resize(fileList.size());

	for (int f=0 ; f<fileList.size() ; f++)
	{
		cout << "Putting raw file " << fileList[f] << endl;
		disc->PutRawFile(fileList[f], track, side, sectorID, fileSectorList[f], useDoubleSide);

		cout << "Nb sectors " << fileSectorList[f].size() << endl;

		for (int s=0 ; s < fileSectorList[f].size() ; s++)
		{
			cout << "T" << fileSectorList[f][s].Track << "[" << fileSectorList[f][s].Side << "], #" << hex << (int)fileSectorList[f][s].SectorID << dec << "\t";
		}
		cout << endl;
		cout << endl;
	}

	cout << "Next sector : T" << track << "[" << side << "], #" << hex << (int)sectorID << endl;

	disc->Close();
}

void DumpDirectory(const string &filename)
{
	auto_ptr<CCPCDataDisc> dsk(dynamic_cast<CCPCDataDisc*>(CCPCDisc::OpenDisc(filename, 0)));

	if (dsk.get() != NULL)
	{
		unsigned char directory[2048];
		dsk->ReadBlock(0, directory);
		dsk->ReadBlock(1, directory+1024);

		int idx = 0;
		int nbOctLine = 16;
		for (int i=0 ; i<(2048/nbOctLine) ; i++)
		{
			if ( (i & 1) == 1)
			{
				idx += nbOctLine;
				continue;
			}

			int oldIdx = idx;
			for (int j=0 ; j<nbOctLine ; j++)
			{
				if (j >= 0x01 && j < 0x0c)
				{
					cout.setf(std::ios::hex, std::ios::basefield);
					cout.fill('0');
					cout.width(2);
					cout << (int)directory[idx++] << ".";
				}
				else
				{
					idx++;
				}
			}
			
			idx = oldIdx;
			
			cout << "\t";

			for (j=0 ; j<nbOctLine ; j++)
			{
				if (j >= 0x01 && j < 0x0c)
				{
					unsigned char val = directory[idx] & 0x7f;
					val = (val < 32) ? '?' : val;
					cout << val;
				}
				idx++;
			}
			cout << endl;
		}
	}
}

int main (int argc, char **argv)
{
	try
	{
		COptionParser optParser(appliOption, appliName, appliUsageShort, appliUsageLong, authorName, authorMail);
		
		optParser.PrintHeader(cout);

		if (!optParser.ParseCommandLine(&argc, &argv, 2, true))
		{
			optParser.PrintError(cout);
			optParser.PrintUsage(cout);
			return -1;
		}

		int nbSide = 1;

		switch (argv[2][0])
		{
		case 'd' :
			{
				DumpDSKFile(argv[1]);
				break;
			}
		case 'f' :
			{
				DSKFormat tracks;
				int nbSide = optParser.GetIntOption('s', 1);

				if (optParser.FindOption('i'))
				{
					InitFormat(optParser.GetStringOption('i', ""), tracks, nbSide);
				}
				else
				{
					CCPCUnknownDisc::CCPCTrackFormat trackFormat;
					int nbTrack;

					CCPCDisc::TDisc format = CCPCDisc::Data42;
					string formatName = optParser.GetStringOption('f', "cpcdata42");
					for (int f=0 ; CCPCDisc::TDiscDesc[f] != NULL ; f++)
					{
						if (formatName == CCPCDisc::TDiscDesc[f])
						{
							format = (CCPCDisc::TDisc)f;
							break;
						}
					}

					GetTrackFormat(format, nbTrack, trackFormat);

					nbTrack = optParser.GetIntOption('t', nbTrack);
					int nbSector = optParser.GetIntOption('n', trackFormat.SectorArray.size());
					int secBase = optParser.GetIntOption('b', trackFormat.SectorArray[0].SectorID);
					int secSize = optParser.GetIntOption('e', trackFormat.SectorArray[0].SectorSize);
					bool interlaced = !optParser.FindOption('j');

					if (nbSector != trackFormat.SectorArray.size() ||
						secBase != trackFormat.SectorArray[0].SectorID ||
						secSize != trackFormat.SectorArray[0].SectorSize ||
						!interlaced)
					{
						trackFormat = CCPCUnknownDisc::CCPCTrackFormat(nbSector, secBase, secSize, interlaced);
					}

					trackFormat.Gap3 = optParser.GetIntOption('g', trackFormat.Gap3);
					trackFormat.RWGap = optParser.GetIntOption('r', trackFormat.RWGap);

					InitFormat(nbTrack, nbSide, trackFormat, tracks);
				}

				FormatDSK(argv[1], nbSide, tracks);
				break;
			}
		case 'p':
			{
				// put file
				int startTrack = optParser.GetIntOption('a', 0);
				int startSide = optParser.GetIntOption('p', 0);
				int startSectorID = optParser.GetIntOption('o', -1);
				bool useDoubleSide = optParser.FindOption('d');
				string prefix = optParser.GetStringOption('m',"");

				vector<string> fileList;
				for (int f=3 ; f<argc ; f++)
				{
					fileList.push_back(argv[f]);
				}

				vector<CCPCUnknownDisc::FileSectorList> fileSectorList;
				PutDSKFiles(argv[1], fileList, fileSectorList, startTrack, startSide, startSectorID, useDoubleSide);

				if (optParser.FindOption('z'))
				{
					string z80Output = optParser.GetStringOption('z', "");

					ofstream z80Out(z80Output.c_str());
					TOOLS_ASSERTMSG( z80Out.good(), "Error while creating \"" << z80Output << "\" file");

					for (f=0 ; f<fileList.size() ; f++)
					{
						PutDSKFilesOutput(z80Out, fileList[f], fileSectorList[f], prefix, outputHeaderMask);
					}
				}

				break;
			}
		case 'x':
			{
				DumpDirectory(argv[1]);

				break;
			}
		default :
			{
				break;
			}
		}
	}
	catch(tools::CException &e)
	{
		cerr << e << endl;
		return -1;
	}

	return 0;
}
