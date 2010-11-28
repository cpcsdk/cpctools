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
		_publicBPP = bpp;

		if (!fs)
		{
			w=CPCVisibleSCRWidth*2;
			h=CPCVisibleSCRHeight;
		}

		_outputWidth = w;
		_outputHeight = h;
		_publicWidth = w;
		_publicHeight = h;

		_publicVideo = sView->fBits;
		_publicPitch = sView->fRowBytes;

		return sView->fBits;
	}
	virtual void SetPalette(ColorARGB8888* c) {}
	virtual bool TryLock() {return sView->locker->Lock();}
	virtual void Unlock() {sView->locker->Unlock();}
    virtual bool LockOutput() {return true;}
    virtual bool TryLockOutput() {return true;}
    virtual void UnlockOutput() {}
	virtual void Flip()
	{ 
	}
	virtual void Close() {}
    virtual bool IsUpdate() {return false;}
	void Screenshot(string filename) {}

	~RVidPlugin() {}

	static ReloadedWin* sView;
};
