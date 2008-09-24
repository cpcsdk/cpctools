#include "CapriceRegistersDialogImpl.h"
#include <wx/textctrl.h>
#include <iostream>
#include <sstream>
#include <string>

#define  UpCRTCRegTextControl(X, Y) if(!X->IsModified()){X->Clear();*X << (int)(emulator->GetCRTC().GetRegisterValue(Y));}
#define  ModifCRTCReg(X, Y) {wxString str; long val; bool test; str = X->GetValue(); if(str.ToLong(&val,10)){cout << val; if((val > 0) && (val < 255)); emulator->GetCRTC().SetRegisterValue(Y, (unsigned char)(val));} X->Clear(); *X << (int)(emulator->GetCRTC().GetRegisterValue(Y));}

// Registers Dialog Event
void CapriceRegistersDialogImpl::OnInitR( wxInitDialogEvent& event )
{
    m_tCrA->Clear();
}

void CapriceRegistersDialogImpl::OnIdleR( wxIdleEvent& event )
{
    ostringstream tmp;

    m_tCrA->Clear();
    *m_tCrA << (int)(emulator->GetZ80().AF.b.h);
    m_tCrF->Clear();
    *m_tCrF << (int)(emulator->GetZ80().AF.b.l);
    m_tCrAx->Clear();
    *m_tCrAx << (int)(emulator->GetZ80().AFx.b.h);
    m_tCrFx->Clear();
    *m_tCrFx << (int)(emulator->GetZ80().AFx.b.l);

    m_tCrB->Clear();
    *m_tCrB << (int)(emulator->GetZ80().BC.b.h);
    m_tCrC->Clear();
    *m_tCrC << (int)(emulator->GetZ80().BC.b.l);
    m_tCrBC->Clear();
    tmp.str("");
    tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().BC.w.l));
    *m_tCrBC << wxString::FromAscii((tmp.str()).data());

    m_tCrBx->Clear();
    *m_tCrBx << (int)(emulator->GetZ80().BCx.b.h);
    m_tCrCx->Clear();
    *m_tCrCx << (int)(emulator->GetZ80().BCx.b.l);
    m_tCrBCx->Clear();
    tmp.str("");
    tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().BCx.w.l));
    *m_tCrBCx << wxString::FromAscii((tmp.str()).data());

    m_tCrD->Clear();
    *m_tCrD << (int)(emulator->GetZ80().DE.b.h);
    m_tCrE->Clear();
    *m_tCrE << (int)(emulator->GetZ80().DE.b.l);
    m_tCrDE->Clear();
    tmp.str("");
    tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().DE.w.l));
    *m_tCrDE << wxString::FromAscii((tmp.str()).data());

    m_tCrDx->Clear();
    *m_tCrDx << (int)(emulator->GetZ80().DEx.b.h);
    m_tCrEx->Clear();
    *m_tCrEx << (int)(emulator->GetZ80().DEx.b.l);
    m_tCrDEx->Clear();
    tmp.str("");
    tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().DEx.w.l));
    *m_tCrDEx << wxString::FromAscii((tmp.str()).data());

    m_tCrH->Clear();
    *m_tCrH << (int)(emulator->GetZ80().HL.b.h);
    m_tCrL->Clear();
    *m_tCrL << (int)(emulator->GetZ80().HL.b.l);
    m_tCrHL->Clear();
    tmp.str("");
    tmp << "#" << uppercase << hex << ((int)(emulator->GetZ80().HL.w.l));
    *m_tCrHL << wxString::FromAscii((tmp.str()).data());

    m_tCrHx->Clear();
    *m_tCrHx << (int)(emulator->GetZ80().HLx.b.h);
    m_tCrLx->Clear();
    *m_tCrLx << (int)(emulator->GetZ80().HLx.b.l);
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

    m_tCrI->Clear();
    *m_tCrI << (int)(emulator->GetZ80().I);
    m_tCrR->Clear();
    *m_tCrR << (int)(emulator->GetZ80().R);

//     CRTC Register

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

/*    tmp.str("");
    tmp << ((int)(emulator->GetCRTC().GetRegisterValue(0) + 1)) << " µs";
    *m_sT_HLineLength << wxString::FromAscii((tmp.str()).data());
    tmp.str("");
    tmp << ((int)(emulator->GetCRTC().GetRegisterValue(0) + 1)*((emulator->GetCRTC().GetRegisterValue(4) + 1)*(emulator->GetCRTC().GetRegisterValue(9) + 1) + emulator->GetCRTC().GetRegisterValue(5) + 1)) << " µs";
    *m_sT_VFrameLength << wxString::FromAscii((tmp.str()).data());*/
}

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