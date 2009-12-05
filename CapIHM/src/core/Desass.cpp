/**
 * File stolen to iDSK ...
 */
/**
 *   $Id$
 *	 CapriceReloded an Amstrad CPC emulator
 *   Copyright (C) 2009  cpcsdk crew
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */  


#include <iostream>
#include <cstring>
#include <cstdio>

#include "memory.h"
using namespace std;

#define Hex(Chaine, Valeur, Digit) snprintf((Chaine), (Digit), "%.*X", (Digit), (Valeur))	
//
// Tableau des OP-Codes Z80...
//
const char * TabInstrCB[ 256 ] =
{
	"RLC B","RLC C","RLC D","RLC E",
	"RLC H","RLC L","RLC (HL)","RLC A",
	"RRC B","RRC C","RRC D","RRC E",
	"RRC H","RRC L","RRC (HL)","RRC A",
	"RL B","RL C","RL D","RL E",
	"RL H","RL L","RL (HL)","RL A",
	"RR B","RR C","RR D","RR E",
	"RR H","RR L","RR (HL)","RR A",
	"SLA B","SLA C","SLA D","SLA E",
	"SLA H","SLA L","SLA (HL)","SLA A",
	"SRA B","SRA C","SRA D","SRA E",
	"SRA H","SRA L","SRA (HL)","SRA A",
	"SLL B","SLL C","SLL D","SLL E",
	"SLL H","SLL L","SLL (HL)","SLL A",
	"SRL B","SRL C","SRL D","SRL E",
	"SRL H","SRL L","SRL (HL)","SRL A",
	"BIT 0,B","BIT 0,C","BIT 0,D","BIT 0,E",
	"BIT 0,H","BIT 0,L","BIT 0,(HL)","BIT 0,A",
	"BIT 1,B","BIT 1,C","BIT 1,D","BIT 1,E",
	"BIT 1,H","BIT 1,L","BIT 1,(HL)","BIT 1,A",
	"BIT 2,B","BIT 2,C","BIT 2,D","BIT 2,E",
	"BIT 2,H","BIT 2,L","BIT 2,(HL)","BIT 2,A",
	"BIT 3,B","BIT 3,C","BIT 3,D","BIT 3,E",
	"BIT 3,H","BIT 3,L","BIT 3,(HL)","BIT 3,A",
	"BIT 4,B","BIT 4,C","BIT 4,D","BIT 4,E",
	"BIT 4,H","BIT 4,L","BIT 4,(HL)","BIT 4,A",
	"BIT 5,B","BIT 5,C","BIT 5,D","BIT 5,E",
	"BIT 5,H","BIT 5,L","BIT 5,(HL)","BIT 5,A",
	"BIT 6,B","BIT 6,C","BIT 6,D","BIT 6,E",
	"BIT 6,H","BIT 6,L","BIT 6,(HL)","BIT 6,A",
	"BIT 7,B","BIT 7,C","BIT 7,D","BIT 7,E",
	"BIT 7,H","BIT 7,L","BIT 7,(HL)","BIT 7,A",
	"RES 0,B","RES 0,C","RES 0,D","RES 0,E",
	"RES 0,H","RES 0,L","RES 0,(HL)","RES 0,A",
	"RES 1,B","RES 1,C","RES 1,D","RES 1,E",
	"RES 1,H","RES 1,L","RES 1,(HL)","RES 1,A",
	"RES 2,B","RES 2,C","RES 2,D","RES 2,E",
	"RES 2,H","RES 2,L","RES 2,(HL)","RES 2,A",
	"RES 3,B","RES 3,C","RES 3,D","RES 3,E",
	"RES 3,H","RES 3,L","RES 3,(HL)","RES 3,A",
	"RES 4,B","RES 4,C","RES 4,D","RES 4,E",
	"RES 4,H","RES 4,L","RES 4,(HL)","RES 4,A",
	"RES 5,B","RES 5,C","RES 5,D","RES 5,E",
	"RES 5,H","RES 5,L","RES 5,(HL)","RES 5,A",
	"RES 6,B","RES 6,C","RES 6,D","RES 6,E",
	"RES 6,H","RES 6,L","RES 6,(HL)","RES 6,A",
	"RES 7,B","RES 7,C","RES 7,D","RES 7,E",
	"RES 7,H","RES 7,L","RES 7,(HL)","RES 7,A",
	"SET 0,B","SET 0,C","SET 0,D","SET 0,E",
	"SET 0,H","SET 0,L","SET 0,(HL)","SET 0,A",
	"SET 1,B","SET 1,C","SET 1,D","SET 1,E",
	"SET 1,H","SET 1,L","SET 1,(HL)","SET 1,A",
	"SET 2,B","SET 2,C","SET 2,D","SET 2,E",
	"SET 2,H","SET 2,L","SET 2,(HL)","SET 2,A",
	"SET 3,B","SET 3,C","SET 3,D","SET 3,E",
	"SET 3,H","SET 3,L","SET 3,(HL)","SET 3,A",
	"SET 4,B","SET 4,C","SET 4,D","SET 4,E",
	"SET 4,H","SET 4,L","SET 4,(HL)","SET 4,A",
	"SET 5,B","SET 5,C","SET 5,D","SET 5,E",
	"SET 5,H","SET 5,L","SET 5,(HL)","SET 5,A",
	"SET 6,B","SET 6,C","SET 6,D","SET 6,E",
	"SET 6,H","SET 6,L","SET 6,(HL)","SET 6,A",
	"SET 7,B","SET 7,C","SET 7,D","SET 7,E",
	"SET 7,H","SET 7,L","SET 7,(HL)","SET 7,A"
};


const char * TabInstrED[ 256 ] =
{
	0,0,0,0, // 0
	0,0,0,0, // 4
	0,0,0,0, // 8
	0,0,0,0, // C
	0,0,0,0, // 10
	0,0,0,0, // 14
	0,0,0,0, // 18
	0,0,0,0, // 1C
	0,0,0,0, // 20
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0, // 30
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	"IN B,(C)","OUT (C),B","SBC HL,BC","LD (0x%04X),BC", // 40
	"NEG","RETN","IM 0","LD I,A",
	"IN C,(C)","OUT (C),C","ADC HL,BC","LD BC,(0x%04X)",
	0,"RETI",0,"LD R,A",
	"IN D,(C)","OUT (C),D","SBC HL,DE","LD (0x%04X),DE", // 50
	0,0,"IM 1","LD A,I",
	"IN E,(C)","OUT (C),E","ADC HL,DE","LD DE,(0x%04X)",
	0,0,"IM 2","LD A,R",
	"IN H,(C)","OUT (C),H","SBC HL,HL",0, // 60
	0,0,0,"RRD",
	"IN L,(C)","OUT (C),L","ADC HL,HL",0,
	0,0,0,"RLD",
	0,"OUT (C),0","SBC HL,SP","LD (0x%04X),SP", // 70
	0,0,0,0,
	"IN A,(C)","OUT (C),A","ADC HL,SP","LD SP,(0x%04X)",
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	"LDI","CPI","INI","OUTI",
	0,0,0,0,
	"LDD","CPD","IND","OUTD",
	0,0,0,0,
	"LDIR","CPIR","INIR","OTIR",
	0,0,0,0,
	"LDDR","CPDR","INDR","OTDR",
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0
};


const char * TabInstrDD[ 256 ] =
{
	0,0,0,0,
	0,0,0,0,
	0,"ADD IX,BC",0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,"ADD IX,DE",0,0,
	0,0,0,0,
	0,"LD IX,0x%04X","LD (0x%04X),IX","INC IX",
	"INC IXh","DEC IXh","LD IXh,0x%02X",0,
	0,"ADD IX,HL","LD IX,(0x%04X)","DEC IX",
	"INC IXl","DEC IXl","LD IXl,0x%02X",0,
	0,0,0,0,
	"INC (IX+%02X)","DEC (IX+%02X)","LD (IX+%02X),0x%02X",0,
	0,"ADD IX,SP",0,0,
	0,0,0,0,
	0,0,0,0,
	"LD B,IXh","LD B,IXl","LD B,(IX+%02X)",0,
	0,0,0,0,
	"LD C,IXh","LD C,IXl","LD C,(IX+%02X)",0,
	0,0,0,0,
	"LD D,IXh","LD D,IXl","LD D,(IX+%02X)",0,
	0,0,0,0,
	"LD E,IXh","LD E,IXl","LD E,(IX+%02X)",0,
	"LD IXh,B","LD IXh,C","LD IXh,D","LD IXh,E",
	"LD IXh,IXh","LD IXh,IXl","LD H,(IX+%02X)","LD IXh,A",
	"LD IXl,B","LD IXl,C","LD IXl,D","LD IXl,E",
	"LD IXl,IXh","LD IXl,IXl","LD L,(IX+%02X)","LD IXl,A",
	"LD (IX+%02X),B","LD (IX+%02X),C","LD (IX+%02X),D","LD (IX+%02X),E",
	"LD (IX+%02X),H","LD (IX+%02X),L",0,"LD (IX+%02X),A",
	0,0,0,0,
	"LD A,IXh","LD A,IXl","LD A,(IX+%02X)",0,
	0,0,0,0,
	"ADD A,IXh","ADD A,IXl","ADD A,(IX+%02X)",0,
	0,0,0,0,
	"ADC A,IXh","ADC A,IXl","ADC A,(IX+%02X)",0,
	0,0,0,0,
	"SUB IXh","SUB IXl","SUB (IX+%02X)",0,
	0,0,0,0,
	"SBC A,IXh","SBC A,IXl","SBC A,(IX+%02X)",0,
	0,0,0,0,
	"AND IXh","AND IXl","AND (IX+%02X)",0,
	0,0,0,0,
	"XOR IXh","XOR IXl","XOR (IX+%02X)",0,
	0,0,0,0,
	"OR IXh","OR IXl","OR (IX+%02X)",0,
	0,0,0,0,
	"CP IXh","CP IXl","CP (IX+%02X)",0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,"POP IX",0,"EX (SP),IX",
	0,"PUSH IX",0,0,
	0,"JP (IX)",0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,"LD SP,IX",0,0,
	0,0,0,0
};


const char * TabInstrDDCB[ 256 ] =
{
	0,0,0,0,0,0,"RLC (IX+%02X)",0,
	0,0,0,0,0,0,"RRC (IX+%02X)",0,
	0,0,0,0,0,0,"RL (IX+%02X)",0,
	0,0,0,0,0,0,"RR (IX+%02X)",0,
	0,0,0,0,0,0,"SLA (IX+%02X)",0,
	0,0,0,0,0,0,"SRA (IX+%02X)",0,
	0,0,0,0,0,0,"SLL (IX+%02X)",0,
	0,0,0,0,0,0,"SRL (IX+%02X)",0,
	0,0,0,0,0,0,"BIT 0,(IX+%02X)",0,
	0,0,0,0,0,0,"BIT 1,(IX+%02X)",0,
	0,0,0,0,0,0,"BIT 2,(IX+%02X)",0,
	0,0,0,0,0,0,"BIT 3,(IX+%02X)",0,
	0,0,0,0,0,0,"BIT 4,(IX+%02X)",0,
	0,0,0,0,0,0,"BIT 5,(IX+%02X)",0,
	0,0,0,0,0,0,"BIT 6,(IX+%02X)",0,
	0,0,0,0,0,0,"BIT 7,(IX+%02X)",0,
	0,0,0,0,0,0,"RES 0,(IX+%02X)",0,
	0,0,0,0,0,0,"RES 1,(IX+%02X)",0,
	0,0,0,0,0,0,"RES 2,(IX+%02X)",0,
	0,0,0,0,0,0,"RES 3,(IX+%02X)",0,
	0,0,0,0,0,0,"RES 4,(IX+%02X)",0,
	0,0,0,0,0,0,"RES 5,(IX+%02X)",0,
	0,0,0,0,0,0,"RES 6,(IX+%02X)",0,
	0,0,0,0,0,0,"RES 7,(IX+%02X)",0,
	0,0,0,0,0,0,"SET 0,(IX+%02X)",0,
	0,0,0,0,0,0,"SET 1,(IX+%02X)",0,
	0,0,0,0,0,0,"SET 2,(IX+%02X)",0,
	0,0,0,0,0,0,"SET 3,(IX+%02X)",0,
	0,0,0,0,0,0,"SET 4,(IX+%02X)",0,
	0,0,0,0,0,0,"SET 5,(IX+%02X)",0,
	0,0,0,0,0,0,"SET 6,(IX+%02X)",0,
	0,0,0,0,0,0,"SET 7,(IX+%02X)",0
};


const char * TabInstrFD[ 256 ] =
{
	0,0,0,0,0,0,0,0,
	0,"ADD IY,BC",0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,"ADD IY,DE",0,0,0,0,0,0,
	0,"LD IY,0x%04X","LD (0x%04X),IY","INC IY","INC IYh","DEC IYh","LD IYh,0x%02X",0,
	0,"ADD IY,HL","LD IY,(0x%04X)","DEC IY","INC IYl","DEC IYl","LD IYl,0x%02X",0,
	0,0,0,0,"INC (IY+%02X)","DEC (IY+%02X)","LD (IY+%02X),0x%02X",0,
	0,"ADD IY,SP",0,0,0,0,0,0,
	0,0,0,0,"LD B,IYh","LD B,IYl","LD B,(IY+%02X)",0,
	0,0,0,0,"LD C,IYh","LD C,IYl","LD C,(IY+%02X)",0,
	0,0,0,0,"LD D,IYh","LD D,IYl","LD D,(IY+%02X)",0,
	0,0,0,0,"LD E,IYh","LD E,IYl","LD E,(IY+%02X)",0,
	"LD IYh,B","LD IYh,C","LD IYh,D","LD IYh,E","LD IYh,IYh","LD IYh,IYl","LD H,(IY+%02X)","LD IYh,A",
	"LD IYl,B","LD IYl,C","LD IYl,D","LD IYl,E","LD IYl,IYh","LD IYl,IYl","LD L,(IY+%02X)","LD IYl,A",
	"LD (IY+%02X),B","LD (IY+%02X),C","LD (IY+%02X),D","LD (IY+%02X),E","LD (IY+%02X),H","LD (IY+%02X),L",0,"LD (IY+%02X),A",
	0,0,0,0,"LD A,IYh","LD A,IYl","LD A,(IY+%02X)",0,
	0,0,0,0,"ADD A,IYh","ADD A,IYl","ADD A,(IY+%02X)",0,
	0,0,0,0,"ADC A,IYh","ADC A,IYl","ADC A,(IY+%02X)",0,
	0,0,0,0,"SUB IYh","SUB IYl","SUB (IY+%02X)",0,
	0,0,0,0,"SBC A,IYh","SBC A,IYl","SBC A,(IY+%02X)",0,
	0,0,0,0,"AND IYh","AND IYl","AND (IY+%02X)",0,
	0,0,0,0,"XOR IYh","XOR IYl","XOR (IY+%02X)",0,
	0,0,0,0,"OR IYh","OR IYl","OR (IY+%02X)",0,
	0,0,0,0,"CP IYh","CP IYl","CP (IY+%02X)",0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,"POP IY",0,"EX (SP),IY",0,"PUSH IY",0,0,
	0,"JP (IY)",0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,"LD SP,IY",0,0,0,0,0,0
};


const char * TabInstrFDCB[ 256 ] =
{
	0,0,0,0,0,0,"RLC (IY+%02X)",0,
	0,0,0,0,0,0,"RRC (IY+%02X)",0,
	0,0,0,0,0,0,"RL (IY+%02X)",0,
	0,0,0,0,0,0,"RR (IY+%02X)",0,
	0,0,0,0,0,0,"SLA (IY+%02X)",0,
	0,0,0,0,0,0,"SRA (IY+%02X)",0,
	0,0,0,0,0,0,"SLL (IY+%02X)",0,
	0,0,0,0,0,0,"SRL (IY+%02X)",0,
	0,0,0,0,0,0,"BIT 0,(IY+%02X)",0,
	0,0,0,0,0,0,"BIT 1,(IY+%02X)",0,
	0,0,0,0,0,0,"BIT 2,(IY+%02X)",0,
	0,0,0,0,0,0,"BIT 3,(IY+%02X)",0,
	0,0,0,0,0,0,"BIT 4,(IY+%02X)",0,
	0,0,0,0,0,0,"BIT 5,(IY+%02X)",0,
	0,0,0,0,0,0,"BIT 6,(IY+%02X)",0,
	0,0,0,0,0,0,"BIT 7,(IY+%02X)",0,
	0,0,0,0,0,0,"RES 0,(IY+%02X)",0,
	0,0,0,0,0,0,"RES 1,(IY+%02X)",0,
	0,0,0,0,0,0,"RES 2,(IY+%02X)",0,
	0,0,0,0,0,0,"RES 3,(IY+%02X)",0,
	0,0,0,0,0,0,"RES 4,(IY+%02X)",0,
	0,0,0,0,0,0,"RES 5,(IY+%02X)",0,
	0,0,0,0,0,0,"RES 6,(IY+%02X)",0,
	0,0,0,0,0,0,"RES 7,(IY+%02X)",0,
	0,0,0,0,0,0,"SET 0,(IY+%02X)",0,
	0,0,0,0,0,0,"SET 1,(IY+%02X)",0,
	0,0,0,0,0,0,"SET 2,(IY+%02X)",0,
	0,0,0,0,0,0,"SET 3,(IY+%02X)",0,
	0,0,0,0,0,0,"SET 4,(IY+%02X)",0,
	0,0,0,0,0,0,"SET 5,(IY+%02X)",0,
	0,0,0,0,0,0,"SET 6,(IY+%02X)",0,
	0,0,0,0,0,0,"SET 7,(IY+%02X)",0
};


const char * TabInstr[ 256 ] =
{
	"NOP","LD BC,0x%04X","LD (BC),A","INC BC", // 0
	"INC B","DEC B","LD B,0x%02X","RLCA", // 4
	"EX AF,AF","ADD HL,BC","LD A,(BC)","DEC BC",
	"INC C","DEC C","LD C,0x%02X","RRCA",
	"DJNZ 0xeeee ","LD DE,0x%04X","LD (DE),A","INC DE", // 10
	"INC D","DEC D","LD D,0x%02X","RLA",
	"JR 0xeeee ","ADD HL,DE","LD A,(DE)","DEC DE",
	"INC E","DEC E","LD E,0x%02X","RRA",
	"JR NZ,0xeeee ","LD HL,0x%04X","LD (0x%04X),HL","INC HL", // 20
	"INC H","DEC H","LD H,0x%02X","DAA",
	"JR Z,0xeeee ","ADD HL,HL","LD HL,(0x%04X)","DEC HL",
	"INC L","DEC L","LD L,0x%02X","CPL",
	"JR NC,0xeeee ","LD SP,0x%04X","LD (0x%04X),A","INC SP", // 30
	"INC (HL)","DEC (HL)","LD (HL),0x%02X","SCF",
	"JR C,0xeeee ","ADD HL,SP","LD A,(0x%04X)","DEC SP",
	"INC A","DEC A","LD A,0x%02X","CCF",
	"LD B,B","LD B,C","LD B,D","LD B,E", // 40
	"LD B,H","LD B,L","LD B,(HL)","LD B,A",
	"LD C,B","LD C,C","LD C,D","LD C,E",
	"LD C,H","LD C,L","LD C,(HL)","LD C,A",
	"LD D,B","LD D,C","LD D,D","LD D,E", // 50
	"LD D,H","LD D,L","LD D,(HL)","LD D,A",
	"LD E,B","LD E,C","LD E,D","LD E,E",
	"LD E,H","LD E,L","LD E,(HL)","LD E,A",
	"LD H,B","LD H,C","LD H,D","LD H,E", // 60
	"LD H,H","LD H,L","LD H,(HL)","LD H,A",
	"LD L,B","LD L,C","LD L,D","LD L,E",
	"LD L,H","LD L,L","LD L,(HL)","LD L,A",
	"LD (HL),B","LD (HL),C","LD (HL),D","LD (HL),E", // 70
	"LD (HL),H","LD (HL),L","HALT","LD (HL),A",
	"LD A,B","LD A,C","LD A,D","LD A,E",
	"LD A,H","LD A,L","LD A,(HL)","LD A,A",
	"ADD A,B","ADD A,C","ADD A,D","ADD A,E", // 80
	"ADD A,H","ADD A,L","ADD A,(HL)","ADD A,A",
	"ADC A,B","ADC A,C","ADC A,D","ADC A,E",
	"ADC A,H","ADC A,L","ADC A,(HL)","ADC A,A",
	"SUB B","SUB C","SUB D","SUB E", // 90
	"SUB H","SUB L","SUB (HL)","SUB A",
	"SBC A,B","SBC A,C","SBC A,D","SBC A,E",
	"SBC A,H","SBC A,L","SBC A,(HL)","SBC A,A",
	"AND B","AND C","AND D","AND E", // A0
	"AND H","AND L","AND (HL)","AND A",
	"XOR B","XOR C","XOR D","XOR E",
	"XOR H","XOR L","XOR (HL)","XOR A",
	"OR B","OR C","OR D","OR E", // B0
	"OR H","OR L","OR (HL)","OR A",
	"CP B","CP C","CP D","CP E",
	"CP H","CP L","CP (HL)","CP A",
	"RET NZ","POP BC","JP NZ,0x%04X","JP 0x%04X", // C0
	"CALL NZ,0x%04X","PUSH BC","ADD A,0x%02X","RST 00",
	"RET Z","RET","JP Z,0x%04X",0,
	"CALL Z,0x%04X","CALL 0x%04X","ADC A,0x%02X","RST 08",
	"RET NC","POP DE","JP NC,0x%04X","OUT (0x%02X),A", // D0
	"CALL NC,0x%04X","PUSH DE","SUB 0x%02X","RST 10",
	"RET C","EXX","JP C,0x%04X","IN A,(0x%02X)",
	"CALL C,0x%04X",0,"SBC A,0x%02X","RST 18",
	"RET PE","POP HL","JP PE,0x%04X","EX (SP),HL", // E0
	"CALL PE,0x%04X","PUSH HL","AND 0x%02X","RST 20",
	"RET PO","JP (HL)","JP PO,0x%04X","EX DE,HL",
	"CALL PO,0x%04X",0,"XOR 0x%02X","RST 28",
	"RET P","POP AF","JP P,0x%04X","DI",
	"CALL P,0x%04X","PUSH AF","OR 0x%02X","RST 30",
	"RET M","LD SP,HL","JP M,0x%04X","EI",
	"CALL M,0x%04X",0,"CP 0x%02X","RST 38"
};


//
// Convertir le buffer en listing désassemblé
// TODO Check size in order to avoid overflow
//
void Desass(t_Memory mem, std::ostream &Listing, int Start, int Longueur )
{
	int i, Instr, Inst2 = 0, Inst3 = 0, Inst4 = 0, Ad16;
	const char * Chaine;
	char *p;
	char Inst[ 1024 ];

	int Adr, OldAdr = 0;
	char Ad8;

	// setup formatting
	Listing.fill('0');
	Listing.setf(ios::hex);

	for ( Adr = Start; Adr < Start + Longueur; )
	{
		OldAdr = Adr;
		Instr = mem.Read(Adr++);

		// No prefix
		Chaine = TabInstr[ Instr ];

		if (Chaine)
		{
			
		}
		// CB-Prefix
		else if ( Instr == 0xCB )
		{
			Inst2 = mem.Read(Adr++);
			Chaine = TabInstrCB[ Inst2 ];
		}
		// DD-Prefix
		else if ( Instr == 0xDD )
		{
			Inst2 = mem.Read(Adr++);
			// DDCB-Prefix
			if ( Inst2 == 0xCB )
			{
				Inst3 = mem.Read(Adr++);
				Inst4 = mem.Read(Adr++);
				Chaine = TabInstrDDCB[ Inst4 ];
				strcpy( Inst, Chaine );
				p = strstr( Inst, "nn" );
				if ( p )
				{
					if ( Inst3 < 0x80 )
						Hex( p, Inst3, 2 );
					else
					{
						Hex( p, -Inst3, 2 );
						p[ -1 ] = '-';
					}
				}
				Chaine = Inst;
			}
			else
				Chaine = TabInstrDD[ Inst2 ];
		}
		// ED-Prefix
		else if ( Instr == 0xED )
		{
			Inst2 = mem.Read(Adr++);
			Chaine = TabInstrED[ Inst2 ];
		}
		// FD-Prefix
		else if ( Instr == 0xFD )
		{
			Inst2 = mem.Read(Adr++);
			// FDCB-Prefix
			if ( Inst2 == 0xCB )
			{
				Ad8 = mem.Read(Adr++);
				Inst3 = mem.Read(Adr++);
				Chaine = TabInstrFDCB[ Inst3 ];
				if ( Chaine )
				{
					strcpy( Inst, Chaine );
					Chaine = Inst;
					p = strstr( Inst, "%02X" );
					if ( p )
						sprintf(Inst, Inst, Ad8);
				}
			}
			else
				Chaine = TabInstrFD[ Inst2 ];
		}


		if ( Chaine )
		{
			if(Instr==6) std::cout << Chaine ;
			strcpy( Inst, Chaine );
			p = strstr( Inst, "%04X" );
			Ad16 = mem.Read(Adr++);
			Ad16 += mem.Read(Adr) << 8;
			Ad8 = ( char ) Ad16;
			if ( p )
			{
				sprintf(Inst, Chaine, Ad16);
				Adr++;
			}
			else
			{
				p = strstr( Inst, "%02X" );
				if ( p )
				{
					sprintf(Inst, Chaine, Ad8);
					p = strstr( Inst, "%02X" );
					if ( p )
					{
						sprintf(Inst, Chaine, Ad16 >> 8);
					}
				}
				else
				{
					p = strstr( Inst, "0xeeee " );
					if ( p )
					{
						sprintf(p, "0x%04X", Adr + Ad8);
					}
					else
						Adr--;
				}
			}
		}
		else
			sprintf( Inst, "%02X %02X %02X ????", Instr, Inst2, Inst3 );

		//Memory adress part
		Listing.width(4);
		Listing << std::hex << OldAdr ;
		Listing << "\t" ;
		//Memory content part
		// Hex dump
		for ( i = OldAdr; i < Adr; i++ )
		{
			int val = mem.Read(i);
			Listing.width(2);
			Listing << val ;
			Listing << ' ' ;
		}
		Listing << "\t" ;
		if (Adr - OldAdr < 3)
			Listing << "\t" ;

		// ASCII dump
		for ( i = OldAdr; i < Adr; i++ )
		{
			int val = mem.Read(i);
			// 127 is DEL. Not printable.
			if (val > 32 && val < 127)
			{
				Listing << (char) val ;
			}
			else
			{
				Listing << ".";
			}
		}

		Listing << "\t" ;

		//Instruction part
		Listing << Inst ;

		//End of line
		Listing << endl;
	}
}
