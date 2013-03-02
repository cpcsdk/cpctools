/* Reloaded Haiku GUI
 * PulkoMandy - Shinra / 2011
 */

#ifndef __HAIKUVIDEOPLUGIN
#define __HAIKUVIDEOPLUGIN

#include <Bitmap.h>
#include <View.h>
#include <Window.h>

#include "RWin.h"
#include "../core/emulator.h"

class RVidPlugin : public VideoPlugin
{
	public:
	RVidPlugin() 
		: VideoPlugin("Reloaded", ALL)
	{
	}

	virtual void* Init(int w,int h,int bpp,bool fs)
	{
		fWindow = new ReloadedWin();
		fWindow->Show();
			// If the window doesn't exist yet, we can't lock.

		if (!LockOutput())
			return NULL;
			// We are going to change about everything... make sure no one is
			// messing with it
		
		VideoPlugin::Init(w,h,bpp,fs);

		_publicVideo = fWindow->getBitmap().Bits();
		_publicPitch = fWindow->getBitmap().BytesPerRow();

		UnlockOutput();

		return _publicVideo;
	}
	
	// TODO
	virtual void SetPalette(ColorARGB8888* c) {}
	
	// Only one surface for us
	virtual bool TryLock() {return LockOutput();}
	virtual void Unlock() {return UnlockOutput();}
	
    virtual bool LockOutput()
    {
    	status_t res = fWindow->getBitmap().LockBits();
    	return res == B_OK ? true : false;
    }
    virtual bool TryLockOutput() {return LockOutput();}
    virtual void UnlockOutput() {fWindow->getBitmap().UnlockBits();}
    
	virtual void Flip()
	{
		fWindow->PostMessage('rfsh');
	}
	
	virtual void Close()
	{
		// TODO	
	}
    virtual bool IsUpdate() {return false;}
	void Screenshot(string filename) {}

	~RVidPlugin() {}
	ReloadedWin* fWindow;
};

#endif
