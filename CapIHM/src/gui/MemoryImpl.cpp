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

#include <sstream>
#include <string>

#include <wx/arrstr.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include "Desass.h"
#include "z80.h"

#include "MemoryImpl.h"
#include "memory.h"

#define cs2ws(s)   	   (wxString(s,wxConvLibc))
#define s2ws(s) (wxString((s).c_str(), wxConvLibc))
MemoryImpl::MemoryImpl(wxWindow* parent,Emulator* emulator)
	: Memory(parent)
{
	_emulator = emulator;

	t_Memory& emu_mem = _emulator -> GetMemory();

	// Hexadecimal view
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

	// Disassembly view
	std::stringstream data;
	Desass(emu_mem.GetRAM(),data,0,0x10000);

/*
	wxString bloat = wxString::FromAscii(data.str().c_str());

	wxStringTokenizer tkz(bloat, wxT("\n"));
	wxArrayString a;
	while ( tkz.HasMoreTokens() )
	{
	    wxString token = tkz.GetNextToken();

		a.Add(token);
	}
	*/

  char line[256];
	wxArrayString a;
  while(data)
  {
    data.getline( line, 256);
    a.Add(wxString::From8BitData(line));
  }

  cout << 1 << endl;
  m_checkList1 -> Set(a,0);
  cout << 2 << endl;
}

MemoryImpl::~MemoryImpl()
{
}

void MemoryImpl::onBreakpoint(wxCommandEvent& event)
{
	int index = event.GetInt(); // Get the index of the checked/unchecked line in the listbox

	// We have to parse the text in the listbox to get the address where to set the break...

	long addr;
	m_checkList1->GetString(index).Truncate(4).ToLong(&addr,16);

	if(m_checkList1->IsChecked(index))
	{
		// Add breakpoint
		_emulator->GetZ80().add_break_point(addr);
	}
	else
	{
		// Remove breakpoint
		_emulator->GetZ80().remove_break_point(addr);
	}
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
