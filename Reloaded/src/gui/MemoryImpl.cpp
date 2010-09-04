/**
 *   $Id$
 *	 CapriceReloded an Amstrad CPC emulator
 *   Copyright (C) 2009,2010 cpcsdk crew
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

#include <sstream>
#include <string>

#include <wx/arrstr.h>
#include <wx/dcclient.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>

#include "Desass.h"
#include "z80.h"
#include "memory.h"

// TODO proper handling of BANKS and ROM mapping. Now you see the z80 view and
// there is no way to get out of it

void MemoryImpl::ChangeAddress(int addr)
{
	scrollRAM->SetThumbPosition(addr/16);
	addressBox->SetValue(wxString::Format(_("%X"),addr));
	addressSpin->SetValue(addr);
}


MemoryImpl::MemoryImpl(wxWindow* parent, Emulator* emulator)
	: Memory(parent)
{
	_emulator = emulator;

	// Hexadecimal view
	RefreshMem(_emulator->GetZ80().PC.w.l);

	// wxFormBuilder does not setup the scrollbar correctly so we do it here.
	// This way we can adjust it depending to the actual memory (64 or 128k)
	scrollRAM-> SetScrollbar(0, 0x200 / 16,
		64 * 1024 / 16, 0x200 / 16);
	addressSpin->SetRange(0,0xFFFF);
	ChangeAddress(emulator->GetZ80().PC.w.l);
}

MemoryImpl::~MemoryImpl()
{
}

void MemoryImpl::setZoneInfo(int addr)
{
	wxString s;
	switch(_emulator->GetMemory().getTypeForAddress(addr<<8)) {
		case 1:
			s = _("System ROM");
			break;
		case 2:
			s = _("Ext. ROM "); // TODO : rom ID
			break;
		case 4:
			s = _("Bank "); // TODO : bank number
			break;
		default:
			s = _("Central RAM");
			break;
	}
	switch(addr) {
		case 0x00:
			zone0->SetLabel(s);
			break;
		case 0x40:
			zone1->SetLabel(s);
			break;
		case 0x80:
			zone2->SetLabel(s);
			break;
		case 0xC0:
			zone3->SetLabel(s);
			break;
	}
}

void MemoryImpl::UpdateOverview(wxPaintEvent& event)
{
	// TODO : it is possible to improve this by only redrawing the invalidated area
	// (wich may not even touch the pixeled zone)
	wxPaintDC drawContext(overviewPanel);
	wxPen p;
	wxColour baseColor;
	wxColour darkColor;
	for (int y = 0; y < 256; y++)
	{
		if(y==0 || y==0x40 || y==0x80 || y==0xC0)
		{
			setZoneInfo(y);
			switch(_emulator->GetMemory().getTypeForAddress(y<<8))
			{
				case 1: // Low ROM
				case 2: // Hi ROM
					baseColor = romColor->GetColour();
					break;
				case 4: // Exp. RAM
					baseColor = bankColor->GetColour();
					break;
				default: // Case 3 : main RAM
					baseColor = centralColor->GetColour();
					break;
			}
			darkColor.Set(max((unsigned char)120,baseColor.Red()) - 120,
				max((unsigned char)120,baseColor.Green()) - 120,
				max((unsigned char)120,baseColor.Blue()) - 120);
		}

		for (int x = 0; x < 256; x++)
		{
			if (_emulator->GetMemory().Read((y << 8) + x) == 0)
			{
				p.SetColour(baseColor);
			}
			else
			{
				p.SetColour(darkColor);
			}
			drawContext.SetPen(p);
			drawContext.DrawPoint(x, y);
		}
	}
}

void MemoryImpl::onBreakpoint(wxCommandEvent& event)
{
	int index = event.GetInt(); // Get the index of the checked/unchecked line
		// in the listbox

	// We have to parse the text in the listbox to get the address where to set
	// the break...

	long addr;
	m_checkList1->GetString(index).Truncate(4).ToLong(&addr, 16);

	if (m_checkList1->IsChecked(index))
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


void MemoryImpl::AddressEntered( wxCommandEvent& event )
{
	long int destination;
	event.GetString().ToLong(&destination, 16);
	RefreshMem(destination);
//	scrollRAM->SetThumbPosition(destination / 16);
	ChangeAddress(destination);
}


void MemoryImpl::JumpToAddress( wxSpinEvent& event )
{
	RefreshMem(event.GetPosition());
//	addressBox->SetValue(wxString::Format(_("%X"),event.GetPosition()));
//	scrollRAM->SetThumbPosition(event.GetPosition() / 16);
	ChangeAddress(event.GetPosition());
}


void MemoryImpl::JumpToPC( wxCommandEvent& event )
{
	RefreshMem( _emulator ->GetZ80().PC.w.l);
//	addressBox->SetValue(wxString::Format(_("%X"),_emulator->GetZ80().PC.w.l));
//	scrollRAM->SetThumbPosition(_emulator->GetZ80().PC.w.l / 16);
	ChangeAddress(_emulator->GetZ80().PC.w.l);
}


void MemoryImpl::JumpToSP( wxCommandEvent& event )
{
	RefreshMem( _emulator ->GetZ80().SP.w.l);
//	addressBox->SetValue(wxString::Format(_("%X"),_emulator->GetZ80().SP.w.l));
//	scrollRAM->SetThumbPosition(_emulator->GetZ80().SP.w.l / 16);
	ChangeAddress(_emulator->GetZ80().SP.w.l);
}

void MemoryImpl::RefreshMem(wxScrollEvent& event)
{
	RefreshMem(event.GetPosition() * 16);
	addressBox->SetValue(wxString::Format(_("%X"),event.GetPosition() * 16));
}

void MemoryImpl::RefreshMem(int startAddress)
{
	t_Memory& emu_mem = _emulator -> GetMemory();

	wxString str;

	hexView->Clear();

	for (int i = 0; i < 32; i++)
	{
		str.Printf(_("%04X"),startAddress + (i * 0x10));
		*hexView << str << _("  ");

		for (int j = 0; j < 16; j++)
		{
			str.Printf(_("%02X"),emu_mem.Read(startAddress + i*16+j));
			*hexView << str << _(" ");
		}
		*hexView << _("\n");
	}

	// Disassembly view
	std::stringstream data;
	Desass(emu_mem, data, startAddress, 0x200);

	char line[256];
	wxArrayString a;
	while (data)
	{
		data.getline( line, 256);
		a.Add(wxString::From8BitData(line));
	}

	m_checkList1 -> Set(a, 0);
	// Work around a bug in wx...
	m_checkList1->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxT("Courier") ) );

}


void MemoryImpl::LoadSymbolTable( wxFileDirPickerEvent& event )
{
	// Open the file parse as :
	// "Label",": equ ","value"
	// And create an hashmap from that

	// Also fill in the label selector
	wxTextFile f(event.GetPath());
	f.Open();

	ChoiceLabels->Clear();

	for (unsigned int i = 0; i < f.GetLineCount(); i++)
	{
		// Parse each line and insert it in the hashmap
		wxString label = f[i].BeforeFirst(':');

		unsigned long adr;
		f[i].AfterLast(' ').ToULong(&adr, 0);

		if (adr) // filter out all these stupid labels that point to 0
		{
			lhm[label]=adr;
			ChoiceLabels->Append(label);
		}
	}
}


void MemoryImpl::JumpToSymbol( wxCommandEvent& event )
{
	// Lookup the text in a hashmap
	RefreshMem( lhm[event.GetString()]);
//	addressBox->SetValue(wxString::Format(_("%X"),lhm[event.GetString()]));
//	scrollRAM->SetThumbPosition(lhm[event.GetString()] / 16);
	ChangeAddress(lhm[event.GetString()]);
}

