#include <Bitmap.h>
#include <View.h>
#include <Window.h>

#include "emulator.h"

class RVidPlugin : public VideoPlugin
{
	private:
	RVidPlugin() 
		: VideoPlugin("Reloaded", ALL, 0)
	{
		mMap = NULL;
	}

	BBitmap* mMap;

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

		switch(bpp) {
			case 8:
				mMap = new BBitmap(BRect(0,0,w,h),B_CMAP8);
				break;
			case 16:
				mMap = new BBitmap(BRect(0,0,w,h),B_RGB16);
				break;
			case 32:
			default:
				mMap = new BBitmap(BRect(0,0,w-1,h-1),B_RGB32);
				break;
		}
		
		_publicVideo = mMap->Bits();
		_publicPitch = w*_publicBPP/8;

		return mMap->Bits();
	}
	virtual void SetPalette(ColorARGB8888* c) {}
	virtual bool TryLock() {return true;}
	virtual void Unlock() {}
    virtual bool LockOutput() {return true;}
    virtual bool TryLockOutput() {return true;}
    virtual void UnlockOutput() {}
	virtual void Flip()
	{ 
		if (sView->LockLooper()) {
			sView->DrawBitmapAsync(mMap,BRect(0,0,_outputWidth - 1, _outputHeight*2-1));
			sView->UnlockLooper();
		}
	}
	virtual void Close() {}
    virtual bool IsUpdate() {return false;}
	void Screenshot(string filename) {}

	~RVidPlugin() {delete mMap;}

	static BView* sView;
};
