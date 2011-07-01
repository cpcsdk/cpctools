/*
 * Class used for registry access
 * (c) Ramlaid 2006
 */

#pragma once
#define _CREGISTRY_H_

#ifndef NOREGISTRY
#include <MAPIDEFS.h>
#include <lmcons.h>
#endif

#include <string>

class CRegistryKey
{
protected:
#ifndef NOREGISTRY
	//! Child registry key
	CRegistryKey	*_child;
	//! Current Registry key
	HKEY			_hkey;
#endif

public:
	//! Constructor
	CRegistryKey(const std::string &path);
	//! Destructor
	~CRegistryKey();

	//! Return value if found
	bool GetValue(const std::string &name, std::string &value);
	//! Return value if found
	bool GetValue(const std::string &name, int &value);
	//! Set the value
	bool SetValue(const std::string &name, const std::string &value);
	//! Set the value
	bool SetValue(const std::string &name, const int value);

protected:
#ifndef NOREGISTRY
	//! Constructor
	CRegistryKey(HKEY parentKey, const std::string &path);
	//! Create key registry
	void CreateHKEY(HKEY parentKey, const std::string &path);

	//! Return registry HKey
	HKEY GetHKEY(void);
#endif
};
