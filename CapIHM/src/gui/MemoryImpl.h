#include <wx/sstream.h>

#include "CapriceUI.h"
#include "emulator.h"

class MemoryImpl: public Memory
{
	private:
		Emulator* _emulator;
		wxStringOutputStream s;

	protected:
		void RefreshMem (wxScrollEvent& event);
		void onBreakpoint(wxCommandEvent& event);
		void JumpToAddress( wxSpinEvent& event );
		void JumpToPC( wxCommandEvent& event );
		void JumpToSP( wxCommandEvent& event );
		void RefreshMem(int startAddress);
	public:
		MemoryImpl(wxWindow* parent, Emulator* emulator);
		~MemoryImpl();
};

