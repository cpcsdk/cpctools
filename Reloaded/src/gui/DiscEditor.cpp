#include "DiscEditor.h"

#include <wx/dcclient.h>
#include <wx/brush.h>
#include <wx/textdlg.h>
#include <wx/log.h>

#include "emulator.h"

t_sector* DiscEditorImpl::sectorClipboard = NULL;

DiscEditorImpl::DiscEditorImpl( wxWindow* parent )
	: DiscEditor(parent)
	, FloppyImage(Emulator::getInstance()->GetDriveA())
{
	// Get the disk info.
	// Fill in the Overview
	wxString s;
	s << FloppyImage.tracks;
	trackCount->SetValue(s);
	s.Empty();
	s << FloppyImage.sides;
	sideCount->SetValue(s);

	// Fill in the sector editor
	lb_sectors->Clear();
	for(unsigned int s = 0;s<FloppyImage.track[0][0].sectors;s++) {
		wxString sNum;
		sNum.Printf(wxT("%x"),(int)FloppyImage.track[0][0].sector[s].CHRN[2]);
		lb_sectors->Append(sNum);
	}
}


void DiscEditorImpl::drawSectorExplorer( wxPaintEvent& event )
{
	const int offset = 20;
	wxPaintDC drawContext(explorerPanel);
	wxFont f(8,wxDEFAULT,wxNORMAL,wxNORMAL);

	drawContext.SetFont(f);
	wxColour c;
	wxBrush fill;
	wxPen p;
	wxString v;

	c.Set(128,128,128);
	p.SetColour(c);
	drawContext.SetPen(p);

	for(unsigned int side = 0; side <= FloppyImage.sides; side++) {
		for(unsigned int row = 0; row < FloppyImage.tracks; row++)
		{
			t_track* currentTrack = &FloppyImage.track[row][side];
			int pos = 0;

			// Draw track number
			v.Printf(wxT("%2d"),row);
			wxPoint l(0, row*12);
			drawContext.DrawText(v,l);

			for(unsigned int col = 0;col < currentTrack->sectors; col++)
			{
			    v.Printf(wxT("%2.2X"),(unsigned char)currentTrack->sector[col].CHRN[2]);
				if(currentTrack->sector[col].size != currentTrack->sector[col].declared_size)
				{
					if(currentTrack->sector[col].flags[1] &0x40)
						c.Set(255,0,255); // Weak and Erased!
					else
						c.Set(255,0,0); // Red : weak sector
				}
				else if(currentTrack->sector[col].flags[1] &0x40)
					c.Set(0,0,255);
				else
					c.Set(255,255,255); // White : ok
				// TODO : erased sectors should have another color
				fill.SetColour(c);
				drawContext.SetBrush(fill);
				wxSize k(currentTrack->sector[col].declared_size/16,12);
				wxPoint l(offset+pos,row*12);
				drawContext.DrawRectangle(l,k);
				drawContext.DrawText(v,l);
				pos += currentTrack->sector[col].declared_size/16;
			}
		}
	}
}


void DiscEditorImpl::setTrack( wxSpinEvent& event )
{
	lb_sectors->Clear();
	currentTrack = event.GetPosition();

	// Add all sectors to the list
	for(unsigned int s = 0;s<FloppyImage.track[event.GetPosition()][0].sectors;s++) {
		wxString sNum;
		sNum.Printf(_("%x"),(int)FloppyImage.track[event.GetPosition()][0].sector[s].CHRN[2]);
		lb_sectors->Append(sNum);
	}
}

void DiscEditorImpl::setSector( wxCommandEvent& event )
{
	// Display sector info and data
	t_sector selectedSector = FloppyImage.track[currentTrack][0].sector[event.GetInt()];
	if(selectedSector.flags[1] & 0x40)
		st_erased->SetLabel(_("erased"));
	else
		st_erased->SetLabel(_("not erased"));

	wxString txt = _("Size : ");

	int size = selectedSector.declared_size/128;
	int i = 0;
	while( size >>= 1) {
		i++;
	}

	txt << i;
	st_size->SetLabel(txt);

	if(selectedSector.declared_size > selectedSector.size)
		st_weak->SetLabel(_("weak"));
	else if(selectedSector.declared_size < selectedSector.size)
		st_weak->SetLabel(_("missing data"));
	else
		st_weak->SetLabel(_("sane"));

	// TODO : build a proper hexdump
	txt.Clear();
	wxString txt2;
	int actualSize = std::min(selectedSector.declared_size, selectedSector.size);
	if(actualSize) {
		int i = 0;
		for(int k = 0; k < actualSize; k+=16) {
			txt2.Clear();
			for(int j=0; j< 16; j++) {
				txt << wxString::Format(_("%02x "),(int)(selectedSector.data[i]));
				if (selectedSector.data[i]> 32)
					txt2 << (wxChar)selectedSector.data[i];
				else
					txt2 <<_(".");

				i++;
			}
			txt.Append(_("  "));
		   	txt.Append(txt2);
			txt.Append(_("\n"));
		}
	} else {
		txt.Printf(_("This sector is empty or invalid\n"
			"Declared size : %d\nActual size : %d"),
			selectedSector.declared_size, selectedSector.size
				);
	}
	tc_sectordata->SetValue(txt);
}


void DiscEditorImpl::sectorLeftClick( wxMouseEvent& event )
{
	int itemID;
	if((itemID = lb_sectors->HitTest(event.GetPosition())))
	{
		lb_sectors->SetSelection(itemID);
	}

	DiscEditor::lb_sectorsOnContextMenu(event);
}


void DiscEditorImpl::cutSector( wxCommandEvent& event )
{
	copySector(event);

	static unsigned char dataClip[16*1024];
	memcpy(dataClip, sectorClipboard->data, sectorClipboard->size);
	sectorClipboard->data = dataClip;

	deleteSector(event);
}


void DiscEditorImpl::copySector( wxCommandEvent& event )
{
	// Get listbox selection
	// Grab selected sector and copy it to clipboard
	int sect_id = lb_sectors->GetSelection();
	if (sect_id == wxNOT_FOUND) return;
	int track_id = spinTrack->GetValue();

	// TODO : handle side 1
	if (sectorClipboard == NULL)
		sectorClipboard = (t_sector*)malloc(sizeof(t_sector));

	memcpy(sectorClipboard, &FloppyImage.track[track_id][0].sector[sect_id], sizeof(t_sector));
}


void DiscEditorImpl::pasteSector( wxCommandEvent& event )
{
	if (sectorClipboard == NULL) return;
	// Copy clipboard to selected track
	// int sect_id = lb_sectors->GetSelection();
		// TODO : paste just below sect_id instead of at sector end
	int track_id = spinTrack->GetValue();

#define TRK FloppyImage.track[track_id][0]

	// TODO : handle side 1
	TRK.size += sectorClipboard->size;

	TRK.data = (unsigned char*)realloc(TRK.data, TRK.size);
	if (TRK.data == NULL) {
		wxLogError(wxT("Memory allocation failed"));
		TRK.size -= sectorClipboard->size;
		return;
	}

	memcpy(
		TRK.data + TRK.size -sectorClipboard->size, sectorClipboard->data, sectorClipboard->size);
	sectorClipboard->data = 
		TRK.data + TRK.size -sectorClipboard->size;
	TRK.sector[TRK.sectors++] = *sectorClipboard;

	FloppyImage.altered = true;

	wxString sNum;
	sNum.Printf(wxT("%x"),(int)sectorClipboard->CHRN[2]);
	lb_sectors->Append(sNum);
}


void DiscEditorImpl::deleteSector( wxCommandEvent& event )
{
	// Get listbox selection
	int sect_id = lb_sectors->GetSelection();
	if (sect_id == wxNOT_FOUND) return;
	int track_id = spinTrack->GetValue();

	// Remove it from the box and the track itself
	
	// We are going to save some info about the sector before we erase it...
	unsigned int sectSize = TRK.sector[sect_id].size;

	// Move the data in the track Memory
	if (sect_id < TRK.sectors - 1) {
		unsigned char* from = TRK.sector[sect_id+1].data;// beginning of next sector
		unsigned char* to = TRK.sector[sect_id].data;// beginning of this sector
		size_t size = TRK.sector[TRK.sectors-1].data - TRK.sector[sect_id].data;// from end of this sector to end of track
		memmove(to, from, size);

		for (int k = sect_id + 1; k < TRK.sectors; k++) {
			TRK.sector[k].data -= sectSize;
		}
	}
	// Realloc the track Memory
	TRK.data = (unsigned char*)realloc(TRK.data, TRK.size - sectSize);
	// decrease the track data size and sector count
	TRK.size -= sectSize;
	TRK.sectors --;

	lb_sectors->Delete(sect_id);

	FloppyImage.altered = true;
}


void DiscEditorImpl::renameSector( wxCommandEvent& event )
{
	int sect_id = lb_sectors->GetSelection();
	if (sect_id == wxNOT_FOUND) return;
	int track_id = spinTrack->GetValue();

	// Ask for the new ID and change it in the track
	wxString newId;
	wxString msg;
	msg.Printf(wxT("Enter new ID for sector %x in track %d"),sect_id, track_id);
	newId.Printf(wxT("%x"),FloppyImage.track[track_id][0].sector[sect_id].CHRN[2]);
   	newId = wxGetTextFromUser(msg, wxT("Rename sector"), newId, this);
	long val;
	if (!newId.IsEmpty() && newId.ToLong(&val, 16)) {
		FloppyImage.track[track_id][0].sector[sect_id].CHRN[2] = val;

		lb_sectors->SetString(sect_id, newId);

		FloppyImage.altered = true;
	}
}

