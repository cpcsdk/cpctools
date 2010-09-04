/**
 *   $Id: CapriceWindowImpl.cpp 60 2008-11-25 17:08:52Z giot.romain $
 *   Reloded an Amstrad CPC emulator
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


#include <wx/app.h>
#include "CapriceWindowImpl.h"
#include "snapshot.h"
#include "video.h"
#include "WXVideo.h"
#include "CapriceInputSettingsImpl.h"

#include "DiscEditor.h"
#include "MemoryImpl.h"
#include "tape.h"
#include "error.h"
#include "dsk.h"

#ifdef WITH_IDE
#include "CapriceIDE.h"
#endif

#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/dcbuffer.h>
#include <wx/msgdlg.h>

#include "Desass.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

CapriceWindowImpl::CapriceWindowImpl(WXEmulator* emu) 
	: EmulatorWindow(NULL)
{
	emulator = emu ;
	emulator->setWindow(this);
    m_panel4->SetFocus();
#if ENABLE_FILLDROP
	dndhandler= new CapriceDNDHandler(emulator);
	this->SetDropTarget(dndhandler);
#endif

#if __WXMAC__
	wxApp::s_macAboutMenuItemId = wxID_ABOUT; //TODO Need to correctly specify this id
	//wxApp::s_macPreferencesMenuItemId = PreferencesID;

#endif

	fdcActive = false;
}

CapriceWindowImpl::~CapriceWindowImpl()
{
}

// =============================== Window Event =============================================
void CapriceWindowImpl::onExit1( wxCloseEvent& event )
{
	// Check if the DSKs are modified.
	if (emulator->GetDriveA().altered &&
		wxMessageBox("DSK in drive A was modified ! Do you want to save before quitting ?",
			"Save DSK?", wxICON_QUESTION | wxYES_NO, this) == wxYES)
	{
		dsk_save(emulator->GetFDC().files[0].c_str(), &emulator->GetDriveA());
		emulator->GetDriveA().altered = false;
	}

	if (emulator->GetDriveB().altered &&
		wxMessageBox("DSK in drive A was modified ! Do you want to save before quitting ?",
			"Save DSK?", wxICON_QUESTION | wxYES_NO, this) == wxYES)
	{
		emulator->GetDriveB().altered = false;
		dsk_save(emulator->GetFDC().files[1].c_str(), &emulator->GetDriveB());
	}

	Close(); // This is not the good way, it will recursively call this event. But apparently, it works ?
}

/**
 * Do the emulation
 */
void CapriceWindowImpl::OnIdle( wxIdleEvent& event )
{
#ifdef USE_PTHREAD
    if(emulator->GetRenderer().GetVideoPlugin()->IsUpdate())
    {
        DebugLogMessage("TryLockOutput succes");
        emulator->GetRenderer().GetVideoPlugin()->LockOutput();
        wxImage *imgPlugin;
        imgPlugin = ((WXDoubleLinePlugin*)emulator->GetRenderer().GetVideoPlugin())->img;
        wxBitmap bmpPlugin = wxBitmap(*imgPlugin);
        wxBitmap bmp = bmpPlugin.GetSubBitmap(wxRect(0, 0, bmpPlugin.GetWidth(), bmpPlugin.GetHeight()));
        wxClientDC dc(getPanel());
        dc.DrawBitmap(bmp,0,0,false);

        emulator->GetRenderer().GetVideoPlugin()->UnlockOutput();
		if (emulator->GetConfig().paused || emulator->GetConfig().breakpoint) {
			int scrpos = emulator->GetRenderer().GetScreenPosition();
			const int width = 1024;
			int y = scrpos/width;
			scrpos -= y*width;
			dc.CrossHair(scrpos, y);
		}

        DebugLogMessage("Finish diplaying");
    }
    event.RequestMore(true);
//    event.Skip();
#else
    if (emulator && ! emulator->GetConfig().paused)
    {
        emulator->Emulate();
        //Ask to continue idle things
        event.RequestMore(true);
    }
    else if (emulator->GetConfig().breakpoint)
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
#endif
}

void CapriceWindowImpl::Pause() {
        //Pause mode
        m_menuItem_pause->Enable(false) ;
        m_menuItem_run->Enable(true);


		InfoLogMessage("Paused!");
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

        int error = emulator->GetFDC().insertA(std::string(CurrentDocPath.mb_str()));
		switch(error) {
			case 0:
        		SetTitle(wxString( wxT("Reloaded - ")) << OpenDialog->GetFilename()); // Set the Title to reflect the file open
				break;
			case ERR_DSK_INVALID:
				wxLogError("Invalid DSK file!");
		   		break;	   
			case ERR_DSK_SIDES:
				wxLogError("Wrong side count!");
				break;
			case ERR_DSK_SECTORS:
				wxLogError("Wrong sector count!");
				break;
			case ERR_DSK_WRITE:
				wxLogError("Write error!");
				break;
		}
    }

}

void CapriceWindowImpl::menu_editDiskA( wxCommandEvent& event ) {
	DiscEditorImpl* de = new DiscEditorImpl(this);
	de->Show(true);
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

        int error = emulator->GetFDC().insertB(std::string(CurrentDocPath.mb_str()));
		switch(error) {
			case 0:
        		SetTitle(wxString( wxT("Reloaded - ")) << OpenDialog->GetFilename()); // Set the Title to reflect the file open
				break;
			case ERR_DSK_INVALID:
				wxLogError("Invalid DSK file!");
		   		break;	   
			case ERR_DSK_SIDES:
				wxLogError("Wrong side count!");
				break;
			case ERR_DSK_SECTORS:
				wxLogError("Wrong sector count!");
				break;
			case ERR_DSK_WRITE:
				wxLogError("Write error!");
				break;
		}
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
    snapshotdir = OpenDialog->GetPath();
    SetTitle(wxString( wxT("Caprice - ")) << OpenDialog->GetFilename()); // Set the Title to reflect the file open

    snapshot_load(*emulator, snapshotdir.mb_str());
  }
}


void CapriceWindowImpl::onReloadSNA( wxCommandEvent& event )
{
    snapshot_load(*emulator, snapshotdir.mb_str());
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
	snapshotdir = FileName.GetFullPath();
  }

  delete SaveDialog;
}


void CapriceWindowImpl::onUpdateSNA( wxCommandEvent& event )
{
    snapshot_save(*emulator, snapshotdir.mb_str());
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
}

void CapriceWindowImpl::OnDebugMemory( wxCommandEvent& event )
{
	MemoryImpl* MemoryDialog = new MemoryImpl(this, emulator);
	MemoryDialog -> Show(true);
}

void CapriceWindowImpl::OnShowAssembler( wxCommandEvent& event)
{
	#ifdef WITH_IDE
	CapriceIDEImpl* ide = new CapriceIDEImpl(this, emulator);
	ide->Show(true);
	#else
	wxMessageDialog(this, wxT("The Integrated Development Environment wasn't enable in this build"), wxT("IDE not compiled!"), wxOK);
	#endif
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

void CapriceWindowImpl::onMultifaceStop( wxCommandEvent& event )
{
	emulator->GetZ80().z80_mf2stop();
}

void CapriceWindowImpl::insertTape( wxCommandEvent& event ) {
    wxFileDialog* OpenDialog = new wxFileDialog(
        this, wxT("Choose a file to open"), wxEmptyString, wxEmptyString, 
        wxT("*CDT files (*.cdt)|*.cdt|All files|*.*"),
        wxOPEN, wxDefaultPosition);
 
    // Creates a "open file" dialog with 4 file types
    if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
    {
        wxString CurrentDocPath = OpenDialog->GetPath();
        SetTitle(wxString( wxT("Caprice - ")) << 
            OpenDialog->GetFilename()); // Set the Title to reflect the file open

		emulator->GetTape().tape_insert(CurrentDocPath.mb_str());
    }
}


void CapriceWindowImpl::pressPlayOnTape( wxCommandEvent& event ) {
	emulator->GetConfig().tape_play_button = 1;
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

void CapriceWindowImpl::fdcLed(bool on) {
	fdcActive = on;

	wxClientDC dc(DriveActivity);
	if (on)
		dc.SetBrush(*wxRED_BRUSH);
	else
		dc.SetBrush(wxBrush(wxColor(127,0,0), wxSOLID));
	dc.FloodFill(3, 3, *wxWHITE, wxFLOOD_BORDER);
}


void CapriceWindowImpl::paintFDCLed( wxPaintEvent& event ) {
	wxPaintDC dc(DriveActivity);
	if (fdcActive)
		dc.SetBrush(*wxRED_BRUSH);
	else
		dc.SetBrush(wxBrush(wxColor(127,0,0), wxSOLID));
	dc.FloodFill(3, 3, *wxWHITE, wxFLOOD_BORDER);
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

