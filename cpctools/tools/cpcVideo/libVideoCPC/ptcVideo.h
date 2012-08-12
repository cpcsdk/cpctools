#ifndef __PTCVIDEO_H__
#define __PTCVIDEO_H__

#include "video.h"

class ptcVideo: public VideoPlugin
{
	private:
		void SetPalette(ColorARGB8888* colors);
		void Flip();
		bool IsUpdate();
		void Screenshot(string filename);

		void Unlock();
		bool TryLock();

		bool TryLockOutput();
		bool LockOutput();
		void UnlockOutput();

		void Close();
	public:
		ptcVideo();
		void* Init(int w, int h, int bpp, bool fullscreen);
};

#endif
