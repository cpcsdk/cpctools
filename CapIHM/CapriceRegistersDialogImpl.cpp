#include "CapriceRegistersDialogImpl.h"
#include <wx/textctrl.h>
#include <iostream>
#include <sstream>
#include <string>

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
    m_tCcHCC->Clear();
    *m_tCcHCC << (int)(emulator->GetCRTC().GetCharCount());
    m_tCcVCC->Clear();
    *m_tCcVCC << (int)(emulator->GetCRTC().GetLineCount());
    m_tCcVLC->Clear();
    *m_tCcVLC << (int)(emulator->GetCRTC().GetRasterCount());
    
    // Horizontal Register
    //m_tCcR0->Clear();
    //*m_tCcR0 << (int)(emulator->GetCRTC().GetRegisterValue(0));
    m_tCcR1->Clear();
    *m_tCcR1 << (int)(emulator->GetCRTC().GetRegisterValue(1));
    m_tCcR2->Clear();
    *m_tCcR2 << (int)(emulator->GetCRTC().GetRegisterValue(2));
    m_tCcR3->Clear();
    *m_tCcR3 << (int)(emulator->GetCRTC().GetRegisterValue(3));
    
    // Vertical Register
    m_tCcR4->Clear();
    *m_tCcR4 << (int)(emulator->GetCRTC().GetRegisterValue(4));
    m_tCcR5->Clear();
    *m_tCcR5 << (int)(emulator->GetCRTC().GetRegisterValue(5));
    m_tCcR6->Clear();
    *m_tCcR6 << (int)(emulator->GetCRTC().GetRegisterValue(6));
    m_tCcR7->Clear();
    *m_tCcR7 << (int)(emulator->GetCRTC().GetRegisterValue(7));
    m_tCcR9->Clear();
    *m_tCcR9 << (int)(emulator->GetCRTC().GetRegisterValue(9));
    /*unsigned char GetRegisterValue(unsigned char reg) const;
	void SetRegisterValue(unsigned char reg, unsigned char val);*/
}

void CapriceRegistersDialogImpl::OnTextR0( wxCommandEvent& event )
{
    /*wxString str;
    long val;
    bool test;
    str = m_tCcR0->GetValue();
    cout << "str:" << (const char*)str.mb_str(wxConvUTF8) << endl;
    test = str.ToLong(&val,10);
    cout << "test : " << test << endl;
    cout << "val : " << val << endl;
    if(str.ToLong(&val,10))
    {
        cout << val;
        if((val > 0) && (val < 255));
        emulator->GetCRTC().SetRegisterValue(0, (unsigned char)(val));
    }
    ostringstream tmp;
    tmp << (int)(emulator->GetCRTC().GetRegisterValue(0));
    m_staticText50->SetLabel(wxString::FromAscii((tmp.str()).data()));*/
    //m_tCcR0->Clear();
    //*m_tCcR0 << (int)(emulator->GetCRTC().GetRegisterValue(0));
}

void CapriceRegistersDialogImpl::OnTextEnterR0( wxCommandEvent& event )
{
    wxString str;
    long val;
    bool test;
    str = m_tCcR0->GetValue();
    cout << "str:" << (const char*)str.mb_str(wxConvUTF8) << endl;
    test = str.ToLong(&val,10);
    cout << "test : " << test << endl;
    cout << "val : " << val << endl;
    if(str.ToLong(&val,10))
    {
        cout << val;
        if((val > 0) && (val < 255));
        emulator->GetCRTC().SetRegisterValue(0, (unsigned char)(val));
    }
    //m_tCcR0->Clear();
    //*m_tCcR0 << (int)(emulator->GetCRTC().GetRegisterValue(0));
}

void CapriceRegistersDialogImpl::OnKillFocusR0( wxFocusEvent& event )
{
    wxString str;
    long val;
    bool test;
    str = m_tCcR0->GetValue();
    cout << "str:" << (const char*)str.mb_str(wxConvUTF8) << endl;
    test = str.ToLong(&val,10);
    cout << "test : " << test << endl;
    cout << "val : " << val << endl;
    if(str.ToLong(&val,10))
    {
        cout << val;
        if((val > 0) && (val < 255));
        emulator->GetCRTC().SetRegisterValue(0, (unsigned char)(val));
    }
}

void CapriceRegistersDialogImpl::SetEmulator(Emulator *emulator)
{
	this->emulator = emulator ;
}