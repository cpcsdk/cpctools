;USING INTERNAL PULL UP RESISTOR


;**********************************************************
;USING B REGISTER

DS_TX:		PUSH C
		PUSH D
		MOV D,B
		LDI C,8
DS_TX_L:	LSR B
		BRCC PC+3
		RCALL DS_WRITE_1
		RJMP PC+2
		RCALL DS_WRITE_0
		DEC C
		BRNE DS_TX_L
		POP D
		POP C
		RET

;***********************************************************
;USING B REGISTER

DS_RX:		PUSH C
		LDI C,8
		CLR B
DS_RX_L:	LSR B
		RCALL DS_READ
		DEC C
		BRNE DS_RX_L
		POP C
		RET


;INITIALIZATION STATE
;USING A & C & DS
DS_RESET:	PUSH C
		CBI DSPORT,3
		SBI DSDDR,3
		

		LDI A,255
		LDI C,CRYSTAL
		DEC A
		BRNE PC-1
		DEC C
		BRNE PC-3

		LDI C,255
	

;PULLING THE LINE UP
		SBI DSPORT,3
		CBI DSDDR,3

;CHECKING THAT THE LINE IS PULLED UP
DS_RESET_L0:	SBIC DSPIN,3
		RJMP DS_RESET_L1
		DEC C
		BRNE DS_RESET_L0
		RJMP DS_RESET_NOK


;DETECTING PRESENCE PULSE (GROUND SIGNAL ON THE LINE)
DS_RESET_L1:	LDI C,255

		SBIS DSPIN,3
		RJMP DS_RESET_L2
		
		DEC C
		BRNE DS_RESET_L1+1
		RJMP DS_RESET_NOK

;WAITING FOR PRESENCE PULSE TO FINISH

DS_RESET_L2:	LDI C,255

		SBIC DSPIN,3
		RJMP DS_RESET_OK

		NOP NOP NOP NOP NOP
		DEC C
		BRNE DS_RESET_L2+1
		RJMP DS_RESET_NOK

DS_RESET_OK:	LDI A,CRYSTAL
		DEC A
		BRNE PC-1
		POP C
		LDI A,255
		RET

DS_RESET_NOK:	LDI A,0
		POP C
		RET




;**************************************************8

DS_WRITE_1:	CBI DSPORT,3
		SBI DSDDR,3

		LDI A,CRYSTAL*2			;15 uSecs max timeout
		DEC A
		BRNE PC-1

		SBI DSPORT,3
		CBI DSDDR,3

		LDI A,20*CRYSTAL		;60 uSecs minimum
		NOP NOP NOP NOP NOP
		DEC A
		BRNE PC-6
		RET




;******************************************


DS_WRITE_0:	CBI DSPORT,3
		SBI DSDDR,3

		LDI A,20*CRYSTAL		;60 uSecs minimum
		NOP NOP NOP NOP NOP
		DEC A
		BRNE PC-6

		SBI DSPORT,3



		CPI D,$44
		BRNE DS_WRITE_0A
		CPI C,1
		BRNE DS_WRITE_0A
		RJMP DS_WRITE_0B



DS_WRITE_0A:	CBI DSDDR,3
DS_WRITE_0B:	LDI A,CRYSTAL
		DEC A
		BRNE PC-1
		RET



;***********************************************


DS_READ:	CBI DSPORT,3
		SBI DSDDR,3

		LDI A,CRYSTAL*3			;1 uSecs min
		DEC A
		BRNE PC-1

		SBI DSPORT,3
		CBI DSDDR,3

		LDI A,CRYSTAL*3			;15 uSecs max
		DEC A
		BRNE PC-1
		
		NOP

		SBIC DSPIN,3
		ORI B,0B10000000

		LDI A,20*CRYSTAL		;60 uSecs minimum
		NOP NOP NOP NOP NOP
		DEC A
		BRNE PC-6
		RET
