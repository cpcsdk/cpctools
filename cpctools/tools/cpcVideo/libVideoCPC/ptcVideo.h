#include "video.h"

class ptcVideo: public VideoPlugin
{
	private:
		ptcVideo();

		void* Init(int w, int h, int bpp, bool fullscreen);
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
		static VideoPlugin* create();
};
