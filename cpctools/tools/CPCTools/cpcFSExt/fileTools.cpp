//
// Generic file tools
//

#include "StdAfx.h"

#include "fileTools.h"

bool FileTool::IsFileExist(const string &file)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(file.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return false;
	}
	else
	{
		FindClose(hFind);
		return ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0);
	}
	return false;
}

bool FileTool::IsFileReadOnly(const string &file)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(file.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return false;
	}
	else
	{
		FindClose(hFind);
		return (((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) &&
				((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0));
	}
	return false;
}

bool FileTool::IsDirectoryExist(const string &file)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(file.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return false;
	}
	else
	{
		FindClose(hFind);
		return ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
	}
	return false;
}

void FileTool::FillFileList(const string &directory, const string &filter, vector<string> &fileList)
{
	fileList.clear();

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	string dir = directory;
	string dirSpec = directory;
	if (dir[dir.size()-1] == '\\')
	{
		dirSpec += filter;
	}
	else
	{
		dir += "\\";
		dirSpec += "\\"+filter;
	}
	
	hFind = FindFirstFile(dirSpec.c_str(), &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		string file = dir+FindFileData.cFileName;
		if (file != "." && file != ".."&&
			(GetFileAttributes(file.c_str()) & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			fileList.push_back(file);
		}
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			file = dir+FindFileData.cFileName;
			DWORD attrib = GetFileAttributes(file.c_str());
			if (file != "." && file != ".."&&
				(attrib & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				fileList.push_back(file);
			}
		}

		FindClose(hFind);
	}
}
