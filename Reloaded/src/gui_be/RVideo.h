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

		_outputWidth = w;
		_outputHeight = h;
		_publicWidth = w;
		_publicHeight = h;
		
		fWindow = new ReloadedWin();
		fWindow->Show();

		_publicVideo = fWindow->getBitmap().Bits();
		_publicPitch = fWindow->getBitmap().BytesPerRow();

		return _publicVideo;
	}
	virtual void SetPalette(ColorARGB8888* c) {}
	virtual bool TryLock() {return true;}
	virtual void Unlock() {}
    virtual bool LockOutput() {return true;}
    virtual bool TryLockOutput() {return true;}
    virtual void UnlockOutput() {}
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
