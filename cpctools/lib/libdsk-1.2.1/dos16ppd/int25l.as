
	.globl	large_data
	.globl	large_code
	.psect	ltext,class=CODE,reloc=16

	.globl	_int25_rdwr
	.signat	_int25_rdwr,8250
;
; Enter with DL = read/write op, DH = drive
;
store_bp: .word	0
store_si: .word	0
store_di: .word	0

_int25_rdwr:
	push	ds
	push	es
	push	si
	push	di
	push	bp      ;    00 02 04 06 08 0a 0c  0e 10
	mov	bp,sp	;BP->bp di si ds es rr rr pkt pkt
	mov	al,dl	;AL = 25h or 26h
	mov	cs:intop0+1, al
	mov	cs:intop1, al
	mov	es,16[bp]
	mov	si,14[bp]	;ES:SI -> packet
	mov	ax,es:2[si]	;Sector, high word
	and	ax,ax
	jnz	read_big
	mov	bx,es:6[si]	;Buffer
	mov	ds,es:8[si]
	mov	cx,#1		;1 sector
	mov	al,dh		;Drive
	mov	di,dx
	mov	dx,es:[si]	;Sector
	mov	cs:store_bp,bp
	mov	cs:store_si,si
	mov	cs:store_di,di
intop0:	.byte	0xCD, 0x25	;Do the operation
	mov	bp,cs:store_bp
	mov	si,cs:store_si
	mov	di,cs:store_di
	mov	dx,di
	mov	sp,bp	
	jnc	j_ioret
	cmp	ax,#0x0001
	jz	read_big
	cmp	ax,#0x701F
	jz	read_big
	cmp	ax,#0x0207	;Did it fail with error 0x207? If so,
	stc			;retry using the DOS 3.31 version.
	jnz	ioret

read_big:
	mov	bx,ss:14[bp]	;BX 
	mov	ds,ss:16[bp]	;DS:BX -> packet
	mov	cx,#0xFFFF
doit:	mov	al,dh		;Drive
	mov	cs:store_bp,bp
	mov	cs:store_si,si
	mov	cs:store_di,di
	.byte	0xCD
intop1:	.byte	0x25	
	mov	bp,cs:store_bp
	mov	si,cs:store_si
	mov	di,cs:store_di
	mov	sp,bp	
j_ioret:
	jnc	ioret
	cmp	ax,#0x0001
	jz	read_fat32
	cmp	ax,#0x701F
	jz	read_fat32
	cmp	ax,#0x207	;Did it fail with error 0x207? If so,
	stc			;use the FAT32 version.
	jnz	ioret
read_fat32:
	mov	al,dl		;AL = 25h to read, 26h to write
	mov	ah,0
	sub	al,#0x25	;AX=0 to read, 1 to write
	mov	dl,dh
	inc	dl		;DL = drive
	mov	bx,ss:14[bp]	;BX 
	mov	ds,ss:16[bp]	;DS:BX -> packet
	mov	si,ax		;SI = read/write flags
	mov	ax,#0x7305
	mov	cx,#0xFFFF
	int	#0x21
	mov	sp,bp	
ioret:	jc	ioret1
	mov	ax,#0
ioret1:
	pop	bp
	pop	di
	pop	si
	pop	es
	pop	ds
	retf	#4
;


