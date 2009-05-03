///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2009)
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
	wxMenuItem* m_menu_insertDiscA;
	m_menu_insertDiscA = new wxMenuItem( m_menu_drivea, wxID_ANY, wxString( wxT("Insert Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menu_insertDiscA );
	
	wxMenuItem* m_menu_insertBlakDiskA;
	m_menu_insertBlakDiskA = new wxMenuItem( m_menu_drivea, wxID_ANY, wxString( wxT("New Blank Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menu_insertBlakDiskA );
	
	wxMenuItem* m_menu_formatDiscA;
	m_menu_formatDiscA = new wxMenuItem( m_menu_drivea, wxID_ANY, wxString( wxT("Format Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menu_formatDiscA );
	m_menu_formatDiscA->Enable( false );
	
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_menu_drivea, wxID_ANY, wxString( wxT("Edit Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menuItem5 );
	m_menuItem5->Enable( false );
	
	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( m_menu_drivea, wxID_ANY, wxString( wxT("Remove Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menuItem7 );
	m_menuItem7->Enable( false );
	
	m_menu_file->Append( -1, wxT("Drive A:"), m_menu_drivea );
	
	m_menu_driveb = new wxMenu();
	wxMenuItem* m_menuItem11;
	m_menuItem11 = new wxMenuItem( m_menu_driveb, wxID_ANY, wxString( wxT("Insert Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_driveb->Append( m_menuItem11 );
	
	wxMenuItem* m_menuItem31;
	m_menuItem31 = new wxMenuItem( m_menu_driveb, wxID_ANY, wxString( wxT("New Blank Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_driveb->Append( m_menuItem31 );
	
	wxMenuItem* m_menuItem41;
	m_menuItem41 = new wxMenuItem( m_menu_driveb, wxID_ANY, wxString( wxT("Format Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_driveb->Append( m_menuItem41 );
	m_menuItem41->Enable( false );
	
	wxMenuItem* m_menuItem51;
	m_menuItem51 = new wxMenuItem( m_menu_driveb, wxID_ANY, wxString( wxT("Edit Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_driveb->Append( m_menuItem51 );
	m_menuItem51->Enable( false );
	
	wxMenuItem* m_menuItem61;
	m_menuItem61 = new wxMenuItem( m_menu_driveb, wxID_ANY, wxString( wxT("Flip Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_driveb->Append( m_menuItem61 );
	
	wxMenuItem* m_menuItem71;
	m_menuItem71 = new wxMenuItem( m_menu_driveb, wxID_ANY, wxString( wxT("Remove Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_driveb->Append( m_menuItem71 );
	m_menuItem71->Enable( false );
	
	m_menu_file->Append( -1, wxT("Drive B:"), m_menu_driveb );
	
	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Swap Disc A and  B") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem8 );
	
	wxMenuItem* m_menuItem24;
	m_menuItem24 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Load Cartridge") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem24 );
	
	m_Tape = new wxMenu();
	wxMenuItem* m_menuItem25;
	m_menuItem25 = new wxMenuItem( m_Tape, wxID_ANY, wxString( wxT("Show Tape Controls") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem25 );
	
	wxMenuItem* m_menuItem26;
	m_menuItem26 = new wxMenuItem( m_Tape, wxID_ANY, wxString( wxT("Insert Tape Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem26 );
	
	wxMenuItem* m_menuItem27;
	m_menuItem27 = new wxMenuItem( m_Tape, wxID_ANY, wxString( wxT("Rewind Tape") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem27 );
	m_menuItem27->Enable( false );
	
	wxMenuItem* m_menuItem28;
	m_menuItem28 = new wxMenuItem( m_Tape, wxID_ANY, wxString( wxT("Remove Tape") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem28 );
	m_menuItem28->Enable( false );
	
	wxMenuItem* m_menuItem29;
	m_menuItem29 = new wxMenuItem( m_Tape, wxID_ANY, wxString( wxT("Press Play") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem29 );
	m_menuItem29->Enable( false );
	
	wxMenuItem* m_menuItem30;
	m_menuItem30 = new wxMenuItem( m_Tape, wxID_ANY, wxString( wxT("Press Record") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem30 );
	
	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( m_Tape, wxID_ANY, wxString( wxT("Flip Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem6 );
	
	m_menu_file->Append( -1, wxT("Tape"), m_Tape );
	
	wxMenuItem* m_separator1;
	m_separator1 = m_menu_file->AppendSeparator();
	
	wxMenuItem* m_mI_LoadSNA;
	m_mI_LoadSNA = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Load Snapshot") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_mI_LoadSNA );
	
	wxMenuItem* m_mI_SaveSNA;
	m_mI_SaveSNA = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Save Snapshot") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_mI_SaveSNA );
	
	wxMenuItem* m_menuItem33;
	m_menuItem33 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Update Snapshot") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem33 );
	
	wxMenuItem* m_separator2;
	m_separator2 = m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menuItem34;
	m_menuItem34 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Playback Session") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem34 );
	
	wxMenuItem* m_menuItem35;
	m_menuItem35 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Record Session") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem35 );
	
	wxMenuItem* m_separator3;
	m_separator3 = m_menu_file->AppendSeparator();
	
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
	
	wxMenuItem* m_separator4;
	m_separator4 = m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menuItem40;
	m_menuItem40 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Pokes") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem40 );
	
	wxMenuItem* m_separator5;
	m_separator5 = m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menuItem411;
	m_menuItem411 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Auto Types") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem411 );
	
	wxMenuItem* m_menuItem42;
	m_menuItem42 = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Paste") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem42 );
	
	wxMenuItem* m_separator6;
	m_separator6 = m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menu_exit;
	m_menu_exit = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menu_exit );
	
	m_menubar1->Append( m_menu_file, wxT("File") );
	
	m_menu_settings = new wxMenu();
	wxMenuItem* m_menuItem43;
	m_menuItem43 = new wxMenuItem( m_menu_settings, wxID_ANY, wxString( wxT("General") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem43 );
	
	wxMenuItem* m_menuItem44;
	m_menuItem44 = new wxMenuItem( m_menu_settings, wxID_ANY, wxString( wxT("Display") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem44 );
	
	wxMenuItem* m_menuItem45;
	m_menuItem45 = new wxMenuItem( m_menu_settings, wxID_ANY, wxString( wxT("Sound") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem45 );
	
	wxMenuItem* m_menuItem46;
	m_menuItem46 = new wxMenuItem( m_menu_settings, wxID_ANY, wxString( wxT("Memory") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem46 );
	
	wxMenuItem* m_menu_input;
	m_menu_input = new wxMenuItem( m_menu_settings, wxID_ANY, wxString( wxT("Input") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menu_input );
	
	wxMenuItem* m_menuItem48;
	m_menuItem48 = new wxMenuItem( m_menu_settings, wxID_ANY, wxString( wxT("Other") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem48 );
	
	wxMenuItem* m_separator7;
	m_separator7 = m_menu_settings->AppendSeparator();
	
	wxMenuItem* m_menuItem49;
	m_menuItem49 = new wxMenuItem( m_menu_settings, wxID_ANY, wxString( wxT("Full Screen") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem49 );
	
	wxMenuItem* m_menuItem50;
	m_menuItem50 = new wxMenuItem( m_menu_settings, wxID_ANY, wxString( wxT("Reset") ) + wxT('\t') + wxT("F5"), wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem50 );
	
	wxMenuItem* m_menuItem511;
	m_menuItem511 = new wxMenuItem( m_menu_settings, wxID_ANY, wxString( wxT("AMX Mouse") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem511 );
	
	wxMenuItem* m_menuItem52;
	m_menuItem52 = new wxMenuItem( m_menu_settings, wxID_ANY, wxString( wxT("Multiface Stop") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem52 );
	
	m_menubar1->Append( m_menu_settings, wxT("Settings") );
	
	m_menu_debug = new wxMenu();
	m_menuItem_run = new wxMenuItem( m_menu_debug, wxID_ANY, wxString( wxT("Run") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_debug->Append( m_menuItem_run );
	m_menuItem_run->Enable( false );
	
	wxMenuItem* m_menuItem_step;
	m_menuItem_step = new wxMenuItem( m_menu_debug, wxID_ANY, wxString( wxT("Step") ) + wxT('\t') + wxT("F7"), wxEmptyString, wxITEM_NORMAL );
	m_menu_debug->Append( m_menuItem_step );
	
	m_menuItem_pause = new wxMenuItem( m_menu_debug, wxID_ANY, wxString( wxT("Pause") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_debug->Append( m_menuItem_pause );
	
	wxMenuItem* m_menuItem_registers;
	m_menuItem_registers = new wxMenuItem( m_menu_debug, wxID_ANY, wxString( wxT("Register") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_debug->Append( m_menuItem_registers );
	
	wxMenuItem* m_menuItem_memory;
	m_menuItem_memory = new wxMenuItem( m_menu_debug, wxID_ANY, wxString( wxT("Memory") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_debug->Append( m_menuItem_memory );
	
	m_menubar1->Append( m_menu_debug, wxT("Debug") );
	
	m_menu4 = new wxMenu();
	wxMenuItem* m_menuItem57;
	m_menuItem57 = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Show Assembler") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem57 );
	
	m_menubar1->Append( m_menu4, wxT("Assembler") );
	
	m_menu5 = new wxMenu();
	wxMenuItem* m_menuItem491;
	m_menuItem491 = new wxMenuItem( m_menu5, wxID_ANY, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( m_menuItem491 );
	
	m_menubar1->Append( m_menu5, wxT("Help") );
	
	this->SetMenuBar( m_menubar1 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 768,540 ), wxTAB_TRAVERSAL );
	m_panel4->SetMinSize( wxSize( 768,540 ) );
	m_panel4->SetMaxSize( wxSize( 768,540 ) );
	
	bSizer12->Add( m_panel4, 1, wxALL|wxFIXED_MINSIZE, 0 );
	
	this->SetSizer( bSizer12 );
	this->Layout();
	bSizer12->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CapriceWindow::onExit1 ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( CapriceWindow::OnIdle ) );
	this->Connect( m_menu_insertDiscA->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onInsertDiscA ) );
	this->Connect( m_menuItem11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onInsertDiscB ) );
	this->Connect( m_mI_LoadSNA->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onLoadSNA ) );
	this->Connect( m_mI_SaveSNA->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onSaveSNA ) );
	this->Connect( m_menu_exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onExit2 ) );
	this->Connect( m_menuItem43->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnOpenConfigGeneral ) );
	this->Connect( m_menu_input->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onMenuInput ) );
	this->Connect( m_menuItem49->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnFullScreen ) );
	this->Connect( m_menuItem50->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnReset ) );
	this->Connect( m_menuItem_run->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnRun ) );
	this->Connect( m_menuItem_step->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnStep ) );
	this->Connect( m_menuItem_pause->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnPause ) );
	this->Connect( m_menuItem_registers->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnRegister ) );
	this->Connect( m_menuItem_memory->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnDebugMemory ) );
	this->Connect( m_menuItem57->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnShowAssembler ) );
	this->Connect( m_menuItem491->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnAbout ) );
	m_panel4->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CapriceWindow::windowKeyDown ), NULL, this );
	m_panel4->Connect( wxEVT_KEY_UP, wxKeyEventHandler( CapriceWindow::windowKeyUp ), NULL, this );
}

CapriceWindow::~CapriceWindow()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CapriceWindow::onExit1 ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( CapriceWindow::OnIdle ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onInsertDiscA ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onInsertDiscB ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onLoadSNA ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onSaveSNA ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onExit2 ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnOpenConfigGeneral ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::onMenuInput ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnFullScreen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnReset ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnRun ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnStep ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnPause ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnRegister ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnDebugMemory ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnShowAssembler ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CapriceWindow::OnAbout ) );
	m_panel4->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CapriceWindow::windowKeyDown ), NULL, this );
	m_panel4->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( CapriceWindow::windowKeyUp ), NULL, this );
}

InputSettings::InputSettings( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook4 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel8 = new wxPanel( m_notebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText63 = new wxStaticText( m_panel8, wxID_ANY, wxT("Preset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText63->Wrap( -1 );
	gSizer1->Add( m_staticText63, 0, 0, 5 );
	
	m_comboBox1 = new wxComboBox( m_panel8, wxID_ANY, wxT("FR-FR"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	gSizer1->Add( m_comboBox1, 0, 0, 5 );
	
	m_staticText64 = new wxStaticText( m_panel8, wxID_ANY, wxT("CPC Layout"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText64->Wrap( -1 );
	gSizer1->Add( m_staticText64, 0, 0, 5 );
	
	wxString m_choice1Choices[] = { wxT("English 6128"), wxT("French 6128"), wxT("Spanish 6128"), wxT("English 464"), wxT("French 464"), wxT("Spanish 464") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
	m_choice1 = new wxChoice( m_panel8, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0 );
	m_choice1->SetSelection( 1 );
	gSizer1->Add( m_choice1, 0, 0, 5 );
	
	bSizer26->Add( gSizer1, 0, wxEXPAND, 5 );
	
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	k_TAB = new wxButton( m_panel8, CPC_TAB, wxT("TAB"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	k_TAB1 = new wxButton( m_panel8, CPC_A, wxT("A"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB1, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB2 = new wxButton( m_panel8, CPC_Z, wxT("Z"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB2, wxGBPosition( 1, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB3 = new wxButton( m_panel8, CPC_E, wxT("E"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB3, wxGBPosition( 1, 4 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB4 = new wxButton( m_panel8, CPC_R, wxT("R"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB4, wxGBPosition( 1, 5 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB5 = new wxButton( m_panel8, CPC_T, wxT("T"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB5, wxGBPosition( 1, 6 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB6 = new wxButton( m_panel8, CPC_Y, wxT("Y"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB6, wxGBPosition( 1, 7 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB7 = new wxButton( m_panel8, CPC_U, wxT("U"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB7, wxGBPosition( 1, 8 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB8 = new wxButton( m_panel8, CPC_I, wxT("I"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB8, wxGBPosition( 1, 9 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB9 = new wxButton( m_panel8, CPC_O, wxT("O"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB9, wxGBPosition( 1, 10 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB10 = new wxButton( m_panel8, CPC_P, wxT("P"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB10, wxGBPosition( 1, 11 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB11 = new wxButton( m_panel8, CPC_PIPE, wxT("|^"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB11, wxGBPosition( 1, 12 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB12 = new wxButton( m_panel8, CPC_LESS, wxT("<*"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB12, wxGBPosition( 1, 13 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB13 = new wxButton( m_panel8, CPC_RETURN, wxT("↵"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB13, wxGBPosition( 1, 14 ), wxGBSpan( 2, 1 ), wxEXPAND, 5 );
	
	k_TAB14 = new wxButton( m_panel8, CPC_F4, wxT("F4"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB14, wxGBPosition( 1, 15 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB15 = new wxButton( m_panel8, CPC_F5, wxT("F5"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB15, wxGBPosition( 1, 16 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB16 = new wxButton( m_panel8, CPC_F6, wxT("F6"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB16, wxGBPosition( 1, 17 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS = new wxButton( m_panel8, CPC_CAPSLOCK, wxT("CAPS"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS, wxGBPosition( 2, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	k_CAPS1 = new wxButton( m_panel8, CPC_Q, wxT("Q"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS1, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS2 = new wxButton( m_panel8, CPC_S, wxT("S"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS2, wxGBPosition( 2, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS3 = new wxButton( m_panel8, CPC_D, wxT("D"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS3, wxGBPosition( 2, 4 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS4 = new wxButton( m_panel8, CPC_F, wxT("F"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS4, wxGBPosition( 2, 5 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS5 = new wxButton( m_panel8, CPC_G, wxT("G"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS5, wxGBPosition( 2, 6 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS6 = new wxButton( m_panel8, CPC_H, wxT("H"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS6, wxGBPosition( 2, 7 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS7 = new wxButton( m_panel8, CPC_J, wxT("J"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS7, wxGBPosition( 2, 8 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS8 = new wxButton( m_panel8, CPC_K, wxT("K"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS8, wxGBPosition( 2, 9 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS9 = new wxButton( m_panel8, CPC_L, wxT("L"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS9, wxGBPosition( 2, 10 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS10 = new wxButton( m_panel8, CPC_M, wxT("M"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS10, wxGBPosition( 2, 11 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS11 = new wxButton( m_panel8, CPC_PERCENT, wxT("%ù"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS11, wxGBPosition( 2, 12 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS12 = new wxButton( m_panel8, CPC_GREATER, wxT(">#"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS12, wxGBPosition( 2, 13 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS13 = new wxButton( m_panel8, CPC_F1, wxT("F1"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS13, wxGBPosition( 2, 15 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS14 = new wxButton( m_panel8, CPC_F2, wxT("F2"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS14, wxGBPosition( 2, 16 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS15 = new wxButton( m_panel8, CPC_F3, wxT("F3"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS15, wxGBPosition( 2, 17 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT = new wxButton( m_panel8, CPC_LSHIFT, wxT("SHIFT"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	k_LSHIFT1 = new wxButton( m_panel8, CPC_W, wxT("W"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT1, wxGBPosition( 3, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT2 = new wxButton( m_panel8, CPC_X, wxT("X"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT2, wxGBPosition( 3, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT3 = new wxButton( m_panel8, CPC_C, wxT("C"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT3, wxGBPosition( 3, 4 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT4 = new wxButton( m_panel8, CPC_V, wxT("V"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT4, wxGBPosition( 3, 5 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT5 = new wxButton( m_panel8, CPC_B, wxT("B"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT5, wxGBPosition( 3, 6 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT6 = new wxButton( m_panel8, CPC_N, wxT("N"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT6, wxGBPosition( 3, 7 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT7 = new wxButton( m_panel8, CPC_QUESTION, wxT("?,"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT7, wxGBPosition( 3, 8 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT8 = new wxButton( m_panel8, CPC_PERIOD, wxT(".;"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT8, wxGBPosition( 3, 9 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT9 = new wxButton( m_panel8, CPC_SLASH, wxT("/:"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT9, wxGBPosition( 3, 10 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT10 = new wxButton( m_panel8, CPC_PLUS, wxT("+="), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT10, wxGBPosition( 3, 11 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT11 = new wxButton( m_panel8, CPC_AT, wxT("@\\$"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT11, wxGBPosition( 3, 12 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT12 = new wxButton( m_panel8, CPC_RSHIFT, wxT("SHIFT"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT12, wxGBPosition( 3, 13 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	k_LSHIFT13 = new wxButton( m_panel8, CPC_F0, wxT("F0"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT13, wxGBPosition( 3, 15 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT14 = new wxButton( m_panel8, CPC_CUR_UP, wxT("↑"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT14, wxGBPosition( 3, 16 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT15 = new wxButton( m_panel8, CPC_FPERIOD, wxT("."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT15, wxGBPosition( 3, 17 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CONTROL = new wxButton( m_panel8, CPC_CONTROL, wxT("CONTROL"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL, wxGBPosition( 4, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	k_CONTROL1 = new wxButton( m_panel8, CPC_COPY, wxT("COPY"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL1, wxGBPosition( 4, 2 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	k_CONTROL2 = new wxButton( m_panel8, CPC_SPACE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL2, wxGBPosition( 4, 4 ), wxGBSpan( 1, 8 ), wxEXPAND, 5 );
	
	k_CONTROL3 = new wxButton( m_panel8, CPC_ENTER, wxT("ENTER"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL3, wxGBPosition( 4, 12 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );
	
	k_CONTROL4 = new wxButton( m_panel8, CPC_CUR_LEFT, wxT("←"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL4, wxGBPosition( 4, 15 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CONTROL5 = new wxButton( m_panel8, CPC_CUR_DOWN, wxT("↓"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL5, wxGBPosition( 4, 16 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CONTROL6 = new wxButton( m_panel8, CPC_CUR_RIGHT, wxT("→"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL6, wxGBPosition( 4, 17 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC = new wxButton( m_panel8, CPC_ESC, wxT("ESC"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC1 = new wxButton( m_panel8, CPC_1, wxT("1&&"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC1, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC2 = new wxButton( m_panel8, CPC_2, wxT("2é"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC2, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC3 = new wxButton( m_panel8, CPC_3, wxT("3\""), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC3, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC4 = new wxButton( m_panel8, CPC_4, wxT("4'"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC4, wxGBPosition( 0, 4 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC5 = new wxButton( m_panel8, CPC_5, wxT("5("), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC5, wxGBPosition( 0, 5 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC6 = new wxButton( m_panel8, CPC_6, wxT("6]"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC6, wxGBPosition( 0, 6 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC7 = new wxButton( m_panel8, CPC_7, wxT("7è"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC7, wxGBPosition( 0, 7 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC8 = new wxButton( m_panel8, CPC_8, wxT("8!"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC8, wxGBPosition( 0, 8 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC9 = new wxButton( m_panel8, CPC_9, wxT("9ç"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC9, wxGBPosition( 0, 9 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC10 = new wxButton( m_panel8, CPC_0, wxT("0à"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC10, wxGBPosition( 0, 10 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC11 = new wxButton( m_panel8, CPC_LBRACKET, wxT("[)"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC11, wxGBPosition( 0, 11 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC12 = new wxButton( m_panel8, CPC_UNDERSCORE, wxT("_-"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC12, wxGBPosition( 0, 12 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC13 = new wxButton( m_panel8, CPC_CLR, wxT("CLR"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC13, wxGBPosition( 0, 13 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC14 = new wxButton( m_panel8, CPC_DEL, wxT("DEL"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC14, wxGBPosition( 0, 14 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC15 = new wxButton( m_panel8, CPC_F7, wxT("F7"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC15, wxGBPosition( 0, 15 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC16 = new wxButton( m_panel8, CPC_F8, wxT("F8"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC16, wxGBPosition( 0, 16 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC17 = new wxButton( m_panel8, CPC_F9, wxT("F9"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC17, wxGBPosition( 0, 17 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	bSizer26->Add( gbSizer1, 0, wxEXPAND, 5 );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText65 = new wxStaticText( m_panel8, wxID_ANY, wxT("Settings for key :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText65->Wrap( -1 );
	gSizer2->Add( m_staticText65, 0, 0, 5 );
	
	m_keyName = new wxStaticText( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_keyName->Wrap( -1 );
	gSizer2->Add( m_keyName, 0, 0, 5 );
	
	m_staticText67 = new wxStaticText( m_panel8, wxID_ANY, wxT("PC key to use :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText67->Wrap( -1 );
	gSizer2->Add( m_staticText67, 0, 0, 5 );
	
	m_regularKey = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB|wxTE_READONLY|wxWANTS_CHARS );
	gSizer2->Add( m_regularKey, 0, 0, 5 );
	
	m_staticText68 = new wxStaticText( m_panel8, wxID_ANY, wxT("PC key to use for shift+key :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText68->Wrap( -1 );
	gSizer2->Add( m_staticText68, 0, 0, 5 );
	
	m_shiftKey = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB|wxTE_READONLY|wxWANTS_CHARS );
	gSizer2->Add( m_shiftKey, 0, 0, 5 );
	
	m_staticText69 = new wxStaticText( m_panel8, wxID_ANY, wxT("PC key to use for control+key :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText69->Wrap( -1 );
	gSizer2->Add( m_staticText69, 0, 0, 5 );
	
	m_ctrlKey = new wxTextCtrl( m_panel8, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB|wxTE_READONLY|wxWANTS_CHARS );
	gSizer2->Add( m_ctrlKey, 0, 0, 5 );
	
	bSizer26->Add( gSizer2, 1, wxEXPAND, 0 );
	
	m_panel8->SetSizer( bSizer26 );
	m_panel8->Layout();
	bSizer26->Fit( m_panel8 );
	m_notebook4->AddPage( m_panel8, wxT("Keyboard"), false );
	
	bSizer24->Add( m_notebook4, 0, 0, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1Save = new wxButton( this, wxID_SAVE );
	m_sdbSizer1->AddButton( m_sdbSizer1Save );
	m_sdbSizer1Apply = new wxButton( this, wxID_APPLY );
	m_sdbSizer1->AddButton( m_sdbSizer1Apply );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	bSizer24->Add( m_sdbSizer1, 0, wxALL, 5 );
	
	this->SetSizer( bSizer24 );
	this->Layout();
	bSizer24->Fit( this );
	
	// Connect Events
	k_TAB->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB16->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC16->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC17->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	m_regularKey->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( InputSettings::onKeyPress ), NULL, this );
	m_shiftKey->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( InputSettings::onKeyPress ), NULL, this );
	m_ctrlKey->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( InputSettings::onKeyPress ), NULL, this );
	m_sdbSizer1Apply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::applySettings ), NULL, this );
	m_sdbSizer1Save->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onSave ), NULL, this );
}

InputSettings::~InputSettings()
{
	// Disconnect Events
	k_TAB->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB15->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_TAB16->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CAPS15->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_LSHIFT15->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_CONTROL6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC15->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC16->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	k_ESC17->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onKeyClick ), NULL, this );
	m_regularKey->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( InputSettings::onKeyPress ), NULL, this );
	m_shiftKey->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( InputSettings::onKeyPress ), NULL, this );
	m_ctrlKey->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( InputSettings::onKeyPress ), NULL, this );
	m_sdbSizer1Apply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::applySettings ), NULL, this );
	m_sdbSizer1Save->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( InputSettings::onSave ), NULL, this );
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
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("Credits :\nWritten by :\nUlrich Doewich\nRamlaid/Arkos <contact@ramlaid.com>\nRomain Giot  - Krusty/Benediction <krusty@cpcscene.com>\nPulkoMandy/Shinra <pulkomandy@gmail.com>\nContributors :\nCloudStrife/Shinra <cloudstrife@cpcscene.com>\nAnd a lot of other contributors\nStSound - LibDSK - Lib765 - SDL - wxWidgets"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer11->Add( m_staticText10, 0, wxALL|wxEXPAND, 5 );
	
	bSizer8->Add( bSizer11, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer8 );
	this->Layout();
	bSizer8->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CapriceAbout::OnAboutClose ) );
}

CapriceAbout::~CapriceAbout()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CapriceAbout::OnAboutClose ) );
}

RegistersStates::RegistersStates( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_nb_Register = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel4 = new wxPanel( m_nb_Register, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
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
	
	m_tCrA = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrA, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_tCrAx = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_tCrAx, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText13 = new wxStaticText( m_panel4, wxID_ANY, wxT("F"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer3->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCrF = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
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
	
	m_cB_SFlags = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_SFlags, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	m_cB_ZFlags = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_ZFlags, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_HFlags = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_HFlags, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_PFlags = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_PFlags, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_NFlags = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_NFlags, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_CFlags = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_CFlags, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_staticText42 = new wxStaticText( m_panel4, wxID_ANY, wxT("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer6->Add( m_staticText42, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cB_SFlagsShadow = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_SFlagsShadow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_ZFlagsShadow = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_ZFlagsShadow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_HFlagsShadow = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_HFlagsShadow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_PFlagsShadow = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_PFlagsShadow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_NFlagsShadow = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_NFlagsShadow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_CFlagsShadow = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_CFlagsShadow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
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
	m_nb_Register->AddPage( m_panel4, wxT("Z80 Registers"), true );
	m_panel8 = new wxPanel( m_nb_Register, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
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
	
	bSizer21->Add( fgSizer8, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( m_panel8, wxID_ANY, wxT("Timing Checking") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer121;
	fgSizer121 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer121->SetFlexibleDirection( wxBOTH );
	fgSizer121->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText59 = new wxStaticText( m_panel8, wxID_ANY, wxT("Horizontal Line Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText59->Wrap( -1 );
	fgSizer121->Add( m_staticText59, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sT_HLineLength = new wxStaticText( m_panel8, wxID_ANY, wxT("xxxxx us"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sT_HLineLength->Wrap( -1 );
	fgSizer121->Add( m_sT_HLineLength, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cB_HLineLengthIsOk = new wxCheckBox( m_panel8, wxID_ANY, wxT("Timing is Ok !"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer121->Add( m_cB_HLineLengthIsOk, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText61 = new wxStaticText( m_panel8, wxID_ANY, wxT("Vertical Frame Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	fgSizer121->Add( m_staticText61, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sT_VFrameLength = new wxStaticText( m_panel8, wxID_ANY, wxT("xxxxx us"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sT_VFrameLength->Wrap( -1 );
	fgSizer121->Add( m_sT_VFrameLength, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cB_VFrameLengthIsOk = new wxCheckBox( m_panel8, wxID_ANY, wxT("Timing is Ok !"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer121->Add( m_cB_VFrameLengthIsOk, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	sbSizer8->Add( fgSizer121, 1, wxEXPAND, 5 );
	
	bSizer20->Add( sbSizer8, 1, wxEXPAND, 5 );
	
	bSizer21->Add( bSizer20, 1, wxEXPAND, 5 );
	
	m_panel8->SetSizer( bSizer21 );
	m_panel8->Layout();
	bSizer21->Fit( m_panel8 );
	m_nb_Register->AddPage( m_panel8, wxT("CRTC Registers and States"), false );
	m_panel9 = new wxPanel( m_nb_Register, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_nb_Register->AddPage( m_panel9, wxT("GateArray States"), false );
	
	bSizer16->Add( m_nb_Register, 1, wxALL, 5 );
	
	this->SetSizer( bSizer16 );
	this->Layout();
	bSizer16->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( RegistersStates::OnCloseR ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( RegistersStates::OnIdleR ) );
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( RegistersStates::OnInitR ) );
	m_tCrA->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusA ), NULL, this );
	m_tCrA->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusA ), NULL, this );
	m_tCrAx->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusAx ), NULL, this );
	m_tCrAx->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusAx ), NULL, this );
	m_tCrF->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusF ), NULL, this );
	m_tCrF->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusF ), NULL, this );
	m_tCrFx->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusFx ), NULL, this );
	m_tCrFx->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusFx ), NULL, this );
	m_tCrB->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusB ), NULL, this );
	m_tCrB->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusB ), NULL, this );
	m_tCrBx->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusBx ), NULL, this );
	m_tCrBx->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusBx ), NULL, this );
	m_tCrC->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusC ), NULL, this );
	m_tCrC->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusC ), NULL, this );
	m_tCrCx->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusCx ), NULL, this );
	m_tCrCx->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusCx ), NULL, this );
	m_tCrD->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusD ), NULL, this );
	m_tCrD->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusD ), NULL, this );
	m_tCrDx->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusDx ), NULL, this );
	m_tCrDx->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusDx ), NULL, this );
	m_tCrE->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusE ), NULL, this );
	m_tCrE->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusE ), NULL, this );
	m_tCrEx->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusEx ), NULL, this );
	m_tCrEx->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusEx ), NULL, this );
	m_tCrH->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusH ), NULL, this );
	m_tCrH->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusH ), NULL, this );
	m_tCrHx->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusHx ), NULL, this );
	m_tCrHx->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusHx ), NULL, this );
	m_tCrL->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusL ), NULL, this );
	m_tCrL->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusL ), NULL, this );
	m_tCrLx->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusLx ), NULL, this );
	m_tCrLx->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusLx ), NULL, this );
	m_tCrI->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusI ), NULL, this );
	m_tCrI->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusI ), NULL, this );
	m_tCrR->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR ), NULL, this );
	m_tCrR->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR ), NULL, this );
	m_tCcHCC->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusHCC ), NULL, this );
	m_tCcHCC->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusHCC ), NULL, this );
	m_tCcVCC->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusVCC ), NULL, this );
	m_tCcVCC->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusVCC ), NULL, this );
	m_tCcVLA->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusVLA ), NULL, this );
	m_tCcVLA->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusVLA ), NULL, this );
	m_tCcVLC->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusVLC ), NULL, this );
	m_tCcVLC->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusVLC ), NULL, this );
	m_tCcR0->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR0 ), NULL, this );
	m_tCcR0->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR0 ), NULL, this );
	m_tCcR0->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( RegistersStates::OnTextR0 ), NULL, this );
	m_tCcR0->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( RegistersStates::OnTextEnterR0 ), NULL, this );
	m_tCcR1->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR1 ), NULL, this );
	m_tCcR1->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR1 ), NULL, this );
	m_tCcR2->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR2 ), NULL, this );
	m_tCcR2->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR2 ), NULL, this );
	m_tCcR3->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR3 ), NULL, this );
	m_tCcR3->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR3 ), NULL, this );
	m_tCcR4->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR4 ), NULL, this );
	m_tCcR4->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR4 ), NULL, this );
	m_tCcR5->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR5 ), NULL, this );
	m_tCcR5->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR5 ), NULL, this );
	m_tCcR6->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR6 ), NULL, this );
	m_tCcR6->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR6 ), NULL, this );
	m_tCcR7->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR7 ), NULL, this );
	m_tCcR7->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR7 ), NULL, this );
	m_tCcR9->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR9 ), NULL, this );
	m_tCcR9->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR9 ), NULL, this );
}

RegistersStates::~RegistersStates()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( RegistersStates::OnCloseR ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( RegistersStates::OnIdleR ) );
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( RegistersStates::OnInitR ) );
	m_tCrA->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusA ), NULL, this );
	m_tCrA->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusA ), NULL, this );
	m_tCrAx->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusAx ), NULL, this );
	m_tCrAx->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusAx ), NULL, this );
	m_tCrF->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusF ), NULL, this );
	m_tCrF->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusF ), NULL, this );
	m_tCrFx->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusFx ), NULL, this );
	m_tCrFx->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusFx ), NULL, this );
	m_tCrB->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusB ), NULL, this );
	m_tCrB->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusB ), NULL, this );
	m_tCrBx->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusBx ), NULL, this );
	m_tCrBx->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusBx ), NULL, this );
	m_tCrC->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusC ), NULL, this );
	m_tCrC->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusC ), NULL, this );
	m_tCrCx->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusCx ), NULL, this );
	m_tCrCx->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusCx ), NULL, this );
	m_tCrD->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusD ), NULL, this );
	m_tCrD->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusD ), NULL, this );
	m_tCrDx->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusDx ), NULL, this );
	m_tCrDx->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusDx ), NULL, this );
	m_tCrE->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusE ), NULL, this );
	m_tCrE->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusE ), NULL, this );
	m_tCrEx->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusEx ), NULL, this );
	m_tCrEx->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusEx ), NULL, this );
	m_tCrH->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusH ), NULL, this );
	m_tCrH->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusH ), NULL, this );
	m_tCrHx->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusHx ), NULL, this );
	m_tCrHx->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusHx ), NULL, this );
	m_tCrL->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusL ), NULL, this );
	m_tCrL->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusL ), NULL, this );
	m_tCrLx->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusLx ), NULL, this );
	m_tCrLx->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusLx ), NULL, this );
	m_tCrI->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusI ), NULL, this );
	m_tCrI->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusI ), NULL, this );
	m_tCrR->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR ), NULL, this );
	m_tCrR->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR ), NULL, this );
	m_tCcHCC->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusHCC ), NULL, this );
	m_tCcHCC->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusHCC ), NULL, this );
	m_tCcVCC->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusVCC ), NULL, this );
	m_tCcVCC->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusVCC ), NULL, this );
	m_tCcVLA->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusVLA ), NULL, this );
	m_tCcVLA->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusVLA ), NULL, this );
	m_tCcVLC->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusVLC ), NULL, this );
	m_tCcVLC->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusVLC ), NULL, this );
	m_tCcR0->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR0 ), NULL, this );
	m_tCcR0->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR0 ), NULL, this );
	m_tCcR0->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( RegistersStates::OnTextR0 ), NULL, this );
	m_tCcR0->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( RegistersStates::OnTextEnterR0 ), NULL, this );
	m_tCcR1->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR1 ), NULL, this );
	m_tCcR1->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR1 ), NULL, this );
	m_tCcR2->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR2 ), NULL, this );
	m_tCcR2->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR2 ), NULL, this );
	m_tCcR3->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR3 ), NULL, this );
	m_tCcR3->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR3 ), NULL, this );
	m_tCcR4->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR4 ), NULL, this );
	m_tCcR4->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR4 ), NULL, this );
	m_tCcR5->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR5 ), NULL, this );
	m_tCcR5->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR5 ), NULL, this );
	m_tCcR6->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR6 ), NULL, this );
	m_tCcR6->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR6 ), NULL, this );
	m_tCcR7->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR7 ), NULL, this );
	m_tCcR7->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR7 ), NULL, this );
	m_tCcR9->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( RegistersStates::OnKillFocusR9 ), NULL, this );
	m_tCcR9->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( RegistersStates::OnSetFocusR9 ), NULL, this );
}

Memory::Memory( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( Memory::m_splitter1OnIdle ), NULL, this );
	m_panel10 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_checkList1Choices;
	m_checkList1 = new wxCheckListBox( m_panel10, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_checkList1Choices, 0 );
	bSizer29->Add( m_checkList1, 0, wxALL|wxEXPAND, 5 );
	
	m_panel10->SetSizer( bSizer29 );
	m_panel10->Layout();
	bSizer29->Fit( m_panel10 );
	m_panel9 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_grid1 = new wxGrid( m_panel9, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	
	// Grid
	m_grid1->CreateGrid( 32, 16 );
	m_grid1->EnableEditing( false );
	m_grid1->EnableGridLines( true );
	m_grid1->EnableDragGridSize( false );
	m_grid1->SetMargins( 0, 0 );
	
	// Columns
	m_grid1->SetColSize( 0, 40 );
	m_grid1->SetColSize( 1, 40 );
	m_grid1->SetColSize( 2, 40 );
	m_grid1->SetColSize( 3, 40 );
	m_grid1->SetColSize( 4, 40 );
	m_grid1->SetColSize( 5, 40 );
	m_grid1->SetColSize( 6, 40 );
	m_grid1->SetColSize( 7, 40 );
	m_grid1->SetColSize( 8, 40 );
	m_grid1->SetColSize( 9, 40 );
	m_grid1->SetColSize( 10, 40 );
	m_grid1->SetColSize( 11, 40 );
	m_grid1->SetColSize( 12, 40 );
	m_grid1->SetColSize( 13, 40 );
	m_grid1->SetColSize( 14, 40 );
	m_grid1->SetColSize( 15, 40 );
	m_grid1->EnableDragColMove( false );
	m_grid1->EnableDragColSize( false );
	m_grid1->SetColLabelSize( 20 );
	m_grid1->SetColLabelValue( 0, wxT("0") );
	m_grid1->SetColLabelValue( 1, wxT("1") );
	m_grid1->SetColLabelValue( 2, wxT("2") );
	m_grid1->SetColLabelValue( 3, wxT("3") );
	m_grid1->SetColLabelValue( 4, wxT("4") );
	m_grid1->SetColLabelValue( 5, wxT("5") );
	m_grid1->SetColLabelValue( 6, wxT("6") );
	m_grid1->SetColLabelValue( 7, wxT("7") );
	m_grid1->SetColLabelValue( 8, wxT("8") );
	m_grid1->SetColLabelValue( 9, wxT("9") );
	m_grid1->SetColLabelValue( 10, wxT("A") );
	m_grid1->SetColLabelValue( 11, wxT("B") );
	m_grid1->SetColLabelValue( 12, wxT("C") );
	m_grid1->SetColLabelValue( 13, wxT("D") );
	m_grid1->SetColLabelValue( 14, wxT("E") );
	m_grid1->SetColLabelValue( 15, wxT("F") );
	m_grid1->SetColLabelAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	
	// Rows
	m_grid1->EnableDragRowSize( false );
	m_grid1->SetRowLabelSize( 60 );
	m_grid1->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	
	// Label Appearance
	m_grid1->SetLabelFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxEmptyString ) );
	
	// Cell Defaults
	m_grid1->SetDefaultCellFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxEmptyString ) );
	m_grid1->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer21->Add( m_grid1, 0, wxBOTTOM|wxLEFT|wxTOP, 5 );
	
	m_scrollBar1 = new wxScrollBar( m_panel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
	bSizer21->Add( m_scrollBar1, 0, wxBOTTOM|wxEXPAND|wxRIGHT|wxTOP, 5 );
	
	m_panel9->SetSizer( bSizer21 );
	m_panel9->Layout();
	bSizer21->Fit( m_panel9 );
	m_splitter1->SplitHorizontally( m_panel10, m_panel9, 90 );
	bSizer26->Add( m_splitter1, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer26 );
	this->Layout();
	bSizer26->Fit( this );
	
	// Connect Events
	m_scrollBar1->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
}

Memory::~Memory()
{
	// Disconnect Events
	m_scrollBar1->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	m_scrollBar1->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
}

ConfigDialog::ConfigDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook3 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel7 = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText62 = new wxStaticText( m_panel7, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText62->Wrap( -1 );
	bSizer23->Add( m_staticText62, 0, wxALL, 5 );
	
	m_textCtrl44 = new wxTextCtrl( m_panel7, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( m_textCtrl44, 0, wxALL, 5 );
	
	m_panel7->SetSizer( bSizer23 );
	m_panel7->Layout();
	bSizer23->Fit( m_panel7 );
	m_notebook3->AddPage( m_panel7, wxT("a page"), false );
	
	bSizer22->Add( m_notebook3, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer22 );
	this->Layout();
	bSizer22->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ConfigDialog::OnCloseC ) );
}

ConfigDialog::~ConfigDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ConfigDialog::OnCloseC ) );
}
