#ifndef _CAPRICEWINDOWIMPL_H_
#define _CAPRICEWINDOWIMPL_H_


#include "CapriceUI.h"
#include "CapriceRegistersDialogImpl.h"
#include "CapriceAboutImpl.h"
#include "emulator.h"

#include <wx/dnd.h>

class CapriceWindowImpl : public CapriceWindow , public wxFileDropTarget 
{
    public:
    CapriceWindowImpl() : CapriceWindow(NULL) 
    {
        paused = true ;
        this->SetDropTarget(this);
    }

    void SetEmulator(Emulator *emulator);   
    wxPanel* getPanel();

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
    virtual void OnPause( wxCommandEvent& event) ;

    virtual void OnRegister( wxCommandEvent& event);

    virtual void OnAbout( wxCommandEvent& event);
    virtual void onMenuInput( wxCommandEvent& event);

    virtual void windowKeyDown( wxKeyEvent& event);
    virtual void windowKeyUp( wxKeyEvent& event );

    // DND
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

    private:
    Emulator*   emulator ;
    bool paused ;
};
#endif

