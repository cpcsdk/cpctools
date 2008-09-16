

CodeSeg	SEGMENT
	ASSUME	CS:CodeSeg,DS:CodeSeg,ES:CodeSeg

CR	EQU	0Dh
LF	EQU	0Ah
EOF	EQU	1Ah

	ORG	5Ch
FCB	LABEL	BYTE

	ORG	78h
INT_1E	LABEL	WORD

	ORG	0100h

Entry	PROC	NEAR
	JMP	XDOS
	DB	11h
	DW	BVMES
	DB	0Eh,09h,0CDh,05h,00h,0C7h
XDOS:	JMP	MAIN

	DB	CR,'DSKWRITE (c) John Elliott',CR,LF
	DB	'08/03/2002',CR,LF,EOF
;
FDOS:	MOV	AH,CL
	INT	21h
	RET
;
BVMES	DB	'This program requires PCDOS v1.00 or later.',CR,LF,'$'

;
; Attempt to load header of DSK file
;
MAIN:	MOV	AL,BYTE PTR FCB+1
	CMP	AL,' '
	JZ	JHLP
	CMP	AL,'/'
	JZ	JHLP
	CMP	AL,'?'
	JZ	JHLP
	CMP	AL,'['
	JZ	JHLP

	MOV	AL,BYTE PTR FCB+10H
	CMP	AL,1
	JC	JHLP
	CMP	AL,3
	JC	NJHLP
JHLP:	JMP	HELP
NJHLP:	DEC	AL
	MOV	DRV,AL

	MOV	AX,0
	MOV	WORD PTR FCB+12,AX
	MOV	WORD PTR FCB+14,AX
	MOV	WORD PTR FCB+32,AX
	MOV	DX,OFFSET FCB
	MOV	CL,0FH
	CALL	FDOS
	INC	AL
	JNZ	G0001
G0000:	JMP	OPENERR
G0001:	MOV	DX,OFFSET DSKH
	MOV	BX,256
	CALL	LOAD
;
; Check for compression.
;
	XOR	AL,AL
	MOV	COMP,AL
	MOV	AX,WORD PTR DSKH
	CMP	AX,0FF76h
	JNZ	NOCMP
	MOV	AL,1
	MOV	COMP,AL
;
; Initialise decompression
;
	MOV	BX,OFFSET DCOMP1
	MOV	DCJ,BX
;
; Close & reopen the file, to rewind it.
;
	MOV	DX,OFFSET FCB
	MOV	CL,10h
	CALL	FDOS
	MOV	AX,0
	MOV	WORD PTR FCB+12,AX
	MOV	WORD PTR FCB+14,AX
	MOV	WORD PTR FCB+32,AX

	MOV	DX,OFFSET FCB
	MOV	CL,0FH
	CALL	FDOS
	INC	AL
	JZ	G0000
	MOV	DX,OFFSET STHUF
	CALL	PRINT
;
; Reload the header - hopefully this time uncompressed
;
	MOV	DX,OFFSET DSKH
	MOV	BX,256
	CALL	LOAD
NOCMP:	CALL	CHKH

DESTOK:	CALL	CONFIRM
	MOV	BX,WORD PTR DSKH+30h
	MOV	CH,BH
	MOV	CL,BL
LOOPC:	PUSH	CX
	MOV	DX,OFFSET TRKH
	MOV	BX,WORD PTR DSKH+32H	; Length of track+header
	CALL	LOAD
	CALL	CHKTH
	POP	CX
	PUSH	CX
	CALL	TRKW
	POP	CX
	DEC	CH
	JNZ	LOOPC
	MOV	AL,BYTE PTR DSKH+31H
	MOV	CH,AL
	DEC	CL
	JNZ	LOOPC
	MOV	DX,OFFSET FCB
	MOV	CL,10H
	CALL	FDOS
	MOV	DX,OFFSET AD$
ERRPR:	CALL	PRINT
EREXT:	MOV	CL,0
	MOV	DX,0
	CALL	FDOS
	INT	20h

HELP:	MOV	DX,OFFSET HELPM
PRINT:	MOV	CL,9
	JMP	FDOS
;
CLOAD:	MOV	CDMA,DX		; Load a block from compressed file
	MOV	CLEN,BX
	MOV	BX,DCJ
	JMP	BX		; Launch or resume decompression thread
;
LOAD:	MOV	AL,COMP
	OR	AL,AL
	JNZ	CLOAD		; Load from SQ'ed file?

	ADD	BX,BX		; BH = number of 128-byte records
				; DX = load address
LOADL:	PUSH	BX
	PUSH	DX
	MOV	CL,1Ah
	CALL	FDOS
	MOV	DX,OFFSET FCB
	MOV	CL,14H
	CALL	FDOS
	OR	AL,AL
	JNZ	SHORT
	POP	DX
	POP	BX
	DEC	BH
	JZ	LOADLE
	MOV	AX,80h
	ADD	AX,DX
	MOV	DX,AX
	JMP	LOADL	

LOADLE:	RET

SHORT:	MOV	DX,OFFSET SHORTM
	JMP	ERRPR
;
CDMA	DW	0
CLEN	DW	0
MSP	DW	0
CSP	DW	0
DCJ	DW	0

DCOMP1:	MOV	MSP,SP
	MOV	SP,OFFSET TABLE
	CALL	USQ
;
; End of Huffman. Flush buffers.
;
	MOV	DX,OFFSET ENDHUF
	CALL	PRINT
	MOV	SP,MSP
	MOV	DI,CDMA
	MOV	SI,BUFF
	MOV	CX,0100h
	CLD
	REP	MOVSB
	RET
;
DCOMP2:	MOV	MSP,SP
	MOV	SP,CSP
	RET
;
BUFULL:	MOV	DI,CDMA
	MOV	SI,BUFF
	MOV	CX,0100h
	CLD
	REP	MOVSB
	MOV	CDMA,DI
	MOV	AX,CLEN
	DEC	AH
	MOV	CLEN,AX
	OR	AH,AH
	JZ	BUFSW
	RET
;
BUFSW:	MOV	AX,OFFSET DCOMP2
	MOV	DCJ,AX
	MOV	CSP,SP
	MOV	SP,MSP
	RET
;

;
SPECB	DB	0,0,0,0,0,0,0
	DB	0,0,0,0,0,0,0
STDS	DW	0
STDSS	DW	0
;
; Get a copy of the standard disc spec
;
GETSPEC:
	PUSH	AX
	PUSH	CX
	PUSH	DI
	PUSH	SI
	MOV	AX,0
	MOV	ES,AX
	LDS	SI,DWORD PTR ES:INT_1E
	MOV	CS:STDS,SI
	MOV	AX,DS
	MOV	CS:STDSS,AX
	MOV	DI,OFFSET SPECB
	MOV	AX,CS
	MOV	ES,AX
	MOV	CX,14
	CLD
	REP	MOVSB
	MOV	AX,CS
	MOV	DS,AX
	MOV	ES,AX
	POP	SI
	POP	DI
	POP	CX
	POP	AX
	RET
;
SETSELF:
	PUSHF
	PUSH	AX
	MOV	AX,0
	MOV	DS,AX
	MOV	AX,CS
	MOV	WORD PTR INT_1E+2,AX
	MOV	AX,OFFSET SPECB
	MOV	WORD PTR INT_1E,AX
	MOV	AX,CS
	MOV	DS,AX
	POP	AX
	POPF	
	RET


SETORIG:
	PUSH	AX
	PUSHF
	MOV	AX,0
	MOV	DS,AX
	MOV	AX,CS:STDSS
	MOV	WORD PTR INT_1E+2,AX
	MOV	AX,CS:STDS
	MOV	WORD PTR INT_1E,AX
	MOV	AX,CS
	MOV	DS,AX
	POPF
	POP	AX
	RET


;
; Write out a track. Firstly generate a disc spec.
;
TRKW:	PUSH	CX
	CALL	GETSPEC
	MOV	AL,BYTE PTR TRKH+14h	;Bytes / sector
	MOV	SPECB+3,AL
	MOV	AL,BYTE PTR TRKH+15h	;Sectors / track
	MOV	SPECB+4,AL
	MOV	AL,BYTE PTR TRKH+16h	;Format gap
	MOV	SPECB+7,AL
	MOV	AL,BYTE PTR TRKH+17h	;Format filler
	MOV	SPECB+8,AL
;
; We have a DPB. Get a track number.
;
TRKW2:	POP	CX
	MOV	AX,WORD PTR DSKH+30H
	SUB	AX,CX		; AH = head AL=cylinder
	PUSH	AX
	CALL	DOFORM		; Format the track
	POP	AX
	CALL	DOWRT		; Write it
	RET
;
;Format cyl AL head AH
;
DOFORM:	PUSH	AX
	CALL	SETSELF
	MOV	AL,DRV
	MOV	DL,AL	;Drive
	MOV	AX,1704h
	INT	13h	;Select 720k density
	CALL	SETORIG
	MOV	AL,BYTE PTR TRKH + 15H	; Sectors
	MOV	SI,OFFSET TRKH+18H ; HL->source
	MOV	DI,OFFSET FMTBUF
ZCPLP:	CLD
	MOVSW
	MOVSW
	INC	SI
	INC	SI
	INC	SI
	INC	SI
	DEC	AL
	JNZ	ZCPLP		; Created format spec at FMTBUF
	MOV	BX,OFFSET FMTBUF
	POP	AX

	MOV	CH,AL		; Cylinder
	MOV	DH,AH		; Head
	MOV	AL,DRV
	MOV	DL,AL		; Drive
	MOV	AL,BYTE PTR TRKH + 15h	; Sectors
	CALL	STATF
	CALL	SETSELF
	MOV	AH,5
	CALL	INT13
	CALL	SETORIG

	JNC	DORT1
	JMP	DSKERR
DORT1:	RET

;
; Write the sectors
;
DOWRT:	PUSH	AX
	MOV	SI,OFFSET TRKH+1Ah	;Physical sector numbers
	MOV	AL,BYTE PTR TRKH+15H
	MOV	DL,AL		; DL = Number of sectors
	MOV	BX,OFFSET TRKBUF ; ->first sector
WSLP:	MOV	AL,[SI]		; Sector number
	MOV	CL,AL		; CL = sector number
	POP	AX
	PUSH	AX
	MOV	CH,AL		; Cylinder
	MOV	DH,AH		; Head
	MOV	AL,DRV
	PUSH	DX
	PUSH	CX
	PUSH	BX
	MOV	AL,DRV		;Drive
	MOV	DL,AL
	CALL	STATW
	MOV	AX,0301h	;Write 1 sector
	CALL	SETSELF
	CALL	INT13
	CALL	SETORIG
	JNC	G0002
	JMP	DSKERR
G0002:	POP	DX
	MOV	AL,BYTE PTR TRKH+14h	;AL = sector shift
	MOV	BX,80h
G0003:	OR	AL,AL
	JZ	G0004
	DEC	AL
	ADD	BX,BX
	JMP	G0003

G0004:	ADD	BX,DX		;BX -> next sector
	MOV	DX,8
	ADD	SI,DX		;SI->next sector number
	POP	CX
	POP	DX
	DEC	DL
	JNZ	WSLP
	POP	AX
	RET
;
SPBUF	DW	SPBASE
SPBASE	DB	'/-\|'
;
SPIN:	LAHF
	PUSH	AX
	PUSH	CX
	PUSH	DX
	PUSH	BX
	PUSH	DI
	PUSH	SI
	MOV	BX,SPBUF
	INC	BX
	MOV	AL,[BX]
	OR	AL,AL
	JNZ	SPIN1
	MOV	BX,OFFSET SPBASE
	MOV	AL,[BX]
SPIN1:	MOV	SPBUF,BX
	MOV	DL,AL
	MOV	CL,2
	CALL	FDOS
	MOV	DL,8
	MOV	CL,2
	CALL	FDOS
POPA:	POP	SI
	POP	DI
	POP	BX
	POP	DX
	POP	CX
	POP	AX
	SAHF
	RET
;
CHKH:	MOV	DX,OFFSET DSKH	;Check for MV - CPC signature
	MOV	BX,OFFSET MVCPC	;in a DSK header
	MOV	CH,8
	JMP	CP$
;
CHKTH:	MOV	DX,OFFSET TRKH	;Check for Track-Info signature
	MOV	BX,OFFSET TRKI	;in a DSK track
	MOV	CH,10
CP$:	XCHG	BX,DX
	MOV	AL,[BX]
	XCHG	BX,DX
	CMP	AL,[BX]
	JNZ	CPSE
	INC	BX
	INC	DX
	DEC	CH
	JNZ	CP$
	RET
;
CPSE:	MOV	DX,OFFSET NOTDSK
	JMP	ERRPR
;
OPENERR:
	MOV	DX,OFFSET OPNE$
	JMP	ERRPR
;
DSKERR:	MOV	DX,OFFSET ERR80
	CMP	AH,80h
	JZ	GOTERR
	MOV	DX,OFFSET ERR40
	CMP	AH,40h
	JZ	GOTERR
	MOV	DX,OFFSET ERR20
	CMP	AH,20h
	JZ	GOTERR
	MOV	BL,AH
	MOV	BH,0
	ADD	BX,BX
	MOV	DX,OFFSET DETBL
	ADD	BX,DX
	MOV	DL,[BX]
	INC	BX
	MOV	DH,[BX]
GOTERR:	PUSH	DX
	MOV	DX,OFFSET CRLF
	CALL	PRINT
	POP	DX
	JMP	ERRPR
;
STATF:	LAHF
	PUSH	AX
	PUSH	CX
	PUSH	DX
	PUSH	BX
	PUSH	DI
	PUSH	SI
	MOV	BX,OFFSET AHEAD
	MOV	AL,DH
	CALL	DEC3
	MOV	BX,OFFSET ATRK
	MOV	AL,CH
	CALL	DEC3
	MOV	DX,OFFSET FMTS
	CALL	PRINT
	JMP	POPA
;
STATW:	LAHF
	PUSH	AX
	PUSH	CX
	PUSH	DX
	PUSH	BX
	PUSH	DI
	PUSH	SI
	MOV	BX,OFFSET BHEAD
	MOV	AL,DH
	CALL	DEC3
	MOV	BX,OFFSET BTRK
	MOV	AL,CH
	CALL	DEC3
	MOV	BX,OFFSET BSEC
	MOV	AL,CL
	CALL	DEC3
	MOV	DX,OFFSET WRTS
	CALL	PRINT
	JMP	POPA
;
FMTS	DB	'Formatting cylinder '
ATRK	DB	'000 head '
AHEAD	DB	'000           ',CR,'$'

WRTS	DB	'Writing cylinder    '
BTRK	DB	'000 head '
BHEAD	DB	'000 sector '
BSEC	DB	'000',CR,'$'

DEC3:	MOV	DL,100
	CALL	DECD
	MOV	DL,10
	CALL	DECD
	ADD	AL,'0'
	MOV	[BX],AL
	RET
;
DECD:	MOV	DH,'0'
DECD1:	CMP	AL,DL
	JC	DECD2
	INC	DH
	SUB	AL,DL
	JMP	DECD1
;
DECD2:	MOV	[BX],DH
	INC	BX
	RET
;
IAX	DW	0
IBX	DW	0
ICX	DW	0
IDX	DW	0

INT13:	MOV	BP,3
	MOV	IAX,AX
	MOV	IBX,BX
	MOV	ICX,CX
	MOV	IDX,DX
INTLP:	MOV	AX,IAX
	MOV	BX,IBX
	MOV	CX,ICX
	MOV	DX,IDX
	PUSH	BP
	INT	13h
	POP	BP
	JNC	OKAY
	DEC	BP
	JNZ	INTLP
	STC
OKAY:	RET
;
CONFIRM:
	MOV	AL,DRV
	ADD	AL,'A'
	MOV	ADRV,AL
	MOV	DX,OFFSET CONF
	MOV	CL,9
	CALL	FDOS
CONF1:	MOV	CL,6
	MOV	DL,0FFh
	CALL	FDOS
	MOV	DX,OFFSET CRLF
	CMP	AL,'y'
	JZ	CONF2
	CMP	AL,'Y'
	JZ	CONF2
	CMP	AL,'n'
	JZ	CONF3
	CMP	AL,'N'
	JNZ	CONF1
CONF3:	JMP	ERRPR
;
CONF2:	JMP	PRINT
;

;******************************************************
; USQ support code				      *
; 10/12/83 by Dave Rand				      *
;******************************************************
;
; The following labels must be defined in the calling program:
;
dle	equ	090h
;
;this is start of baseline USQ code
;
usq:	xor	al,al		;force init char read
	mov	numlft,al
	mov	rcnt,al		;and zero repeats
	mov	bx,lastmem
	mov	sob,bx
	mov	eob,bx
	call	getw
usq1:	call	getw		;get cksum, and store
	mov	filecrc,bx
usq2:	call	get1
	jz	G0101
	jmp	erext
G0101:	or	al,al
	jz	usq3a
	jmp	usq2
usq3a:	call	getw
	mov	numvals,bx
	mov	dx,OFFSET 258
	call	cmpdehl
	jc	usq3b
	mov	dx,OFFSET ildec
	jmp	errpr

;
usq3b:	mov	dx,OFFSET table
usq4:	mov	max,bx
	mov	al,bh
	or	al,bl
	jnz	G0102
	jmp	usq5

G0102:	push	dx
	call	getw
	pop	dx
	xchg	bx,dx
	mov	[bx],dl
	inc	bx
	mov	[bx],dh
	inc	bx
	push	bx
	call	getw
	xchg	bx,dx
	pop	bx
	mov	[bx],dl
	inc	bx
	mov	[bx],dh
	inc	bx
	xchg	bx,dx
	mov	bx,max
	dec	bx
	jmp	usq4
;
usq5:	mov	bx,0
usq6:	push	bx
	call	getnxt
	pop	bx
	jz	G0103
	jmp	usq8
G0103:	mov	dl,al
	mov	dh,0
	add	bx,dx
	push	bx
	lahf
	push	ax
	MOV	BX,nextadr	; PT TO LOAD ADDRESS
	MOV	AL,topram	; CHECK AGAINST END PAGE OF TPA
	CMP	AL,BH		; IF AT SAME PAGE, YES
	jnz	nofull		;buffer is not full yet
	call	bufull		;buffer full, process buffer
	mov	bx,OFFSET buff	;reset buffer pointer
nofull:	pop	ax
	sahf
	mov	[bx],al
	inc	bx
	mov	nextadr,bx
	pop	bx
	jmp	usq6
;
usq8:	xchg	bx,dx
	mov	bx,filecrc
	call	cmpdehl
usq9:	jnz	usq10
	ret

usq10:	mov	dx,offset ckerr
	jmp	errpr
;
cmpdehl:
	mov	al,bh
	cmp	al,dh
	jz	cdh1
	ret

cdh1:	mov	al,bl
	cmp	al,dl
	ret
;
get1:	mov	bx,eob
	xchg	bx,dx
	mov	bx,sob
	call	cmpdehl
	jz	get1r
	mov	al,[bx]
	inc	bx
	mov	sob,bx
	cmp	al,al
	ret
;
get1r:	mov	bx,lastmem
	mov	sob,bx
	mov	eob,bx
get1r1:	push	bx
	xchg	bx,dx
	mov	cl,26
	call	FDOS
	mov	dx,OFFSET fcb
	mov	cl,20
	call	FDOS
	pop	bx
	cmp	al,3	; [JCE] A partial read is acceptable. The last
	jz	get1r9	; record of the file may be < 128 bytes.
	or	al,al
	jnz	get1r2
get1r9:	mov	dx,OFFSET 128
	add	bx,dx
	xchg	bx,dx
	mov	bx,endmem
	call	cmpdehl
	xchg	bx,dx
	jnc	get1r1
get1r2:	mov	eob,bx
	xchg	bx,dx
	mov	bx,sob
	call	cmpdehl
	jnz	get1
	mov	al,255
	or	al,al
	ret
;
getw:	call	get1
	jnz	badr
	lahf
	push	ax
	call	get1
	jnz	badr
	mov	bh,al
	pop	ax
	sahf
	mov	bl,al
	ret
;
badr:	mov	dx,offset preof
	jmp	errpr
;
getnxt:	mov	al,rcnt		;see if in the middle of
	or	al,al		;repeat sequence...
	jz	getn7
	dec	al
	mov	rcnt,al
	mov	al,last
	cmp	al,al
	ret
getn7:	call	getn4
	cmp	al,dle
	jnz	getn5
	call	getn4
	or	al,al
	jnz	getn6
	mov	al,dle		;dle is encoded as dle,0
	cmp	al,al
	ret
getn6:	dec	al
	dec	al
	mov	rcnt,al
	mov	al,last
	cmp	al,al
	ret
getn5:	mov	last,al
	cmp	al,al
	ret
;
getn4:	mov	dx,OFFSET 0	;pointer @ sot
	mov	al,charch
	mov	cl,al
getn1:	mov	al,numlft
	or	al,al
	jnz	getn2
	push	dx
	call	get1
	jnz	badr
	pop	dx
	mov	cl,al
	mov	al,8
getn2:	dec	al
	mov	numlft,al
	mov	al,cl
	ror	al,1
	mov	cl,al
	mov	bx,OFFSET table
	jnc	getn3
	inc	bx
	inc	bx		;add 2 to point to right node
getn3:	add	bx,dx
	add	bx,dx
	add	bx,dx
	add	bx,dx		;ok.. pointing close to right plc..
	mov	dl,[bx]
	inc	bx
	mov	dh,[bx]
	mov	al,dh
	and	al,128
	jz	getn1
	mov	al,cl
	mov	charch,al
	mov	al,dh
	cmp	al,254		;is special eof?
	mov	al,eof
	jz	geteof
	mov	al,dl
	not	al
	cmp	al,al
	ret
;
geteof:	pop	bx
	or	al,al
	ret
;
;end of baseline USQ code
;
lastmem dw	80h
endmem	dw	80h+127
sob	dw	80h
eob	dw	80h
;
nextadr	dw	buff
numlft	db	0
rcnt	db	0
filecrc	dw	0
last	db	0
charch	db	0
numvals	dw	0
max	dw	0

ildec	db	13,10,'File has illegal decode size. Aborting.',cr,lf,'$'
preof	db	13,10,'Premature EOF on file... aborted.',cr,lf,'$'
ckerr	db	13,10,'ERROR - Checksum error in file ',cr,lf,'$'
topram	db	81h


ENTRY	EndP

;
COMP	DB	0
DRV	DB	0
DETBL	DW	ERR0, ERR1, ERR2, ERR3, ERR4, ERR5, ERR6, ERR7, ERR8, ERR9
ERR0	DB	'Error: Unknown error.',CR,LF,'$'
ERR1	DB	'Error: Invalid parameter.',CR,LF,'$'
ERR2	DB	'Error: Missing address mark.',CR,LF,'$'
ERR3	DB	'Error: Disc is read-only.',CR,LF,'$'
ERR4	DB	'Error: Sector not found / read error.',CR,LF,'$'
ERR5	DB	'Error: Reset failed.',CR,LF,'$'
ERR6	DB	'Error: Disc changed.',CR,LF,'$'
ERR7	DB	'Error: Drive parameter activity failed.',CR,LF,'$'
ERR8	DB	'Error: DMA overrun.',CR,LF,'$'
ERR9	DB	'Error: DMA over 64k boundary.',CR,LF,'$'
ERR20	DB	'Error: Controller failed.',CR,LF,'$'
ERR40	DB	'Error: Seek failed.',CR,LF,'$'
ERR80	DB	'Error: No disc in drive.',CR,LF,'$'
MVCPC	DB	'MV - CPC'
TRKI	DB	'Track-Info'
AD$	DB	CR,LF,'All done!'
CRLF	DB	CR,LF,'$'
NODPB	DB	CR,LF,'Error: Could not determine format for track.',CR,LF,'$'
OPNE$	DB	CR,LF,'Error: Cannot open DSK file.',CR,LF,'$'
SHORTM	DB	CR,LF,'Error: Unexpected end of DSK file.',CR,LF,'$'
NOTDSK	DB	CR,LF,'Error: DSK file is not in the standard MV - CPC format.'
	DB	CR,LF,'$'
STHUF	DB	'(DSK file is squeezed, unsqueezing)         ',cr,lf,'$'
ENDHUF	DB	'(Unsqueezing complete)                      ',cr,lf,'$'

HELPM	DB	'To turn a .DSK into a working disc, inse'
	DB	'rt a blank floppy disc and type:',CR,LF,LF
	DB	'    DSKWRITE filename A:',CR,LF
	DB	'or  DSKWRITE filename B:',CR,LF,'$'
CONF	DB	'Warning: All data on the disc in drive '
ADRV	DB	'X: will be destroyed. Continue (Y/N)? $'
;
; Must be at the end...
;
FMTBUF	EQU	$
DSKH	EQU	FMTBUF+80
TRKH	EQU	DSKH+256
TRKBUF	EQU	TRKH+256
BUFF	EQU	8000h
TABLE	EQU	7000h

CodeSeg ENDS
	END	Entry

B	'/-\|'
;
SPIN:	LAHF
	PUSH	AX
	PUSH	CX
	PUSH	DX
	PUSH	BX
	PUSH	DI
	P