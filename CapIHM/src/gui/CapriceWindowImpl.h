#ifndef _CAPRICEWINDOWIMPL_H_
#define _CAPRICEWINDOWIMPL_H_


#include "CapriceUI.h"
#include "CapriceRegistersDialogImpl.h"
#include "CapriceAboutImpl.h"
#include "emulator.h"

#include <wx/dnd.h>
#include <wx/image.h>

#if ENABLE_FILEDROP
class CapriceDNDHandler : public wxFileDropTarget
{
	public:
		CapriceDNDHandler(Emulator* emu);
	protected:
    	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
	private:
		Emulator* emulator;
};
#endif

class CapriceWindowImpl : public CapriceWindow
{
  public:

	CapriceWindowImpl(Emulator* emu);
	virtual ~CapriceWindowImpl();

    void SetEmulator(Emulator *emulator);   
    Emulator* GetEmulator() {return emulator;};   
    /**
     * Get the emulator panel
     */
    wxPanel* getPanel();
    /**
     * Set the image to show
     */
    inline void SetEmuImage(wxImage* img)
    {
      this->image = img ;
    };

    protected:

    virtual void onExit1( wxCloseEvent& );
    virtual void OnIdle( wxIdleEvent& event );

    virtual void onInsertDiscA( wxCommandEvent& event );
    virtual void onInsertDiscB( wxCommandEvent& event );

    virtual void onLoadSNA( wxCommandEvent& event );
    virtual void onSaveSNA( wxCommandEvent& event );

    virtual void onExit2( wxCommandEvent& );
    
    
    virtual void OnFullScreen( wxCommandEvent& event);
    virtual void OnReset( wxCommandEvent& event);

    virtual void OnRun( wxCommandEvent& event) ;
    virtual void OnStep( wxCommandEvent& event) ;
    virtual void OnPause( wxCommandEvent& event) ;

    virtual void OnRegister( wxCommandEvent& event);
    virtual void OnDebugMemory( wxCommandEvent& event );

    virtual void OnAbout( wxCommandEvent& event);
    virtual void onMenuInput( wxCommandEvent& event);

    virtual void windowKeyDown( wxKeyEvent& event);
    virtual void windowKeyUp( wxKeyEvent& event );

    private:
    /**
     * Pointer to the emu
     */
    Emulator*   emulator ;
    /**
     * Pointer to the emu display
     */
    wxImage*    image;

#if ENABLE_FILEDROP
	CapriceDNDHandler* dndhandler;
#endif
};
#endif

