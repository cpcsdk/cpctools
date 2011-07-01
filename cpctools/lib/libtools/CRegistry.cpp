/*
 * Class used for registry access
 * (c) Ramlaid 2006
 */

#include "CRegistry.h"

#ifndef NOREGISTRY
CRegistryKey::CRegistryKey(const std::string &path)
{
	CreateHKEY(NULL, path);
}

CRegistryKey::CRegistryKey(HKEY parentKey, const std::string &path)
{
	CreateHKEY(parentKey, path);
}

CRegistryKey::~CRegistryKey()
{
	if (_child)
		delete _child;
	RegCloseKey(_hkey);
}

void CRegistryKey::CreateHKEY(HKEY parentKey, const std::string &path)
{
	unsigned long	exist;

	_child = NULL;

	int pos = path.find('/');
	if (pos == std::string::npos)
		pos = path.find('\\');

	std::string lpath = path.substr(0, pos);
	std::string lpath2 = path.substr(pos+1, path.size() - pos + 1);

	if(lpath == std::string("HKEY_CLASSES_ROOT"))
		_hkey = HKEY_CLASSES_ROOT;
	else if(lpath == std::string("HKEY_CURRENT_USER"))
		_hkey = HKEY_CURRENT_USER;
	else if(lpath == std::string("HKEY_LOCAL_MACHINE"))
		_hkey = HKEY_LOCAL_MACHINE;
	else if(lpath == std::string("HKEY_USERS"))
		_hkey = HKEY_USERS;
	else
	{
		LONG r = RegCreateKeyEx(parentKey, lpath.c_str(), 0, "", REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS, NULL, &_hkey, &exist);
	}

	if(pos != std::string::npos)
	{
		_child = new CRegistryKey (_hkey, lpath2);
	}
}

HKEY CRegistryKey::GetHKEY(void)
{
	CRegistryKey* r;
	for (r = this; r->_child != NULL; r = r->_child);

	return(r->_hkey);
}

bool CRegistryKey::GetValue(const std::string &name, std::string &value)
{
	unsigned char buff[4096];
	unsigned long buffSize = 4096;
	unsigned long valueType;

	if ( RegQueryValueEx(GetHKEY(), name.c_str(), NULL, &valueType, buff, &buffSize) == ERROR_SUCCESS)
	{
		if (valueType == REG_SZ)
		{
			value = (char*)buff;
			return true;
		}
	}

	return false;
}
bool CRegistryKey::GetValue(const std::string &name, int &value)
{
	unsigned char buff[4096];
	unsigned long buffSize = 4096;
	unsigned long valueType;

	if ( RegQueryValueEx(GetHKEY(), name.c_str(), NULL, &valueType, buff, &buffSize) == ERROR_SUCCESS)
	{
		if (valueType == REG_DWORD)
		{
			value = *((int*)buff);
			return true;
		}
	}

	return false;
}

bool CRegistryKey::SetValue(const std::string &name, const std::string &value)
{
	if ( RegSetValueEx(GetHKEY(), name.c_str(), NULL, REG_SZ, (const unsigned char*)value.c_str(), value.size()+1) == ERROR_SUCCESS)
	{
		return true;
	}

	return false;
}
bool CRegistryKey::SetValue(const std::string &name, const int value)
{
	if ( RegSetValueEx(GetHKEY(), name.c_str(), NULL, REG_DWORD, (const unsigned char*)&value, sizeof(int)) == ERROR_SUCCESS)
	{
		return true;
	}

	return false;
}

#else
CRegistryKey::CRegistryKey(const std::string &path)
{
}

CRegistryKey::~CRegistryKey()
{
}

bool CRegistryKey::GetValue(const std::string &name, std::string &value)
{
	return false;
}
bool CRegistryKey::GetValue(const std::string &name, int &value)
{
	return false;
}

bool CRegistryKey::SetValue(const std::string &name, const std::string &value)
{
	return false;
}
bool CRegistryKey::SetValue(const std::string &name, const int value)
{
	return false;
}
#endif
