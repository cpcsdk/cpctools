/*
 * Class for parsing command line
 * (c) Ramlaid 2006
 */

#pragma once
#define _COPTIONPARSER_H_

#include <string>
#include "optionParser.h"

class COptionParser
{
protected:
	//! Parsing options
	const SOption	*_options;
	//! Parsed options result
	SOptionOut		*_optionsOut;
	//! Parsed application name
	std::string		_applicationName;
	//! Parsed application short usage
	std::string		_applicationShortUsage;
	//! Parsed application long usage
	std::string		_applicationLongUsage;
	//! Application author name
	std::string		_author;
	//! Application author mail
	std::string		_authorMail;

public:
	//! Constructor using a config
	COptionParser(const SOption *opt, const std::string &appliName, const std::string &appliShortUsage, const std::string &appliLongUsage, const std::string &author, const std::string &authorMail);
	//! Destructor
	~COptionParser();

	//! Parse given command line and return true if everthing OK
	bool ParseCommandLine(int *argc, char ***argv, int nbMinArg, bool useConfig);

	//! Return true if option is finded
	bool FindOption(const char shortName) const;
	//! Return number of params for given option
	int GetOptionNbParam(const char shortName) const;
	//! Return int option if found
	int GetIntOption(const char shortName, const int defaultVal, int paramIdx = 0) const;
	//! Return string option if found
	std::string GetStringOption(const char shortName, const std::string &defaultVal, int paramIdx = 0) const;

	//! Return shortname of option
	char GetOptionI(const int idx) const;
	//! Return number of params for given option
	int GetOptionINbParam(const int idx) const;
	//! Return int option if found
	int GetIntOptionI(const int idx, int paramIdx = 0) const;
	//! Return string option if found
	std::string GetStringOptionI(const int idx, int paramIdx = 0) const;

	//! Print application header
	void PrintHeader(std::ostream &str) const;
	//! Print error message
	void PrintError(std::ostream &str) const;
	//! Print usage
	void PrintUsage(std::ostream &str) const;

private:
	//! Default constructor
	COptionParser();
};
