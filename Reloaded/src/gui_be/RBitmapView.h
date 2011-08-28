/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2011
 */
#pragma once
 
#include <iostream>
 
class RBitmapView: public BView
{
	public:
		RBitmapView(BBitmap* display)
		: BView(BRect(0,0,VideoPlugin::CPCVisibleSCRWidth * 2,
			VideoPlugin::CPCVisibleSCRHeight * 2), "bitmapview", B_FOLLOW_NONE,
			B_WILL_DRAW)
		{
			fDisplay = display;
		}
		
		void Draw(BRect updateRect)
		{
			DrawBitmapAsync(fDisplay, Bounds());
		}
	private:
		BBitmap* fDisplay;	
};
