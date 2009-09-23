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
#include "CapriceIDE.h"
#include "snapshot.h"
#include "video.h"
#include "CapriceInputSettingsImpl.h"
#include "MemoryImpl.h"

#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/dcbuffer.h>
#include "CapriceApp.h"

#include "Desass.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

CapriceWindowImpl::CapriceWindowImpl(WXEmulator* emu) 
	: CapriceWindow(NULL)
{
	emulator = emu ;
  m_panel4->SetFocus();
#if ENABLE_FILLDROP
	dndhandler= new CapriceDNDHandler(emulator);
	this->SetDropTarget(dndhandler);
#endif
}

CapriceWindowImpl::~CapriceWindowImpl()
{
}

// =============================== Window Event =============================================
void CapriceWindowImpl::onExit1( wxCloseEvent& event )
{
  this->Destroy();
}

/**
 * Do the emulation
 */
void CapriceWindowImpl::OnIdle( wxIdleEvent& event )
{
	if (emulator && ! emulator->GetConfig().paused)
	{
        emulator->Emulate();
		wxClientDC dc(DriveActivity);
		if (emulator->GetFDC().GetMotor())
			dc.SetBrush(*wxRED_BRUSH);
		else
			dc.SetBrush(wxBrush(wxColor(127,0,0), wxSOLID));
		dc.FloodFill(3, 3, *wxWHITE, wxFLOOD_BORDER);
        //Ask to continue idle things
        event.RequestMore(true);
     }
    else
    { 
      if (emulator->GetConfig().breakpoint)
      {
         //TODO: use listeners to do that only when emulator is paused 
        //(due to pause not controlled by ihm)
        m_menuItem_pause->Enable(false) ;
        m_menuItem_run->Enable(true);

       //Breakpoint mode
       	if (IsShownOnScreen() && !IsIconized())
	      {
    	    wxBitmap bmp( *image);
    	    wxClientDC dc(getPanel());
		      dc.DrawBitmap(bmp,0,0,false);
	      }
      }
      else
      {
        //Pause mode
        //
        //TODO: use listeners to do that only when emulator is paused 
        //(due to pause not controlled by ihm)
        m_menuItem_pause->Enable(false) ;
        m_menuItem_run->Enable(true);


        //TODO: modify to do that only one time
        wxBitmap bitmap;
	      if (bitmap.LoadFile( wxT( DATA_PATH "pause.png"), wxBITMAP_TYPE_PNG))
        {
          wxClientDC dc(getPanel());
			    dc.DrawBitmap(bitmap,0,0,false);
        //    event.RequestMore(false);
        }
      }

    }
}

// =============================== Menus Event ===============================================

void CapriceWindowImpl::onExit2( wxCommandEvent& event )
{
  this->Close();
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

  delete SaveDialog;
}


/**
 * Operate a screen capture
 */
void CapriceWindowImpl::onSaveScreen( wxCommandEvent& event)
{
  emulator->SaveScreenshot("capture.png");
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

void CapriceWindowImpl::OnStep( wxCommandEvent& event)
{
    emulator->Step();
    m_menuItem_run->Enable(true);
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

void CapriceWindowImpl::OnDebugMemory( wxCommandEvent& event )
{
	MemoryImpl* MemoryDialog = new MemoryImpl(this, emulator);
	MemoryDialog -> Show(true);
}

void CapriceWindowImpl::OnShowAssembler( wxCommandEvent& event)
{
  CapriceIDEImpl* ide = new CapriceIDEImpl(this, emulator);
  ide->Show(true);
}

void CapriceWindowImpl::OnAbout( wxCommandEvent& event)
{
    CapriceAboutImpl *AboutDialog = new CapriceAboutImpl();
    AboutDialog->Show(true);
}

void CapriceWindowImpl::onMenuInput(wxCommandEvent& event)
{
	CapriceInputSettingsImpl* InputSettingsWindow
		= new CapriceInputSettingsImpl(this);
	InputSettingsWindow->m_notebook4->ChangeSelection(0);
	InputSettingsWindow->Show(true);
}

void CapriceWindowImpl::onMenuMemory(wxCommandEvent& event)
{
	CapriceInputSettingsImpl* InputSettingsWindow
		= new CapriceInputSettingsImpl(this);
	InputSettingsWindow->m_notebook4->ChangeSelection(1);
	InputSettingsWindow->Show(true);
}

void CapriceWindowImpl::onMenuVideo(wxCommandEvent& event)
{
	CapriceInputSettingsImpl* InputSettingsWindow
		= new CapriceInputSettingsImpl(this);
	InputSettingsWindow->m_notebook4->ChangeSelection(2);
	InputSettingsWindow->Show(true);
}

void CapriceWindowImpl::onMenuSound(wxCommandEvent& event)
{
	CapriceInputSettingsImpl* InputSettingsWindow
		= new CapriceInputSettingsImpl(this);
	InputSettingsWindow->m_notebook4->ChangeSelection(3);
	InputSettingsWindow->Show(true);
}

void CapriceWindowImpl::onMenuMisc(wxCommandEvent& event)
{
	CapriceInputSettingsImpl* InputSettingsWindow
		= new CapriceInputSettingsImpl(this);
	InputSettingsWindow->m_notebook4->ChangeSelection(4);
	InputSettingsWindow->Show(true);
}

// ============================= Various functions

/**
 * Get the Caprice SDL panel
 */
wxPanel* CapriceWindowImpl::getPanel() { return m_panel4; }

/********************************************************
* Keyboard management : for now, pass the events to SDL *
********************************************************/
void CapriceWindowImpl::windowKeyDown( wxKeyEvent& event )
{
	emulator->PressKey(event.GetKeyCode(),event.GetModifiers());
}

void CapriceWindowImpl::windowKeyUp( wxKeyEvent& event )
{
	emulator->ReleaseKey(event.GetKeyCode(),event.GetModifiers());
}

#if ENABLE_FILEDROP
// ============== DND ===================
CapriceDNDHandler::CapriceDNDHandler(WXEmulator* emu)
	: wxFileDropTarget()
{
	emulator=emu;
}

bool CapriceDNDHandler::OnDropFiles(wxCoord x, wxCoord y , const wxArrayString& filenames)
{
    wxString CurrentDocPath = filenames[0];
    wxFileName FileName(CurrentDocPath);
    if(FileName.HasExt())
    {
        std::cout << "FileDrop : " << std::string(FileName.GetExt().mb_str()) << endl;
        if(FileName.GetExt().CmpNoCase(wxT("dsk")) == 0)
        {
            std::cout << "Loading Disk file" << endl;
            emulator->GetFDC().insertA(std::string(CurrentDocPath.mb_str()));
        }
        else if(FileName.GetExt().CmpNoCase(wxT("sna")) == 0)
        {
            std::cout << "Loading Snapshot file" << endl;
            snapshot_load(*emulator, CurrentDocPath.mb_str());
        }
        else
        {
            cout << "This file type was unmanaged" << endl;
        }
    }
    return true;
}
#endif

