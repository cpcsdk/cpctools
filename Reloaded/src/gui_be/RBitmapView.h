/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2011
 */
#if __GNUC__ > 2
#pragma once
#endif

#ifndef __RBITMAPVIEW_H__
#define __RBITMAPVIEW_H__
 
#include <iostream>

#include <Bitmap.h>

#include "../core/video.h"
 
class RBitmapView: public BView
{
	public:
		RBitmapView(BWindow* parent, bool overlay)
		: BView(BRect(0,0,VideoPlugin::CPCVisibleSCRWidth * 2 - 1,
			VideoPlugin::CPCVisibleSCRHeight * 2 - 1), "bitmapview", B_FOLLOW_NONE,
			B_WILL_DRAW)
		{
			parent->AddChild(this); // needs to be done before setting the view bitmap
			
			BRect bitmapArea(Bounds());
			bitmapArea.bottom /= 2; // No line doubling here
			
			if (overlay)
			{
				fDisplay = new BBitmap(bitmapArea, B_BITMAP_WILL_OVERLAY | B_BITMAP_RESERVE_OVERLAY_CHANNEL, B_RGB32);
				status_t res = fDisplay->InitCheck();
				if (res == B_OK) {
					SetViewOverlay(fDisplay, bitmapArea, Bounds(), 0);
					return;
				} else {
					std::cout << "Error while creating overlay bitmap: " << strerror(res) <<
					". Falling back to slow method.\n"; 
					delete fDisplay;
				}
			}
			
			// If overlay failed, fallback to normal bitmap...
			fDisplay = new BBitmap(bitmapArea, B_RGB32);
			SetViewBitmap(fDisplay, bitmapArea, Bounds(), B_FOLLOW_TOP|B_FOLLOW_LEFT, 0);
		}

		BBitmap* fDisplay;
};

#endif
