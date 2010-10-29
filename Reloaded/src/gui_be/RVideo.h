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
		printf("create vid : w%d h%d d%d f%d\n",w,h,bpp,fs);
		switch(bpp) {
			case 8:
				mMap = new BBitmap(BRect(0,0,w,h),B_CMAP8);
				break;
			case 16:
				mMap = new BBitmap(BRect(0,0,w,h),B_RGB16);
				break;
			case 32:
			default:
				mMap = new BBitmap(BRect(0,0,w,h),B_RGB32);
				break;
		}
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
		if (sView->Window()->LockLooper()) {
			sView->DrawBitmap(mMap,BPoint(0,0));
			sView->Window()->UnlockLooper();
		}
	}
	virtual void Close() {}
    virtual bool IsUpdate() {return false;}
	void Screenshot(string filename) {}

	~RVidPlugin() {delete mMap;}

	static BView* sView;
};
