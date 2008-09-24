#ifndef _CAPRICEREGISTERSDIALOGIMPL_H_
#define _CAPRICEREGISTERSDIALOGIMPL_H_


#include "CapriceUI.h"
#include "emulator.h"
#include "z80.h"
#include "crtc.h"

class CapriceRegistersDialogImpl : public RegistersStates 
{
	public:
	CapriceRegistersDialogImpl() : RegistersStates(NULL)
	{
	}

	void SetEmulator(Emulator *emulator);	

	protected:
	virtual void OnCloseR( wxCloseEvent& event );
	virtual void OnIdleR( wxIdleEvent& event );
	virtual void OnInitR( wxInitDialogEvent& event );
	virtual void OnKillFocusHCC( wxFocusEvent& event );
	virtual void OnSetFocusHCC( wxFocusEvent& event );
	virtual void OnKillFocusVCC( wxFocusEvent& event );
	virtual void OnSetFocusVCC( wxFocusEvent& event );
	virtual void OnKillFocusVLA( wxFocusEvent& event );
	virtual void OnSetFocusVLA( wxFocusEvent& event );
	virtual void OnKillFocusVLC( wxFocusEvent& event );
	virtual void OnSetFocusVLC( wxFocusEvent& event );
	virtual void OnKillFocusR0( wxFocusEvent& event );
	virtual void OnSetFocusR0( wxFocusEvent& event );
	virtual void OnTextR0( wxCommandEvent& event );
	virtual void OnTextEnterR0( wxCommandEvent& event );
	virtual void OnKillFocusR1( wxFocusEvent& event );
	virtual void OnSetFocusR1( wxFocusEvent& event );
	virtual void OnKillFocusR2( wxFocusEvent& event );
	virtual void OnSetFocusR2( wxFocusEvent& event );
	virtual void OnKillFocusR3( wxFocusEvent& event );
	virtual void OnSetFocusR3( wxFocusEvent& event );
	virtual void OnKillFocusR4( wxFocusEvent& event );
	virtual void OnSetFocusR4( wxFocusEvent& event );
	virtual void OnKillFocusR5( wxFocusEvent& event );
	virtual void OnSetFocusR5( wxFocusEvent& event );
	virtual void OnKillFocusR6( wxFocusEvent& event );
	virtual void OnSetFocusR6( wxFocusEvent& event );
	virtual void OnKillFocusR7( wxFocusEvent& event );
	virtual void OnSetFocusR7( wxFocusEvent& event );
	virtual void OnKillFocusR9( wxFocusEvent& event );
	virtual void OnSetFocusR9( wxFocusEvent& event );

	private:
	Emulator* emulator;
};
#endif
