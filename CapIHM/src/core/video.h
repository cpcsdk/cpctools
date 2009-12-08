/* Caprice32 - Amstrad CPC Emulator
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

#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <SDL.h>
#include <string>
#include "cap32type.h"

using namespace std;

class VideoPlugin
{
public:
	enum VideoType
	{
//		Scanlines			= 0,
		DoubleSize			= 1,
//		DoubleWidth			= 2,
//		HalfSize			= 3,
//		HardwareHalfSize	= 4,
//		SuperEagle			= 5,
//		Scale2x				= 6,
//		AdvancedScale2x		= 7,
//		TVScale2x			= 8,
//		SoftwareBilinear	= 9,
//		SoftwareBicubic		= 10,
//		DotMatrix			= 11,
		OpenGLScale			= 12,
		OpenGLScale25		= 13,
		OpenGLScale50		= 14,
		OpenGLScale75		= 15,
		OpenGLScale100		= 16
	};
protected:
	//! the pixel formats video plugins can support
	enum VideoFormat
	{
		F8_BPP				= 1<<0,
		F15_BPP				= 1<<1,
		F15_BPP_REV			= 1<<2,
		F16_BPP				= 1<<3,
		F16_BPP_REV			= 1<<4,
		F24_BPP				= 1<<5,
		F24_BPP_REV			= 1<<6,
		F32_BPP				= 1<<7,
		F32_BPP_REV			= 1<<8,

		ALL					= 0xffffffff
	};

protected:
	static int CPCVisibleSCRWidth;
	static int CPCVisibleSCRHeight;

protected:
	//! Plugin name
	string			_name;
	// what you can feed to this plugin :
	//! the pixel formats supported
	dword			_formats;
	//! this plugin wants : 0 half sized pixels (320x200 screen)/1 full sized pixels (640x200 screen)*/
	byte			_halfPixels;

		//! Post render call back
	int				(*_postRenderCallBack)();

public:
		//! the video surface shown by the plugin to the application
	SDL_Surface*	_publicVideo;
//! the real video surface
	SDL_Surface*	_video;

//! Return halfsize config
	inline bool IsHalfSize() const	{ return _halfPixels == 1;	};

	//! initializes the video plugin ; returns the surface that you must draw into, NULL in the (unlikely ;) event of a failure
	virtual SDL_Surface* Init(int w,int h,int bpp,bool fs) = 0;
	
	virtual void SetPalette(SDL_Color* c) = 0;

	//! Return current surface width
	inline int GetWidth() const { return _video->w; }
	//! Return current surface width
	inline int GetHeight() const { return _video->h; }
	
	inline int GetRenderSurfaceWidth() const { return _publicVideo->w; }
	inline int GetRenderSurfaceHeight() const { return _publicVideo->h; }
	
	inline int GetRenderBPP() const { return _publicVideo->format->BitsPerPixel; }
	
	//! locks the surface if needed
	virtual bool Lock() = 0;
	//! unlocks the surface if needed
	virtual void Unlock() = 0;
	//! "flips" the video surface. Note that this might not always do a real flip
	virtual void Flip() = 0;
	//! closes the plugin
	virtual void Close() = 0;
	
	//! Set generic bool option
	virtual void SetOption(const string &optionName, bool val);
	
	//! Set post render call back
	inline void SetPostRenderCallBack(int (*cb)()) { _postRenderCallBack = cb;}

protected:
	//! Default constructor
	VideoPlugin(const string name, const dword format, const byte halfPixels);

	//! computes the clipping of two rectangles and changes src and dst accordingly
	void ComputeRects(SDL_Rect* src, SDL_Rect* dst);
public:
	//static VideoPlugin* Create(VideoType type);
	//static VideoPlugin* Create(VideoPlugin *ptr);
	static VideoPlugin* Create();

	virtual ~VideoPlugin();

protected:
	static bool HaveOpenGLExtension(const string &name_ext);
};

#endif
