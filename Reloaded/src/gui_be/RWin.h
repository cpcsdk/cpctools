/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010
 */

#ifndef __RWIN_H__
#define __RWIN_H__

#include <DirectWindow.h>
#include <Locker.h>
#include <MenuBar.h>
#include <MenuItem.h>

#include "REmulator.h"

class ReloadedWin : public BDirectWindow
{
	public:
		ReloadedWin() : BDirectWindow(BRect(80,80,847,619),"Reloaded", B_TITLED_WINDOW,
			B_NOT_ZOOMABLE|B_NOT_RESIZABLE|B_QUIT_ON_WINDOW_CLOSE)
		{
			SetSizeLimits(847-80,847-80,619-61,619-61);
			fConnected = false;
			fConnectionDisabled = false;
			locker = new BLocker();
			fClipList = NULL;
			fNumClipRects = 0;

			BMenuBar* menu = new BMenuBar(BRect(0,0,847,16),"mainmenu");
				BMenu* file = new BMenu("File");
				menu->AddItem(file);
					BMenuItem* insertdsk = new BMenuItem("Insert Disc", new BMessage());
					file->AddItem(insertdsk);
			AddChild(menu);

			if (!SupportsWindowMode()) {
				SetFullScreen(true);
			}

			fDirty = true;
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
					BDirectWindow::MessageReceived(mess);
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

		void MenusBeginning() {locker->Lock();}
		void MenusEnding() {locker->Unlock();}
};

#endif
