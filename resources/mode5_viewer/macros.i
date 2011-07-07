; ---------------------------------------------------------------------------
; Macros / (c) SyX 2009
; ---------------------------------------------------------------------------

; ---------------------------------------------------------------------------
; Añade un breakpoint usable por el WinApe
; ---------------------------------------------------------------------------
 MACRO BREAKPOINT
    DEFB $ED,$FF
 ENDM

; ---------------------------------------------------------------------------
; Espera N microsegundos (espera máxima 1021 ms / longitud 4 - 7 bytes)
; ---------------------------------------------------------------------------
 MACRO WAIT_MICROSEG
\@ EQU \1 - 1
    LD   B,\@ / 4                       ; (2 = 1 que restamos y 1 que falta del DJNZ)
    DJNZ $                              ; (4 * [N - 1] + 3 = 4 * N - 1)
    DEFS \@ % 4                         ; (1 * [4 * N - 1] % 4)
 ENDM

; ---------------------------------------------------------------------------
; BC += A
; ---------------------------------------------------------------------------
 MACRO ADD_A_BC
    ADD  A,C
    JR   NC,.no_se_pasa\@
    INC  B
.no_se_pasa\@
    LD   C,A
 ENDM

; ---------------------------------------------------------------------------
; DE += A
; ---------------------------------------------------------------------------
 MACRO ADD_A_DE
    ADD  A,E
    JR   NC,.no_se_pasa\@
    INC  D
.no_se_pasa\@
    LD   E,A
 ENDM

; ---------------------------------------------------------------------------
; HL += A
; ---------------------------------------------------------------------------
 MACRO ADD_A_HL
    ADD  A,L                            ; (1)
    JR   NC,.no_se_pasa\@               ; (2 / 3)
    INC  H                              ; (1)
.no_se_pasa\@
    LD   L,A                            ; (1)
 ENDM                                   ; Total 5 microsegundos

; ---------------------------------------------------------------------------
; IX += A
; ---------------------------------------------------------------------------
 MACRO ADD_A_IX
    ADD  A,IXL
    JR   NC,.no_se_pasa\@
    INC  IXH
.no_se_pasa\@
    LD   IXL,A
 ENDM

; ---------------------------------------------------------------------------
; IY += A
; ---------------------------------------------------------------------------
 MACRO ADD_A_IY
    ADD  A,IYL
    JR   NC,.no_se_pasa\@
    INC  IYH
.no_se_pasa\@
    LD   IYL,A
 ENDM

; ---------------------------------------------------------------------------
; Paramos el Z80
; ---------------------------------------------------------------------------
 MACRO STOP_CPU
    DI
    HALT
 ENDM

; ---------------------------------------------------------------------------
; Bucle Infinito
; ---------------------------------------------------------------------------
 MACRO INFINITO
    SET_BORDER ROJO
.bucle_infinito\@
    JR   .bucle_infinito\@
 ENDM

; ---------------------------------------------------------------------------
; SET_BORDER: Cambia el color del border
; Corrompe: BC,A
; ---------------------------------------------------------------------------
 MACRO SET_BORDER ;color
    LD   A,BORDER                       ; (2)
    LD   BC,GATE_ARRAY + INK + \1 ;color    ; (3)
    OUT  (C),A                          ; (4) Pluma
    OUT  (C),C                          ; (4) Color
 ENDM                                    ; Total 13 microsegundos

; ---------------------------------------------------------------------------
; SET_PEN: Cambia el color de una pluma
; Corrompe: BC,A
; ---------------------------------------------------------------------------
 MACRO SET_PEN ;pluma, color
    LD   A,\1 ;pluma                        ; (2)
    LD   BC,GATE_ARRAY + INK + \2 ;color    ; (3)
    OUT  (C),A                          ; (4) Pluma
    OUT  (C),C                          ; (4) Color
 ENDM                                    ; Total 13 microsegundos

; ---------------------------------------------------------------------------
; SET_PAGE: Activa la página de RAM extendida indicada
; Corrompe: BC
; ---------------------------------------------------------------------------
 MACRO SET_PAGE
    LD   BC,GATE_ARRAY + \1             ; (3)
    OUT  (C),C                          ; (4)
 ENDM

; ---------------------------------------------------------------------------
; Muestra los registros del Asic en $4000
; Corrompe: BC
; ---------------------------------------------------------------------------
 MACRO ASIC_ON
    LD   BC,GATE_ARRAY + $B8
    OUT  (C),C
 ENDM

; ---------------------------------------------------------------------------
; Esconde los registros del Asic en $4000
; Corrompe: BC
; ---------------------------------------------------------------------------
 MACRO ASIC_OFF
    LD   BC,GATE_ARRAY + $A0
    OUT  (C),C
 ENDM

; ---------------------------------------------------------------------------
; WAIT_VBL: Esperamos al comienzo del refresco de pantalla
; Corrompe: A, BC
; ---------------------------------------------------------------------------
 MACRO WAIT_VBL
    LD   B,>PPI_B
.wait_vbl\@
    IN   A,(C)
    RRA
    JR   NC,.wait_vbl\@
 ENDM

; ---------------------------------------------------------------------------
; WRITE_CRTC: Escribe un valor en un registro del CRTC
; Corrompe: BC
; ---------------------------------------------------------------------------
 MACRO WRITE_CRTC ;registro,valor
    LD   BC,CRTC_SELECT + \1 ;registro
    OUT  (C),C
    LD   BC,CRTC_WRITE + \2 ;valor
    OUT  (C),C
 ENDM

; ---------------------------------------------------------------------------
; WRITE_CRTC_REG: Escribe un registro del Z80 en un registro del CRTC
; ---------------------------------------------------------------------------
 MACRO WRITE_CRTC_REG ;registro_crtc, registro_z80
    LD   BC,CRTC_SELECT + \1 ;registro_crtc
    OUT  (C),C
    INC  B
    OUT  (C),\2 ;registro_z80
 ENDM

; ---------------------------------------------------------------------------
; COPY_12: Copia 12 bytes haciendo uso de la pila
; Corrompe: AF, BC, DE, AF', BC', DE'
; ---------------------------------------------------------------------------
 MACRO COPY_12
    LD   SP,HL          ; HL ==> ORIGEN

    LD   BC,12
    ADD  HL,BC          ; NEXT_CHUNCK

    POP  AF
    POP  DE
    POP  BC
    EX   AF,AF' ;DEFB $08            ; EX AF,AF' (ERROR en pasmo6)
    POP  AF
    EXX
    POP  DE
    POP  BC

    LD   SP,HL          ; HL ==> DESTINO
    PUSH BC
    PUSH DE
    PUSH AF

    OR   A              ; Limpia Carry
    LD   BC,12
    SBC  HL,BC          ; NEXT_CHUNCK

    EXX      
    PUSH BC
    PUSH DE
    EX   AF,AF' ;DEFB $08            ; EX AF,AF' (ERROR en pasmo6)
    PUSH AF
 ENDM

; ---------------------------------------------------------------------------
; COPY_16: Copia 16 bytes haciendo uso de la pila
; Corrompe: AF, BC, DE, IX, IY, AF', BC', DE'
; ---------------------------------------------------------------------------
 MACRO COPY_16
    LD   SP,HL          ; HL ==> ORIGEN

    LD   BC,16
    ADD  HL,BC          ; NEXT_CHUNCK

    POP  AF
    POP  DE
    POP  BC
    EX   AF,AF' ;DEFB $08            ; EX AF,AF' (ERROR en pasmo6)
    EXX
    POP  AF
    POP  DE
    POP  BC
    POP  IX
    POP  IY

    LD   SP,HL          ; HL ==> DESTINO
    PUSH IY
    PUSH IX
    PUSH BC
    PUSH DE
    PUSH AF

    OR   A              ; Limpia Carry
    LD   BC,16
    SBC  HL,BC          ; NEXT_CHUNCK

    EXX      
    EX   AF,AF'     ;DEFB $08            ; EX AF,AF' (ERROR en pasmo6)
    PUSH BC
    PUSH DE
    PUSH AF
 ENDM

; ---------------------------------------------------------------------------
;SET_PPI_CONTROL: Establece el modo de funcionamiento y el sentido de los puertos del PPI
; ---------------------------------------------------------------------------
 MACRO SET_PPI_CONTROL ;VALOR
    LD   BC,PPI_CONTROL + \1 ;|VALOR               ; (3)
    OUT  (C),C                              ; (4)
 ENDM                                    ; Total 7 microsegundos

; ---------------------------------------------------------------------------
; INACTIVE_PSG: Desactivamos el PSG
; ---------------------------------------------------------------------------
 MACRO INACTIVE_PSG
; PPI C bits 7 y 6:
; 0 0 ---> PSG Inactivo
; 0 1 ---> Lee el valor del registro del PSG seleccionado.
; 1 0 ---> Escribe el valor en el registro del PSG seleccionado.
; 1 1 ---> Selecciona un registro del PSG.
; NOTA: En los CPC+ es necesario activar el modo inactivo del PSG entre cambios de 
; funciones del PSG.
    LD   B,>PPI_C                       ; (2) Antes era --> LD BC,PPI_C|%00000000 (3)
    DEFB $ED,$71                            ; (4) OUT (C),0 Antes era --> OUT (C),C
 ENDM                                    ; Total 6 microsegundos

; ---------------------------------------------------------------------------
; SEND_PSG_REG: Envia el número de un registro del PSG por el Puerto A del PPI 
; ---------------------------------------------------------------------------
 MACRO SEND_PSG_REG ;REGISTRO 
    LD   BC,PPI_A + \1 ;|REGISTRO                  ; (3)
    OUT  (C),C                              ; (4) Registro del PSG con el que queremos trabajar.
 ENDM                                    ; Total 7 microsegundos

; ---------------------------------------------------------------------------
; SEND_PSG_DATA: Envia un byte para un registro del PSG por el Puerto A del PPI 
; ---------------------------------------------------------------------------
 MACRO SEND_PSG_DATA ;VALOR
    LD   BC,PPI_A + \1 ;|VALOR                     ; (3)
    OUT  (C),C                              ; (4) Escribimos los datos para el registro del PSG
 ENDM                                    ; Total 7 microsegundos

; ---------------------------------------------------------------------------
; SELECT_PSG: Selecciona un registro del PSG
; ---------------------------------------------------------------------------
 MACRO SELECT_PSG
    LD   BC,PPI_C|%11000000                 ; (3)
    OUT  (C),C                              ; (4) Seleccionamos el registro del PSG
 ENDM                                    ; Total 7 microsegundos

; ---------------------------------------------------------------------------
; WRITE_PSG: Escribe un byte a un registro del PSG
; ---------------------------------------------------------------------------
 MACRO WRITE_PSG
;1-El PPI Port A debe estar funcionando como salida.
;2-Seleccionar el registro escribiendo el número de registro en el PPI Port A.
;3-Establecer los bits 7 y 6 del PPI Port C a 1.
;4-Escribir el dato en el PPI Port A
;5-Establecer el bit 7 a 1 y el bit 6 a 0 del PPI Port C.
; NOTA: En los CPC+ es necesario activar el modo inactivo del PSG entre cambios de 
; funciones del PSG.
; PPI C bits 7 y 6:
; 0 0 ---> PSG Inactivo
; 0 1 ---> Lee el valor del registro del PSG seleccionado.
; 1 0 ---> Escribe el valor en el registro del PSG seleccionado.
; 1 1 ---> Selecciona un registro del PSG.
    LD   BC,PPI_C|%10000000                 ; (3)
    OUT  (C),C                              ; (4) Función de escribir un valor en el registro del PSG
 ENDM                                    ; Total 7 microsegundos

; ---------------------------------------------------------------------------
; READ_PSG_RA: Leemos un byte de un registro del PSG en el Acumulador
; ---------------------------------------------------------------------------
 MACRO READ_PSG_RA
    LD   B,>PPI_A                       ; (2)
    IN   A,(C)                              ; (4) Leemos los datos del registro del PSG
 ENDM                                    ; Total 6 microsegundos

; ---------------------------------------------------------------------------
; READ_PSG_RB: Leemos un byte de un registro del PSG en el registro B
; ---------------------------------------------------------------------------
 MACRO READ_PSG_RB
    LD   B,>PPI_A                       ; (2)
    IN   B,(C)                              ; (4) Leemos los datos del registro del PSG
 ENDM                                    ; Total 6 microsegundos

; ---------------------------------------------------------------------------
; INIT_PSG: Inicializamos el PSG 
; ---------------------------------------------------------------------------
 MACRO INIT_PSG
; Estado del CPC por defecto es:
; Puerto A de I/O del PSG establecido a Entrada
; Generadores de Ruido y Tono desconectados
; R7 ---> In Port B | In Port A | Noise C | Noise B| Noise A | Tone C | Tone B | Tone A (lógica inversa)
    SEND_PSG_REG 7                          ; (7) Enviamos el registro 7 del PSG
    SELECT_PSG                              ; (7) Seleccionamos un registro del PSG

;    INACTIVE_PSG                             ; (6) PSG Inactivo (CPC+)
    DEFB $ED,$71                            ; (4) PSG Inactivo (CPC+) / OUT (C),0

    SEND_PSG_DATA %00111111                 ; (7) Escribimos los datos para el registro del PSG
    WRITE_PSG                               ; (7) Función de escribir un valor en el registro del PSG

;    INACTIVE_PSG                            ; (6) PSG Inactivo (CPC+)
    DEFB $ED,$71                            ; (4) PSG Inactivo (CPC+) / OUT (C),0
 ENDM                                    ; Total 36 microsegundos

; ---------------------------------------------------------------------------
; INIT_PSG_FOR_SCANKEYS: Inicializamos el PSG para escanear el teclado
; ---------------------------------------------------------------------------
 MACRO INIT_PSG_FOR_SCANKEYS
    SEND_PSG_REG 14                         ; (7) 1 Enviamos el número del registro del PSG

    SELECT_PSG                              ; (7) 2 Operación: Seleccionar registro del PSG

;    INACTIVE_PSG                            ; (6) PSG Inactivo (CPC+)
    DEFB $ED,$71                            ; (4) PSG Inactivo (CPC+) / OUT (C),0

    SET_PPI_CONTROL %10010010               ; (7) 4 Establecer el PPI Port A de entrada
 ENDM                                    ; Total 25 microsegundos

; ---------------------------------------------------------------------------
; RESTORE_PSG_OF_SCANKEYS: Restauramos el PSG después de escanear el teclado
; ---------------------------------------------------------------------------
 MACRO RESTORE_PSG_OF_SCANKEYS
    SET_PPI_CONTROL %10000010               ; (7) 9 Establecer el PPI Port A a salida
;    INACTIVE_PSG                            ; (6) 10 PSG Inactivo (CPC+)
    DEC  B                                  ; (1) PPI_CONTROL - 1 => PPI_C
    DEFB $ED,$71                            ; (4) PSG Inactivo (CPC+) / OUT (C),0
 ENDM                                    ; Total 12 microsegundos
