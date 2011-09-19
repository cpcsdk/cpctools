#include <Bitmap.h>
#include <View.h>
#include <Window.h>

#include "RWin.h"
#include "emulator.h"

class RVidPlugin : public VideoPlugin
{
	private:
	RVidPlugin() 
		: VideoPlugin("Reloaded", ALL, 0)
	{
	}

	public:
	static VideoPlugin* Create()
	{
		return (new RVidPlugin());
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
		_publicBPP = bpp;

		_outputWidth = w;
		_outputHeight = h;
		_publicWidth = w;
		_publicHeight = h;

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
