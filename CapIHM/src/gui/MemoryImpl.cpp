#include "MemoryImpl.h"
#include "memory.h"

MemoryImpl::MemoryImpl(wxWindow* parent,Emulator* emulator)
	: Memory(parent)
{
	_emulator = emulator;

	t_Memory& emu_mem = _emulator -> GetMemory();

	wxString str;

	for(int i=0;i<0x200;i++)
	{
		str.Printf(_("%2X"),emu_mem.Read(i));
		m_grid1 -> SetCellValue(i>>4,i&0xF,str);	
	}
}

MemoryImpl::~MemoryImpl()
{
}

void MemoryImpl::RefreshMem(wxScrollEvent& event)
{
	event.Skip();
}
