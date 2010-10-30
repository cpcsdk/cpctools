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
			mainView = new BView(BRect(0,0,800,600), "main", 0, B_WILL_DRAW);
			AddChild(mainView);
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

		BView* mainView;
};
