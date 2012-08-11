/*
 * Classe de visualisation de screen
 */

#include "CCPCScreenView.h"

CCPCScreenView::CCPCScreenView(unsigned char* data, unsigned int size)
:CCPCDataView(data,size),
_offset(0)
{
}

CCPCScreenView::CCPCScreenView(const CCPCDataView &data)
:CCPCDataView(data),
_offset(0)
{
}
	
bool CCPCScreenView::display(CCPCVideo &scr)
{
	_offset = (_offset > 0) ? _offset : 0;

	scr.Cls();
	if (_offset < _dataSize)
		scr.PutBytes(0,(_data+_offset),(_dataSize-_offset));
	scr.Display();
	return false;
}

bool CCPCScreenView::keyPressed(CCPCVideo &scr, const SDLKey &key, const SDLMod &mod, bool &redisplay)
{
	int r1 = scr.GetCRTCValue(1);
	//int r6 = scr.GetCRTCValue(6);

	switch (mod & 0xfff)
	{
	case KMOD_NONE:
		{
			switch (key)
			{
			case SDLK_UP:	{_offset-=r1*2;redisplay = true;return true;}
			case SDLK_DOWN:	{_offset+=r1*2;redisplay = true;return true;}
			case SDLK_LEFT:	{_offset--;redisplay = true;return true;}
			case SDLK_RIGHT:{_offset++;redisplay = true;return true;}
			default:{return false;}
			}
			return false;
		}
	default:{return false;}
	}

	return false;
}
