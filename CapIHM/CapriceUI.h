///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 19 2008)
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
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/checkbox.h>
#include <wx/toolbar.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/listbox.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include <wx/imaglist.h>
#include <wx/dialog.h>
#include <wx/statbmp.h>
#include <wx/statline.h>
#include <wx/statbox.h>

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
		wxPanel* m_panel4;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onExit1( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnIdle( wxIdleEvent& event ){ event.Skip(); }
		virtual void onInsertDiscA( wxCommandEvent& event ){ event.Skip(); }
		virtual void onExit2( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnFullScreen( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnReset( wxCommandEvent& event ){ event.Skip(); }
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
		wxStaticBitmap* m_bitmap2;
		wxStaticText* m_staticText8;
		wxStaticText* m_staticText9;
		wxStaticText* m_staticText5;
		wxStaticText* m_staticText10;
	
	public:
		CapriceAbout( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About CapriceReloaded"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );
		~CapriceAbout();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class RegistersStates
///////////////////////////////////////////////////////////////////////////////
class RegistersStates : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebook2;
		wxPanel* m_panel4;
		wxStaticText* m_staticText9;
		wxStaticText* m_staticText10;
		wxStaticText* m_staticText11;
		wxStaticText* m_staticText12;
		wxTextCtrl* m_textCtrl3;
		wxTextCtrl* m_textCtrl4;
		wxStaticText* m_staticText13;
		wxTextCtrl* m_textCtrl5;
		wxTextCtrl* m_textCtrl6;
		wxStaticText* m_staticText14;
		wxTextCtrl* m_textCtrl7;
		wxTextCtrl* m_textCtrl8;
		wxStaticText* m_staticText15;
		wxTextCtrl* m_textCtrl9;
		wxTextCtrl* m_textCtrl10;
		wxStaticText* m_staticText16;
		wxTextCtrl* m_textCtrl11;
		wxTextCtrl* m_textCtrl12;
		wxStaticText* m_staticText17;
		wxTextCtrl* m_textCtrl13;
		wxTextCtrl* m_textCtrl14;
		wxStaticText* m_staticText18;
		wxTextCtrl* m_textCtrl15;
		wxTextCtrl* m_textCtrl16;
		wxStaticText* m_staticText19;
		wxTextCtrl* m_textCtrl17;
		wxTextCtrl* m_textCtrl18;
		wxStaticText* m_staticText31;
		wxTextCtrl* m_textCtrl33;
		wxStaticText* m_staticText32;
		wxStaticText* m_staticText33;
		wxTextCtrl* m_textCtrl34;
		wxStaticText* m_staticText21;
		wxStaticText* m_staticText22;
		wxStaticText* m_staticText23;
		wxStaticText* m_staticText24;
		wxTextCtrl* m_textCtrl19;
		wxTextCtrl* m_textCtrl20;
		wxStaticText* m_staticText25;
		wxTextCtrl* m_textCtrl21;
		wxTextCtrl* m_textCtrl22;
		wxStaticText* m_staticText26;
		wxTextCtrl* m_textCtrl23;
		wxTextCtrl* m_textCtrl24;
		wxStaticText* m_staticText27;
		wxTextCtrl* m_textCtrl25;
		wxTextCtrl* m_textCtrl26;
		wxStaticText* m_staticText28;
		wxTextCtrl* m_textCtrl27;
		wxTextCtrl* m_textCtrl28;
		wxStaticText* m_staticText29;
		wxTextCtrl* m_textCtrl29;
		wxStaticText* m_staticText43;
		wxStaticText* m_staticText30;
		wxTextCtrl* m_textCtrl31;
		wxStaticText* m_staticText34;
		wxStaticText* m_staticText35;
		wxStaticText* m_staticText36;
		wxStaticText* m_staticText37;
		wxStaticText* m_staticText38;
		wxStaticText* m_staticText39;
		wxStaticText* m_staticText40;
		wxStaticText* m_staticText41;
		wxCheckBox* m_checkBox3;
		wxCheckBox* m_checkBox4;
		wxCheckBox* m_checkBox5;
		wxCheckBox* m_checkBox6;
		wxCheckBox* m_checkBox7;
		wxCheckBox* m_checkBox8;
		wxStaticText* m_staticText42;
		wxCheckBox* m_checkBox9;
		wxCheckBox* m_checkBox10;
		wxCheckBox* m_checkBox11;
		wxCheckBox* m_checkBox12;
		wxCheckBox* m_checkBox13;
		wxCheckBox* m_checkBox14;
		wxPanel* m_panel5;
	
	public:
		RegistersStates( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Registers States"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );
		~RegistersStates();
	
};

#endif //__CapriceUI__
