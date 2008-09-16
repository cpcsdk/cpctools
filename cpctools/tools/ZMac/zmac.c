/* A Bison parser, made by GNU Bison 1.875d.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     STRING = 258,
     NOOPERAND = 259,
     ARITHC = 260,
     ADD = 261,
     LOGICAL = 262,
     AND = 263,
     OR = 264,
     XOR = 265,
     BIT = 266,
     RES = 267,
     CALL = 268,
     INCDEC = 269,
     DJNZ = 270,
     EX = 271,
     IM = 272,
     PHASE = 273,
     DEPHASE = 274,
     IN = 275,
     JP = 276,
     JR = 277,
     LD = 278,
     OUT = 279,
     PUSHPOP = 280,
     RET = 281,
     SHIFT = 282,
     RST = 283,
     REGNAME = 284,
     ACC = 285,
     C = 286,
     RP = 287,
     HL = 288,
     INDEX = 289,
     HIDREG = 290,
     AF = 291,
     SP = 292,
     MISCREG = 293,
     F = 294,
     COND = 295,
     SPCOND = 296,
     NUMBER = 297,
     UNDECLARED = 298,
     END = 299,
     ORG = 300,
     DEFB = 301,
     DEFS = 302,
     DEFW = 303,
     EQU = 304,
     DEFL = 305,
     LABEL = 306,
     EQUATED = 307,
     WASEQUATED = 308,
     DEFLED = 309,
     MULTDEF = 310,
     MOD = 311,
     SHL = 312,
     SHR = 313,
     NOT = 314,
     LT = 315,
     GT = 316,
     EQ = 317,
     LE = 318,
     GE = 319,
     NE = 320,
     IF = 321,
     ELSE = 322,
     ENDIF = 323,
     ARGPSEUDO = 324,
     LIST = 325,
     MINMAX = 326,
     MACRO = 327,
     MNAME = 328,
     OLDMNAME = 329,
     ARG = 330,
     ENDM = 331,
     MPARM = 332,
     ONECHAR = 333,
     TWOCHAR = 334,
     UNARY = 335
   };
#endif
#define STRING 258
#define NOOPERAND 259
#define ARITHC 260
#define ADD 261
#define LOGICAL 262
#define AND 263
#define OR 264
#define XOR 265
#define BIT 266
#define RES 267
#define CALL 268
#define INCDEC 269
#define DJNZ 270
#define EX 271
#define IM 272
#define PHASE 273
#define DEPHASE 274
#define IN 275
#define JP 276
#define JR 277
#define LD 278
#define OUT 279
#define PUSHPOP 280
#define RET 281
#define SHIFT 282
#define RST 283
#define REGNAME 284
#define ACC 285
#define C 286
#define RP 287
#define HL 288
#define INDEX 289
#define HIDREG 290
#define AF 291
#define SP 292
#define MISCREG 293
#define F 294
#define COND 295
#define SPCOND 296
#define NUMBER 297
#define UNDECLARED 298
#define END 299
#define ORG 300
#define DEFB 301
#define DEFS 302
#define DEFW 303
#define EQU 304
#define DEFL 305
#define LABEL 306
#define EQUATED 307
#define WASEQUATED 308
#define DEFLED 309
#define MULTDEF 310
#define MOD 311
#define SHL 312
#define SHR 313
#define NOT 314
#define LT 315
#define GT 316
#define EQ 317
#define LE 318
#define GE 319
#define NE 320
#define IF 321
#define ELSE 322
#define ENDIF 323
#define ARGPSEUDO 324
#define LIST 325
#define MINMAX 326
#define MACRO 327
#define MNAME 328
#define OLDMNAME 329
#define ARG 330
#define ENDM 331
#define MPARM 332
#define ONECHAR 333
#define TWOCHAR 334
#define UNARY 335




/* Copy the first part of user declarations.  */
#line 1 "zmac.y"

/*
 *  zmac -- macro cross-assembler for the Zilog Z80 microprocessor
 *
 *  Bruce Norskog	4/78
 *
 *  Last modification 2000-07-01 by mgr
 *
 *  This assembler is modeled after the Intel 8080 macro cross-assembler
 *  for the Intel 8080 by Ken Borgendale.  The major features are:
 *	1.  Full macro capabilities
 *	2.  Conditional assembly
 *	3.  A very flexible set of listing options and pseudo-ops
 *	4.  Symbol table output
 *	5.  Error report
 *	6.  Elimination of sequential searching
 *	7.  Commenting of source
 *	8.  Facilities for system definiton files
 *
 * (Revision history is now in ChangeLog. -rjm)
 */

#define ZMAC_VERSION	"2.0"
#define ZMAC_BETA	"b2"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "mio.h"
#include "getoptn.h"

#define UNUSED(var) ((void) var)

#if defined (__riscos__) && !defined (__riscos)
#define __riscos
#endif

#ifdef __riscos
#include "swis.h"
#define DDEUtils_Prefix		0x42580
#define DDEUtils_ThrowbackStart	0x42587
#define DDEUtils_ThrowbackSend	0x42588
#define DDEUtils_ThrowbackEnd	0x42589
#endif

#ifndef OS_DIR_SEP
#if defined (MSDOS)
#define OS_DIR_SEP '\\'
#elif defined (__riscos)
#define OS_DIR_SEP '.'
#else
#define OS_DIR_SEP '/'
#endif
#endif

#ifndef OS_EXT_SEP
#if defined (__riscos)
#define OS_EXT_SEP '/'
#else
#define OS_EXT_SEP '.'
#endif
#endif

/*
 * DEBUG turns on pass reporting.
 * Macro debug and Token debug enables.
#define	DEBUG
#define	M_DEBUG
#define	T_DEBUG
 */

#define ITEMTABLESIZE	2000
#define TEMPBUFSIZE	200
#define LINEBUFFERSIZE	200
#define EMITBUFFERSIZE	200
#define MAXSYMBOLSIZE	40
#define IFSTACKSIZE	20
#define MAXIFS		1024
#define TITLELEN	50
#define BINPERLINE	16
#define	PARMMAX		25
#define MAXEXP		25
#define SYMMAJIC	07203
#define	NEST_IN		8


#define loop	for(;;)

void yyerror(char *err)
{
	UNUSED (err);		/* we will do our own error printing */
	/* printf ("Oops! %s\n", err); */
}

struct	item	{
	char	*i_string;
	int	i_value;
	int	i_token;
	int	i_uses;
	int	i_equbad;
};

FILE	*fout,
	*fbuf,
	*fin[NEST_IN],
	*now_file ;

int	pass2;	/* set when pass one completed */
int	dollarsign ;	/* location counter */
int	olddollar ;	/* kept to put out binary */

/* program counter save for PHASE/DEPHASE */
int	phdollar, phbegin, phaseflag ;

char	*src_name[NEST_IN] ;
int	linein[NEST_IN] ;
int	now_in ;


#define bflag	0	/* balance error */
#define eflag	1	/* expression error */
#define fflag	2	/* syntax error */
#define iflag	3	/* bad digits */
#define mflag	4	/* multiply defined */
#define pflag	5	/* phase error */
#define uflag	6	/* undeclared used */
#define vflag	7	/* value out of range */
#define oflag	8	/* phase/dephase error */
#define frflag	9	/* double forward ref. via equ error */
#define zflag	10	/* Z80-only instruction (when `-z' option in use) */
#define orgflag 11	/* retrograde org error (when `-h' option not in use) */

#define FLAGS	12	/* number of flags */

char	err[FLAGS];
int	keeperr[FLAGS];
char	errlet[FLAGS]="BEFIMPUVORZG";
char	*errname[FLAGS]={
	"Balance",
	"Expression",
	"Syntax",
	"Digit",
	"Mult. def.",
	"Phase",
	"Undeclared",
	"Value",
	"Phase/Dephase",
	"Forward ref. to EQU with forward ref.",
	"Z80-specific instruction",
	"Retrograde ORG"
};
char	*warnname[]={
	"Symbol length exceeded",
	"Non-standard syntax",
	"Could replace JP with JR",
	"Could replace LD A, 0 with XOR A if flags unimportant",
	"Could replace RLC A with RLCA if S, Z and P/V flags unimportant",
	"Could replace RRC A with RRCA if S, Z and P/V flags unimportant",
	"Could replace RL A with RLA if S, Z and P/V flags unimportant",
	"Could replace RR A with RRA if S, Z and P/V flags unimportant",
	"Could replace SLA A with ADD A, A if H and P/V flags unimportant"
};

/* for "0 symbols", "1 symbol", "2 symbols", etc. */
#define DO_PLURAL(x)	(x),((x)==1)?"":"s"

char	linebuf[LINEBUFFERSIZE];
char	*lineptr;
char	*linemax = linebuf+LINEBUFFERSIZE;

char	outbin[BINPERLINE];
char	*outbinp = outbin;
char	*outbinm = outbin+BINPERLINE;

char	emitbuf[EMITBUFFERSIZE];
char	*emitptr;

char	ifstack[IFSTACKSIZE];
char	*ifptr;
char	*ifstmax = ifstack+IFSTACKSIZE-1;


char	expif[MAXIFS];
char	*expifp;
char	*expifmax = expif+MAXIFS;

char	hexadec[] = "0123456789ABCDEF" ;
char	*expstack[MAXEXP];
int	expptr;


int	nitems;
int	linecnt;
int	nbytes;
int	invented;


char	tempbuf[TEMPBUFSIZE];
char	*tempmax = tempbuf+TEMPBUFSIZE-1;

char	inmlex;
char	arg_flag;
char	quoteflag;
int	parm_number;
int	exp_number;
char	symlong[] = "Symbol too long";

int	disp;
#define FLOC	PARMMAX
#define TEMPNUM	PARMMAX+1
char	**est;
char	**est2;

char	*floc;
int	mfptr;
FILE	*mfile;


char	*title;
char	titlespace[TITLELEN];
char	*timp;
char	*sourcef;
/* changed to cope with filenames longer than 14 chars -rjm 1998-12-15 */
char	src[1024];
char	bin[1024];
char	mtmp[1024];
char	listf[1024];
char	writesyms[1024];
#ifdef __riscos
char	riscos_thbkf[1024];
#endif

char	bopt = 1,
	edef = 1,
	eopt = 1,
	fdef = 0,
	fopt = 0,
	gdef = 1,
	gopt = 1,
	iopt = 0 ,	/* list include files */
	lstoff = 0,
	lston = 0,	/* flag to force listing on */
	lopt = 0,
	mdef = 0,
	mopt = 0,
	nopt = 1,	/* line numbers on as default */
	oldoopt = 0,
	popt = 1,	/* form feed as default page eject */
	sopt = 0,	/* turn on symbol table listing */
	output_hex = 0,	/* `-h', output .hex rather than .bin -rjm */
	output_8080_only = 0,	/* `-z', output 8080-compat. ops only -rjm */
	show_error_line = 0,	/* `-S', show line which caused error -rjm */
	terse_lst_errors = 0,	/* `-t', terse errors in listing -rjm */
	continuous_listing = 1,	/* `-d', discontinuous - with page breaks */
	suggest_optimise = 0,	/* `-O', suggest optimisations -mgr */
#ifdef __riscos
	riscos_thbk = 0,	/* `-T', RISC OS throwback -mep */
#endif
	output_amsdos = 0,	/* `-A', AMSDOS binary file output -mep */
	saveopt;

char	xeq_flag = 0;
int	xeq;

time_t	now;
int	line;
int	page = 1;

int	had_errors = 0;		/* if program had errors, do exit(1) */
#ifdef __riscos
int	riscos_throwback_started = 0;
#endif
int	not_seen_org = 1;
int	first_org_store = 0;

struct stab {
	char	t_name[MAXSYMBOLSIZE+1];
	int	t_value;
	int	t_token;
};

/*
 *  push back character
 */
int	peekc;


/* function prototypes */
int addtoline(int ac);
int iflist(void);
int yylex(void);
int tokenofitem(int deftoken);
int nextchar(void);
int skipline(int ac);
void usage(void);
int main(int argc, char *argv[]);
int getarg(void);
int getm(void);
void yyerror(char *err);
void emit(int num, ...);
void emit1(int opcode,int regvalh,int data16,int type);
void emitdad(int rp1,int rp2);
void emitjr(int opcode,int expr);
void emitjp(int opcode,int expr);
void putbin(int v);
void flushbin(void);
void puthex(char byte, FILE *buf);
void list(int optarg);
void lineout(void);
void eject(void);
void space(int n);
void lsterr1(void);
void lsterr2(int lst);
void errorprt(int errnum);
void warnprt(int warnnum, int warnoff);
void list1(void);
void interchange(int i, int j);
void custom_qsort(int m, int n);
void setvars(void);
void error(char *as);
void fileerror(char *as,char *filename);
void justerror(char *as);
void putsymtab(void);
void erreport(void);
void mlex(void);
void suffix_if_none(char *str,char *suff);
void suffix(char *str,char *suff);
void decanonicalise(char *str);
void putm(char c);
void popsi(void);
char *getlocal(int c, int n);
void insymtab(char *name);
void outsymtab(char *name);
void copyname(char *st1, char *st2);
void next_source(char *sp);
void doatexit (void);
#ifdef __riscos
void riscos_set_csd(char *sp);
void riscos_throwback(int severity, char *file, int line, char *error);
#endif




/*
 *  add a character to the output line buffer
 */
int addtoline(int ac)
{
	/* check for EOF from stdio */
	if (ac == -1)
		ac = 0 ;
	if (inmlex)
		return(ac);
	if (lineptr >= linemax)
		error("line buffer overflow");
	*lineptr++ = ac;
	return(ac);
}


/*
 *  put values in buffer for outputing
 */

void emit(int bytes, ...)
{
	va_list ap;
	unsigned char *oldemitptr=(unsigned char *)emitptr;
	int c;

	va_start(ap,bytes);

	while	(--bytes >= 0)
		if (emitptr >= &emitbuf[EMITBUFFERSIZE])
			error("emit buffer overflow");
		else {
			*emitptr++ = va_arg(ap,int);
		}

	if (output_8080_only) {
		/* test for Z80-specific ops. These start with one of
		 * sixteen byte values, listed below. The values were
		 * taken from "A Z80 Workshop Manual" by E. A. Parr. -rjm
		 */
		/* As far as I can tell from my own literature
		 * review, 0x02, 0x0a, 0x12 and 0x1a are valid
		 * 8080 opcodes (LDAX/STAX B/D) -mgr
		 */
		c=*oldemitptr;
		if (/* c==0x02 || */ c==0x08 || /* c==0x0a || */ c==0x10 ||
		    /* c==0x12 || */ c==0x18 || /* c==0x1a || */ c==0x20 ||
		    c==0x28 || c==0x30 || c==0x38 || c==0xcb ||
		    c==0xd9 || c==0xdd || c==0xed || c==0xfd)
			err[zflag]++;
	}

	va_end(ap);
}

/* for emitted data - as above, without 8080 test.
 * Duplicating the code was easier than putting an extra arg in all
 * those emit()s. :-} Hopefully this isn't too unbearably nasty. -rjm
 */
void dataemit(int bytes, ...)
{
	va_list ap;

	va_start(ap,bytes);

	while	(--bytes >= 0)
		if (emitptr >= &emitbuf[EMITBUFFERSIZE])
			error("emit buffer overflow");
		else {
			*emitptr++ = va_arg(ap,int);
		}
	va_end(ap);
}


void emit1(int opcode,int regvalh,int data16,int type)
{
	if ((regvalh & 0x8000)) {	/* extra brackets to silence -Wall */
		if ((type & 1) == 0 && (disp > 127 || disp < -128) && ((regvalh & 7)!=0))
			err[vflag]++;
		switch(type) {
		case 0:
			if (opcode & 0x8000)
				emit(4, regvalh >> 8, opcode >> 8, disp, opcode);
			else
				if (regvalh & 7)
					emit(2,regvalh >> 8, opcode);
				else
					emit(3, regvalh >> 8, opcode, disp);
			break;
		case 1:
			emit(2, regvalh >> 8, opcode);
			break;
		case 2:
			if (data16 > 255 || data16 < -128)
				err[vflag]++;
			if(regvalh & 7)
				emit(3,regvalh >> 8, opcode,data16);
			else
				emit(4, regvalh >> 8, opcode, disp, data16);
			break;
		case 3:
			emit(2,regvalh >> 8, opcode);
			break;
		case 5:
			emit(4, regvalh >> 8, opcode, data16, data16 >> 8);
		}
	} else
		switch(type) {
		case 0:
			if (opcode & 0100000)
				emit(2, opcode >> 8, opcode);
			else
				emit(1, opcode);
			break;
		case 1:
			if (opcode & 0100000)
				emit(2, opcode >> 8, opcode);
			else
				emit(1, opcode);
			break;
		case 2:
			if (data16 > 255 || data16 < -128)
				err[vflag]++;
			emit(2, opcode, data16);
			break;
		case 3:
			if (data16 >255 || data16 < -128)
				err[vflag]++;
			emit(2, opcode, data16);
			break;
		case 5:
			if (opcode & 0100000)
				emit(4, opcode >> 8, opcode, data16, data16 >> 8);
			else
				emit(3, opcode, data16, data16 >> 8);
		}
}




void emitdad(int rp1,int rp2)
{
	if (rp1 & 0x8000)
		emit(2,rp1 >> 8, rp2 + 9);
	else
		emit(1,rp2 + 9);
}


void emitjr(int opcode,int expr)
{
	disp = expr - dollarsign - 2;
	if (disp > 127 || disp < -128)
		err[vflag]++;
	emit(2, opcode, disp);
}



void emitjp(int opcode,int expr)
{
	if (suggest_optimise && pass2 && opcode <= 0xda && !output_8080_only) {
		disp = expr - dollarsign - 2;
		if (disp <= 127 && disp >= -128)
			warnprt (2, 0);
	}
	emit(3, opcode, expr, expr >> 8);
}




/*
 *  put out a byte of binary
 */
void putbin(int v)
{
	if(!pass2 || !bopt) return;
	*outbinp++ = v;
	if (outbinp >= outbinm) flushbin();
}



/*
 *  output one line of binary in INTEL standard form
 */
void flushbin()
{
	char *p;
	int check=outbinp-outbin;

	if (!pass2 || !bopt)
		return;
	nbytes += check;
	if (check) {
		if (output_hex) {
			putc(':', fbuf);
			puthex(check, fbuf);
			puthex(olddollar>>8, fbuf);
			puthex(olddollar, fbuf);
			puthex(0, fbuf);
		}
		check += (olddollar >> 8) + olddollar;
		olddollar += (outbinp-outbin);
		for (p=outbin; p<outbinp; p++) {
			if (output_hex)
				puthex(*p, fbuf);
			else
				fputc(*p, fbuf);
			check += *p;
		}
		if (output_hex) {
			puthex(256-check, fbuf);
			putc('\n', fbuf);
		}
		outbinp = outbin;
	}
}



/*
 *  put out one byte of hex
 */
void puthex(char byte, FILE *buf)
{
	putc(hexadec[(byte >> 4) & 017], buf);
	putc(hexadec[byte & 017], buf);
}

/*
 *  put out a line of output -- also put out binary
 */
void list(int optarg)
{
	char *	p;
	int	i;
	int  lst;

	if (!expptr)
		linecnt++;
	addtoline('\0');
	if (pass2) {
		lst = iflist();
		if (lst) {
			lineout();
			if (nopt)
				fprintf(fout, "%4d:\t", linein[now_in]);
			puthex(optarg >> 8, fout);
			puthex(optarg, fout);
			fputs("  ", fout);
			for (p = emitbuf; (p < emitptr) && (p - emitbuf < 4); p++) {
				puthex(*p, fout);
			}
			for (i = 4 - (p-emitbuf); i > 0; i--)
				fputs("  ", fout);
			putc('\t', fout);
			fputs(linebuf, fout);
		}

		if (bopt) {
			for (p = emitbuf; p < emitptr; p++)
				putbin(*p);
		}


		p = emitbuf+4;
		while (lst && gopt && p < emitptr) {
			lineout();
			if (nopt) putc('\t', fout);
			fputs("      ", fout);
			for (i = 0; (i < 4) && (p < emitptr);i++) {
				puthex(*p, fout);
				p++;
			}
			putc('\n', fout);
		}


		lsterr2(lst);
	} else
		lsterr1();
	dollarsign += emitptr - emitbuf;
	emitptr = emitbuf;
	lineptr = linebuf;
}



/*
 *  keep track of line numbers and put out headers as necessary
 */
void lineout()
{
	if (continuous_listing) {
		line = 1;
		return;
	}
	if (line == 60) {
		if (popt)
			putc('\014', fout);	/* send the form feed */
		else
			fputs("\n\n\n\n\n", fout);
		line = 0;
	}
	if (line == 0) {
		fprintf(fout, "\n\n%s %s\t%s\t Page %d\n\n\n",
			&timp[4], &timp[20], title, page++);
		line = 4;
	}
	line++;
}


/*
 *  cause a page eject
 */
void eject()
{
	if (pass2 && !continuous_listing && iflist()) {
		if (popt) {
			putc('\014', fout);	/* send the form feed */
		} else {
			while (line < 65) {
				line++;
				putc('\n', fout);
			}
		}
	}
	line = 0;
}


/*
 *  space n lines on the list file
 */
void space(int n)
{
	int	i ;
	if (pass2 && iflist())
		for (i = 0; i<n; i++) {
			lineout();
			putc('\n', fout);
		}
}


/*
 *  Error handling - pass 1
 */
void lsterr1()
{
	int i;
	for (i = 0; i <= 4; i++)
		if (err[i]) {
			errorprt(i);
			err[i] = 0;
		}
}


/*
 *  Error handling - pass 2.
 */
void lsterr2(int lst)
{
	int i;
	for (i=0; i<FLAGS; i++)
		if (err[i]) {
			if (lst) {
				lineout();
				/* verbose inline error messages now,
				 * must override with `-t' to get old
				 * behaviour. -rjm
				 */
				if (terse_lst_errors)
					putc(errlet[i], fout);
				else
					fprintf(fout,"*** %s error ***",
							errname[i]);
				putc('\n', fout);
			}
			err[i] = 0;
			keeperr[i]++;
			if (i > 4)
				errorprt(i);
		}

	fflush(fout);	/* to avoid putc(har) mix bug */
}

/*
 *  print diagnostic to error terminal
 */
void errorprt(int errnum)
{
	had_errors=1;
	fprintf(stderr,"%s:%d: %s error\n",
		src_name[now_in], linein[now_in], errname[errnum]);
	if(show_error_line)
		fprintf(stderr, "%s\n", linebuf);
#ifdef __riscos
	if (riscos_thbk)
		riscos_throwback (1, src_name[now_in], linein[now_in], errname[errnum]);
#endif
}


/*
 *  print warning to error terminal
 */
void warnprt(int warnnum, int warnoff)
{
	fprintf(stderr,"%s:%d: warning: %s\n",
		src_name[now_in], linein[now_in] + warnoff, warnname[warnnum]);
		/* Offset needed if warning issued while line is being parsed */
#ifdef __riscos
	if (riscos_thbk)
		riscos_throwback (0, src_name[now_in], linein[now_in] + warnoff, warnname[warnnum]);
#endif
	/* if(show_error_line)
		Can't show line because it isn't necessarily complete
		fprintf(stderr, "%s\n", linebuf); */
}


/*
 *  list without address -- for comments and if skipped lines
 */
void list1()
{
	int lst;

	addtoline('\0');
	lineptr = linebuf;
	if (!expptr) linecnt++;
	if (pass2)
	{
		if ((lst = iflist())) {
			lineout();
			if (nopt)
				fprintf(fout, "%4d:\t", linein[now_in]);
			fprintf(fout, "\t\t%s", linebuf);
			lsterr2(lst);
		}
	} else
		lsterr1();
}


/*
 *  see if listing is desired
 */
int iflist()
{
	int i, j;

	if (lston)
		return(1) ;
	if (lopt)
		return(0);
	if (*ifptr && !fopt)
		return(0);
	if (!lstoff && !expptr)
		return(1);
	j = 0;
	for (i=0; i<FLAGS; i++)
		if (err[i])
			j++;
	if (expptr)
		return(mopt || j);
	if (eopt && j)
		return(1);
	return(0);
}


/* moved out of %{..%} bit in parse routine because `bison -y'
 * didn't like it... -rjm
 */
char  *cp;

int list_tmp1,list_tmp2;
int equ_bad_label=0;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 838 "zmac.y"
typedef union YYSTYPE {
	struct item *itemptr;
	int ival;
	char *cval;
	} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 1079 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 1091 "y.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2208

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  104
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  38
/* YYNRULES -- Number of rules. */
#define YYNRULES  203
/* YYNRULES -- Number of states. */
#define YYNSTATES  366

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   335

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      94,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    91,     2,     2,   101,    90,    82,   100,
      98,    99,    88,    86,    95,    87,    97,    89,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    96,     2,
      84,    83,    85,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   102,     2,   103,    81,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    80,     2,    92,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    93
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    14,    19,    24,    31,
      35,    38,    41,    45,    50,    55,    61,    66,    69,    73,
      78,    81,    87,    89,    91,    93,    94,    96,    97,   100,
     103,   105,   108,   111,   114,   117,   122,   125,   130,   133,
     136,   139,   142,   147,   152,   157,   162,   165,   170,   173,
     178,   181,   184,   187,   190,   195,   200,   205,   210,   213,
     218,   221,   226,   231,   236,   239,   242,   247,   252,   259,
     264,   269,   272,   277,   280,   285,   288,   290,   293,   298,
     303,   310,   315,   322,   327,   332,   337,   342,   347,   352,
     357,   362,   370,   377,   382,   389,   396,   401,   408,   415,
     418,   421,   423,   426,   429,   432,   434,   435,   437,   441,
     443,   444,   446,   450,   452,   454,   456,   458,   460,   462,
     464,   466,   468,   470,   474,   476,   481,   485,   487,   489,
     491,   493,   495,   497,   499,   501,   503,   505,   507,   509,
     511,   513,   515,   517,   521,   523,   525,   527,   529,   533,
     535,   537,   539,   541,   543,   547,   549,   551,   553,   555,
     557,   559,   561,   563,   565,   569,   573,   577,   581,   585,
     589,   593,   597,   601,   605,   609,   613,   617,   621,   625,
     629,   633,   637,   641,   645,   649,   653,   657,   661,   664,
     667,   670,   673,   676,   678,   680,   682,   684,   686,   688,
     689,   690,   691,   692
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     105,     0,    -1,    -1,   105,   106,    -1,   109,    94,    -1,
     109,   110,    94,    -1,   136,    49,   133,    94,    -1,   136,
      50,   133,    94,    -1,   136,    71,   133,    95,   133,    94,
      -1,    66,   133,    94,    -1,    67,    94,    -1,    68,    94,
      -1,   109,    44,    94,    -1,   109,    44,   133,    94,    -1,
     109,    47,   133,    94,    -1,    69,   138,    75,   139,    94,
      -1,    69,   138,   139,    94,    -1,    70,    94,    -1,    70,
     133,    94,    -1,    43,    72,   111,    94,    -1,    74,    72,
      -1,   109,    73,   137,   113,    94,    -1,     1,    -1,    96,
      -1,    97,    -1,    -1,    96,    -1,    -1,   107,   136,    -1,
     136,   108,    -1,     4,    -1,    21,   133,    -1,    13,   133,
      -1,    28,   133,    -1,     6,   133,    -1,     6,    30,    95,
     133,    -1,     5,   133,    -1,     5,    30,    95,   133,    -1,
       7,   133,    -1,     8,   133,    -1,     9,   133,    -1,    10,
     133,    -1,     7,    30,    95,   133,    -1,     8,    30,    95,
     133,    -1,     9,    30,    95,   133,    -1,    10,    30,    95,
     133,    -1,     6,   115,    -1,     6,    30,    95,   115,    -1,
       5,   115,    -1,     5,    30,    95,   115,    -1,     7,   115,
      -1,     8,   115,    -1,     9,   115,    -1,    10,   115,    -1,
       7,    30,    95,   115,    -1,     8,    30,    95,   115,    -1,
       9,    30,    95,   115,    -1,    10,    30,    95,   115,    -1,
      27,   115,    -1,    27,   115,    95,   115,    -1,    14,   115,
      -1,     5,    33,    95,   124,    -1,     6,   125,    95,   123,
      -1,     6,   125,    95,   125,    -1,    14,   121,    -1,    25,
     122,    -1,    11,   133,    95,   115,    -1,    12,   133,    95,
     115,    -1,    12,   133,    95,   120,    95,   116,    -1,    21,
     126,    95,   133,    -1,    21,    98,   125,    99,    -1,    21,
     125,    -1,    13,   126,    95,   133,    -1,    22,   133,    -1,
      22,   127,    95,   133,    -1,    15,   133,    -1,    26,    -1,
      26,   126,    -1,    23,   115,    95,   115,    -1,    23,   115,
      95,   135,    -1,    23,   115,    95,    98,    32,    99,    -1,
      23,   115,    95,   134,    -1,    23,    98,    32,    99,    95,
      30,    -1,    23,   134,    95,    30,    -1,    23,   115,    95,
      38,    -1,    23,    38,    95,    30,    -1,    23,   121,    95,
     132,    -1,    23,   121,    95,   134,    -1,    23,   134,    95,
     121,    -1,    23,   121,    95,   125,    -1,    16,    32,    95,
      33,    -1,    16,    36,    95,    36,   140,   100,   141,    -1,
      16,    98,    37,    99,    95,   125,    -1,    20,   118,    95,
     134,    -1,    20,   118,    95,    98,    31,    99,    -1,    20,
      39,    95,    98,    31,    99,    -1,    24,   134,    95,    30,
      -1,    24,    98,    31,    99,    95,   118,    -1,    24,    98,
      31,    99,    95,   133,    -1,    17,   133,    -1,    18,   133,
      -1,    19,    -1,    45,   133,    -1,    46,   128,    -1,    48,
     130,    -1,    76,    -1,    -1,   112,    -1,   111,    95,   112,
      -1,    43,    -1,    -1,   114,    -1,   113,    95,   114,    -1,
      75,    -1,   118,    -1,   119,    -1,   117,    -1,   118,    -1,
     117,    -1,    35,    -1,    29,    -1,    30,    -1,    31,    -1,
      98,    33,    99,    -1,   120,    -1,    98,    34,   133,    99,
      -1,    98,    34,    99,    -1,   123,    -1,   125,    -1,    32,
      -1,    36,    -1,   125,    -1,    32,    -1,    37,    -1,   123,
      -1,    33,    -1,    33,    -1,    34,    -1,   127,    -1,    40,
      -1,    41,    -1,    31,    -1,   129,    -1,   128,    95,   129,
      -1,    79,    -1,     3,    -1,   133,    -1,   131,    -1,   130,
      95,   131,    -1,   133,    -1,   135,    -1,    79,    -1,   134,
      -1,   135,    -1,    98,   133,    99,    -1,    51,    -1,    42,
      -1,    78,    -1,    52,    -1,    53,    -1,    54,    -1,   101,
      -1,    43,    -1,    55,    -1,   133,    86,   133,    -1,   133,
      87,   133,    -1,   133,    89,   133,    -1,   133,    88,   133,
      -1,   133,    90,   133,    -1,   133,    56,   133,    -1,   133,
      82,   133,    -1,   133,     8,   133,    -1,   133,    80,   133,
      -1,   133,     9,   133,    -1,   133,    81,   133,    -1,   133,
      10,   133,    -1,   133,    57,   133,    -1,   133,    58,   133,
      -1,   133,    84,   133,    -1,   133,    83,   133,    -1,   133,
      85,   133,    -1,   133,    60,   133,    -1,   133,    62,   133,
      -1,   133,    61,   133,    -1,   133,    63,   133,    -1,   133,
      64,   133,    -1,   133,    65,   133,    -1,   102,   133,   103,
      -1,    59,   133,    -1,    92,   133,    -1,    91,   133,    -1,
      86,   133,    -1,    87,   133,    -1,    43,    -1,    51,    -1,
      55,    -1,    52,    -1,    53,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   939,   939,   942,   947,   952,   956,   987,  1000,  1015,
    1041,  1050,  1056,  1061,  1068,  1087,  1124,  1130,  1136,  1184,
    1196,  1212,  1229,  1257,  1259,  1262,  1265,  1270,  1272,  1295,
    1321,  1324,  1327,  1330,  1342,  1345,  1348,  1351,  1354,  1357,
    1360,  1363,  1366,  1369,  1372,  1375,  1378,  1382,  1386,  1389,
    1392,  1395,  1398,  1401,  1404,  1407,  1410,  1413,  1416,  1425,
    1434,  1437,  1444,  1447,  1456,  1459,  1462,  1469,  1476,  1483,
    1486,  1489,  1492,  1495,  1498,  1501,  1504,  1507,  1510,  1521,
    1528,  1536,  1545,  1548,  1551,  1560,  1563,  1566,  1574,  1582,
    1592,  1602,  1605,  1608,  1621,  1624,  1627,  1634,  1637,  1647,
    1655,  1667,  1677,  1703,  1705,  1707,  1711,  1713,  1715,  1720,
    1730,  1733,  1735,  1740,  1751,  1753,  1755,  1758,  1760,  1763,
    1769,  1774,  1779,  1785,  1790,  1793,  1799,  1806,  1808,  1811,
    1816,  1821,  1824,  1829,  1835,  1837,  1843,  1848,  1854,  1856,
    1862,  1867,  1871,  1873,  1876,  1881,  1888,  1898,  1900,  1905,
    1914,  1916,  1920,  1922,  1926,  1931,  1934,  1936,  1938,  1941,
    1953,  1956,  1959,  1966,  1969,  1972,  1975,  1978,  1981,  1984,
    1987,  1990,  1993,  1996,  1999,  2002,  2005,  2008,  2011,  2014,
    2017,  2020,  2023,  2026,  2029,  2032,  2035,  2038,  2041,  2044,
    2047,  2050,  2053,  2058,  2060,  2062,  2064,  2066,  2068,  2073,
    2086,  2090,  2094,  2098
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "STRING", "NOOPERAND", "ARITHC", "ADD",
  "LOGICAL", "AND", "OR", "XOR", "BIT", "RES", "CALL", "INCDEC", "DJNZ",
  "EX", "IM", "PHASE", "DEPHASE", "IN", "JP", "JR", "LD", "OUT", "PUSHPOP",
  "RET", "SHIFT", "RST", "REGNAME", "ACC", "C", "RP", "HL", "INDEX",
  "HIDREG", "AF", "SP", "MISCREG", "F", "COND", "SPCOND", "NUMBER",
  "UNDECLARED", "END", "ORG", "DEFB", "DEFS", "DEFW", "EQU", "DEFL",
  "LABEL", "EQUATED", "WASEQUATED", "DEFLED", "MULTDEF", "MOD", "SHL",
  "SHR", "NOT", "LT", "GT", "EQ", "LE", "GE", "NE", "IF", "ELSE", "ENDIF",
  "ARGPSEUDO", "LIST", "MINMAX", "MACRO", "MNAME", "OLDMNAME", "ARG",
  "ENDM", "MPARM", "ONECHAR", "TWOCHAR", "'|'", "'^'", "'&'", "'='", "'<'",
  "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "UNARY", "'\\n'",
  "','", "':'", "'.'", "'('", "')'", "'''", "'$'", "'['", "']'", "$accept",
  "statements", "statement", "colonordot", "maybecolon", "label.part",
  "operation", "parm.list", "parm.element", "arg.list", "arg.element",
  "reg", "common8bitreg", "hiddenreg", "realreg", "mem", "index",
  "evenreg", "pushable", "bcdesp", "bcdehlsp", "mar", "condition",
  "spcondition", "db.list", "db.list.element", "dw.list",
  "dw.list.element", "lxexpression", "expression", "parenexpr",
  "noparenexpr", "symbol", "al", "arg_on", "arg_off", "setqf", "clrqf", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     124,    94,    38,    61,    60,    62,    43,    45,    42,    47,
      37,    33,   126,   335,    10,    44,    58,    46,    40,    41,
      39,    36,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   104,   105,   105,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   107,   107,   108,   108,   109,   109,   109,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   111,   111,   111,   112,
     113,   113,   113,   114,   115,   115,   115,   116,   116,   117,
     118,   118,   118,   119,   119,   120,   120,   121,   121,   122,
     122,   122,   123,   123,   124,   124,   125,   125,   126,   126,
     127,   127,   128,   128,   129,   129,   129,   130,   130,   131,
     132,   132,   133,   133,   134,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   136,   136,   136,   136,   136,   136,   137,
     138,   139,   140,   141
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     2,     2,     3,     4,     4,     6,     3,
       2,     2,     3,     4,     4,     5,     4,     2,     3,     4,
       2,     5,     1,     1,     1,     0,     1,     0,     2,     2,
       1,     2,     2,     2,     2,     4,     2,     4,     2,     2,
       2,     2,     4,     4,     4,     4,     2,     4,     2,     4,
       2,     2,     2,     2,     4,     4,     4,     4,     2,     4,
       2,     4,     4,     4,     2,     2,     4,     4,     6,     4,
       4,     2,     4,     2,     4,     2,     1,     2,     4,     4,
       6,     4,     6,     4,     4,     4,     4,     4,     4,     4,
       4,     7,     6,     4,     6,     6,     4,     6,     6,     2,
       2,     1,     2,     2,     2,     1,     0,     1,     3,     1,
       0,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     4,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     0,
       0,     0,     0,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       2,     0,     1,    22,   193,   194,   196,   197,   198,   195,
       0,     0,     0,   200,     0,     0,    23,    24,     3,     0,
       0,    25,   106,   156,   162,   155,   158,   159,   160,   163,
       0,   157,     0,     0,     0,     0,     0,   161,     0,     0,
     152,   153,    10,    11,   201,    17,     0,    20,   193,    28,
      30,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,     0,     0,
       0,     0,    76,     0,     0,     0,     0,     0,     0,     0,
     199,   105,     4,     0,     0,     0,     0,    26,    29,   109,
       0,   107,   188,   191,   192,   190,   189,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     9,   201,     0,    18,   120,   121,   122,     0,
     119,     0,    48,   116,   114,   115,   124,    36,   121,   136,
     137,    46,     0,    34,   121,    50,    38,   121,    51,    39,
     121,    52,    40,   121,    53,    41,     0,     0,   141,   139,
     140,     0,   138,    32,   121,   132,   133,     0,    60,    64,
     127,   128,    75,     0,     0,     0,    99,   100,     0,     0,
       0,    71,     0,    31,     0,    73,     0,     0,     0,     0,
       0,     0,     0,   129,   130,    65,   131,    77,    58,    33,
      12,     0,   102,   145,   144,   103,   142,   146,     0,   104,
     147,   149,   110,     5,     0,     0,     0,    19,     0,   154,
     187,   171,   173,   175,   169,   176,   177,   181,   183,   182,
     184,   185,   186,   172,   174,   170,   179,   178,   180,   164,
     165,   167,   166,   168,     0,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    13,     0,    14,     0,   113,
       0,   111,     6,     7,     0,   108,    15,    49,    37,   135,
     134,    61,   123,   126,     0,    47,    35,    62,    63,    54,
      42,    55,    43,    56,    44,    57,    45,    66,    67,   124,
      72,    90,   202,     0,     0,     0,    93,    70,    69,    74,
      85,     0,    84,     0,    78,     0,   152,   153,   151,    89,
      86,   152,   153,    83,    88,     0,    96,    59,   143,   148,
      21,     0,     0,   125,     0,     0,     0,     0,     0,     0,
       0,     0,   112,     8,    68,   118,   117,   203,    92,    95,
      94,    82,    80,    97,    98,    91
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     1,    18,    19,    88,    20,    83,    90,    91,   280,
     281,   132,   354,   133,   134,   135,   136,   169,   195,   170,
     291,   171,   161,   162,   205,   206,   209,   210,   330,    97,
      40,    41,    21,   212,    44,   124,   345,   365
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -189
static const short int yypact[] =
{
    -189,   299,  -189,  -189,   -64,  -189,  -189,  -189,  -189,  -189,
    2106,   -75,   -44,  -189,  1983,   -20,  -189,  -189,  -189,   112,
     514,   -34,    18,  -189,  -189,  -189,  -189,  -189,  -189,  -189,
    2106,  -189,  2106,  2106,  2106,  2106,  2106,  -189,  2106,   649,
    -189,  -189,  -189,  -189,    -3,  -189,   684,  -189,  -189,  -189,
    -189,  1150,  1086,  1268,  1321,  1374,  1427,  2106,  2106,  1614,
     104,  2106,   -27,  2106,  2106,  -189,   150,  1561,  1643,    48,
     -28,   185,    -5,    85,  2106,  2036,  2106,    70,  2106,  2106,
    -189,  -189,  -189,    25,  2106,  2106,  2106,  -189,  -189,  -189,
     -55,  -189,  -189,  -189,  -189,  -189,  -189,   378,   320,  2106,
    2106,  2106,  2106,  2106,  2106,  2106,  2106,  2106,  2106,  2106,
    2106,  2106,  2106,  2106,  2106,  2106,  2106,  2106,  2106,  2106,
    2106,  2106,  -189,  -189,    38,  -189,  -189,   -11,  -189,    22,
    -189,  1904,  -189,  -189,  -189,  -189,  -189,   963,    45,  -189,
    -189,  -189,    58,   963,    65,  -189,   963,    83,  -189,   963,
      96,  -189,   963,   100,  -189,   963,   487,   555,  -189,  -189,
    -189,   101,  -189,   963,  -189,  -189,  -189,    -4,  -189,  -189,
    -189,  -189,   963,   103,   108,   145,   963,   963,   114,   121,
    1965,  -189,   127,   963,   128,   963,   129,  1785,   130,   131,
     133,  1695,   134,  -189,  -189,  -189,  -189,  -189,   135,   963,
    -189,   742,   963,  -189,  -189,   136,  -189,   963,   777,   138,
    -189,   963,   124,  -189,   835,   870,   591,  -189,    18,  -189,
    -189,  1008,   973,  1024,  -189,    98,    98,    87,    87,   361,
      87,    87,   361,   973,  1024,  1008,   361,    87,    87,    62,
      62,  -189,  -189,  -189,   140,  -189,  1480,    23,   139,  2054,
    1480,    94,  1480,  1480,  1480,  1480,    85,    85,  2106,   202,
     201,   142,   148,   149,   151,  2106,  2106,   219,   152,  1215,
    1875,   160,   153,   223,    85,  -189,    70,  -189,  2106,  -189,
      -7,  -189,  -189,  -189,  2106,  -189,  -189,  -189,   963,  -189,
    -189,  -189,  -189,  -189,   418,  -189,   963,  -189,  -189,  -189,
     963,  -189,   963,  -189,   963,  -189,   963,  -189,  -189,   159,
     963,  -189,  -189,   162,   227,  1724,  -189,  -189,   963,   963,
    -189,   166,  -189,  1814,  -189,   963,   168,   169,  -189,  -189,
    -189,   170,   171,  -189,  -189,   172,  -189,  -189,  -189,  -189,
    -189,   124,   928,  -189,     3,   173,   125,   176,   177,   239,
     178,  1532,  -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,
    -189,  -189,  -189,  -189,   963,  -189
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -189,  -189,  -189,  -189,  -189,  -189,  -189,  -189,    52,  -189,
     -70,   -42,  -189,   -72,   -65,  -189,    21,   -66,  -189,  -188,
    -189,   -50,    75,   212,  -189,     5,  -189,     4,  -189,   -10,
     -63,  -100,   264,  -189,  -189,   161,  -189,  -189
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -151
static const short int yytable[] =
{
      39,   179,   142,   189,    46,   173,   190,   192,    22,   174,
     141,   145,   148,   151,   154,    84,    85,   181,   168,    42,
      92,   196,    93,    94,    95,    96,   158,   188,    98,   248,
     249,   198,   126,   164,   128,   159,   160,    86,   130,   217,
     218,   137,   143,   146,   149,   152,   155,   156,   157,   163,
      43,   172,    47,   176,   177,   165,   289,   183,   185,   290,
     166,    89,    87,   297,   199,   201,   202,   207,   208,   211,
     191,   175,   123,   203,   214,   215,   216,   126,   164,   128,
     165,   139,   140,   130,   246,   166,   186,   340,   341,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,    23,    24,   126,   164,   128,   247,   102,   213,
     130,    25,    26,    27,    28,    29,   165,   139,   140,    30,
     264,   166,   245,   126,   164,   128,   165,   139,   140,   130,
     250,   166,   182,   102,   103,   104,   187,   197,    31,   204,
     119,   120,   121,   251,   102,    48,    32,    33,   139,   140,
     252,    34,    35,     5,     6,     7,     8,     9,    36,   327,
     332,    37,    38,   117,   118,   119,   120,   121,   253,   126,
     164,   128,   261,   167,   117,   118,   119,   120,   121,   178,
     333,   254,   165,   139,   140,   255,   258,   166,   259,   279,
     316,   298,   167,   260,   287,   334,   326,   331,   295,   262,
     299,   301,   303,   305,   307,   308,   263,   193,   139,   140,
     329,   194,   265,   266,   267,   269,   270,   324,   271,   273,
     274,   276,   337,   278,   286,   311,   288,   312,   292,   294,
     296,   313,   300,   302,   304,   306,   314,   315,   310,   320,
     317,   321,   335,   336,   344,   318,   319,   346,   347,   325,
     325,   349,   -81,   -79,   -87,  -150,   207,   351,   211,   361,
     285,   352,   355,   357,   342,   359,   360,   362,   309,   356,
     184,   338,   339,    49,   244,     0,   363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   358,     0,     0,     2,
       3,     0,     0,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
     -27,   -27,   -27,   -27,   -27,   -27,   -27,   -27,    99,   100,
     101,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   364,     4,   -27,   -27,   -27,   -27,   -27,     0,     0,
       5,     6,     7,     8,     9,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    10,    11,    12,    13,    14,
       0,     0,   -27,    15,     0,   -27,   102,   103,   104,     0,
     105,   106,   107,   108,   109,   110,    99,   100,   101,     0,
       0,     0,     0,   -27,     0,    16,    17,     0,     0,     0,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,     0,     0,     0,     0,     0,     0,   102,   103,   104,
       0,   105,   106,   220,   108,   109,    99,   100,   101,     0,
       0,     0,     0,     0,   102,   103,   104,     0,   105,   106,
     107,   108,   109,   110,     0,   115,   116,   117,   118,   119,
     120,   121,     0,     0,     0,     0,     0,     0,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,     0,
       0,     0,     0,     0,   102,   103,   104,   219,   105,   106,
     107,   108,   109,   110,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,   100,   101,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,     0,
       0,     0,     0,     0,     0,     0,     0,   343,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,   102,   103,   104,     0,   105,   106,   107,
     108,   109,   110,     0,     0,     0,     0,     0,    75,    76,
      77,    78,    79,    99,   100,   101,     0,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,     0,     0,
       0,     0,   256,     0,     0,     0,     0,    80,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,     0,    99,
     100,   101,     0,     0,     0,     0,     0,     0,    82,     0,
       0,   102,   103,   104,     0,   105,   106,   107,   108,   109,
     110,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,     0,   102,   103,   104,
     257,   105,   106,   107,   108,   109,   110,    99,   100,   101,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,     0,     0,     0,     0,   284,     0,     0,     0,
       0,     0,    99,   100,   101,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   102,   103,   104,     0,   105,
     106,   107,   108,   109,   110,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     102,   103,   104,   122,   105,   106,   107,   108,   109,   110,
      99,   100,   101,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,     0,     0,     0,   125,     0,
       0,     0,     0,     0,     0,    99,   100,   101,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   102,   103,
     104,     0,   105,   106,   107,   108,   109,   110,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   102,   103,   104,   275,   105,   106,   107,
     108,   109,   110,    99,   100,   101,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,     0,     0,
       0,   277,     0,     0,     0,     0,     0,     0,    99,   100,
     101,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   102,   103,   104,     0,   105,   106,   107,   108,   109,
     110,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   102,   103,   104,   282,
     105,   106,   107,   108,   109,   110,    99,   100,   101,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,     0,     0,     0,   283,     0,     0,     0,     0,     0,
       0,    99,   100,   101,     0,     0,     0,     0,     0,     0,
       0,    99,     0,   101,   102,   103,   104,     0,   105,   106,
     107,   108,   109,   110,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   102,
     103,   104,   353,   105,   106,   107,   108,   109,   110,   102,
     103,   104,    99,   105,   106,   107,   108,   109,   110,     0,
       0,     0,     0,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   102,   103,   104,     0,   105,   106,
     107,   108,   109,   110,     0,     0,     0,     0,     0,     0,
     102,   103,   104,     0,   105,   106,   107,   108,   109,   110,
       0,   114,   115,   116,   117,   118,   119,   120,   121,     0,
       0,     0,     0,     0,     0,     0,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   126,   138,   128,     0,   139,
     140,   130,     0,     0,     0,     0,     0,     0,    23,    24,
       0,     0,     0,     0,     0,     0,     0,    25,    26,    27,
      28,    29,     0,     0,     0,    30,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    31,     0,     0,     0,     0,     0,
       0,     0,    32,    33,     0,     0,     0,    34,    35,   126,
     127,   128,     0,   129,   131,   130,     0,    37,    38,     0,
       0,     0,    23,    24,     0,     0,     0,     0,     0,     0,
       0,    25,    26,    27,    28,    29,     0,     0,     0,    30,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    31,     0,
       0,     0,     0,     0,     0,     0,    32,    33,     0,     0,
       0,    34,    35,     0,   126,   164,   128,     0,   131,     0,
     130,    37,    38,   322,     0,     0,     0,    23,    24,     0,
       0,     0,     0,     0,     0,     0,    25,    26,    27,    28,
      29,     0,     0,     0,    30,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    31,     0,     0,     0,   126,   144,   128,
       0,    32,    33,   130,     0,     0,    34,    35,     0,     0,
      23,    24,     0,   323,     0,     0,    37,    38,     0,    25,
      26,    27,    28,    29,     0,     0,     0,    30,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    31,     0,     0,     0,
     126,   147,   128,     0,    32,    33,   130,     0,     0,    34,
      35,     0,     0,    23,    24,     0,   131,     0,     0,    37,
      38,     0,    25,    26,    27,    28,    29,     0,     0,     0,
      30,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    31,
       0,     0,     0,   126,   150,   128,     0,    32,    33,   130,
       0,     0,    34,    35,     0,     0,    23,    24,     0,   131,
       0,     0,    37,    38,     0,    25,    26,    27,    28,    29,
       0,     0,     0,    30,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,     0,     0,     0,   126,   153,   128,     0,
      32,    33,   130,     0,     0,    34,    35,     0,     0,    23,
      24,     0,   131,     0,     0,    37,    38,     0,    25,    26,
      27,    28,    29,     0,     0,     0,    30,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,     0,     0,     0,   126,
     164,   128,     0,    32,    33,   130,     0,     0,    34,    35,
       0,     0,    23,    24,     0,   131,     0,     0,    37,    38,
       0,    25,    26,    27,    28,    29,     0,     0,     0,    30,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    31,     0,
       0,   126,   164,   128,     0,     0,    32,    33,     0,     0,
       0,    34,    35,     0,    23,    24,     0,     0,   131,     0,
       0,    37,    38,    25,    26,    27,    28,    29,     0,     0,
       0,    30,   158,     0,   139,   140,     0,     0,     0,     0,
       0,   159,   160,    23,    24,     0,     0,     0,     0,     0,
      31,     0,    25,    26,    27,    28,    29,     0,    32,    33,
      30,     0,     0,    34,    35,     0,     0,     0,     0,     0,
      36,     0,     0,    37,    38,     0,     0,     0,     0,    31,
       0,     0,     0,     0,     0,   158,     0,    32,    33,     0,
       0,     0,    34,    35,   159,   160,    23,    24,     0,   180,
       0,     0,    37,    38,     0,    25,    26,    27,    28,    29,
       0,     0,     0,    30,   158,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   160,    23,    24,     0,     0,     0,
       0,     0,    31,     0,    25,    26,    27,    28,    29,     0,
      32,    33,    30,     0,     0,    34,    35,     0,     0,     0,
       0,     0,    36,     0,     0,    37,    38,     0,     0,     0,
       0,    31,     0,     0,     0,     0,   272,     0,     0,    32,
      33,     0,     0,     0,    34,    35,     0,    23,    24,     0,
       0,    36,     0,     0,    37,    38,    25,    26,    27,    28,
      29,     0,     0,     0,    30,   348,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,    24,     0,     0,
       0,     0,     0,    31,     0,    25,    26,    27,    28,    29,
       0,    32,    33,    30,     0,     0,    34,    35,     0,     0,
       0,     0,     0,    36,     0,     0,    37,    38,     0,     0,
       0,     0,    31,     0,     0,     0,     0,     0,     0,     0,
      32,    33,     0,     0,     0,    34,    35,   268,   248,   249,
       0,     0,    36,     0,     0,    37,    38,    23,    24,     0,
       0,     0,     0,     0,     0,     0,    25,    26,    27,    28,
      29,     0,     0,     0,    30,     0,   350,   248,   249,     0,
       0,     0,     0,     0,     0,     0,    23,    24,     0,     0,
       0,     0,     0,    31,     0,    25,    26,    27,    28,    29,
       0,    32,    33,    30,     0,     0,    34,    35,     0,     0,
       0,     0,     0,    36,     0,     0,    37,    38,     0,     0,
       0,     0,    31,     0,     0,     0,     0,     0,     0,     0,
      32,    33,     0,     0,     0,    34,    35,     0,   139,   140,
       0,     0,    36,     0,     0,    37,    38,    23,    24,     0,
       0,     0,     0,     0,     0,     0,    25,    26,    27,    28,
      29,     0,     0,     0,    30,     0,     0,   248,   249,     0,
       0,     0,     0,     0,     0,     0,    23,    24,     0,     0,
       0,     0,     0,    31,   328,    25,    26,    27,    28,    29,
       0,    32,    33,    30,     0,     0,    34,    35,     0,     0,
       0,     0,     0,    36,     0,     0,    37,    38,     0,     0,
       0,     0,    31,     0,     0,     0,     0,     0,     0,     0,
      32,    33,     0,     0,     0,    34,    35,     0,   139,   140,
       0,     0,    36,     0,     0,    37,    38,    23,    24,     0,
       0,     0,     0,     0,     0,     0,    25,    26,    27,    28,
      29,     0,     0,     0,    30,    23,    24,     0,     0,     0,
       0,     0,     0,     0,    25,    26,    27,    28,    29,     0,
       0,     0,    30,    31,     0,     0,     0,     0,     0,     0,
       0,    32,    33,     0,     0,     0,    34,    35,     0,     0,
       0,    31,     0,    36,     0,     0,    37,    38,     0,    32,
      33,     0,     0,     0,    34,    35,     0,    45,    23,    24,
       0,    36,     0,     0,    37,    38,     0,    25,    26,    27,
      28,    29,     0,     0,     0,    30,    23,    24,     0,     0,
       0,     0,     0,     0,     0,    25,    26,    27,    28,    29,
       0,     0,     0,    30,    31,     0,     0,     0,     0,     0,
       0,     0,    32,    33,     0,     0,     0,    34,    35,     0,
     200,     0,    31,     0,    36,     0,     0,    37,    38,     0,
      32,    33,     0,     0,     0,    34,    35,     0,    23,    24,
       0,     0,    36,   293,     0,    37,    38,    25,    26,    27,
      28,    29,     0,     0,     0,    30,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    31,     0,     0,     0,     0,     0,
       0,     0,    32,    33,     0,     0,     0,    34,    35,     0,
       0,     0,     0,     0,    36,     0,     0,    37,    38
};

static const short int yycheck[] =
{
      10,    66,    52,    69,    14,    32,    69,    70,    72,    36,
      52,    53,    54,    55,    56,    49,    50,    67,    60,    94,
      30,    71,    32,    33,    34,    35,    31,    69,    38,    33,
      34,    73,    29,    30,    31,    40,    41,    71,    35,    94,
      95,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      94,    61,    72,    63,    64,    32,    33,    67,    68,   247,
      37,    43,    96,   251,    74,    75,    76,    77,    78,    79,
      98,    98,    75,     3,    84,    85,    86,    29,    30,    31,
      32,    33,    34,    35,    95,    37,    38,    94,    95,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    42,    43,    29,    30,    31,    95,    56,    94,
      35,    51,    52,    53,    54,    55,    32,    33,    34,    59,
     180,    37,    94,    29,    30,    31,    32,    33,    34,    35,
      95,    37,    67,    56,    57,    58,    98,    72,    78,    79,
      88,    89,    90,    95,    56,    43,    86,    87,    33,    34,
      95,    91,    92,    51,    52,    53,    54,    55,    98,   269,
     270,   101,   102,    86,    87,    88,    89,    90,    95,    29,
      30,    31,    37,    98,    86,    87,    88,    89,    90,    39,
      30,    95,    32,    33,    34,    95,    95,    37,    95,    75,
     263,   251,    98,    95,   246,   271,   269,   270,   250,    95,
     252,   253,   254,   255,   256,   257,    95,    32,    33,    34,
     270,    36,    95,    95,    95,    95,    95,   269,    95,    95,
      95,    95,   274,    95,    94,    33,   246,    36,    99,   249,
     250,    99,   252,   253,   254,   255,    98,    98,   258,    30,
      99,    99,    99,    30,    95,   265,   266,    95,    31,   269,
     270,    95,    94,    94,    94,    94,   276,    95,   278,    30,
     218,   341,   344,   100,   284,    99,    99,    99,   257,   344,
      68,   276,   278,    19,   123,    -1,   351,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   346,    -1,    -1,     0,
       1,    -1,    -1,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,     8,     9,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,    43,    44,    45,    46,    47,    48,    -1,    -1,
      51,    52,    53,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      -1,    -1,    73,    74,    -1,    76,    56,    57,    58,    -1,
      60,    61,    62,    63,    64,    65,     8,     9,    10,    -1,
      -1,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    58,
      -1,    60,    61,   103,    63,    64,     8,     9,    10,    -1,
      -1,    -1,    -1,    -1,    56,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,    -1,    84,    85,    86,    87,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    56,    57,    58,    99,    60,    61,
      62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,     9,    10,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    56,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    44,    45,
      46,    47,    48,     8,     9,    10,    -1,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    73,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,
       9,    10,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      -1,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    56,    57,    58,
      95,    60,    61,    62,    63,    64,    65,     8,     9,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      56,    57,    58,    94,    60,    61,    62,    63,    64,    65,
       8,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,     8,     9,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,
      58,    -1,    60,    61,    62,    63,    64,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    56,    57,    58,    94,    60,    61,    62,
      63,    64,    65,     8,     9,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,     8,     9,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    56,    57,    58,    94,
      60,    61,    62,    63,    64,    65,     8,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     8,    -1,    10,    56,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    56,
      57,    58,    94,    60,    61,    62,    63,    64,    65,    56,
      57,    58,     8,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    56,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      56,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    29,    30,    31,    -1,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,
      54,    55,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    91,    92,    29,
      30,    31,    -1,    33,    98,    35,    -1,   101,   102,    -1,
      -1,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    52,    53,    54,    55,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    91,    92,    -1,    29,    30,    31,    -1,    98,    -1,
      35,   101,   102,    38,    -1,    -1,    -1,    42,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,    54,
      55,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    29,    30,    31,
      -1,    86,    87,    35,    -1,    -1,    91,    92,    -1,    -1,
      42,    43,    -1,    98,    -1,    -1,   101,   102,    -1,    51,
      52,    53,    54,    55,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      29,    30,    31,    -1,    86,    87,    35,    -1,    -1,    91,
      92,    -1,    -1,    42,    43,    -1,    98,    -1,    -1,   101,
     102,    -1,    51,    52,    53,    54,    55,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    29,    30,    31,    -1,    86,    87,    35,
      -1,    -1,    91,    92,    -1,    -1,    42,    43,    -1,    98,
      -1,    -1,   101,   102,    -1,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    29,    30,    31,    -1,
      86,    87,    35,    -1,    -1,    91,    92,    -1,    -1,    42,
      43,    -1,    98,    -1,    -1,   101,   102,    -1,    51,    52,
      53,    54,    55,    -1,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    29,
      30,    31,    -1,    86,    87,    35,    -1,    -1,    91,    92,
      -1,    -1,    42,    43,    -1,    98,    -1,    -1,   101,   102,
      -1,    51,    52,    53,    54,    55,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    29,    30,    31,    -1,    -1,    86,    87,    -1,    -1,
      -1,    91,    92,    -1,    42,    43,    -1,    -1,    98,    -1,
      -1,   101,   102,    51,    52,    53,    54,    55,    -1,    -1,
      -1,    59,    31,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    51,    52,    53,    54,    55,    -1,    86,    87,
      59,    -1,    -1,    91,    92,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    86,    87,    -1,
      -1,    -1,    91,    92,    40,    41,    42,    43,    -1,    98,
      -1,    -1,   101,   102,    -1,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    59,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    42,    43,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    51,    52,    53,    54,    55,    -1,
      86,    87,    59,    -1,    -1,    91,    92,    -1,    -1,    -1,
      -1,    -1,    98,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    31,    -1,    -1,    86,
      87,    -1,    -1,    -1,    91,    92,    -1,    42,    43,    -1,
      -1,    98,    -1,    -1,   101,   102,    51,    52,    53,    54,
      55,    -1,    -1,    -1,    59,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    51,    52,    53,    54,    55,
      -1,    86,    87,    59,    -1,    -1,    91,    92,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    91,    92,    32,    33,    34,
      -1,    -1,    98,    -1,    -1,   101,   102,    42,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,    54,
      55,    -1,    -1,    -1,    59,    -1,    32,    33,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    51,    52,    53,    54,    55,
      -1,    86,    87,    59,    -1,    -1,    91,    92,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    91,    92,    -1,    33,    34,
      -1,    -1,    98,    -1,    -1,   101,   102,    42,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,    54,
      55,    -1,    -1,    -1,    59,    -1,    -1,    33,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    -1,    -1,
      -1,    -1,    -1,    78,    79,    51,    52,    53,    54,    55,
      -1,    86,    87,    59,    -1,    -1,    91,    92,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    91,    92,    -1,    33,    34,
      -1,    -1,    98,    -1,    -1,   101,   102,    42,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,    54,
      55,    -1,    -1,    -1,    59,    42,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    52,    53,    54,    55,    -1,
      -1,    -1,    59,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    91,    92,    -1,    -1,
      -1,    78,    -1,    98,    -1,    -1,   101,   102,    -1,    86,
      87,    -1,    -1,    -1,    91,    92,    -1,    94,    42,    43,
      -1,    98,    -1,    -1,   101,   102,    -1,    51,    52,    53,
      54,    55,    -1,    -1,    -1,    59,    42,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    59,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    91,    92,    -1,
      94,    -1,    78,    -1,    98,    -1,    -1,   101,   102,    -1,
      86,    87,    -1,    -1,    -1,    91,    92,    -1,    42,    43,
      -1,    -1,    98,    99,    -1,   101,   102,    51,    52,    53,
      54,    55,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    91,    92,    -1,
      -1,    -1,    -1,    -1,    98,    -1,    -1,   101,   102
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,   105,     0,     1,    43,    51,    52,    53,    54,    55,
      66,    67,    68,    69,    70,    74,    96,    97,   106,   107,
     109,   136,    72,    42,    43,    51,    52,    53,    54,    55,
      59,    78,    86,    87,    91,    92,    98,   101,   102,   133,
     134,   135,    94,    94,   138,    94,   133,    72,    43,   136,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    44,    45,    46,    47,    48,
      73,    76,    94,   110,    49,    50,    71,    96,   108,    43,
     111,   112,   133,   133,   133,   133,   133,   133,   133,     8,
       9,    10,    56,    57,    58,    60,    61,    62,    63,    64,
      65,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    94,    75,   139,    94,    29,    30,    31,    33,
      35,    98,   115,   117,   118,   119,   120,   133,    30,    33,
      34,   115,   125,   133,    30,   115,   133,    30,   115,   133,
      30,   115,   133,    30,   115,   133,   133,   133,    31,    40,
      41,   126,   127,   133,    30,    32,    37,    98,   115,   121,
     123,   125,   133,    32,    36,    98,   133,   133,    39,   118,
      98,   125,   126,   133,   127,   133,    38,    98,   115,   121,
     134,    98,   134,    32,    36,   122,   125,   126,   115,   133,
      94,   133,   133,     3,    79,   128,   129,   133,   133,   130,
     131,   133,   137,    94,   133,   133,   133,    94,    95,    99,
     103,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   139,    94,    95,    95,    33,    34,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    37,    95,    95,   125,    95,    95,    95,    32,    95,
      95,    95,    31,    95,    95,    94,    95,    94,    95,    75,
     113,   114,    94,    94,    95,   112,    94,   115,   133,    33,
     123,   124,    99,    99,   133,   115,   133,   123,   125,   115,
     133,   115,   133,   115,   133,   115,   133,   115,   115,   120,
     133,    33,    36,    99,    98,    98,   134,    99,   133,   133,
      30,    99,    38,    98,   115,   133,   134,   135,    79,   125,
     132,   134,   135,    30,   121,    99,    30,   115,   129,   131,
      94,    95,   133,    99,    95,   140,    95,    31,    31,    95,
      32,    95,   114,    94,   116,   117,   118,   100,   125,    99,
      99,    30,    99,   118,   133,   141
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  register short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;


  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 947 "zmac.y"
    {
		if (yyvsp[-1].itemptr) list(dollarsign);
		else  list1();
	}
    break;

  case 5:
#line 952 "zmac.y"
    {
		list(dollarsign);
	}
    break;

  case 6:
#line 956 "zmac.y"
    {
		/* a forward reference to a label in the expression cannot
		 * be fixed in a forward reference to the EQU;
		 * it would need three passes. -rjm
		 */
		if(!pass2 && equ_bad_label) {
			/* this indicates that the equ has an incorrect
			 * (i.e. pass 1) value.
			 */
			yyvsp[-3].itemptr->i_equbad = 1;
		} else {
			/* but if 2nd pass or no forward reference, it's ok. */
			yyvsp[-3].itemptr->i_equbad = 0;
		}
		equ_bad_label=0;
		switch(yyvsp[-3].itemptr->i_token) {
		case UNDECLARED: case WASEQUATED:
			yyvsp[-3].itemptr->i_token = EQUATED;
			yyvsp[-3].itemptr->i_value = yyvsp[-1].ival;
			break;
		case EQUATED:
			if (yyvsp[-3].itemptr->i_value == yyvsp[-1].ival)
				break; /* Allow benign redefinition -mgr */
			/* Drop-through intentional */
		default:
			err[mflag]++;
			yyvsp[-3].itemptr->i_token = MULTDEF;
		}
		list(yyvsp[-1].ival);
	}
    break;

  case 7:
#line 987 "zmac.y"
    {
		switch(yyvsp[-3].itemptr->i_token) {
		case UNDECLARED: case DEFLED:
			yyvsp[-3].itemptr->i_token = DEFLED;
			yyvsp[-3].itemptr->i_value = yyvsp[-1].ival;
			break;
		default:
			err[mflag]++;
			yyvsp[-3].itemptr->i_token = MULTDEF;
		}
		list(yyvsp[-1].ival);
	}
    break;

  case 8:
#line 1000 "zmac.y"
    {
		switch (yyvsp[-5].itemptr->i_token) {
		case UNDECLARED: case DEFLED:
			yyvsp[-5].itemptr->i_token = DEFLED;
			if (yyvsp[-4].itemptr->i_value)	/* max */
				list(yyvsp[-5].itemptr->i_value = (yyvsp[-3].ival > yyvsp[-1].ival? yyvsp[-3].ival:yyvsp[-1].ival));
			else list(yyvsp[-5].itemptr->i_value = (yyvsp[-3].ival < yyvsp[-1].ival? yyvsp[-3].ival:yyvsp[-1].ival));
			break;
		default:
			err[mflag]++;
			yyvsp[-5].itemptr->i_token = MULTDEF;
			list(yyvsp[-5].itemptr->i_value);
		}
	}
    break;

  case 9:
#line 1015 "zmac.y"
    {
		/* all $2's here were yypc[2].ival before.
		 * I think the idea was perhaps to allow constants
		 * only...? Anyway, it now allows any expression -
		 * which would seem to make sense given the definition
		 * above, right? :-)  -rjm
		 */
		if (ifptr >= ifstmax)
			error("Too many ifs");
		else {
			if (pass2) {
				*++ifptr = *expifp++;
				if (*ifptr != !(yyvsp[-1].ival)) err[pflag]++;
			} else {
				if (expifp >= expifmax)
					error("Too many ifs!");
				*expifp++ = !(yyvsp[-1].ival);
				*++ifptr = !(yyvsp[-1].ival);
			}
		}
		saveopt = fopt;
		fopt = 1;
		list(yyvsp[-1].ival);
		fopt = saveopt;
	}
    break;

  case 10:
#line 1041 "zmac.y"
    {
		/* FIXME: it would be nice to spot repeated ELSEs, but how? */
		*ifptr = !*ifptr;
		saveopt = fopt;
		fopt = 1;
		list1();
		fopt = saveopt;
	}
    break;

  case 11:
#line 1050 "zmac.y"
    {
		if (ifptr == ifstack) err[bflag]++;
		else --ifptr;
		list1();
	}
    break;

  case 12:
#line 1056 "zmac.y"
    {
		list(dollarsign);
		peekc = 0;
	}
    break;

  case 13:
#line 1061 "zmac.y"
    {
		xeq_flag++;
		xeq = yyvsp[-1].ival;
		list(yyvsp[-1].ival);
		peekc = 0;
	}
    break;

  case 14:
#line 1068 "zmac.y"
    {
		if (yyvsp[-1].ival < 0) err[vflag]++;
		list(dollarsign);
		if (yyvsp[-1].ival) {
			flushbin();
			dollarsign += yyvsp[-1].ival;
			olddollar = dollarsign;

			/* if it's not hex output though, we also need
			 * to output zeroes as appropriate. -rjm
			 */
			if(!output_hex && pass2) {
				int f;
				for (f=0;f<(yyvsp[-1].ival);f++)
					fputc(0, fbuf);
			}
		}
	}
    break;

  case 15:
#line 1087 "zmac.y"
    {
		list1();
		switch (yyvsp[-4].itemptr->i_value) {

		case 0:		/* title */
			lineptr = linebuf;
			cp = tempbuf;
			title = titlespace;
			while ((*title++ = *cp++) && (title < &titlespace[TITLELEN]));
			*title = 0;
			title = titlespace;
			break;

		case 1:		/* rsym */
			if (pass2) break;
			insymtab(tempbuf);
			break;

		case 2:		/* wsym */
			strcpy(writesyms, tempbuf);
			break;

		case 3:		/* include file */
			if (*tempbuf == '"' || *tempbuf == '\'')
			{
				if (tempbuf[strlen (tempbuf) - 1] == '"' || tempbuf[strlen (tempbuf) - 1] == '\'')
					tempbuf[strlen (tempbuf) - 1] = 0;
				next_source(tempbuf + 1) ;
			}
			else
			{
				next_source(tempbuf) ;
			}
			break ;
		}
	}
    break;

  case 16:
#line 1124 "zmac.y"
    {
		fprintf(stderr,"ARGPSEUDO error\n");
		err[fflag]++;
		list(dollarsign);
	}
    break;

  case 17:
#line 1130 "zmac.y"
    {
		list_tmp1=yyvsp[-1].itemptr->i_value;
		list_tmp2=1;
		goto dolopt;
	}
    break;

  case 18:
#line 1136 "zmac.y"
    {
		list_tmp1=yyvsp[-2].itemptr->i_value;
		list_tmp2=yyvsp[-1].ival;
	dolopt:
		linecnt++;
		if (pass2) {
			lineptr = linebuf;
			switch (list_tmp1) {
			case 0:	/* list */
				if (list_tmp2 < 0) lstoff = 1;
				if (list_tmp2 > 0) lstoff = 0;
				break;

			case 1:	/* eject */
				if (list_tmp2) eject();
				break;

			case 2:	/* space */
				if ((line + list_tmp2) > 60) eject();
				else space(list_tmp2);
				break;

			case 3:	/* elist */
				eopt = edef;
				if (list_tmp2 < 0) eopt = 0;
				if (list_tmp2 > 0) eopt = 1;
				break;

			case 4:	/* fopt */
				fopt = fdef;
				if (list_tmp2 < 0) fopt = 0;
				if (list_tmp2 > 0) fopt = 1;
				break;

			case 5:	/* gopt */
				gopt = gdef;
				if (list_tmp2 < 0) gopt = 1;
				if (list_tmp2 > 0) gopt = 0;
				break;

			case 6: /* mopt */
				mopt = mdef;
				if (list_tmp2 < 0) mopt = 0;
				if (list_tmp2 > 0) mopt = 1;
			}
		}
	}
    break;

  case 19:
#line 1184 "zmac.y"
    {
		yyvsp[-3].itemptr->i_token = MNAME;
		yyvsp[-3].itemptr->i_value = mfptr;
#ifdef M_DEBUG
		fprintf (stderr, "[UNDECLARED MACRO %s]\n", yyvsp[-3].itemptr->i_string);
#endif
		mfseek(mfile, (long)mfptr, 0);
		list1();
		mlex() ;
		parm_number = 0;
	}
    break;

  case 20:
#line 1196 "zmac.y"
    {
		yyvsp[-1].itemptr->i_token = MNAME;
#ifdef M_DEBUG
		fprintf (stderr, "[OLDNAME MACRO %s]\n", yyvsp[-1].itemptr->i_string);
#endif
		while (yychar != ENDM && yychar) {
			while (yychar != '\n' && yychar)
				yychar = yylex();
			list1();
			yychar = yylex();
		}
		while (yychar != '\n' && yychar) yychar = yylex();
		list1();
		yychar = yylex();
	}
    break;

  case 21:
#line 1212 "zmac.y"
    {
#ifdef M_DEBUG
		fprintf (stderr, "[MNAME %s]\n", yyvsp[-3].itemptr->i_string);
#endif
		yyvsp[-3].itemptr->i_uses++ ;
		arg_flag = 0;
		parm_number = 0;
		list(dollarsign);
		expptr++;
		est = est2;
		est2 = NULL;
		est[FLOC] = floc;
		est[TEMPNUM] = (char *)(long)exp_number++;
		floc = (char *)(long)(yyvsp[-3].itemptr->i_value);
		mfseek(mfile, (long)floc, 0);
	}
    break;

  case 22:
#line 1229 "zmac.y"
    {
		fprintf (stderr, "Error met.\n");
		err[fflag]++;
		quoteflag = 0;
		arg_flag = 0;
		parm_number = 0;

		if (est2)
		{
			int i;
			for (i=0; i<PARMMAX; i++) {
				if (est2[i])
#ifdef M_DEBUG
	fprintf (stderr, "[Freeing2 arg%u(%p)]\n", i, est2[i]),
#endif
						free(est2[i]);
			}
			free(est2);
			est2 = NULL;
		}

		while(yychar != '\n' && yychar != '\0') yychar = yylex();
		list(dollarsign);
		yyclearin;yyerrok;
	}
    break;

  case 27:
#line 1270 "zmac.y"
    {	yyval.itemptr = NULL;	}
    break;

  case 28:
#line 1272 "zmac.y"
    {
		switch(yyvsp[0].itemptr->i_token) {
		case UNDECLARED:
			if (pass2)
				err[pflag]++;
			else {
				yyvsp[0].itemptr->i_token = LABEL;
				yyvsp[0].itemptr->i_value = dollarsign;
			}
			break;
		case LABEL:
			if (!pass2) {
				yyvsp[0].itemptr->i_token = MULTDEF;
				err[mflag]++;
			} else if (yyvsp[0].itemptr->i_value != dollarsign)
				err[pflag]++;
			break;
		default:
			err[mflag]++;
			yyvsp[0].itemptr->i_token = MULTDEF;
		}
	}
    break;

  case 29:
#line 1295 "zmac.y"
    {
		switch(yyvsp[-1].itemptr->i_token) {
		case UNDECLARED:
			if (pass2)
				err[pflag]++;
			else {
				yyvsp[-1].itemptr->i_token = LABEL;
				yyvsp[-1].itemptr->i_value = dollarsign;
			}
			break;
		case LABEL:
			if (!pass2) {
				yyvsp[-1].itemptr->i_token = MULTDEF;
				err[mflag]++;
			} else if (yyvsp[-1].itemptr->i_value != dollarsign)
				err[pflag]++;
			break;
		default:
			err[mflag]++;
			yyvsp[-1].itemptr->i_token = MULTDEF;
		}
	}
    break;

  case 30:
#line 1322 "zmac.y"
    { emit1(yyvsp[0].itemptr->i_value, 0, 0, 1); }
    break;

  case 31:
#line 1325 "zmac.y"
    { emitjp(0303, yyvsp[0].ival); }
    break;

  case 32:
#line 1328 "zmac.y"
    { emit(3, 0315, yyvsp[0].ival, yyvsp[0].ival >> 8); }
    break;

  case 33:
#line 1331 "zmac.y"
    { int a = yyvsp[0].ival, doneerr=0;
		/* added support for normal RST form -rjm */
		if (a >= 8) {
			if ((a&7)!=0) doneerr=1,err[vflag]++;
			a >>= 3;
		}
		if ((a > 7 || a < 0) && !doneerr) /* don't give two errs... */
			err[vflag]++;
		emit(1, yyvsp[-1].itemptr->i_value + (a << 3));
	}
    break;

  case 34:
#line 1343 "zmac.y"
    { emit1(0306, 0, yyvsp[0].ival, 3); if (pass2) warnprt (1, 0); }
    break;

  case 35:
#line 1346 "zmac.y"
    { emit1(0306, 0, yyvsp[0].ival, 3); }
    break;

  case 36:
#line 1349 "zmac.y"
    { emit1(0306 + (yyvsp[-1].itemptr->i_value << 3), 0, yyvsp[0].ival, 3); if (pass2) warnprt (1, 0); }
    break;

  case 37:
#line 1352 "zmac.y"
    { emit1(0306 + (yyvsp[-3].itemptr->i_value << 3), 0, yyvsp[0].ival, 3); }
    break;

  case 38:
#line 1355 "zmac.y"
    { emit1(0306 | (yyvsp[-1].itemptr->i_value << 3), 0, yyvsp[0].ival, 3); }
    break;

  case 39:
#line 1358 "zmac.y"
    { emit1(0306 | (yyvsp[-1].itemptr->i_value << 3), 0, yyvsp[0].ival, 3); }
    break;

  case 40:
#line 1361 "zmac.y"
    { emit1(0306 | (yyvsp[-1].itemptr->i_value << 3), 0, yyvsp[0].ival, 3); }
    break;

  case 41:
#line 1364 "zmac.y"
    { emit1(0306 | (yyvsp[-1].itemptr->i_value << 3), 0, yyvsp[0].ival, 3); }
    break;

  case 42:
#line 1367 "zmac.y"
    { emit1(0306 | (yyvsp[-3].itemptr->i_value << 3), 0, yyvsp[0].ival, 3); if (pass2) warnprt (1, 0); }
    break;

  case 43:
#line 1370 "zmac.y"
    { emit1(0306 | (yyvsp[-3].itemptr->i_value << 3), 0, yyvsp[0].ival, 3); if (pass2) warnprt (1, 0); }
    break;

  case 44:
#line 1373 "zmac.y"
    { emit1(0306 | (yyvsp[-3].itemptr->i_value << 3), 0, yyvsp[0].ival, 3); if (pass2) warnprt (1, 0); }
    break;

  case 45:
#line 1376 "zmac.y"
    { emit1(0306 | (yyvsp[-3].itemptr->i_value << 3), 0, yyvsp[0].ival, 3); if (pass2) warnprt (1, 0); }
    break;

  case 46:
#line 1379 "zmac.y"
    { emit1(0200 + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0); if (pass2) warnprt (1, 0); }
    break;

  case 47:
#line 1383 "zmac.y"
    { emit1(0200 + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0); }
    break;

  case 48:
#line 1387 "zmac.y"
    { emit1(0200 + (yyvsp[-1].itemptr->i_value << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0); if (pass2) warnprt (1, 0); }
    break;

  case 49:
#line 1390 "zmac.y"
    { emit1(0200 + (yyvsp[-3].itemptr->i_value << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0); }
    break;

  case 50:
#line 1393 "zmac.y"
    { emit1(0200 + (yyvsp[-1].itemptr->i_value << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0); }
    break;

  case 51:
#line 1396 "zmac.y"
    { emit1(0200 + (yyvsp[-1].itemptr->i_value << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0); }
    break;

  case 52:
#line 1399 "zmac.y"
    { emit1(0200 + (yyvsp[-1].itemptr->i_value << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0); }
    break;

  case 53:
#line 1402 "zmac.y"
    { emit1(0200 + (yyvsp[-1].itemptr->i_value << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0); }
    break;

  case 54:
#line 1405 "zmac.y"
    { emit1(0200 + (yyvsp[-3].itemptr->i_value << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0); if (pass2) warnprt (1, 0); }
    break;

  case 55:
#line 1408 "zmac.y"
    { emit1(0200 + (yyvsp[-3].itemptr->i_value << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0); if (pass2) warnprt (1, 0); }
    break;

  case 56:
#line 1411 "zmac.y"
    { emit1(0200 + (yyvsp[-3].itemptr->i_value << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0); if (pass2) warnprt (1, 0); }
    break;

  case 57:
#line 1414 "zmac.y"
    { emit1(0200 + (yyvsp[-3].itemptr->i_value << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0); if (pass2) warnprt (1, 0); }
    break;

  case 58:
#line 1417 "zmac.y"
    {
			if (suggest_optimise && pass2 && (yyvsp[0].ival & 0377) == 7 && yyvsp[-1].itemptr->i_value <= 4)
				warnprt (yyvsp[-1].itemptr->i_value + 4, 0);
			if (pass2 && yyvsp[-1].itemptr->i_value == 6)
				warnprt (1, 0);
			emit1(0145400 + (yyvsp[-1].itemptr->i_value << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0);
		}
    break;

  case 59:
#line 1426 "zmac.y"
    {
			if((yyvsp[-2].ival < 0377) || (yyvsp[0].ival > 07)) {
				fprintf(stderr,"Only SHIFT (IX+d),r or SHIFT (IY+d),r are allowed\n");
				err[fflag]++;
			}
			emit1(0145400 + (yyvsp[-3].itemptr->i_value << 3) + (yyvsp[0].ival & 0377), yyvsp[-2].ival, 0, 0);
		}
    break;

  case 60:
#line 1435 "zmac.y"
    { emit1(yyvsp[-1].itemptr->i_value + ((yyvsp[0].ival & 0377) << 3) + 4, yyvsp[0].ival, 0, 0); }
    break;

  case 61:
#line 1438 "zmac.y"
    { if (yyvsp[-3].itemptr->i_value == 1)
				emit(2,0355,0112+yyvsp[0].ival);
			else
				emit(2,0355,0102+yyvsp[0].ival);
		}
    break;

  case 62:
#line 1445 "zmac.y"
    { emitdad(yyvsp[-2].ival,yyvsp[0].ival); }
    break;

  case 63:
#line 1448 "zmac.y"
    {
			if (yyvsp[-2].ival != yyvsp[0].ival) {
				fprintf(stderr,"ADD mar, mar error\n");
				err[fflag]++;
			}
			emitdad(yyvsp[-2].ival,yyvsp[0].ival);
		}
    break;

  case 64:
#line 1457 "zmac.y"
    { emit1((yyvsp[-1].itemptr->i_value << 3) + (yyvsp[0].ival & 0377) + 3, yyvsp[0].ival, 0, 1); }
    break;

  case 65:
#line 1460 "zmac.y"
    { emit1(yyvsp[-1].itemptr->i_value + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 1); }
    break;

  case 66:
#line 1463 "zmac.y"
    {
			if (yyvsp[-2].ival < 0 || yyvsp[-2].ival > 7)
				err[vflag]++;
			emit1(yyvsp[-3].itemptr->i_value + ((yyvsp[-2].ival & 7) << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0);
		}
    break;

  case 67:
#line 1470 "zmac.y"
    {
			if (yyvsp[-2].ival < 0 || yyvsp[-2].ival > 7)
				err[vflag]++;
			emit1(yyvsp[-3].itemptr->i_value + ((yyvsp[-2].ival & 7) << 3) + (yyvsp[0].ival & 0377), yyvsp[0].ival, 0, 0);
		}
    break;

  case 68:
#line 1477 "zmac.y"
    {
			if (yyvsp[-4].ival < 0 || yyvsp[-4].ival > 7)
				err[vflag]++;
			emit1(((yyvsp[-5].itemptr->i_value + ((yyvsp[-4].ival & 7) << 3) + (yyvsp[-2].ival & 0377)) & 0x0FFF8) + (yyvsp[0].ival & 7), yyvsp[-2].ival, 0, 0);
		}
    break;

  case 69:
#line 1484 "zmac.y"
    { emitjp(0302 + yyvsp[-2].ival, yyvsp[0].ival); }
    break;

  case 70:
#line 1487 "zmac.y"
    { emit1(0351, yyvsp[-1].ival, 0, 1); }
    break;

  case 71:
#line 1490 "zmac.y"
    { emit1(0351, yyvsp[0].ival, 0, 1); if (pass2) warnprt (1, 0); }
    break;

  case 72:
#line 1493 "zmac.y"
    { emit(3, 0304 + yyvsp[-2].ival, yyvsp[0].ival, yyvsp[0].ival >> 8); }
    break;

  case 73:
#line 1496 "zmac.y"
    { emitjr(030,yyvsp[0].ival); }
    break;

  case 74:
#line 1499 "zmac.y"
    { emitjr(yyvsp[-3].itemptr->i_value + yyvsp[-2].ival, yyvsp[0].ival); }
    break;

  case 75:
#line 1502 "zmac.y"
    { emitjr(yyvsp[-1].itemptr->i_value, yyvsp[0].ival); }
    break;

  case 76:
#line 1505 "zmac.y"
    { emit(1, yyvsp[0].itemptr->i_value); }
    break;

  case 77:
#line 1508 "zmac.y"
    { emit(1, 0300 + yyvsp[0].ival); }
    break;

  case 78:
#line 1511 "zmac.y"
    {
			if (((yyvsp[-2].ival & 0377) == 6 && (yyvsp[0].ival & 0377) == 6)||(((yyvsp[-2].ival>>8)^(yyvsp[0].ival>>8))==0x020)||((yyvsp[-2].ival>=0x0DD00)&&(yyvsp[0].ival>=0x0DD00)&&(((yyvsp[-2].ival&07)|(yyvsp[0].ival&07))>=6))) {
				if (!pass2)
					fprintf(stderr,"LD reg, reg error\n");
				err[fflag]++;
			}
			emit1(0100 + ((yyvsp[-2].ival & 7) << 3) + (yyvsp[0].ival & 7),(yyvsp[-2].ival>yyvsp[0].ival?yyvsp[-2].ival:yyvsp[0].ival), 0, 0);
/*			emit1(0100 + (($2 & 7) << 3) + ($4 & 7),$2 | $4, 0, 0);*/
		}
    break;

  case 79:
#line 1522 "zmac.y"
    {
			if (suggest_optimise && pass2 && yyvsp[0].ival == 0 && (yyvsp[-2].ival & 0377) == 7)
				warnprt (3, 0);
			emit1(6 + ((yyvsp[-2].ival & 0377) << 3), yyvsp[-2].ival, yyvsp[0].ival, 2);
		}
    break;

  case 80:
#line 1529 "zmac.y"
    {	if (yyvsp[-4].ival != 7) {
				fprintf(stderr,"LD reg, (RP) error\n");
				err[fflag]++;
			}
			else emit(1, 012 + yyvsp[-1].itemptr->i_value);
		}
    break;

  case 81:
#line 1537 "zmac.y"
    {
			if (yyvsp[-2].ival != 7) {
				fprintf(stderr,"LD reg, (expr) error\n");
				err[fflag]++;
			}
			else emit(3, 072, yyvsp[0].ival, yyvsp[0].ival >> 8);
		}
    break;

  case 82:
#line 1546 "zmac.y"
    { emit(1, 2 + yyvsp[-3].itemptr->i_value); }
    break;

  case 83:
#line 1549 "zmac.y"
    { emit(3, 062, yyvsp[-2].ival, yyvsp[-2].ival >> 8); }
    break;

  case 84:
#line 1552 "zmac.y"
    {
			if (yyvsp[-2].ival != 7) {
				fprintf(stderr,"LD reg, MISCREG error\n");
				err[fflag]++;
			}
			else emit(2, 0355, 0127 + yyvsp[0].itemptr->i_value);
		}
    break;

  case 85:
#line 1561 "zmac.y"
    { emit(2, 0355, 0107 + yyvsp[-2].itemptr->i_value); }
    break;

  case 86:
#line 1564 "zmac.y"
    { emit1(1 + (yyvsp[-2].ival & 060), yyvsp[-2].ival, yyvsp[0].ival, 5); }
    break;

  case 87:
#line 1567 "zmac.y"
    {
			if ((yyvsp[-2].ival & 060) == 040)
				emit1(052, yyvsp[-2].ival, yyvsp[0].ival, 5);
			else
				emit(4, 0355, 0113 + yyvsp[-2].ival, yyvsp[0].ival, yyvsp[0].ival >> 8);
		}
    break;

  case 88:
#line 1575 "zmac.y"
    {
			if ((yyvsp[0].ival & 060) == 040)
				emit1(042, yyvsp[0].ival, yyvsp[-2].ival, 5);
			else
				emit(4, 0355, 0103 + yyvsp[0].ival, yyvsp[-2].ival, yyvsp[-2].ival >> 8);
		}
    break;

  case 89:
#line 1583 "zmac.y"
    {
			if (yyvsp[-2].ival != 060) {
				fprintf(stderr,"LD evenreg error\n");
				err[fflag]++;
			}
			else
				emit1(0371, yyvsp[0].ival, 0, 1);
		}
    break;

  case 90:
#line 1593 "zmac.y"
    {
			if (yyvsp[-2].itemptr->i_value != 020) {
				fprintf(stderr,"EX RP, HL error\n");
				err[fflag]++;
			}
			else
				emit(1, 0353);
		}
    break;

  case 91:
#line 1603 "zmac.y"
    { emit(1, 010); }
    break;

  case 92:
#line 1606 "zmac.y"
    { emit1(0343, yyvsp[0].ival, 0, 1); }
    break;

  case 93:
#line 1609 "zmac.y"
    {
			if (yyvsp[-2].ival != 7) {
				fprintf(stderr,"IN reg, (expr) error\n");
				err[fflag]++;
			}
			else	{
				if (yyvsp[0].ival < 0 || yyvsp[0].ival > 255)
					err[vflag]++;
				emit(2, yyvsp[-3].itemptr->i_value, yyvsp[0].ival);
			}
		}
    break;

  case 94:
#line 1622 "zmac.y"
    { emit(2, 0355, 0100 + (yyvsp[-4].ival << 3)); }
    break;

  case 95:
#line 1625 "zmac.y"
    { emit(2, 0355, 0160); }
    break;

  case 96:
#line 1628 "zmac.y"
    {
			if (yyvsp[-2].ival < 0 || yyvsp[-2].ival > 255)
				err[vflag]++;
			emit(2, yyvsp[-3].itemptr->i_value, yyvsp[-2].ival);
		}
    break;

  case 97:
#line 1635 "zmac.y"
    { emit(2, 0355, 0101 + (yyvsp[0].ival << 3)); }
    break;

  case 98:
#line 1638 "zmac.y"
    {
			if (yyvsp[0].ival!=0) {
				fprintf(stderr,"Only 'OUT (C),0' is allowed\n");
				err[fflag]++;
			}
			else
				emit(2,0x0ed,0x071);
		}
    break;

  case 99:
#line 1648 "zmac.y"
    {
			if (yyvsp[0].ival > 2 || yyvsp[0].ival < 0)
				err[vflag]++;
			else
				emit(2, yyvsp[-1].itemptr->i_value >> 8, yyvsp[-1].itemptr->i_value + ((yyvsp[0].ival + (yyvsp[0].ival > 0)) << 3));
		}
    break;

  case 100:
#line 1656 "zmac.y"
    {
			if (phaseflag) {
				err[oflag]++;
			} else {
				phaseflag = 1;
				phdollar = dollarsign;
				dollarsign = yyvsp[0].ival;
				phbegin = dollarsign;
			}
		}
    break;

  case 101:
#line 1668 "zmac.y"
    {
			if (!phaseflag) {
				err[oflag]++;
			} else {
				phaseflag = 0;
				dollarsign = phdollar + dollarsign - phbegin;
			}
		}
    break;

  case 102:
#line 1678 "zmac.y"
    {
			if (not_seen_org)
				first_org_store=yyvsp[0].ival;
			not_seen_org=0;
			if (phaseflag) {
				err[oflag]++;
				dollarsign = phdollar + dollarsign - phbegin;
				phaseflag = 0;
			}
			if (yyvsp[0].ival-dollarsign) {
				flushbin();
				if (pass2 && !output_hex && dollarsign != 0) {
					if (yyvsp[0].ival < dollarsign) {
						err[orgflag]++;
					} else {
						int f;
						for (f=0;f<(yyvsp[0].ival - dollarsign);f++)
							fputc(0, fbuf);
					}
				}
				olddollar = yyvsp[0].ival;
				dollarsign = yyvsp[0].ival;
			}
		}
    break;

  case 109:
#line 1721 "zmac.y"
    {
			yyvsp[0].itemptr->i_token = MPARM;
			if (parm_number >= PARMMAX)
				error("Too many parameters");
			yyvsp[0].itemptr->i_value = parm_number++;
		}
    break;

  case 113:
#line 1741 "zmac.y"
    {
			cp = malloc(strlen(tempbuf)+1);
#ifdef M_DEBUG
			fprintf (stderr, "[Arg%u(%p): %s]\n", parm_number, cp, tempbuf);
#endif
			est2[parm_number++] = cp;
			strcpy(cp, tempbuf);
		}
    break;

  case 119:
#line 1764 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 120:
#line 1770 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 121:
#line 1775 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 122:
#line 1780 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 123:
#line 1786 "zmac.y"
    {
			yyval.ival = 6;
		}
    break;

  case 125:
#line 1794 "zmac.y"
    {
			disp = yyvsp[-1].ival;
			yyval.ival = (yyvsp[-2].itemptr->i_value & 0177400) | 6;
		}
    break;

  case 126:
#line 1800 "zmac.y"
    {
			disp = 0;
			yyval.ival = (yyvsp[-1].itemptr->i_value & 0177400) | 6;
		}
    break;

  case 129:
#line 1812 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 130:
#line 1817 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 132:
#line 1825 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 133:
#line 1830 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 135:
#line 1838 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 136:
#line 1844 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 137:
#line 1849 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 139:
#line 1857 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 140:
#line 1863 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value;
		}
    break;

  case 141:
#line 1868 "zmac.y"
    {	yyval.ival = 030;	}
    break;

  case 144:
#line 1877 "zmac.y"
    {
			dataemit(2, yyvsp[0].ival, yyvsp[0].ival>>8);
		}
    break;

  case 145:
#line 1882 "zmac.y"
    {
			cp = yyvsp[0].cval;
			while (*cp != '\0')
				dataemit(1,*cp++);
		}
    break;

  case 146:
#line 1889 "zmac.y"
    {
			if (yyvsp[0].ival < -128 || yyvsp[0].ival > 255)
					err[vflag]++;
			dataemit(1, yyvsp[0].ival & 0377);
		}
    break;

  case 149:
#line 1906 "zmac.y"
    {
			dataemit(2, yyvsp[0].ival, yyvsp[0].ival>>8);
		}
    break;

  case 154:
#line 1927 "zmac.y"
    {	yyval.ival = yyvsp[-1].ival;	}
    break;

  case 155:
#line 1932 "zmac.y"
    {	yyval.ival = yyvsp[0].itemptr->i_value; yyvsp[0].itemptr->i_uses++ ;	}
    break;

  case 158:
#line 1939 "zmac.y"
    {	yyval.ival = yyvsp[0].itemptr->i_value; yyvsp[0].itemptr->i_uses++ ;	}
    break;

  case 159:
#line 1942 "zmac.y"
    {
			yyval.ival = yyvsp[0].itemptr->i_value; yyvsp[0].itemptr->i_uses++ ;
			if (yyvsp[0].itemptr->i_equbad) {
				/* forward reference to equ with a forward
				 * reference of its own cannot be resolved
				 * in two passes. -rjm
				 */
				err[frflag]++;
			}
		}
    break;

  case 160:
#line 1954 "zmac.y"
    {	yyval.ival = yyvsp[0].itemptr->i_value; yyvsp[0].itemptr->i_uses++ ;	}
    break;

  case 161:
#line 1957 "zmac.y"
    {	yyval.ival = dollarsign;	}
    break;

  case 162:
#line 1960 "zmac.y"
    {
			err[uflag]++;
			equ_bad_label=1;
			yyval.ival = 0;
		}
    break;

  case 163:
#line 1967 "zmac.y"
    {	yyval.ival = yyvsp[0].itemptr->i_value;	}
    break;

  case 164:
#line 1970 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival + yyvsp[0].ival;	}
    break;

  case 165:
#line 1973 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival - yyvsp[0].ival;	}
    break;

  case 166:
#line 1976 "zmac.y"
    {	if (yyvsp[0].ival == 0) err[eflag]++; else yyval.ival = yyvsp[-2].ival / yyvsp[0].ival;	}
    break;

  case 167:
#line 1979 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival * yyvsp[0].ival;	}
    break;

  case 168:
#line 1982 "zmac.y"
    {	if (yyvsp[0].ival == 0) err[eflag]++; else yyval.ival = yyvsp[-2].ival % yyvsp[0].ival;	}
    break;

  case 169:
#line 1985 "zmac.y"
    {	if (yyvsp[0].ival == 0) err[eflag]++; else yyval.ival = yyvsp[-2].ival % yyvsp[0].ival;	}
    break;

  case 170:
#line 1988 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival & yyvsp[0].ival;	}
    break;

  case 171:
#line 1991 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival & yyvsp[0].ival;	}
    break;

  case 172:
#line 1994 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival | yyvsp[0].ival;	}
    break;

  case 173:
#line 1997 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival | yyvsp[0].ival;	}
    break;

  case 174:
#line 2000 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival ^ yyvsp[0].ival;	}
    break;

  case 175:
#line 2003 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival ^ yyvsp[0].ival;	}
    break;

  case 176:
#line 2006 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival << yyvsp[0].ival;	}
    break;

  case 177:
#line 2009 "zmac.y"
    {	yyval.ival = ((yyvsp[-2].ival >> 1) & 077777) >> (yyvsp[0].ival - 1);	}
    break;

  case 178:
#line 2012 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival < yyvsp[0].ival;	}
    break;

  case 179:
#line 2015 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival == yyvsp[0].ival;	}
    break;

  case 180:
#line 2018 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival > yyvsp[0].ival;	}
    break;

  case 181:
#line 2021 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival < yyvsp[0].ival;	}
    break;

  case 182:
#line 2024 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival == yyvsp[0].ival;	}
    break;

  case 183:
#line 2027 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival > yyvsp[0].ival;	}
    break;

  case 184:
#line 2030 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival <= yyvsp[0].ival;	}
    break;

  case 185:
#line 2033 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival >= yyvsp[0].ival;	}
    break;

  case 186:
#line 2036 "zmac.y"
    {	yyval.ival = yyvsp[-2].ival != yyvsp[0].ival;	}
    break;

  case 187:
#line 2039 "zmac.y"
    {	yyval.ival = yyvsp[-1].ival;	}
    break;

  case 188:
#line 2042 "zmac.y"
    {	yyval.ival = ~yyvsp[0].ival;	}
    break;

  case 189:
#line 2045 "zmac.y"
    {	yyval.ival = ~yyvsp[0].ival;	}
    break;

  case 190:
#line 2048 "zmac.y"
    {	yyval.ival = !yyvsp[0].ival;	}
    break;

  case 191:
#line 2051 "zmac.y"
    {	yyval.ival = yyvsp[0].ival;	}
    break;

  case 192:
#line 2054 "zmac.y"
    {	yyval.ival = -yyvsp[0].ival;	}
    break;

  case 199:
#line 2073 "zmac.y"
    { int i;
		if (expptr >= MAXEXP)
			error("Macro expansion level");
		est2 = (char **) malloc((PARMMAX +4) * sizeof(char *));
		expstack[expptr] = (char *)est2 ;
		for (i=0; i<PARMMAX; i++)
			est2[i] = 0;
		arg_flag++;
	}
    break;

  case 200:
#line 2086 "zmac.y"
    {	arg_flag++;	}
    break;

  case 201:
#line 2090 "zmac.y"
    {	arg_flag = 0;	}
    break;

  case 202:
#line 2094 "zmac.y"
    {	quoteflag++;	}
    break;

  case 203:
#line 2098 "zmac.y"
    {	quoteflag = 0;	}
    break;


    }

/* Line 1010 of yacc.c.  */
#line 4050 "y.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
	  yychar = YYEMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 2102 "zmac.y"


#define F_END	0
#define OTHER	1
#define SPACE	2
#define DIGIT	3
#define LETTER	4
#define STARTER 5
#define HEXIN	6


/*
 *  This is the table of character classes.  It is used by the lexical
 *  analyser. (yylex())
 */
char	charclass[] = {
	F_END,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,
	OTHER,	SPACE,	OTHER,	OTHER,	OTHER,	SPACE,	OTHER,	OTHER,
	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,
	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,
	SPACE,	OTHER,	OTHER,	HEXIN,	HEXIN,	OTHER,	HEXIN,	OTHER,
	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,
	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,
	DIGIT,	DIGIT,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	STARTER,
	STARTER,LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
	LETTER, LETTER, LETTER, OTHER,	OTHER,	OTHER,	OTHER,	LETTER,
	OTHER,	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
	LETTER, LETTER, LETTER, OTHER,	OTHER,	OTHER,	OTHER,	OTHER,
};


/*
 *  the following table tells which characters are parts of numbers.
 *  The entry is non-zero for characters which can be parts of numbers.
 */
char	numpart[] = {
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	'0',	'1',	'2',	'3',	'4',	'5',	'6',	'7',
	'8',	'9',	0,	0,	0,	0,	0,	0,
	0,	'A',	'B',	'C',	'D',	'E',	'F',	0,
	'H',	0,	0,	0,	0,	0,	0,	'O',
	0,	'Q',	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	'a',	'b',	'c',	'd',	'e',	'f',	0,
	'h',	0,	0,	0,	0,	0,	0,	'o',
	0,	'q',	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0};




/*
 *  the following table is a list of assembler mnemonics;
 *  for each mnemonic the associated machine-code bit pattern
 *  and symbol type are given.
 */
struct	item	keytab[] = {
	{"a",	7,	ACC,		0},
	{"adc",	1,	ARITHC,		0},
	{"add",	0,	ADD,		0},
	{"af",	060,	AF,		0},
	{"and",	4,	AND,		0},
	{"ascii",0,	DEFB,		0},
	{"b",	0,	REGNAME,	0},
	{"bc",	0,	RP,		0},
	{"bit",	0145500,BIT,		0},
	{"block",0,	DEFS,		0},
	{"byte",0,	DEFB,		0},
	{"c",	1,	C,		0},
	{"call",0315,	CALL,		0},
	{"ccf",	077,	NOOPERAND,	0},
	{"cmp",	7,	LOGICAL,	0},		/* -cdk */
	{"cond",0,	IF,		0},
	{"cp",	7,	LOGICAL,	0},
	{"cpd",	0166651,NOOPERAND,	0},
	{"cpdr",0166671,NOOPERAND,	0},
	{"cpi",	0166641,NOOPERAND,	0},
	{"cpir",0166661,NOOPERAND,	0},
	{"cpl",	057,	NOOPERAND,	0},
	{"d",	2,	REGNAME,	0},
	{"daa",	0047,	NOOPERAND,	0},
	{"db",	0,	DEFB,		0},
	{"de",	020,	RP,		0},
	{"dec",	1,	INCDEC,		0},
	{"defb",0,	DEFB,		0},
	{"defl",0,	DEFL,		0},
	{"defm",0,	DEFB,		0},
	{"defs",0,	DEFS,		0},
	{"defw",0,	DEFW,		0},
	{"dephase",0,	DEPHASE,	0},
	{"di",	0363,	NOOPERAND,	0},
	{"djnz",020,	DJNZ,		0},
	{"ds",	0,	DEFS,		0},
	{"dw",	0,	DEFW,		0},
	{"e",	3,	REGNAME,	0},
	{"ei",	0373,	NOOPERAND,	0},
	{"eject",1,	LIST,		0},
	{"elist",3,	LIST,		0},
	{"else",0,	ELSE,		0},
	{"end",	0,	END,		0},
	{"endc",0,	ENDIF,		0},
	{"endif",0,	ENDIF,		0},
	{"endm", 0,	ENDM,		0},
	{"eq",	0,	EQ,		0},
	{"equ",	0,	EQU,		0},
	{"ex",	0,	EX,		0},
	{"exx",	0331,	NOOPERAND,	0},
	{"f",	0,	F,		0},
	{"flist",4,	LIST,		0},
	{"ge",	0,	GE,		0},
	{"glist",5,	LIST,		0},
	{"gt",	0,	GT,		0},
	{"h",	4,	REGNAME,	0},
	{"halt",0166,	NOOPERAND,	0},
	{"hl",	040,	HL,		0},
	{"i",	0,	MISCREG,	0},
	{"if",	0,	IF,		0},
	{"im",	0166506,IM,		0},
	{"in",	0333,	IN,		0},
	{"inc",	0,	INCDEC,		0},
	{"include", 3,	ARGPSEUDO,	0},
	{"ind",	0166652,NOOPERAND,	0},
	{"indr",0166672,NOOPERAND,	0},
	{"ini",	0166642,NOOPERAND,	0},
	{"inir",0166662,NOOPERAND,	0},
	{"ix",	0156440,INDEX,		0},
	{"ixh",	0156404,HIDREG,		0},
	{"ixl",	0156405,HIDREG,		0},
	{"iy",	0176440,INDEX,		0},
	{"iyh",	0176404,HIDREG,		0},
	{"iyl",	0176405,HIDREG,		0},
	{"jmp",	0303,	JP,		0},		/* -cdk */
	{"jp",	0303,	JP,		0},
	{"jr",	040,	JR,		0},
	{"l",	5,	REGNAME,	0},
	{"ld",	0,	LD,		0},
	{"ldd",	0166650,NOOPERAND,	0},
	{"lddr",0166670,NOOPERAND,	0},
	{"ldi",	0166640,NOOPERAND,	0},
	{"ldir",0166660,NOOPERAND,	0},
	{"le",	0,	LE,		0},
	{"list",0,	LIST,		0},
	{"lt",	0,	LT,		0},
	{"m",	070,	COND,		0},
	{"macro",0,	MACRO,		0},
	{"max",	1,	MINMAX,		0},
	{"min",	0,	MINMAX,		0},
	{"mlist",6,	LIST,		0},
	{"mod",	0,	MOD,		0},
	{"nc",	020,	SPCOND,		0},
	{"ne",	0,	NE,		0},
	{"neg",	0166504,NOOPERAND,	0},
	{"nolist",-1,	LIST,		0},
	{"nop",	0,	NOOPERAND,	0},
	{"not",	0,	NOT,		0},
	{"nv",	040,	COND,		0},
	{"nz",	0,	SPCOND,		0},
	{"or",	6,	OR,		0},
	{"org",	0,	ORG,		0},
	{"otdr",0166673,NOOPERAND,	0},
	{"otir",0166663,NOOPERAND,	0},
	{"out",	0323,	OUT,		0},
	{"outd",0166653,NOOPERAND,	0},
	{"outi",0166643,NOOPERAND,	0},
	{"p",	060,	COND,		0},
	{"pe",	050,	COND,		0},
	{"phase",0,	PHASE,		0},
	{"po",	040,	COND,		0},
	{"pop",	0301,	PUSHPOP,	0},
	{"push", 0305,	PUSHPOP,	0},
	{"r",	010,	MISCREG,	0},
	{"read", 3,	ARGPSEUDO,	0},
	{"res",	0145600,RES,		0},
	{"ret",	0311,	RET,		0},
	{"reti",0166515,NOOPERAND,	0},
	{"retn",0166505,NOOPERAND,	0},
	{"rl",	2,	SHIFT,		0},
	{"rla",	027,	NOOPERAND,	0},
	{"rlc",	0,	SHIFT,		0},
	{"rlca",07,	NOOPERAND,	0},
	{"rld",	0166557,NOOPERAND,	0},
	{"rmem",0,	DEFS,		0},
	{"rr",	3,	SHIFT,		0},
	{"rra",	037,	NOOPERAND,	0},
	{"rrc",	1,	SHIFT,		0},
	{"rrca",017,	NOOPERAND,	0},
	{"rrd",	0166547,NOOPERAND,	0},
	{"rst",	0307,	RST,		0},
	{"rsym",1,	ARGPSEUDO,	0},
	{"sbc",	3,	ARITHC,		0},
	{"scf",	067,	NOOPERAND,	0},
	{"set",	0145700,RES,		0},
	{"shl",	0,	SHL,		0},
	{"shr",	0,	SHR,		0},
	{"sla",	4,	SHIFT,		0},
	{"sll",	6,	SHIFT,		0}, /* Undocumented */
	{"sp",	060,	SP,		0},
	{"space",2,	LIST,		0},
	{"sra",	5,	SHIFT,		0},
	{"srl",	7,	SHIFT,		0},
	{"sub",	2,	LOGICAL,	0},
	{"text",0,	DEFB,		0},
	{"title",0,	ARGPSEUDO,	0},
	{"v",	050,	COND,		0},
	{"word",0,	DEFW,		0},
	{"wsym",2,	ARGPSEUDO,	0},
	{"xor",	5,	XOR,		0},
	{"z",	010,	SPCOND,		0}
};

/*
 *  user-defined items are tabulated in the following table.
 */

struct item	itemtab[ITEMTABLESIZE];
struct item	*itemmax = itemtab+ITEMTABLESIZE;





/*
 *  lexical analyser, called by yyparse.
 */
int yylex()
{
	int	c;
	char *p;
	int	radix;
	int  limit;
	int leadinghex = 0;

	if (arg_flag)
		return(getarg());
loop switch(charclass[c = nextchar()]) {
	case F_END:
		if (expptr) {
			popsi();
			continue;
		} else return(0);

	case SPACE:
		break;
	case LETTER:
	case STARTER:
		p = tempbuf;
		do {
			if (p >= tempmax)
				error(symlong);
			*p++ = (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
			while	((c = nextchar()) == '$')
				;
		} while	(charclass[c]==LETTER || charclass[c]==DIGIT);
		if (p - tempbuf > MAXSYMBOLSIZE)
		{
			if (pass2) warnprt (0, 1);
			p = tempbuf + MAXSYMBOLSIZE;
		}
		*p++ = '\0';
		peekc = c;
		return(tokenofitem(UNDECLARED));
	case HEXIN:
	{
		int corig = c;
		if (*ifptr) return (skipline(c));
		while ((c = nextchar ()) == '$');
		if (!numpart[c])
		{
			peekc = c;
			return (corig);
		}
		leadinghex = 1;
		/* fall through */
	}
	case DIGIT:
		if (*ifptr) return (skipline(c));
		p = tempbuf;
		do	{
			if (p >= tempmax)
				error(symlong);
			*p++ = (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
			while	((c = nextchar()) == '$');
			}
			while(numpart[c]);
		peekc = c;
		if (leadinghex)
		{
			*p++ = 'h';
		}
		*p-- = '\0';
		switch(*p)	{
			case 'o':
			case 'q':
				radix = 8;
				limit = 020000;
				*p = '\0';
				break;
			case 'd':
				radix = 10;
				limit = 6553;
				*p = '\0';
				break;
			case 'h':
				radix = 16;
				limit = 010000;
				*p = '\0';
				break;
			case 'b':
				radix = 2;
				limit = 077777;
				*p = '\0';
				break;
			default:
				radix = 10;
				limit = 6553;
				p++;
				break;
			}

		/*
		 *  tempbuf now points to the number, null terminated
		 *  with radix 'radix'.
		 */
		yylval.ival = 0;
		p = tempbuf;
		do	{
			c = *p - (*p > '9' ? ('a' - 10) : '0');
			if (c >= radix)
				{
				err[iflag]++;
				yylval.ival = 0;
				break;
				}
			if (yylval.ival < limit ||
				(radix == 10 && yylval.ival == 6553 && c < 6) ||
				(radix == 2 && yylval.ival == limit))
				yylval.ival = yylval.ival * radix + c;
			else {
				err[vflag]++;
				yylval.ival = 0;
				break;
				}
			}
			while(*++p != '\0');
		return(NUMBER);
	default:
		if (*ifptr)
			return(skipline(c));
		switch(c) {
		int corig;
		case ';':
			return(skipline(c));
		case '\'':
			if (quoteflag) return('\'');
		case '"':
			corig = c;
			p = tempbuf;
			p[1] = 0;
			do	switch(c = nextchar())	{
			case '\0':
			case '\n':
				err[bflag]++;
				goto retstring;
			case '\'':
			case '"':
				if (c == corig && (c = nextchar()) != corig) {
				retstring:
					peekc = c;
					*p = '\0';
					if ((p-tempbuf) >2) {
						yylval.cval = tempbuf;
						return(STRING);
					} else if (p-tempbuf == 2)	{
						p = tempbuf;
						yylval.ival = *p++ ;
						yylval.ival |= *p<<8;
						return(TWOCHAR);
					} else	{
						p = tempbuf;
						yylval.ival = *p++;
						return(ONECHAR);
					}
				}
			default:
				*p++ = c;
			} while (p < tempmax);
			/*
			 *  if we break out here, our string is longer than
			 *  our input line
			 */
			error("string buffer overflow");
		case '<':
			corig = c;
			switch (c = nextchar ()) {
			case '=':
				return LE;
			case '<':
				return SHL;
			case '>':
				return NE;
			default:
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		case '>':
			corig = c;
			switch (c = nextchar ()) {
			case '=':
				return GE;
			case '>':
				return SHR;
			default:
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		case '!':
			corig = c;
			switch (c = nextchar ()) {
			case '=':
				return NE;
			default:
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		case '=':
			corig = c;
			switch (c = nextchar ()) {
			case '=':
				return EQ;
			default:
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		default:
			return(c);
		}
	}
}

/*
 *  return the token associated with the string pointed to by
 *  tempbuf.  if no token is associated with the string, associate
 *  deftoken with the string and return deftoken.
 *  in either case, cause yylval to point to the relevant
 *  symbol table entry.
 */

int tokenofitem(int deftoken)
{
	char *p;
	struct item *	ip;
	int  i;
	int  r, l, u, hash;


#ifdef T_DEBUG
	fputs("'tokenofitem entry'	", stderr) ;
	fputs(tempbuf, stderr) ;
#endif
	if (strcmp (tempbuf, "cmp") == 0 ||
	    strcmp (tempbuf, "jmp") == 0 ||
	    strcmp (tempbuf, "v")   == 0 ||
	    strcmp (tempbuf, "nv")  == 0)
		if (pass2) warnprt (1, 1);
	/*
	 *  binary search
	 */
	l = 0;
	u = (sizeof keytab/sizeof keytab[0])-1;
	while (l <= u) {
		i = (l+u)/2;
		ip = &keytab[i];
		if ((r = strcmp(tempbuf, ip->i_string)) == 0)
			goto found;
		if (r < 0)
			u = i-1;
		else
			l = i+1;
	}

	/*
	 *  hash into item table
	 */
	hash = 0;
	p = tempbuf;
	while (*p) hash += *p++;
	hash %= ITEMTABLESIZE;
	ip = &itemtab[hash];

	loop {
		if (ip->i_token == 0)
			break;
		if (strcmp(tempbuf, ip->i_string) == 0)
			goto found;
		if (++ip >= itemmax)
			ip = itemtab;
	}

	if (!deftoken) {
		i = 0 ;
		goto token_done ;
	}
	if (++nitems > ITEMTABLESIZE-20)
		error("item table overflow");
	ip->i_string = malloc(strlen(tempbuf)+1);
	ip->i_token = deftoken;
	ip->i_uses = 0;
	ip->i_equbad = 0;
	strcpy(ip->i_string, tempbuf);

found:
	if (*ifptr) {
		if (ip->i_token == ENDIF) {
			i = ENDIF ;
			goto token_done ;
		}
		if (ip->i_token == ELSE) {
			/* We must only honour the ELSE if it is not
			   in a nested failed IF/ELSE */
			char forbid = 0;
			char *ifstackptr;
			for (ifstackptr = ifstack; ifstackptr != ifptr; ++ifstackptr) {
				if (*ifstackptr) {
					forbid = 1;
					break;
				}
			}
			if (!forbid) {
				i = ELSE;
				goto token_done;
			}
		}
		if (ip->i_token == IF) {
			if (ifptr >= ifstmax)
				error("Too many ifs");
			else *++ifptr = 1;
		}
		i = skipline(' ');
		goto token_done ;
	}
	yylval.itemptr = ip;
	i = ip->i_token;
	if (i == EQU) equ_bad_label=0;
token_done:
#ifdef T_DEBUG
	fputs("\t'tokenofitem exit'\n", stderr) ;
#endif
	return(i) ;
}


/*
 *  interchange two entries in the item table -- used by custom_qsort
 */
void interchange(int i, int j)
{
	struct item *fp, *tp;
	struct item temp;

	fp = &itemtab[i];
	tp = &itemtab[j];
	temp.i_string = fp->i_string;
	temp.i_value = fp->i_value;
	temp.i_token = fp->i_token;
	temp.i_uses = fp->i_uses;
	temp.i_equbad = fp->i_equbad;

	fp->i_string = tp->i_string;
	fp->i_value = tp->i_value;
	fp->i_token = tp->i_token;
	fp->i_uses = tp->i_uses;
	fp->i_equbad = tp->i_equbad;

	tp->i_string = temp.i_string;
	tp->i_value = temp.i_value;
	tp->i_token = temp.i_token;
	tp->i_uses = temp.i_uses;
	tp->i_equbad = temp.i_equbad;
}



/*
 *  quick sort -- used by putsymtab to sort the symbol table
 */
void custom_qsort(int m, int n)
{
	int  i, j;

	if (m < n) {
		i = m;
		j = n+1;
		loop {
			do i++; while(strcmp(itemtab[i].i_string,
					itemtab[m].i_string) < 0);
			do j--; while(strcmp(itemtab[j].i_string,
					itemtab[m].i_string) > 0);
			if (i < j) interchange(i, j); else break;
		}
		interchange(m, j);
		custom_qsort(m, j-1);
		custom_qsort(j+1, n);
	}
}



/*
 *  get the next character
 */
int nextchar()
{
	int c, ch;
	static  char  *earg;

	if (peekc != -1) {
		c = peekc;
		peekc = -1;
		return(c);
	}

start:
	if (earg) {
		if (*earg)
			return(addtoline(*earg++));
		earg = 0;
	}

	if (expptr) {
		if ((ch = getm()) == '\1') {	/*  expand argument  */
			ch = getm() - 'A';
			if (ch >= 0 && ch < PARMMAX && est[ch])
				earg = est[ch];
			goto start;
		}
		if (ch == '\2') {	/*  local symbol  */
			ch = getm() - 'A';
			if (ch >= 0 && ch < PARMMAX && est[ch]) {
				earg = est[ch];
				goto start;
			}
			earg = getlocal(ch, (int)(long)est[TEMPNUM]);
			goto start;
		}

		return(addtoline(ch));
	}
	ch = getc(now_file) ;
	/* if EOF, check for include file */
	if (ch == EOF) {
		while (ch == EOF && now_in) {
			fclose(fin[now_in]) ;
			free(src_name[now_in]) ;
			now_file = fin[--now_in] ;
			ch = getc(now_file) ;
		}
		if (linein[now_in] < 0) {
			lstoff = 1 ;
			linein[now_in] = -linein[now_in] ;
		} else {
			lstoff = 0 ;
		}
		if (pass2 && iflist()) {
			lineout() ;
			fprintf(fout, "**** %s ****\n", src_name[now_in]) ;
		}
	}
	if (ch == '\n')
	{
		linein[now_in]++ ;
	}

	return(addtoline(ch)) ;
}


/*
 *  skip to rest of the line -- comments and if skipped lines
 */
int skipline(int ac)
{
	int  c;

	c = ac;
	while (c != '\n' && c != '\0')
		c = nextchar();
	return('\n');
}



void usage()
{
	printf(
"zmac " ZMAC_VERSION
#ifdef ZMAC_BETA
	ZMAC_BETA
#endif
", a Z80 macro cross-assembler.\n"
"Public domain by Bruce Norskog and others.\n"
"\n"
#ifdef __riscos
"usage: zmac [--help] [--version] [-AbcdefghilLmnOpsStTz]\n"
#else
"usage: zmac [--help] [--version] [-AbcdefghilLmnOpsStz]\n"
#endif
"       [-o outfile] [-x listfile] [filename]\n"
"\n"
"       --help  give this usage help.\n"
"       --version  report version number.\n"
"       -A      output AMSDOS binary file rather than default binary file.\n"
"       -b      don't generate the m/c output at all.\n"
"       -c      make the listing continuous, i.e. don't generate any\n"
"               page breaks or page headers. (This is the default.)\n"
"       -d      make the listing discontinuous.\n"
"       -e      omit the `error report' section in the listing.\n"
"       -f      list instructions not assembled due to `if' expressions being\n"
"               false. (Normally these are not shown in the listing.)\n"
"       -g      list only the first line of equivalent hex for a source line.\n"
"       -h      output CP/M-ish Intel hex format (using extension `.hex')\n"
"               rather than default binary file (extension `.bin').\n"
"       -i      don't list files included with `include'.\n"
"       -l      don't generate a listing at all.\n"
"       -L      generate listing; overrides any conflicting options.\n"
"       -m      list macro expansions.\n"
"       -n      omit line numbers from listing.\n"
"       -o      assemble output to `outfile'.\n"
"       -O      suggest possible optimisations (as warnings).\n"
"       -p      use linefeeds for page break in listing rather than ^L.\n"
"       -s      omit the symbol table from the listing.\n"
"       -S      show relevant line when reporting errors.\n"
"       -t      give terse (single-letter) error codes in listing.\n"
#ifdef __riscos
"       -T      enable DDE throwback for reporting warnings and errors.\n"
#endif
"       -x      generate listing to `listfile' (`-' for stdout).\n"
"       -z      accept 8080-compatible instructions only; flag any\n"
"               Z80-specific ones as errors.\n");
}



int main(int argc, char *argv[])
{
	struct item *ip;
	int  i, c;

#ifdef ZMAC_BETA
	printf ("*** THIS IS A BETA VERSION; NOT FOR GENERAL DISTRIBUTION ***\n");
#endif

	if(argc==1)
		usage(),exit(0);

	if(argc>=2) {
		if(strcmp(argv[1],"--help")==0)
			usage(),exit(0);
		else if(strcmp(argv[1],"--version")==0)
			puts("zmac " ZMAC_VERSION
#ifdef ZMAC_BETA
					ZMAC_BETA
#endif
				),exit(0);
	}

	fout = stdout ;
	fin[0] = stdin ;
	now_file = stdin ;

	*bin = *listf = 0;
	optnerr = 0;

	while((c = getoptn(argc,argv,
#ifdef __riscos
		"AbcdefghilLmno:OpsStTx:z"
#else
		"AbcdefghilLmno:OpsStx:z"
#endif
		)) != EOF) {
		switch(c) {

		case 'A':	/*  AMSDOS binary -mep */
			output_amsdos = 1;
			output_hex = 0;
			break;

		case 'b':	/*  no binary  */
			bopt = 0;
			break;

		case 'c':	/*  continuous listing  */
			continuous_listing = 1;
			break;

		case 'd':	/*  discontinuous listing  */
			continuous_listing = 0;
			break;

		case 'e':	/*  error list only  */
			eopt = 0;
			edef = 0;
			break;

		case 'f':	/*  print if skipped lines  */
			fopt++;
			fdef++;
			break;

		case 'g':	/*  do not list extra code  */
			gopt = 0;
			gdef = 0;
			break;

		case 'h':	/* output .hex not .bin -rjm */
			output_hex = 1;
			output_amsdos = 0;
			break;

		case 'i':	/* do not list include files */
			iopt = 1 ;
			break;

		case 'l':	/*  no list  */
			lopt++;
			break;

		case 'L':	/*  force listing of everything */
			lston++;
			break;

		case 'm':	/*  print macro expansions  */
			mdef++;
			mopt++;
			break;

		case 'n':	/*  put line numbers off */
			nopt-- ;
			break;

		case 'o':	/*  specify m/c output file */
			strcpy(bin, optnarg);
			break;

		case 'O':	/*  suggest optimisations  */
			suggest_optimise = 1;
			break;

		case 'p':	/*  put out four \n's for eject */
			popt-- ;
			break;

		case 's':	/*  don't produce a symbol list  */
			sopt++;
			break;

		case 'S':	/*  show line which caused error */
			show_error_line = 1;
			break;

		case 't':	/*  terse error messages in listing  */
			terse_lst_errors = 1;
			break;

#ifdef __riscos
		case 'T':	/*  RISC OS throwback  -mep */
			riscos_thbk = 1;
			break;
#endif

		case 'x':	/*  specify listing file */
			if(strcmp(optnarg, "-") == 0)
				oldoopt++;	/* list to stdout (old `-o') */
			else
				strcpy(listf, optnarg);
			break;

		case 'z':	/*  8080-compatible ops only  */
			output_8080_only = 1;
			break;

		case '?':
		default:	/*  error  */
			justerror("Unknown option or missing argument");
			break;

		}
	}

	if(optnind != argc-1) justerror("Missing, extra or mispositioned argument");

	atexit (doatexit);

	sourcef = argv[optnind];
	strcpy(src, sourcef);

	if ((now_file = fopen(src, "r")) == NULL)
	{
		/* If filename has no pre-existing suffix, then try .z */
		suffix_if_none (src, "z");
		if ((now_file = fopen(src, "r")) == NULL)
			fileerror("Cannot open source file", src);
	}
	now_in = 0 ;
	fin[now_in] = now_file ;
	src_name[now_in] = src ;
#ifdef __riscos
	riscos_set_csd(src); /* -mep */
#endif

	/* If we haven't got a bin file filename, then create one from the
	 * source filename (.hex extension if option -h is specified).
	 */
	if (*bin == 0) {
		strcpy(bin, sourcef);
		if (output_hex)
			suffix(bin,"hex");
		else
			suffix(bin,"bin");
	}
	if (bopt)
		if (( fbuf = fopen(bin, output_hex ? "w" : "wb")) == NULL)
			fileerror("Cannot create binary file", bin);
	if (output_amsdos)
		for(i=0; i<128; i++)
			putc(0,fbuf); /* -mep */

	if (!lopt && !oldoopt) {
		/* If we've not got a filename for the listing file
		 * (-x option) then create one from the source filename
		 * (.lst extension)
		 */
		if( *listf == 0 ) {
			strcpy(listf, sourcef);
			suffix(listf,"lst");
		}
		if ((fout = fopen(listf, "w")) == NULL)
			fileerror("Cannot create list file", listf);
	} else
		fout = stdout ;

	strcpy(mtmp, sourcef);
	suffix(mtmp,"tmp");
	mfile = mfopen(mtmp,"w+b") ;
	if (mfile == NULL) {
		fileerror("Cannot create temp file", mtmp);
	}

	/*
	 *  get the time
	 */
	time(&now);
	timp = ctime(&now);
	timp[16] = 0;
	timp[24] = 0;

	title = sourcef;
	/*
	 * pass 1
	 */
#ifdef DEBUG
	fputs("DEBUG-pass 1\n", stderr) ;
#endif
	setvars();
	yyparse();
	pass2++;
	ip = &itemtab[-1];
	while (++ip < itemmax) {
		/* reset use count */
		ip->i_uses = 0 ;

		/* set macro names, equated and defined names */
		switch	(ip->i_token) {
		case MNAME:
			ip->i_token = OLDMNAME;
			break;

		case EQUATED:
			ip->i_token = WASEQUATED;
			break;

		case DEFLED:
			ip->i_token = UNDECLARED;
			break;
		}
	}
	setvars();
	fseek(now_file, (long)0, 0);

#ifdef DEBUG
	fputs("DEBUG- pass 2\n", stderr) ;
#endif
	yyparse();


	if (bopt) {
		flushbin();
		if (output_hex) {
			putc(':', fbuf);
			if (xeq_flag) {
				puthex(0, fbuf);
				puthex(xeq >> 8, fbuf);
				puthex(xeq, fbuf);
				puthex(1, fbuf);
				puthex(255-(xeq >> 8)-xeq, fbuf);
			} else
				for	(i = 0; i < 10; i++)
					putc('0', fbuf);
			putc('\n', fbuf);
		}
		if (output_amsdos) {
			char leafname[] = "FILENAMEBIN";
			unsigned int chk;
			unsigned int filelen = dollarsign - first_org_store;
			if (filelen & 0x7f)
			{
				putc (0x1a, fbuf); /* CP/M EOF char */
			}
			rewind(fbuf);
			chk=0;
			putc(0,fbuf);
			for(i=0;i<11;i++) {
				putc(leafname[i],fbuf);
				chk+=leafname[i];
			}
			for(i=0;i<6;i++)
				putc(0,fbuf);
			putc(2,fbuf); /* Unprotected binary */
			chk+=2;
			putc(0,fbuf);
			putc(0,fbuf);
			putc(first_org_store & 0xFF,fbuf);
			chk+=first_org_store & 0xFF;
			putc(first_org_store >> 8,fbuf);
			chk+=first_org_store >> 8;
			putc(0,fbuf);
			putc(filelen & 0xFF,fbuf);
			chk+=filelen & 0xFF;
			putc(filelen >> 8,fbuf);
			chk+=filelen >> 8;
			/* Next bit should be entry address really */
			putc(first_org_store & 0xFF,fbuf);
			chk+=first_org_store & 0xFF;
			putc(first_org_store >> 8,fbuf);
			chk+=first_org_store >> 8;
			for(i=28;i<64;i++)
				putc(0,fbuf);
			putc(filelen & 0xFF,fbuf);
			chk+=filelen & 0xFF;
			putc(filelen >> 8,fbuf);
			chk+=filelen >> 8;
			putc(0,fbuf); /* this would be used if length>64K */
			putc(chk & 0xFF,fbuf);
			putc(chk >> 8,fbuf);
		}
		fflush(fbuf);
	}

	if (!lopt)
		fflush(fout);
	if (*writesyms)
		outsymtab(writesyms);
	if (eopt)
		erreport();
	if (!lopt && !sopt)
		putsymtab();
	if (!lopt) {
		eject();
		fflush(fout);
	}
	exit(had_errors);
	return(had_errors); /* main () does return int, after all... */
}


void doatexit (void)
{
#ifdef __riscos
	if (riscos_throwback_started)
	{
		_swix(DDEUtils_ThrowbackEnd,0);
	}
	_swix(DDEUtils_Prefix,1,0); /* Unset CSD */
#endif
}


/*
 *  set some data values before each pass
 */
void setvars()
{
	int  i;

	peekc = -1;
	linein[now_in] = linecnt = 0;
	exp_number = 0;
	emitptr = emitbuf;
	lineptr = linebuf;
	ifptr = ifstack;
	expifp = expif;
	*ifptr = 0;
	dollarsign = 0;
	olddollar = 0;
	phaseflag = 0;
	for (i=0; i<FLAGS; i++) err[i] = 0;
}



/*
 *  print out an error message and die
 */
void error(char *as)
{
	*linemax = 0;
	fprintf(fout, "%s\n", linebuf);
	fflush(fout);
	fprintf(stderr, "%s\n", as) ;
	exit(1);
}


/*
 *  alternate version
 */
void fileerror(char *as,char *filename)
{
	*linemax = 0;
	if (fout != NULL && fout != stdout)
		fprintf(fout, "%s\n", linebuf);
	fflush(fout);
	fprintf(stderr, "%s `%s'\n", as, filename) ;
	exit(1);
}



/*
 *  alternate alternate version
 */
void justerror(char *as)
{
	fprintf(stderr, "%s\n", as) ;
	exit(1);
}


/*
 *  output the symbol table
 */
void putsymtab()
{
	struct item *tp, *fp;
	int  i, j, k, t, rows;
	char c, c1 ;

	if (!nitems)
		return;

	/* compact the table so unused and UNDECLARED entries are removed */
	tp = &itemtab[-1];
	for (fp = itemtab; fp<itemmax; fp++) {
		if (fp->i_token == UNDECLARED) {
			nitems--;
			continue;
		}
		if (fp->i_token == 0)
			continue;
		tp++;
		if (tp != fp) {
			tp->i_string = fp->i_string;
			tp->i_value = fp->i_value;
			tp->i_token = fp->i_token;
			tp->i_uses = fp->i_uses ;
			tp->i_equbad = fp->i_equbad ;
		}
	}

	tp++;
	tp->i_string = "{";

	/*  sort the table */
	custom_qsort(0, nitems-1);

	title = "**  Symbol Table  **";

	rows = (nitems+3) / 3;
	if (rows+5+line > 60)
		eject();
	lineout();
	fprintf(fout,"\n\n\nSymbol Table:\n\n");
	line += 4;

	for (i=0; i<rows; i++) {
		for(j=0; j<3; j++) {
			k = rows*j+i;
			if (k < nitems) {
				tp = &itemtab[k];
				t = tp->i_token;
				c = ' ' ;
				if (t == EQUATED || t == DEFLED)
					c = '=' ;
				if (tp->i_uses == 0)
					c1 = '+' ;
				else
					c1 = ' ' ;
				fprintf(fout, "%-15s%c%4x%c    ",
					tp->i_string, c, tp->i_value & 0xffff, c1);
			}
		}
		lineout();
		putc('\n', fout);
	}
}




/*
 *  put out error report
 */
void erreport()
{
	int i, numerr;

	if (line > 50) eject();
	lineout();
	numerr = 0;
	for (i=0; i<FLAGS; i++) numerr += keeperr[i];
	if (numerr) {
		fputs("\n\n\nError report:\n\n", fout);
		fprintf(fout, "%6d error%s\n", DO_PLURAL(numerr));
		line += 5;
	} else {
		fputs("\n\n\nStatistics:\n", fout);
		line += 3;
	}

	for (i=0; i<FLAGS; i++)
		if (keeperr[i]) {
			lineout();
			if (terse_lst_errors)
				/* no plural on this because it would
				 * odd, I think. -rjm
				 */
				fprintf(fout, "%6d %c -- %s error\n",
					keeperr[i], errlet[i], errname[i]);
			else
				/* can't use DO_PLURAL for this due to
				 * the %s in the middle... -rjm
				 */
				fprintf(fout, "%6d %s error%s\n",
					keeperr[i], errname[i],
					(keeperr[i]==1)?"":"s");
		}

	if (line > 55) eject();
	lineout();
	fprintf(fout, "\n%6d\tsymbol%s\n", DO_PLURAL(nitems));
	fprintf(fout, "%6d\tbyte%s\n", DO_PLURAL(nbytes));
	line += 2;
	if (mfptr) {
		if (line > 53) eject();
		lineout();
		fprintf(fout, "\n%6d\tmacro call%s\n", DO_PLURAL(exp_number));
		fprintf(fout, "%6d\tmacro byte%s\n", DO_PLURAL(mfptr));
		fprintf(fout, "%6d\tinvented symbol%s\n",
						DO_PLURAL(invented/2));
		line += 3;
	}
}


/*
 *  lexical analyser for macro definition
 */
void mlex()
{
	char  *p;
	int  c;
	int  t;

	/*
	 *  move text onto macro file, changing formal parameters
	 */
#ifdef	M_DEBUG
	fprintf(stderr,"enter 'mlex'\n") ;
#endif
	inmlex++;

	c = nextchar();
loop {
	switch(charclass[c]) {

	case DIGIT:
		while (numpart[c]) {
			putm(c);
			c = nextchar();
		}
		continue;

	case STARTER:
	case LETTER:
		t = 0;
		p = tempbuf+MAXSYMBOLSIZE+2;
		do {
			if (p >= tempmax)
				error(symlong);
			*p++ = c;
			if (t < MAXSYMBOLSIZE)
				tempbuf[t++] = (c >= 'A' && c <= 'Z')  ?
					c+'a'-'A' : c;
			else
				if (pass2) warnprt (0, 1);
			c = nextchar();
		} while (charclass[c]==LETTER || charclass[c]==DIGIT);

		tempbuf[t] = 0;
		*p++ = '\0';
		p = tempbuf+MAXSYMBOLSIZE+2;
		t = tokenofitem(0);
		if (t != MPARM) while (*p) putm(*p++);
		else {
			if (*(yylval.itemptr->i_string) == '?') putm('\2');
			else putm('\1');
			putm(yylval.itemptr->i_value + 'A');
		}
		if (t == ENDM) goto done;
		continue;

	case F_END:
		if (expptr) {
			popsi();
			c = nextchar();
			continue;
		}

		goto done;

	default:
		if (c == '\n') {
			linecnt++;
		}
		if (c != '\1') putm(c);
		c = nextchar();
	}
}

	/*
	 *  finish off the file entry
	 */
done:
	while(c != EOF && c != '\n' && c != '\0') c = nextchar();
	linecnt++;
	putm('\n');
	putm('\n');
	putm(0);

	for (c=0; c<ITEMTABLESIZE; c++)
		if (itemtab[c].i_token == MPARM) {
			itemtab[c].i_token = UNDECLARED;
		}
	inmlex = 0;
#ifdef	M_DEBUG
	fprintf(stderr,"exit 'mlex'\n") ;
#endif
}



/*
 *  lexical analyser for the arguments of a macro call
 */
int getarg()
{
	int c;
	char *p;
	static int comma;

	*tempbuf = 0;
	yylval.cval = tempbuf;
	while(charclass[c = nextchar()] == SPACE);

	switch(c) {

	case '\0':
		popsi();
	case '\n':
	case ';':
		comma = 0;
		return(skipline(c));

	case ',':
		if (comma) {
			comma = 0;
			return(',');
		}
		else {
			comma++;
			return(ARG);
		}

	case '\'':
		p = tempbuf;
		do switch (c = nextchar()) {
			case '\0':
			case '\n':
				peekc = c;
				*p = 0;
				err[bflag]++;
				return(ARG);
			case '\'':
				if ((c = nextchar()) != '\'') {
					peekc = c;
					*p = '\0';
					comma++;
					return(ARG);
				}
			default:
				*p++ = c;
		} while (p < tempmax);
		error(symlong);		/* doesn't return */

	default:  /* unquoted string */
		p = tempbuf;
		peekc = c;
		do switch(c = nextchar()) {
			case '\0':
			case '\n':
			case '\t':
			case ' ':
			case ',':
				peekc = c;
				*p = '\0';
				comma++;
				return(ARG);
			default:
				*p++ = c;
		} while (p < tempmax);
	}

	/* in practice it can't get here, but FWIW and to satisfy
	 * -Wall... -rjm */
	error("can't happen - in zmac.y:getarg(), infinite unquoted string!?");
	return(0);
}



/*
 * Add suffix to pathname if leafname doesn't already have a suffix.
 * The suffix passed should not include an extension separator.
 * The pathname passed should be in local format.
 */
void suffix_if_none (char *str, char *suff)
{
	char *leafname, *extension;

	leafname = strrchr (str, OS_DIR_SEP);
	if (leafname == NULL)
	{
		leafname = str;
	}

	extension = strchr (leafname, OS_EXT_SEP);
	if (extension == NULL)
	{
		size_t leafsize = strlen (leafname);

		leafname[leafsize] = OS_EXT_SEP;
		strcpy (leafname + leafsize + 1, suff);
	}
}



/*
 * Add or change pathname suffix.
 * The suffix passed should not include an extension separator.
 * The pathname passed should be in local format.
 * If the leafname passed has more than one extension, the last is changed.
 */
void suffix (char *str, char *suff)
{
	char *leafname, *extension;

	leafname = strrchr (str, OS_DIR_SEP);
	if (leafname == NULL)
	{
		leafname = str;
	}

	extension = strrchr (leafname, OS_EXT_SEP);
	if (extension == NULL)
	{
		extension = leafname + strlen (leafname);
	}

	*extension = OS_EXT_SEP;
	strcpy (extension + 1, suff);
}



/*
 * Decanonicalise a canonical pathname.
 * A canonical pathname uses '/' as the directory separator,
 * '.' as the extension separator, ".." as the parent directory,
 * "." as the current directory, and a leading '/' as the root
 * directory (it would be more user-friendly not to use this!).
 */
void decanonicalise (char *pathname)
{
#if defined (MSDOS)

	char *directory = pathname;

	/* Just need to change all '/'s to '\'s */

	while ((directory = strchr (directory, '/')) != NULL)
	{
		*directory = OS_DIR_SEP;
	}

#elif defined (__riscos)

	char *directory = pathname, *dirend;

	/* First deal with leading '/' */

	if (*directory == '/')
	{
		memmove (directory + 1, directory, strlen (directory) + 1);
		*directory = '$';
		++directory;
	}

	/* Then deal with non-leaf ".."s and "."s */

	while (1)
	{
		dirend = strchr (directory, '/');
		if (dirend == NULL)
		{
			break;
		}

		*dirend = '\0';

		if (strcmp (directory, "..") == 0)
		{
			*directory = '^';
			memmove (directory + 2, directory + 3, strlen (directory + 3) + 1);
			dirend = directory + 1;
		}
		else if (strcmp (directory, ".") == 0)
		{
			memmove (directory, directory + 2, strlen (directory + 2) + 1);
			continue;
		}

		*dirend = '/';
		directory = dirend + 1;
	}

	directory = pathname;

	/* Finally, swap '/' and '.' */

	while ((directory = strpbrk (directory, "/.")) != NULL)
	{
		if (*directory == '/')
		{
			*directory = OS_DIR_SEP;
		}
		else
		{
			*directory = OS_EXT_SEP;
		}
		++directory;
	}

#else

	/* Local form is canonical form */

	UNUSED (pathname);

#endif
}



/*
 *  put out a byte to the macro file, keeping the offset
 */
void putm(char c)
{
#ifdef M_DEBUG
	fputc (c, stderr);
#endif
	mfptr++;
	mfputc(c,mfile) ;
}



/*
 *  get a byte from the macro file
 */
int getm()
{
	int ch;

	floc++;
	ch = mfgetc(mfile) ;
	if (ch == EOF) {
		ch = 0;
		fprintf(stderr,"bad macro read\n") ;
	}
	return(ch);
}



/*
 *  pop standard input
 */
void popsi()
{
	int  i;

	if (est)
	{
		for (i=0; i<PARMMAX; i++) {
			if (est[i])
#ifdef M_DEBUG
	fprintf (stderr, "[Freeing arg%u(%p)]\n", i, est[i]),
#endif
					free(est[i]);
		}
		floc = est[FLOC];
		free(est);
		expptr--;
		est = expptr ? (char **) expstack[expptr-1] : (char **) 0;
		mfseek(mfile, (long)floc, 0);
		if (lineptr > linebuf) lineptr--;
	}
}



/*
 *  return a unique name for a local symbol
 *  c is the parameter number, n is the macro number.
 */

char *getlocal(int c, int n)
{
	static char local_label[10];

	invented++;
	if (c >= 26)
		c += 'a' - '0';
	sprintf(local_label, "?%c%04d", c+'a', n) ;
	return(local_label);
}



/*
 *  read in a symbol table
 */
void insymtab(char *name)
{
	struct stab *t;
	int  s, i;
	FILE *sfile;

	t = (struct stab *) tempbuf;
	decanonicalise (name);
	if ((sfile = fopen(name, "rb")) == NULL)
		return;
	fread((char *)t, 1, sizeof *t, sfile);
	if (t->t_value != SYMMAJIC)
	{
		fclose (sfile);
		return;
	}

	s = t->t_token;
	for (i=0; i<s; i++) {
		fread((char *)t, 1, sizeof *t, sfile);
		if (tokenofitem(UNDECLARED) != UNDECLARED)
			continue;
		yylval.itemptr->i_token = t->t_token;
		yylval.itemptr->i_value = t->t_value;
		if (t->t_token == MACRO)
			yylval.itemptr->i_value += mfptr;
	}

	while ((s = fread(tempbuf, 1, TEMPBUFSIZE, sfile)) > 0) {
		mfptr += s;
		mfwrite(tempbuf, 1, s, mfile) ;
	}

	fclose (sfile);
}



/*
 *  write out symbol table
 */
void outsymtab(char *name)
{
	struct stab *t;
	struct item *ip;
	int  i;
	FILE *sfile;

	t = (struct stab *) tempbuf;
	decanonicalise (name);
	if ((sfile = fopen(name, "wb")) == NULL)
		return;
	for (ip=itemtab; ip<itemmax; ip++) {
		if (ip->i_token == UNDECLARED) {
			ip->i_token = 0;
			nitems--;
		}
	}

	copyname(title, (char *)t);
	t->t_value = SYMMAJIC;
	t->t_token = nitems;
	fwrite((char *)t, 1, sizeof *t, sfile);

	for (ip=itemtab; ip<itemmax; ip++) {
		if (ip->i_token != 0) {
			t->t_token = ip->i_token;
			t->t_value = ip->i_value;
			copyname(ip->i_string, (char *)t);
			fwrite((char *)t, 1, sizeof *t, sfile);
		}
	}

	mfseek(mfile, (long)0, 0);
	while((i = mfread(tempbuf, 1, TEMPBUFSIZE, mfile) ) > 0)
		fwrite(tempbuf, 1, i, sfile);

	fclose (sfile);
}



/*
 *  copy a name into the symbol file
 */
void copyname(char *st1, char *st2)
{
	char  *s1, *s2;
	int  i;

	i = (MAXSYMBOLSIZE+2) & ~01;
	s1 = st1;
	s2 = st2;

	while((*s2++ = *s1++)) i--;		/* -Wall-ishness :-) -RJM */
	while(--i > 0) *s2++ = '\0';
}

/* get the next source file */
void next_source(char *sp)
{

	if(now_in == NEST_IN -1)
		error("Too many nested includes") ;
	decanonicalise (sp);
	if ((now_file = fopen(sp, "r")) == NULL) {
#ifdef __riscos
		if (riscos_thbk)
			riscos_throwback(2,src_name[now_in],linein[now_in],"Cannot open include file");
#endif
		fileerror("Cannot open include file", sp) ;
	}
	if (pass2 && iflist()) {
		lineout() ;
		fprintf(fout, "**** %s ****\n",sp) ;
	}

	/* save the list control flag with the current line number */
	if (lstoff)
		linein[now_in] = - linein[now_in] ;

	/* no list if include files are turned off */
	lstoff |= iopt ;

	/* save the new file descriptor. */
	fin[++now_in] = now_file ;
	/* start with line 0 */
	linein[now_in] = 0 ;
	/* save away the file name */
	src_name[now_in] = malloc(strlen(sp)+1) ;
	strcpy(src_name[now_in],sp) ;
}

#ifdef __riscos
/*
 * On entry sp should point to the full pathname of a file in RISC OS form.
 * Searches for the last dot, and sets the local CSD to that path.
 * Does not corrupt the string.
 */
void riscos_set_csd(char *sp)
{
	char *s1 = strrchr (sp, '.');

	if (s1 != NULL)
	{
		*s1=0;
		_swix(DDEUtils_Prefix,1,sp);
		*s1='.';
	}
}

void riscos_throwback(int severity, char *file, int line, char *error)
{
	if (riscos_throwback_started==0)
	{
		riscos_throwback_started=1;
		*riscos_thbkf=0;
		_swix(DDEUtils_ThrowbackStart,0);
	}
	if (strcmp(file, riscos_thbkf)!=0)
	{
		_swix(DDEUtils_ThrowbackSend,4+1,0,file); /* Notify of a change of file */
		strcpy(riscos_thbkf,file);
	}
	_swix(DDEUtils_ThrowbackSend,32+16+8+4+1,1,file,line,severity,error);
}
#endif


