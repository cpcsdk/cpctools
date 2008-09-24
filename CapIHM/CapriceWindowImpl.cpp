#include "CapriceWindowImpl.h"

#include <wx/filedlg.h>


// =============================== Window Event =============================================
void CapriceWindowImpl::onExit1( wxCloseEvent& event )
{
	exit(0);
}

/**
 * Do the emulation
 */
void CapriceWindowImpl::OnIdle( wxIdleEvent& event )
{

    emulator->Emulate();
    //Ask to continue idle things
    event.RequestMore(true);
}

// =============================== Menus Event ===============================================

void CapriceWindowImpl::onExit2( wxCommandEvent& event )
{
	exit(0);
}


/**
 * Ask from the menu to change the disc in the drive A
 */
void CapriceWindowImpl::onInsertDiscA( wxCommandEvent& event )
{
	wxFileDialog* OpenDialog = new wxFileDialog(
		this, wxT("Choose a file to open"), wxEmptyString, wxEmptyString, 
		wxT("*DSK files (*.dsk)|*.dsk|All files|*.*"),
		wxOPEN, wxDefaultPosition);
 
	// Creates a "open file" dialog with 4 file types
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		wxString CurrentDocPath = OpenDialog->GetPath();
		SetTitle(wxString( wxT("Edit - ")) << 
			OpenDialog->GetFilename()); // Set the Title to reflect the file open

		emulator->GetFDC().insertA(std::string(CurrentDocPath.mb_str()));
	}

}

void CapriceWindowImpl::OnPause( wxCommandEvent& event)
{
    emulator->Pause();
    m_menuItem_pause->Enable(false) ;
    m_menuItem_run->Enable(true);
}


void CapriceWindowImpl::OnRun( wxCommandEvent& event)
{
    emulator->Run();
    m_menuItem_pause->Enable(true) ;
    m_menuItem_run->Enable(false);

}

void CapriceWindowImpl::OnFullScreen( wxCommandEvent& event){
    emulator->GetRenderer().ToggleFullScreen() ;
}

void CapriceWindowImpl::OnReset( wxCommandEvent& event)
{
    emulator->emulator_reset(1);
}

void CapriceWindowImpl::OnRegister( wxCommandEvent& event)
{
    CapriceRegistersDialogImpl *RegistersDialog = new CapriceRegistersDialogImpl();
    RegistersDialog->Show(true);
    RegistersDialog->SetEmulator(emulator);
}

void CapriceWindowImpl::OnAbout( wxCommandEvent& event)
{
    CapriceAboutImpl *AboutDialog = new CapriceAboutImpl();
    AboutDialog->Show(true);
}

// ============================= Various functions ===================================

/**
 * Get the Caprice SDL panel
 */
//inline SDLPanel &CapriceWindowImpl::getPanel() { return *panel; }

/**
 * Set the emulator
 * @param emulator Emulator to set
 */
void CapriceWindowImpl::SetEmulator(Emulator *emulator)
{
	this->emulator = emulator ;
//	panel2 = new SDLPanel(this, emulator);
}
