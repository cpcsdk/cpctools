/**
 *   $Id$
 *	 CapriceReloded an Amstrad CPC emulator
 *   Copyright (C) 2009  cpcsdk crew
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


#ifndef _CAPRICE_IDE_IMPL_H_
#define _CAPRICE_IDE_IMPL_H_

#include "wx/stedit/stedit.h"

#include "CapriceUI.h"
#include "WXEmulator.h"

/**
 * Window allowing the editing, compilation of assembly files
 */
class CapriceIDEImpl : public IDE
{
  public:
    CapriceIDEImpl(wxWindow* parent, WXEmulator *emulator);
    ~CapriceIDEImpl();


  protected:
    /**
     * Ask to create a new file
     */
    virtual void OnNew( wxCommandEvent& event ) ;
    /**
     * Ask to open a file
     */
		virtual void onOpen( wxCommandEvent& event ) ;
    /**
     * Ask to save the edited file
     */
		virtual void onSave( wxCommandEvent& event ) ;
    /**
     * Ask to save with another name
     */
		virtual void onSaveAs( wxCommandEvent& event ) ;
    /**
     * Ask to close the file
     */
		virtual void onClose( wxCommandEvent& event ) ;
    /**
     * Ask to exit the frame
     */
		virtual void onExit( wxCommandEvent& event ) ;
    /**
     * Ask to assemble the file
     */
    virtual void OnAssemble( wxCommandEvent& event);

  private:
    wxSTEditorNotebook* GetNotebook(){ return m_steNotebook; }


  private:
    //Inspiration from the steframe
    wxSplitterWindow*     m_mainSplitter ;
    wxSTEditorNotebook*     m_steNotebook;

    /**
     * Emulator (necessary for compilation)
     */
    WXEmulator* emulator;
};
#endif
