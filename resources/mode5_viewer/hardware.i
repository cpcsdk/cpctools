; ---------------------------------------------------------------------------
; Definiciones del Hardware del CPC / (c) SyX 2009
; ---------------------------------------------------------------------------

; ---------------------------------------------------------------------------
; Constantes del sistema
; ---------------------------------------------------------------------------
; Limites del hardware de video
MAX_SCR_WIDTH       EQU 384     ; Ancho Visualizable del CPC 384 = (4 + 40 + 4) * 8
;MAX_SCR_HEIGHT      EQU 270     ; Alto Visualizable del CPC  270 = (4,375 + 25 + 4,375) * 8  // 0,375 = 3 líneas 
MAX_SCR_HEIGHT      EQU 272     ; Alto Visualizable del CPC  270 = (4,5 + 25 + 4,5) * 8

; ---------------------------------------------------------------------------
; Comienzos de las páginas de RAM del sistema
RAM_P0              EQU $0000
RAM_P1              EQU $4000
RAM_P2              EQU $8000
RAM_P3              EQU $C000

; ---------------------------------------------------------------------------
; Gate Array
; ---------------------------------------------------------------------------
; Puertos de Acceso
GATE_ARRAY          EQU $7F00

; ---------------------------------------------------------------------------
; Modos de operación del Gate Array
PEN                 EQU %00000000
INK                 EQU %01000000
MODE_Y_ROM          EQU %10000000
MODE                EQU %10000000
ROM                 EQU %10000000
INTERRUPT           EQU %10000000
RAM                 EQU %11000000

; ---------------------------------------------------------------------------
; Selección de Pluma
BORDER              EQU %00010000
PEN_00              EQU %00000000
PEN_01              EQU %00000001
PEN_02              EQU %00000010
PEN_03              EQU %00000011
PEN_04              EQU %00000100
PEN_05              EQU %00000101
PEN_06              EQU %00000110
PEN_07              EQU %00000111
PEN_08              EQU %00001000
PEN_09              EQU %00001001
PEN_0A              EQU %00001010
PEN_0B              EQU %00001011
PEN_0C              EQU %00001100
PEN_0D              EQU %00001101
PEN_0E              EQU %00001110
PEN_0F              EQU %00001111

; ---------------------------------------------------------------------------
; Tintas
BLANCO              EQU $00     ; White 
;BLANCO             EQU $01     ; White (no existe oficialmente) 
VERDE_MAR           EQU $02     ; Sea Green 
AMARILLO_PASTEL     EQU $03     ; Pastel Yellow 
AZUL                EQU $04     ; Blue 
PURPURA             EQU $05     ; Purple 
CIAN                EQU $06     ; Cyan 
ROSA                EQU $07     ; Pink 
;PURPURA            EQU $08     ; Purple (no existe oficialmente) 
;AMARILLO_PASTEL    EQU $09     ; Pastel Yellow (no existe oficialmente) 
AMARILLO_BRILLANTE  EQU $0A     ; Bright Yellow 
BLANCO_BRILLANTE    EQU $0B     ; Bright White 
ROJO_BRILLANTE      EQU $0C     ; Bright Red 
MAGENTA_BRILLANTE   EQU $0D     ; Bright Magenta 
NARANJA             EQU $0E     ; Orange 
MAGENTA_PASTEL      EQU $0F     ; Pastel Magenta 
;AZUL               EQU $10     ; Blue (no existe oficialmente) 
;VERDE_MAR          EQU $11     ; Sea Green (no existe oficialmente) 
VERDE_BRILLANTE     EQU $12     ; Bright Green 
CIAN_BRILLANTE      EQU $13     ; Bright Cyan 
NEGRO               EQU $14     ; Black 
AZUL_BRILLANTE      EQU $15     ; Bright Blue 
VERDE               EQU $16     ; Green 
AZUL_CIELO          EQU $17     ; Sky Blue 
MAGENTA             EQU $18     ; Magenta 
VERDE_PASTEL        EQU $19     ; Pastel Green 
LIMA                EQU $1A     ; Lime 
CIAN_PASTEL         EQU $1B     ; Pastel Cyan 
ROJO                EQU $1C     ; Red 
MALVA               EQU $1D     ; Mauve 
AMARILLO            EQU $1E     ; Yellow 
AZUL_PASTEL         EQU $1F     ; Pastel Blue 

; ---------------------------------------------------------------------------
; Modos de pantalla
MODE_0              EQU $00
MODE_1              EQU $01
MODE_2              EQU $02
MODE_3              EQU $03

; ---------------------------------------------------------------------------
; ROM
ROM_OFF             EQU %00001100
UPPER_OFF           EQU %00001000
UPPER_ON            EQU %00000000
LOWER_OFF           EQU %00000100
LOWER_ON            EQU %00000000

; ---------------------------------------------------------------------------
; RASTER 52 DIVIDER
CLEAR_RASTER_DIV    EQU %00010000

; ---------------------------------------------------------------------------
; CRTC
; ---------------------------------------------------------------------------
; Puertos de acceso
CRTC_SELECT         EQU $BC00
CRTC_WRITE          EQU $BD00
CRTC_STATUS         EQU $BE00
CRTC_READ           EQU $BF00

; ---------------------------------------------------------------------------
; Registros del CRTC
REG_00              EQU $00     ; Horizontal Total
REG_01              EQU $01     ; Horizontal Displayed
REG_02              EQU $02     ; Horizontal Sync Position
REG_03              EQU $03     ; VSYNC, HSYNC Widths
REG_04              EQU $04     ; Vertical Total
REG_05              EQU $05     ; Vertical Total Adjust
REG_06              EQU $06     ; Vertical Displayed
REG_07              EQU $07     ; Vertical Sync Position
REG_08              EQU $08     ; Mode Control
REG_09              EQU $09     ; Scan Line
REG_0A              EQU $0A     ; Cursor Start
REG_0B              EQU $0B     ; Cursor End
REG_0C              EQU $0C     ; Display Start Addr (H)
REG_0D              EQU $0D     ; Display Start Addr (L)
REG_0E              EQU $0E     ; Cursor Position (H)
REG_0F              EQU $0F     ; Cursor Position (L)
REG_10              EQU $10     ; Light Pen Reg (H)
REG_11              EQU $11     ; Light Pen Reg (L)

; REG_12 %xxDD MMxx xxxx xxxx
VRAM16              EQU $00     ; 16 KBs de VRAM
VRAM32              EQU $0C     ; 32 KBs de VRAM
VRAM_P0             EQU $00     ; VRAM en $0000 ... $3FFF
VRAM_P1             EQU $10     ; VRAM en $4000 ... $7FFF
VRAM_P2             EQU $20     ; VRAM en $8000 ... $BFFF
VRAM_P3             EQU $30     ; VRAM en $C000 ... $FFFF

VRAM_BLOCK_SIZE     EQU $0800   ; 2 KBs

; ---------------------------------------------------------------------------
; PPI
; ---------------------------------------------------------------------------
; Puertos de acceso
PPI_A               EQU $F400
PPI_B               EQU $F500
PPI_C               EQU $F600
PPI_CONTROL         EQU $F700

; ---------------------------------------------------------------------------
; Constantes relacionadas con la unidad de cinta
TAPE_MOTOR_ON       EQU %00010000
TAPE_MOTOR_OFF      EQU %00000000
TAPE_WRITE_DATA     EQU %00100000
TAPE_READ_DATA      EQU %10000000

; ---------------------------------------------------------------------------
; Líneas del teclado asociadas a los joysticks
JOY0                EQU %01001001   ; Para el joy0 es la 9
JOY1                EQU %01000110   ; Para el joy1 es la 6

; ---------------------------------------------------------------------------
; Direcciones del Joystick
; Bits
JOY_UP              EQU 0
JOY_DOWN            EQU 1
JOY_LEFT            EQU 2
JOY_RIGHT           EQU 3
JOY_FIRE1           EQU 4
JOY_FIRE2           EQU 5

; Direcciones para el JOY0
J0_UP               EQU 0
J0_DOWN             EQU 1
J0_LEFT             EQU 2
J0_RIGHT            EQU 3
J0_FIRE1            EQU 4
J0_FIRE2            EQU 5

; Direcciones para el JOY1
J1_UP               EQU 0
J1_DOWN             EQU 1
J1_LEFT             EQU 2
J1_RIGHT            EQU 3
J1_FIRE1            EQU 4
J1_FIRE2            EQU 5

; ---------------------------------------------------------------------------
; Matriz de teclado del CPC
;                                                         Líneas
; Bits  0               1              2         3     4     5        6                 7     8            9
;  7    F.              F0             Ctrl      >.    <,    Space    V                 X     Z            Del
;  6    Enter           F2             `\        ?/    M     N        B                 C     Caps Lock    Unused
;  5    F3              F1             Shift     *:    K     J        F/Joy 1 Fire 1    D     A            Joy 0 Fire 1
;  4    F6              F5             F4        +;    L     H        G/Joy 1 Fire 2    S     Tab          Joy 0 Fire 2
;  3    F9              F8             }]        P     I     Y        T/Joy 1 Right     W     Q            Joy 0 Right
;  2    Cursor Down     F7             Return    |@    O     U        R/Joy 1 Left      E     Esc          Joy 0 Left
;  1    Cursor Right    Copy           {[        =-    )9    '7       %5/Joy 1 Down     #3    "2           Joy 0 Down
;  0    Cursor Up       Cursor Left    Clr       £^    _0    (8       &6/Joy 1 Up       $4    !1           Joy 0 Up

; Líneas de la matriz del teclado
KEY_LINE0           EQU 0
KEY_LINE1           EQU 1
KEY_LINE2           EQU 2
KEY_LINE3           EQU 3
KEY_LINE4           EQU 4
KEY_LINE5           EQU 5
KEY_LINE6           EQU 6
KEY_LINE7           EQU 7
KEY_LINE8           EQU 8
KEY_LINE9           EQU 9

; Códigos de las teclas
KEY_FPERIOD         EQU 7
KEY_F0              EQU 7
KEY_CTRL            EQU 7
KEY_GREATER         EQU 7
KEY_MINOR           EQU 7
KEY_SPACE           EQU 7
KEY_V               EQU 7
KEY_X               EQU 7
KEY_Z               EQU 7
KEY_DEL             EQU 7
   
KEY_ENTER           EQU 6
KEY_F2              EQU 6
KEY_BACKSLASH       EQU 6
KEY_SLASH           EQU 6
KEY_M               EQU 6
KEY_N               EQU 6
KEY_B               EQU 6
KEY_C               EQU 6
KEY_CAPS_LOCK       EQU 6

KEY_F3              EQU 5
KEY_F1              EQU 5
KEY_SHIFT           EQU 5
KEY_COLON           EQU 5
KEY_K               EQU 5
KEY_J               EQU 5
KEY_F               EQU 5
KEY_D               EQU 5
KEY_A               EQU 5

KEY_F6              EQU 4
KEY_F5              EQU 4
KEY_F4              EQU 4
KEY_PLUS            EQU 4
KEY_L               EQU 4
KEY_H               EQU 4
KEY_G               EQU 4
KEY_S               EQU 4
KEY_TAB             EQU 4

KEY_F9              EQU 3
KEY_F8              EQU 3
KEY_RBRACKET        EQU 3
KEY_P               EQU 3
KEY_I               EQU 3
KEY_Y               EQU 3
KEY_T               EQU 3
KEY_W               EQU 3
KEY_Q               EQU 3

CURSOR_DOWN         EQU 2
KEY_F7              EQU 2
KEY_RETURN          EQU 2
KEY_AT              EQU 2
KEY_O               EQU 2
KEY_U               EQU 2
KEY_R               EQU 2
KEY_E               EQU 2
KEY_ESC             EQU 2

CURSOR_RIGHT        EQU 1
KEY_COPY            EQU 1
KEY_LBRACKET        EQU 1
EQUAL               EQU 1
KEY_9               EQU 1
KEY_7               EQU 1
KEY_5               EQU 1
KEY_3               EQU 1
KEY_2               EQU 1

CURSOR_UP           EQU 0
CURSOR_LEFT         EQU 0
KEY_CLR             EQU 0
KEY_POUND           EQU 0
KEY_0               EQU 0
KEY_8               EQU 0
KEY_6               EQU 0
KEY_4               EQU 0
KEY_1               EQU 0

; ---------------------------------------------------------------------------
; Hardware de la disquetera
FDC_MOTOR           EQU $FA7E   ; Floppy Motor On/Off Flipflop
FDC_STATUS          EQU $FB7E   ; FDC 765 Main Status Register (read only)
FDC_DATA            EQU $FB7F   ; FDC 765 Data Register (read/write) 

; Para parar los motores de todas las disqueteras
;    LD   BC,$FA7E
;    DEFB #ED,#71        ; OUT (C),0 ahorrandonos el hacer uso del acumulador
