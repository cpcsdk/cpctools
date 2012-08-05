/*
 * Classe de visualisation de data (classe mere)
 */

#include "CError.h"
#include "CCPCDataView.h"

CCPCDataView::CCPCDataView()
:_data(NULL),
_dataSize(0)
{
}

CCPCDataView::CCPCDataView(unsigned char *data, unsigned int size)
:_data(new unsigned char[size]),
_dataSize(size)
{
	memcpy(_data,data,_dataSize);
}

CCPCDataView::CCPCDataView(const CCPCDataView &data)
:_data(new unsigned char[data._dataSize]),
_dataSize(data._dataSize)
{
	memcpy(_data,data._data,_dataSize);
}

CCPCDataView::~CCPCDataView()
{
	if (_data != NULL)
		delete[] _data;
}

CCPCDataView& CCPCDataView::operator=(const CCPCDataView &data)
{
	if (_data != NULL)
		delete[] _data;
	_data = new unsigned char[data._dataSize];
	_dataSize = data._dataSize;

	memcpy(_data,data._data,_dataSize);
	
	return *this;
}