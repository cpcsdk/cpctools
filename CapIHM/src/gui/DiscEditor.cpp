#include "DiscEditor.h"

#include "emulator.h"

DiscEditorImpl::DiscEditorImpl( wxWindow* parent )
:
DiscEditor( parent )
{
	// Get the disk info.
	FloppyImage = Emulator::getInstance()->GetFDC().GetDriveA();
	// Fill in the Overview
	wxString s;
	s << FloppyImage.tracks;
	trackCount->SetValue(s);
	s.Empty();
	s << FloppyImage.sides;
	sideCount->SetValue(s);

	wxColour c;

	// TODO : it would probably be faster if we use a wxPanel and draw on it by hand.
	// All this stuff is taking a lot of time to instanciate and handle.
	for(unsigned int side = 0; side <= FloppyImage.sides; side++) {
		for(unsigned int row = 0; row < FloppyImage.tracks; row++) {
			t_track* currentTrack = &FloppyImage.track[row][side];
			int pos = 0;
			for(unsigned int col = 0;col < currentTrack->sectors; col++)
			{
				wxGBPosition p(row,pos + side*40);
				wxString v;
			    v << (int)currentTrack->sector[col].CHRN[2];
				wxButton* b = new wxButton(m_panel17, wxID_ANY, v);
				if(currentTrack->sector[col].size != currentTrack->sector[col].declared_size)
					c.Set(255,0,0); // Red : weak sector
				else
					c.Set(255,255,255); // White : ok
				b->SetBackgroundColour(c);
				wxSize k(currentTrack->sector[col].declared_size/16,8);
				b->SetMinSize(k);
				b->SetMaxSize(k);
				diskOverview->Add(b, p, wxGBSpan(1,currentTrack->sector[col].declared_size/128), 0, 0);
				pos += currentTrack->sector[col].declared_size/128;
			}
		}
	}

	// Fill in the sector editor
	lb_sectors->Clear();
	for(unsigned int s = 0;s<FloppyImage.track[0][0].sectors;s++) {
		wxString sNum;
		sNum.Printf(_("%x"),(int)FloppyImage.track[0][0].sector[s].CHRN[2]);
		lb_sectors->Append(sNum);
	}
}

void DiscEditorImpl::setTrack( wxSpinEvent& event )
{
	lb_sectors->Clear();
	for(unsigned int s = 0;s<FloppyImage.track[event.GetPosition()][0].sectors;s++) {
		wxString sNum;
		sNum.Printf(_("%x"),(int)FloppyImage.track[event.GetPosition()][0].sector[s].CHRN[2]);
		lb_sectors->Append(sNum);
	}
}
