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
	virtual void OnIdleR( wxIdleEvent& event );
	virtual void OnInitR( wxInitDialogEvent& event );
	virtual void OnTextR0( wxCommandEvent& event );
	virtual void OnTextEnterR0( wxCommandEvent& event );
	virtual void OnKillFocusR0( wxFocusEvent& event );

	private:
	Emulator* emulator;
};
#endif
