/*
 * Class for parsing command line
 * (c) Ramlaid 2006
 */

#include "COptionParser.h"
#include "CError.h"
#include <errno.h>

COptionParser::COptionParser() :
_options(NULL),
_optionsOut(NULL),
_applicationName(""),
_applicationShortUsage(""),
_applicationLongUsage(""),
_author(""),
_authorMail("")
{
}

COptionParser::COptionParser(const SOption *opt, const std::string &appliName, const std::string &appliShortUsage, const std::string &appliLongUsage, const std::string &author, const std::string &authorMail) :
_options(opt),
_optionsOut(NULL),
_applicationName(appliName),
_applicationShortUsage(appliShortUsage),
_applicationLongUsage(appliLongUsage),
_author(author),
_authorMail(authorMail)
{
}

COptionParser::~COptionParser()
{
	if (_optionsOut != NULL)
		freeOptionOut(_optionsOut);
	_optionsOut = NULL;
}

bool COptionParser::ParseCommandLine(int *argc, char ***argv, int nbMinArg, bool useConfig)
{
	return (parseCommandLine(argc, argv, const_cast<SOption*>(_options), &_optionsOut, nbMinArg, useConfig ? 0 : 1) == NO_ERROR);
}

bool COptionParser::FindOption(const char shortName) const
{
	return (optionIndex(_optionsOut, shortName) != ERROR_UNKNOWN);
}
int COptionParser::GetOptionNbParam(const char shortName) const
{
	int val = 0;
	int idx;
	if ((idx = optionIndex(_optionsOut, shortName)) != ERROR_UNKNOWN)
	{
		val = _optionsOut[idx].argc;
	}
	return val;
}
int COptionParser::GetIntOption(const char shortName, const int defaultVal, int paramIdx) const
{
	int val = defaultVal;
	int idx;
	if ((idx = optionIndex(_optionsOut, shortName)) != ERROR_UNKNOWN)
	{
		val = strtol(_optionsOut[idx].argv[paramIdx],NULL,0);
		TOOLS_ASSERTMSG(errno != ERANGE, "Wrong value " << _optionsOut[idx].argv[paramIdx]);
	}
	return val;
}

std::string COptionParser::GetStringOption(const char shortName, const std::string &defaultVal, int paramIdx) const
{
	std::string val = defaultVal;
	int idx;
	if ((idx = optionIndex(_optionsOut, shortName)) != ERROR_UNKNOWN)
	{
		val = _optionsOut[idx].argv[paramIdx];
	}
	return val;
}

char COptionParser::GetOptionI(const int idx) const
{
	return _optionsOut[idx].shortName;
}
int COptionParser::GetOptionINbParam(const int idx) const
{
	return _optionsOut[idx].argc;
}
int COptionParser::GetIntOptionI(const int idx, int paramIdx) const
{
	int val = strtol(_optionsOut[idx].argv[paramIdx],NULL,0);
	TOOLS_ASSERTMSG(errno != ERANGE, "Wrong value " << _optionsOut[idx].argv[paramIdx]);
	return val;
}
std::string COptionParser::GetStringOptionI(const int idx, int paramIdx) const
{
	return _optionsOut[idx].argv[paramIdx];
}


void COptionParser::PrintHeader(std::ostream &str) const
{
	str << _applicationName << " (c) " << _author << " (" << __DATE__ << ")" << std::endl << std::endl;
}
void COptionParser::PrintError(std::ostream &str) const
{
	str << getErrorString() << std::endl;
}
void COptionParser::PrintUsage(std::ostream &str) const
{
	int i=0;
	int j=0;
	str << "Usage : " << _applicationName << " " << _applicationShortUsage << " ";
	while (_options[i].shortName !=0)
	{
		if (_options[i].nbMin == 0)
			str << "[";
		else
			str << "<";
		str << "-" << _options[i].shortName;
		for (j=0;j<_options[i].nbArgs;j++)
			str << " $" << j+1;
		if (_options[i].nbMin == 0)
		{
			str << "]";
			if (_options[i].nbMax == 0)
				str << "*";
		}
		else
		{
			str << ">";
			if (_options[i].nbMax == 0)
				str << "+";
		}
		str << " ";
		i++;
	}
	str << "[-h]";
	str << std::endl << std::endl << _applicationName <<  _applicationLongUsage << std::endl << std::endl;
	i = 0;
	while (_options[i].shortName !=0)
	{
		str << "\t-" << _options[i].shortName;
		for (j=0;j<_options[i].nbArgs;j++)
			str << " <$" << j+1 << ">";
		str << " | --" << _options[i].longName;
		for (j=0;j<_options[i].nbArgs;j++)
			str << " <$" << j+1 << ">";
		str << "\n\t\t" << _options[i].description << std::endl;
		i++;
	}
	str << "\t-h | --help\n\t\tDisplay help" << std::endl << std::endl;

	str << "Mail : " << _authorMail << std::endl;
}
