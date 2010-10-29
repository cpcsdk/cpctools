/* Reloaded - Amstrad CPC emulator
 * Haiku/BeOS GUI by PulkoMandy - 2010
 */

#include <Window.h>

class ReloadedWin : public BWindow
{
	public:
		ReloadedWin() : BWindow(BRect(80,80,806,606),"Reloaded", B_TITLED_WINDOW,
			B_NOT_ZOOMABLE|B_NOT_RESIZABLE|B_QUIT_ON_WINDOW_CLOSE)
		{
			mainView = new BView(BRect(0,0,800,600), "main", 0, 0);
			AddChild(mainView);
		}

		BView* mainView;
};
