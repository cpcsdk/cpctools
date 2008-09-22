#ifndef _CAPRICEWINDOWIMPL_H_
#define _CAPRICEWINDOWIMPL_H_


#include "CapriceUI.h"
#include "CapriceRegistersDialogImpl.h"
#include "emulator.h"

#   include "SDLPanel.h"


class CapriceWindowImpl : public CapriceWindow 
{
	public:
	CapriceWindowImpl() : CapriceWindow(NULL) 
	{
		// create the SDLPanel
	}

	void SetEmulator(Emulator *emulator);	

	protected:

	virtual void onExit1( wxCloseEvent& );
	virtual void OnIdle( wxIdleEvent& event );


	virtual void onInsertDiscA( wxCommandEvent& event );
	virtual void onExit2( wxCommandEvent& );
	
	
	virtual void OnFullScreen( wxCommandEvent& event);
	virtual void OnReset( wxCommandEvent& event);

	virtual void OnRun( wxCommandEvent& event) ;
	virtual void OnPause( wxCommandEvent& event) ;

	virtual void OnRegister( wxCommandEvent& event);

	virtual void OnAbout( wxCommandEvent& event);
	
	private:
	SDLPanel*	panel2;
	Emulator*	emulator ;
};
#endif
