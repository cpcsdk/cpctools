/*
 * Classe de visualisation de window
 */

#include "CCPCWindowView.h"
#include <iostream>

CCPCWindowView::CCPCWindowView(unsigned char* data, unsigned int size, unsigned int w, unsigned int h)
:CCPCDataView(data,size),
_width(w),
_height(h),
_displayBox(true)
{
}

CCPCWindowView::CCPCWindowView(const CCPCDataView &data, unsigned int w, unsigned int h)
:CCPCDataView(data),
_width(w),
_height(h),
_displayBox(true)
{
}
	
bool CCPCWindowView::display(CCPCVideo &scr)
{
	forceWindowSize(scr);

	scr.Cls();
	scr.PutWindow(0,_data,_width,_height);
	scr.Display();
	if (_displayBox)
	{
		scr.BeginOverdraw();
		unsigned int mode = scr.GetMode();
		switch(mode)
		{
		case 0:{scr.OverdrawWindow(0,0,_width*2,_height,196,196,196);break;}
		case 1:{scr.OverdrawWindow(0,0,_width*4,_height,196,196,196);break;}
		case 2:{scr.OverdrawWindow(0,0,_width*8,_height,196,196,196);break;}
		}
		scr.EndOverdraw();
	}
	return false;
}

bool CCPCWindowView::keyPressed(CCPCVideo &scr, const SDLKey &key, const SDLMod &mod, bool &redisplay)
{
	switch (mod & 0xfff)
	{
	case KMOD_NONE:
		{
			switch (key)
			{
			case SDLK_UP:	{_height--;redisplay = true;return true;}
			case SDLK_DOWN:	{_height++;redisplay = true;return true;}
			case SDLK_LEFT:	{_width--;redisplay = true;return true;}
			case SDLK_RIGHT:{_width++;redisplay = true;return true;}
			/*
			case SDLK_F5:
			{
				std::string filename = "output.scr";
				scr.SaveBytes(0x0000,filename,0x4000);
				break;
			}
			case SDLK_F6:
			{
				std::string filename = "output.win";
				switch(scr.GetMode())
				{
				case Mode0:{scr.SaveWindow(filename,0,0,_width*2,_height);break;}
				case Mode1:{scr.SaveWindow(filename,0,0,_width*4,_height);break;}
				case Mode2:{scr.SaveWindow(filename,0,0,_width*8,_height);break;}
				}
				break;
			}
			*/
			case SDLK_F7:{
				int wpix;
				switch(scr.GetMode())
				{
				case 0:{wpix = _width*2;break;}
				case 1:{wpix = _width*4;break;}
				case 2:{wpix = _width*8;break;}
				}
				std::cout << "Windows size " << wpix << " (" << _width << "bytes) * " << _height << " in mode " << scr.GetMode() << std::endl;
			}
			case SDLK_d:	{_displayBox = !_displayBox;redisplay = true;return true;}
			default:{return false;}
			}
			return false;
		}
	case KMOD_RSHIFT:
	case KMOD_LSHIFT:
		{
			switch (key)
			{
			case SDLK_UP:	{_height-=8;redisplay = true;return true;}
			case SDLK_DOWN:	{_height+=8;redisplay = true;return true;}
			case SDLK_LEFT:	{_width-=8;redisplay = true;return true;}
			case SDLK_RIGHT:{_width+=8;redisplay = true;return true;}
			default:{return false;}
			}
			return false;
		}
	default:{return false;}
	}

	return false;
}

void CCPCWindowView::forceWindowSize(CCPCVideo &scr)
{
	//int r1,r6;
	//scr.getCRTCValue(r1,r6);
	//int widthMax = r1*2;
	//int heightMax = r6*8;

	//_width = (_width < widthMax) ? _width : widthMax;
	//_height = (_height < heightMax) ? _height : heightMax;
	_width = (_width > 1) ? _width : 1;
	_height = (_height > 1) ? _height : 1;

	if (_width < _dataSize)
	{
		int heightMax = ((_dataSize -(_dataSize % _width)) / _width);
		_height = (_height < heightMax) ? _height : heightMax;
	}
	else
	{
		_width = _dataSize;
	}
}