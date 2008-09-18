///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 21 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __CapriceUI__
#define __CapriceUI__

#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/checkbox.h>
#include <wx/toolbar.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/listbox.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include <wx/imaglist.h>
#include <wx/dialog.h>
#include <wx/statbmp.h>
#include <wx/statline.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class CapriceWindow
///////////////////////////////////////////////////////////////////////////////
class CapriceWindow : public wxFrame 
{
	private:
	
	protected:
		wxStatusBar* m_statusBar1;
		wxMenuBar* m_menubar1;
		wxMenu* m_menu_file;
		wxMenu* m_menu_drivea;
		wxMenu* m_menu_drivea1;
		wxMenu* m_menu8;
		wxMenu* m_menu3;
		wxMenu* m_menu2;
		wxMenuItem* m_menuItem_run;
		wxMenuItem* m_menuItem_pause;
		wxMenu* m_menu4;
		wxMenu* m_menu5;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onExit1( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnIdle( wxIdleEvent& event ){ event.Skip(); }
		virtual void onInsertDiscA( wxCommandEvent& event ){ event.Skip(); }
		virtual void onExit2( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnFullScreen( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRun( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnPause( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRegister( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnShowAssembler( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		CapriceWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("CapriceReloaded"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("CapriceMainWindow") );
		~CapriceWindow();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DiscEditor
///////////////////////////////////////////////////////////////////////////////
class DiscEditor : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebook2;
		wxPanel* m_panel1;
		wxToolBar* m_toolBar1;
		wxCheckBox* m_checkBox1;
		wxCheckBox* m_checkBox2;
		wxListbook* m_listbook1;
		wxPanel* m_panel2;
		wxToolBar* m_toolBar3;
		wxStaticText* m_staticText23;
		wxSpinCtrl* m_spinCtrl1;
		wxStaticText* m_staticText24;
		wxListBox* m_listBox1;
		wxTextCtrl* m_textCtrl78;
	
	public:
		DiscEditor( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Disc Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 643,345 ), long style = wxDEFAULT_DIALOG_STYLE );
		~DiscEditor();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class FileProperties
///////////////////////////////////////////////////////////////////////////////
class FileProperties : public wxDialog 
{
	private:
	
	protected:
		wxStaticBitmap* m_bitmap2;
		wxTextCtrl* m_textCtrl80;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticText26;
		wxStaticText* m_staticText27;
		wxStaticLine* m_staticline2;
	
	public:
		FileProperties( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 411,367 ), long style = wxDEFAULT_DIALOG_STYLE );
		~FileProperties();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CapriceAbout
///////////////////////////////////////////////////////////////////////////////
class CapriceAbout : public wxDialog 
{
	private:
	
	protected:
	
	public:
		CapriceAbout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About CapriceReloadd"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~CapriceAbout();
	
};

#endif //__CapriceUI__
