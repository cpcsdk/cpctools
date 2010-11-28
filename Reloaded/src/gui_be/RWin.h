/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010
 */

#ifndef __RWIN_H__
#define __RWIN_H__

#include <DirectWindow.h>
#include <Locker.h>

#include "REmulator.h"

class ReloadedWin : public BDirectWindow
{
	public:
		ReloadedWin() : BDirectWindow(BRect(80,80,847,619),"Reloaded", B_TITLED_WINDOW,
			B_NOT_ZOOMABLE|B_NOT_RESIZABLE|B_QUIT_ON_WINDOW_CLOSE)
		{
			fConnected = false;
			fConnectionDisabled = false;
			locker = new BLocker();
			fClipList = NULL;
			fNumClipRects = 0;

//			AddChild(new SampleView(Bounds()));

			if (!SupportsWindowMode()) {
				SetFullScreen(true);
			}

			fDirty = true;
//			fDrawThreadID = spawn_thread(DrawingThread, "drawing_thread",
//					B_NORMAL_PRIORITY, (void *) this);
//			resume_thread(fDrawThreadID);
			Show();
		}

		void MessageReceived(BMessage* mess)
		{
			int k;
			int m;

			switch(mess->what)
			{
				case B_KEY_DOWN:
				case B_UNMAPPED_KEY_DOWN:
					m = mess->FindInt32("modifiers");
					k = mess->FindInt32("key");
					REmulator::getInstance()->PressKey(k,m);
					break;
				case B_KEY_UP:
				case B_UNMAPPED_KEY_UP:
					m = mess->FindInt32("modifiers");
					k = mess->FindInt32("key");
					REmulator::getInstance()->ReleaseKey(k,m);
					break;

				default:
					BWindow::MessageReceived(mess);
					break;
			}
		}

		void   DirectConnected(direct_buffer_info *info);

		uint8*		fBits;
		BLocker*       locker;
		int32          fRowBytes;
	private:
		color_space    fFormat;
		clipping_rect  fBounds;

		uint32         fNumClipRects;
		clipping_rect* fClipList;

		bool           fDirty;      // needs refresh?
		bool           fConnected;
		bool           fConnectionDisabled;
		thread_id      fDrawThreadID;
};

#endif
