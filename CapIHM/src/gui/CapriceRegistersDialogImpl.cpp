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
#include <wx/textctrl.h>
#include <iostream>
#include <sstream>
#include <string>

#define  UpCRTCRegTextControl(X, Y) if(!X->IsModified()){X->Clear();*X << (int)(emulator->GetCRTC().GetRegisterValue(Y));}
#define  ModifCRTCReg(X, Y) {wxString str; long val; str = X->GetValue(); if(str.ToLong(&val,10)){cout << val; if((val > 0) && (val < 255)); emulator->GetCRTC().SetRegisterValue(Y, (unsigned char)(val));} X->Clear(); *X << (int)(emulator->GetCRTC().GetRegisterValue(Y));}

#define  UpZ80Reg8bitsTextControl(X, Y) if(!X->IsModified()){X->Clear();*X << (int)(emulator->GetZ80().Y);}
#define  ModifZ80Reg8bits(X, Y) {wxString str; long val; str = X->GetValue(); if(str.ToLong(&val,10)){cout << val; if((val > 0) && (val < 255)); emulator->GetZ80().Y = (unsigned char)(val);} X->Clear(); *X << (int)(emulator->GetZ80().Y);}

// Registers Dialog Event
void CapriceRegistersDialogImpl::OnInitR( wxInitDialogEvent& event )
{
    m_tCrA->Clear();
}

void CapriceRegistersDialogImpl::OnIdleR( wxIdleEvent& event )
{
    ostringstream tmp;

//Z80 Register

    if(m_nb_Register->GetSelection() == 0)
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

    if(m_nb_Register->GetSelection() == 1)
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
    if(m_nb_Register->GetSelection() == 2)
	{
		m_pen->Clear();
		*m_pen << emulator->GetGateArray().GetPen();

		m_ink0->Clear();
		*m_ink0 << emulator->GetGateArray().GetInk(0);
		m_ink1->Clear();
		*m_ink1 << emulator->GetGateArray().GetInk(1);
		m_ink2->Clear();
		*m_ink2 << emulator->GetGateArray().GetInk(2);
		m_ink3->Clear();
		*m_ink3 << emulator->GetGateArray().GetInk(3);
		m_ink4->Clear();
		*m_ink4 << emulator->GetGateArray().GetInk(4);
		m_ink5->Clear();
		*m_ink5 << emulator->GetGateArray().GetInk(5);
		m_ink6->Clear();
		*m_ink6 << emulator->GetGateArray().GetInk(6);
		m_ink7->Clear();
		*m_ink7 << emulator->GetGateArray().GetInk(7);
		m_ink8->Clear();
		*m_ink8 << emulator->GetGateArray().GetInk(8);
		m_ink9->Clear();
		*m_ink9 << emulator->GetGateArray().GetInk(9);
		m_ink10->Clear();
		*m_ink10 << emulator->GetGateArray().GetInk(10);
		m_ink11->Clear();
		*m_ink11 << emulator->GetGateArray().GetInk(11);
		m_ink12->Clear();
		*m_ink12 << emulator->GetGateArray().GetInk(12);
		m_ink13->Clear();
		*m_ink13 << emulator->GetGateArray().GetInk(13);
		m_ink14->Clear();
		*m_ink14 << emulator->GetGateArray().GetInk(14);
		m_ink15->Clear();
		*m_ink15 << emulator->GetGateArray().GetInk(15);
		m_border->Clear();
		*m_border << emulator->GetGateArray().GetInk(16);

		m_vmode->Clear();
		*m_vmode << (int)emulator->GetGateArray().GetMode();
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

void CapriceRegistersDialogImpl::SetEmulator(Emulator *emulator)
{
	this->emulator = emulator ;
}

void CapriceRegistersDialogImpl::OnCloseR( wxCloseEvent& event )
{
   cout << "Close" << endl;
   this->Destroy();
}
