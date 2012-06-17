; ---------------------------------------------------------------------------
; Patches for Amèlie
; ---------------------------------------------------------------------------

AMSDOS_HOOK             EQU $BE7F
amelie_cas_hook         EQU $C073   ; * Change if we add more RSXs *

    ORG  AMSDOS_HOOK

; ---------------------------------------------------------------------------
; CAS vectors hook
hook_for_amsdos_cas_vectors
    RST  $18                        ; FAR_CALL amelie_hook
    DEFW amelie_cas_hook_far_call
    RET

amelie_cas_hook_far_call
    DEFW amelie_cas_hook            ; ROM Address
    DEFB 0                          ; ROM number for our ROM

; ---------------------------------------------------------------------------
; Amsdos variables
amsdos_cas_vectors
    DEFS 2                          ; Amsdos ROM CAS Vectors (CPC = $CD4C / CPC+ = $CDB0)

amsdos_buffers
    DEFS 2                          ; Amsdos reserved area + 560 
                                    ; Start of Catalog (128 bytes) and Sector (512) buffers
; ---------------------------------------------------------------------------
; Far address of overloaded RSXs
farcall_amsdos_drive
    DEFS 2                          ; |DRIVE RSX address
    DEFS 1                          ; |DRIVE RSX ROM number

farcall_amsdos_dir
    DEFS 2                          ; |DIR RSX address
    DEFS 1                          ; |DIR RSX ROM number

farcall_amsdos_era
    DEFS 2                          ; |ERA RSX address
    DEFS 1                          ; |ERA RSX ROM number

farcall_amsdos_ren
    DEFS 2                          ; |REN RSX address
    DEFS 1                          ; |REN RSX ROM number
 
farcall_amsdos_read_sector
    DEFS 2                          ; |^D  RSX address
    DEFS 1                          ; |^D RSX ROM number

farcall_amsdos_write_sector
    DEFS 2                          ; |^E RSX address
    DEFS 1                          ; |^E RSX ROM number

farcall_amsdos_format_track
    DEFS 2                          ; |^F RSX address
    DEFS 1                          ; |^F RSX ROM number

; ---------------------------------------------------------------------------
; SD Commands

; ---------------------------------------------------------------------------
    END
