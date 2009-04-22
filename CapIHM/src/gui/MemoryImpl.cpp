/**
 *   $Id$
 *	 CapriceReloded an Amstrad CPC emulator
 *   Copyright (C) 2009  cpcsdk crew
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */  


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
