/**
 *   $Id: CapriceWindowImpl.cpp 60 2008-11-25 17:08:52Z giot.romain $
 *   CapriceReloded an Amstrad CPC emulator
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



#include "CapriceWindowImpl.h"
#include "snapshot.h"
#include "video.h"
#include "CapriceInputSettingsImpl.h"

#include <wx/filedlg.h>
#include <wx/filename.h>

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

/*
 * Ask from the menu to change the disc in the drive B
 */
void CapriceWindowImpl::onInsertDiscB( wxCommandEvent& event )
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

        emulator->GetFDC().insertB(std::string(CurrentDocPath.mb_str()));
    }

}

/*
 * Ask from the menu to load a snapshot
 */
void CapriceWindowImpl::onLoadSNA( wxCommandEvent& event )
{
  wxFileDialog* OpenDialog = new wxFileDialog(
    this, wxT("Choose a file to open"), wxEmptyString, wxEmptyString,
    wxT("*SNA files (*.sna)|*.sna|All files|*.*"),
    wxOPEN, wxDefaultPosition);

  if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
  {
    wxString CurrentDocPath = OpenDialog->GetPath();
    SetTitle(wxString( wxT("Edit - ")) << OpenDialog->GetFilename()); // Set the Title to reflect the file open

    snapshot_load(*emulator, CurrentDocPath.mb_str());
  }
}

/*
 * Ask from the menu to save a snapshot
 */
void CapriceWindowImpl::onSaveSNA( wxCommandEvent& event )
{
  wxFileDialog* SaveDialog = new wxFileDialog(
    this, wxT("Choose a file to save"), wxEmptyString, wxEmptyString,
    wxT("*SNA files (*.sna)|*.sna|All files|*.*"),
    wxSAVE, wxDefaultPosition);

  if (SaveDialog->ShowModal() == wxID_OK) // if the user click "Save" instead of "Cancel"
  {
    wxFileName FileName = SaveDialog->GetPath();

    if(!FileName.HasExt())
    {
      FileName.SetExt(wxT("sna"));
    }

    snapshot_save(*emulator, FileName.GetFullPath().mb_str());
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

void CapriceWindowImpl::onMenuInput(wxCommandEvent& event)
{
    CapriceInputSettingsImpl* InputSettingsWindow = new CapriceInputSettingsImpl(this);
    InputSettingsWindow->Show(true);
}

// ============================= Various functions ===================================

/**
 * Get the Caprice SDL panel
 */
wxPanel* CapriceWindowImpl::getPanel() { return m_panel4; }

/**
 * Set the emulator
 * @param emulator Emulator to set
 */
void CapriceWindowImpl::SetEmulator(Emulator *emulator)
{
    this->emulator = emulator ;
//  panel2 = new SDLPanel(this, emulator);
}

/********************************************************
* Keyboard management : for now, pass the events to SDL *
********************************************************/
void CapriceWindowImpl::windowKeyDown( wxKeyEvent& event )
{
    SDL_Event evt;
    evt.type = SDL_KEYDOWN;
    evt.key.keysym.sym = (SDLKey)event.GetKeyCode();
    evt.key.keysym.mod = (SDLMod)event.GetModifiers();
    SDL_PushEvent(&evt);
}

void CapriceWindowImpl::windowKeyUp( wxKeyEvent& event )
{
    SDL_Event evt;
    evt.type = SDL_KEYUP;
    evt.key.keysym.sym = (SDLKey)event.GetKeyCode();
    evt.key.keysym.mod = (SDLMod)event.GetModifiers();
    SDL_PushEvent(&evt);

    cout << "keydown" << event.GetKeyCode() << endl;
}


// ============== DND ===================
bool CapriceWindowImpl::OnDropFiles(wxCoord x, wxCoord y , const wxArrayString& filenames)
{
    wxString CurrentDocPath = filenames[0];
    emulator->GetFDC().insertA(std::string(CurrentDocPath.mb_str()));
    return true;
}
