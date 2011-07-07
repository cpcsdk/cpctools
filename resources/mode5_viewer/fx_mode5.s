; FX Mode 5 
; (c) SyX 2010

; Limites del modo 5:
; * Screen resolution 288 x 256 pixels.
; * 1 Fixed colour for the picture (Pen 3).
; * 2 Fixed colour for each scanline (Pens 1 and 2), main reason for the width limitation.
; * 1 Variable colour that change six times (48 pixels) for each scanlie (Pen 0).

; Format of colour array:
; 1.- Fixed colour for the picture (Pen 3)
; 2.- First colour for the scanline (Pen 2)
; 3.- Second colour for the scanline (Pen 1)
; 4.- Six variable colours for the scanline (Pen 0)
; 5.- Go to 2

; ---------------------------------------------------------------------------
; Constants

SYNCHRO_DELAY   EQU 6               ; Delay for the sinchronization

NUM_SCANLINES   EQU 0               ; Scanlines where to keep the FX (0 = 256)

; ---------------------------------------------------------------------------
; FX mode 5
fx_mode5
    ; Wait refresh
    WAIT_VBL

    ; Set fixed colour for the picture (Pen 3)
    LD   B,>GATE_ARRAY
    LD   HL,M5_PALETTE
    LD   A,3
    OUT  (C),A              ; Pen 3 (Most used colour)
    INC  B
    OUTI

    ; Wait to the begin of the screen to disable interrupts
    HALT
    DI
    ; Wait for the next frame
    WAIT_MICROSEG 63
    WAIT_SCANLINES 311

loop_fx_mode5
    ; Wait to the begin of display (37 scanlines)
    WAIT_SCANLINES 37

    ; Preload pens values in the registers for faster rasters :P
    XOR  A                                  ; (1) A = Pen 0 (variable colours)
    LD   BC,GATE_ARRAY + NUM_SCANLINES      ; (3) B = Gate Array / C = Scanlines where keep the FX
    LD   HL,M5_PALETTE + 1                  ; (3) HL = Colour array
    LD   DE,$0201                           ; (3) DE = Pens 2 and 1 (fixed colours for each scanline)

    ; Set fixed colours for the first scanline (Pens 2 and 1)
    OUT  (C),D                              ; (4) Pen 2
    INC  B                                  ; (1)
    OUTI                                    ; (5)

    OUT  (C),E                              ; (4) Pen 1
    INC  B                                  ; (1)
    OUTI                                    ; (5)

    ; Wait to the position in the scanline where the rasters begin.
wait_synch_01
    ; We have to deduct a few microseconds of preload the pens values (10),
    ; set fixed colours for the fisrt scanline (20) and the SYNCHRO delay.
    DEFS 64 - 10 - 10 - 10 - SYNCHRO_DELAY

keep_fx_mode5
    ; Set colours for Pen 0 
;    DEFB $ED,$71                            ; (4) OUT (C),0 Select Pen 0 and free the Acumulator
    OUT  (C),A                              ; (4) Select Pen 0

    REPT 6                                  ; Make the six colours change (48 pixels)
        INC  B                              ; (1)
        OUTI                                ; (5)
    ENDR                                    ; Total 36 microseconds

    ; Set fixed colours for the next scanline
    OUT  (C),D                              ; (4) Pen 2
    INC  B                                  ; (1)
    OUTI                                    ; (5)

    OUT  (C),E                              ; (4) Pen 1
    INC  B                                  ; (1)
    OUTI                                    ; (5)
                                            
    DEC  C                                  ; (1)
    JP   NZ,keep_fx_mode5                   ; (3)
                                            ; Total 64 microseconds

    ; Wait to the end of the last scanline
wait_synch_02
    ; We have to deduct a few microseconds for the JP loop_fx_mode5 (3)
    DEFS SYNCHRO_DELAY - 3                  ; Apply the SYNCHRO_DELAY

    ; We need to wait 18 scanlines until the begin of the new frame
    WAIT_SCANLINES 17 ;18                      

    NOP
    CALL test_esc_key                       ; (60)
    JP   Z,end_fx_rutine

    JP   loop_fx_mode5                      ; (3)

; ---------------------------------------------------------------------------
; Test for ESCAPE key
;   Exits:
;       Zero flag enabled if key pressed
; ---------------------------------------------------------------------------
test_esc_key                    ; (5) CALL test_esc_key
    INIT_PSG_FOR_SCANKEYS       ; (25) 1º, 2º, 3º y 4º (Destruye BC)

    LD   A,%01001000            ; (2) 5º Línea de la matriz del teclado a comprobar (bits 3-0 del PPI Port C)

    DEC  B                      ; (1)
    OUT  (C),A                  ; (4) 6º Operación: Leer registro del PSG

    READ_PSG_RA                 ; (6) 7º Leemos la línea de la matriz del teclado desde el PPI Port A
    RESTORE_PSG_OF_SCANKEYS     ; (12) 9º y 10º (Destruye BC)

    BIT  KEY_ESC,A              ; (2)

    RET                         ; (3)
                                ; Total 60 microsegundos
