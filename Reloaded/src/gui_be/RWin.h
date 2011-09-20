/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010-2011
 */

#ifndef __RWIN_H__
#define __RWIN_H__

#include <Window.h>
#include <Bitmap.h>

#include "RBitmapView.h"

class ReloadedWin : public BWindow
{
	public:
		ReloadedWin();

		void MessageReceived(BMessage* mess);
		
		BBitmap& getBitmap() {return *fBitmapView->fDisplay;}
	private:
		RBitmapView* fBitmapView;
		BView* fStatusLed;
};

#endif
