/*
 * Classe de visualisation de window
 */

#include "CCPCFontView.h"
#include "CError.h"

CCPCFontView::CCPCFontView(unsigned char* data, unsigned int size, unsigned int w, unsigned int h, unsigned int nbChar)
:CCPCDataView(data,size),
_charWidth(w),
_charHeight(h),
_nbChar(nbChar),
_verticalMode(false),
_displayBox(true)
{
}

CCPCFontView::CCPCFontView(const CCPCDataView &data, unsigned int w, unsigned int h, unsigned int nbChar)
:CCPCDataView(data),
_charWidth(w),
_charHeight(h),
_nbChar(nbChar),
_verticalMode(false),
_displayBox(true)
{
}
	
bool CCPCFontView::display(CCPCVideo &scr)
{
	forceFontSize(scr);

	int r1 = scr.GetCRTCValue(1);
	//int r6 = scr.GetCRTCValue(6);
	int widthMax = r1*2;
	//int heightMax = r6*8;
	unsigned int nbCharWMax = (widthMax - (widthMax % _charWidth)) / _charWidth;
	//int nbCharHMax = (heightMax - (heightMax % _charHeight)) / _charHeight;
	int nbCharW = (_nbChar < nbCharWMax) ? _nbChar : nbCharWMax;
	int nbCharH = (_nbChar < nbCharWMax) ? 1 : (((_nbChar - (_nbChar % nbCharWMax))/nbCharWMax)+1);
	
	unsigned char *win = new unsigned char[nbCharW*_charWidth*nbCharH*_charHeight];
	memset(win,0,nbCharW*_charWidth*nbCharH*_charHeight);

	unsigned int ichar=0;
	int offData=0;
	int wWidth = nbCharW*_charWidth;
	for (int cH=0;cH<nbCharH;cH++)
		for (int cW=0;cW<nbCharW;cW++)
		{
			if (ichar < _nbChar)
			{
				if (_verticalMode)
				{
					int cX = cW * _charWidth;
					int cY = cH * _charHeight;
					for (unsigned int x=0;x<_charWidth;x++)
						for (unsigned int y=0;y<_charHeight;y++)
						{
							int offWin = (cY+y)*wWidth + cX+x;
							win[offWin] = _data[offData++];
						}
				}
				else
				{
					int cX = cW * _charWidth;
					int cY = cH * _charHeight;
					for (unsigned int y=0;y<_charHeight;y++)
						for (unsigned int x=0;x<_charWidth;x++)
						{
							int offWin = (cY+y)*wWidth + cX+x;
							win[offWin] = _data[offData++];
						}
				}
			}
			ichar++;
		}

	scr.Cls();
	scr.PutWindow(0,win,nbCharW*_charWidth,nbCharH*_charHeight);
	scr.Display();
	
	if (_displayBox)
	{
		scr.BeginOverdraw();
		unsigned int mode = scr.GetMode();
		switch(mode)
		{
		case 0:
			{
				for (int cH=0;cH<nbCharH+1;cH++)
					scr.OverdrawHLine(cH*_charHeight,0,(nbCharW*_charWidth*2),196,196,196);
				for (int cW=0;cW<nbCharW+1;cW++)
					scr.OverdrawVLine(cW*_charWidth*2,0,nbCharH*_charHeight,196,196,196);
				break;
			}
		case 1:
			{
				for (int cH=0;cH<nbCharH+1;cH++)
					scr.OverdrawHLine(cH*_charHeight,0,(nbCharW*_charWidth*4),196,196,196);
				for (int cW=0;cW<nbCharW+1;cW++)
					scr.OverdrawVLine(cW*_charWidth*4,0,nbCharH*_charHeight,196,196,196);
				break;
			}
		case 2:
			{
				for (int cH=0;cH<nbCharH+1;cH++)
					scr.OverdrawHLine(cH*_charHeight,0,(nbCharW*_charWidth*8),196,196,196);
				for (int cW=0;cW<nbCharW+1;cW++)
					scr.OverdrawVLine(cW*_charWidth*8,0,nbCharH*_charHeight,196,196,196);
				break;
			}
		}
		scr.EndOverdraw();
	}

	delete[] win;
	return false;
}

bool CCPCFontView::keyPressed(CCPCVideo &scr, const SDLKey &key, const SDLMod &mod, bool &redisplay)
{
	switch (mod & 0xfff)
	{
	case KMOD_NONE:
		{
			switch (key)
			{
			case SDLK_UP:	{_charHeight--;redisplay = true;return true;}
			case SDLK_DOWN:	{_charHeight++;redisplay = true;return true;}
			case SDLK_LEFT:	{_charWidth--;redisplay = true;return true;}
			case SDLK_RIGHT:{_charWidth++;redisplay = true;return true;}
			case SDLK_d:	{_displayBox = !_displayBox;redisplay = true;return true;}
			case SDLK_v:	{_verticalMode = true;redisplay = true;return true;}
			case SDLK_h:	{_verticalMode = false;redisplay = true;return true;}
			default:{return false;}
			}
			return false;
		}
	case KMOD_RSHIFT:
	case KMOD_LSHIFT:
		{
			switch (key)
			{
			case SDLK_LEFT:	{_nbChar--;redisplay = true;return true;}
			case SDLK_RIGHT:{_nbChar++;redisplay = true;return true;}
			default:{return false;}
			}
			return false;
		}
	default:{return false;}
	}

	return false;
}

void CCPCFontView::forceFontSize(CCPCVideo &scr)
{
	int r1 = scr.GetCRTCValue(1);
	int r6 = scr.GetCRTCValue(6);
	unsigned int widthMax = r1*2;
	unsigned int heightMax = r6*8;

	_charWidth = (_charWidth < widthMax) ? _charWidth : widthMax;
	_charHeight = (_charHeight < heightMax) ? _charHeight : heightMax;
	_charWidth = (_charWidth > 1) ? _charWidth : 1;
	_charHeight = (_charHeight > 1) ? _charHeight : 1;

	if (_charWidth < _dataSize)
	{
		unsigned int charHeightMax = ((_dataSize -(_dataSize % _charWidth)) / _charWidth);
		_charHeight = (_charHeight < charHeightMax) ? _charHeight : charHeightMax;
	}
	else
	{
		_charWidth = _dataSize;
	}

	int charSize = _charWidth * _charHeight;
	unsigned int maxChar = ((_dataSize -(_dataSize % charSize)) / charSize);
	_nbChar = (_nbChar > 1) ? _nbChar : 1;
	_nbChar = (_nbChar < maxChar) ? _nbChar : maxChar;


	int nbCharWMax = (widthMax - (widthMax % _charWidth)) / _charWidth;
	int nbCharHMax = (heightMax - (heightMax % _charHeight)) / _charHeight;
	unsigned int nbCharMax = (nbCharWMax*nbCharHMax);
	_nbChar = (_nbChar < nbCharMax) ? _nbChar : nbCharMax;
}
