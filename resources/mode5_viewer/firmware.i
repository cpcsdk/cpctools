; ---------------------------------------------------------------------------
; Definiciones del Firmware del CPC / (c) SyX 2009
; ---------------------------------------------------------------------------

; ---------------------------------------------------------------------------
; Firmware Jump block

; LOW KERNEL JUMBLOCK
RESET_ENTRY                     EQU $0000   ; RST $00
LOW_JUMP                        EQU $0008   ; RST $08
KL_LOW_PCHL                     EQU $000B
PCBC                            EQU $000E
SIDE_CALL                       EQU $0010   ; RST $10
KL_SIDE_PCHL                    EQU $0013
PCDE                            EQU $0016
FAR_CALL                        EQU $0018   ; RST $18
KL_FAR_PCHL                     EQU $001B
PCHL                            EQU $001E
RAM_LAM                         EQU $0020   ; RST $20
KL_FAR_CALL                     EQU $0023
FIRM_JUMP                       EQU $0028   ; RST $28
USER_RESTART                    EQU $0030   ; RST $30
INTERRUPT_ENTRY                 EQU $0038   ; RST $38
EXT_INTERRUPT                   EQU $003B

; HIGH KERNEL JUMPBLOCK
KL_U_ROM_ENABLE                 EQU $B900
KL_U_ROM_DISABLE                EQU $B903
KL_L_ROM_ENABLE                 EQU $B906
KL_L_ROM_DISABLE                EQU $B909
KL_ROM_RESTORE                  EQU $B90C
KL_ROM_SELECT                   EQU $B90F
KL_CURR_SELECTION               EQU $B912
KL_PROBE_ROM                    EQU $B915
KL_ROM_DESELECT                 EQU $B918
KL_LDIR                         EQU $B91B
KL_LDDR                         EQU $B91E
KL_POLL_SYNCHRONOUS             EQU $B921
KL_SCAN_NEEDED                  EQU $B92A

; The Key Manager
KM_INITIALISE                   EQU $BB00
KM_RESET                        EQU $BB03
KM_WAIT_CHAR                    EQU $BB06
KM_READ_CHAR                    EQU $BB09
KM_CHAR_RETURN                  EQU $BB0C
KM_SET_EXPAND                   EQU $BB0F
KM_GET_EXPAND                   EQU $BB12
KM_EXP_BUFFER                   EQU $BB15
KM_WAIT_KEY                     EQU $BB18
KM_READ_KEY                     EQU $BB1B
KM_TEST_KEY                     EQU $BB1E
KM_GET_STATE                    EQU $BB21
KM_GET_JOYSTICK                 EQU $BB24
KM_SET_TRANSLATE                EQU $BB27
KM_GET_TRANSLATE                EQU $BB2A
KM_SET_SHIFT                    EQU $BB2D
KM_GET_SHIFT                    EQU $BB30
KM_SET_CONTROL                  EQU $BB33
KM_GET_CONTROL                  EQU $BB36
KM_SET_REPEAT                   EQU $BB39
KM_GET_REPEAT                   EQU $BB3C
KM_SET_DELAY                    EQU $BB3F
KM_GET_DELAY                    EQU $BB42
KM_ARM_BREAKS                   EQU $BB45
KM_DISARM_BREAK                 EQU $BB48
KM_BREAK_EVENT                  EQU $BB4B

; The Text VDU
TXT_INITIALISE                  EQU $BB4E
TXT_RESET                       EQU $BB51
TXT_VDU_ENABLE                  EQU $BB54
TXT_VDU_DISABLE                 EQU $BB57
TXT_OUTPUT                      EQU $BB5A
TXT_WR_CHAR                     EQU $BB5D
TXT_RD_CHAR                     EQU $BB60
TXT_SET_GRAPHIC                 EQU $BB63
TXT_WIN_ENABLE                  EQU $BB66
TXT_GET_WINDOW                  EQU $BB69
TXT_CLEAR_WINDOW                EQU $BB6C
TXT_SET_COLUMN                  EQU $BB6F
TXT_SET_ROW                     EQU $BB72
TXT_SET_CURSOR                  EQU $BB75
TXT_GET_CURSOR                  EQU $BB78
TXT_CUR_ENABLE                  EQU $BB7B
TXT_CUR_DISABLE                 EQU $BB7E
TXT_CUR_ON                      EQU $BB81
TXT_CUR_OFF                     EQU $BB84
TXT_VALIDATE                    EQU $BB87
TXT_PLACE_CURSOR                EQU $BB8A
TXT_REMOVE_CURSOR               EQU $BB8D
TXT_SET_PEN                     EQU $BB90
TXT_GET_PEN                     EQU $BB93
TXT_SET_PAPER                   EQU $BB96
TXT_GET_PAPER                   EQU $BB99
TXT_INVERSE                     EQU $BB9C
TXT_SET_BACK                    EQU $BB9F
TXT_GET_BACK                    EQU $BBA2
TXT_GET_MATRIX                  EQU $BBA5
TXT_SET_MATRIX                  EQU $BBA8
TXT_SET_M_TABLE                 EQU $BBAB
TXT_GET_M_TABLE                 EQU $BBAE
TXT_GET_CONTROLS                EQU $BBB1
TXT_STR_SELECT                  EQU $BBB4
TXT_SWAP_STREAMS                EQU $BBB7

; The Graphics VDU
GRA_INITIALISE                  EQU $BBBA
GRA_RESET                       EQU $BBBD
GRA_MOVE_ABSOLUTE               EQU $BBC0
GRA_MOVE_RELATIVE               EQU $BBC3
GRA_ASK_CURSOR                  EQU $BBC6
GRA_SET_ORIGIN                  EQU $BBC9
GRA_GET_ORIGIN                  EQU $BBCC
GRA_WIN_WIDTH                   EQU $BBCF
GRA_WIN_HEIGHT                  EQU $BBD2
GRA_GET_W_WIDTH                 EQU $BBD5
GRA_GET_W_HEIGHT                EQU $BBD8
GRA_CLEAR_WINDOW                EQU $BBDB
GRA_SET_PEN                     EQU $BBDE
GRA_GET_PEN                     EQU $BBE1
GRA_SET_PAPER                   EQU $BBE4
GRA_GET_PAPER                   EQU $BBE7
GRA_PLOT_ABSOLUTE               EQU $BBEA
GRA_PLOT_RELATIVE               EQU $BBED
GRA_TEST_ABSOLUTE               EQU $BBF0
GRA_TEST_RELATIVE               EQU $BBF3
GRA_LINE_ABSOLUTE               EQU $BBF6
GRA_LINE_RELATIVE               EQU $BBF9
GRA_WR_CHAR                     EQU $BBFC

; the Screen Pack
SCR_INITIALISE                  EQU $BBFF
SCR_RESET                       EQU $BC02
SCR_SET_OFFSET                  EQU $BC05
SCR_SET_BASE                    EQU $BC08
SCR_GET_LOCATION                EQU $BC0B
SCR_SET_MODE                    EQU $BC0E
SCR_GET_MODE                    EQU $BC11
SCR_CLEAR                       EQU $BC14
SCR_CHAR_LIMITS                 EQU $BC17
SCR_CHAR_POSITION               EQU $BC1A
SCR_DOT_POSITION                EQU $BC1D
SCR_NEXT_BYTE                   EQU $BC20
SCR_PREV_BYTE                   EQU $BC23
SCR_NEXT_LINE                   EQU $BC26
SCR_PREV_LINE                   EQU $BC29
SCR_INK_ENCODE                  EQU $BC2C
SCR_INK_DECODE                  EQU $BC2F
SCR_SET_INK                     EQU $BC32
SCR_GET_INK                     EQU $BC35
SCR_SET_BORDER                  EQU $BC38
SCR_GET_BORDER                  EQU $BC3B
SCR_SET_FLASHING                EQU $BC3E
SCR_GET_FLASHING                EQU $BC41
SCR_FILL_BOX                    EQU $BC44
SCR_FLOOD_BOX                   EQU $BC47
SCR_CHAR_INVERT                 EQU $BC4A
SCR_HW_ROLL                     EQU $BC4D
SCR_SW_ROLL                     EQU $BC50
SCR_UNPACK                      EQU $BC53
SCR_REPACK                      EQU $BC56
SCR_ACCESS                      EQU $BC59
SCR_PIXELS                      EQU $BC5C
SCR_HORIZONTAL                  EQU $BC5F
SCR_VERTICAL                    EQU $BC62

; The Cassette/AmsDOS Manager
CAS_INITIALISE                  EQU $BC65
CAS_SET_SPEED                   EQU $BC68
CAS_NOISY                       EQU $BC6B
CAS_START_MOTOR                 EQU $BC6E
CAS_STOP_MOTOR                  EQU $BC71
CAS_RESTORE_MOTOR               EQU $BC74
CAS_IN_OPEN                     EQU $BC77
CAS_IN_CLOSE                    EQU $BC7A
CAS_IN_ABANDON                  EQU $BC7D
CAS_IN_CHAR                     EQU $BC80
CAS_IN_DIRECT                   EQU $BC83
CAS_RETURN                      EQU $BC86
CAS_TEST_EOF                    EQU $BC89
CAS_OUT_OPEN                    EQU $BC8C
CAS_OUT_CLOSE                   EQU $BC8F
CAS_OUT_ABANDON                 EQU $BC92
CAS_OUT_CHAR                    EQU $BC95
CAS_OUT_DIRECT                  EQU $BC98
CAS_CATALOG                     EQU $BC9B
CAS_WRITE                       EQU $BC9E
CAS_READ                        EQU $BCA1
CAS_CHECK                       EQU $BCA4

; The Sound Manager
SOUND_RESET                     EQU $BCA7
SOUND_QUEUE                     EQU $BCAA
SOUND_CHECK                     EQU $BCAD
SOUND_ARM_EVENT                 EQU $BCB0
SOUND_RELEASE                   EQU $BCB3
SOUND_HOLD                      EQU $BCB6
SOUND_CONTINUE                  EQU $BCB9
SOUND_AMPL_ENVELOPE             EQU $BCBC
SOUND_TONE_ENVELOPE             EQU $BCBF
SOUND_A_ADDRESS                 EQU $BCC2
SOUND_T_ADDRESS                 EQU $BCC5

; The Kernel
KL_CHOKE_OFF                    EQU $BCC8
KL_ROM_WALK                     EQU $BCCB
KL_INIT_BACK                    EQU $BCCE
KL_LOG_EXT                      EQU $BCD1
KL_FIND_COMMAND                 EQU $BCD4
KL_NEW_FRAME_FLY                EQU $BCD7
KL_ADD_FRAME_FLY                EQU $BCDA
KL_DEL_FRAME_FLY                EQU $BCDD
KL_NEW_FAST_TICKER              EQU $BCE0
KL_ADD_FAST_TICKER              EQU $BCE3
KL_DEL_FAST_TICKER              EQU $BCE6
KL_ADD_TICKER                   EQU $BCE9
KL_DEL_TICKER                   EQU $BCEC
KL_INIT_EVENT                   EQU $BCEF
KL_EVENT                        EQU $BCF2
KL_SYNC_RESET                   EQU $BCF5
KL_DEL_SYNCHRONOUS              EQU $BCF8
KL_NEXT_SYNCH                   EQU $BCFB
KL_DO_SYNCH                     EQU $BCFE
KL_DONE_SYNCH                   EQU $BD01
KL_EVENT_DISABLE                EQU $BD04
KL_EVENT_ENABLE                 EQU $BD07
KL_DISARM_EVENT                 EQU $BD0A
KL_TIME_PLEASE                  EQU $BD0D
KL_TIME_SET                     EQU $BD10

; The Machine Pack
MC_BOOT_PROGRAM                 EQU $BD13
MC_START_PROGRAM                EQU $BD16
MC_WAIT_FLYBACK                 EQU $BD19
MC_SET_MODE                     EQU $BD1C
MC_SCREEN_OFFSET                EQU $BD1F
MC_CLEAR_INKS                   EQU $BD22
MC_SET_INKS                     EQU $BD25
MC_RESET_PRINTER                EQU $BD28
MC_PRINT_CHAR                   EQU $BD2B
MC_PRINTER_BUSY                 EQU $BD2E
MC_SEND_PRINTER                 EQU $BD31
MC_SOUND_REGISTER               EQU $BD34
JUMP_RESTORE                    EQU $BD37
KM_SET_LOCKS                    EQU $BD3A
TXT_ASK_STATE                   EQU $BD40
GRA_DEFAULT                     EQU $BD43
GRA_SET_BACK                    EQU $BD46
GRA_SET_FIRST                   EQU $BD49
GRA_SET_LINE_MASK               EQU $BD4C
GRA_FROM_USER                   EQU $BD4F
GRA_FILL                        EQU $BD52
SCR_SET_POSITION                EQU $BD55
MC_PRINT_TRANSLATION            EQU $BD58
KL_BANK_SWITCH                  EQU $BD5B

; The Firmware Indirections
IND_TXT_DRAW_CURSOR             EQU $BDCD
IND_TXT_UNDRAW_CURSOR           EQU $BDD0
IND_TXT_WRITE_CHAR              EQU $BDD3
IND_TXT_UNWRITE                 EQU $BDD6
IND_TXT_OUT_ACTION              EQU $BDD9
IND_GRA_PLOT                    EQU $BDDC
IND_GRA_TEST                    EQU $BDDF
IND_GRA_LINE                    EQU $BDE2
IND_SCR_READ                    EQU $BDE5
IND_SCR_WRITE                   EQU $BDE8
IND_SCR_MODE_CLEAR              EQU $BDEB
IND_KM_TEST_BREAK               EQU $BDEE
IND_MC_WAIT_PRINTER             EQU $BDF1
IND_KM_SCAN_KEYS                EQU $BDF4

; The Maths Firmware
MOVE_REAL                       EQU $BD61
INTEGER_TO_REAL                 EQU $BD64
BINARY_TO_REAL                  EQU $BD67
REAL_TO_INTEGER                 EQU $BD6A
REAL_TO_BINARY                  EQU $BD6D
REAL_FIX                        EQU $BD70
REAL_INT                        EQU $BD73
INTERNAL_SUBROUTINE             EQU $BD76
REAL_x10POWA                    EQU $BD79
REAL_ADDITION                   EQU $BD7C
REAL_REVERSE_SUBTRACTION        EQU $BD82
REAL_MULTIPLICATION             EQU $BD85
REAL_DIVISION                   EQU $BD88
REAL_COMPARISON                 EQU $BD8E
REAL_UNARY_MINUS                EQU $BD91
REAL_SIGNUM_SGN                 EQU $BD94
SET_ANGLE_MODE                  EQU $BD97
REAL_PI                         EQU $BD9A
REAL_SQR                        EQU $BD9D
REAL_POWER                      EQU $BDA0
REAL_LOG                        EQU $BDA3
REAL_LOG_10                     EQU $BDA6
REAL_EXP                        EQU $BDA9
REAL_SINE                       EQU $BDAC
REAL_COSINE                     EQU $BDAF
REAL_TANGENT                    EQU $BDB2
REAL_ARCTANGENT                 EQU $BDB5

; Maths Routines for the 464 only
REAL_SUBTRACTION                EQU $BD5B
REAL_EXPONENT_ADDITION          EQU $BD67
INTEGER_ADDITION                EQU $BDAC
INTEGER_SUBTRACTION             EQU $BDAF
INTEGER_REVERSE_SUBTRACTION     EQU $BDB2
INTEGER_MULTIPLICATION          EQU $BDB5
INTEGER_DIVISION                EQU $BDB8
INTEGER_DIVISION_2              EQU $BDBB
INTEGER_COMPARISON              EQU $BDC4
INTEGER_UNARY_MINUS             EQU $BDC7
INTEGER_SIGNUM_SGN              EQU $BDCA

; Subroutines for the 664 and 6128 only
TEXT_INPUT                      EQU $BD5E
REAL_RND                        EQU $BD7F
REAL_RND0                       EQU $BD8B

; AMSDOS and BIOS Firmware
BIOS_SET_MESSAGE                EQU $C033
BIOS_SETUP_DISC                 EQU $C036
BIOS_SELECT_FORMAT              EQU $C039
BIOS_READ_SECTOR                EQU $C03C
BIOS_WRITE_SECTOR               EQU $C03F
BIOS_FORMAT_TRACK               EQU $C042
BIOS_MOVE_TRACK                 EQU $C045
BIOS_GET_STATUS                 EQU $C048
BIOS_SET_RETRY_COUNT            EQU $C04B
GET_SECTOR_DATA                 EQU $C56C

; ---------------------------------------------------------------------------
; System Variables

; Related to AMSDOS
AMSDOS_RESERVED_AREA            EQU $BE7D
