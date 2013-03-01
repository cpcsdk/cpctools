#include <wx/sstream.h>

#include "CapriceUI.h"
#include "core/emulator.h"

 WX_DECLARE_STRING_HASH_MAP( int, LabelsHashMap );

class MemoryImpl: public Memory
{
	private:
		Emulator* _emulator;
		wxStringOutputStream s;

		LabelsHashMap lhm;

		void ChangeAddress(int addr);
		void setZoneInfo(int);

		char overviewType;

	protected:
		void RefreshMem (wxScrollEvent& event);
		void UpdateOverview( wxPaintEvent& event );
		void onBreakpoint(wxCommandEvent& event);
		virtual void AddressEntered( wxCommandEvent& event );
		void JumpToAddress( wxSpinEvent& event );
		void JumpToPC( wxCommandEvent& event );
		void JumpToSP( wxCommandEvent& event );
		void RefreshMem(int startAddress);
		void LoadSymbolTable( wxFileDirPickerEvent& event );
		void JumpToSymbol( wxCommandEvent& event );
		void onChangeView( wxCommandEvent& event );

		void searchASCII( wxCommandEvent& event );
		void searchNumber( wxCommandEvent& event );
	public:
		MemoryImpl(wxWindow* parent, Emulator* emulator);
		~MemoryImpl();
};

