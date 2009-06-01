/*
	Starkos song relocator
	by Targhan/Arkos
	on 22/03/03

	C conversion by PulkoMandy
	on 31/05/09
*/

#include <stdio.h>
#include <stdlib.h>

int DIFFDATA;
char buffer[4];

int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		printf("Usage: %s song newaddr\n",argv[0]);
		exit(1);
	}

	// HL <> argv[1] <> address originale
	// DE <> argv[2] <> nouvelle adresse
	
	FILE* songfile = fopen(argv[1],"r+b");
	if(!songfile)
	{
		puts("unable to open file !");
		exit (2);
	}
	int NEWOFFSET; 
	sscanf(argv[2],"%i",&NEWOFFSET);

	fread(buffer,1,4,songfile);

	if(buffer[0]=='S' && buffer[1]=='K' && buffer[2]=='1' && buffer[3]=='0')
		puts("song opened ok!");
	else
	{
		puts("file is not a compiled starkos song !");
		exit(3);
	}

	// Get base adress of the song data
	int SONGADR;
	fread(buffer,1,2,songfile);
	SONGADR = buffer[0]+buffer[1]*256;
	if(SONGADR == NEWOFFSET)
	{
		puts("song already at this address !");
		exit (0);
	}
	printf("old song address : 0x%x\n",SONGADR);
	DIFFDATA = SONGADR - NEWOFFSET;

	// Code new base address
	fseek(songfile,4,SEEK_SET);
	buffer[0]=NEWOFFSET&0xFF;
	buffer[1]=NEWOFFSET>>8;
	fwrite(buffer,1,2,songfile);

	// Change the offset of some internal pointers
	int b;
	int hl=9;
	for(b=8;b>0;b--)
	{
		change_offset_relative(hl);
		hl-=2;
	}

	int TRACKSTAB = get_word_relative(11);
	int STRACKSTAB = get_word_relative(13);
	int INSTRS = get_word_relative(15);

	//Modify the TRACKS table
	//TODO vérifier si OR A set le carry à 1 ou à 0
	modify_linear_table(STRACKSTAB-TRACKSTAB/2,TRACKSTAB);

	//Modify the STRACKS table

STRLOOP	// HL = STRACKTABS
	ld a,(hl)
	inc hl
	srl a
	jr c,STROLP
	xor a
STROLP	call CHOFF
	hl += 2;
	a --;
	jr nc,STROLP
;
	hl = INSTRS - de;
	or a
	sbc hl,de
	jr nz,STRLOOP


	//Modify the INSTRS table
	ld hl,(INSTRS)
	ld de,(hl)
	ex de,hl			; de = INSTRS et HL = (INSTRS)
	ld bc,(DIFFDATA)
	add hl,bc			; hl = (INSTRS) + DIFFDATA
	ld (INSTEND+1),hl
	or a
	sbc hl,de			; hl = (INSTRS) + DIFFDATA - INSTRS
	bc = (hl /= 2)

	modify_linear_table(bc,INSTRS);



	//Modify the INSTRUMENTS data
	ld hl,(INSTRS)
	ld (PTINSTAB+1),hl

PTINSTAB ld hl,0
	ld e,(hl)
	inc hl
	ld d,(hl)
	inc hl
	ld (PTINSTAB+1),hl

	ex de,hl
	call CHOFF	;Modify instr internal pointers
	inc hl
	inc hl
	call CHOFF

INSTEND ld hl,0	// !!! AUTOMODIF HL < (instrs)+diffdata
	ld de,(PTINSTAB+1)
	or a
	sbc hl,de
	jr nz,PTINSTAB
	ret

	exit (0);
}

/***************
;LOW-LEVEL ROUTINES
;***************/

//Add the offset to the word pointed by HL.
//CHOFF
void change_offset(int address)
{
	fseek(songfile,address,SEEK_SET);
	fread(buffer,1,2,songfile);
	// add the offset
	int fixed = buffer[0]+buffer[1]*256+DIFFDATA;

	// write it back
	fseek(songfile,address,SEEK_SET);
	buffer[0]=fixed&0xFF;
	buffer[1]=fixed>>8;
	fwrite(buffer,1,2,songfile);
}

// CHOFFREL
/*Add the offset to the relative address in HL.*/
void change_offset_relative(int address)
{
	// read the old address
	fseek(songfile,address,SEEK_SET);
	fread(buffer,1,2,songfile);
	// add the offset
	int fixed = buffer[0]+buffer[1]*256+DIFFDATA;

	// write it back
	fseek(songfile,address,SEEK_SET);
	buffer[0]=fixed&0xFF;
	buffer[1]=fixed>>8;
	fwrite(buffer,1,2,songfile);
}

// GETWORDREL
//Return in DE the word pointed by the relative
//adress in HL.
int get_word_relative(int address)
{
	fseek(songfile,address,SEEK_SET);
	fread(buffer,1,2,songfile);
	return buffer[0]+buffer[1]*256;
}

// MODLINTAB
// Modify a linear table, composed of BC elem, pointed by HL.
void modify_linear_table(int count, int address)
{
	do{
		change_offset(address);
		address+=2;
	}while(bc--);
}
