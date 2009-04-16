#include "MemoryImpl.h"
#include "memory.h"

MemoryImpl::MemoryImpl(wxWindow* parent,Emulator* emulator)
	: Memory(parent)
{
	_emulator = emulator;

	t_Memory& emu_mem = _emulator -> GetMemory();

	wxString str;

	for(int i=0;i<32;i++)
	{
		str.Printf(_("%04X"),i*16);
		m_grid1 -> SetRowLabelValue(i,str);
	}

	for(int i=0;i<0x200;i++)
	{
		str.Printf(_("%02X"),emu_mem.Read(i));
		m_grid1 -> SetCellValue(i>>4,i&0xF,str);	
	}

	// wxFormBuilder does not setup the scrollbar correctly so we do it here.
	// This way we can adjust it depending to the actual memory (64 or 128k)
	m_scrollBar1 -> SetScrollbar(0,0x200/16,emulator->GetConfig().ram_size*1024/16,0x200/16);
}

MemoryImpl::~MemoryImpl()
{
}

void MemoryImpl::RefreshMem(wxScrollEvent& event)
{
	t_Memory& emu_mem = _emulator -> GetMemory();

	wxString str;

	for(int i=0;i<32;i++)
	{
		str.Printf(_("%04X"),(i+event.GetPosition())*16);
		m_grid1 -> SetRowLabelValue(i,str);
	}
	for(int i=0;i<0x200;i++)
	{
		str.Printf(_("%02X"),emu_mem.Read(i+event.GetPosition()*16));
		m_grid1 -> SetCellValue(i>>4,i&0xF,str);	
	}
}
