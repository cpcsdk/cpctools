///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 19 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/imagpng.h>

#include "CapriceUI.h"

///////////////////////////////////////////////////////////////////////////

CapriceWindow::CapriceWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxFrame( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_statusBar1 = this->CreateStatusBar( 3, wxST_SIZEGRIP, wxID_ANY );
	m_menubar1 = new wxMenuBar( 0 );
	m_menu_file = new wxMenu();
	m_menu_drivea = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu_drivea, wxID_ANY, wxString( wxT("Insert Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menuItem1 );
	
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_menu_drivea, wxID_ANY, wxString( wxT("New Blank Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menuItem3 );
	
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_menu_drivea, wxID_ANY, wxString( wxT("Format Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menuItem4 );
	m_menuItem4->Enable( false );
	
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_menu_drivea, wxID_ANY, wxString( wxT("Edit Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menuItem5 );
	m_menuItem5->Enable( false );
	
	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( m_menu_drivea, wxID_ANY, wxString( wxT("Flip Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menuItem6 );
	
	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( m_menu_drivea, wxID_ANY, wxString( wxT("Remove Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menuItem7 );
	m_menuItem7->Enable( false );
	
	m_menu_file->Append( -1, wxT("Drive A:"), m_menu_drivea );
	
	m_menu_drivea1 = new wxMenu();
	wxMenuItem* m_menuItem11;
	m_menuItem11 = new wxMenuItem( m_menu_drivea1, wxID_ANY, wxString( wxT("Insert Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea1->Append( m_menuItem11 );
	
	wxMenuItem* m_menuItem31;
	m_menuItem31 = new wxMenuItem( m_menu_drivea1, wxID_ANY, wxString( wxT("New Blank Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea1->Append( m_menuItem31 );
	
	wxMenuItem* m_menuItem41;
	m_menuItem41 = new wxMenuItem( m_menu_drivea1, wxID_ANY, wxString( wxT("Format Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea1->Append( m_menuItem41 );
	m_menuItem41->Enable( false );
	
	wxMenuItem* m_menuItem51;
	m_menuItem51 = new wxMenuItem( m_menu_drivea1, wxID_ANY, wxString( wxT("Edit Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea1->Append( m_menuItem51 );
	m_menuItem51->Enable( false );
	
	wxMenuItem* m_menuItem61;
	m_menuItem61 = new wxMenuItem( m_menu_drivea1, wxID_ANY, wxString( wxT("Flip Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea1->Append( m_menuItem61 );
	
	wxMenuItem* m_menuItem71;
	m_menuItem71 = new wxMenuItem( m_menu_drivea1, wxID_ANY, wxString( wxT("Remove Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea1->Append( m_menuItem71 );
	m_menuItem71->Enable( false );
	
	m_menu_file->Append( -1, wxT("Drive B:"), m_menu_drivea1 );
	
	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Swap Disc A and  B") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem8 );
	
	wxMenuItem* m_menuItem24;
	m_menuItem24 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Load Cartridge") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem24 );
	
	m_menu8 = new wxMenu();
	wxMenuItem* m_menuItem25;
	m_menuItem25 = new wxMenuItem( m_menu8, wxID_ANY, wxString( wxT("Show Tape Controls") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu8->Append( m_menuItem25 );
	
	wxMenuItem* m_menuItem26;
	m_menuItem26 = new wxMenuItem( m_menu8, wxID_ANY, wxString( wxT("Insert Tape Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu8->Append( m_menuItem26 );
	
	wxMenuItem* m_menuItem27;
	m_menuItem27 = new wxMenuItem( m_menu8, wxID_ANY, wxString( wxT("Rewind Tape") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu8->Append( m_menuItem27 );
	m_menuItem27->Enable( false );
	
	wxMenuItem* m_menuItem28;
	m_menuItem28 = new wxMenuItem( m_menu8, wxID_ANY, wxString( wxT("Remove Tape") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu8->Append( m_menuItem28 );
	m_menuItem28->Enable( false );
	
	wxMenuItem* m_menuItem29;
	m_menuItem29 = new wxMenuItem( m_menu8, wxID_ANY, wxString( wxT("Press Play") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu8->Append( m_menuItem29 );
	m_menuItem29->Enable( false );
	
	wxMenuItem* m_menuItem30;
	m_menuItem30 = new wxMenuItem( m_menu8, wxID_ANY, wxString( wxT("Press Record") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu8->Append( m_menuItem30 );
	
	m_menu_file->Append( -1, wxT("Tape"), m_menu8 );
	
	m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menuItem311;
	m_menuItem311 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Load Snapshot") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem311 );
	
	wxMenuItem* m_menuItem32;
	m_menuItem32 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Save Snapshot") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem32 );
	
	wxMenuItem* m_menuItem33;
	m_menuItem33 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Update Snapshot") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem33 );
	
	m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menuItem34;
	m_menuItem34 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Playback Session") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem34 );
	
	wxMenuItem* m_menuItem35;
	m_menuItem35 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Record Session") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem35 );
	
	m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menuItem36;
	m_menuItem36 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Save Screenshot") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem36 );
	
	wxMenuItem* m_menuItem37;
	m_menuItem37 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Record AVI") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem37 );
	
	wxMenuItem* m_menuItem38;
	m_menuItem38 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Record WAV") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem38 );
	
	wxMenuItem* m_menuItem39;
	m_menuItem39 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Record YM") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem39 );
	
	m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menuItem40;
	m_menuItem40 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Pokes") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem40 );
	
	m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menuItem411;
	m_menuItem411 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Auto Types") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem411 );
	
	wxMenuItem* m_menuItem42;
	m_menuItem42 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Paste") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem42 );
	
	m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menu_exit;
	m_menu_exit = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menu_exit );
	
	m_menubar1->Append( m_menu_file, wxT("File") );
	
	m_menu3 = new wxMenu();
	wxMenuItem* m_menuItem43;
	m_menuItem43 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("General") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem43 );
	
	wxMenuItem* m_menuItem44;
	m_menuItem44 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Display") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem44 );
	
	wxMenuItem* m_menuItem45;
	m_menuItem45 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Sound") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem45 );
	
	wxMenuItem* m_menuItem46;
	m_menuItem46 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Memory") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem46 );
	
	wxMenuItem* m_menuItem47;
	m_menuItem47 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Input") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem47 );
	
	wxMenuItem* m_menuItem48;
	m_menuItem48 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Other") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem48 );
	
	m_menu3->AppendSeparator();
	
	wxMenuItem* m_menuItem49;
	m_menuItem49 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Full Screen") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem49 );
	
	wxMenuItem* m_menuItem50;
	m_menuItem50 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Reset") ) + wxT('\t') + wxT("F5"), wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem50 );
	
	wxMenuItem* m_menuItem511;
	m_menuItem511 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("AMX Mouse") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem511 );
	
	wxMenuItem* m_menuItem52;
	m_menuItem52 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Multiface Stop") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem52 );
	
	m_menubar1->Append( m_menu3, wxT("Settings") );
	
	m_menu2 = new wxMenu();
	m_menuItem_run = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Run") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem_run );
	m_menuItem_run->Enable( false );
	
	m_menuItem_pause = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Pause") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem_pause );
	
	wxMenuItem* m_menuItem_registers;
	m_menuItem_registers = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Register") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem_registers );
	
	m_menubar1->Append( m_menu2, wxT("Debug") );
	
	m_menu4 = new wxMenu();
	wxMenuItem* m_menuItem57;
	m_menuItem57 = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Show Assembler") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem57 );
	
	m_menubar1->Append( m_menu4, wxT("Assembler") );
	
	m_menu5 = new wxMenu();
	wxMenuItem* m_menuItem58;
	m_menuItem58 = new wxMenuItem( m_menu5, wxID_ANY, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( m_menuItem58 );
	
	m_menubar1->Append( m_menu5, wxT("Help") );
	
	this->SetMenuBar( m_menubar1 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer12->Add( m_panel4, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer12 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CapriceWindow::onExit1 ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( CapriceWindow::OnIdle ) );
	this->Connect( m_menuItem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onInsertDiscA ) );
	this->Connect( m_menuItem11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onInsertDiscA ) );
	this->Connect( m_menu_exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onExit2 ) );
	this->Connect( m_menuItem49->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnFullScreen ) );
	this->Connect( m_menuItem50->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnReset ) );
	this->Connect( m_menuItem_run->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnRun ) );
	this->Connect( m_menuItem_pause->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnPause ) );
	this->Connect( m_menuItem_registers->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnRegister ) );
	this->Connect( m_menuItem57->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnShowAssembler ) );
	this->Connect( m_menuItem58->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnAbout ) );
}

CapriceWindow::~CapriceWindow()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CapriceWindow::onExit1 ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( CapriceWindow::OnIdle ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onInsertDiscA ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onInsertDiscA ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onExit2 ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnFullScreen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnReset ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnRun ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnPause ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnRegister ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnShowAssembler ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnAbout ) );
}

DiscEditor::DiscEditor( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	wxSize m_notebook2ImageSize = wxSize( 2,-1 );
	int m_notebook2Index = 0;
	wxImageList* m_notebook2Images = new wxImageList( m_notebook2ImageSize.GetWidth(), m_notebook2ImageSize.GetHeight() );
	m_notebook2->AssignImageList( m_notebook2Images );
	wxBitmap m_notebook2Bitmap;
	wxImage m_notebook2Image;
	m_panel1 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_toolBar1 = new wxToolBar( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL ); 
	m_checkBox1 = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("Show System files"), wxDefaultPosition, wxDefaultSize, 0 );
	
	m_toolBar1->AddControl( m_checkBox1 );
	m_checkBox2 = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("Add/Remove Amsdos Header"), wxDefaultPosition, wxDefaultSize, 0 );
	
	m_toolBar1->AddControl( m_checkBox2 );
	m_toolBar1->Realize();
	
	bSizer4->Add( m_toolBar1, 0, wxEXPAND, 5 );
	
	m_listbook1 = new wxListbook( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_DEFAULT );
	#ifndef __WXGTK__ // Small icon style not supported in GTK
	wxListView* m_listbook1ListView = m_listbook1->GetListView();
	long m_listbook1Flags = m_listbook1ListView->GetWindowStyleFlag();
	m_listbook1Flags = ( m_listbook1Flags & ~wxLC_ICON ) | wxLC_SMALL_ICON;
	m_listbook1ListView->SetWindowStyleFlag( m_listbook1Flags );
	#endif
	
	bSizer4->Add( m_listbook1, 1, wxEXPAND | wxALL, 5 );
	
	m_panel1->SetSizer( bSizer4 );
	m_panel1->Layout();
	bSizer4->Fit( m_panel1 );
	m_notebook2->AddPage( m_panel1, wxT("File System"), false );
	m_panel2 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_toolBar3 = new wxToolBar( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL ); 
	m_staticText23 = new wxStaticText( m_toolBar3, wxID_ANY, wxT("Track:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	m_toolBar3->AddControl( m_staticText23 );
	m_spinCtrl1 = new wxSpinCtrl( m_toolBar3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	m_toolBar3->AddControl( m_spinCtrl1 );
	m_staticText24 = new wxStaticText( m_toolBar3, wxID_ANY, wxT("Sector:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	m_toolBar3->AddControl( m_staticText24 );
	m_listBox1 = new wxListBox( m_toolBar3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB|wxLB_SINGLE );
	m_listBox1->Append( wxT("C1") );
	m_listBox1->Append( wxT("C2") );
	m_listBox1->Append( wxT("C3") );
	m_listBox1->Append( wxT("C4") );
	m_listBox1->Append( wxT("C5") );
	m_listBox1->Append( wxT("C6") );
	m_listBox1->Append( wxT("C7") );
	m_listBox1->Append( wxT("C8") );
	m_listBox1->Append( wxT("C9") );
	m_toolBar3->AddControl( m_listBox1 );
	m_toolBar3->Realize();
	
	bSizer5->Add( m_toolBar3, 0, wxEXPAND, 5 );
	
	m_textCtrl78 = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_textCtrl78->Enable( false );
	
	bSizer5->Add( m_textCtrl78, 0, wxALL, 5 );
	
	m_panel2->SetSizer( bSizer5 );
	m_panel2->Layout();
	bSizer5->Fit( m_panel2 );
	m_notebook2->AddPage( m_panel2, wxT("Sector Editor"), true );
	
	bSizer2->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
}

DiscEditor::~DiscEditor()
{
}

FileProperties::FileProperties( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_bitmap2 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_bitmap2, 0, wxALL, 5 );
	
	m_textCtrl80 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_textCtrl80, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer7, 0, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer6->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText26 = new wxStaticText( this, wxID_ANY, wxT("Type Of File"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	bSizer8->Add( m_staticText26, 0, wxALL, 5 );
	
	m_staticText27 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	bSizer8->Add( m_staticText27, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer8, 0, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer6->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	bSizer6->Add( bSizer9, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer6 );
	this->Layout();
}

FileProperties::~FileProperties()
{
}

CapriceAbout::CapriceAbout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	m_bitmap2 = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("logo.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_bitmap2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	bSizer9->Add( bSizer14, 1, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Caprice Reloaded"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText8->Wrap( -1 );
	m_staticText8->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Sans") ) );
	
	bSizer15->Add( m_staticText8, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("An OpenSource Amstrad CPC Emulator"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText9->Wrap( -1 );
	m_staticText9->SetFont( wxFont( 12, 74, 90, 92, false, wxT("Sans") ) );
	
	bSizer15->Add( m_staticText9, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	bSizer9->Add( bSizer15, 0, wxEXPAND, 5 );
	
	bSizer8->Add( bSizer9, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("The Original Caprice is :\n(c) 1997 - 2005 Ulrich Doewich\n\nCaprice Reloaded is :\n(c) 1997 - 2008 The Caprice Reloaded Author's"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText5->Wrap( -1 );
	bSizer10->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	bSizer8->Add( bSizer10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("Credit :\nWrited by :\nUlrich Doewich\nRamlaid/Arkos <contact@ramlaid.com>\nRomain Giot  - Krusty/Benediction <krusty@cpcscene.com>\nContributor :\nPulkoMandy/Shinra <pulkomandy@gmail.com>\nCloudStrife/Shinra <cloudstrife@cpcscene.com>\nAnd a lot of other contributor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer11->Add( m_staticText10, 0, wxALL|wxEXPAND, 5 );
	
	bSizer8->Add( bSizer11, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer8 );
	this->Layout();
	bSizer8->Fit( this );
}

CapriceAbout::~CapriceAbout()
{
}

RegistersStates::RegistersStates( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel4 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("8bits Registers") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 1, 3, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText9 = new wxStaticText( m_panel4, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	m_staticText9->Hide();
	
	fgSizer3->Add( m_staticText9, 0, wxALL, 5 );
	
	m_staticText10 = new wxStaticText( m_panel4, wxID_ANY, wxT("Primary"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer3->Add( m_staticText10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText11 = new wxStaticText( m_panel4, wxID_ANY, wxT("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer3->Add( m_staticText11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText12 = new wxStaticText( m_panel4, wxID_ANY, wxT("A"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer3->Add( m_staticText12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCrA = new wxTextCtrl( m_panel4, wxID_ANY, wxT("a"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrA, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_tCrAx = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrAx, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText13 = new wxStaticText( m_panel4, wxID_ANY, wxT("F"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer3->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCrF = new wxTextCtrl( m_panel4, wxID_ANY, wxT("f"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrF, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_tCrFx = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrFx, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText14 = new wxStaticText( m_panel4, wxID_ANY, wxT("B"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer3->Add( m_staticText14, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrB = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrB, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_tCrBx = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrBx, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText15 = new wxStaticText( m_panel4, wxID_ANY, wxT("C"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	fgSizer3->Add( m_staticText15, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrC = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrC, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_tCrCx = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrCx, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText16 = new wxStaticText( m_panel4, wxID_ANY, wxT("D"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	fgSizer3->Add( m_staticText16, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrD = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrD, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_tCrDx = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrDx, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText17 = new wxStaticText( m_panel4, wxID_ANY, wxT("E"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	fgSizer3->Add( m_staticText17, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrE = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrE, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_tCrEx = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrEx, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText18 = new wxStaticText( m_panel4, wxID_ANY, wxT("H"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer3->Add( m_staticText18, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrH = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrH, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_tCrHx = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrHx, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText19 = new wxStaticText( m_panel4, wxID_ANY, wxT("L"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	fgSizer3->Add( m_staticText19, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrL = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrL, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_tCrLx = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrLx, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText31 = new wxStaticText( m_panel4, wxID_ANY, wxT("I"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	fgSizer3->Add( m_staticText31, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrI = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrI, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText32 = new wxStaticText( m_panel4, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	m_staticText32->Hide();
	
	fgSizer3->Add( m_staticText32, 0, wxALL, 5 );
	
	m_staticText33 = new wxStaticText( m_panel4, wxID_ANY, wxT("R"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33->Wrap( -1 );
	fgSizer3->Add( m_staticText33, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrR = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrR, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer1->Add( fgSizer3, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	sbSizer1->Add( fgSizer4, 1, wxEXPAND, 5 );
	
	fgSizer2->Add( sbSizer1, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("16bits Registers") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText21 = new wxStaticText( m_panel4, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	m_staticText21->Hide();
	
	fgSizer5->Add( m_staticText21, 0, wxALL, 5 );
	
	m_staticText22 = new wxStaticText( m_panel4, wxID_ANY, wxT("Primary"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	fgSizer5->Add( m_staticText22, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText23 = new wxStaticText( m_panel4, wxID_ANY, wxT("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	fgSizer5->Add( m_staticText23, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText24 = new wxStaticText( m_panel4, wxID_ANY, wxT("BC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer5->Add( m_staticText24, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrBC = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_tCrBC, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrBCx = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_tCrBCx, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText25 = new wxStaticText( m_panel4, wxID_ANY, wxT("DE"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	fgSizer5->Add( m_staticText25, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrDE = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_tCrDE, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrDEx = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_tCrDEx, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText26 = new wxStaticText( m_panel4, wxID_ANY, wxT("HL"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	fgSizer5->Add( m_staticText26, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrHL = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_tCrHL, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrHLx = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_tCrHLx, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText27 = new wxStaticText( m_panel4, wxID_ANY, wxT("IX"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	fgSizer5->Add( m_staticText27, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrIX = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_tCrIX, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText45 = new wxStaticText( m_panel4, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText45->Wrap( -1 );
	m_staticText45->Hide();
	
	fgSizer5->Add( m_staticText45, 0, wxALL, 5 );
	
	m_staticText28 = new wxStaticText( m_panel4, wxID_ANY, wxT("IY"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	fgSizer5->Add( m_staticText28, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrIY = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_tCrIY, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText44 = new wxStaticText( m_panel4, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	m_staticText44->Hide();
	
	fgSizer5->Add( m_staticText44, 0, wxALL, 5 );
	
	m_staticText29 = new wxStaticText( m_panel4, wxID_ANY, wxT("PC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText29->Wrap( -1 );
	fgSizer5->Add( m_staticText29, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrPC = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_tCrPC, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText43 = new wxStaticText( m_panel4, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText43->Wrap( -1 );
	m_staticText43->Hide();
	
	fgSizer5->Add( m_staticText43, 0, wxALL, 5 );
	
	m_staticText30 = new wxStaticText( m_panel4, wxID_ANY, wxT("SP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer5->Add( m_staticText30, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrSP = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_tCrSP, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	sbSizer2->Add( fgSizer5, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	bSizer18->Add( sbSizer2, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("Flags States") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 2, 7, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText34 = new wxStaticText( m_panel4, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	m_staticText34->Hide();
	
	fgSizer6->Add( m_staticText34, 0, wxALL, 5 );
	
	m_staticText35 = new wxStaticText( m_panel4, wxID_ANY, wxT("S"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	fgSizer6->Add( m_staticText35, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText36 = new wxStaticText( m_panel4, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	fgSizer6->Add( m_staticText36, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText37 = new wxStaticText( m_panel4, wxID_ANY, wxT("H"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText37->Wrap( -1 );
	fgSizer6->Add( m_staticText37, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText38 = new wxStaticText( m_panel4, wxID_ANY, wxT("P"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	fgSizer6->Add( m_staticText38, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText39 = new wxStaticText( m_panel4, wxID_ANY, wxT("N"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	fgSizer6->Add( m_staticText39, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText40 = new wxStaticText( m_panel4, wxID_ANY, wxT("C"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText40->Wrap( -1 );
	fgSizer6->Add( m_staticText40, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText41 = new wxStaticText( m_panel4, wxID_ANY, wxT("Primary"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	fgSizer6->Add( m_staticText41, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_checkBox3 = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( m_checkBox3, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	m_checkBox4 = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( m_checkBox4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_checkBox5 = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( m_checkBox5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_checkBox6 = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( m_checkBox6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_checkBox7 = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( m_checkBox7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_checkBox8 = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( m_checkBox8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_staticText42 = new wxStaticText( m_panel4, wxID_ANY, wxT("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer6->Add( m_staticText42, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_checkBox9 = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( m_checkBox9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_checkBox10 = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( m_checkBox10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_checkBox11 = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( m_checkBox11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_checkBox12 = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( m_checkBox12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_checkBox13 = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( m_checkBox13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_checkBox14 = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer6->Add( m_checkBox14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	sbSizer3->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	bSizer18->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	fgSizer2->Add( bSizer18, 0, wxEXPAND, 5 );
	
	bSizer23->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("Shadow Register States") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_checkBox15 = new wxCheckBox( m_panel4, wxID_ANY, wxT("AF Register in Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer14->Add( m_checkBox15, 0, wxALL, 5 );
	
	m_checkBox16 = new wxCheckBox( m_panel4, wxID_ANY, wxT("General Register in Shadow Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer14->Add( m_checkBox16, 0, wxALL, 5 );
	
	sbSizer7->Add( fgSizer14, 1, wxEXPAND, 5 );
	
	bSizer24->Add( sbSizer7, 1, wxEXPAND, 5 );
	
	bSizer23->Add( bSizer24, 0, wxEXPAND, 5 );
	
	m_panel4->SetSizer( bSizer23 );
	m_panel4->Layout();
	bSizer23->Fit( m_panel4 );
	m_notebook2->AddPage( m_panel4, wxT("Z80 Registers"), false );
	m_panel8 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_panel8, wxID_ANY, wxT("Internal Counter") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText46 = new wxStaticText( m_panel8, wxID_ANY, wxT("HCC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText46->Wrap( -1 );
	fgSizer9->Add( m_staticText46, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCcHCC = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcHCC->SetToolTip( wxT("Horizontal Caracter Counter") );
	
	fgSizer9->Add( m_tCcHCC, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText47 = new wxStaticText( m_panel8, wxID_ANY, wxT("VCC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText47->Wrap( -1 );
	fgSizer9->Add( m_staticText47, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCcVCC = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcVCC->SetToolTip( wxT("Vertical Caracter Counter") );
	
	fgSizer9->Add( m_tCcVCC, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText48 = new wxStaticText( m_panel8, wxID_ANY, wxT("VLA"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	fgSizer9->Add( m_staticText48, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCcVLA = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcVLA->SetToolTip( wxT("Vertical Line Adjust") );
	
	fgSizer9->Add( m_tCcVLA, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText49 = new wxStaticText( m_panel8, wxID_ANY, wxT("VLC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText49->Wrap( -1 );
	fgSizer9->Add( m_staticText49, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCcVLC = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcVLC->SetToolTip( wxT("Vertical Line Counter") );
	
	fgSizer9->Add( m_tCcVLC, 0, wxALIGN_CENTER|wxALL, 5 );
	
	sbSizer4->Add( fgSizer9, 1, wxEXPAND, 5 );
	
	fgSizer8->Add( sbSizer4, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( m_panel8, wxID_ANY, wxT("Horizontal Register") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText50 = new wxStaticText( m_panel8, wxID_ANY, wxT("R0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( -1 );
	fgSizer11->Add( m_staticText50, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR0 = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcR0->SetToolTip( wxT("Horizontal total character number") );
	
	fgSizer11->Add( m_tCcR0, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText51 = new wxStaticText( m_panel8, wxID_ANY, wxT("R1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer11->Add( m_staticText51, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR1 = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcR1->SetToolTip( wxT("Horizontal displayed character number") );
	
	fgSizer11->Add( m_tCcR1, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText52 = new wxStaticText( m_panel8, wxID_ANY, wxT("R2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText52->Wrap( -1 );
	fgSizer11->Add( m_staticText52, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR2 = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcR2->SetToolTip( wxT("Position of horizontal sync. pulse") );
	
	fgSizer11->Add( m_tCcR2, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText53 = new wxStaticText( m_panel8, wxID_ANY, wxT("R3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53->Wrap( -1 );
	fgSizer11->Add( m_staticText53, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR3 = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcR3->SetToolTip( wxT("Width of horizontal/vertical sync. pulses") );
	
	fgSizer11->Add( m_tCcR3, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	fgSizer10->Add( fgSizer11, 1, wxEXPAND, 5 );
	
	sbSizer5->Add( fgSizer10, 1, wxEXPAND, 5 );
	
	fgSizer8->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_panel8, wxID_ANY, wxT("Vertical Register") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText54 = new wxStaticText( m_panel8, wxID_ANY, wxT("R4"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText54->Wrap( -1 );
	fgSizer12->Add( m_staticText54, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR4 = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcR4->SetToolTip( wxT(" Vertical total Line character number") );
	
	fgSizer12->Add( m_tCcR4, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText55 = new wxStaticText( m_panel8, wxID_ANY, wxT("R5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText55->Wrap( -1 );
	fgSizer12->Add( m_staticText55, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR5 = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcR5->SetToolTip( wxT("Vertical raster adjust") );
	
	fgSizer12->Add( m_tCcR5, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText56 = new wxStaticText( m_panel8, wxID_ANY, wxT("R6"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText56->Wrap( -1 );
	fgSizer12->Add( m_staticText56, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR6 = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcR6->SetToolTip( wxT("Vertical displayed character number") );
	
	fgSizer12->Add( m_tCcR6, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText57 = new wxStaticText( m_panel8, wxID_ANY, wxT("R7"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	fgSizer12->Add( m_staticText57, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR7 = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcR7->SetToolTip( wxT("Position of vertical sync. pulse") );
	
	fgSizer12->Add( m_tCcR7, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText58 = new wxStaticText( m_panel8, wxID_ANY, wxT("R9"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText58->Wrap( -1 );
	fgSizer12->Add( m_staticText58, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR9 = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tCcR9->SetToolTip( wxT("Maximum raster") );
	
	fgSizer12->Add( m_tCcR9, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer6->Add( fgSizer12, 1, wxEXPAND, 5 );
	
	fgSizer8->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	bSizer21->Add( fgSizer8, 1, wxEXPAND, 5 );
	
	m_panel8->SetSizer( bSizer21 );
	m_panel8->Layout();
	bSizer21->Fit( m_panel8 );
	m_notebook2->AddPage( m_panel8, wxT("CRTC Registers and States"), true );
	m_panel9 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook2->AddPage( m_panel9, wxT("GateArray States"), false );
	
	bSizer16->Add( m_notebook2, 1, wxALL, 5 );
	
	this->SetSizer( bSizer16 );
	this->Layout();
	bSizer16->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( RegistersStates::OnIdleR ) );
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( RegistersStates::OnInitR ) );
	m_tCcR0->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR0 ), NULL, this );
	m_tCcR0->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( RegistersStates::OnTextR0 ), NULL, this );
	m_tCcR0->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( RegistersStates::OnTextEnterR0 ), NULL, this );
}

RegistersStates::~RegistersStates()
{
	// Disconnect Events
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( RegistersStates::OnIdleR ) );
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( RegistersStates::OnInitR ) );
	m_tCcR0->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR0 ), NULL, this );
	m_tCcR0->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( RegistersStates::OnTextR0 ), NULL, this );
	m_tCcR0->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( RegistersStates::OnTextEnterR0 ), NULL, this );
}
