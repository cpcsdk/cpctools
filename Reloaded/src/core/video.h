/* Reloded an Amstrad CPC emulator
   (c) Copyright 2008-2009  cpcsdk crew
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

#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <string>
#include "cap32type.h"

#include "misc/synchro.h"

using std::string;

class VideoPlugin
{
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

public:
	static const int CPCVisibleSCRWidth;
	static const int CPCVisibleSCRHeight;

protected:
	//! Plugin name
	string			_name;
	// what you can feed to this plugin :
	//! the pixel formats supported
	dword			_formats;

	//! Post render call back
	int				(*_postRenderCallBack)();

public:

	//! The video surface shown by the plugin to the renderer function.
    SysSync publicSurfaceSync;
	void* _publicVideo;
	int _publicHeight;
	int _publicWidth;
	int _publicBPP;
	int _publicPitch;

	//! The real video surface
	void* _outputSurface;
	int _outputHeight;
	int _outputWidth;
	int _outputBPP;
	int _outputPitch;

	//TODO: Rework on surface system
	//! initializes the video plugin ; returns the surface that you must draw into as a void*, you do use this pointer only for detect allocation error (NULL), you do use a internal private pointer to use surface, NULL in the (unlikely ;) event of a failure
	virtual void* Init(int w,int h,int bpp,bool fs);
	
	virtual void* GetSurface() {return _publicVideo;}

	virtual void SetPalette(ColorARGB8888* c) = 0;

	//! Set output surface width
	inline void SetOutputSurfaceWidth(int w) {_outputWidth = w;}

	//! Return output surface width
	inline int GetOutputSurfaceWidth() const { return _outputWidth; }

	//! Set output surface width
	inline void SetOutputSurfaceHeight(int h) {_outputHeight = h;}

	//! Return output surface width
	inline int GetOutputSurfaceHeight() const { return _outputHeight; }

	//! Return output surface pitch (bytes number by lines) ! TODO
	inline int GetOutputSurfacePitch() const { return _outputPitch; }

	//! Return output surface bits per pixels
	inline int GetOutputSurfaceBPP() const { return _outputBPP; }

	virtual inline int GetRenderSurfaceWidth() const { return _publicWidth; }
	virtual inline int GetRenderSurfaceHeight() const { return _publicHeight; }
	
	virtual inline int GetRenderSurfacePitch() const { return _publicPitch; }

	virtual inline int GetRenderSurfaceBPP() const { return _publicBPP; }



	//! locks the public surface if needed
	virtual bool TryLock() = 0;
	//! unlocks the public surface if needed
	virtual void Unlock() = 0;
    // TODO locks the output surface if needed
    virtual bool TryLockOutput() = 0;
    virtual bool LockOutput() = 0;
    // TODO unlocks the output surface if needed
    virtual void UnlockOutput() = 0;
	//! "flips" the video surface. Note that this might not always do a real flip
	virtual void Flip() = 0;
	//! closes the plugin
	virtual void Close() = 0;

    // TODO Output has been update.
    virtual bool IsUpdate() = 0;
    SpinSync isUpdateSync;
	
	//! Set generic bool option
	virtual void SetOption(const string &optionName, bool val);
	
	//! Set post render call back
	inline void SetPostRenderCallBack(int (*cb)()) { _postRenderCallBack = cb;}

	virtual void Screenshot(string filename) = 0;

	bool _is_init;
	virtual bool isInit() const {return _is_init;}
protected:
	//! Default constructor
	VideoPlugin(const string name, const dword format);

public:
	virtual ~VideoPlugin();
};


class NullVideoPlugin : public VideoPlugin
{
public:
	NullVideoPlugin() 
		: VideoPlugin("No output", ALL)
	{
		surf = NULL;
	}

	virtual void* Init(int w,int h,int bpp,bool fs)
	{
		surf = new unsigned char[w*h*bpp/8];
		return (void*)surf;
	}
	virtual void SetPalette(ColorARGB8888* c) {}
	virtual bool TryLock() {return true;}
	virtual void Unlock() {}
	virtual bool LockOutput() {return true;}
	virtual bool TryLockOutput() {return true;}
	virtual void UnlockOutput() {}
	virtual void Flip() {}
	virtual void Close() {}
	virtual bool IsUpdate() {return false;}
	void Screenshot(string filename) {}

	~NullVideoPlugin() {delete [] surf;}

private:
	unsigned char* surf;
};

#endif
