; ---------------------------------------------------------------------------
; Loader FX Mode 5 pictures / (c) SyX 2011
; ---------------------------------------------------------------------------

; ---------------------------------------------------------------------------
; Constants

; ---------------------------------------------------------------------------
; Mode 5 constants
; ---------------------------------------------------------------------------
; (288 x 256) --> 296 in CPC (4 + 288 + 4) and CPC+ (2 + 288 + 6)
; 2048 / 72 = 28 lines and 32 bytes
; 28 * 8 = 224 scanlines (16128 bytes) and the second half 32 scanlines (2304 bytes)
WIDTH_CRTC                  EQU 37              ; 36 + 1 chars in mode 1 (288 + 8 pixels)
HEIGHT_CRTC                 EQU 32              ; 32 chars (256 scanlines)
OFFSET_HORIZONTAL           EQU 26 + WIDTH_CRTC / 2    
OFFSET_VERTICAL             EQU 16 + (HEIGHT_CRTC * 8) / 14
OFFSET_CRTC                 EQU 25              ; 16 for the split
SCANS_PART1                 EQU 216             ; 224 scanlines for the first part of the screen

; ---------------------------------------------------------------------------
PROGRAM_ORG                 EQU $2000
STACK_SIZE                  EQU 256
M5_PALETTE                  EQU PROGRAM_ORG - STACK_SIZE - HEIGHT_CRTC * 8 * 8 - 1
M5_GFX                      EQU $3800           ; GFX load address
SOUND_CLICK                 EQU $4000

; ---------------------------------------------------------------------------
; Macro for wait N scanlines
; ---------------------------------------------------------------------------
 MACRO WAIT_SCANLINES
    LD   IX,\1 * 4 - 1          ; (4) 4
    CALL wait_scanlines_ix      ; (5) 3
 ENDM                           ; (9) 7 bytes

; ---------------------------------------------------------------------------
    include "firmware.i"
    include "hardware.i"
    include "macros.i"

    ORG  PROGRAM_ORG

start
    ; Get active disk drive and save it for after
    LD   HL,(AMSDOS_RESERVED_AREA)
    LD   A,(HL)                 ; Active disk drive for Amsdos (0 = A / 1 = B)
    LD   (disk_drive + 1),A
restart    
    LD   C,$FF                  ; Disable ROMs
    LD   HL,main_program        ; HL = Start program address
    CALL MC_START_PROGRAM       ; Run program

main_program
    CALL TXT_VDU_DISABLE        ; Disable VDU
    CALL KL_ROM_WALK            ; Enable ROMS
    CALL TXT_VDU_ENABLE         ; Enable VDU

    ; Set active diskdrive for the Amsdos (the same used for load this program)
disk_drive
    LD   A,0
    LD   HL,(AMSDOS_RESERVED_AREA)
    LD   (HL),A

launch_console
    CALL execute_console

    ; Load files
    ; * Palette *
    LD   HL,pal_file                ; HL = address filename
    LD   B,end_pal_file - pal_file  ; B = length filename
    CALL CAS_IN_OPEN
    LD   HL,M5_PALETTE              ; HL = load address
    CALL CAS_IN_DIRECT
    JR   NC,launch_console
    CALL CAS_IN_CLOSE 

    ; * GFX *
    LD   HL,gfx_file                ; HL = address filename
    LD   B,end_gfx_file - gfx_file  ; B = length filename
    CALL CAS_IN_OPEN
    LD   HL,M5_GFX                  ; HL = load address
    CALL CAS_IN_DIRECT 
    JR   NC,launch_console
    CALL CAS_IN_CLOSE 

    ; Wait to no key is pressed
.no_key_press
    CALL KM_READ_KEY
    JR   C,.no_key_press

    ; Take the system!!! :P (save the firmware)
    DI
    LD   (firmware_sp + 1),SP
    CALL save_firmware
    IM   1
    LD   HL,$C9FB
    LD   ($0038),HL
    LD   SP,PROGRAM_ORG
    EI                 

    CALL define_screen_mode5        ; Set screen mode and display size

    CALL draw_screen

    HALT

    ; Launch FX
fx_rutine
    include "fx_mode5.s"
end_fx_rutine

    ; Restore the firmware
    DI
    CALL restore_firmware
firmware_sp
    LD   SP,$0000
    EI

    JP   restart

; ---------------------------------------------------------------------------
; Save firmware
; ---------------------------------------------------------------------------
save_firmware
    LD   HL,($0038)
    LD   (sm_firmware_int + 1),HL

    LD   HL,$A800                  
    LD   DE,$C000 + 288
    LD   A,6
.loop_save_firmware
    PUSH DE
    LD   BC,$400
    LDIR
    POP  DE
    EX   AF,AF'
    LD   A,D
    ADD  A,8
    LD   D,A
    EX   AF,AF'
    DEC  A
    JR   NZ,.loop_save_firmware

    RET

; ---------------------------------------------------------------------------
; Restore firmware
; ---------------------------------------------------------------------------
restore_firmware
sm_firmware_int
    LD   HL,$0000
    LD   ($0038),HL

    LD   HL,$C000 + 288
    LD   DE,$A800
    LD   A,6
.loop_restore_firmware
    PUSH HL
    LD   BC,$400
    LDIR
    POP  HL
    LD   BC,$800
    ADD  HL,BC
    DEC  A
    JR   NZ,.loop_restore_firmware

    ; Restore alternative registers used by the firmware
    EXX
    LD   BC,$7F8D
    EXX  
    EX   AF,AF'
    XOR  A
    EX   AF,AF'

    CALL define_screen_firmware
    RET

; ---------------------------------------------------------------------------
; Wait N scanlines using IX
; Entradas:
;   IX = (Scanlines * 4) - 1
; Salidas:
;   Se corrompen IX y A
; ---------------------------------------------------------------------------
wait_scanlines_ix                           ; (9) Inicialización LD IX (4) + CALL (5)
    DEFS 5                                  ; (5) 
.loop_wait_scanlines_ix
    DEFS 6                                  ; (6)
    DEC  IX                                 ; (3)
    LD   A,IXH                              ; (2)
    OR   IXL                                ; (2)
    JR   NZ,.loop_wait_scanlines_ix         ; (2/3)
                                            ; Total bucle --> 16 * (IX - 1) + 15
    RET                                     ; (3)
                                            ; Total 16 + 16 * IX

; ---------------------------------------------------------------------------
; Draw the screen with 4 pixels offset (4 + 288 + 4)
; ---------------------------------------------------------------------------
draw_screen
    ; Pinta la primera parte de la pantalla en un CPC (4 + 288 + 4)
pinta_primera_parte
    ; Y la pintamos a partir de $8000
    LD   HL,M5_GFX
    LD   DE,RAM_P2 + OFFSET_CRTC * 2
    LD   A,SCANS_PART1              ; Líneas de la imagen
.bucle_pinta_pantalla8
    CALL pinta_scanline
    PUSH AF
    EX   DE,HL
    LD   A,H
    ADD  A,8
    BIT  6,A
    JR   Z,.no_wrap8
    RES  6,A
    LD   H,A
    LD   A,L
    ADD  A,WIDTH_CRTC * 2 ; Ancho de la pantalla
    LD   L,A
    LD   A,H
    JR   NC,.no_wrap8
    INC  A
.no_wrap8
    LD   H,A
    EX   DE,HL
    POP  AF
    DEC  A
    JR   NZ,.bucle_pinta_pantalla8

    ; Pinta la segunda parte de la pantalla en un CPC (4 + 288 + 4)
pinta_segunda_parte
    ; Y la pintamos en $C000
    LD   DE,RAM_P3
    LD   A,256 - SCANS_PART1       ; Líneas de la imagen
.bucle_pinta_pantallac
    CALL pinta_scanline
    PUSH AF
    EX   DE,HL
    LD   A,H
    ADD  A,8
    JR   NC,.no_wrapc
    LD   A,L
    ADD  A,WIDTH_CRTC * 2 ; Ancho de la imagen
    LD   L,A
    LD   A,H
    ADC  A,$C8
.no_wrapc
    LD   H,A
    EX   DE,HL
    POP  AF
    DEC  A
    JR   NZ,.bucle_pinta_pantallac
    RET

pinta_scanline
    PUSH DE
    ; Byte del comienzo del scanline
    EX   DE,HL
    LD   (HL),$FF
    EX   DE,HL
    INC  DE

    LD   BC,WIDTH_CRTC * 2 - 2 ; Ancho de la imagen
    LDIR
    ; Byte del final del scanline
    EX   DE,HL
    LD   (HL),$FF
    EX   DE,HL

    POP  DE
    RET
; ---------------------------------------------------------------------------
; Set screen mode and display size for the mode 5 fx
; ---------------------------------------------------------------------------
define_screen_mode5
    WAIT_VBL

    ; Set all pens black
    LD   A,16               ; 16 Pens + Border
    LD   BC,GATE_ARRAY + INK + NEGRO
.loop_black_inks
    OUT  (C),A              ; Pen
    OUT  (C),C              ; Ink
    DEC  A
    CP   $FF
    JR   NZ,.loop_black_inks

    ; Set screen mode
    LD   BC,GATE_ARRAY + MODE_Y_ROM + ROM_OFF + MODE_1
    OUT  (C),C

    ; Set dimensions and center the display 
    LD   HL,crtc_mode5_values
    LD   A,7
    CALL set_crtc_values
    RET

crtc_mode5_values
    DEFB REG_01,WIDTH_CRTC
    DEFB REG_06,HEIGHT_CRTC
    DEFB REG_02,OFFSET_HORIZONTAL
    DEFB REG_07,OFFSET_VERTICAL
    DEFB REG_0C,VRAM_P2 + VRAM32
    DEFB REG_0D,OFFSET_CRTC
    DEFB REG_03,$88             ; Make the 300Hz interrupt happens in the same positions in CPC and CPC+.

; ---------------------------------------------------------------------------
; Set screen mode and display size for the firmware
; ---------------------------------------------------------------------------
define_screen_firmware
    WAIT_VBL
    LD   HL,crtc_firmware_values
    LD   A,7
    CALL set_crtc_values
    RET

crtc_firmware_values
    DEFB REG_01,$28
    DEFB REG_06,$19
    DEFB REG_02,$2E
    DEFB REG_07,$1E
    DEFB REG_0C,VRAM_P3
    DEFB REG_0D,$00
    DEFB REG_03,$8E

; ---------------------------------------------------------------------------
; Set CRTC values
; Entry:
;    HL : Array with the CRTC values
;     A : Counter
; ---------------------------------------------------------------------------
set_crtc_values
    LD   B,>CRTC_SELECT + 1
.loop_set_display
    OUTI        ; CRTC Register
    INC  B
    INC  B
    OUTI        ; Value
    DEC  A
    JR   NZ,.loop_set_display
    RET

; ---------------------------------------------------------------------------
; Show the files with mode 5 pictures from the disk and let you choose the 
; file to view.
; ---------------------------------------------------------------------------
execute_console
    LD   HL,console_text
    CALL print_text

    CALL print_disk_catalog

    LD   HL,prompt_text
    CALL print_text

    ; Initialize the filename string
    LD   HL,pal_file
    LD   (HL)," "
    LD   DE,pal_file + 1
    LD   BC,7
    LDIR

    ; Copy sound_data to central ram
    LD   HL,sound_data
    LD   DE,SOUND_CLICK
    LD   BC,end_sound_data - sound_data
    LDIR

    ; Input filename
    LD   DE,pal_file
    LD   B,9
.loop_key_input
    CALL KM_WAIT_KEY
    CALL make_sound
    CP   $0D
    JR   Z,.end_loop_key_input
    CP   $7F
    JR   NZ,.print_char
    LD   A,B
    CP   9
    JR   Z,.loop_key_input
    LD   HL,delete_char_text
    CALL print_text
    INC  B
    DEC  DE
    LD   A," "
    LD   (DE),A
    JR   .loop_key_input       
.print_char
    LD   L,A
    LD   A,B
    CP   1
    JR   Z,.loop_key_input
    LD   A,L
    CALL TXT_OUTPUT
    LD   (DE),A
    INC  DE
    LD   HL,cursor_text
    CALL print_text
    DJNZ .loop_key_input
.end_loop_key_input
    LD   A," "
    CALL TXT_OUTPUT

    ; Copy filename pal string to gfx string
    LD   HL,pal_file
    LD   DE,gfx_file
    LD   BC,8
    LDIR

    RET

; ---------------------------------------------------------------------------
; Get and print the disk catalog
; ---------------------------------------------------------------------------
print_disk_catalog
    ; Get disk catalog
    CALL TXT_VDU_DISABLE            ; Disable VDU
    XOR  A
    LD   DE,catalog_buffer
    LD   (DE),A                     ; Set flag catalog buffer clear
    CALL CAS_CATALOG
    CALL TXT_VDU_ENABLE             ; Enable VDU

    ; Print filenames with Mode 5 GFXs
    LD   IX,catalog_buffer
.loop_decode_cat
    LD   A,(IX + 0)
    OR   A
    RET  Z
    ; Test for CM5 extension
    LD   A,(IX + 9)
    CP   "C"
    JR   NZ,.next_file
    LD   A,(IX + 10)
    CP   "M"
    JR   NZ,.next_file
    LD   A,(IX + 11)
    CP   "5"
    JR   NZ,.next_file
    ; Mode 5 file found
    LD   (IX + 0),$20
    LD   (IX + 9),$20
    LD   (IX + 10),$FF
    LD   D,IXH
    LD   E,IXL
    EX   DE,HL
    CALL print_text
.next_file
    LD   BC,14
    ADD  IX,BC
    JR   .loop_decode_cat

; ---------------------------------------------------------------------------
; Print a string finished in $FF using the firmware
; Entry:
;    HL : Pointer to the string
; ---------------------------------------------------------------------------
print_text
    LD   A,(HL)
.loop_print_text
    CALL TXT_OUTPUT
    INC  HL
    LD   A,(HL)
.continue_print_text
    CP   $FF
    JR   NZ,.loop_print_text
    RET

; ---------------------------------------------------------------------------
; Make a click, to give feedback when a key is pressed
; ---------------------------------------------------------------------------
make_sound
    PUSH AF
    PUSH BC
    PUSH DE
    PUSH HL
    PUSH IX

    LD   HL,SOUND_CLICK     ; sound_data in central ram
    CALL SOUND_QUEUE

    POP  IX
    POP  HL
    POP  DE
    POP  BC
    POP  AF
    RET

sound_data
    DEFB 2,0,0
    DEFW 478
    DEFB 0,15
    DEFW 1
end_sound_data

; ---------------------------------------------------------------------------
; Text data for the console screen
console_text
    DEFB $1C,$00,$00,$00    ; INK 0,0,0
    DEFB $1C,$01,$1A,$1A    ; INK 1,26,26
    DEFB $1C,$02,$12,$12    ; INK 2,18,18
    DEFB $1C,$03,$19,$19    ; INK 3,25,25
    DEFB $1D,$00,$00        ; BORDER 0,0
    DEFB $04,$01            ; MODE 1
    DEFB $1F,$01,$01        ; LOCATE 1,1
    DEFB $0F,$01            ; PEN 1
    DEFB $18                ; VIDEO INVERSE ON
    DEFB "MODE 5 VIEWER"
    DEFB $12                ; CLEAR FROM CURSOR TO RIGHT
    DEFB $1F,$1F,$01        ; LOCATE 1,30
    DEFB $A4," SyX 2011"
    DEFB $18                ; VIDEO INVERSE OFF
    DEFB $0F,$03            ; PEN 3
    DEFB $FF

prompt_text
    DEFB $1F,$01,$19        ; LOCATE 1,25
    DEFB $0F,$02            ; PEN 2
    DEFB "syx@arnold:~$ "
    DEFB $0F,$01            ; PEN 1
    DEFB "_",$08
    DEFB $FF

delete_char_text
    DEFB " ",$08," ",$08,$08
cursor_text
    DEFB "_",$08,$FF

; ---------------------------------------------------------------------------
; Files with the GFX + PAL
pal_file
    DEFB "SYX@@DAD.CM5"
end_pal_file

gfx_file
    DEFB "DAD@@SYX.GFX"
end_gfx_file

; ---------------------------------------------------------------------------
; Format catalog buffer:
; $FF, file name (8), file extension (3), size in KBs (2),
; $FF, file name (8), file extension (3), size in KBs (2),
; ...
; $00
catalog_buffer
    END
