/* Reloded an Amstrad CPC emulator
   (c) Copyright 2008-2010  cpcsdk crew
   Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2004 Ulrich Doewich

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef _WXVIDEO_H_
#define _WXVIDEO_H_

#include <wx/image.h>

class WXDoubleLinePlugin : public VideoPlugin
{
public:
	WXDoubleLinePlugin() 
		: VideoPlugin("Double size", ALL) 
		, img(NULL)
		, bmp(NULL)
	{}

	virtual void* Init(int w,int h,int bpp,bool fs);
	//virtual void SetPalette(SDL_Color* c);
	virtual void SetPalette(ColorARGB8888* c);

	virtual bool TryLock();
	virtual void Unlock();
	virtual void Flip();
	virtual void Close();

    virtual bool LockOutput();
    virtual bool TryLockOutput();
    virtual void UnlockOutput();

    // Detect if output have been updated since last IsUpdate call
    virtual bool IsUpdate();

	void Screenshot(string filename);

	wxImage* img;
	wxBitmap* bmp;
};

#if 0
#include "glfuncs.h"

class OpenGLPlugin : public VideoPlugin
{
protected:
	int		_texWidth;
	int		_texHeight;
	GLuint	_screenTexnum;
	GLuint	_modulateTexnum;
	int		_GLScanlines;

	bool	_openGLFilter;
	bool	_remanency;
public:
/*	OpenGLPlugin(const string &name, const Uint32 &format, const Uint8 &halfPixels) :
	  VideoPlugin(name, format, halfPixels) {}*/

	OpenGLPlugin() : VideoPlugin("OpenGL scaling", ALL, 1) {}

	virtual void SetPalette(ColorARGB8888* c);
	virtual bool Lock();
	virtual void Unlock();
	virtual void Flip();
	virtual void Close();

	virtual void SetOption(const string &optionName, bool val);
protected:
	SDL_Surface* OpenGLInit(int w,int h, int bpp, bool fs, int glScanline);
	inline void* Init(int w,int h, int bpp, bool fs)
	{
		return OpenGLInit(w,h,bpp,fs,0);
	}
};
#endif // #if 0
#endif
