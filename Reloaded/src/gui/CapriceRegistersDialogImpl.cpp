/**
 *   $Id: CapriceRegistersDialogImpl.cpp 60 2008-11-25 17:08:52Z giot.romain $
 *	 CapriceReloded an Amstrad CPC emulator
 *   Copyright (C) 2008  cpcsdk crew
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


#include "CapriceRegistersDialogImpl.h"

#include <iostream>
#include <sstream>
#include <string>

#include <wx/textctrl.h>

#include "WXEmulator.h"

#define  UpCRTCRegTextControl(X, Y) if(!X->IsModified()){X->Clear();*X << (int)(emulator->GetCRTC().GetRegisterValue(Y));}
#define  ModifCRTCReg(X, Y) {wxString str; long val; str = X->GetValue(); if(str.ToLong(&val,10)){cout << val; if((val > 0) && (val < 255)); emulator->GetCRTC().SetRegisterValue(Y, (unsigned char)(val));} X->Clear(); *X << (int)(emulator->GetCRTC().GetRegisterValue(Y));}

#define  UpZ80Reg8bitsTextControl(X, Y) if(!X->IsModified()){X->Clear();*X << (int)(emulator->GetZ80().Y);}
#define  ModifZ80Reg8bits(X, Y) {wxString str; long val; str = X->GetValue(); if(str.ToLong(&val,10)){cout << val; if((val > 0) && (val < 255)); emulator->GetZ80().Y = (unsigned char)(val);} X->Clear(); *X << (int)(emulator->GetZ80().Y);}

// Registers Dialog Event
void CapriceRegistersDialogImpl::OnInitR( wxInitDialogEvent& event )
{
    m_tCrA->Clear();

	// Fill in the FDC access log list
	wxString text;
	unsigned int t = emulator->fdcAccess.size();
		// Saved as a var since we pop the elements so the size changes
	for(unsigned int i = 0; i < t; i++) {
		WXEmulator::fdcLog l = emulator->fdcAccess.front();
		if (l.v > 0)
			text.Printf("Read side %d, track %d, sector %X", l.s, l.t, l.u);
		else if (l.v < 0)
			text.Printf("Track read side %d, track %d, starting at sector %X", l.s, l.t, l.u);
		else
			text.Printf("Read error on side %d, track %d, sector %X", l.s, l.t, l.u);
		FDCAccessLog->Append(text);
		emulator->fdcAccess.pop();
	}
}

void CapriceRegistersDialogImpl::OnIdleR( wxIdleEvent& event )
{
	static uint16_t oldPC;
	if (emulator->GetZ80()._rPC == oldPC)
		return;
	oldPC = emulator->GetZ80()._rPC;

    ostringstream tmp;

//Z80 Register

    if(z80_tab->IsShownOnScreen())
    {
        UpZ80Reg8bitsTextControl(m_tCrA, AF.b.h);
        UpZ80Reg8bitsTextControl(m_tCrF, AF.b.l);
        UpZ80Reg8bitsTextControl(m_tCrAx, AFx.b.h);
        UpZ80Reg8bitsTextControl(m_tCrFx, AFx.b.l);

        UpZ80Reg8bitsTextControl(m_tCrB, BC.b.h);
        UpZ80Reg8bitsTextControl(m_tCrC, BC.b.l);
        m_tCrBC->Clear();
        tmp.str("");
        tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().BC.w.l));
        *m_tCrBC << wxString::FromAscii((tmp.str()).data());

        UpZ80Reg8bitsTextControl(m_tCrBx, BCx.b.h);
        UpZ80Reg8bitsTextControl(m_tCrCx, BCx.b.l);
        m_tCrBCx->Clear();
        tmp.str("");
        tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().BCx.w.l));
        *m_tCrBCx << wxString::FromAscii((tmp.str()).data());

        UpZ80Reg8bitsTextControl(m_tCrD, DE.b.h);
        UpZ80Reg8bitsTextControl(m_tCrE, DE.b.l);
        m_tCrDE->Clear();
        tmp.str("");
        tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().DE.w.l));
        *m_tCrDE << wxString::FromAscii((tmp.str()).data());

        UpZ80Reg8bitsTextControl(m_tCrDx, DEx.b.h);
        UpZ80Reg8bitsTextControl(m_tCrEx, DEx.b.l);
        m_tCrDEx->Clear();
        tmp.str("");
        tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().DEx.w.l));
        *m_tCrDEx << wxString::FromAscii((tmp.str()).data());

        UpZ80Reg8bitsTextControl(m_tCrH, HL.b.h);
        UpZ80Reg8bitsTextControl(m_tCrL, HL.b.l);
        m_tCrHL->Clear();
        tmp.str("");
        tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().HL.w.l));
        *m_tCrHL << wxString::FromAscii((tmp.str()).data());

        UpZ80Reg8bitsTextControl(m_tCrHx, HLx.b.h);
        UpZ80Reg8bitsTextControl(m_tCrLx, HLx.b.l);
        m_tCrHLx->Clear();
        tmp.str("");
        tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().HLx.w.l));
        *m_tCrHLx << wxString::FromAscii((tmp.str()).data());

        m_tCrIX->Clear();
        tmp.str("");
        tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().IX.w.l));
        *m_tCrIX << wxString::FromAscii((tmp.str()).data());
        m_tCrIY->Clear();
        tmp.str("");
        tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().IY.w.l));
        *m_tCrIY << wxString::FromAscii((tmp.str()).data());

        m_tCrPC->Clear();
        tmp.str("");
        tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().PC.w.l));
        *m_tCrPC << wxString::FromAscii((tmp.str()).data());
        m_tCrSP->Clear();
        tmp.str("");
        tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().SP.w.l));
        *m_tCrSP << wxString::FromAscii((tmp.str()).data());

        UpZ80Reg8bitsTextControl(m_tCrI, I);
        UpZ80Reg8bitsTextControl(m_tCrR, R);
        
        m_cB_SFlags->SetValue((bool)(((emulator->GetZ80().AF.b.l) >> 7 ) & 1));
        m_cB_SFlagsShadow->SetValue((bool)(((emulator->GetZ80().AFx.b.l) >> 7 ) & 1));
        m_cB_ZFlags->SetValue((bool)(((emulator->GetZ80().AF.b.l) >> 6 ) & 1));
        m_cB_ZFlagsShadow->SetValue((bool)(((emulator->GetZ80().AFx.b.l) >> 6 ) & 1));
        m_cB_HFlags->SetValue((bool)(((emulator->GetZ80().AF.b.l) >> 4 ) & 1));
        m_cB_HFlagsShadow->SetValue((bool)(((emulator->GetZ80().AFx.b.l) >> 4 ) & 1));
        m_cB_PFlags->SetValue((bool)(((emulator->GetZ80().AF.b.l) >> 2 ) & 1));
        m_cB_PFlagsShadow->SetValue((bool)(((emulator->GetZ80().AFx.b.l) >> 2 ) & 1));
        m_cB_NFlags->SetValue((bool)(((emulator->GetZ80().AF.b.l) >> 1 ) & 1));
        m_cB_NFlagsShadow->SetValue((bool)(((emulator->GetZ80().AFx.b.l) >> 1 ) & 1));
        m_cB_CFlags->SetValue((bool)((emulator->GetZ80().AF.b.l) & 1));
        m_cB_CFlagsShadow->SetValue((bool)((emulator->GetZ80().AFx.b.l) & 1));

		// Z80 interrupt flags
		m_cb_interruptenabled->SetValue(emulator->GetZ80().IFF1);
		m_c_interruptMode->Select(emulator->GetZ80().IM);
		m_cb_interruptpending->SetValue(emulator->GetZ80().int_pending);
		// TODO : NMI ? (seems not handled by our z80!! it does a RST(0x66) on MF2 button)
    }

//     CRTC Register

    if(crtc_tab->IsShownOnScreen())
    {
        // Internal Counter
        if(!m_tCcHCC->IsModified())
        {
        m_tCcHCC->Clear();
        *m_tCcHCC << (int)(emulator->GetCRTC().GetCharCount());
        }
        if(!m_tCcVCC->IsModified())
        {
        m_tCcVCC->Clear();
        *m_tCcVCC << (int)(emulator->GetCRTC().GetLineCount());
        }
        if(!m_tCcVLC->IsModified())
        {
        m_tCcVLC->Clear();
        *m_tCcVLC << (int)(emulator->GetCRTC().GetRasterCount());
        }

        // Horizontal Register
        UpCRTCRegTextControl(m_tCcR0, 0);
        UpCRTCRegTextControl(m_tCcR1, 1);
        UpCRTCRegTextControl(m_tCcR2, 2);
        UpCRTCRegTextControl(m_tCcR3, 3);


        // Vertical Register
        UpCRTCRegTextControl(m_tCcR4, 4);
        UpCRTCRegTextControl(m_tCcR5, 5);
        UpCRTCRegTextControl(m_tCcR6, 6);
        UpCRTCRegTextControl(m_tCcR7, 7);
        UpCRTCRegTextControl(m_tCcR9, 9);

        tmp.str("");
        tmp << dec << ((int)(emulator->GetCRTC().GetRegisterValue(0) + 1)) << " us";
        m_sT_HLineLength->SetLabel(wxString::FromAscii((tmp.str()).data()));
        tmp.str("");
        tmp << dec << ((int)(emulator->GetCRTC().GetRegisterValue(0) + 1)*((emulator->GetCRTC().GetRegisterValue(4) + 1)*(emulator->GetCRTC().GetRegisterValue(9) + 1) + emulator->GetCRTC().GetRegisterValue(5))) << " us";
        m_sT_VFrameLength->SetLabel(wxString::FromAscii((tmp.str()).data()));
    }

	// GA registers
    if(ga_tab->IsShownOnScreen())
	{
		static const char ink2bas[32] = {13,27,19,25,1,7,10,16,28,29,24,26,6,8,15,17,30,31,18,20,0,2,9,11,4,22,21,23,3,5,12,14};
		m_pen->Clear();
		*m_pen << emulator->GetGateArray().GetPen();

		wxColour c;
		wxString s;
		int ink;

		ink = emulator->GetGateArray().GetInk(0);
		m_ink0->Clear();
		*m_ink0 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas0->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas0->SetForegroundColour(*wxWHITE);
		else
			m_bas0->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		emulator->logMessage(s.mb_str());
		m_bas0->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(1);
		m_ink1->Clear();
		*m_ink1 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas1->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas1->SetForegroundColour(*wxWHITE);
		else
			m_bas1->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas1->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(2);
		m_ink2->Clear();
		*m_ink2 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas2->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas2->SetForegroundColour(*wxWHITE);
		else
			m_bas2->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas2->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(3);
		m_ink3->Clear();
		*m_ink3 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas3->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas3->SetForegroundColour(*wxWHITE);
		else
			m_bas3->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas3->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(4);
		m_ink4->Clear();
		*m_ink4 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas4->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas4->SetForegroundColour(*wxWHITE);
		else
			m_bas4->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas4->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(5);
		m_ink5->Clear();
		*m_ink5 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas5->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas5->SetForegroundColour(*wxWHITE);
		else
			m_bas5->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas5->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(6);
		m_ink6->Clear();
		*m_ink6 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas6->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas6->SetForegroundColour(*wxWHITE);
		else
			m_bas6->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas6->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(7);
		m_ink7->Clear();
		*m_ink7 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas7->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas7->SetForegroundColour(*wxWHITE);
		else
			m_bas7->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas7->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(8);
		m_ink8->Clear();
		*m_ink8 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas8->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas8->SetForegroundColour(*wxWHITE);
		else
			m_bas8->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas8->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(9);
		m_ink9->Clear();
		*m_ink9 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas9->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas9->SetForegroundColour(*wxWHITE);
		else
			m_bas9->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas9->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(10);
		m_ink10->Clear();
		*m_ink10 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas10->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas10->SetForegroundColour(*wxWHITE);
		else
			m_bas10->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas10->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(11);
		m_ink11->Clear();
		*m_ink11 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas11->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas11->SetForegroundColour(*wxWHITE);
		else
			m_bas11->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas11->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(12);
		m_ink12->Clear();
		*m_ink12 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas12->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas12->SetForegroundColour(*wxWHITE);
		else
			m_bas12->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas12->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(13);
		m_ink13->Clear();
		*m_ink13 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas13->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas13->SetForegroundColour(*wxWHITE);
		else
			m_bas13->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas13->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(14);
		m_ink14->Clear();
		*m_ink14 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas14->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas14->SetForegroundColour(*wxWHITE);
		else
			m_bas14->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas14->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(15);
		m_ink15->Clear();
		*m_ink15 << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas15->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas15->SetForegroundColour(*wxWHITE);
		else
			m_bas15->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas15->SetValue(s); 

		ink = emulator->GetGateArray().GetInk(16);
		m_border->Clear();
		*m_border << ink;
		c.Set(emulator->GetRenderer().mapGAEntryToRGB(ink));
		m_bas16->SetBackgroundColour(c);
		if (ink == 0x14 || ink == 0x4 || ink == 0x18 || ink == 0x6 || ink == 0x2E || ink == 0x10)
			m_bas16->SetForegroundColour(*wxWHITE);
		else
			m_bas16->SetForegroundColour(*wxBLACK);
		s.Printf(wxT("%d"),ink2bas[ink]);
		m_bas16->SetValue(s); 


		m_vmode->Clear();
		*m_vmode << (int)emulator->GetGateArray().GetMode();
	}

	// FDC
    if(fdc_tab->IsShownOnScreen())
	{
		a_track->SetValue(wxString::Format(wxT("%d"),emulator->GetDriveA().current_track));
		a_sector->SetValue(wxString::Format(wxT("%d"),emulator->GetDriveA().current_sector));
		b_track->SetValue(wxString::Format(wxT("%d"),emulator->GetDriveB().current_track));
		b_sector->SetValue(wxString::Format(wxT("%d"),emulator->GetDriveB().current_sector));

		t_st0->SetValue(wxString::Format(wxT("%x"),emulator->GetFDC().getST(0)));
		t_st1->SetValue(wxString::Format(wxT("%x"),emulator->GetFDC().getST(1)));
		t_st2->SetValue(wxString::Format(wxT("%x"),emulator->GetFDC().getST(2)));
		t_st3->SetValue(wxString::Format(wxT("%x"),emulator->GetFDC().getST(3)));

		c_motor->SetValue(emulator->GetFDC().GetMotor());
	}
}

// Z80 8bits Registers Focus
void CapriceRegistersDialogImpl::OnKillFocusA( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrA, AF.b.h);
}
void CapriceRegistersDialogImpl::OnSetFocusA( wxFocusEvent& event )
{
	m_tCrA->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusAx( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrAx, AFx.b.h);
}
void CapriceRegistersDialogImpl::OnSetFocusAx( wxFocusEvent& event )
{
	m_tCrAx->MarkDirty();
}

void CapriceRegistersDialogImpl::OnKillFocusF( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrF, AF.b.l);
}
void CapriceRegistersDialogImpl::OnSetFocusF( wxFocusEvent& event )
{
	m_tCrF->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusFx( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrFx, AFx.b.l);
}
void CapriceRegistersDialogImpl::OnSetFocusFx( wxFocusEvent& event )
{
	m_tCrFx->MarkDirty();
}

void CapriceRegistersDialogImpl::OnKillFocusB( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrB, BC.b.h);
}
void CapriceRegistersDialogImpl::OnSetFocusB( wxFocusEvent& event )
{
	m_tCrB->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusBx( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrBx, BCx.b.h);
}
void CapriceRegistersDialogImpl::OnSetFocusBx( wxFocusEvent& event )
{
	m_tCrBx->MarkDirty();
}

void CapriceRegistersDialogImpl::OnKillFocusC( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrC, BC.b.l);
}
void CapriceRegistersDialogImpl::OnSetFocusC( wxFocusEvent& event )
{
	m_tCrC->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusCx( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrCx, BCx.b.l);
}
void CapriceRegistersDialogImpl::OnSetFocusCx( wxFocusEvent& event )
{
	m_tCrCx->MarkDirty();
}

void CapriceRegistersDialogImpl::OnKillFocusD( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrD, DE.b.h);
}
void CapriceRegistersDialogImpl::OnSetFocusD( wxFocusEvent& event )
{
	m_tCrD->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusDx( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrDx, DEx.b.h);
}
void CapriceRegistersDialogImpl::OnSetFocusDx( wxFocusEvent& event )
{
	m_tCrDx->MarkDirty();
}

void CapriceRegistersDialogImpl::OnKillFocusE( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrE, DE.b.l);
}
void CapriceRegistersDialogImpl::OnSetFocusE( wxFocusEvent& event )
{
	m_tCrE->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusEx( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrEx, DEx.b.l);
}
void CapriceRegistersDialogImpl::OnSetFocusEx( wxFocusEvent& event )
{
	m_tCrEx->MarkDirty();
}

void CapriceRegistersDialogImpl::OnKillFocusH( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrH, HL.b.h);
}
void CapriceRegistersDialogImpl::OnSetFocusH( wxFocusEvent& event )
{
	m_tCrH->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusHx( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrHx, HLx.b.h);
}
void CapriceRegistersDialogImpl::OnSetFocusHx( wxFocusEvent& event )
{
	m_tCrHx->MarkDirty();
}

void CapriceRegistersDialogImpl::OnKillFocusL( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrL, HL.b.l);
}
void CapriceRegistersDialogImpl::OnSetFocusL( wxFocusEvent& event )
{
	m_tCrL->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusLx( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrLx, HLx.b.l);
}
void CapriceRegistersDialogImpl::OnSetFocusLx( wxFocusEvent& event )
{
	m_tCrLx->MarkDirty();
}

void CapriceRegistersDialogImpl::OnKillFocusI( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrI, I);
}
void CapriceRegistersDialogImpl::OnSetFocusI( wxFocusEvent& event )
{
	m_tCrI->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusR( wxFocusEvent& event )
{
	ModifZ80Reg8bits(m_tCrR, R);
}
void CapriceRegistersDialogImpl::OnSetFocusR( wxFocusEvent& event )
{
	m_tCrR->MarkDirty();
}

// Crtc Registers Focus

void CapriceRegistersDialogImpl::OnTextR0( wxCommandEvent& event )
{
}

void CapriceRegistersDialogImpl::OnTextEnterR0( wxCommandEvent& event )
{
}

void CapriceRegistersDialogImpl::OnKillFocusHCC( wxFocusEvent& event )
{
}
void CapriceRegistersDialogImpl::OnSetFocusHCC( wxFocusEvent& event )
{
}
void CapriceRegistersDialogImpl::OnKillFocusVCC( wxFocusEvent& event )
{
}
void CapriceRegistersDialogImpl::OnSetFocusVCC( wxFocusEvent& event )
{
}
void CapriceRegistersDialogImpl::OnKillFocusVLA( wxFocusEvent& event )
{
}
void CapriceRegistersDialogImpl::OnSetFocusVLA( wxFocusEvent& event )
{
}
void CapriceRegistersDialogImpl::OnKillFocusVLC( wxFocusEvent& event )
{
}
void CapriceRegistersDialogImpl::OnSetFocusVLC( wxFocusEvent& event )
{
}
void CapriceRegistersDialogImpl::OnKillFocusR0( wxFocusEvent& event )
{
    ModifCRTCReg(m_tCcR0, 0);
}
void CapriceRegistersDialogImpl::OnSetFocusR0( wxFocusEvent& event )
{
    m_tCcR0->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusR1( wxFocusEvent& event )
{
    ModifCRTCReg(m_tCcR1, 1);
}
void CapriceRegistersDialogImpl::OnSetFocusR1( wxFocusEvent& event )
{
    m_tCcR1->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusR2( wxFocusEvent& event )
{
    ModifCRTCReg(m_tCcR2, 2);
}
void CapriceRegistersDialogImpl::OnSetFocusR2( wxFocusEvent& event )
{
    m_tCcR2->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusR3( wxFocusEvent& event )
{
    ModifCRTCReg(m_tCcR3, 3);
}
void CapriceRegistersDialogImpl::OnSetFocusR3( wxFocusEvent& event )
{
    m_tCcR3->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusR4( wxFocusEvent& event )
{
    ModifCRTCReg(m_tCcR4, 4);
}
void CapriceRegistersDialogImpl::OnSetFocusR4( wxFocusEvent& event )
{
    m_tCcR4->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusR5( wxFocusEvent& event )
{
    ModifCRTCReg(m_tCcR5, 5);
}
void CapriceRegistersDialogImpl::OnSetFocusR5( wxFocusEvent& event )
{
    m_tCcR5->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusR6( wxFocusEvent& event )
{
    ModifCRTCReg(m_tCcR6, 6);
}
void CapriceRegistersDialogImpl::OnSetFocusR6( wxFocusEvent& event )
{
    m_tCcR6->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusR7( wxFocusEvent& event )
{
    ModifCRTCReg(m_tCcR7, 7);
}
void CapriceRegistersDialogImpl::OnSetFocusR7( wxFocusEvent& event )
{
    m_tCcR7->MarkDirty();
}
void CapriceRegistersDialogImpl::OnKillFocusR9( wxFocusEvent& event )
{
    ModifCRTCReg(m_tCcR9, 9);
}
void CapriceRegistersDialogImpl::OnSetFocusR9( wxFocusEvent& event )
{
    m_tCcR9->MarkDirty();
}

void CapriceRegistersDialogImpl::OnCloseR( wxCloseEvent& event )
{
   cout << "Close" << endl;
   this->Destroy();
}
