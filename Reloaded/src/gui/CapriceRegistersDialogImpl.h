#ifndef _CAPRICEREGISTERSDIALOGIMPL_H_
#define _CAPRICEREGISTERSDIALOGIMPL_H_


#include "CapriceUI.h"
#include "WXEmulator.h"
#include "core/z80.h"
#include "core/crtc.h"


class CapriceRegistersDialogImpl : public RegistersStates 
{
	public:
	CapriceRegistersDialogImpl()
		: RegistersStates(NULL)
		, emulator(WXEmulator::getInstance())
	{
	}

	// void SetEmulator(WXEmulator *emulator);	

	protected:
	virtual void OnCloseR( wxCloseEvent& event );
	virtual void OnIdleR( wxIdleEvent& event );
	virtual void OnInitR( wxInitDialogEvent& event );
	virtual void OnKillFocusA( wxFocusEvent& event );
	virtual void OnSetFocusA( wxFocusEvent& event );
	virtual void OnKillFocusAx( wxFocusEvent& event );
	virtual void OnSetFocusAx( wxFocusEvent& event );
	virtual void OnKillFocusF( wxFocusEvent& event );
	virtual void OnSetFocusF( wxFocusEvent& event );
	virtual void OnKillFocusFx( wxFocusEvent& event );
	virtual void OnSetFocusFx( wxFocusEvent& event );
	virtual void OnKillFocusB( wxFocusEvent& event );
	virtual void OnSetFocusB( wxFocusEvent& event );
	virtual void OnKillFocusBx( wxFocusEvent& event );
	virtual void OnSetFocusBx( wxFocusEvent& event );
	virtual void OnKillFocusC( wxFocusEvent& event );
	virtual void OnSetFocusC( wxFocusEvent& event );
	virtual void OnKillFocusCx( wxFocusEvent& event );
	virtual void OnSetFocusCx( wxFocusEvent& event );
	virtual void OnKillFocusD( wxFocusEvent& event );
	virtual void OnSetFocusD( wxFocusEvent& event );
	virtual void OnKillFocusDx( wxFocusEvent& event );
	virtual void OnSetFocusDx( wxFocusEvent& event );
	virtual void OnKillFocusE( wxFocusEvent& event );
	virtual void OnSetFocusE( wxFocusEvent& event );
	virtual void OnKillFocusEx( wxFocusEvent& event );
	virtual void OnSetFocusEx( wxFocusEvent& event );
	virtual void OnKillFocusH( wxFocusEvent& event );
	virtual void OnSetFocusH( wxFocusEvent& event );
	virtual void OnKillFocusHx( wxFocusEvent& event );
	virtual void OnSetFocusHx( wxFocusEvent& event );
	virtual void OnKillFocusL( wxFocusEvent& event );
	virtual void OnSetFocusL( wxFocusEvent& event );
	virtual void OnKillFocusLx( wxFocusEvent& event );
	virtual void OnSetFocusLx( wxFocusEvent& event );
	virtual void OnKillFocusI( wxFocusEvent& event );
	virtual void OnSetFocusI( wxFocusEvent& event );
	virtual void OnKillFocusR( wxFocusEvent& event );
	virtual void OnSetFocusR( wxFocusEvent& event );
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
	WXEmulator* emulator;
};
#endif
