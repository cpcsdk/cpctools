///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  2 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/imagpng.h>

#include "CapriceUI.h"

///////////////////////////////////////////////////////////////////////////

EmulatorWindow::EmulatorWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxFrame( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	m_menubar1 = new wxMenuBar( 0 );
	m_menu_file = new wxMenu();
	m_menu_drivea = new wxMenu();
	wxMenuItem* m_menu_insertDiscA;
	m_menu_insertDiscA = new wxMenuItem( m_menu_drivea, 9999, wxString( wxT("Insert Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menu_insertDiscA );
	
	wxMenuItem* m_menu_insertBlakDiskA;
	m_menu_insertBlakDiskA = new wxMenuItem( m_menu_drivea, 9998, wxString( wxT("New Blank Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menu_insertBlakDiskA );
	
	wxMenuItem* m_menu_formatDiscA;
	m_menu_formatDiscA = new wxMenuItem( m_menu_drivea, 9997, wxString( wxT("Format Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menu_formatDiscA );
	m_menu_formatDiscA->Enable( false );
	
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_menu_drivea, 9996, wxString( wxT("Edit Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menuItem5 );
	
	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( m_menu_drivea, 9995, wxString( wxT("Remove Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_drivea->Append( m_menuItem7 );
	m_menuItem7->Enable( false );
	
	m_menu_file->Append( -1, wxT("Drive A:"), m_menu_drivea );
	
	m_menu_driveb = new wxMenu();
	wxMenuItem* m_menuItem11;
	m_menuItem11 = new wxMenuItem( m_menu_driveb, 9994, wxString( wxT("Insert Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_driveb->Append( m_menuItem11 );
	
	wxMenuItem* m_menuItem31;
	m_menuItem31 = new wxMenuItem( m_menu_driveb, 9993, wxString( wxT("New Blank Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_driveb->Append( m_menuItem31 );
	
	wxMenuItem* m_menuItem41;
	m_menuItem41 = new wxMenuItem( m_menu_driveb, 9992, wxString( wxT("Format Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_driveb->Append( m_menuItem41 );
	m_menuItem41->Enable( false );
	
	wxMenuItem* m_menuItem51;
	m_menuItem51 = new wxMenuItem( m_menu_driveb, 9991, wxString( wxT("Edit Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_driveb->Append( m_menuItem51 );
	m_menuItem51->Enable( false );
	
	wxMenuItem* m_menuItem61;
	m_menuItem61 = new wxMenuItem( m_menu_driveb, 9990, wxString( wxT("Flip Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_driveb->Append( m_menuItem61 );
	
	wxMenuItem* m_menuItem71;
	m_menuItem71 = new wxMenuItem( m_menu_driveb, 9989, wxString( wxT("Remove Disc") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_driveb->Append( m_menuItem71 );
	m_menuItem71->Enable( false );
	
	m_menu_file->Append( -1, wxT("Drive B:"), m_menu_driveb );
	
	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( m_menu_file, 9988, wxString( wxT("Swap Disc A and  B") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem8 );
	
	wxMenuItem* m_menuItem24;
	m_menuItem24 = new wxMenuItem( m_menu_file, 9987, wxString( wxT("Load Cartridge") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem24 );
	
	m_Tape = new wxMenu();
	wxMenuItem* m_menuItem25;
	m_menuItem25 = new wxMenuItem( m_Tape, 9986, wxString( wxT("Show Tape Controls") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem25 );
	
	wxMenuItem* m_menuItem26;
	m_menuItem26 = new wxMenuItem( m_Tape, 9985, wxString( wxT("Insert Tape Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem26 );
	
	wxMenuItem* m_menuItem27;
	m_menuItem27 = new wxMenuItem( m_Tape, 9984, wxString( wxT("Rewind Tape") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem27 );
	m_menuItem27->Enable( false );
	
	wxMenuItem* m_menuItem28;
	m_menuItem28 = new wxMenuItem( m_Tape, 9983, wxString( wxT("Remove Tape") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem28 );
	m_menuItem28->Enable( false );
	
	wxMenuItem* m_menuItem29;
	m_menuItem29 = new wxMenuItem( m_Tape, 9982, wxString( wxT("Press Play") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem29 );
	
	wxMenuItem* m_menuItem30;
	m_menuItem30 = new wxMenuItem( m_Tape, 9981, wxString( wxT("Press Record") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem30 );
	
	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( m_Tape, 9980, wxString( wxT("Flip Tape") ) , wxEmptyString, wxITEM_NORMAL );
	m_Tape->Append( m_menuItem6 );
	
	m_menu_file->Append( -1, wxT("Tape"), m_Tape );
	
	wxMenuItem* m_separator1;
	m_separator1 = m_menu_file->AppendSeparator();
	
	wxMenuItem* m_mI_LoadSNA;
	m_mI_LoadSNA = new wxMenuItem( m_menu_file, 9979, wxString( wxT("Load Snapshot") ) + wxT('\t') + wxT("F3"), wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_mI_LoadSNA );
	
	wxMenuItem* m_ml_ReloadSNA;
	m_ml_ReloadSNA = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Reload Snapshot") ) + wxT('\t') + wxT("CTRL+F3"), wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_ml_ReloadSNA );
	
	wxMenuItem* m_mI_SaveSNA;
	m_mI_SaveSNA = new wxMenuItem( m_menu_file, 9978, wxString( wxT("Save Snapshot") ) + wxT('\t') + wxT("F2"), wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_mI_SaveSNA );
	
	wxMenuItem* m_ml_UpdateSNA;
	m_ml_UpdateSNA = new wxMenuItem( m_menu_file, wxID_ANY, wxString( wxT("Update Snapshot") ) + wxT('\t') + wxT("CTRL+F2"), wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_ml_UpdateSNA );
	
	wxMenuItem* m_separator2;
	m_separator2 = m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menuItem34;
	m_menuItem34 = new wxMenuItem( m_menu_file, 9976, wxString( wxT("Playback Session") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem34 );
	
	wxMenuItem* m_menuItem35;
	m_menuItem35 = new wxMenuItem( m_menu_file, 9975, wxString( wxT("Record Session") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem35 );
	
	wxMenuItem* m_separator3;
	m_separator3 = m_menu_file->AppendSeparator();
	
	wxMenuItem* m_mI_SaveSCR;
	m_mI_SaveSCR = new wxMenuItem( m_menu_file, 9974, wxString( wxT("Save Screenshot") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_mI_SaveSCR );
	
	wxMenuItem* m_menu_insertDiscA1;
	m_menu_insertDiscA1 = new wxMenuItem( m_menu_file, 9973, wxString( wxT("Insert Disc Image") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menu_insertDiscA1 );
	
	wxMenuItem* m_menuItem37;
	m_menuItem37 = new wxMenuItem( m_menu_file, 9972, wxString( wxT("Record AVI") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem37 );
	
	wxMenuItem* m_menuItem38;
	m_menuItem38 = new wxMenuItem( m_menu_file, 9971, wxString( wxT("Record WAV") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem38 );
	
	wxMenuItem* m_menuItem39;
	m_menuItem39 = new wxMenuItem( m_menu_file, 9970, wxString( wxT("Record YM") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem39 );
	
	wxMenuItem* m_separator4;
	m_separator4 = m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menuItem40;
	m_menuItem40 = new wxMenuItem( m_menu_file, 9969, wxString( wxT("Pokes") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem40 );
	
	wxMenuItem* m_separator5;
	m_separator5 = m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menuItem411;
	m_menuItem411 = new wxMenuItem( m_menu_file, 9968, wxString( wxT("Auto Types") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem411 );
	
	wxMenuItem* m_menuItem42;
	m_menuItem42 = new wxMenuItem( m_menu_file, 9967, wxString( wxT("Paste") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem42 );
	
	wxMenuItem* m_separator6;
	m_separator6 = m_menu_file->AppendSeparator();
	
	wxMenuItem* m_menu_exit;
	m_menu_exit = new wxMenuItem( m_menu_file, 9966, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menu_exit );
	
	m_menubar1->Append( m_menu_file, wxT("File") ); 
	
	m_menu_settings = new wxMenu();
	wxMenuItem* m_menu_input;
	m_menu_input = new wxMenuItem( m_menu_settings, 9965, wxString( wxT("Input") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menu_input );
	
	wxMenuItem* m_menu_memory;
	m_menu_memory = new wxMenuItem( m_menu_settings, 9964, wxString( wxT("Memory") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menu_memory );
	
	wxMenuItem* m_menu_video;
	m_menu_video = new wxMenuItem( m_menu_settings, 9963, wxString( wxT("Video") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menu_video );
	
	wxMenuItem* m_menu_sound;
	m_menu_sound = new wxMenuItem( m_menu_settings, 9962, wxString( wxT("Sound") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menu_sound );
	
	wxMenuItem* m_menu_miscsettings;
	m_menu_miscsettings = new wxMenuItem( m_menu_settings, 9961, wxString( wxT("Other") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menu_miscsettings );
	
	wxMenuItem* m_separator7;
	m_separator7 = m_menu_settings->AppendSeparator();
	
	wxMenuItem* m_menuItem49;
	m_menuItem49 = new wxMenuItem( m_menu_settings, 9960, wxString( wxT("Full Screen") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem49 );
	
	wxMenuItem* m_menuItem50;
	m_menuItem50 = new wxMenuItem( m_menu_settings, 9959, wxString( wxT("Reset") ) + wxT('\t') + wxT("F5"), wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem50 );
	
	wxMenuItem* m_menuItem511;
	m_menuItem511 = new wxMenuItem( m_menu_settings, 9958, wxString( wxT("AMX Mouse") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem511 );
	
	wxMenuItem* m_menuItem52;
	m_menuItem52 = new wxMenuItem( m_menu_settings, 9957, wxString( wxT("Multiface Stop") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_settings->Append( m_menuItem52 );
	
	m_menubar1->Append( m_menu_settings, wxT("Settings") ); 
	
	m_menu_debug = new wxMenu();
	m_menuItem_run = new wxMenuItem( m_menu_debug, 9956, wxString( wxT("Run") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_debug->Append( m_menuItem_run );
	m_menuItem_run->Enable( false );
	
	wxMenuItem* m_menuItem_step;
	m_menuItem_step = new wxMenuItem( m_menu_debug, 9955, wxString( wxT("Step") ) + wxT('\t') + wxT("F7"), wxEmptyString, wxITEM_NORMAL );
	m_menu_debug->Append( m_menuItem_step );
	
	m_menuItem_pause = new wxMenuItem( m_menu_debug, 9954, wxString( wxT("Pause") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_debug->Append( m_menuItem_pause );
	
	wxMenuItem* m_menuItem_registers;
	m_menuItem_registers = new wxMenuItem( m_menu_debug, 9953, wxString( wxT("Register") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_debug->Append( m_menuItem_registers );
	
	wxMenuItem* m_menuItem_memory;
	m_menuItem_memory = new wxMenuItem( m_menu_debug, 9952, wxString( wxT("Memory") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_debug->Append( m_menuItem_memory );
	
	m_menubar1->Append( m_menu_debug, wxT("Debug") ); 
	
	m_menu4 = new wxMenu();
	wxMenuItem* m_menuItem57;
	m_menuItem57 = new wxMenuItem( m_menu4, 9951, wxString( wxT("Show Assembler") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem57 );
	
	m_menubar1->Append( m_menu4, wxT("Assembler") ); 
	
	m_menu5 = new wxMenu();
	wxMenuItem* m_menuItem491;
	m_menuItem491 = new wxMenuItem( m_menu5, 9950, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( m_menuItem491 );
	
	m_menubar1->Append( m_menu5, wxT("Help") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 768,540 ), wxWANTS_CHARS );
	m_panel4->SetMinSize( wxSize( 768,540 ) );
	m_panel4->SetMaxSize( wxSize( 768,540 ) );
	
	bSizer12->Add( m_panel4, 1, wxALL|wxFIXED_MINSIZE, 0 );
	
	wxBoxSizer* statusBar;
	statusBar = new wxBoxSizer( wxHORIZONTAL );
	
	DriveActivity = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 20,20 ), wxRAISED_BORDER );
	DriveActivity->SetMinSize( wxSize( 20,20 ) );
	DriveActivity->SetMaxSize( wxSize( 20,20 ) );
	
	statusBar->Add( DriveActivity, 0, wxALL|wxFIXED_MINSIZE, 1 );
	
	m_statusText = new wxStaticText( this, wxID_ANY, wxT("Welcome to Reloaded!"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE|wxSUNKEN_BORDER );
	m_statusText->Wrap( -1 );
	m_statusText->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	statusBar->Add( m_statusText, 1, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 2 );
	
	bSizer12->Add( statusBar, 0, wxALL|wxEXPAND, 1 );
	
	this->SetSizer( bSizer12 );
	this->Layout();
	bSizer12->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( EmulatorWindow::onExit1 ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( EmulatorWindow::OnIdle ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( EmulatorWindow::windowKeyDown ) );
	this->Connect( wxEVT_KEY_UP, wxKeyEventHandler( EmulatorWindow::windowKeyUp ) );
	this->Connect( m_menu_insertDiscA->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onInsertDiscA ) );
	this->Connect( m_menuItem5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::menu_editDiskA ) );
	this->Connect( m_menuItem11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onInsertDiscB ) );
	this->Connect( m_menuItem26->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::insertTape ) );
	this->Connect( m_menuItem29->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::pressPlayOnTape ) );
	this->Connect( m_mI_LoadSNA->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onLoadSNA ) );
	this->Connect( m_ml_ReloadSNA->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onReloadSNA ) );
	this->Connect( m_mI_SaveSNA->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onSaveSNA ) );
	this->Connect( m_ml_UpdateSNA->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onUpdateSNA ) );
	this->Connect( m_mI_SaveSCR->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onSaveScreen ) );
	this->Connect( m_menu_insertDiscA1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onSaveScreenshot ) );
	this->Connect( m_menu_exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onExit2 ) );
	this->Connect( m_menu_input->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onMenuInput ) );
	this->Connect( m_menu_memory->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onMenuMemory ) );
	this->Connect( m_menu_video->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onMenuVideo ) );
	this->Connect( m_menu_sound->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onMenuSound ) );
	this->Connect( m_menu_miscsettings->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onMenuMisc ) );
	this->Connect( m_menuItem49->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnFullScreen ) );
	this->Connect( m_menuItem50->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnReset ) );
	this->Connect( m_menuItem52->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onMultifaceStop ) );
	this->Connect( m_menuItem_run->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnRun ) );
	this->Connect( m_menuItem_step->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnStep ) );
	this->Connect( m_menuItem_pause->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnPause ) );
	this->Connect( m_menuItem_registers->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnRegister ) );
	this->Connect( m_menuItem_memory->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnDebugMemory ) );
	this->Connect( m_menuItem57->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnShowAssembler ) );
	this->Connect( m_menuItem491->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnAbout ) );
	m_panel4->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( EmulatorWindow::windowKeyDown ), NULL, this );
	m_panel4->Connect( wxEVT_KEY_UP, wxKeyEventHandler( EmulatorWindow::windowKeyUp ), NULL, this );
}

EmulatorWindow::~EmulatorWindow()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( EmulatorWindow::onExit1 ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( EmulatorWindow::OnIdle ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( EmulatorWindow::windowKeyDown ) );
	this->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( EmulatorWindow::windowKeyUp ) );
	this->Disconnect( 9999, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onInsertDiscA ) );
	this->Disconnect( 9996, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::menu_editDiskA ) );
	this->Disconnect( 9994, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onInsertDiscB ) );
	this->Disconnect( 9985, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::insertTape ) );
	this->Disconnect( 9982, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::pressPlayOnTape ) );
	this->Disconnect( 9979, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onLoadSNA ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onReloadSNA ) );
	this->Disconnect( 9978, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onSaveSNA ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onUpdateSNA ) );
	this->Disconnect( 9974, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onSaveScreen ) );
	this->Disconnect( 9973, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onSaveScreenshot ) );
	this->Disconnect( 9966, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onExit2 ) );
	this->Disconnect( 9965, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onMenuInput ) );
	this->Disconnect( 9964, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onMenuMemory ) );
	this->Disconnect( 9963, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onMenuVideo ) );
	this->Disconnect( 9962, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onMenuSound ) );
	this->Disconnect( 9961, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onMenuMisc ) );
	this->Disconnect( 9960, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnFullScreen ) );
	this->Disconnect( 9959, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnReset ) );
	this->Disconnect( 9957, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::onMultifaceStop ) );
	this->Disconnect( 9956, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnRun ) );
	this->Disconnect( 9955, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnStep ) );
	this->Disconnect( 9954, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnPause ) );
	this->Disconnect( 9953, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnRegister ) );
	this->Disconnect( 9952, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnDebugMemory ) );
	this->Disconnect( 9951, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnShowAssembler ) );
	this->Disconnect( 9950, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EmulatorWindow::OnAbout ) );
	m_panel4->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( EmulatorWindow::windowKeyDown ), NULL, this );
	m_panel4->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( EmulatorWindow::windowKeyUp ), NULL, this );
	
}

Settings::Settings( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook4 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0|wxWANTS_CHARS );
	m_panel_input = new wxPanel( m_notebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS );
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText63 = new wxStaticText( m_panel_input, wxID_ANY, wxT("Preset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText63->Wrap( -1 );
	gSizer1->Add( m_staticText63, 0, wxFIXED_MINSIZE, 5 );
	
	m_comboBox1 = new wxComboBox( m_panel_input, wxID_ANY, wxT("FR-FR"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	gSizer1->Add( m_comboBox1, 1, 0, 5 );
	
	m_staticText64 = new wxStaticText( m_panel_input, wxID_ANY, wxT("CPC Layout"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText64->Wrap( -1 );
	gSizer1->Add( m_staticText64, 0, wxFIXED_MINSIZE, 5 );
	
	wxString m_choice1Choices[] = { wxT("English 6128"), wxT("French 6128"), wxT("Spanish 6128"), wxT("English 464"), wxT("French 464"), wxT("Spanish 464") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
	m_choice1 = new wxChoice( m_panel_input, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0 );
	m_choice1->SetSelection( 1 );
	gSizer1->Add( m_choice1, 1, 0, 5 );
	
	bSizer26->Add( gSizer1, 0, 0, 5 );
	
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	k_TAB = new wxButton( m_panel_input, CPC_TAB, wxT("TAB"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	k_TAB1 = new wxButton( m_panel_input, CPC_Q, wxT("A"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB1, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB2 = new wxButton( m_panel_input, CPC_W, wxT("Z"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB2, wxGBPosition( 1, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB3 = new wxButton( m_panel_input, CPC_E, wxT("E"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB3, wxGBPosition( 1, 4 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB4 = new wxButton( m_panel_input, CPC_R, wxT("R"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB4, wxGBPosition( 1, 5 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB5 = new wxButton( m_panel_input, CPC_T, wxT("T"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB5, wxGBPosition( 1, 6 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB6 = new wxButton( m_panel_input, CPC_Y, wxT("Y"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB6, wxGBPosition( 1, 7 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB7 = new wxButton( m_panel_input, CPC_U, wxT("U"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB7, wxGBPosition( 1, 8 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB8 = new wxButton( m_panel_input, CPC_I, wxT("I"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB8, wxGBPosition( 1, 9 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB9 = new wxButton( m_panel_input, CPC_O, wxT("O"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB9, wxGBPosition( 1, 10 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB10 = new wxButton( m_panel_input, CPC_P, wxT("P"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB10, wxGBPosition( 1, 11 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB11 = new wxButton( m_panel_input, CPC_AT, wxT("|^"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB11, wxGBPosition( 1, 12 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB12 = new wxButton( m_panel_input, CPC_LBRACKET, wxT("<*"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB12, wxGBPosition( 1, 13 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB13 = new wxButton( m_panel_input, CPC_RETURN, wxT("RET"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT|wxWANTS_CHARS );
	gbSizer1->Add( k_TAB13, wxGBPosition( 1, 14 ), wxGBSpan( 2, 1 ), wxEXPAND, 5 );
	
	k_TAB14 = new wxButton( m_panel_input, CPC_F4, wxT("F4"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB14, wxGBPosition( 1, 15 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB15 = new wxButton( m_panel_input, CPC_F5, wxT("F5"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB15, wxGBPosition( 1, 16 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_TAB16 = new wxButton( m_panel_input, CPC_F6, wxT("F6"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_TAB16, wxGBPosition( 1, 17 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS = new wxButton( m_panel_input, CPC_CAPSLOCK, wxT("CAPS"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS, wxGBPosition( 2, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	k_CAPS1 = new wxButton( m_panel_input, CPC_A, wxT("Q"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS1, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS2 = new wxButton( m_panel_input, CPC_S, wxT("S"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS2, wxGBPosition( 2, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS3 = new wxButton( m_panel_input, CPC_D, wxT("D"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS3, wxGBPosition( 2, 4 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS4 = new wxButton( m_panel_input, CPC_F, wxT("F"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS4, wxGBPosition( 2, 5 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS5 = new wxButton( m_panel_input, CPC_G, wxT("G"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS5, wxGBPosition( 2, 6 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS6 = new wxButton( m_panel_input, CPC_H, wxT("H"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS6, wxGBPosition( 2, 7 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS7 = new wxButton( m_panel_input, CPC_J, wxT("J"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS7, wxGBPosition( 2, 8 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS8 = new wxButton( m_panel_input, CPC_K, wxT("K"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS8, wxGBPosition( 2, 9 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS9 = new wxButton( m_panel_input, CPC_L, wxT("L"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS9, wxGBPosition( 2, 10 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS10 = new wxButton( m_panel_input, CPC_COLON, wxT("M"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS10, wxGBPosition( 2, 11 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS11 = new wxButton( m_panel_input, CPC_SEMICOLON, wxT("%"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS11, wxGBPosition( 2, 12 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS12 = new wxButton( m_panel_input, CPC_RBRACKET, wxT(">#"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS12, wxGBPosition( 2, 13 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS13 = new wxButton( m_panel_input, CPC_F1, wxT("F1"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS13, wxGBPosition( 2, 15 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS14 = new wxButton( m_panel_input, CPC_F2, wxT("F2"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS14, wxGBPosition( 2, 16 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CAPS15 = new wxButton( m_panel_input, CPC_F3, wxT("F3"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CAPS15, wxGBPosition( 2, 17 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT = new wxButton( m_panel_input, CPC_LSHIFT, wxT("SHIFT"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	k_LSHIFT1 = new wxButton( m_panel_input, CPC_Z, wxT("W"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT1, wxGBPosition( 3, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT2 = new wxButton( m_panel_input, CPC_X, wxT("X"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT2, wxGBPosition( 3, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT3 = new wxButton( m_panel_input, CPC_C, wxT("C"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT3, wxGBPosition( 3, 4 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT4 = new wxButton( m_panel_input, CPC_V, wxT("V"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT4, wxGBPosition( 3, 5 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT5 = new wxButton( m_panel_input, CPC_B, wxT("B"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT5, wxGBPosition( 3, 6 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT6 = new wxButton( m_panel_input, CPC_N, wxT("N"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT6, wxGBPosition( 3, 7 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT7 = new wxButton( m_panel_input, CPC_M, wxT("?,"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT7, wxGBPosition( 3, 8 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT8 = new wxButton( m_panel_input, CPC_COMMA, wxT(".;"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT8, wxGBPosition( 3, 9 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT9 = new wxButton( m_panel_input, CPC_PERIOD, wxT("/:"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT9, wxGBPosition( 3, 10 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT10 = new wxButton( m_panel_input, CPC_SLASH, wxT("+="), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT10, wxGBPosition( 3, 11 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT11 = new wxButton( m_panel_input, CPC_BACKSLASH, wxT("@\\$"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT11, wxGBPosition( 3, 12 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT12 = new wxButton( m_panel_input, CPC_RSHIFT, wxT("SHIFT"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT12, wxGBPosition( 3, 13 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	k_LSHIFT13 = new wxButton( m_panel_input, CPC_F0, wxT("F0"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT13, wxGBPosition( 3, 15 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT14 = new wxButton( m_panel_input, CPC_CUR_UP, wxT("^^"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT14, wxGBPosition( 3, 16 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_LSHIFT15 = new wxButton( m_panel_input, CPC_FPERIOD, wxT("."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_LSHIFT15, wxGBPosition( 3, 17 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CONTROL = new wxButton( m_panel_input, CPC_CONTROL, wxT("CONTROL"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL, wxGBPosition( 4, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	k_CONTROL1 = new wxButton( m_panel_input, CPC_COPY, wxT("COPY"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL1, wxGBPosition( 4, 2 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	k_CONTROL2 = new wxButton( m_panel_input, CPC_SPACE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL2, wxGBPosition( 4, 4 ), wxGBSpan( 1, 8 ), wxEXPAND, 5 );
	
	k_CONTROL3 = new wxButton( m_panel_input, CPC_ENTER, wxT("ENTER"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL3, wxGBPosition( 4, 12 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );
	
	k_CONTROL4 = new wxButton( m_panel_input, CPC_CUR_LEFT, wxT("<<"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL4, wxGBPosition( 4, 15 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CONTROL5 = new wxButton( m_panel_input, CPC_CUR_DOWN, wxT("vv"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT|wxWANTS_CHARS );
	gbSizer1->Add( k_CONTROL5, wxGBPosition( 4, 16 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_CONTROL6 = new wxButton( m_panel_input, CPC_CUR_RIGHT, wxT(">>"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_CONTROL6, wxGBPosition( 4, 17 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC = new wxButton( m_panel_input, CPC_ESC, wxT("ESC"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC1 = new wxButton( m_panel_input, CPC_1, wxT("1&&"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC1, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC2 = new wxButton( m_panel_input, CPC_2, wxT("2"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC2, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC3 = new wxButton( m_panel_input, CPC_3, wxT("3\""), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC3, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC4 = new wxButton( m_panel_input, CPC_4, wxT("4'"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC4, wxGBPosition( 0, 4 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC5 = new wxButton( m_panel_input, CPC_5, wxT("5("), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC5, wxGBPosition( 0, 5 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC6 = new wxButton( m_panel_input, CPC_6, wxT("6]"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC6, wxGBPosition( 0, 6 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC7 = new wxButton( m_panel_input, CPC_7, wxT("7"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC7, wxGBPosition( 0, 7 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC8 = new wxButton( m_panel_input, CPC_8, wxT("8!"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC8, wxGBPosition( 0, 8 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC9 = new wxButton( m_panel_input, CPC_9, wxT("9"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC9, wxGBPosition( 0, 9 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC10 = new wxButton( m_panel_input, CPC_0, wxT("0"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC10, wxGBPosition( 0, 10 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC11 = new wxButton( m_panel_input, CPC_MINUS, wxT("[)"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC11, wxGBPosition( 0, 11 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC12 = new wxButton( m_panel_input, CPC_POWER, wxT("_-"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC12, wxGBPosition( 0, 12 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC13 = new wxButton( m_panel_input, CPC_CLR, wxT("CLR"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC13, wxGBPosition( 0, 13 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC14 = new wxButton( m_panel_input, CPC_DEL, wxT("DEL"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC14, wxGBPosition( 0, 14 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC15 = new wxButton( m_panel_input, CPC_F7, wxT("F7"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC15, wxGBPosition( 0, 15 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC16 = new wxButton( m_panel_input, CPC_F8, wxT("F8"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC16, wxGBPosition( 0, 16 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	k_ESC17 = new wxButton( m_panel_input, CPC_F9, wxT("F9"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	gbSizer1->Add( k_ESC17, wxGBPosition( 0, 17 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	bSizer26->Add( gbSizer1, 0, wxEXPAND, 5 );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText65 = new wxStaticText( m_panel_input, wxID_ANY, wxT("Settings for key :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText65->Wrap( -1 );
	gSizer2->Add( m_staticText65, 0, 0, 5 );
	
	m_keyName = new wxStaticText( m_panel_input, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_keyName->Wrap( -1 );
	gSizer2->Add( m_keyName, 0, 0, 5 );
	
	m_staticText67 = new wxStaticText( m_panel_input, wxID_ANY, wxT("PC key to use :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText67->Wrap( -1 );
	gSizer2->Add( m_staticText67, 0, 0, 5 );
	
	m_regularKey = new wxTextCtrl( m_panel_input, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB|wxTE_READONLY );
	m_regularKey->SetExtraStyle( wxWS_EX_BLOCK_EVENTS );
	m_regularKey->Enable( false );
	
	gSizer2->Add( m_regularKey, 0, 0, 5 );
	
	m_staticText68 = new wxStaticText( m_panel_input, wxID_ANY, wxT("PC key to use for shift+key :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText68->Wrap( -1 );
	gSizer2->Add( m_staticText68, 0, 0, 5 );
	
	m_shiftKey = new wxTextCtrl( m_panel_input, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB|wxTE_READONLY );
	gSizer2->Add( m_shiftKey, 0, 0, 5 );
	
	m_staticText69 = new wxStaticText( m_panel_input, wxID_ANY, wxT("PC key to use for control+key :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText69->Wrap( -1 );
	gSizer2->Add( m_staticText69, 0, 0, 5 );
	
	m_ctrlKey = new wxTextCtrl( m_panel_input, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB|wxTE_READONLY );
	gSizer2->Add( m_ctrlKey, 0, 0, 5 );
	
	bSizer26->Add( gSizer2, 1, wxFIXED_MINSIZE, 0 );
	
	m_panel_input->SetSizer( bSizer26 );
	m_panel_input->Layout();
	bSizer26->Fit( m_panel_input );
	m_notebook4->AddPage( m_panel_input, wxT("Input"), true );
	m_panel_memory = new wxPanel( m_notebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText62 = new wxStaticText( m_panel_memory, wxID_ANY, wxT("RAM"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText62->Wrap( -1 );
	bSizer23->Add( m_staticText62, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	RAMSize64 = new wxRadioButton( m_panel_memory, wxID_ANY, wxT("64k"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer23->Add( RAMSize64, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	RAMSize128 = new wxRadioButton( m_panel_memory, wxID_ANY, wxT("128k"), wxDefaultPosition, wxDefaultSize, 0 );
	RAMSize128->SetValue( true ); 
	bSizer23->Add( RAMSize128, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	RAMSize576 = new wxRadioButton( m_panel_memory, wxID_ANY, wxT("576k"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( RAMSize576, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	bSizer28->Add( bSizer23, 0, wxFIXED_MINSIZE, 5 );
	
	m_staticline3 = new wxStaticLine( m_panel_memory, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer28->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText71 = new wxStaticText( m_panel_memory, wxID_ANY, wxT("ROMs"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	bSizer31->Add( m_staticText71, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	wxArrayString ROM0fileChoices;
	ROM0file = new wxChoice( m_panel_memory, wxID_ANY, wxDefaultPosition, wxDefaultSize, ROM0fileChoices, 0 );
	ROM0file->SetSelection( 0 );
	bSizer31->Add( ROM0file, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	wxArrayString ROM1fileChoices;
	ROM1file = new wxChoice( m_panel_memory, wxID_ANY, wxDefaultPosition, wxDefaultSize, ROM1fileChoices, 0 );
	ROM1file->SetSelection( 0 );
	bSizer31->Add( ROM1file, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	wxArrayString ROM2fileChoices;
	ROM2file = new wxChoice( m_panel_memory, wxID_ANY, wxDefaultPosition, wxDefaultSize, ROM2fileChoices, 0 );
	ROM2file->SetSelection( 0 );
	bSizer31->Add( ROM2file, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	wxArrayString ROM3fileChoices;
	ROM3file = new wxChoice( m_panel_memory, wxID_ANY, wxDefaultPosition, wxDefaultSize, ROM3fileChoices, 0 );
	ROM3file->SetSelection( 0 );
	bSizer31->Add( ROM3file, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	wxArrayString ROM4fileChoices;
	ROM4file = new wxChoice( m_panel_memory, wxID_ANY, wxDefaultPosition, wxDefaultSize, ROM4fileChoices, 0 );
	ROM4file->SetSelection( 0 );
	bSizer31->Add( ROM4file, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	wxArrayString ROM5fileChoices;
	ROM5file = new wxChoice( m_panel_memory, wxID_ANY, wxDefaultPosition, wxDefaultSize, ROM5fileChoices, 0 );
	ROM5file->SetSelection( 0 );
	bSizer31->Add( ROM5file, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	wxArrayString ROM6fileChoices;
	ROM6file = new wxChoice( m_panel_memory, wxID_ANY, wxDefaultPosition, wxDefaultSize, ROM6fileChoices, 0 );
	ROM6file->SetSelection( 0 );
	bSizer31->Add( ROM6file, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	wxArrayString ROM7fileChoices;
	ROM7file = new wxChoice( m_panel_memory, wxID_ANY, wxDefaultPosition, wxDefaultSize, ROM7fileChoices, 0 );
	ROM7file->SetSelection( 0 );
	bSizer31->Add( ROM7file, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	bSizer28->Add( bSizer31, 0, wxFIXED_MINSIZE, 5 );
	
	m_staticline31 = new wxStaticLine( m_panel_memory, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer28->Add( m_staticline31, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer( wxVERTICAL );
	
	LKs = new wxStaticText( m_panel_memory, wxID_ANY, wxT("LKs"), wxDefaultPosition, wxDefaultSize, 0 );
	LKs->Wrap( -1 );
	bSizer45->Add( LKs, 0, wxALL, 5 );
	
	wxString manufacturerNameChoices[] = { wxT("Amstrad"), wxT("Orion"), wxT("Schneider"), wxT("Awa"), wxT("Solavox"), wxT("Saisho"), wxT("Triumph"), wxT("ISP") };
	int manufacturerNameNChoices = sizeof( manufacturerNameChoices ) / sizeof( wxString );
	manufacturerName = new wxChoice( m_panel_memory, wxID_ANY, wxDefaultPosition, wxDefaultSize, manufacturerNameNChoices, manufacturerNameChoices, 0 );
	manufacturerName->SetSelection( 0 );
	bSizer45->Add( manufacturerName, 0, wxALL, 5 );
	
	radio50 = new wxRadioButton( m_panel_memory, wxID_ANY, wxT("50Hz"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	radio50->SetValue( true ); 
	bSizer45->Add( radio50, 0, wxALL, 5 );
	
	radio60 = new wxRadioButton( m_panel_memory, wxID_ANY, wxT("60Hz"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer45->Add( radio60, 0, wxALL, 5 );
	
	bSizer28->Add( bSizer45, 1, wxEXPAND, 5 );
	
	m_panel_memory->SetSizer( bSizer28 );
	m_panel_memory->Layout();
	bSizer28->Fit( m_panel_memory );
	m_notebook4->AddPage( m_panel_memory, wxT("Memory"), false );
	m_panel_video = new wxPanel( m_notebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxVERTICAL );
	
	check_OpenGL = new wxCheckBox( m_panel_video, wxID_ANY, wxT("OpenGL"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer32->Add( check_OpenGL, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText76 = new wxStaticText( m_panel_video, wxID_ANY, wxT("CRTC: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText76->Wrap( -1 );
	bSizer36->Add( m_staticText76, 0, wxALL, 5 );
	
	spin_CRTC = new wxSpinCtrl( m_panel_video, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 4, 0 );
	bSizer36->Add( spin_CRTC, 0, wxALL, 5 );
	
	bSizer32->Add( bSizer36, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer361;
	bSizer361 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText761 = new wxStaticText( m_panel_video, wxID_ANY, wxT("Palette:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText761->Wrap( -1 );
	bSizer361->Add( m_staticText761, 0, wxALL, 5 );
	
	wxString choice_colorPaletteChoices[] = { wxT("GT65"), wxT("Grey"), wxT("Plus color"), wxT("CTM644"), wxT("Custom") };
	int choice_colorPaletteNChoices = sizeof( choice_colorPaletteChoices ) / sizeof( wxString );
	choice_colorPalette = new wxChoice( m_panel_video, wxID_ANY, wxDefaultPosition, wxDefaultSize, choice_colorPaletteNChoices, choice_colorPaletteChoices, 0 );
	choice_colorPalette->SetSelection( 0 );
	bSizer361->Add( choice_colorPalette, 0, wxALL, 5 );
	
	m_button76 = new wxButton( m_panel_video, wxID_ANY, wxT("Edit custom palette"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button76->Hide();
	
	bSizer361->Add( m_button76, 0, wxALL, 5 );
	
	bSizer32->Add( bSizer361, 0, wxEXPAND, 5 );
	
	m_panel_video->SetSizer( bSizer32 );
	m_panel_video->Layout();
	bSizer32->Fit( m_panel_video );
	m_notebook4->AddPage( m_panel_video, wxT("Video"), false );
	m_panel_sound = new wxPanel( m_notebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxHORIZONTAL );
	
	m_radioBtn11 = new wxRadioButton( m_panel_sound, wxID_ANY, wxT("8 bit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer42->Add( m_radioBtn11, 0, wxALL, 5 );
	
	m_radioBtn12 = new wxRadioButton( m_panel_sound, wxID_ANY, wxT("16 bit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer42->Add( m_radioBtn12, 0, wxALL, 5 );
	
	bSizer34->Add( bSizer42, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText75 = new wxStaticText( m_panel_sound, wxID_ANY, wxT("Bitrate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	bSizer35->Add( m_staticText75, 0, wxALL, 5 );
	
	m_spinCtrl3 = new wxSpinCtrl( m_panel_sound, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 300000, 44100 );
	bSizer35->Add( m_spinCtrl3, 0, wxALL, 5 );
	
	bSizer34->Add( bSizer35, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer421;
	bSizer421 = new wxBoxSizer( wxHORIZONTAL );
	
	m_radioBtn111 = new wxRadioButton( m_panel_sound, wxID_ANY, wxT("Mono"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizer421->Add( m_radioBtn111, 0, wxALL, 5 );
	
	m_radioBtn121 = new wxRadioButton( m_panel_sound, wxID_ANY, wxT("Stereo"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer421->Add( m_radioBtn121, 0, wxALL, 5 );
	
	bSizer34->Add( bSizer421, 1, wxEXPAND, 5 );
	
	m_panel_sound->SetSizer( bSizer34 );
	m_panel_sound->Layout();
	bSizer34->Fit( m_panel_sound );
	m_notebook4->AddPage( m_panel_sound, wxT("Sound"), false );
	
	bSizer24->Add( m_notebook4, 0, 0, 5 );
	
	wxBoxSizer* bSizer46;
	bSizer46 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button80 = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer46->Add( m_button80, 0, wxALL, 5 );
	
	m_button81 = new wxButton( this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer46->Add( m_button81, 0, wxALL, 5 );
	
	m_button79 = new wxButton( this, wxID_ANY, wxT("Save as default"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer46->Add( m_button79, 0, wxALL, 5 );
	
	m_comboBox2 = new wxComboBox( this, wxID_ANY, wxT("CPC 6128"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_comboBox2->Append( wxT("CPC 464") );
	m_comboBox2->Append( wxT("CPC 664") );
	m_comboBox2->Append( wxT("CPC 6128") );
	m_comboBox2->Append( wxT("464 Plus") );
	m_comboBox2->Append( wxT("6128 Plus") );
	bSizer46->Add( m_comboBox2, 0, wxALL, 5 );
	
	m_button78 = new wxButton( this, wxID_ANY, wxT("Save as profile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer46->Add( m_button78, 0, wxALL, 5 );
	
	bSizer24->Add( bSizer46, 1, wxEXPAND, 0 );
	
	this->SetSizer( bSizer24 );
	this->Layout();
	bSizer24->Fit( this );
	
	// Connect Events
	m_panel_input->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	m_panel_input->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	m_panel_input->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB1->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB1->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB1->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB2->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB2->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB2->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB3->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB3->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB3->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB4->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB4->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB4->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB5->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB5->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB5->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB6->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB6->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB6->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB7->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB7->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB7->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB8->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB8->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB8->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB9->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB9->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB9->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB10->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB10->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB10->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB11->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB11->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB11->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB12->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB12->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB12->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB13->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB13->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB13->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB14->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB14->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB14->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB15->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB15->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB15->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB16->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB16->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB16->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB16->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS1->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS1->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS1->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS2->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS2->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS2->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS3->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS3->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS3->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS4->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS4->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS4->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS5->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS5->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS5->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS6->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS6->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS6->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS7->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS7->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS7->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS8->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS8->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS8->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS9->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS9->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS9->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS10->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS10->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS10->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS11->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS11->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS11->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS12->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS12->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS12->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS13->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS13->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS13->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS14->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS14->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS14->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS15->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS15->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS15->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT1->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT1->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT1->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT2->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT2->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT2->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT3->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT3->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT3->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT4->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT4->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT4->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT5->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT5->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT5->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT6->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT6->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT6->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT7->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT7->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT7->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT8->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT8->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT8->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT9->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT9->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT9->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT10->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT10->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT10->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT11->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT11->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT11->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT12->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT12->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT12->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT13->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT13->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT13->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT14->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT14->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT14->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT15->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT15->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT15->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL1->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL1->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL1->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL2->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL2->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL2->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL3->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL3->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL3->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL4->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL4->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL4->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL5->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL5->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL5->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL6->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL6->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL6->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC1->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC1->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC1->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC2->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC2->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC2->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC3->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC3->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC3->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC4->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC4->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC4->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC5->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC5->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC5->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC6->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC6->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC6->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC7->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC7->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC7->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC8->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC8->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC8->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC9->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC9->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC9->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC10->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC10->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC10->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC11->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC11->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC11->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC12->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC12->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC12->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC13->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC13->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC13->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC14->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC14->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC14->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC15->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC15->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC15->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC16->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC16->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC16->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC16->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC17->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC17->Connect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC17->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC17->Connect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	m_shiftKey->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	m_ctrlKey->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	ROM0file->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM1file->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM2file->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM3file->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM4file->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM5file->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM6file->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM7file->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	manufacturerName->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::SelectManufacturer ), NULL, this );
	radio50->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( Settings::Select50HZ ), NULL, this );
	radio60->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( Settings::Select60HZ ), NULL, this );
	spin_CRTC->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( Settings::changeCRTCType ), NULL, this );
	choice_colorPalette->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::changeColorPalette ), NULL, this );
	m_button80->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::restoreSettings ), NULL, this );
	m_button81->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::applySettings ), NULL, this );
	m_button79->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onSave ), NULL, this );
	m_comboBox2->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( Settings::LoadPreset ), NULL, this );
	m_button78->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::SavePreset ), NULL, this );
}

Settings::~Settings()
{
	// Disconnect Events
	m_panel_input->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	m_panel_input->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	m_panel_input->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB1->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB1->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB1->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB2->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB2->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB2->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB3->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB3->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB3->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB4->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB4->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB4->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB5->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB5->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB5->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB6->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB6->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB6->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB7->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB7->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB7->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB8->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB8->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB8->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB9->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB9->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB9->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB10->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB10->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB10->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB11->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB11->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB11->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB12->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB12->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB12->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB13->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB13->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB13->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB14->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB14->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB14->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB15->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB15->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB15->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB15->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB16->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_TAB16->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB16->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_TAB16->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS1->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS1->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS1->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS2->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS2->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS2->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS3->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS3->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS3->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS4->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS4->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS4->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS5->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS5->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS5->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS6->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS6->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS6->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS7->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS7->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS7->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS8->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS8->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS8->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS9->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS9->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS9->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS10->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS10->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS10->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS11->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS11->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS11->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS12->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS12->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS12->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS13->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS13->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS13->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS14->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS14->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS14->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS15->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CAPS15->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS15->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CAPS15->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT1->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT1->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT1->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT2->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT2->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT2->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT3->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT3->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT3->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT4->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT4->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT4->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT5->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT5->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT5->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT6->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT6->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT6->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT7->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT7->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT7->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT8->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT8->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT8->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT9->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT9->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT9->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT10->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT10->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT10->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT11->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT11->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT11->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT12->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT12->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT12->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT13->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT13->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT13->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT14->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT14->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT14->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT15->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_LSHIFT15->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT15->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_LSHIFT15->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL1->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL1->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL1->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL2->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL2->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL2->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL3->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL3->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL3->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL4->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL4->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL4->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL5->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL5->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL5->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_CONTROL6->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL6->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_CONTROL6->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC1->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC1->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC1->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC2->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC2->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC2->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC3->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC3->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC3->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC4->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC4->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC4->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC5->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC5->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC5->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC6->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC6->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC6->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC7->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC7->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC7->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC8->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC8->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC8->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC9->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC9->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC9->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC10->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC10->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC10->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC11->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC11->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC11->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC12->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC12->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC12->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC13->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC13->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC13->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC14->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC14->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC14->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC15->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC15->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC15->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC15->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC16->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC16->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC16->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC16->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC17->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onKeyClick ), NULL, this );
	k_ESC17->Disconnect( wxEVT_CHAR, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC17->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	k_ESC17->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	m_shiftKey->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	m_ctrlKey->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Settings::onKeyPress ), NULL, this );
	ROM0file->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM1file->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM2file->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM3file->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM4file->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM5file->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM6file->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	ROM7file->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::RomChanged ), NULL, this );
	manufacturerName->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::SelectManufacturer ), NULL, this );
	radio50->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( Settings::Select50HZ ), NULL, this );
	radio60->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( Settings::Select60HZ ), NULL, this );
	spin_CRTC->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( Settings::changeCRTCType ), NULL, this );
	choice_colorPalette->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Settings::changeColorPalette ), NULL, this );
	m_button80->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::restoreSettings ), NULL, this );
	m_button81->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::applySettings ), NULL, this );
	m_button79->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::onSave ), NULL, this );
	m_comboBox2->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( Settings::LoadPreset ), NULL, this );
	m_button78->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::SavePreset ), NULL, this );
	
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

About::About( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
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
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Reloaded"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
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
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Based on sourcecode from Caprice 32 :\n(c) 1997 - 2005 Ulrich Doewich\n\nReloaded is :\n(c) 1997 - 2010 The Reloaded Author's"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText5->Wrap( -1 );
	bSizer10->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	bSizer8->Add( bSizer10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("Credits :\nWritten by :\nUlrich Doewich\nRamlaid/Arkos <contact@ramlaid.com>\nRomain Giot  - Krusty/Benediction <krusty@cpcscene.com>\nAdrien Destugues - PulkoMandy/Shinra <pulkomandy@pulkomandy.ath.cx>\nCloudStrife/Shinra <cloudstrife@cpcscene.com>\nContributors :\nStSound library by Leonard/Oxygene\nwith finetuning by Targhan/Arkos\nSDL - wxWidgets - Sjasmplus\nand a lot of other contributors\n"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer11->Add( m_staticText10, 0, wxALL|wxEXPAND, 5 );
	
	bSizer8->Add( bSizer11, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer8 );
	this->Layout();
	bSizer8->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( About::OnAboutClose ) );
}

About::~About()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( About::OnAboutClose ) );
	
}

RegistersStates::RegistersStates( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_nb_Register = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	z80_tab = new wxPanel( m_nb_Register, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( z80_tab, wxID_ANY, wxT("8bits Registers") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 1, 3, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText9 = new wxStaticText( z80_tab, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	m_staticText9->Hide();
	
	fgSizer3->Add( m_staticText9, 0, wxALL, 5 );
	
	m_staticText10 = new wxStaticText( z80_tab, wxID_ANY, wxT("Primary"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer3->Add( m_staticText10, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText11 = new wxStaticText( z80_tab, wxID_ANY, wxT("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer3->Add( m_staticText11, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText12 = new wxStaticText( z80_tab, wxID_ANY, wxT("A"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer3->Add( m_staticText12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCrA = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrA, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrAx = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrAx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText13 = new wxStaticText( z80_tab, wxID_ANY, wxT("F"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer3->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCrF = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrF, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrFx = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrFx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText14 = new wxStaticText( z80_tab, wxID_ANY, wxT("B"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer3->Add( m_staticText14, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrB = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrB, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrBx = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrBx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText15 = new wxStaticText( z80_tab, wxID_ANY, wxT("C"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	fgSizer3->Add( m_staticText15, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrC = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrC, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrCx = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrCx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText16 = new wxStaticText( z80_tab, wxID_ANY, wxT("D"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	fgSizer3->Add( m_staticText16, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrD = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrD, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrDx = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrDx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText17 = new wxStaticText( z80_tab, wxID_ANY, wxT("E"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	fgSizer3->Add( m_staticText17, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrE = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrE, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrEx = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrEx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText18 = new wxStaticText( z80_tab, wxID_ANY, wxT("H"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer3->Add( m_staticText18, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrH = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrH, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrHx = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrHx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText19 = new wxStaticText( z80_tab, wxID_ANY, wxT("L"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	fgSizer3->Add( m_staticText19, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrL = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrL, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrLx = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrLx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText31 = new wxStaticText( z80_tab, wxID_ANY, wxT("I"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	fgSizer3->Add( m_staticText31, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrI = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrI, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText33 = new wxStaticText( z80_tab, wxID_ANY, wxT("R"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33->Wrap( -1 );
	fgSizer3->Add( m_staticText33, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrR = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer3->Add( m_tCrR, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	sbSizer1->Add( fgSizer3, 0, wxEXPAND, 5 );
	
	fgSizer2->Add( sbSizer1, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( z80_tab, wxID_ANY, wxT("16bits Registers") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText22 = new wxStaticText( z80_tab, wxID_ANY, wxT("Primary"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	fgSizer5->Add( m_staticText22, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText23 = new wxStaticText( z80_tab, wxID_ANY, wxT("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	fgSizer5->Add( m_staticText23, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText24 = new wxStaticText( z80_tab, wxID_ANY, wxT("BC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer5->Add( m_staticText24, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrBC = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer5->Add( m_tCrBC, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrBCx = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer5->Add( m_tCrBCx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText25 = new wxStaticText( z80_tab, wxID_ANY, wxT("DE"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	fgSizer5->Add( m_staticText25, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrDE = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer5->Add( m_tCrDE, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrDEx = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer5->Add( m_tCrDEx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText26 = new wxStaticText( z80_tab, wxID_ANY, wxT("HL"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	fgSizer5->Add( m_staticText26, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrHL = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer5->Add( m_tCrHL, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrHLx = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer5->Add( m_tCrHLx, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText27 = new wxStaticText( z80_tab, wxID_ANY, wxT("IX"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	fgSizer5->Add( m_staticText27, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrIX = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer5->Add( m_tCrIX, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText28 = new wxStaticText( z80_tab, wxID_ANY, wxT("IY"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	fgSizer5->Add( m_staticText28, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrIY = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer5->Add( m_tCrIY, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText29 = new wxStaticText( z80_tab, wxID_ANY, wxT("PC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText29->Wrap( -1 );
	fgSizer5->Add( m_staticText29, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrPC = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer5->Add( m_tCrPC, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText30 = new wxStaticText( z80_tab, wxID_ANY, wxT("SP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer5->Add( m_staticText30, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCrSP = new wxTextCtrl( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer5->Add( m_tCrSP, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	sbSizer2->Add( fgSizer5, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	bSizer18->Add( sbSizer2, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( z80_tab, wxID_ANY, wxT("Flags States") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 2, 7, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText35 = new wxStaticText( z80_tab, wxID_ANY, wxT("S"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	fgSizer6->Add( m_staticText35, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText36 = new wxStaticText( z80_tab, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	fgSizer6->Add( m_staticText36, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText37 = new wxStaticText( z80_tab, wxID_ANY, wxT("H"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText37->Wrap( -1 );
	fgSizer6->Add( m_staticText37, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText38 = new wxStaticText( z80_tab, wxID_ANY, wxT("P"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	fgSizer6->Add( m_staticText38, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText39 = new wxStaticText( z80_tab, wxID_ANY, wxT("N"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	fgSizer6->Add( m_staticText39, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText40 = new wxStaticText( z80_tab, wxID_ANY, wxT("C"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText40->Wrap( -1 );
	fgSizer6->Add( m_staticText40, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText41 = new wxStaticText( z80_tab, wxID_ANY, wxT("Primary"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	fgSizer6->Add( m_staticText41, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cB_SFlags = new wxCheckBox( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_SFlags, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	m_cB_ZFlags = new wxCheckBox( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_ZFlags, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_HFlags = new wxCheckBox( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_HFlags, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_PFlags = new wxCheckBox( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_PFlags, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_NFlags = new wxCheckBox( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_NFlags, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_CFlags = new wxCheckBox( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_CFlags, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_staticText42 = new wxStaticText( z80_tab, wxID_ANY, wxT("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer6->Add( m_staticText42, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cB_SFlagsShadow = new wxCheckBox( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_SFlagsShadow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_ZFlagsShadow = new wxCheckBox( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_ZFlagsShadow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_HFlagsShadow = new wxCheckBox( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_HFlagsShadow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_PFlagsShadow = new wxCheckBox( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_PFlagsShadow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_NFlagsShadow = new wxCheckBox( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_NFlagsShadow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	m_cB_CFlagsShadow = new wxCheckBox( z80_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cB_CFlagsShadow, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxALIGN_CENTER_VERTICAL, 1 );
	
	sbSizer3->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	bSizer18->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	fgSizer2->Add( bSizer18, 0, wxEXPAND, 5 );
	
	bSizer23->Add( fgSizer2, 1, 0, 5 );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( z80_tab, wxID_ANY, wxT("Shadow Register States") ), wxHORIZONTAL );
	
	m_checkBox15 = new wxCheckBox( z80_tab, wxID_ANY, wxT("AF Register in Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer7->Add( m_checkBox15, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	m_checkBox16 = new wxCheckBox( z80_tab, wxID_ANY, wxT("General Register in Shadow Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer7->Add( m_checkBox16, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	bSizer23->Add( sbSizer7, 0, wxALIGN_TOP|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( z80_tab, wxID_ANY, wxT("Interrupts") ), wxHORIZONTAL );
	
	m_cb_interruptenabled = new wxCheckBox( z80_tab, wxID_ANY, wxT("enabled"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer9->Add( m_cb_interruptenabled, 0, wxALL, 5 );
	
	wxString m_c_interruptModeChoices[] = { wxT("IM0"), wxT("IM1"), wxT("IM2") };
	int m_c_interruptModeNChoices = sizeof( m_c_interruptModeChoices ) / sizeof( wxString );
	m_c_interruptMode = new wxChoice( z80_tab, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_c_interruptModeNChoices, m_c_interruptModeChoices, 0 );
	m_c_interruptMode->SetSelection( 0 );
	sbSizer9->Add( m_c_interruptMode, 0, wxALL, 5 );
	
	m_cb_interruptpending = new wxCheckBox( z80_tab, wxID_ANY, wxT("pending"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer9->Add( m_cb_interruptpending, 0, wxALL, 5 );
	
	m_cb_nmipending = new wxCheckBox( z80_tab, wxID_ANY, wxT("NMI pending"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer9->Add( m_cb_nmipending, 0, wxALL, 5 );
	
	bSizer23->Add( sbSizer9, 0, wxEXPAND|wxTOP, 5 );
	
	z80_tab->SetSizer( bSizer23 );
	z80_tab->Layout();
	bSizer23->Fit( z80_tab );
	m_nb_Register->AddPage( z80_tab, wxT("Z80"), true );
	crtc_tab = new wxPanel( m_nb_Register, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( crtc_tab, wxID_ANY, wxT("Internal") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText46 = new wxStaticText( crtc_tab, wxID_ANY, wxT("HCC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText46->Wrap( -1 );
	fgSizer9->Add( m_staticText46, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCcHCC = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcHCC->SetMaxLength( 3 ); 
	m_tCcHCC->SetToolTip( wxT("Horizontal Caracter Counter") );
	
	fgSizer9->Add( m_tCcHCC, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText47 = new wxStaticText( crtc_tab, wxID_ANY, wxT("VCC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText47->Wrap( -1 );
	fgSizer9->Add( m_staticText47, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCcVCC = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcVCC->SetToolTip( wxT("Vertical Caracter Counter") );
	
	fgSizer9->Add( m_tCcVCC, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText48 = new wxStaticText( crtc_tab, wxID_ANY, wxT("VLA"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	fgSizer9->Add( m_staticText48, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCcVLA = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcVLA->SetToolTip( wxT("Vertical Line Adjust") );
	
	fgSizer9->Add( m_tCcVLA, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText49 = new wxStaticText( crtc_tab, wxID_ANY, wxT("VLC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText49->Wrap( -1 );
	fgSizer9->Add( m_staticText49, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tCcVLC = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcVLC->SetToolTip( wxT("Vertical Line Counter") );
	
	fgSizer9->Add( m_tCcVLC, 0, wxALIGN_CENTER|wxALL, 5 );
	
	sbSizer4->Add( fgSizer9, 1, wxEXPAND, 5 );
	
	bSizer42->Add( sbSizer4, 1, wxEXPAND|wxLEFT, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( crtc_tab, wxID_ANY, wxT("Horizontal") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText50 = new wxStaticText( crtc_tab, wxID_ANY, wxT("R0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( -1 );
	fgSizer11->Add( m_staticText50, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR0 = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcR0->SetToolTip( wxT("Horizontal total character number") );
	
	fgSizer11->Add( m_tCcR0, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText51 = new wxStaticText( crtc_tab, wxID_ANY, wxT("R1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer11->Add( m_staticText51, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR1 = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcR1->SetToolTip( wxT("Horizontal displayed character number") );
	
	fgSizer11->Add( m_tCcR1, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText52 = new wxStaticText( crtc_tab, wxID_ANY, wxT("R2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText52->Wrap( -1 );
	fgSizer11->Add( m_staticText52, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR2 = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcR2->SetToolTip( wxT("Position of horizontal sync. pulse") );
	
	fgSizer11->Add( m_tCcR2, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText53 = new wxStaticText( crtc_tab, wxID_ANY, wxT("R3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53->Wrap( -1 );
	fgSizer11->Add( m_staticText53, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR3 = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcR3->SetToolTip( wxT("Width of horizontal/vertical sync. pulses") );
	
	fgSizer11->Add( m_tCcR3, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	fgSizer10->Add( fgSizer11, 1, wxEXPAND, 5 );
	
	sbSizer5->Add( fgSizer10, 1, wxEXPAND, 5 );
	
	bSizer42->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( crtc_tab, wxID_ANY, wxT("Vertical") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText54 = new wxStaticText( crtc_tab, wxID_ANY, wxT("R4"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText54->Wrap( -1 );
	fgSizer12->Add( m_staticText54, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR4 = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcR4->SetToolTip( wxT(" Vertical total Line character number") );
	
	fgSizer12->Add( m_tCcR4, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText55 = new wxStaticText( crtc_tab, wxID_ANY, wxT("R5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText55->Wrap( -1 );
	fgSizer12->Add( m_staticText55, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR5 = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcR5->SetToolTip( wxT("Vertical raster adjust") );
	
	fgSizer12->Add( m_tCcR5, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText56 = new wxStaticText( crtc_tab, wxID_ANY, wxT("R6"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText56->Wrap( -1 );
	fgSizer12->Add( m_staticText56, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR6 = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcR6->SetToolTip( wxT("Vertical displayed character number") );
	
	fgSizer12->Add( m_tCcR6, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText57 = new wxStaticText( crtc_tab, wxID_ANY, wxT("R7"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	fgSizer12->Add( m_staticText57, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR7 = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcR7->SetToolTip( wxT("Position of vertical sync. pulse") );
	
	fgSizer12->Add( m_tCcR7, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticText58 = new wxStaticText( crtc_tab, wxID_ANY, wxT("R9"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText58->Wrap( -1 );
	fgSizer12->Add( m_staticText58, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_tCcR9 = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_tCcR9->SetToolTip( wxT("Maximum raster") );
	
	fgSizer12->Add( m_tCcR9, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer6->Add( fgSizer12, 1, wxEXPAND, 5 );
	
	bSizer42->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer12;
	sbSizer12 = new wxStaticBoxSizer( new wxStaticBox( crtc_tab, wxID_ANY, wxT("Other") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer13->SetFlexibleDirection( wxBOTH );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText80 = new wxStaticText( crtc_tab, wxID_ANY, wxT("R8"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText80->Wrap( -1 );
	fgSizer13->Add( m_staticText80, 0, wxALL, 5 );
	
	m_textCtrl47 = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer13->Add( m_textCtrl47, 0, wxALL, 5 );
	
	m_staticText81 = new wxStaticText( crtc_tab, wxID_ANY, wxT("R12"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText81->Wrap( -1 );
	fgSizer13->Add( m_staticText81, 0, wxALL, 5 );
	
	m_textCtrl48 = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer13->Add( m_textCtrl48, 0, wxALL, 5 );
	
	m_staticText82 = new wxStaticText( crtc_tab, wxID_ANY, wxT("R13"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText82->Wrap( -1 );
	fgSizer13->Add( m_staticText82, 0, wxALL, 5 );
	
	m_textCtrl49 = new wxTextCtrl( crtc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer13->Add( m_textCtrl49, 0, wxALL, 5 );
	
	sbSizer12->Add( fgSizer13, 1, wxEXPAND, 5 );
	
	bSizer42->Add( sbSizer12, 1, wxEXPAND|wxRIGHT, 5 );
	
	bSizer21->Add( bSizer42, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( crtc_tab, wxID_ANY, wxT("Timing Check") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer121;
	fgSizer121 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer121->SetFlexibleDirection( wxBOTH );
	fgSizer121->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText59 = new wxStaticText( crtc_tab, wxID_ANY, wxT("Horizontal Line Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText59->Wrap( -1 );
	fgSizer121->Add( m_staticText59, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sT_HLineLength = new wxStaticText( crtc_tab, wxID_ANY, wxT("xxxxx us"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sT_HLineLength->Wrap( -1 );
	fgSizer121->Add( m_sT_HLineLength, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cB_HLineLengthIsOk = new wxCheckBox( crtc_tab, wxID_ANY, wxT("Timing is Ok !"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer121->Add( m_cB_HLineLengthIsOk, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText61 = new wxStaticText( crtc_tab, wxID_ANY, wxT("Vertical Frame Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	fgSizer121->Add( m_staticText61, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sT_VFrameLength = new wxStaticText( crtc_tab, wxID_ANY, wxT("xxxxx us"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sT_VFrameLength->Wrap( -1 );
	fgSizer121->Add( m_sT_VFrameLength, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_cB_VFrameLengthIsOk = new wxCheckBox( crtc_tab, wxID_ANY, wxT("Timing is Ok !"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer121->Add( m_cB_VFrameLengthIsOk, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	sbSizer8->Add( fgSizer121, 1, wxEXPAND, 5 );
	
	bSizer21->Add( sbSizer8, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	crtc_tab->SetSizer( bSizer21 );
	crtc_tab->Layout();
	bSizer21->Fit( crtc_tab );
	m_nb_Register->AddPage( crtc_tab, wxT("CRTC"), false );
	ga_tab = new wxPanel( m_nb_Register, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer14;
	sbSizer14 = new wxStaticBoxSizer( new wxStaticBox( ga_tab, wxID_ANY, wxT("Video") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 2, 6, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText84 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText84->Wrap( -1 );
	fgSizer14->Add( m_staticText84, 0, wxALL, 5 );
	
	m_pen = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_pen->SetMaxLength( 2 ); 
	m_pen->SetMinSize( wxSize( 100,-1 ) );
	m_pen->SetMaxSize( wxSize( 100,-1 ) );
	
	fgSizer14->Add( m_pen, 1, wxALL|wxFIXED_MINSIZE, 5 );
	
	
	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText85 = new wxStaticText( ga_tab, wxID_ANY, wxT("Video mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText85->Wrap( -1 );
	fgSizer14->Add( m_staticText85, 0, wxALL, 5 );
	
	m_vmode = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_vmode, 0, wxALL, 5 );
	
	
	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText86 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText86->Wrap( -1 );
	fgSizer14->Add( m_staticText86, 0, wxALL, 5 );
	
	m_ink0 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_ink0->SetMaxLength( 2 ); 
	fgSizer14->Add( m_ink0, 1, wxALL, 5 );
	
	m_bas0 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas0, 0, wxALL, 5 );
	
	m_staticText87 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText87->Wrap( -1 );
	fgSizer14->Add( m_staticText87, 0, wxALL, 5 );
	
	m_ink1 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_ink1, 0, wxALL, 5 );
	
	m_bas1 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas1, 0, wxALL, 5 );
	
	m_staticText88 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText88->Wrap( -1 );
	fgSizer14->Add( m_staticText88, 0, wxALL, 5 );
	
	m_ink2 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_ink2->SetMaxLength( 2 ); 
	fgSizer14->Add( m_ink2, 1, wxALL, 5 );
	
	m_bas2 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas2, 0, wxALL, 5 );
	
	m_staticText89 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText89->Wrap( -1 );
	fgSizer14->Add( m_staticText89, 0, wxALL, 5 );
	
	m_ink3 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_ink3, 0, wxALL, 5 );
	
	m_bas3 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas3, 0, wxALL, 5 );
	
	m_staticText90 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 4"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText90->Wrap( -1 );
	fgSizer14->Add( m_staticText90, 0, wxALL, 5 );
	
	m_ink4 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_ink4->SetMaxLength( 2 ); 
	fgSizer14->Add( m_ink4, 1, wxALL, 5 );
	
	m_bas4 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas4, 0, wxALL, 5 );
	
	m_staticText91 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	fgSizer14->Add( m_staticText91, 0, wxALL, 5 );
	
	m_ink5 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_ink5, 0, wxALL, 5 );
	
	m_bas5 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas5, 0, wxALL, 5 );
	
	m_staticText92 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 6"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText92->Wrap( -1 );
	fgSizer14->Add( m_staticText92, 0, wxALL, 5 );
	
	m_ink6 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_ink6->SetMaxLength( 2 ); 
	fgSizer14->Add( m_ink6, 1, wxALL, 5 );
	
	m_bas6 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas6, 0, wxALL, 5 );
	
	m_staticText93 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 7"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText93->Wrap( -1 );
	fgSizer14->Add( m_staticText93, 0, wxALL, 5 );
	
	m_ink7 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_ink7, 0, wxALL, 5 );
	
	m_bas7 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas7, 0, wxALL, 5 );
	
	m_staticText94 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 8"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText94->Wrap( -1 );
	fgSizer14->Add( m_staticText94, 0, wxALL, 5 );
	
	m_ink8 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_ink8->SetMaxLength( 2 ); 
	fgSizer14->Add( m_ink8, 1, wxALL, 5 );
	
	m_bas8 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas8, 0, wxALL, 5 );
	
	m_staticText95 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 9"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText95->Wrap( -1 );
	fgSizer14->Add( m_staticText95, 0, wxALL, 5 );
	
	m_ink9 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_ink9, 0, wxALL, 5 );
	
	m_bas9 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas9, 0, wxALL, 5 );
	
	m_staticText96 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 10"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText96->Wrap( -1 );
	fgSizer14->Add( m_staticText96, 0, wxALL, 5 );
	
	m_ink10 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_ink10->SetMaxLength( 2 ); 
	fgSizer14->Add( m_ink10, 1, wxALL, 5 );
	
	m_bas10 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas10, 0, wxALL, 5 );
	
	m_staticText97 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 11"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText97->Wrap( -1 );
	fgSizer14->Add( m_staticText97, 0, wxALL, 5 );
	
	m_ink11 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_ink11, 0, wxALL, 5 );
	
	m_bas11 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas11, 0, wxALL, 5 );
	
	m_staticText98 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 12"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText98->Wrap( -1 );
	fgSizer14->Add( m_staticText98, 0, wxALL, 5 );
	
	m_ink12 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_ink12->SetMaxLength( 2 ); 
	fgSizer14->Add( m_ink12, 1, wxALL, 5 );
	
	m_bas12 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas12, 0, wxALL, 5 );
	
	m_staticText99 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 13"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText99->Wrap( -1 );
	fgSizer14->Add( m_staticText99, 0, wxALL, 5 );
	
	m_ink13 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_ink13, 0, wxALL, 5 );
	
	m_bas13 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas13, 0, wxALL, 5 );
	
	m_staticText100 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 14"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText100->Wrap( -1 );
	fgSizer14->Add( m_staticText100, 0, wxALL, 5 );
	
	m_ink14 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_ink14->SetMaxLength( 2 ); 
	fgSizer14->Add( m_ink14, 1, wxALL, 5 );
	
	m_bas14 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas14, 0, wxALL, 5 );
	
	m_staticText101 = new wxStaticText( ga_tab, wxID_ANY, wxT("Pen 15"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	fgSizer14->Add( m_staticText101, 0, wxALL, 5 );
	
	m_ink15 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_ink15, 0, wxALL, 5 );
	
	m_bas15 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas15, 0, wxALL, 5 );
	
	m_staticText102 = new wxStaticText( ga_tab, wxID_ANY, wxT("Border"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText102->Wrap( -1 );
	fgSizer14->Add( m_staticText102, 0, wxALL, 5 );
	
	m_border = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	m_border->SetMaxLength( 2 ); 
	fgSizer14->Add( m_border, 1, wxALL, 5 );
	
	m_bas16 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer14->Add( m_bas16, 0, wxALL, 5 );
	
	sbSizer14->Add( fgSizer14, 1, wxFIXED_MINSIZE, 5 );
	
	bSizer43->Add( sbSizer14, 0, 0, 5 );
	
	wxStaticBoxSizer* sbSizer16;
	sbSizer16 = new wxStaticBoxSizer( new wxStaticBox( ga_tab, wxID_ANY, wxT("Memory") ), wxHORIZONTAL );
	
	m_staticText103 = new wxStaticText( ga_tab, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText103->Wrap( -1 );
	sbSizer16->Add( m_staticText103, 0, wxALL, 5 );
	
	m_textCtrl70 = new wxTextCtrl( ga_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer16->Add( m_textCtrl70, 0, wxALL, 5 );
	
	bSizer43->Add( sbSizer16, 0, 0, 5 );
	
	ga_tab->SetSizer( bSizer43 );
	ga_tab->Layout();
	bSizer43->Fit( ga_tab );
	m_nb_Register->AddPage( ga_tab, wxT("Gate Array"), false );
	fdc_tab = new wxPanel( m_nb_Register, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("FDC") );
	wxBoxSizer* bSizer48;
	bSizer48 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer161;
	sbSizer161 = new wxStaticBoxSizer( new wxStaticBox( fdc_tab, wxID_ANY, wxT("Drive A") ), wxVERTICAL );
	
	wxGridSizer* gSizer4;
	gSizer4 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText115 = new wxStaticText( fdc_tab, wxID_ANY, wxT("Track"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText115->Wrap( -1 );
	gSizer4->Add( m_staticText115, 0, wxALL, 5 );
	
	a_track = new wxTextCtrl( fdc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer4->Add( a_track, 0, wxALL, 5 );
	
	m_staticText116 = new wxStaticText( fdc_tab, wxID_ANY, wxT("Sector"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText116->Wrap( -1 );
	gSizer4->Add( m_staticText116, 0, wxALL, 5 );
	
	a_sector = new wxTextCtrl( fdc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer4->Add( a_sector, 0, wxALL, 5 );
	
	sbSizer161->Add( gSizer4, 1, wxEXPAND, 5 );
	
	bSizer48->Add( sbSizer161, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer17;
	sbSizer17 = new wxStaticBoxSizer( new wxStaticBox( fdc_tab, wxID_ANY, wxT("Drive B") ), wxVERTICAL );
	
	wxGridSizer* gSizer5;
	gSizer5 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText117 = new wxStaticText( fdc_tab, wxID_ANY, wxT("Track"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText117->Wrap( -1 );
	gSizer5->Add( m_staticText117, 0, wxALL, 5 );
	
	b_track = new wxTextCtrl( fdc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer5->Add( b_track, 0, wxALL, 5 );
	
	m_staticText118 = new wxStaticText( fdc_tab, wxID_ANY, wxT("Sector"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText118->Wrap( -1 );
	gSizer5->Add( m_staticText118, 0, wxALL, 5 );
	
	b_sector = new wxTextCtrl( fdc_tab, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer5->Add( b_sector, 0, wxALL, 5 );
	
	sbSizer17->Add( gSizer5, 1, wxEXPAND, 5 );
	
	bSizer48->Add( sbSizer17, 1, wxEXPAND, 5 );
	
	fdc_tab->SetSizer( bSizer48 );
	fdc_tab->Layout();
	bSizer48->Fit( fdc_tab );
	m_nb_Register->AddPage( fdc_tab, wxT("FDC"), false );
	
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
	m_cb_interruptenabled->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( RegistersStates::onInterruptToggle ), NULL, this );
	m_c_interruptMode->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( RegistersStates::onInterruptModeChange ), NULL, this );
	m_cb_interruptpending->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( RegistersStates::onInterruptSend ), NULL, this );
	m_cb_nmipending->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( RegistersStates::onNMISend ), NULL, this );
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
	m_cb_interruptenabled->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( RegistersStates::onInterruptToggle ), NULL, this );
	m_c_interruptMode->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( RegistersStates::onInterruptModeChange ), NULL, this );
	m_cb_interruptpending->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( RegistersStates::onInterruptSend ), NULL, this );
	m_cb_nmipending->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( RegistersStates::onNMISend ), NULL, this );
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
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxHORIZONTAL );
	
	m_notebook4 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel16 = new wxPanel( m_notebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer46;
	bSizer46 = new wxBoxSizer( wxHORIZONTAL );
	
	overviewPanel = new wxPanel( m_panel16, wxID_ANY, wxDefaultPosition, wxSize( 256,256 ), 0 );
	overviewPanel->SetMinSize( wxSize( 256,256 ) );
	overviewPanel->SetMaxSize( wxSize( 256,256 ) );
	
	bSizer46->Add( overviewPanel, 0, wxALL|wxFIXED_MINSIZE, 5 );
	
	wxBoxSizer* bSizer47;
	bSizer47 = new wxBoxSizer( wxVERTICAL );
	
	bSizer47->SetMinSize( wxSize( -1,256 ) ); 
	m_staticText107 = new wxStaticText( m_panel16, wxID_ANY, wxT("&&0000"), wxDefaultPosition, wxSize( -1,64 ), wxST_NO_AUTORESIZE );
	m_staticText107->Wrap( -1 );
	m_staticText107->SetMinSize( wxSize( -1,64 ) );
	m_staticText107->SetMaxSize( wxSize( -1,64 ) );
	
	bSizer47->Add( m_staticText107, 0, wxFIXED_MINSIZE|wxLEFT, 5 );
	
	m_staticText108 = new wxStaticText( m_panel16, wxID_ANY, wxT("&&4000"), wxDefaultPosition, wxSize( -1,64 ), 0 );
	m_staticText108->Wrap( -1 );
	bSizer47->Add( m_staticText108, 0, wxLEFT, 5 );
	
	m_staticText109 = new wxStaticText( m_panel16, wxID_ANY, wxT("&&8000"), wxDefaultPosition, wxSize( -1,64 ), 0 );
	m_staticText109->Wrap( -1 );
	bSizer47->Add( m_staticText109, 0, wxLEFT, 5 );
	
	m_staticText110 = new wxStaticText( m_panel16, wxID_ANY, wxT("&&C000"), wxDefaultPosition, wxSize( -1,64 ), 0 );
	m_staticText110->Wrap( -1 );
	bSizer47->Add( m_staticText110, 0, wxLEFT, 5 );
	
	bSizer46->Add( bSizer47, 0, wxFIXED_MINSIZE, 5 );
	
	wxBoxSizer* bSizer48;
	bSizer48 = new wxBoxSizer( wxVERTICAL );
	
	bSizer48->SetMinSize( wxSize( -1,256 ) ); 
	
	bSizer48->Add( 0, 32, 1, wxFIXED_MINSIZE|wxLEFT, 5 );
	
	zone0 = new wxStaticText( m_panel16, wxID_ANY, wxT("Central RAM"), wxDefaultPosition, wxSize( -1,64 ), wxST_NO_AUTORESIZE );
	zone0->Wrap( -1 );
	bSizer48->Add( zone0, 0, wxEXPAND|wxLEFT, 5 );
	
	zone1 = new wxStaticText( m_panel16, wxID_ANY, wxT("Central RAM"), wxDefaultPosition, wxSize( -1,64 ), wxST_NO_AUTORESIZE );
	zone1->Wrap( -1 );
	bSizer48->Add( zone1, 0, wxEXPAND|wxLEFT, 5 );
	
	zone2 = new wxStaticText( m_panel16, wxID_ANY, wxT("Central RAM"), wxDefaultPosition, wxSize( -1,64 ), wxST_NO_AUTORESIZE );
	zone2->Wrap( -1 );
	bSizer48->Add( zone2, 0, wxEXPAND|wxLEFT, 5 );
	
	zone3 = new wxStaticText( m_panel16, wxID_ANY, wxT("Central RAM"), wxDefaultPosition, wxSize( -1,32 ), wxST_NO_AUTORESIZE );
	zone3->Wrap( -1 );
	bSizer48->Add( zone3, 0, wxEXPAND|wxLEFT, 5 );
	
	bSizer46->Add( bSizer48, 1, wxFIXED_MINSIZE, 5 );
	
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText102 = new wxStaticText( m_panel16, wxID_ANY, wxT("View"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText102->Wrap( -1 );
	fgSizer12->Add( m_staticText102, 1, wxALL|wxFIXED_MINSIZE, 5 );
	
	wxString m_choice14Choices[] = { wxT("Central RAM"), wxT("Bank RAM"), wxT("z80 mapping") };
	int m_choice14NChoices = sizeof( m_choice14Choices ) / sizeof( wxString );
	m_choice14 = new wxChoice( m_panel16, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice14NChoices, m_choice14Choices, 0 );
	m_choice14->SetSelection( 0 );
	fgSizer12->Add( m_choice14, 5, wxALL, 5 );
	
	centralColor = new wxColourPickerCtrl( m_panel16, wxID_ANY, wxColour( 255, 255, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer12->Add( centralColor, 1, wxALL|wxFIXED_MINSIZE, 5 );
	
	x = new wxStaticText( m_panel16, wxID_ANY, wxT("Central RAM"), wxDefaultPosition, wxDefaultSize, 0 );
	x->Wrap( -1 );
	fgSizer12->Add( x, 5, wxALL, 5 );
	
	bankColor = new wxColourPickerCtrl( m_panel16, wxID_ANY, wxColour( 64, 191, 64 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer12->Add( bankColor, 1, wxALL|wxFIXED_MINSIZE, 5 );
	
	y = new wxStaticText( m_panel16, wxID_ANY, wxT("Bank RAM"), wxDefaultPosition, wxDefaultSize, 0 );
	y->Wrap( -1 );
	fgSizer12->Add( y, 5, wxALL, 5 );
	
	romColor = new wxColourPickerCtrl( m_panel16, wxID_ANY, wxColour( 191, 64, 64 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer12->Add( romColor, 1, wxALL|wxFIXED_MINSIZE, 5 );
	
	z = new wxStaticText( m_panel16, wxID_ANY, wxT("ROM"), wxDefaultPosition, wxDefaultSize, 0 );
	z->Wrap( -1 );
	fgSizer12->Add( z, 5, wxALL, 5 );
	
	bSizer46->Add( fgSizer12, 0, wxFIXED_MINSIZE, 5 );
	
	bSizer41->Add( bSizer46, 1, wxEXPAND, 5 );
	
	m_staticText106 = new wxStaticText( m_panel16, wxID_ANY, wxT("Double-click somewhere to jump!"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText106->Wrap( -1 );
	bSizer41->Add( m_staticText106, 0, wxALL, 5 );
	
	m_staticline5 = new wxStaticLine( m_panel16, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer41->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );
	
	wxGridSizer* gSizer4;
	gSizer4 = new wxGridSizer( 3, 3, 0, 0 );
	
	m_staticText771 = new wxStaticText( m_panel16, wxID_ANY, wxT("Load symbols from"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText771->Wrap( -1 );
	gSizer4->Add( m_staticText771, 1, wxALL|wxFIXED_MINSIZE, 5 );
	
	m_filePicker9 = new wxFilePickerCtrl( m_panel16, wxID_ANY, wxEmptyString, wxT("Select sjasmplus symbol file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN );
	gSizer4->Add( m_filePicker9, 0, wxALL, 5 );
	
	
	gSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText100 = new wxStaticText( m_panel16, wxID_ANY, wxT("Search (ASCII)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText100->Wrap( -1 );
	gSizer4->Add( m_staticText100, 0, wxALL, 5 );
	
	m_textCtrl73 = new wxTextCtrl( m_panel16, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer4->Add( m_textCtrl73, 0, wxALL, 5 );
	
	m_checkBox23 = new wxCheckBox( m_panel16, wxID_ANY, wxT("in whole RAM"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox23->SetValue(true); 
	gSizer4->Add( m_checkBox23, 0, wxALL, 5 );
	
	m_staticText101 = new wxStaticText( m_panel16, wxID_ANY, wxT("Search (number)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	gSizer4->Add( m_staticText101, 0, wxALL, 5 );
	
	m_textCtrl74 = new wxTextCtrl( m_panel16, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer4->Add( m_textCtrl74, 0, wxALL, 5 );
	
	bSizer41->Add( gSizer4, 0, wxFIXED_MINSIZE, 5 );
	
	m_panel16->SetSizer( bSizer41 );
	m_panel16->Layout();
	bSizer41->Fit( m_panel16 );
	m_notebook4->AddPage( m_panel16, wxT("Overview"), false );
	m_panel12 = new wxPanel( m_notebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxVERTICAL );
	
	m_grid1 = new wxGrid( m_panel12, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	
	// Grid
	m_grid1->CreateGrid( 32, 16 );
	m_grid1->EnableEditing( true );
	m_grid1->EnableGridLines( false );
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
	m_grid1->AutoSizeColumns();
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
	bSizer38->Add( m_grid1, 1, wxEXPAND, 5 );
	
	m_panel12->SetSizer( bSizer38 );
	m_panel12->Layout();
	bSizer38->Fit( m_panel12 );
	m_notebook4->AddPage( m_panel12, wxT("Hex view"), false );
	m_panel13 = new wxPanel( m_notebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer391;
	bSizer391 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_checkList1Choices;
	m_checkList1 = new wxCheckListBox( m_panel13, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_checkList1Choices, 0 );
	m_checkList1->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxT("Courier") ) );
	
	bSizer391->Add( m_checkList1, 1, wxALL|wxEXPAND, 5 );
	
	m_panel13->SetSizer( bSizer391 );
	m_panel13->Layout();
	bSizer391->Fit( m_panel13 );
	m_notebook4->AddPage( m_panel13, wxT("Asm view"), true );
	
	bSizer29->Add( m_notebook4, 1, wxEXPAND | wxALL, 5 );
	
	scrollRAM = new wxScrollBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
	bSizer29->Add( scrollRAM, 0, wxBOTTOM|wxEXPAND|wxRIGHT|wxTOP, 5 );
	
	bSizer26->Add( bSizer29, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText77 = new wxStaticText( this, wxID_ANY, wxT("Jump to"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText77->Wrap( -1 );
	m_staticText77->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer39->Add( m_staticText77, 0, wxALL, 5 );
	
	addressBox = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer39->Add( addressBox, 0, wxALL, 0 );
	
	addressSpin = new wxSpinButton( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer39->Add( addressSpin, 0, wxBOTTOM|wxEXPAND, 10 );
	
	m_button76 = new wxButton( this, wxID_ANY, wxT("PC"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer39->Add( m_button76, 0, wxALL, 5 );
	
	m_button77 = new wxButton( this, wxID_ANY, wxT("SP"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer39->Add( m_button77, 0, wxALL, 5 );
	
	wxString ChoiceLabelsChoices[] = { wxT("(no symbol table loaded)") };
	int ChoiceLabelsNChoices = sizeof( ChoiceLabelsChoices ) / sizeof( wxString );
	ChoiceLabels = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoiceLabelsNChoices, ChoiceLabelsChoices, 0 );
	ChoiceLabels->SetSelection( 0 );
	bSizer39->Add( ChoiceLabels, 0, wxALL, 5 );
	
	bSizer26->Add( bSizer39, 0, 0, 5 );
	
	this->SetSizer( bSizer26 );
	this->Layout();
	
	// Connect Events
	overviewPanel->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( Memory::onOverviewLeftDClick ), NULL, this );
	overviewPanel->Connect( wxEVT_PAINT, wxPaintEventHandler( Memory::UpdateOverview ), NULL, this );
	m_choice14->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Memory::onChangeView ), NULL, this );
	m_filePicker9->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( Memory::LoadSymbolTable ), NULL, this );
	m_textCtrl73->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( Memory::onSearchASCII ), NULL, this );
	m_textCtrl74->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( Memory::onSearchNumber ), NULL, this );
	m_checkList1->Connect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( Memory::onBreakpoint ), NULL, this );
	scrollRAM->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	addressBox->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( Memory::AddressEntered ), NULL, this );
	addressSpin->Connect( wxEVT_SCROLL_THUMBTRACK, wxSpinEventHandler( Memory::JumpToAddress ), NULL, this );
	m_button76->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Memory::JumpToPC ), NULL, this );
	m_button77->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Memory::JumpToSP ), NULL, this );
	ChoiceLabels->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Memory::JumpToSymbol ), NULL, this );
}

Memory::~Memory()
{
	// Disconnect Events
	overviewPanel->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( Memory::onOverviewLeftDClick ), NULL, this );
	overviewPanel->Disconnect( wxEVT_PAINT, wxPaintEventHandler( Memory::UpdateOverview ), NULL, this );
	m_choice14->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Memory::onChangeView ), NULL, this );
	m_filePicker9->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( Memory::LoadSymbolTable ), NULL, this );
	m_textCtrl73->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( Memory::onSearchASCII ), NULL, this );
	m_textCtrl74->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( Memory::onSearchNumber ), NULL, this );
	m_checkList1->Disconnect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( Memory::onBreakpoint ), NULL, this );
	scrollRAM->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	scrollRAM->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( Memory::RefreshMem ), NULL, this );
	addressBox->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( Memory::AddressEntered ), NULL, this );
	addressSpin->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxSpinEventHandler( Memory::JumpToAddress ), NULL, this );
	m_button76->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Memory::JumpToPC ), NULL, this );
	m_button77->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Memory::JumpToSP ), NULL, this );
	ChoiceLabels->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Memory::JumpToSymbol ), NULL, this );
	
}

IDE::IDE( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar2 = new wxMenuBar( 0 );
	m_menu6 = new wxMenu();
	wxMenuItem* m_menuItemNew;
	m_menuItemNew = new wxMenuItem( m_menu6, wxID_ANY, wxString( wxT("New") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItemNew );
	
	wxMenuItem* m_menuItemOpen;
	m_menuItemOpen = new wxMenuItem( m_menu6, wxID_ANY, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItemOpen );
	
	wxMenuItem* m_menuItemSave;
	m_menuItemSave = new wxMenuItem( m_menu6, wxID_ANY, wxString( wxT("Save") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItemSave );
	
	wxMenuItem* m_menuItemSaveAs;
	m_menuItemSaveAs = new wxMenuItem( m_menu6, wxID_ANY, wxString( wxT("Save as") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItemSaveAs );
	
	wxMenuItem* m_menuItemClose;
	m_menuItemClose = new wxMenuItem( m_menu6, wxID_ANY, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItemClose );
	
	wxMenuItem* m_separator8;
	m_separator8 = m_menu6->AppendSeparator();
	
	wxMenuItem* m_menuItemExit;
	m_menuItemExit = new wxMenuItem( m_menu6, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItemExit );
	
	m_menubar2->Append( m_menu6, wxT("File") ); 
	
	m_menu10 = new wxMenu();
	m_menubar2->Append( m_menu10, wxT("Edit") ); 
	
	m_menu11 = new wxMenu();
	wxMenuItem* m_menuAssemble;
	m_menuAssemble = new wxMenuItem( m_menu11, wxID_ANY, wxString( wxT("Assemble") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu11->Append( m_menuAssemble );
	
	wxMenuItem* m_menuItem58;
	m_menuItem58 = new wxMenuItem( m_menu11, wxID_ANY, wxString( wxT("Run") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu11->Append( m_menuItem58 );
	
	wxMenuItem* m_separator9;
	m_separator9 = m_menu11->AppendSeparator();
	
	wxMenuItem* m_menuItem59;
	m_menuItem59 = new wxMenuItem( m_menu11, wxID_ANY, wxString( wxT("Symbols") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu11->Append( m_menuItem59 );
	
	m_menubar2->Append( m_menu11, wxT("Assemble") ); 
	
	this->SetMenuBar( m_menubar2 );
	
	m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( m_menuItemNew->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::OnNew ) );
	this->Connect( m_menuItemOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::onOpen ) );
	this->Connect( m_menuItemSave->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::onSave ) );
	this->Connect( m_menuItemSaveAs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::onSaveAs ) );
	this->Connect( m_menuItemClose->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::onClose ) );
	this->Connect( m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::onExit ) );
	this->Connect( m_menuAssemble->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::OnAssemble ) );
}

IDE::~IDE()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::OnNew ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::onOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::onSave ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::onSaveAs ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::onClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::onExit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IDE::OnAssemble ) );
	
}

DiscEditor::DiscEditor( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText97 = new wxStaticText( this, wxID_ANY, wxT("Tracks"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText97->Wrap( -1 );
	bSizer45->Add( m_staticText97, 0, wxALL, 5 );
	
	trackCount = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	trackCount->Enable( false );
	
	bSizer45->Add( trackCount, 0, wxALL, 5 );
	
	m_staticText98 = new wxStaticText( this, wxID_ANY, wxT("Sides"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText98->Wrap( -1 );
	bSizer45->Add( m_staticText98, 0, wxALL, 5 );
	
	sideCount = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sideCount->Enable( false );
	
	bSizer45->Add( sideCount, 0, wxALL, 5 );
	
	bSizer2->Add( bSizer45, 0, 0, 5 );
	
	DiskEd_Mode_Tabs = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel17 = new wxPanel( DiskEd_Mode_Tabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer44;
	bSizer44 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer49;
	bSizer49 = new wxBoxSizer( wxHORIZONTAL );
	
	wxString m_choice15Choices[] = { wxT("Side A"), wxT("Side B") };
	int m_choice15NChoices = sizeof( m_choice15Choices ) / sizeof( wxString );
	m_choice15 = new wxChoice( m_panel17, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice15NChoices, m_choice15Choices, 0 );
	m_choice15->SetSelection( 0 );
	bSizer49->Add( m_choice15, 0, wxALL, 5 );
	
	m_staticText119 = new wxStaticText( m_panel17, wxID_ANY, wxT(" Normal sector "), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_staticText119->Wrap( -1 );
	m_staticText119->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	bSizer49->Add( m_staticText119, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText120 = new wxStaticText( m_panel17, wxID_ANY, wxT(" Weak "), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_staticText120->Wrap( -1 );
	m_staticText120->SetBackgroundColour( wxColour( 255, 0, 0 ) );
	
	bSizer49->Add( m_staticText120, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText121 = new wxStaticText( m_panel17, wxID_ANY, wxT(" Erased "), wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_staticText121->Wrap( -1 );
	m_staticText121->SetBackgroundColour( wxColour( 0, 0, 255 ) );
	
	bSizer49->Add( m_staticText121, 0, wxALL|wxEXPAND, 5 );
	
	bSizer44->Add( bSizer49, 0, wxEXPAND, 5 );
	
	explorerPanel = new wxPanel( m_panel17, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	explorerPanel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 91, false, wxT("Tahoma") ) );
	
	bSizer44->Add( explorerPanel, 1, wxEXPAND | wxALL, 5 );
	
	m_panel17->SetSizer( bSizer44 );
	m_panel17->Layout();
	bSizer44->Fit( m_panel17 );
	DiskEd_Mode_Tabs->AddPage( m_panel17, wxT("Explorer"), true );
	m_panel1 = new wxPanel( DiskEd_Mode_Tabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer50;
	bSizer50 = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBox1 = new wxCheckBox( m_panel1, wxID_ANY, wxT("Show System files"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer50->Add( m_checkBox1, 0, wxALL, 5 );
	
	m_checkBox2 = new wxCheckBox( m_panel1, wxID_ANY, wxT("Add/Remove Amsdos Header"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer50->Add( m_checkBox2, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer50, 0, wxEXPAND, 5 );
	
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
	DiskEd_Mode_Tabs->AddPage( m_panel1, wxT("File System"), false );
	m_panel2 = new wxPanel( DiskEd_Mode_Tabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxVERTICAL );
	
	Track = new wxStaticText( m_panel2, wxID_ANY, wxT("Track"), wxDefaultPosition, wxDefaultSize, 0 );
	Track->Wrap( -1 );
	bSizer43->Add( Track, 0, wxALL, 5 );
	
	spinTrack = new wxSpinCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 0, 255, 0 );
	bSizer43->Add( spinTrack, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText96 = new wxStaticText( m_panel2, wxID_ANY, wxT("Sector"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText96->Wrap( -1 );
	bSizer43->Add( m_staticText96, 0, wxALL, 5 );
	
	lb_sectors = new wxListBox( m_panel2, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, 0 ); 
	m_menu9 = new wxMenu();
	wxMenuItem* m_menuItem61;
	m_menuItem61 = new wxMenuItem( m_menu9, wxID_ANY, wxString( wxT("Cut") ) + wxT('\t') + wxT("CTRL+X"), wxEmptyString, wxITEM_NORMAL );
	m_menu9->Append( m_menuItem61 );
	
	wxMenuItem* m_menuItem62;
	m_menuItem62 = new wxMenuItem( m_menu9, wxID_ANY, wxString( wxT("Copy") ) + wxT('\t') + wxT("CTRL+C"), wxEmptyString, wxITEM_NORMAL );
	m_menu9->Append( m_menuItem62 );
	
	wxMenuItem* m_menuItem63;
	m_menuItem63 = new wxMenuItem( m_menu9, wxID_ANY, wxString( wxT("Paste") ) + wxT('\t') + wxT("CTRL+V"), wxEmptyString, wxITEM_NORMAL );
	m_menu9->Append( m_menuItem63 );
	
	wxMenuItem* m_menuItem64;
	m_menuItem64 = new wxMenuItem( m_menu9, wxID_ANY, wxString( wxT("MyMenuItem") ) + wxT('\t') + wxT("Delete"), wxT("DEL"), wxITEM_NORMAL );
	m_menu9->Append( m_menuItem64 );
	
	wxMenuItem* m_menuItem65;
	m_menuItem65 = new wxMenuItem( m_menu9, wxID_ANY, wxString( wxT("Change ID") ) + wxT('\t') + wxT("CTRL+I"), wxEmptyString, wxITEM_NORMAL );
	m_menu9->Append( m_menuItem65 );
	
	lb_sectors->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( DiscEditor::lb_sectorsOnContextMenu ), NULL, this ); 
	
	bSizer43->Add( lb_sectors, 1, wxALL|wxEXPAND, 5 );
	
	st_size = new wxStaticText( m_panel2, wxID_ANY, wxT("Size"), wxDefaultPosition, wxDefaultSize, 0 );
	st_size->Wrap( -1 );
	bSizer43->Add( st_size, 0, wxLEFT|wxTOP, 5 );
	
	st_weak = new wxStaticText( m_panel2, wxID_ANY, wxT("sane"), wxDefaultPosition, wxDefaultSize, 0 );
	st_weak->Wrap( -1 );
	bSizer43->Add( st_weak, 0, wxLEFT|wxTOP, 5 );
	
	st_erased = new wxStaticText( m_panel2, wxID_ANY, wxT("erased"), wxDefaultPosition, wxDefaultSize, 0 );
	st_erased->Wrap( -1 );
	bSizer43->Add( st_erased, 0, wxALL, 5 );
	
	bSizer5->Add( bSizer43, 1, wxEXPAND, 5 );
	
	tc_sectordata = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	tc_sectordata->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxEmptyString ) );
	
	bSizer5->Add( tc_sectordata, 4, wxALL|wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer5 );
	m_panel2->Layout();
	bSizer5->Fit( m_panel2 );
	DiskEd_Mode_Tabs->AddPage( m_panel2, wxT("Sector Editor"), false );
	
	bSizer2->Add( DiskEd_Mode_Tabs, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	explorerPanel->Connect( wxEVT_PAINT, wxPaintEventHandler( DiscEditor::drawSectorExplorer ), NULL, this );
	spinTrack->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( DiscEditor::setTrack ), NULL, this );
	lb_sectors->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DiscEditor::setSector ), NULL, this );
	lb_sectors->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( DiscEditor::sectorLeftClick ), NULL, this );
	this->Connect( m_menuItem61->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DiscEditor::cutSector ) );
	this->Connect( m_menuItem62->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DiscEditor::copySector ) );
	this->Connect( m_menuItem63->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DiscEditor::pasteSector ) );
	this->Connect( m_menuItem64->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DiscEditor::deleteSector ) );
	this->Connect( m_menuItem65->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DiscEditor::renameSector ) );
}

DiscEditor::~DiscEditor()
{
	// Disconnect Events
	explorerPanel->Disconnect( wxEVT_PAINT, wxPaintEventHandler( DiscEditor::drawSectorExplorer ), NULL, this );
	spinTrack->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( DiscEditor::setTrack ), NULL, this );
	lb_sectors->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DiscEditor::setSector ), NULL, this );
	lb_sectors->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( DiscEditor::sectorLeftClick ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DiscEditor::cutSector ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DiscEditor::copySector ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DiscEditor::pasteSector ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DiscEditor::deleteSector ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DiscEditor::renameSector ) );
	
	delete m_menu9; 
}
