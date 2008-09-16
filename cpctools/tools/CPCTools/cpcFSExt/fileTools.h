//
// Generic file tools
//

#pragma once
#define _FILE_TOOLS_H_

#include <string>
#include <vector>

using namespace std;

class FileTool
{
public:
	//! Check if file exists
	static bool IsFileExist(const string &file);
	//! Check if file exists and is Read Only
	static bool IsFileReadOnly(const string &file);
	//! Check if directory exists
	static bool IsDirectoryExist(const string &file);
	//! Fill fileList with files in directory
	static void FillFileList(const string &directory, const string &filter, vector<string> &fileList);
};