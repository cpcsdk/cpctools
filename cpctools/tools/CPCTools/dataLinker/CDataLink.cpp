/**
 * @class CDataLink
 * Classe permettant la gestion d'un data pour le linker
 * @author Thierry JOUIN
 * @version 1.1
 * @date 04/08/2003
 */

#include "CDataLink.h"
#include <iostream>
#include <fstream>
#include <cstring>

#include "CError.h"

CDataLink::CDataLink()
:mFilename(""),
mLabel(""),
mSize(0),
mMask(0xffff),
mData(NULL)
{
}
CDataLink::CDataLink(std::string &i_filename,std::string &i_label, int i_mask)
:mFilename(i_filename),
mLabel(i_label),
mSize(0),
mMask(i_mask),
mData(NULL)
{
}
CDataLink::CDataLink(const CDataLink &i_data)
:mFilename(i_data.mFilename),
mLabel(i_data.mLabel),
mSize(i_data.mSize),
mMask(i_data.mMask)
{
	mData = new unsigned char[mSize];
	memcpy(mData,i_data.mData,mSize);
}
CDataLink::~CDataLink()
{
	if (mData != NULL)
	{
		delete[] mData;
	}
}

CDataLink& CDataLink::operator=(const CDataLink &i_data)
{
	if (mData != NULL)
	{
		delete[] mData;
	}

	mFilename = i_data.mFilename;
	mLabel = i_data.mLabel;
	mSize = i_data.mSize;
	mMask = i_data.mMask;
	if (i_data.mData != NULL)
	{
		mData = new unsigned char[mSize];
		memcpy(mData,i_data.mData,mSize);
	}
	else
	{
		mData = NULL;
	}

	return *this;
}

void CDataLink::loadFile()
{
	std::ifstream in(mFilename.c_str(),std::ios::binary);

	TOOLS_ASSERTMSG(in.good(), "Unable to open " << mFilename.c_str());

	in.seekg(0,std::ios::end);
	mSize = in.tellg();
	in.seekg(0,std::ios::beg);

	mData = new unsigned char[mSize];
	in.read((char*)mData,mSize);
}

int CDataLink::getSize() const
{
	return mSize;
}
int CDataLink::getMask() const
{
	return mMask;
}
std::string CDataLink::getLabel()
{
	return mLabel;
}
unsigned char* CDataLink::getDatas()
{
	return mData;
}

CDataLink CDataLink::NullData(unsigned int i_size)
{
	CDataLink null;
	null.mData = NULL;
	null.mLabel = "";
	null.mFilename = "";
	null.mSize = i_size;
	null.mMask = 0xffff;

	return null;
}
