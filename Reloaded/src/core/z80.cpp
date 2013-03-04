/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2004 Ulrich Doewich

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/* Zilog Z80A Microprocessor emulation
   (c) Copyright 1997-2003 Ulrich Doewich

   code portions and ideas taken from the Z80 emulations by:
    Juergen Buchmueller (MAME Z80 core v3.3)
    Marat Fayzullin
    and my own Z80 x86 assembly code (Caprice32 up to version 2.00b2)

   Oct 03, 2000 - 18:56    all un-prefixed opcodes done
   Oct 07, 2000 - 11:04    all CB opcodes done
   Oct 07, 2000 - 15:06    all DD opcodes done
   Oct 07, 2000 - 15:23    all DD CB opcodes done
   Oct 09, 2000 - 12:41    all ED, FD, and FD CB opcodes done
   Oct 14, 2000 - 17:48    added interrupt processing to z80_getopcode
   Oct 22, 2000 - 19:18    removed R register update from DDCB and FDCB opcode handlers
   Oct 22, 2000 - 19:43    added break-point and trace capabilities
   Oct 24, 2000 - 17:57    changed math based opcodes to always work with unsigned parameters
   Oct 29, 2000 - 20:46    fixed 16 bit memory read/write opcodes (read both halves from the low byte!)
   Oct 29, 2000 - 20:51    fixed LD L,byte; RRC r (used wrong registers - forgot to change them after copy!)
   Nov 06, 2000 - 21:08    fixed a couple of IX/IY instructions (forgot to change a few I?h/I?l related opcodes!)
   Nov 06, 2000 - 21:20    fixed some DDCB/FDCB instructions (one too many M cycles for BIT (I?+o) & co.)
   Nov 07, 2000 - 18:58    complete overhaul of DDCB/FDCB instructions (offset byte handling was wrong!)
   Jan 24, 2001 - 18:26    fixed LD (IX/IY + o), L and LD (IX/IY + o), H (uses L and H, not I?l and I?h!)
   Feb 19, 2001 - 18:37    removed machine cycle specific code; added cycle count tables and 'wait state' routine
   Mar 05, 2001 - 22:58    reworked all cycle count tables - verfied with the real CPC & an oscilloscope
   Mar 29, 2001 - 19:10    fixed the timing problem (z80_wait_states was called after interrupts even if they were disabled!)
   Apr 03, 2001 - 18:25    incorporated the changes from the MAME Z80 core v3.1 to v3.2 update
   Apr 09, 2001 - 19:30    fixed the problem with some CPC programs crashing (offset for IX/IY instructions was unsigned!)
   Jul 31, 2001 - 23:34    put the 'old' NOP cycle timing table back in
   Nov 12, 2001 - 18:15    incorporated the changes from the MAME Z80 core v3.2 to v3.3 update
   Nov 14, 2002 - 21:39    changed the length of processing an interrupt in IM2 from 28T to 76T
   Feb 10, 2003 - 18:24    corrected the cycle count of CPI/CPIR & CPD/CPDR with the help of Richard's PlusTest
   Feb 12, 2003 - 17:29    added the wait state adjustment on interrupts for a specific number of instructions
                           (see Richard's document on andercheran for the complete list)
   Apr 07, 2003 - 19:10    added z80_mf2stop to emulate the NMI caused by the stop button of the MF2
   Apr 07, 2003 - 22:48    added code to z80_execute to monitor when the MF2 finishes and has to be made 'invisible'
   May 10, 2003 - 19:12    fixed the unofficial DDCB/FDCB RES/SET instructions: the unaltered value was
                           stored in the associated register; some minor code clean up
   May 15, 2003 - 23:19    "Thomas the Tank Engine", "N.E.X.O.R." and "Jocky Wilson's Darts Compendium" work now:
                           DI did not clear the z80.EI_issued counter
*/

#include <iostream>
#include "z80.h"
#include "cap32.h"
#include "tape.h"

#include "psg.h"
#include "fdc.h"
#include "gatearray.h"
#include "vdu.h"
#include "crtc.h"

#include "emulator.h"

#include "debug.h"

extern byte *pbTapeImage;
extern dword dwMF2Flags;
extern dword dwMF2ExitAddr;

#ifdef USE_DEBUGGER_Z80
extern FILE *pfoDebug;
extern dword dwDebugFlag;
unsigned int dbg_z80_lastPC, dbg_z80_diff;
#endif

enum opcodes {
	nop, ld_bc_word, ld_mbc_a, inc_bc, inc_b, dec_b, ld_b_byte, rlca,
		ex_af_af, add_hl_bc, ld_a_mbc, dec_bc, inc_c, dec_c, ld_c_byte, rrca,
		djnz, ld_de_word, ld_mde_a, inc_de, inc_d, dec_d, ld_d_byte, rla,
		jr, add_hl_de, ld_a_mde, dec_de, inc_e, dec_e, ld_e_byte, rra,
		jr_nz, ld_hl_word, ld_mword_hl, inc_hl, inc_h, dec_h, ld_h_byte, daa,
		jr_z, add_hl_hl, ld_hl_mword, dec_hl, inc_l, dec_l, ld_l_byte, cpl,
		jr_nc, ld_sp_word, ld_mword_a, inc_sp, inc_mhl, dec_mhl, ld_mhl_byte, scf,
		jr_c, add_hl_sp, ld_a_mword, dec_sp, inc_a, dec_a, ld_a_byte, ccf,
		ld_b_b, ld_b_c, ld_b_d, ld_b_e, ld_b_h, ld_b_l, ld_b_mhl, ld_b_a,
		ld_c_b, ld_c_c, ld_c_d, ld_c_e, ld_c_h, ld_c_l, ld_c_mhl, ld_c_a,
		ld_d_b, ld_d_c, ld_d_d, ld_d_e, ld_d_h, ld_d_l, ld_d_mhl, ld_d_a,
		ld_e_b, ld_e_c, ld_e_d, ld_e_e, ld_e_h, ld_e_l, ld_e_mhl, ld_e_a,
		ld_h_b, ld_h_c, ld_h_d, ld_h_e, ld_h_h, ld_h_l, ld_h_mhl, ld_h_a,
		ld_l_b, ld_l_c, ld_l_d, ld_l_e, ld_l_h, ld_l_l, ld_l_mhl, ld_l_a,
		ld_mhl_b, ld_mhl_c, ld_mhl_d, ld_mhl_e, ld_mhl_h, ld_mhl_l, halt, ld_mhl_a,
		ld_a_b, ld_a_c, ld_a_d, ld_a_e, ld_a_h, ld_a_l, ld_a_mhl, ld_a_a,
		add_b, add_c, add_d, add_e, add_h, add_l, add_mhl, add_a,
		adc_b, adc_c, adc_d, adc_e, adc_h, adc_l, adc_mhl, adc_a,
		sub_b, sub_c, sub_d, sub_e, sub_h, sub_l, sub_mhl, sub_a,
		sbc_b, sbc_c, sbc_d, sbc_e, sbc_h, sbc_l, sbc_mhl, sbc_a,
		and_b, and_c, and_d, and_e, and_h, and_l, and_mhl, and_a,
		xor_b, xor_c, xor_d, xor_e, xor_h, xor_l, xor_mhl, xor_a,
		or_b, or_c, or_d, or_e, or_h, or_l, or_mhl, or_a,
		cp_b, cp_c, cp_d, cp_e, cp_h, cp_l, cp_mhl, cp_a,
		ret_nz, pop_bc, jp_nz, jp, call_nz, push_bc, add_byte, rst00,
		ret_z, ret, jp_z, pfx_cb, call_z, call, adc_byte, rst08,
		ret_nc, pop_de, jp_nc, outa, call_nc, push_de, sub_byte, rst10,
		ret_c, exx, jp_c, ina, call_c, pfx_dd, sbc_byte, rst18,
		ret_po, pop_hl, jp_po, ex_msp_hl, call_po, push_hl, and_byte, rst20,
		ret_pe, ld_pc_hl, jp_pe, ex_de_hl, call_pe, pfx_ed, xor_byte, rst28,
		ret_p, pop_af, jp_p, di, call_p, push_af, or_byte, rst30,
		ret_m, ld_sp_hl, jp_m, ei, call_m, pfx_fd, cp_byte, rst38
};

enum CBcodes {
	rlc_b, rlc_c, rlc_d, rlc_e, rlc_h, rlc_l, rlc_mhl, rlc_a,
		rrc_b, rrc_c, rrc_d, rrc_e, rrc_h, rrc_l, rrc_mhl, rrc_a,
		rl_b, rl_c, rl_d, rl_e, rl_h, rl_l, rl_mhl, rl_a,
		rr_b, rr_c, rr_d, rr_e, rr_h, rr_l, rr_mhl, rr_a,
		sla_b, sla_c, sla_d, sla_e, sla_h, sla_l, sla_mhl, sla_a,
		sra_b, sra_c, sra_d, sra_e, sra_h, sra_l, sra_mhl, sra_a,
		sll_b, sll_c, sll_d, sll_e, sll_h, sll_l, sll_mhl, sll_a,
		srl_b, srl_c, srl_d, srl_e, srl_h, srl_l, srl_mhl, srl_a,
		bit0_b, bit0_c, bit0_d, bit0_e, bit0_h, bit0_l, bit0_mhl, bit0_a,
		bit1_b, bit1_c, bit1_d, bit1_e, bit1_h, bit1_l, bit1_mhl, bit1_a,
		bit2_b, bit2_c, bit2_d, bit2_e, bit2_h, bit2_l, bit2_mhl, bit2_a,
		bit3_b, bit3_c, bit3_d, bit3_e, bit3_h, bit3_l, bit3_mhl, bit3_a,
		bit4_b, bit4_c, bit4_d, bit4_e, bit4_h, bit4_l, bit4_mhl, bit4_a,
		bit5_b, bit5_c, bit5_d, bit5_e, bit5_h, bit5_l, bit5_mhl, bit5_a,
		bit6_b, bit6_c, bit6_d, bit6_e, bit6_h, bit6_l, bit6_mhl, bit6_a,
		bit7_b, bit7_c, bit7_d, bit7_e, bit7_h, bit7_l, bit7_mhl, bit7_a,
		res0_b, res0_c, res0_d, res0_e, res0_h, res0_l, res0_mhl, res0_a,
		res1_b, res1_c, res1_d, res1_e, res1_h, res1_l, res1_mhl, res1_a,
		res2_b, res2_c, res2_d, res2_e, res2_h, res2_l, res2_mhl, res2_a,
		res3_b, res3_c, res3_d, res3_e, res3_h, res3_l, res3_mhl, res3_a,
		res4_b, res4_c, res4_d, res4_e, res4_h, res4_l, res4_mhl, res4_a,
		res5_b, res5_c, res5_d, res5_e, res5_h, res5_l, res5_mhl, res5_a,
		res6_b, res6_c, res6_d, res6_e, res6_h, res6_l, res6_mhl, res6_a,
		res7_b, res7_c, res7_d, res7_e, res7_h, res7_l, res7_mhl, res7_a,
		set0_b, set0_c, set0_d, set0_e, set0_h, set0_l, set0_mhl, set0_a,
		set1_b, set1_c, set1_d, set1_e, set1_h, set1_l, set1_mhl, set1_a,
		set2_b, set2_c, set2_d, set2_e, set2_h, set2_l, set2_mhl, set2_a,
		set3_b, set3_c, set3_d, set3_e, set3_h, set3_l, set3_mhl, set3_a,
		set4_b, set4_c, set4_d, set4_e, set4_h, set4_l, set4_mhl, set4_a,
		set5_b, set5_c, set5_d, set5_e, set5_h, set5_l, set5_mhl, set5_a,
		set6_b, set6_c, set6_d, set6_e, set6_h, set6_l, set6_mhl, set6_a,
		set7_b, set7_c, set7_d, set7_e, set7_h, set7_l, set7_mhl, set7_a
};

enum EDcodes {
	ed_00, ed_01, ed_02, ed_03, ed_04, ed_05, ed_06, ed_07,
		ed_08, ed_09, ed_0a, ed_0b, ed_0c, ed_0d, ed_0e, ed_0f,
		ed_10, ed_11, ed_12, ed_13, ed_14, ed_15, ed_16, ed_17,
		ed_18, ed_19, ed_1a, ed_1b, ed_1c, ed_1d, ed_1e, ed_1f,
		ed_20, ed_21, ed_22, ed_23, ed_24, ed_25, ed_26, ed_27,
		ed_28, ed_29, ed_2a, ed_2b, ed_2c, ed_2d, ed_2e, ed_2f,
		ed_30, ed_31, ed_32, ed_33, ed_34, ed_35, ed_36, ed_37,
		ed_38, ed_39, ed_3a, ed_3b, ed_3c, ed_3d, ed_3e, ed_3f,
		in_b_c, out_c_b, sbc_hl_bc, ld_EDmword_bc, neg, retn, im_0, ld_i_a,
		in_c_c, out_c_c, adc_hl_bc, ld_EDbc_mword, neg_1, reti, im_0_1, ld_r_a,
		in_d_c, out_c_d, sbc_hl_de, ld_EDmword_de, neg_2, retn_1, im_1, ld_a_i,
		in_e_c, out_c_e, adc_hl_de, ld_EDde_mword, neg_3, reti_1, im_2, ld_a_r,
		in_h_c, out_c_h, sbc_hl_hl, ld_EDmword_hl, neg_4, retn_2, im_0_2, rrd,
		in_l_c, out_c_l, adc_hl_hl, ld_EDhl_mword, neg_5, reti_2, im_0_3, rld,
		in_0_c, out_c_0, sbc_hl_sp, ld_EDmword_sp, neg_6, retn_3, im_1_1, ed_77,
		in_a_c, out_c_a, adc_hl_sp, ld_EDsp_mword, neg_7, reti_3, im_2_1, ed_7f,
		ed_80, ed_81, ed_82, ed_83, ed_84, ed_85, ed_86, ed_87,
		ed_88, ed_89, ed_8a, ed_8b, ed_8c, ed_8d, ed_8e, ed_8f,
		ed_90, ed_91, ed_92, ed_93, ed_94, ed_95, ed_96, ed_97,
		ed_98, ed_99, ed_9a, ed_9b, ed_9c, ed_9d, ed_9e, ed_9f,
		ldi, cpi, ini, outi, ed_a4, ed_a5, ed_a6, ed_a7,
		ldd, cpd, ind, outd, ed_ac, ed_ad, ed_ae, ed_af,
		ldir, cpir, inir, otir, ed_b4, ed_b5, ed_b6, ed_b7,
		lddr, cpdr, indr, otdr, ed_bc, ed_bd, ed_be, ed_bf,
		ed_c0, ed_c1, ed_c2, ed_c3, ed_c4, ed_c5, ed_c6, ed_c7,
		ed_c8, ed_c9, ed_ca, ed_cb, ed_cc, ed_cd, ed_ce, ed_cf,
		ed_d0, ed_d1, ed_d2, ed_d3, ed_d4, ed_d5, ed_d6, ed_d7,
		ed_d8, ed_d9, ed_da, ed_db, ed_dc, ed_dd, ed_de, ed_df,
		ed_e0, ed_e1, ed_e2, ed_e3, ed_e4, ed_e5, ed_e6, ed_e7,
		ed_e8, ed_e9, ed_ea, ed_eb, ed_ec, ed_ed, ed_ee, ed_ef,
		ed_f0, ed_f1, ed_f2, ed_f3, ed_f4, ed_f5, ed_f6, ed_f7,
		ed_f8, ed_f9, ed_fa, ed_fb, ed_fc, ed_fd, ed_fe, ed_ff
};


byte t_z80regs::SZ[256]; // zero and sign flags
byte t_z80regs::SZ_BIT[256]; // zero, sign and parity/overflow (=zero) flags for BIT opcode
byte t_z80regs::SZP[256]; // zero, sign and parity flags
byte t_z80regs::SZHV_inc[256]; // zero, sign, half carry and overflow flags INC r8
byte t_z80regs::SZHV_dec[256]; // zero, sign, half carry and overflow flags DEC r8

#include "z80daa.h"

/*
static const byte irep_tmp1[4][4] = {
	{0, 0, 1, 0}, 
	{0, 1, 0, 1}, 
	{1, 0, 1, 1}, 
	{0, 1, 1, 0}
};
*/

/* tmp1 value for ind/indr/outd/otdr for [C.1-0][io.1-0] */
/*
static const byte drep_tmp1[4][4] = {
	{0, 1, 0, 0}, 
	{1, 0, 0, 1}, 
	{0, 0, 1, 0}, 
	{0, 1, 0, 1}
};*/

/* tmp2 value for all in/out repeated opcodes for B.7-0 */
/*
static const byte breg_tmp2[256] = {
	0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
		1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
		1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
		1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
		1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
		1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
		1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1,
		0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
		1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0,
		1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1
};*/

#define Oa 8
#define Oa_ 4
#define Ia 12
#define Ia_ 0

static const byte cc_op[256] = {
    4, 12,  8,  8,  4,  4,  8,  4,  4, 12,  8,  8,  4,  4,  8,  4,
		12, 12,  8,  8,  4,  4,  8,  4, 12, 12,  8,  8,  4,  4,  8,  4,
		8, 12, 20,  8,  4,  4,  8,  4,  8, 12, 20,  8,  4,  4,  8,  4,
		8, 12, 16,  8, 12, 12, 12,  4,  8, 12, 16,  8,  4,  4,  8,  4,
		4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
		4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
		4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
		8,  8,  8,  8,  8,  8,  4,  8,  4,  4,  4,  4,  4,  4,  8,  4,
		4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
		4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
		4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
		4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
		8, 12, 12, 12, 12, 16,  8, 16,  8, 12, 12,  4, 12, 20,  8, 16,
		8, 12, 12, Oa, 12, 16,  8, 16,  8,  4, 12, Ia, 12,  4,  8, 16,
		8, 12, 12, 24, 12, 16,  8, 16,  8,  4, 12,  4, 12,  4,  8, 16,
		8, 12, 12,  4, 12, 16,  8, 16,  8,  8, 12,  4, 12,  4,  8, 16
};

static const byte cc_cb[256] = {
    4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
		4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
		4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
		4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
		4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
		4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
		4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
		4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
		4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
		4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
		4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
		4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
		4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
		4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
		4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4,
		4,  4,  4,  4,  4,  4, 12,  4,  4,  4,  4,  4,  4,  4, 12,  4
};

#define Ox 8
#define Ox_ 4
#define Oy 12
#define Oy_ 4
#define Ix 12
#define Ix_ 0
#define Iy 16
#define Iy_ 0

static const byte cc_ed[256] = {
    4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		Ix, Ox, 12, 20,  4, 12,  4,  8, Ix, Ox, 12, 20,  4, 12,  4,  8,
		Ix, Ox, 12, 20,  4, 12,  4,  8, Ix, Ox, 12, 20,  4, 12,  4,  8,
		Ix, Ox, 12, 20,  4, 12,  4, 16, Ix, Ox, 12, 20,  4, 12,  4, 16,
		Ix, Ox, 12, 20,  4, 12,  4,  4, Ix, Ox, 12, 20,  4, 12,  4,  4,
		4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		16, 12, Iy, Oy,  4,  4,  4,  4, 16, 12, Iy, Oy,  4,  4,  4,  4,
		16, 12, Iy, Oy,  4,  4,  4,  4, 16, 12, Iy, Oy,  4,  4,  4,  4,
		4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4
};

static const byte cc_xy[256] = {
    4, 12,  8,  8,  4,  4,  8,  4,  4, 12,  8,  8,  4,  4,  8,  4,
		12, 12,  8,  8,  4,  4,  8,  4, 12, 12,  8,  8,  4,  4,  8,  4,
		8, 12, 20,  8,  4,  4,  8,  4,  8, 12, 20,  8,  4,  4,  8,  4,
		8, 12, 16,  8, 20, 20, 20,  4,  8, 12, 16,  8,  4,  4,  8,  4,
		4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
		4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
		4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
		16, 16, 16, 16, 16, 16,  4, 16,  4,  4,  4,  4,  4,  4, 16,  4,
		4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
		4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
		4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
		4,  4,  4,  4,  4,  4, 16,  4,  4,  4,  4,  4,  4,  4, 16,  4,
		8, 12, 12, 12, 12, 16,  8, 16,  8, 12, 12,  4, 12, 20,  8, 16,
		8, 12, 12, Oa, 12, 16,  8, 16,  8,  4, 12, Ia, 12,  4,  8, 16,
		8, 12, 12, 24, 12, 16,  8, 16,  8,  4, 12,  4, 12,  4,  8, 16,
		8, 12, 12,  4, 12, 16,  8, 16,  8,  8, 12,  4, 12,  4,  8, 16
};

static const byte cc_xycb[256] = {
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
		16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
		16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
		16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20
};

static const byte cc_ex[256] = {
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		4,  0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,
		4,  0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		4,  8,  4,  4,  0,  0,  0,  0,  4,  8,  4,  4,  0,  0,  0,  0,
		8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,
		8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,
		8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,
		8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0,  8,  0,  0,  0
};

t_z80regs::t_z80regs(shared_ptr<Emulator> emulator) :
emulator(emulator),
_ioPort(emulator),
VDU(emulator->GetVDU()),
PSG(emulator->GetPSG()),
Memory(emulator->GetMemory()),
CPC(emulator->GetConfig())
{
}

void t_z80regs::reset()
{
	// clear all Z80 registers and support variables
	_rAFdword = 0x00000000;
	// set zero flag
	_rF = Zflag;

	_rBCdword = 0x00000000;
	_rDEdword = 0x00000000;
	_rHLdword = 0x00000000;
	_rPCdword = 0x00000000;
	_rSP = 0x0000;
	AFx.w.l = 0x0000;
	BCx.w.l = 0x0000;
	DEx.w.l = 0x0000;
	HLx.w.l = 0x0000;
	// IX and IY are FFFF after a reset!
	_rIXdword = 0xffffffff;
	_rIYdword = 0xffffffff;

	I = 0x00;
	R = 0x00;
	Rb7 = 0x00;
	IFF1 = 0x00;
	IFF2 = 0x00;
	IM = 0x00;
	HALT = 0x00;
	EI_issued = 0x00;
	int_pending = 0x00;

	break_points.clear();
	adressAlreadyBlocked = false;

	trace = 0x00000000;
}

#define z80_wait_states														\
{																			\
	if (iCycleCount)														\
	{																		\
		auto emu = emulator.lock();											\
		auto CRTC = emu->GetCRTC();											\
		auto FDC = emu->GetFDC();											\
		CRTC->Emulate(iCycleCount >> 2);									\
		if (CPC->snd_enabled)												\
		{																	\
			auto PSG = emu->GetPSG();										\
			PSG->Emulate(iCycleCount);										\
		}																	\
																			\
		FDC->Emulate(iCycleCount);											\
																			\
		if ((CPC->tape_motor) && (CPC->tape_play_button))					\
		{																	\
			auto Tape = emu->GetTape();										\
			Tape->Emulate(iCycleCount);										\
		}																	\
																			\
		CPC->cycle_count -= iCycleCount;									\
	}																		\
}

#define ADC(value) \
{ \
	unsigned val = value; \
	unsigned res = _rA + val + (_rF & Cflag); \
	_rF = SZ[res & 0xff] | ((res >> 8) & Cflag) | ((_rA ^ res ^ val) & Hflag) | \
	(((val ^ _rA ^ 0x80) & (val ^ res) & 0x80) >> 5); \
	_rA = res; \
}

#define ADD(value) \
{ \
	unsigned val = value; \
	unsigned res = _rA + val; \
	_rF = SZ[(byte)res] | ((res >> 8) & Cflag) | ((_rA ^ res ^ val) & Hflag) | \
	(((val ^ _rA ^ 0x80) & (val ^ res) & 0x80) >> 5); \
	_rA = (byte)res; \
}

#define ADD16(dest, src) \
{ \
	dword res = dest.w.l + src.w.l; \
	WZ.d = dest.d+1; \
	_rF = (_rF & (Sflag | Zflag | Vflag)) | (((dest.d ^ res ^ src.d) >> 8) & Hflag) | \
	((res >> 16) & Cflag) | ((res >> 8) & Xflags); \
	dest.w.l = (word)res; \
}

#define AND(val) \
{ \
	_rA &= val; \
	_rF = SZP[_rA] | Hflag; \
}

#define CALL \
{ \
	write_mem(--_rSP, PC.b.h); /* store high byte of current PC */ \
	write_mem(--_rSP, PC.b.l); /* store low byte of current PC */ \
	_rPC = WZ.w.l; /* continue execution at subroutine (address was loaded by GET_ADDR */ \
}

#define CP(value) \
{ \
	unsigned val = value; \
	unsigned res = _rA - val; \
	_rF = (SZ[res & 0xff] & (Sflag | Zflag)) | (val & Xflags) | ((res >> 8) & Cflag) | Nflag | ((_rA ^ res ^ val) & Hflag) | \
	((((val ^ _rA) & (_rA ^ res)) >> 5) & Vflag); \
}

#define DAA \
{ \
	int idx = _rA; \
	if(_rF & Cflag) \
	idx |= 0x100; \
	if(_rF & Hflag) \
	idx |= 0x200; \
	if(_rF & Nflag) \
	idx |= 0x400; \
	_rAF = DAATable[idx]; \
}

#define DEC(reg) \
{ \
	reg--; \
	_rF = (_rF & Cflag) | SZHV_dec[reg]; \
}

#define JR \
{ \
	signed char offset; \
	offset = (signed char)(read_mem(_rPC)); /* grab signed jump offset */ \
	_rPC += offset + 1; /* add offset & correct PC */ \
	WZ.w.l = _rPC; \
}

#define EXX \
{ \
	reg_pair temp; \
	temp = BCx; \
	BCx = BC; \
	BC = temp; \
	temp = DEx; \
	DEx = DE; \
	DE = temp; \
	temp = HLx; \
	HLx = HL; \
	HL = temp; \
}

#define EX(op1, op2) \
{ \
	reg_pair temp; \
	temp = op1; \
	op1 = op2; \
	op2 = temp; \
}

#define EX_SP(reg) \
{ \
	WZ.b.l = read_mem(_rSP++); \
	WZ.b.h = read_mem(_rSP); \
	write_mem(_rSP--, reg.b.h); \
	write_mem(_rSP, reg.b.l); \
	reg.w.l = WZ.w.l; \
}

#define INC(reg) \
{ \
	reg++; \
	_rF = (_rF & Cflag) | SZHV_inc[reg]; \
}

#define GET_ADDR \
{ \
	WZ.b.l = read_mem(_rPC++); \
	WZ.b.h = read_mem(_rPC++); \
}

#define JP \
{ \
	_rPC = WZ.w.l; \
}

#define LD16_MEM(reg) \
{ \
	WZ.b.l = read_mem(_rPC++); \
	WZ.b.h = read_mem(_rPC++); \
	reg.b.l = read_mem(WZ.w.l); \
	reg.b.h = read_mem(++WZ.w.l); \
}

#define LDMEM_16(reg) \
{ \
	WZ.b.l = read_mem(_rPC++); \
	WZ.b.h = read_mem(_rPC++); \
	write_mem(WZ.w.l, reg.b.l); \
	write_mem(++WZ.w.l, reg.b.h); \
}

#define OR(val) \
{ \
	_rA |= val; \
	_rF = SZP[_rA]; \
}

#define POP(reg) \
{ \
	reg.b.l = read_mem(_rSP++); \
	reg.b.h = read_mem(_rSP++); \
}

#define PUSH(reg) \
{ \
	write_mem(--_rSP, reg.b.h); \
	write_mem(--_rSP, reg.b.l); \
}

#define RET \
{ \
	PC.b.l = read_mem(_rSP++); \
	PC.b.h = read_mem(_rSP++); \
	WZ.w.l = _rPC;\
}

#define RLA \
{ \
	byte res = (_rA << 1) | (_rF & Cflag); \
	byte carry = (_rA & 0x80) ? Cflag : 0; \
	_rF = (_rF & (Sflag | Zflag | Pflag)) | carry | (res & Xflags); \
	_rA = res; \
}

#define RLCA \
{ \
	_rA = (_rA << 1) | (_rA >> 7); \
	_rF = (_rF & (Sflag | Zflag | Pflag)) | (_rA & (Xflags | Cflag)); \
}

#define RRA \
{ \
	byte res = (_rA >> 1) | (_rF << 7); \
	byte carry = (_rA & 0x01) ? Cflag : 0; \
	_rF = (_rF & (Sflag | Zflag | Pflag)) | carry | (res & Xflags); \
	_rA = res; \
}

#define RRCA \
{ \
	_rF = (_rF & (Sflag | Zflag | Pflag)) | (_rA & Cflag); \
	_rA = (_rA >> 1) | (_rA << 7); \
	_rF |= (_rA & Xflags); \
}

#define RST(addr) \
{ \
	write_mem(--_rSP, PC.b.h); /* store high byte of current PC */ \
	write_mem(--_rSP, PC.b.l); /* store low byte of current PC */ \
	_rPC = addr; /* continue execution at restart address */ \
	WZ.w.l = addr; \
}

#define SBC(value) \
{ \
	unsigned val = value; \
	unsigned res = _rA - val - (_rF & Cflag); \
	_rF = SZ[res & 0xff] | ((res >> 8) & Cflag) | Nflag | ((_rA ^ res ^ val) & Hflag) | \
	(((val ^ _rA) & (_rA ^ res) & 0x80) >> 5); \
	_rA = res; \
}

#define SUB(value) \
{ \
	unsigned val = value; \
	unsigned res = _rA - val; \
	_rF = SZ[res & 0xff] | ((res >> 8) & Cflag) | Nflag | ((_rA ^ res ^ val) & Hflag) | \
	(((val ^ _rA) & (_rA ^ res) & 0x80) >> 5); \
	_rA = res; \
}

#define XOR(val) \
{ \
	_rA ^= val; \
	_rF = SZP[_rA]; \
}

#define BIT(bit, reg) \
_rF = (_rF & Cflag) | Hflag | (SZ_BIT[reg & (1 << bit)] & ~Xflags) | (reg & Xflags) 

#define BIT_HL(bit, reg) \
_rF = (_rF & Cflag) | Hflag | (SZ_BIT[reg & (1 << bit)] & ~Xflags) | (WZ.b.h & Xflags) 

#define BIT_XY BIT

#define ADC16(reg) \
{ \
	WZ.d = _rHL+1; \
	dword res = _rHLdword + reg.d + (_rF & Cflag); \
	_rF = (((_rHLdword ^ res ^ reg.d) >> 8) & Hflag) | \
	((res >> 16) & Cflag) | \
	((res >> 8) & (Sflag | Xflags)) | \
	((res & 0xffff) ? 0 : Zflag) | \
	(((reg.d ^ _rHLdword ^ 0x8000) & (reg.d ^ res) & 0x8000) >> 13); \
	_rHL = (word)res; \
}

#define CPD \
{ \
	byte val = read_mem(_rHL); \
	byte res = _rA - val; \
	_rHL--; \
	_rBC--; \
	_rF = (_rF & Cflag) | (SZ[res] & ~Xflags) | ((_rA ^ val ^ res) & Hflag) | Nflag; \
	if(_rF & Hflag) res -= 1; \
	if(res & 0x02) _rF |= 0x20; \
	if(res & 0x08) _rF |= 0x08; \
	if(_rBC) _rF |= Vflag; \
	WZ.w.l--;\
}

#define CPDR \
	CPD; \
	if(_rBC && !(_rF & Zflag)) \
{ \
	iCycleCount += cc_ex[bOpCode]; \
	_rPC -= 2; \
	iWSAdjust++; \
	WZ.w.l = _rPC+1;\
}

#define CPI \
{ \
	byte val = read_mem(_rHL); \
	byte res = _rA - val; \
	_rHL++; \
	_rBC--; \
	_rF = (_rF & Cflag) | (SZ[res] & ~Xflags) | ((_rA ^ val ^ res) & Hflag) | Nflag; \
	if(_rF & Hflag) res -= 1; \
	if(res & 0x02) _rF |= 0x20; \
	if(res & 0x08) _rF |= 0x08; \
	if(_rBC) _rF |= Vflag; \
	WZ.w.l++;\
}

#define CPIR \
	CPI; \
	if(_rBC && !(_rF & Zflag)) \
{ \
	iCycleCount += cc_ex[bOpCode]; \
	_rPC -= 2; \
	iWSAdjust++; \
	WZ.w.l = _rPC+1;\
}

#define IND \
{ \
	byte io = z80_IN_handler(BC); \
	WZ.w.l = _rBC-1;\
	_rB--; \
	write_mem(_rHL, io); \
	_rHL--; \
	_rF = SZ[_rB]; \
	if(io & Sflag) _rF |= Nflag; \
	if((((_rC - 1) & 0xff) + io) & 0x100) _rF |= Hflag | Cflag; \
	/*if((drep_tmp1[_rC & 3][io & 3] ^ breg_tmp2[_rB] ^ (_rC >> 2) ^ (io >> 2)) & 1) \
	_rF |= Pflag;*/ \
	_rF |= SZP[((((_rC-1)&0xFF)+io) & 0x07) ^ _rB] & Pflag;				\
}

#define INDR \
	IND; \
	if(_rB) \
{ \
	iCycleCount += cc_ex[bOpCode]; \
	_rPC -= 2; \
}

#define INI \
{ \
	byte io = z80_IN_handler(BC); \
	WZ.w.l = _rBC+1;\
	_rB--; \
	write_mem(_rHL, io); \
	_rHL++; \
	_rF = SZ[_rB]; \
	if(io & Sflag) _rF |= Nflag; \
	if((((_rC + 1) & 0xff) + io) & 0x100) _rF |= Hflag | Cflag; \
	/*if((irep_tmp1[_rC & 3][io & 3] ^ breg_tmp2[_rB] ^ (_rC >> 2) ^ (io >> 2)) & 1) \
	_rF |= Pflag; */\
	_rF |= SZP[((((_rC+1)&0xff) +io) & 0x07) ^ _rB] & Pflag;				\
}

#define INIR \
	INI; \
	if(_rB) \
{ \
	iCycleCount += cc_ex[bOpCode]; \
	_rPC -= 2; \
}

#define LDD \
{ \
	byte io = read_mem(_rHL); \
	write_mem(_rDE, io); \
	_rF &= Sflag | Zflag | Cflag; \
	if((_rA + io) & 0x02) _rF |= 0x20; \
	if((_rA + io) & 0x08) _rF |= 0x08; \
	_rHL--; \
	_rDE--; \
	_rBC--; \
	if(_rBC) _rF |= Vflag; \
}

#define LDDR \
	LDD; \
	if(_rBC) \
   { \
   iCycleCount += cc_ex[bOpCode]; \
   _rPC -= 2; \
   WZ.w.l = _rPC+1; \
   }

#define LDI \
{ \
	byte io = read_mem(_rHL); \
	write_mem(_rDE, io); \
	_rF &= Sflag | Zflag | Cflag; \
	if((_rA + io) & 0x02) _rF |= 0x20; \
	if((_rA + io) & 0x08) _rF |= 0x08; \
	_rHL++; \
	_rDE++; \
	_rBC--; \
	if(_rBC) _rF |= Vflag; \
}

#define LDIR \
	LDI; \
	if(_rBC) \
   { \
   iCycleCount += cc_ex[bOpCode]; \
   _rPC -= 2; \
   WZ.w.l = _rPC+1; \
   }

#define NEG \
{ \
	byte value = _rA; \
	_rA = 0; \
	SUB(value); \
}

#define OUTD \
{ \
	byte io = read_mem(_rHL); \
	_rB--; \
	WZ.w.l = _rBC-1;\
	z80_OUT_handler(BC, io); \
	_rHL--; \
	_rF = SZ[_rB]; \
	if(io & Sflag) _rF |= Nflag; \
	if((_rL + io) & 0x100) _rF |= Hflag | Cflag; \
	/*if((drep_tmp1[_rC & 3][io & 3] ^ breg_tmp2[_rB] ^ (_rC >> 2) ^ (io >> 2)) & 1) */\
	/*_rF |= Pflag;*/ \
	_rF |= SZP[((_rL+io) & 0x07) ^ _rB] & Pflag; }

#define OTDR \
	OUTD; \
	if(_rB) \
   { \
   iCycleCount += cc_ex[bOpCode]; \
   _rPC -= 2; \
   }

#define OUTI \
{ \
	byte io = read_mem(_rHL); \
	_rB--; \
	WZ.w.l = _rBC+1;\
	z80_OUT_handler(BC, io); \
	_rHL++; \
	_rF = SZ[_rB]; \
	if(io & Sflag) _rF |= Nflag; \
	/* if((((_rC + 1) & 0xff) + io) & 0x100) _rF |= Hflag | Cflag; */\
	if ((_rL+io) & 0x100) _rF |= Hflag | Cflag;							\
	/*if((irep_tmp1[_rC & 3][io & 3] ^ breg_tmp2[_rB] ^ (_rC >> 2) ^ (io >> 2)) & 1) \
	_rF |= Pflag;*/ \
	_rF |= SZP[((_rL+io) & 0x07) ^ _rB] & Pflag;				\
}

#define OTIR \
	OUTI; \
	if(_rB) \
   { \
   iCycleCount += cc_ex[bOpCode]; \
   _rPC -= 2; \
   }

#define RLD \
{ \
	WZ.w.l = _rHL + 1; \
	byte n = read_mem(_rHL); \
	write_mem(_rHL, (n << 4) | (_rA & 0x0f)); \
	_rA = (_rA & 0xf0) | (n >> 4); \
	_rF = (_rF & Cflag) | SZP[_rA]; \
}

#define RRD \
{ \
	WZ.w.l = _rHL + 1; \
	byte n = read_mem(_rHL); \
	write_mem(_rHL, (n >> 4) | (_rA << 4)); \
	_rA = (_rA & 0xf0) | (n & 0x0f); \
	_rF = (_rF & Cflag) | SZP[_rA]; \
}

#define SBC16(reg) \
{ \
	WZ.d = _rHLdword+1;\
	dword res = _rHLdword - reg.d - (_rF & Cflag); \
	_rF = (((_rHLdword ^ res ^ reg.d) >> 8) & Hflag) | Nflag | \
	((res >> 16) & Cflag) | \
	((res >> 8) & (Sflag | Xflags)) | \
	((res & 0xffff) ? 0 : Zflag) | \
	(((reg.d ^ _rHLdword) & (_rHLdword ^ res) &0x8000) >> 13); \
	_rHL = (word)res; \
}



#define z80_int_handler \
{ \
	if (_rIFF1) { /* process interrupts? */ \
	_rR++; \
	_rIFF1 = _rIFF2 = 0; /* clear interrupt flip-flops */ \
	int_pending = 0; \
	emulator.lock()->GetGateArray()->SetSLCount(emulator.lock()->GetGateArray()->GetSLCount() & 0x1f); /* clear bit 5 of GA scanline counter */ \
	if (_rHALT) { /* HALT instruction active? */ \
	_rHALT = 0; /* exit HALT 'loop' */ \
	_rPC++; /* correct PC */ \
	} \
	if (_rIM < 2) { /* interrupt mode 0 or 1? (IM0 = IM1 on the CPC) */ \
	iCycleCount = 20; \
	if (iWSAdjust) { \
	iCycleCount -= 4; \
	} \
	RST(0x0038); \
	z80_wait_states \
	} \
	else { /* interrupt mode 2 */ \
	reg_pair addr; \
	iCycleCount = 28; /* was 76 */ \
	if (iWSAdjust) { \
	iCycleCount -= 4; \
	} \
	write_mem(--_rSP, PC.b.h); /* store high byte of current PC */ \
	write_mem(--_rSP, PC.b.l); /* store low byte of current PC */ \
	addr.b.l = 0xff; /* assemble pointer */ \
	addr.b.h = _rI; \
	PC.b.l = read_mem(addr.w.l); /* retrieve low byte of vector */ \
	PC.b.h = read_mem(addr.w.l+1); /* retrieve high byte of vector */ \
	WZ.w.l=PC.w.l;\
	z80_wait_states \
      } \
	} \
}



void t_z80regs::z80_init_tables(void)
{
	int i, p;

	for (i = 0; i < 256; i++) {
		p = 0;
		if(i & 0x01) ++p;
		if(i & 0x02) ++p;
		if(i & 0x04) ++p;
		if(i & 0x08) ++p;
		if(i & 0x10) ++p;
		if(i & 0x20) ++p;
		if(i & 0x40) ++p;
		if(i & 0x80) ++p;
		SZ[i] = i ? i & Sflag : Zflag;
		SZ[i] |= (i & Xflags);
		SZ_BIT[i] = i ? i & Sflag : Zflag | Pflag;
		SZ_BIT[i] |= (i & Xflags);
		SZP[i] = SZ[i] | ((p & 1) ? 0 : Pflag);
		SZHV_inc[i] = SZ[i];
		if(i == 0x80) SZHV_inc[i] |= Vflag;
		if((i & 0x0f) == 0x00) SZHV_inc[i] |= Hflag;
		SZHV_dec[i] = SZ[i] | Nflag;
		if(i == 0x7f) SZHV_dec[i] |= Vflag;
		if((i & 0x0f) == 0x0f) SZHV_dec[i] |= Hflag;
	}
}



void t_z80regs::z80_mf2stop(void)
{
	_rR++;
	_rIFF1 = 0;
	EI_issued = 0;
	iCycleCount = 20;
	if (iWSAdjust) {
		iCycleCount -= 4;
	}
	dwMF2ExitAddr = _rPCdword;
	RST(0x0066); // MF2 stop button causes a Z80 NMI
	z80_wait_states
		dwMF2Flags = MF2_ACTIVE | MF2_RUNNING;
}



int t_z80regs::z80_execute(void)
{
	byte bOpCode;

	while ( adressAlreadyBlocked ||
        break_points.end() == break_points.find(_rPCdword)           ) { // loop until break point

    adressAlreadyBlocked = false ;

		if (dwMF2Flags & MF2_RUNNING) {
			if (_rPCdword == dwMF2ExitAddr) { // have we returned from the MF2?
				dwMF2Flags = MF2_INVISIBLE; // clear running flag and make the MF2 'invisible'
			}
		}

		bOpCode = read_mem(_rPC++);

#ifdef USE_DEBUGGER_Z80
		if (dwDebugFlag)
		{
			dbg_z80_diff = (int)abs( (int) (dbg_z80_lastPC - _rPC));
			if (dbg_z80_diff > 0x100)
			{
				fprintf(pfoDebug, "Z80 PC : %04x\n", _rPC);
				fprintf(pfoDebug, "Oc: %04x\n", bOpCode);
			}
			else
			{
			       fprintf(pfoDebug, "%04x ", _rPC);
			       fprintf(pfoDebug, "Oc %04x ", bOpCode);
			}
			dbg_z80_lastPC = _rPC;
		}
#endif

		iCycleCount = cc_op[bOpCode];
		_rR++;
		switch(bOpCode)
		{
		case adc_a:       ADC(_rA); break;
		case adc_b:       ADC(_rB); break;
		case adc_byte:    ADC(read_mem(_rPC++)); break;
		case adc_c:       ADC(_rC); break;
		case adc_d:       ADC(_rD); break;
		case adc_e:       ADC(_rE); break;
		case adc_h:       ADC(_rH); break;
		case adc_l:       ADC(_rL); break;
		case adc_mhl:     ADC(read_mem(_rHL)); break;
		case add_a:       ADD(_rA); break;
		case add_b:       ADD(_rB); break;
		case add_byte:    ADD(read_mem(_rPC++)); break;
		case add_c:       ADD(_rC); break;
		case add_d:       ADD(_rD); break;
		case add_e:       ADD(_rE); break;
		case add_h:       ADD(_rH); break;
		case add_hl_bc:   ADD16(HL, BC); break;
		case add_hl_de:   ADD16(HL, DE); break;
		case add_hl_hl:   ADD16(HL, HL); break;
		case add_hl_sp:   ADD16(HL, SP); break;
		case add_l:       ADD(_rL); break;
		case add_mhl:     ADD(read_mem(_rHL)); break;
		case and_a:       AND(_rA); break;
		case and_b:       AND(_rB); break;
		case and_byte:    AND(read_mem(_rPC++)); break;
		case and_c:       AND(_rC); break;
		case and_d:       AND(_rD); break;
		case and_e:       AND(_rE); break;
		case and_h:       AND(_rH); break;
		case and_l:       AND(_rL); break;
		case and_mhl:     AND(read_mem(_rHL)); break;
		case call:        GET_ADDR; CALL; break;
		case call_c:      GET_ADDR; if (_rF & Cflag) { iCycleCount += cc_ex[bOpCode]; CALL } break;
		case call_m:      GET_ADDR; if (_rF & Sflag) { iCycleCount += cc_ex[bOpCode]; CALL } break;
		case call_nc:     GET_ADDR; if (!(_rF & Cflag)) { iCycleCount += cc_ex[bOpCode]; CALL } break;
		case call_nz:     GET_ADDR; if (!(_rF & Zflag)) { iCycleCount += cc_ex[bOpCode]; CALL } break;
		case call_p:      GET_ADDR; if (!(_rF & Sflag)) { iCycleCount += cc_ex[bOpCode]; CALL } break;
		case call_pe:     GET_ADDR; if (_rF & Pflag) { iCycleCount += cc_ex[bOpCode]; CALL } break;
		case call_po:     GET_ADDR; if (!(_rF & Pflag)) { iCycleCount += cc_ex[bOpCode]; CALL } break;
		case call_z:      GET_ADDR; if (_rF & Zflag) { iCycleCount += cc_ex[bOpCode]; CALL } break;
		case ccf:         _rF = ((_rF & (Sflag | Zflag | Pflag | Cflag | Xflags)) | ((_rF & CF) << 4) | (_rA & Xflags)) ^ CF; break;
		case cpl:         _rA ^= 0xff; _rF = (_rF & (Sflag | Zflag | Pflag | Cflag)) | Hflag | Nflag | (_rA & Xflags); break;
		case cp_a:        CP(_rA); break;
		case cp_b:        CP(_rB); break;
		case cp_byte:     CP(read_mem(_rPC++)); break;
		case cp_c:        CP(_rC); break;
		case cp_d:        CP(_rD); break;
		case cp_e:        CP(_rE); break;
		case cp_h:        CP(_rH); break;
		case cp_l:        CP(_rL); break;
		case cp_mhl:      CP(read_mem(_rHL)); break;
		case daa:         DAA; break;
		case dec_a:       DEC(_rA); break;
		case dec_b:       DEC(_rB); break;
		case dec_bc:      _rBC--; iWSAdjust++; break;
		case dec_c:       DEC(_rC); break;
		case dec_d:       DEC(_rD); break;
		case dec_de:      _rDE--; iWSAdjust++; break;
		case dec_e:       DEC(_rE); break;
		case dec_h:       DEC(_rH); break;
		case dec_hl:      _rHL--; iWSAdjust++; break;
		case dec_l:       DEC(_rL); break;
		case dec_mhl:     { byte b = read_mem(_rHL); DEC(b); write_mem(_rHL, b); } break;
		case dec_sp:      _rSP--; iWSAdjust++; break;
		case di:          _rIFF1 = _rIFF2 = 0; EI_issued = 0; break;
		case djnz:        if (--_rB) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; } break;
		case ei:          EI_issued = 2; break;
		case exx:         EXX; break;
		case ex_af_af:    EX(AF, AFx); break;
		case ex_de_hl:    EX(DE, HL); break;
		case ex_msp_hl:   EX_SP(HL); iWSAdjust++; break;
		case halt:        _rHALT = 1; _rPC--; break;
		case ina:         { z80_wait_states iCycleCount = Ia_;} { WZ.b.l = read_mem(_rPC++); WZ.b.h = _rA; _rA = z80_IN_handler(WZ); WZ.w.l++;} break;
		case inc_a:       INC(_rA); break;
		case inc_b:       INC(_rB); break;
		case inc_bc:      _rBC++; iWSAdjust++; break;
		case inc_c:       INC(_rC); break;
		case inc_d:       INC(_rD); break;
		case inc_de:      _rDE++; iWSAdjust++; break;
		case inc_e:       INC(_rE); break;
		case inc_h:       INC(_rH); break;
		case inc_hl:      _rHL++; iWSAdjust++; break;
		case inc_l:       INC(_rL); break;
		case inc_mhl:     { byte b = read_mem(_rHL); INC(b); write_mem(_rHL, b); } break;
		case inc_sp:      _rSP++; iWSAdjust++; break;
		case jp:          GET_ADDR; JP; break;
		case jp_c:        GET_ADDR; if (_rF & Cflag) { JP } break;
		case jp_m:        GET_ADDR; if (_rF & Sflag) { JP } break;
		case jp_nc:       GET_ADDR; if (!(_rF & Cflag)) { JP } break;
		case jp_nz:       GET_ADDR; if (!(_rF & Zflag)) { JP } break;
		case jp_p:        GET_ADDR; if (!(_rF & Sflag)) { JP } break;
		case jp_pe:       GET_ADDR; if (_rF & Pflag) { JP } break;
		case jp_po:       GET_ADDR; if (!(_rF & Pflag)) { JP } break;
		case jp_z:        GET_ADDR; if (_rF & Zflag) { JP } break;
		case jr:          JR; break;
		case jr_c:        if (_rF & Cflag) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; }; break;
		case jr_nc:       if (!(_rF & Cflag)) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; }; break;
		case jr_nz:       if (!(_rF & Zflag)) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; }; break;
		case jr_z:        if (_rF & Zflag) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; }; break;
		case ld_a_a:      break;
		case ld_a_b:      _rA = _rB; break;
		case ld_a_byte:   _rA = read_mem(_rPC++); break;
		case ld_a_c:      _rA = _rC; break;
		case ld_a_d:      _rA = _rD; break;
		case ld_a_e:      _rA = _rE; break;
		case ld_a_h:      _rA = _rH; break;
		case ld_a_l:      _rA = _rL; break;
		case ld_a_mbc:    _rA = read_mem(_rBC); WZ.w.l = _rBC+1; break;
		case ld_a_mde:    _rA = read_mem(_rDE); WZ.w.l = _rDE+1; break;
		case ld_a_mhl:    _rA = read_mem(_rHL); break;
		case ld_a_mword:  { WZ.b.l = read_mem(_rPC++); WZ.b.h = read_mem(_rPC++); _rA = read_mem(WZ.w.l); WZ.w.l++;} break;
		case ld_bc_word:  BC.b.l = read_mem(_rPC++); BC.b.h = read_mem(_rPC++); break;
		case ld_b_a:      _rB = _rA; break;
		case ld_b_b:      break;
		case ld_b_byte:   _rB = read_mem(_rPC++); break;
		case ld_b_c:      _rB = _rC; break;
		case ld_b_d:      _rB = _rD; break;
		case ld_b_e:      _rB = _rE; break;
		case ld_b_h:      _rB = _rH; break;
		case ld_b_l:      _rB = _rL; break;
		case ld_b_mhl:    _rB = read_mem(_rHL); break;
		case ld_c_a:      _rC = _rA; break;
		case ld_c_b:      _rC = _rB; break;
		case ld_c_byte:   _rC = read_mem(_rPC++); break;
		case ld_c_c:      break;
		case ld_c_d:      _rC = _rD; break;
		case ld_c_e:      _rC = _rE; break;
		case ld_c_h:      _rC = _rH; break;
		case ld_c_l:      _rC = _rL; break;
		case ld_c_mhl:    _rC = read_mem(_rHL); break;
		case ld_de_word:  DE.b.l = read_mem(_rPC++); DE.b.h = read_mem(_rPC++); break;
		case ld_d_a:      _rD = _rA; break;
		case ld_d_b:      _rD = _rB; break;
		case ld_d_byte:   _rD = read_mem(_rPC++); break;
		case ld_d_c:      _rD = _rC; break;
		case ld_d_d:      break;
		case ld_d_e:      _rD = _rE; break;
		case ld_d_h:      _rD = _rH; break;
		case ld_d_l:      _rD = _rL; break;
		case ld_d_mhl:    _rD = read_mem(_rHL); break;
		case ld_e_a:      _rE = _rA; break;
		case ld_e_b:      _rE = _rB; break;
		case ld_e_byte:   _rE = read_mem(_rPC++); break;
		case ld_e_c:      _rE = _rC; break;
		case ld_e_d:      _rE = _rD; break;
		case ld_e_e:      break;
		case ld_e_h:      _rE = _rH; break;
		case ld_e_l:      _rE = _rL; break;
		case ld_e_mhl:    _rE = read_mem(_rHL); break;
		case ld_hl_mword: LD16_MEM(HL); break;
		case ld_hl_word:  HL.b.l = read_mem(_rPC++); HL.b.h = read_mem(_rPC++); break;
		case ld_h_a:      _rH = _rA; break;
		case ld_h_b:      _rH = _rB; break;
		case ld_h_byte:   _rH = read_mem(_rPC++); break;
		case ld_h_c:      _rH = _rC; break;
		case ld_h_d:      _rH = _rD; break;
		case ld_h_e:      _rH = _rE; break;
		case ld_h_h:      break;
		case ld_h_l:      _rH = _rL; break;
		case ld_h_mhl:    _rH = read_mem(_rHL); break;
		case ld_l_a:      _rL = _rA; break;
		case ld_l_b:      _rL = _rB; break;
		case ld_l_byte:   _rL = read_mem(_rPC++); break;
		case ld_l_c:      _rL = _rC; break;
		case ld_l_d:      _rL = _rD; break;
		case ld_l_e:      _rL = _rE; break;
		case ld_l_h:      _rL = _rH; break;
		case ld_l_l:      break;
		case ld_l_mhl:    _rL = read_mem(_rHL); break;
		case ld_mbc_a:    write_mem(_rBC, _rA); WZ.w.l=_rBC+1; WZ.b.h=_rA; break;
		case ld_mde_a:    write_mem(_rDE, _rA); break;
		case ld_mhl_a:    write_mem(_rHL, _rA); break;
		case ld_mhl_b:    write_mem(_rHL, _rB); break;
		case ld_mhl_byte: { byte b = read_mem(_rPC++); write_mem(_rHL, b); } break;
		case ld_mhl_c:    write_mem(_rHL, _rC); break;
		case ld_mhl_d:    write_mem(_rHL, _rD); break;
		case ld_mhl_e:    write_mem(_rHL, _rE); break;
		case ld_mhl_h:    write_mem(_rHL, _rH); break;
		case ld_mhl_l:    write_mem(_rHL, _rL); break;
		case ld_mword_a:  { WZ.b.l = read_mem(_rPC++); WZ.b.h = read_mem(_rPC++); write_mem(WZ.w.l, _rA); WZ.w.l++; WZ.b.h = _rA;} break;
		case ld_mword_hl: LDMEM_16(HL); break;
		case ld_pc_hl:    _rPC = _rHL; break;
		case ld_sp_hl:    _rSP = _rHL; iWSAdjust++; break;
		case ld_sp_word:  SP.b.l = read_mem(_rPC++); SP.b.h = read_mem(_rPC++); break;
		case nop:         break;
		case or_a:        OR(_rA); break;
		case or_b:        OR(_rB); break;
		case or_byte:     OR(read_mem(_rPC++)); break;
		case or_c:        OR(_rC); break;
		case or_d:        OR(_rD); break;
		case or_e:        OR(_rE); break;
		case or_h:        OR(_rH); break;
		case or_l:        OR(_rL); break;
		case or_mhl:      OR(read_mem(_rHL)); break;
		case outa:        { z80_wait_states iCycleCount = Oa_;} { WZ.b.l = read_mem(_rPC++); WZ.b.h = _rA; z80_OUT_handler(WZ, _rA); } break;
		case pfx_cb:      z80_pfx_cb(); break;
		case pfx_dd:      z80_pfx_dd(); break;
		case pfx_ed:      z80_pfx_ed(); break;
		case pfx_fd:      z80_pfx_fd(); break;
		case pop_af:      POP(AF); break;
		case pop_bc:      POP(BC); break;
		case pop_de:      POP(DE); break;
		case pop_hl:      POP(HL); break;
		case push_af:     PUSH(AF); break;
		case push_bc:     PUSH(BC); break;
		case push_de:     PUSH(DE); break;
		case push_hl:     PUSH(HL); break;
		case ret:         RET; break;
		case ret_c:       if (_rF & Cflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		case ret_m:       if (_rF & Sflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		case ret_nc:      if (!(_rF & Cflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		case ret_nz:      if (!(_rF & Zflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		case ret_p:       if (!(_rF & Sflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		case ret_pe:      if (_rF & Pflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		case ret_po:      if (!(_rF & Pflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		case ret_z:       if (_rF & Zflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
		case rla:         RLA; break;
		case rlca:        RLCA; break;
		case rra:         RRA; break;
		case rrca:        RRCA; break;
		case rst00:       RST(0x0000); break;
		case rst08:       RST(0x0008); break;
		case rst10:       RST(0x0010); break;
		case rst18:       RST(0x0018); break;
		case rst20:       RST(0x0020); break;
		case rst28:       RST(0x0028); break;
		case rst30:       RST(0x0030); break;
		case rst38:       RST(0x0038); break;
		case sbc_a:       SBC(_rA); break;
		case sbc_b:       SBC(_rB); break;
		case sbc_byte:    SBC(read_mem(_rPC++)); break;
		case sbc_c:       SBC(_rC); break;
		case sbc_d:       SBC(_rD); break;
		case sbc_e:       SBC(_rE); break;
		case sbc_h:       SBC(_rH); break;
		case sbc_l:       SBC(_rL); break;
		case sbc_mhl:     SBC(read_mem(_rHL)); break;
		case scf:         _rF = (_rF & (Sflag | Zflag | Pflag | Xflags)) | Cflag | (_rA & Xflags); break;
		case sub_a:       SUB(_rA); break;
		case sub_b:       SUB(_rB); break;
		case sub_byte:    SUB(read_mem(_rPC++)); break;
		case sub_c:       SUB(_rC); break;
		case sub_d:       SUB(_rD); break;
		case sub_e:       SUB(_rE); break;
		case sub_h:       SUB(_rH); break;
		case sub_l:       SUB(_rL); break;
		case sub_mhl:     SUB(read_mem(_rHL)); break;
		case xor_a:       XOR(_rA); break;
		case xor_b:       XOR(_rB); break;
		case xor_byte:    XOR(read_mem(_rPC++)); break;
		case xor_c:       XOR(_rC); break;
		case xor_d:       XOR(_rD); break;
		case xor_e:       XOR(_rE); break;
		case xor_h:       XOR(_rH); break;
		case xor_l:       XOR(_rL); break;
		case xor_mhl:     XOR(read_mem(_rHL)); break;
      }

      z80_wait_states

		  if (EI_issued) { // EI 'delay' in effect?
			  if (--EI_issued == 0) {
				  _rIFF1 = _rIFF2 = Pflag; // set interrupt flip-flops
				  if (int_pending) {
					  z80_int_handler
				  }
			  }
		  }
		  else if (int_pending) { // any interrupts pending?
			  z80_int_handler
		  }
		  iWSAdjust = 0;

		  if (trace) { // tracing instructions?
			  trace = 0; // reset trace condition
			  return EC_TRACE; // exit emulation loop
		  }
		  else if (VDU->IsFrameFinished()) { // video emulation finished building frame?
			  return EC_FRAME_COMPLETE; // exit emulation loop
		  }
		  else if (PSG->GetBufferFull()) { // sound emulation finished filling a buffer?
			  PSG->SetBufferFull(0);
			  return EC_SOUND_BUFFER; // exit emulation loop
		  }
		  else if (CPC->cycle_count <= 0) { // emulation loop ran for one frame?
			  CPC->cycle_count += CYCLE_COUNT_INIT;
			  return EC_CYCLE_COUNT; // exit emulation loop
		  }
   }

#ifdef USE_DEBUGGER_Z80
   std::cout << "[DEBUG] Breakpoint in " << _rPCdword << endl ;
#endif
   adressAlreadyBlocked = true ;
   return EC_BREAKPOINT;
}



void t_z80regs::z80_pfx_cb(void)
{
	byte bOpCode;

	bOpCode = read_mem(_rPC++);
	iCycleCount += cc_cb[bOpCode];
	_rR++;
	switch(bOpCode)
	{
	case bit0_a:      BIT(0, _rA); break;
	case bit0_b:      BIT(0, _rB); break;
	case bit0_c:      BIT(0, _rC); break;
	case bit0_d:      BIT(0, _rD); break;
	case bit0_e:      BIT(0, _rE); break;
	case bit0_h:      BIT(0, _rH); break;
	case bit0_l:      BIT(0, _rL); break;
	case bit0_mhl:    BIT_HL(0, read_mem(_rHL)); break;
	case bit1_a:      BIT(1, _rA); break;
	case bit1_b:      BIT(1, _rB); break;
	case bit1_c:      BIT(1, _rC); break;
	case bit1_d:      BIT(1, _rD); break;
	case bit1_e:      BIT(1, _rE); break;
	case bit1_h:      BIT(1, _rH); break;
	case bit1_l:      BIT(1, _rL); break;
	case bit1_mhl:    BIT_HL(1, read_mem(_rHL)); break;
	case bit2_a:      BIT(2, _rA); break;
	case bit2_b:      BIT(2, _rB); break;
	case bit2_c:      BIT(2, _rC); break;
	case bit2_d:      BIT(2, _rD); break;
	case bit2_e:      BIT(2, _rE); break;
	case bit2_h:      BIT(2, _rH); break;
	case bit2_l:      BIT(2, _rL); break;
	case bit2_mhl:    BIT_HL(2, read_mem(_rHL)); break;
	case bit3_a:      BIT(3, _rA); break;
	case bit3_b:      BIT(3, _rB); break;
	case bit3_c:      BIT(3, _rC); break;
	case bit3_d:      BIT(3, _rD); break;
	case bit3_e:      BIT(3, _rE); break;
	case bit3_h:      BIT(3, _rH); break;
	case bit3_l:      BIT(3, _rL); break;
	case bit3_mhl:    BIT_HL(3, read_mem(_rHL)); break;
	case bit4_a:      BIT(4, _rA); break;
	case bit4_b:      BIT(4, _rB); break;
	case bit4_c:      BIT(4, _rC); break;
	case bit4_d:      BIT(4, _rD); break;
	case bit4_e:      BIT(4, _rE); break;
	case bit4_h:      BIT(4, _rH); break;
	case bit4_l:      BIT(4, _rL); break;
	case bit4_mhl:    BIT_HL(4, read_mem(_rHL)); break;
	case bit5_a:      BIT(5, _rA); break;
	case bit5_b:      BIT(5, _rB); break;
	case bit5_c:      BIT(5, _rC); break;
	case bit5_d:      BIT(5, _rD); break;
	case bit5_e:      BIT(5, _rE); break;
	case bit5_h:      BIT(5, _rH); break;
	case bit5_l:      BIT(5, _rL); break;
	case bit5_mhl:    BIT_HL(5, read_mem(_rHL)); break;
	case bit6_a:      BIT(6, _rA); break;
	case bit6_b:      BIT(6, _rB); break;
	case bit6_c:      BIT(6, _rC); break;
	case bit6_d:      BIT(6, _rD); break;
	case bit6_e:      BIT(6, _rE); break;
	case bit6_h:      BIT(6, _rH); break;
	case bit6_l:      BIT(6, _rL); break;
	case bit6_mhl:    BIT_HL(6, read_mem(_rHL)); break;
	case bit7_a:      BIT(7, _rA); break;
	case bit7_b:      BIT(7, _rB); break;
	case bit7_c:      BIT(7, _rC); break;
	case bit7_d:      BIT(7, _rD); break;
	case bit7_e:      BIT(7, _rE); break;
	case bit7_h:      BIT(7, _rH); break;
	case bit7_l:      BIT(7, _rL); break;
	case bit7_mhl:    BIT_HL(7, read_mem(_rHL)); break;
	case res0_a:      _rA = RES(0, _rA); break;
	case res0_b:      _rB = RES(0, _rB); break;
	case res0_c:      _rC = RES(0, _rC); break;
	case res0_d:      _rD = RES(0, _rD); break;
	case res0_e:      _rE = RES(0, _rE); break;
	case res0_h:      _rH = RES(0, _rH); break;
	case res0_l:      _rL = RES(0, _rL); break;
	case res0_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, RES(0, b)); } break;
	case res1_a:      _rA = RES(1, _rA); break;
	case res1_b:      _rB = RES(1, _rB); break;
	case res1_c:      _rC = RES(1, _rC); break;
	case res1_d:      _rD = RES(1, _rD); break;
	case res1_e:      _rE = RES(1, _rE); break;
	case res1_h:      _rH = RES(1, _rH); break;
	case res1_l:      _rL = RES(1, _rL); break;
	case res1_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, RES(1, b)); } break;
	case res2_a:      _rA = RES(2, _rA); break;
	case res2_b:      _rB = RES(2, _rB); break;
	case res2_c:      _rC = RES(2, _rC); break;
	case res2_d:      _rD = RES(2, _rD); break;
	case res2_e:      _rE = RES(2, _rE); break;
	case res2_h:      _rH = RES(2, _rH); break;
	case res2_l:      _rL = RES(2, _rL); break;
	case res2_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, RES(2, b)); } break;
	case res3_a:      _rA = RES(3, _rA); break;
	case res3_b:      _rB = RES(3, _rB); break;
	case res3_c:      _rC = RES(3, _rC); break;
	case res3_d:      _rD = RES(3, _rD); break;
	case res3_e:      _rE = RES(3, _rE); break;
	case res3_h:      _rH = RES(3, _rH); break;
	case res3_l:      _rL = RES(3, _rL); break;
	case res3_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, RES(3, b)); } break;
	case res4_a:      _rA = RES(4, _rA); break;
	case res4_b:      _rB = RES(4, _rB); break;
	case res4_c:      _rC = RES(4, _rC); break;
	case res4_d:      _rD = RES(4, _rD); break;
	case res4_e:      _rE = RES(4, _rE); break;
	case res4_h:      _rH = RES(4, _rH); break;
	case res4_l:      _rL = RES(4, _rL); break;
	case res4_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, RES(4, b)); } break;
	case res5_a:      _rA = RES(5, _rA); break;
	case res5_b:      _rB = RES(5, _rB); break;
	case res5_c:      _rC = RES(5, _rC); break;
	case res5_d:      _rD = RES(5, _rD); break;
	case res5_e:      _rE = RES(5, _rE); break;
	case res5_h:      _rH = RES(5, _rH); break;
	case res5_l:      _rL = RES(5, _rL); break;
	case res5_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, RES(5, b)); } break;
	case res6_a:      _rA = RES(6, _rA); break;
	case res6_b:      _rB = RES(6, _rB); break;
	case res6_c:      _rC = RES(6, _rC); break;
	case res6_d:      _rD = RES(6, _rD); break;
	case res6_e:      _rE = RES(6, _rE); break;
	case res6_h:      _rH = RES(6, _rH); break;
	case res6_l:      _rL = RES(6, _rL); break;
	case res6_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, RES(6, b)); } break;
	case res7_a:      _rA = RES(7, _rA); break;
	case res7_b:      _rB = RES(7, _rB); break;
	case res7_c:      _rC = RES(7, _rC); break;
	case res7_d:      _rD = RES(7, _rD); break;
	case res7_e:      _rE = RES(7, _rE); break;
	case res7_h:      _rH = RES(7, _rH); break;
	case res7_l:      _rL = RES(7, _rL); break;
	case res7_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, RES(7, b)); } break;
	case rlc_a:       _rA = RLC(_rA); break;
	case rlc_b:       _rB = RLC(_rB); break;
	case rlc_c:       _rC = RLC(_rC); break;
	case rlc_d:       _rD = RLC(_rD); break;
	case rlc_e:       _rE = RLC(_rE); break;
	case rlc_h:       _rH = RLC(_rH); break;
	case rlc_l:       _rL = RLC(_rL); break;
	case rlc_mhl:     { byte b = read_mem(_rHL); write_mem(_rHL, RLC(b)); } break;
	case rl_a:        _rA = RL(_rA); break;
	case rl_b:        _rB = RL(_rB); break;
	case rl_c:        _rC = RL(_rC); break;
	case rl_d:        _rD = RL(_rD); break;
	case rl_e:        _rE = RL(_rE); break;
	case rl_h:        _rH = RL(_rH); break;
	case rl_l:        _rL = RL(_rL); break;
	case rl_mhl:      { byte b = read_mem(_rHL); write_mem(_rHL, RL(b)); } break;
	case rrc_a:       _rA = RRC(_rA); break;
	case rrc_b:       _rB = RRC(_rB); break;
	case rrc_c:       _rC = RRC(_rC); break;
	case rrc_d:       _rD = RRC(_rD); break;
	case rrc_e:       _rE = RRC(_rE); break;
	case rrc_h:       _rH = RRC(_rH); break;
	case rrc_l:       _rL = RRC(_rL); break;
	case rrc_mhl:     { byte b = read_mem(_rHL); write_mem(_rHL, RRC(b)); } break;
	case rr_a:        _rA = RR(_rA); break;
	case rr_b:        _rB = RR(_rB); break;
	case rr_c:        _rC = RR(_rC); break;
	case rr_d:        _rD = RR(_rD); break;
	case rr_e:        _rE = RR(_rE); break;
	case rr_h:        _rH = RR(_rH); break;
	case rr_l:        _rL = RR(_rL); break;
	case rr_mhl:      { byte b = read_mem(_rHL); write_mem(_rHL, RR(b)); } break;
	case set0_a:      _rA = SET(0, _rA); break;
	case set0_b:      _rB = SET(0, _rB); break;
	case set0_c:      _rC = SET(0, _rC); break;
	case set0_d:      _rD = SET(0, _rD); break;
	case set0_e:      _rE = SET(0, _rE); break;
	case set0_h:      _rH = SET(0, _rH); break;
	case set0_l:      _rL = SET(0, _rL); break;
	case set0_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, SET(0, b)); } break;
	case set1_a:      _rA = SET(1, _rA); break;
	case set1_b:      _rB = SET(1, _rB); break;
	case set1_c:      _rC = SET(1, _rC); break;
	case set1_d:      _rD = SET(1, _rD); break;
	case set1_e:      _rE = SET(1, _rE); break;
	case set1_h:      _rH = SET(1, _rH); break;
	case set1_l:      _rL = SET(1, _rL); break;
	case set1_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, SET(1, b)); } break;
	case set2_a:      _rA = SET(2, _rA); break;
	case set2_b:      _rB = SET(2, _rB); break;
	case set2_c:      _rC = SET(2, _rC); break;
	case set2_d:      _rD = SET(2, _rD); break;
	case set2_e:      _rE = SET(2, _rE); break;
	case set2_h:      _rH = SET(2, _rH); break;
	case set2_l:      _rL = SET(2, _rL); break;
	case set2_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, SET(2, b)); } break;
	case set3_a:      _rA = SET(3, _rA); break;
	case set3_b:      _rB = SET(3, _rB); break;
	case set3_c:      _rC = SET(3, _rC); break;
	case set3_d:      _rD = SET(3, _rD); break;
	case set3_e:      _rE = SET(3, _rE); break;
	case set3_h:      _rH = SET(3, _rH); break;
	case set3_l:      _rL = SET(3, _rL); break;
	case set3_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, SET(3, b)); } break;
	case set4_a:      _rA = SET(4, _rA); break;
	case set4_b:      _rB = SET(4, _rB); break;
	case set4_c:      _rC = SET(4, _rC); break;
	case set4_d:      _rD = SET(4, _rD); break;
	case set4_e:      _rE = SET(4, _rE); break;
	case set4_h:      _rH = SET(4, _rH); break;
	case set4_l:      _rL = SET(4, _rL); break;
	case set4_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, SET(4, b)); } break;
	case set5_a:      _rA = SET(5, _rA); break;
	case set5_b:      _rB = SET(5, _rB); break;
	case set5_c:      _rC = SET(5, _rC); break;
	case set5_d:      _rD = SET(5, _rD); break;
	case set5_e:      _rE = SET(5, _rE); break;
	case set5_h:      _rH = SET(5, _rH); break;
	case set5_l:      _rL = SET(5, _rL); break;
	case set5_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, SET(5, b)); } break;
	case set6_a:      _rA = SET(6, _rA); break;
	case set6_b:      _rB = SET(6, _rB); break;
	case set6_c:      _rC = SET(6, _rC); break;
	case set6_d:      _rD = SET(6, _rD); break;
	case set6_e:      _rE = SET(6, _rE); break;
	case set6_h:      _rH = SET(6, _rH); break;
	case set6_l:      _rL = SET(6, _rL); break;
	case set6_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, SET(6, b)); } break;
	case set7_a:      _rA = SET(7, _rA); break;
	case set7_b:      _rB = SET(7, _rB); break;
	case set7_c:      _rC = SET(7, _rC); break;
	case set7_d:      _rD = SET(7, _rD); break;
	case set7_e:      _rE = SET(7, _rE); break;
	case set7_h:      _rH = SET(7, _rH); break;
	case set7_l:      _rL = SET(7, _rL); break;
	case set7_mhl:    { byte b = read_mem(_rHL); write_mem(_rHL, SET(7, b)); } break;
	case sla_a:       _rA = SLA(_rA); break;
	case sla_b:       _rB = SLA(_rB); break;
	case sla_c:       _rC = SLA(_rC); break;
	case sla_d:       _rD = SLA(_rD); break;
	case sla_e:       _rE = SLA(_rE); break;
	case sla_h:       _rH = SLA(_rH); break;
	case sla_l:       _rL = SLA(_rL); break;
	case sla_mhl:     { byte b = read_mem(_rHL); write_mem(_rHL, SLA(b)); } break;
	case sll_a:       _rA = SLL(_rA); break;
	case sll_b:       _rB = SLL(_rB); break;
	case sll_c:       _rC = SLL(_rC); break;
	case sll_d:       _rD = SLL(_rD); break;
	case sll_e:       _rE = SLL(_rE); break;
	case sll_h:       _rH = SLL(_rH); break;
	case sll_l:       _rL = SLL(_rL); break;
	case sll_mhl:     { byte b = read_mem(_rHL); write_mem(_rHL, SLL(b)); } break;
	case sra_a:       _rA = SRA(_rA); break;
	case sra_b:       _rB = SRA(_rB); break;
	case sra_c:       _rC = SRA(_rC); break;
	case sra_d:       _rD = SRA(_rD); break;
	case sra_e:       _rE = SRA(_rE); break;
	case sra_h:       _rH = SRA(_rH); break;
	case sra_l:       _rL = SRA(_rL); break;
	case sra_mhl:     { byte b = read_mem(_rHL); write_mem(_rHL, SRA(b)); } break;
	case srl_a:       _rA = SRL(_rA); break;
	case srl_b:       _rB = SRL(_rB); break;
	case srl_c:       _rC = SRL(_rC); break;
	case srl_d:       _rD = SRL(_rD); break;
	case srl_e:       _rE = SRL(_rE); break;
	case srl_h:       _rH = SRL(_rH); break;
	case srl_l:       _rL = SRL(_rL); break;
	case srl_mhl:     { byte b = read_mem(_rHL); write_mem(_rHL, SRL(b)); } break;
   }
}



void t_z80regs::z80_pfx_dd(void)
{
	byte bOpCode;

	bOpCode = read_mem(_rPC++);
	iCycleCount += cc_xy[bOpCode];
	_rR++;
	switch(bOpCode)
	{
	case adc_a:       ADC(_rA); break;
	case adc_b:       ADC(_rB); break;
	case adc_byte:    ADC(read_mem(_rPC++)); break;
	case adc_c:       ADC(_rC); break;
	case adc_d:       ADC(_rD); break;
	case adc_e:       ADC(_rE); break;
	case adc_h:       ADC(_rIXh); break;
	case adc_l:       ADC(_rIXl); break;
	case adc_mhl:     { signed char o = read_mem(_rPC++); ADC(read_mem(_rIX+o)); WZ.w.l=_rIX+o; } break;
	case add_a:       ADD(_rA); break;
	case add_b:       ADD(_rB); break;
	case add_byte:    ADD(read_mem(_rPC++)); break;
	case add_c:       ADD(_rC); break;
	case add_d:       ADD(_rD); break;
	case add_e:       ADD(_rE); break;
	case add_h:       ADD(_rIXh); break;
	case add_hl_bc:   ADD16(IX, BC); break;
	case add_hl_de:   ADD16(IX, DE); break;
	case add_hl_hl:   ADD16(IX, IX); break;
	case add_hl_sp:   ADD16(IX, SP); break;
	case add_l:       ADD(_rIXl); break;
	case add_mhl:     { signed char o = read_mem(_rPC++); ADD(read_mem(_rIX+o)); WZ.w.l=_rIX+o;} break;
	case and_a:       AND(_rA); break;
	case and_b:       AND(_rB); break;
	case and_byte:    AND(read_mem(_rPC++)); break;
	case and_c:       AND(_rC); break;
	case and_d:       AND(_rD); break;
	case and_e:       AND(_rE); break;
	case and_h:       AND(_rIXh); break;
	case and_l:       AND(_rIXl); break;
	case and_mhl:     { signed char o = read_mem(_rPC++); AND(read_mem(_rIX+o)); WZ.w.l=_rIX+o; } break;
	case call:        GET_ADDR; CALL; break;
	case call_c:      GET_ADDR; if (_rF & Cflag) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_m:      GET_ADDR; if (_rF & Sflag) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_nc:     GET_ADDR; if (!(_rF & Cflag)) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_nz:     GET_ADDR; if (!(_rF & Zflag)) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_p:      GET_ADDR; if (!(_rF & Sflag)) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_pe:     GET_ADDR; if (_rF & Pflag) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_po:     GET_ADDR; if (!(_rF & Pflag)) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_z:      GET_ADDR; if (_rF & Zflag) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case ccf:         _rF = ((_rF & (Sflag | Zflag | Pflag | Cflag)) | ((_rF & CF) << 4) | (_rA & Xflags)) ^ CF; break;
	case cpl:         _rA ^= 0xff; _rF = (_rF & (Sflag | Zflag | Pflag | Cflag)) | Hflag | Nflag | (_rA & Xflags); break;
	case cp_a:        CP(_rA); break;
	case cp_b:        CP(_rB); break;
	case cp_byte:     CP(read_mem(_rPC++)); break;
	case cp_c:        CP(_rC); break;
	case cp_d:        CP(_rD); break;
	case cp_e:        CP(_rE); break;
	case cp_h:        CP(_rIXh); break;
	case cp_l:        CP(_rIXl); break;
	case cp_mhl:      { signed char o = read_mem(_rPC++); CP(read_mem(_rIX+o)); WZ.w.l = _rIX+o; } break;
	case daa:         DAA; break;
	case dec_a:       DEC(_rA); break;
	case dec_b:       DEC(_rB); break;
	case dec_bc:      _rBC--; iWSAdjust++; break;
	case dec_c:       DEC(_rC); break;
	case dec_d:       DEC(_rD); break;
	case dec_de:      _rDE--; iWSAdjust++; break;
	case dec_e:       DEC(_rE); break;
	case dec_h:       DEC(_rIXh); break;
	case dec_hl:      _rIX--; iWSAdjust++; break;
	case dec_l:       DEC(_rIXl); break;
	case dec_mhl:     { signed char o = read_mem(_rPC++); byte b = read_mem(_rIX+o); DEC(b); write_mem(_rIX+o, b); WZ.w.l=_rIX+o;} break;
	case dec_sp:      _rSP--; iWSAdjust++; break;
	case di:          _rIFF1 = _rIFF2 = 0; EI_issued = 0; break;
	case djnz:        if (--_rB) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; } break;
	case ei:          EI_issued = 2; break;
	case exx:         EXX; break;
	case ex_af_af:    EX(AF, AFx); break;
	case ex_de_hl:    EX(DE, HL); break;
	case ex_msp_hl:   EX_SP(IX); iWSAdjust++; break;
	case halt:        _rHALT = 1; _rPC--; break;
	case ina:         { z80_wait_states iCycleCount = Ia_;} { WZ.b.l = read_mem(_rPC++); WZ.b.h = _rA; _rA = z80_IN_handler(WZ); WZ.w.l++;} break;
	case inc_a:       INC(_rA); break;
	case inc_b:       INC(_rB); break;
	case inc_bc:      _rBC++; iWSAdjust++; break;
	case inc_c:       INC(_rC); break;
	case inc_d:       INC(_rD); break;
	case inc_de:      _rDE++; iWSAdjust++; break;
	case inc_e:       INC(_rE); break;
	case inc_h:       INC(_rIXh); break;
	case inc_hl:      _rIX++; iWSAdjust++; break;
	case inc_l:       INC(_rIXl); break;
	case inc_mhl:     { signed char o = read_mem(_rPC++); byte b = read_mem(_rIX+o); INC(b); write_mem(_rIX+o, b); WZ.w.l = _rIX+o;} break;
	case inc_sp:      _rSP++; iWSAdjust++; break;
	case jp:          GET_ADDR; JP; break;
	case jp_c:        GET_ADDR; if (_rF & Cflag) { JP } break;
	case jp_m:        GET_ADDR; if (_rF & Sflag) { JP } break;
	case jp_nc:       GET_ADDR; if (!(_rF & Cflag)) { JP } break;
	case jp_nz:       GET_ADDR; if (!(_rF & Zflag)) { JP } break;
	case jp_p:        GET_ADDR; if (!(_rF & Sflag)) { JP } break;
	case jp_pe:       GET_ADDR; if (_rF & Pflag) { JP } break;
	case jp_po:       GET_ADDR; if (!(_rF & Pflag)) { JP } break;
	case jp_z:        GET_ADDR; if (_rF & Zflag) { JP } break;
	case jr:          JR; break;
	case jr_c:        if (_rF & Cflag) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; }; break;
	case jr_nc:       if (!(_rF & Cflag)) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; }; break;
	case jr_nz:       if (!(_rF & Zflag)) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; }; break;
	case jr_z:        if (_rF & Zflag) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; }; break;
	case ld_a_a:      break;
	case ld_a_b:      _rA = _rB; break;
	case ld_a_byte:   _rA = read_mem(_rPC++); break;
	case ld_a_c:      _rA = _rC; break;
	case ld_a_d:      _rA = _rD; break;
	case ld_a_e:      _rA = _rE; break;
	case ld_a_h:      _rA = _rIXh; break;
	case ld_a_l:      _rA = _rIXl; break;
	case ld_a_mbc:    _rA = read_mem(_rBC); break;
	case ld_a_mde:    _rA = read_mem(_rDE); break;
	case ld_a_mhl:    { signed char o = read_mem(_rPC++); _rA = read_mem(_rIX+o); WZ.w.l=_rIX+o; } break;
	case ld_a_mword:  { WZ.b.l = read_mem(_rPC++); WZ.b.h = read_mem(_rPC++); _rA = read_mem(WZ.w.l); WZ.w.l++;} break;
	case ld_bc_word:  BC.b.l = read_mem(_rPC++); BC.b.h = read_mem(_rPC++); break;
	case ld_b_a:      _rB = _rA; break;
	case ld_b_b:      break;
	case ld_b_byte:   _rB = read_mem(_rPC++); break;
	case ld_b_c:      _rB = _rC; break;
	case ld_b_d:      _rB = _rD; break;
	case ld_b_e:      _rB = _rE; break;
	case ld_b_h:      _rB = _rIXh; break;
	case ld_b_l:      _rB = _rIXl; break;
	case ld_b_mhl:    { signed char o = read_mem(_rPC++); _rB = read_mem(_rIX+o); WZ.w.l=_rIX+o;} break;
	case ld_c_a:      _rC = _rA; break;
	case ld_c_b:      _rC = _rB; break;
	case ld_c_byte:   _rC = read_mem(_rPC++); break;
	case ld_c_c:      break;
	case ld_c_d:      _rC = _rD; break;
	case ld_c_e:      _rC = _rE; break;
	case ld_c_h:      _rC = _rIXh; break;
	case ld_c_l:      _rC = _rIXl; break;
	case ld_c_mhl:    { signed char o = read_mem(_rPC++); _rC = read_mem(_rIX+o); WZ.w.l=_rIX+o;} break;
	case ld_de_word:  DE.b.l = read_mem(_rPC++); DE.b.h = read_mem(_rPC++); break;
	case ld_d_a:      _rD = _rA; break;
	case ld_d_b:      _rD = _rB; break;
	case ld_d_byte:   _rD = read_mem(_rPC++); break;
	case ld_d_c:      _rD = _rC; break;
	case ld_d_d:      break;
	case ld_d_e:      _rD = _rE; break;
	case ld_d_h:      _rD = _rIXh; break;
	case ld_d_l:      _rD = _rIXl; break;
	case ld_d_mhl:    { signed char o = read_mem(_rPC++); _rD = read_mem(_rIX+o); WZ.w.l=_rIX+o;} break;
	case ld_e_a:      _rE = _rA; break;
	case ld_e_b:      _rE = _rB; break;
	case ld_e_byte:   _rE = read_mem(_rPC++); break;
	case ld_e_c:      _rE = _rC; break;
	case ld_e_d:      _rE = _rD; break;
	case ld_e_e:      break;
	case ld_e_h:      _rE = _rIXh; break;
	case ld_e_l:      _rE = _rIXl; break;
	case ld_e_mhl:    { signed char o = read_mem(_rPC++); _rE = read_mem(_rIX+o); WZ.w.l=_rIX+o;} break;
	case ld_hl_mword: LD16_MEM(IX); break;
	case ld_hl_word:  IX.b.l = read_mem(_rPC++); IX.b.h = read_mem(_rPC++); break;
	case ld_h_a:      _rIXh = _rA; break;
	case ld_h_b:      _rIXh = _rB; break;
	case ld_h_byte:   _rIXh = read_mem(_rPC++); break;
	case ld_h_c:      _rIXh = _rC; break;
	case ld_h_d:      _rIXh = _rD; break;
	case ld_h_e:      _rIXh = _rE; break;
	case ld_h_h:      break;
	case ld_h_l:      _rIXh = _rIXl; break;
	case ld_h_mhl:    { signed char o = read_mem(_rPC++); _rH = read_mem(_rIX+o); WZ.w.l=_rIX+o;} break;
	case ld_l_a:      _rIXl = _rA; break;
	case ld_l_b:      _rIXl = _rB; break;
	case ld_l_byte:   _rIXl = read_mem(_rPC++); break;
	case ld_l_c:      _rIXl = _rC; break;
	case ld_l_d:      _rIXl = _rD; break;
	case ld_l_e:      _rIXl = _rE; break;
	case ld_l_h:      _rIXl = _rIXh; break;
	case ld_l_l:      break;
	case ld_l_mhl:    { signed char o = read_mem(_rPC++); _rL = read_mem(_rIX+o); WZ.w.l=_rIX+o;} break;
	case ld_mbc_a:    write_mem(_rBC, _rA); break;
	case ld_mde_a:    write_mem(_rDE, _rA); break;
	case ld_mhl_a:    { signed char o = read_mem(_rPC++); write_mem(_rIX+o, _rA); WZ.w.l=_rIX+o;} break;
	case ld_mhl_b:    { signed char o = read_mem(_rPC++); write_mem(_rIX+o, _rB); WZ.w.l=_rIX+o;} break;
	case ld_mhl_byte: { signed char o = read_mem(_rPC++); byte b = read_mem(_rPC++); write_mem(_rIX+o, b); WZ.w.l=_rIX+o;} break;
	case ld_mhl_c:    { signed char o = read_mem(_rPC++); write_mem(_rIX+o, _rC); WZ.w.l=_rIX+o;} break;
	case ld_mhl_d:    { signed char o = read_mem(_rPC++); write_mem(_rIX+o, _rD); WZ.w.l=_rIX+o; } break;
	case ld_mhl_e:    { signed char o = read_mem(_rPC++); write_mem(_rIX+o, _rE); WZ.w.l=_rIX+o; } break;
	case ld_mhl_h:    { signed char o = read_mem(_rPC++); write_mem(_rIX+o, _rH); WZ.w.l=_rIX+o; } break;
	case ld_mhl_l:    { signed char o = read_mem(_rPC++); write_mem(_rIX+o, _rL); WZ.w.l=_rIX+o; } break;
	case ld_mword_a:  { WZ.b.l = read_mem(_rPC++); WZ.b.h = read_mem(_rPC++); write_mem(WZ.w.l, _rA); WZ.w.l++; WZ.b.h=_rA;} break;
	case ld_mword_hl: LDMEM_16(IX); break;
	case ld_pc_hl:    _rPC = _rIX; break;
	case ld_sp_hl:    _rSP = _rIX; iWSAdjust++; break;
	case ld_sp_word:  SP.b.l = read_mem(_rPC++); SP.b.h = read_mem(_rPC++); break;
	case nop:         break;
	case or_a:        OR(_rA); break;
	case or_b:        OR(_rB); break;
	case or_byte:     OR(read_mem(_rPC++)); break;
	case or_c:        OR(_rC); break;
	case or_d:        OR(_rD); break;
	case or_e:        OR(_rE); break;
	case or_h:        OR(_rIXh); break;
	case or_l:        OR(_rIXl); break;
	case or_mhl:      { signed char o = read_mem(_rPC++); OR(read_mem(_rIX+o));  WZ.w.l=_rIX+o;} break;
	case outa:        { z80_wait_states iCycleCount = Oa_;} { WZ.b.l = read_mem(_rPC++); WZ.b.h = _rA; z80_OUT_handler(WZ, _rA); WZ.b.l++;} break;
	case pfx_cb:      z80_pfx_ddcb(); break;
	case pfx_dd:      z80_pfx_dd(); break;
	case pfx_ed:      z80_pfx_ed(); break;
	case pfx_fd:      z80_pfx_fd(); break;
	case pop_af:      POP(AF); break;
	case pop_bc:      POP(BC); break;
	case pop_de:      POP(DE); break;
	case pop_hl:      POP(IX); break;
	case push_af:     PUSH(AF); break;
	case push_bc:     PUSH(BC); break;
	case push_de:     PUSH(DE); break;
	case push_hl:     PUSH(IX); break;
	case ret:         RET; break;
	case ret_c:       if (_rF & Cflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_m:       if (_rF & Sflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_nc:      if (!(_rF & Cflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_nz:      if (!(_rF & Zflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_p:       if (!(_rF & Sflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_pe:      if (_rF & Pflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_po:      if (!(_rF & Pflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_z:       if (_rF & Zflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case rla:         RLA; break;
	case rlca:        RLCA; break;
	case rra:         RRA; break;
	case rrca:        RRCA; break;
	case rst00:       RST(0x0000); break;
	case rst08:       RST(0x0008); break;
	case rst10:       RST(0x0010); break;
	case rst18:       RST(0x0018); break;
	case rst20:       RST(0x0020); break;
	case rst28:       RST(0x0028); break;
	case rst30:       RST(0x0030); break;
	case rst38:       RST(0x0038); break;
	case sbc_a:       SBC(_rA); break;
	case sbc_b:       SBC(_rB); break;
	case sbc_byte:    SBC(read_mem(_rPC++)); break;
	case sbc_c:       SBC(_rC); break;
	case sbc_d:       SBC(_rD); break;
	case sbc_e:       SBC(_rE); break;
	case sbc_h:       SBC(_rIXh); break;
	case sbc_l:       SBC(_rIXl); break;
	case sbc_mhl:     { signed char o = read_mem(_rPC++); SBC(read_mem(_rIX+o)); WZ.w.l=_rIX+o; } break;
	case scf:         _rF = (_rF & (Sflag | Zflag | Pflag)) | Cflag | (_rA & Xflags); break;
	case sub_a:       SUB(_rA); break;
	case sub_b:       SUB(_rB); break;
	case sub_byte:    SUB(read_mem(_rPC++)); break;
	case sub_c:       SUB(_rC); break;
	case sub_d:       SUB(_rD); break;
	case sub_e:       SUB(_rE); break;
	case sub_h:       SUB(_rIXh); break;
	case sub_l:       SUB(_rIXl); break;
	case sub_mhl:     { signed char o = read_mem(_rPC++); SUB(read_mem(_rIX+o)); WZ.w.l=_rIX+o; } break;
	case xor_a:       XOR(_rA); break;
	case xor_b:       XOR(_rB); break;
	case xor_byte:    XOR(read_mem(_rPC++)); break;
	case xor_c:       XOR(_rC); break;
	case xor_d:       XOR(_rD); break;
	case xor_e:       XOR(_rE); break;
	case xor_h:       XOR(_rIXh); break;
	case xor_l:       XOR(_rIXl); break;
	case xor_mhl:     { signed char o = read_mem(_rPC++); XOR(read_mem(_rIX+o)); WZ.w.l=_rIX+o; } break;
   }
}



void t_z80regs::z80_pfx_ddcb(void)
{
	signed char o;
	byte bOpCode;

	o = read_mem(_rPC++); // offset
	bOpCode = read_mem(_rPC++);
	iCycleCount += cc_xycb[bOpCode];
	WZ.w.l=_rIX+o;
	switch(bOpCode)
	{
	case bit0_a:      BIT_XY(0, read_mem(_rIX+o)); break;
	case bit0_b:      BIT_XY(0, read_mem(_rIX+o)); break;
	case bit0_c:      BIT_XY(0, read_mem(_rIX+o)); break;
	case bit0_d:      BIT_XY(0, read_mem(_rIX+o)); break;
	case bit0_e:      BIT_XY(0, read_mem(_rIX+o)); break;
	case bit0_h:      BIT_XY(0, read_mem(_rIX+o)); break;
	case bit0_l:      BIT_XY(0, read_mem(_rIX+o)); break;
	case bit0_mhl:    BIT_HL(0, read_mem(_rIX+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags); break;
	case bit1_a:      BIT_XY(1, read_mem(_rIX+o)); break;
	case bit1_b:      BIT_XY(1, read_mem(_rIX+o)); break;
	case bit1_c:      BIT_XY(1, read_mem(_rIX+o)); break;
	case bit1_d:      BIT_XY(1, read_mem(_rIX+o)); break;
	case bit1_e:      BIT_XY(1, read_mem(_rIX+o)); break;
	case bit1_h:      BIT_XY(1, read_mem(_rIX+o)); break;
	case bit1_l:      BIT_XY(1, read_mem(_rIX+o)); break;
	case bit1_mhl:    BIT_HL(1, read_mem(_rIX+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit2_a:      BIT_XY(2, read_mem(_rIX+o)); break;
	case bit2_b:      BIT_XY(2, read_mem(_rIX+o)); break;
	case bit2_c:      BIT_XY(2, read_mem(_rIX+o)); break;
	case bit2_d:      BIT_XY(2, read_mem(_rIX+o)); break;
	case bit2_e:      BIT_XY(2, read_mem(_rIX+o)); break;
	case bit2_h:      BIT_XY(2, read_mem(_rIX+o)); break;
	case bit2_l:      BIT_XY(2, read_mem(_rIX+o)); break;
	case bit2_mhl:    BIT_HL(2, read_mem(_rIX+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit3_a:      BIT_XY(3, read_mem(_rIX+o)); break;
	case bit3_b:      BIT_XY(3, read_mem(_rIX+o)); break;
	case bit3_c:      BIT_XY(3, read_mem(_rIX+o)); break;
	case bit3_d:      BIT_XY(3, read_mem(_rIX+o)); break;
	case bit3_e:      BIT_XY(3, read_mem(_rIX+o)); break;
	case bit3_h:      BIT_XY(3, read_mem(_rIX+o)); break;
	case bit3_l:      BIT_XY(3, read_mem(_rIX+o)); break;
	case bit3_mhl:    BIT_HL(3, read_mem(_rIX+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit4_a:      BIT_XY(4, read_mem(_rIX+o)); break;
	case bit4_b:      BIT_XY(4, read_mem(_rIX+o)); break;
	case bit4_c:      BIT_XY(4, read_mem(_rIX+o)); break;
	case bit4_d:      BIT_XY(4, read_mem(_rIX+o)); break;
	case bit4_e:      BIT_XY(4, read_mem(_rIX+o)); break;
	case bit4_h:      BIT_XY(4, read_mem(_rIX+o)); break;
	case bit4_l:      BIT_XY(4, read_mem(_rIX+o)); break;
	case bit4_mhl:    BIT_HL(4, read_mem(_rIX+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit5_a:      BIT_XY(5, read_mem(_rIX+o)); break;
	case bit5_b:      BIT_XY(5, read_mem(_rIX+o)); break;
	case bit5_c:      BIT_XY(5, read_mem(_rIX+o)); break;
	case bit5_d:      BIT_XY(5, read_mem(_rIX+o)); break;
	case bit5_e:      BIT_XY(5, read_mem(_rIX+o)); break;
	case bit5_h:      BIT_XY(5, read_mem(_rIX+o)); break;
	case bit5_l:      BIT_XY(5, read_mem(_rIX+o)); break;
	case bit5_mhl:    BIT_HL(5, read_mem(_rIX+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit6_a:      BIT_XY(6, read_mem(_rIX+o)); break;
	case bit6_b:      BIT_XY(6, read_mem(_rIX+o)); break;
	case bit6_c:      BIT_XY(6, read_mem(_rIX+o)); break;
	case bit6_d:      BIT_XY(6, read_mem(_rIX+o)); break;
	case bit6_e:      BIT_XY(6, read_mem(_rIX+o)); break;
	case bit6_h:      BIT_XY(6, read_mem(_rIX+o)); break;
	case bit6_l:      BIT_XY(6, read_mem(_rIX+o)); break;
	case bit6_mhl:    BIT_HL(6, read_mem(_rIX+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit7_a:      BIT_XY(7, read_mem(_rIX+o)); break;
	case bit7_b:      BIT_XY(7, read_mem(_rIX+o)); break;
	case bit7_c:      BIT_XY(7, read_mem(_rIX+o)); break;
	case bit7_d:      BIT_XY(7, read_mem(_rIX+o)); break;
	case bit7_e:      BIT_XY(7, read_mem(_rIX+o)); break;
	case bit7_h:      BIT_XY(7, read_mem(_rIX+o)); break;
	case bit7_l:      BIT_XY(7, read_mem(_rIX+o)); break;
	case bit7_mhl:    BIT_HL(7, read_mem(_rIX+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case res0_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = RES(0, _rA)); break;
	case res0_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = RES(0, _rB)); break;
	case res0_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = RES(0, _rC)); break;
	case res0_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = RES(0, _rD)); break;
	case res0_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = RES(0, _rE)); break;
	case res0_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = RES(0, _rH)); break;
	case res0_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = RES(0, _rL)); break;
	case res0_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, RES(0, b)); } break;
	case res1_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = RES(1, _rA)); break;
	case res1_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = RES(1, _rB)); break;
	case res1_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = RES(1, _rC)); break;
	case res1_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = RES(1, _rD)); break;
	case res1_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = RES(1, _rE)); break;
	case res1_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = RES(1, _rH)); break;
	case res1_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = RES(1, _rL)); break;
	case res1_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, RES(1, b)); } break;
	case res2_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = RES(2, _rA)); break;
	case res2_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = RES(2, _rB)); break;
	case res2_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = RES(2, _rC)); break;
	case res2_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = RES(2, _rD)); break;
	case res2_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = RES(2, _rE)); break;
	case res2_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = RES(2, _rH)); break;
	case res2_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = RES(2, _rL)); break;
	case res2_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, RES(2, b)); } break;
	case res3_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = RES(3, _rA)); break;
	case res3_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = RES(3, _rB)); break;
	case res3_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = RES(3, _rC)); break;
	case res3_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = RES(3, _rD)); break;
	case res3_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = RES(3, _rE)); break;
	case res3_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = RES(3, _rH)); break;
	case res3_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = RES(3, _rL)); break;
	case res3_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, RES(3, b)); } break;
	case res4_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = RES(4, _rA)); break;
	case res4_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = RES(4, _rB)); break;
	case res4_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = RES(4, _rC)); break;
	case res4_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = RES(4, _rD)); break;
	case res4_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = RES(4, _rE)); break;
	case res4_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = RES(4, _rH)); break;
	case res4_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = RES(4, _rL)); break;
	case res4_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, RES(4, b)); } break;
	case res5_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = RES(5, _rA)); break;
	case res5_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = RES(5, _rB)); break;
	case res5_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = RES(5, _rC)); break;
	case res5_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = RES(5, _rD)); break;
	case res5_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = RES(5, _rE)); break;
	case res5_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = RES(5, _rH)); break;
	case res5_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = RES(5, _rL)); break;
	case res5_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, RES(5, b)); } break;
	case res6_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = RES(6, _rA)); break;
	case res6_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = RES(6, _rB)); break;
	case res6_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = RES(6, _rC)); break;
	case res6_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = RES(6, _rD)); break;
	case res6_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = RES(6, _rE)); break;
	case res6_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = RES(6, _rH)); break;
	case res6_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = RES(6, _rL)); break;
	case res6_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, RES(6, b)); } break;
	case res7_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = RES(7, _rA)); break;
	case res7_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = RES(7, _rB)); break;
	case res7_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = RES(7, _rC)); break;
	case res7_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = RES(7, _rD)); break;
	case res7_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = RES(7, _rE)); break;
	case res7_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = RES(7, _rH)); break;
	case res7_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = RES(7, _rL)); break;
	case res7_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, RES(7, b)); } break;
	case rlc_a:       _rA = read_mem(_rIX+o); _rA = RLC(_rA); write_mem(_rIX+o, _rA); break;
	case rlc_b:       _rB = read_mem(_rIX+o); _rB = RLC(_rB); write_mem(_rIX+o, _rB); break;
	case rlc_c:       _rC = read_mem(_rIX+o); _rC = RLC(_rC); write_mem(_rIX+o, _rC); break;
	case rlc_d:       _rD = read_mem(_rIX+o); _rD = RLC(_rD); write_mem(_rIX+o, _rD); break;
	case rlc_e:       _rE = read_mem(_rIX+o); _rE = RLC(_rE); write_mem(_rIX+o, _rE); break;
	case rlc_h:       _rH = read_mem(_rIX+o); _rH = RLC(_rH); write_mem(_rIX+o, _rH); break;
	case rlc_l:       _rL = read_mem(_rIX+o); _rL = RLC(_rL); write_mem(_rIX+o, _rL); break;
	case rlc_mhl:     { byte b = read_mem(_rIX+o); write_mem(_rIX+o, RLC(b)); } break;
	case rl_a:        _rA = read_mem(_rIX+o); _rA = RL(_rA); write_mem(_rIX+o, _rA); break;
	case rl_b:        _rB = read_mem(_rIX+o); _rB = RL(_rB); write_mem(_rIX+o, _rB); break;
	case rl_c:        _rC = read_mem(_rIX+o); _rC = RL(_rC); write_mem(_rIX+o, _rC); break;
	case rl_d:        _rD = read_mem(_rIX+o); _rD = RL(_rD); write_mem(_rIX+o, _rD); break;
	case rl_e:        _rE = read_mem(_rIX+o); _rE = RL(_rE); write_mem(_rIX+o, _rE); break;
	case rl_h:        _rH = read_mem(_rIX+o); _rH = RL(_rH); write_mem(_rIX+o, _rH); break;
	case rl_l:        _rL = read_mem(_rIX+o); _rL = RL(_rL); write_mem(_rIX+o, _rL); break;
	case rl_mhl:      { byte b = read_mem(_rIX+o); write_mem(_rIX+o, RL(b)); } break;
	case rrc_a:       _rA = read_mem(_rIX+o); _rA = RRC(_rA); write_mem(_rIX+o, _rA); break;
	case rrc_b:       _rB = read_mem(_rIX+o); _rB = RRC(_rB); write_mem(_rIX+o, _rB); break;
	case rrc_c:       _rC = read_mem(_rIX+o); _rC = RRC(_rC); write_mem(_rIX+o, _rC); break;
	case rrc_d:       _rD = read_mem(_rIX+o); _rD = RRC(_rD); write_mem(_rIX+o, _rD); break;
	case rrc_e:       _rE = read_mem(_rIX+o); _rE = RRC(_rE); write_mem(_rIX+o, _rE); break;
	case rrc_h:       _rH = read_mem(_rIX+o); _rH = RRC(_rH); write_mem(_rIX+o, _rH); break;
	case rrc_l:       _rL = read_mem(_rIX+o); _rL = RRC(_rL); write_mem(_rIX+o, _rL); break;
	case rrc_mhl:     { byte b = read_mem(_rIX+o); write_mem(_rIX+o, RRC(b)); } break;
	case rr_a:        _rA = read_mem(_rIX+o); _rA = RR(_rA); write_mem(_rIX+o, _rA); break;
	case rr_b:        _rB = read_mem(_rIX+o); _rB = RR(_rB); write_mem(_rIX+o, _rB); break;
	case rr_c:        _rC = read_mem(_rIX+o); _rC = RR(_rC); write_mem(_rIX+o, _rC); break;
	case rr_d:        _rD = read_mem(_rIX+o); _rD = RR(_rD); write_mem(_rIX+o, _rD); break;
	case rr_e:        _rE = read_mem(_rIX+o); _rE = RR(_rE); write_mem(_rIX+o, _rE); break;
	case rr_h:        _rH = read_mem(_rIX+o); _rH = RR(_rH); write_mem(_rIX+o, _rH); break;
	case rr_l:        _rL = read_mem(_rIX+o); _rL = RR(_rL); write_mem(_rIX+o, _rL); break;
	case rr_mhl:      { byte b = read_mem(_rIX+o); write_mem(_rIX+o, RR(b)); } break;
	case set0_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = SET(0, _rA)); break;
	case set0_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = SET(0, _rB)); break;
	case set0_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = SET(0, _rC)); break;
	case set0_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = SET(0, _rD)); break;
	case set0_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = SET(0, _rE)); break;
	case set0_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = SET(0, _rH)); break;
	case set0_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = SET(0, _rL)); break;
	case set0_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, SET(0, b)); } break;
	case set1_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = SET(1, _rA)); break;
	case set1_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = SET(1, _rB)); break;
	case set1_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = SET(1, _rC)); break;
	case set1_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = SET(1, _rD)); break;
	case set1_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = SET(1, _rE)); break;
	case set1_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = SET(1, _rH)); break;
	case set1_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = SET(1, _rL)); break;
	case set1_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, SET(1, b)); } break;
	case set2_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = SET(2, _rA)); break;
	case set2_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = SET(2, _rB)); break;
	case set2_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = SET(2, _rC)); break;
	case set2_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = SET(2, _rD)); break;
	case set2_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = SET(2, _rE)); break;
	case set2_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = SET(2, _rH)); break;
	case set2_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = SET(2, _rL)); break;
	case set2_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, SET(2, b)); } break;
	case set3_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = SET(3, _rA)); break;
	case set3_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = SET(3, _rB)); break;
	case set3_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = SET(3, _rC)); break;
	case set3_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = SET(3, _rD)); break;
	case set3_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = SET(3, _rE)); break;
	case set3_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = SET(3, _rH)); break;
	case set3_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = SET(3, _rL)); break;
	case set3_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, SET(3, b)); } break;
	case set4_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = SET(4, _rA)); break;
	case set4_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = SET(4, _rB)); break;
	case set4_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = SET(4, _rC)); break;
	case set4_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = SET(4, _rD)); break;
	case set4_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = SET(4, _rE)); break;
	case set4_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = SET(4, _rH)); break;
	case set4_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = SET(4, _rL)); break;
	case set4_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, SET(4, b)); } break;
	case set5_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = SET(5, _rA)); break;
	case set5_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = SET(5, _rB)); break;
	case set5_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = SET(5, _rC)); break;
	case set5_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = SET(5, _rD)); break;
	case set5_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = SET(5, _rE)); break;
	case set5_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = SET(5, _rH)); break;
	case set5_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = SET(5, _rL)); break;
	case set5_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, SET(5, b)); } break;
	case set6_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = SET(6, _rA)); break;
	case set6_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = SET(6, _rB)); break;
	case set6_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = SET(6, _rC)); break;
	case set6_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = SET(6, _rD)); break;
	case set6_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = SET(6, _rE)); break;
	case set6_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = SET(6, _rH)); break;
	case set6_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = SET(6, _rL)); break;
	case set6_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, SET(6, b)); } break;
	case set7_a:      _rA = read_mem(_rIX+o); write_mem(_rIX+o, _rA = SET(7, _rA)); break;
	case set7_b:      _rB = read_mem(_rIX+o); write_mem(_rIX+o, _rB = SET(7, _rB)); break;
	case set7_c:      _rC = read_mem(_rIX+o); write_mem(_rIX+o, _rC = SET(7, _rC)); break;
	case set7_d:      _rD = read_mem(_rIX+o); write_mem(_rIX+o, _rD = SET(7, _rD)); break;
	case set7_e:      _rE = read_mem(_rIX+o); write_mem(_rIX+o, _rE = SET(7, _rE)); break;
	case set7_h:      _rH = read_mem(_rIX+o); write_mem(_rIX+o, _rH = SET(7, _rH)); break;
	case set7_l:      _rL = read_mem(_rIX+o); write_mem(_rIX+o, _rL = SET(7, _rL)); break;
	case set7_mhl:    { byte b = read_mem(_rIX+o); write_mem(_rIX+o, SET(7, b)); } break;
	case sla_a:       _rA = read_mem(_rIX+o); _rA = SLA(_rA); write_mem(_rIX+o, _rA); break;
	case sla_b:       _rB = read_mem(_rIX+o); _rB = SLA(_rB); write_mem(_rIX+o, _rB); break;
	case sla_c:       _rC = read_mem(_rIX+o); _rC = SLA(_rC); write_mem(_rIX+o, _rC); break;
	case sla_d:       _rD = read_mem(_rIX+o); _rD = SLA(_rD); write_mem(_rIX+o, _rD); break;
	case sla_e:       _rE = read_mem(_rIX+o); _rE = SLA(_rE); write_mem(_rIX+o, _rE); break;
	case sla_h:       _rH = read_mem(_rIX+o); _rH = SLA(_rH); write_mem(_rIX+o, _rH); break;
	case sla_l:       _rL = read_mem(_rIX+o); _rL = SLA(_rL); write_mem(_rIX+o, _rL); break;
	case sla_mhl:     { byte b = read_mem(_rIX+o); write_mem(_rIX+o, SLA(b)); } break;
	case sll_a:       _rA = read_mem(_rIX+o); _rA = SLL(_rA); write_mem(_rIX+o, _rA); break;
	case sll_b:       _rB = read_mem(_rIX+o); _rB = SLL(_rB); write_mem(_rIX+o, _rB); break;
	case sll_c:       _rC = read_mem(_rIX+o); _rC = SLL(_rC); write_mem(_rIX+o, _rC); break;
	case sll_d:       _rD = read_mem(_rIX+o); _rD = SLL(_rD); write_mem(_rIX+o, _rD); break;
	case sll_e:       _rE = read_mem(_rIX+o); _rE = SLL(_rE); write_mem(_rIX+o, _rE); break;
	case sll_h:       _rH = read_mem(_rIX+o); _rH = SLL(_rH); write_mem(_rIX+o, _rH); break;
	case sll_l:       _rL = read_mem(_rIX+o); _rL = SLL(_rL); write_mem(_rIX+o, _rL); break;
	case sll_mhl:     { byte b = read_mem(_rIX+o); write_mem(_rIX+o, SLL(b)); } break;
	case sra_a:       _rA = read_mem(_rIX+o); _rA = SRA(_rA); write_mem(_rIX+o, _rA); break;
	case sra_b:       _rB = read_mem(_rIX+o); _rB = SRA(_rB); write_mem(_rIX+o, _rB); break;
	case sra_c:       _rC = read_mem(_rIX+o); _rC = SRA(_rC); write_mem(_rIX+o, _rC); break;
	case sra_d:       _rD = read_mem(_rIX+o); _rD = SRA(_rD); write_mem(_rIX+o, _rD); break;
	case sra_e:       _rE = read_mem(_rIX+o); _rE = SRA(_rE); write_mem(_rIX+o, _rE); break;
	case sra_h:       _rH = read_mem(_rIX+o); _rH = SRA(_rH); write_mem(_rIX+o, _rH); break;
	case sra_l:       _rL = read_mem(_rIX+o); _rL = SRA(_rL); write_mem(_rIX+o, _rL); break;
	case sra_mhl:     { byte b = read_mem(_rIX+o); write_mem(_rIX+o, SRA(b)); } break;
	case srl_a:       _rA = read_mem(_rIX+o); _rA = SRL(_rA); write_mem(_rIX+o, _rA); break;
	case srl_b:       _rB = read_mem(_rIX+o); _rB = SRL(_rB); write_mem(_rIX+o, _rB); break;
	case srl_c:       _rC = read_mem(_rIX+o); _rC = SRL(_rC); write_mem(_rIX+o, _rC); break;
	case srl_d:       _rD = read_mem(_rIX+o); _rD = SRL(_rD); write_mem(_rIX+o, _rD); break;
	case srl_e:       _rE = read_mem(_rIX+o); _rE = SRL(_rE); write_mem(_rIX+o, _rE); break;
	case srl_h:       _rH = read_mem(_rIX+o); _rH = SRL(_rH); write_mem(_rIX+o, _rH); break;
	case srl_l:       _rL = read_mem(_rIX+o); _rL = SRL(_rL); write_mem(_rIX+o, _rL); break;
	case srl_mhl:     { byte b = read_mem(_rIX+o); write_mem(_rIX+o, SRL(b)); } break;
   }
}



void t_z80regs::z80_pfx_ed(void)
{
	byte bOpCode;

	bOpCode = read_mem(_rPC++);
	iCycleCount += cc_ed[bOpCode];
	_rR++;
	switch(bOpCode)
	{
	case adc_hl_bc:   ADC16(BC); break;
	case adc_hl_de:   ADC16(DE); break;
	case adc_hl_hl:   ADC16(HL); break;
	case adc_hl_sp:   ADC16(SP); break;
	case cpd:         CPD; break;
	case cpdr:        CPDR; break;
	case cpi:         CPI; break;
	case cpir:        CPIR; break;
	case ed_00:       break;
	case ed_01:       break;
	case ed_02:       break;
	case ed_03:       break;
	case ed_04:       break;
	case ed_05:       break;
	case ed_06:       break;
	case ed_07:       break;
	case ed_08:       break;
	case ed_09:       break;
	case ed_0a:       break;
	case ed_0b:       break;
	case ed_0c:       break;
	case ed_0d:       break;
	case ed_0e:       break;
	case ed_0f:       break;
	case ed_10:       break;
	case ed_11:       break;
	case ed_12:       break;
	case ed_13:       break;
	case ed_14:       break;
	case ed_15:       break;
	case ed_16:       break;
	case ed_17:       break;
	case ed_18:       break;
	case ed_19:       break;
	case ed_1a:       break;
	case ed_1b:       break;
	case ed_1c:       break;
	case ed_1d:       break;
	case ed_1e:       break;
	case ed_1f:       break;
	case ed_20:       break;
	case ed_21:       break;
	case ed_22:       break;
	case ed_23:       break;
	case ed_24:       break;
	case ed_25:       break;
	case ed_26:       break;
	case ed_27:       break;
	case ed_28:       break;
	case ed_29:       break;
	case ed_2a:       break;
	case ed_2b:       break;
	case ed_2c:       break;
	case ed_2d:       break;
	case ed_2e:       break;
	case ed_2f:       break;
	case ed_30:       break;
	case ed_31:       break;
	case ed_32:       break;
	case ed_33:       break;
	case ed_34:       break;
	case ed_35:       break;
	case ed_36:       break;
	case ed_37:       break;
	case ed_38:       break;
	case ed_39:       break;
	case ed_3a:       break;
	case ed_3b:       break;
	case ed_3c:       break;
	case ed_3d:       break;
	case ed_3e:       break;
	case ed_3f:       break;
	case ed_77:       break;
	case ed_7f:       break;
	case ed_80:       break;
	case ed_81:       break;
	case ed_82:       break;
	case ed_83:       break;
	case ed_84:       break;
	case ed_85:       break;
	case ed_86:       break;
	case ed_87:       break;
	case ed_88:       break;
	case ed_89:       break;
	case ed_8a:       break;
	case ed_8b:       break;
	case ed_8c:       break;
	case ed_8d:       break;
	case ed_8e:       break;
	case ed_8f:       break;
	case ed_90:       break;
	case ed_91:       break;
	case ed_92:       break;
	case ed_93:       break;
	case ed_94:       break;
	case ed_95:       break;
	case ed_96:       break;
	case ed_97:       break;
	case ed_98:       break;
	case ed_99:       break;
	case ed_9a:       break;
	case ed_9b:       break;
	case ed_9c:       break;
	case ed_9d:       break;
	case ed_9e:       break;
	case ed_9f:       break;
	case ed_a4:       break;
	case ed_a5:       break;
	case ed_a6:       break;
	case ed_a7:       break;
	case ed_ac:       break;
	case ed_ad:       break;
	case ed_ae:       break;
	case ed_af:       break;
	case ed_b4:       break;
	case ed_b5:       break;
	case ed_b6:       break;
	case ed_b7:       break;
	case ed_bc:       break;
	case ed_bd:       break;
	case ed_be:       break;
	case ed_bf:       break;
	case ed_c0:       break;
	case ed_c1:       break;
	case ed_c2:       break;
	case ed_c3:       break;
	case ed_c4:       break;
	case ed_c5:       break;
	case ed_c6:       break;
	case ed_c7:       break;
	case ed_c8:       break;
	case ed_c9:       break;
	case ed_ca:       break;
	case ed_cb:       break;
	case ed_cc:       break;
	case ed_cd:       break;
	case ed_ce:       break;
	case ed_cf:       break;
	case ed_d0:       break;
	case ed_d1:       break;
	case ed_d2:       break;
	case ed_d3:       break;
	case ed_d4:       break;
	case ed_d5:       break;
	case ed_d6:       break;
	case ed_d7:       break;
	case ed_d8:       break;
	case ed_d9:       break;
	case ed_da:       break;
	case ed_db:       break;
	case ed_dc:       break;
	case ed_dd:       break;
	case ed_de:       break;
	case ed_df:       break;
	case ed_e0:       break;
	case ed_e1:       break;
	case ed_e2:       break;
	case ed_e3:       break;
	case ed_e4:       break;
	case ed_e5:       break;
	case ed_e6:       break;
	case ed_e7:       break;
	case ed_e8:       break;
	case ed_e9:       break;
	case ed_ea:       break;
	case ed_eb:       break;
	case ed_ec:       break;
	case ed_ed:       break;
	case ed_ee:       break;
	case ed_ef:       break;
	case ed_f0:       break;
	case ed_f1:       break;
	case ed_f2:       break;
	case ed_f3:       break;
	case ed_f4:       break;
	case ed_f5:       break;
	case ed_f6:       break;
	case ed_f7:       break;
	case ed_f8:       break;
	case ed_f9:       break;
	case ed_fa:       break;
	case ed_fb:       break;
	case ed_fc:       break;
	case ed_fd:       break;
	case ed_fe:       break;
	case ed_ff:       break;
	case im_0:        _rIM = 0; break;
	case im_0_1:      _rIM = 0; break;
	case im_0_2:      _rIM = 0; break;
	case im_0_3:      _rIM = 0; break;
	case im_1:        _rIM = 1; break;
	case im_1_1:      _rIM = 1; break;
	case im_2:        _rIM = 2; break;
	case im_2_1:      _rIM = 2; break;
	case ind:         { z80_wait_states iCycleCount = Iy_;} IND; break;
	case indr:        { z80_wait_states iCycleCount = Iy_;} INDR; break;
	case ini:         { z80_wait_states iCycleCount = Iy_;} INI; break;
	case inir:        { z80_wait_states iCycleCount = Iy_;} INIR; break;
	case in_0_c:      { z80_wait_states iCycleCount = Ix_;} { byte res = z80_IN_handler(BC); _rF = (_rF & Cflag) | SZP[res]; } break;
	case in_a_c:      { z80_wait_states iCycleCount = Ix_;} _rA = z80_IN_handler(BC); _rF = (_rF & Cflag) | SZP[_rA]; WZ.w.l = _rBC; break;
	case in_b_c:      { z80_wait_states iCycleCount = Ix_;} _rB = z80_IN_handler(BC); _rF = (_rF & Cflag) | SZP[_rB]; break;
	case in_c_c:      { z80_wait_states iCycleCount = Ix_;} _rC = z80_IN_handler(BC); _rF = (_rF & Cflag) | SZP[_rC]; break;
	case in_d_c:      { z80_wait_states iCycleCount = Ix_;} _rD = z80_IN_handler(BC); _rF = (_rF & Cflag) | SZP[_rD]; break;
	case in_e_c:      { z80_wait_states iCycleCount = Ix_;} _rE = z80_IN_handler(BC); _rF = (_rF & Cflag) | SZP[_rE]; break;
	case in_h_c:      { z80_wait_states iCycleCount = Ix_;} _rH = z80_IN_handler(BC); _rF = (_rF & Cflag) | SZP[_rH]; break;
	case in_l_c:      { z80_wait_states iCycleCount = Ix_;} _rL = z80_IN_handler(BC); _rF = (_rF & Cflag) | SZP[_rL]; break;
	case ldd:         LDD; iWSAdjust++; break;
	case lddr:        LDDR; iWSAdjust++; break;
	case ldi:         LDI; iWSAdjust++; break;
	case ldir:        LDIR; iWSAdjust++; break;
	case ld_a_i:      _rA = _rI; _rF = (_rF & Cflag) | SZ[_rA] | _rIFF2; iWSAdjust++; break;
	case ld_a_r:      _rA = (_rR & 0x7f) | _rRb7; _rF = (_rF & Cflag) | SZ[_rA] | _rIFF2; iWSAdjust++; break;
	case ld_EDbc_mword:  LD16_MEM(BC); break;
	case ld_EDde_mword:  LD16_MEM(DE); break;
	case ld_EDhl_mword:  LD16_MEM(HL); break;
	case ld_EDmword_bc:  LDMEM_16(BC); break;
	case ld_EDmword_de:  LDMEM_16(DE); break;
	case ld_EDmword_hl:  LDMEM_16(HL); break;
	case ld_EDmword_sp:  LDMEM_16(SP); break;
	case ld_EDsp_mword:  LD16_MEM(SP); break;
	case ld_i_a:      _rI = _rA; iWSAdjust++; break;
	case ld_r_a:      _rR = _rA; _rRb7 = _rA & 0x80; iWSAdjust++; break;
	case neg:         NEG; break;
	case neg_1:       NEG; break;
	case neg_2:       NEG; break;
	case neg_3:       NEG; break;
	case neg_4:       NEG; break;
	case neg_5:       NEG; break;
	case neg_6:       NEG; break;
	case neg_7:       NEG; break;
	case otdr:        { z80_wait_states iCycleCount = Oy_;} OTDR; break;
	case otir:        { z80_wait_states iCycleCount = Oy_;} OTIR; break;
	case outd:        { z80_wait_states iCycleCount = Oy_;} OUTD; break;
	case outi:        { z80_wait_states iCycleCount = Oy_;} OUTI; break;
	case out_c_0:     { z80_wait_states iCycleCount = Ox_;} z80_OUT_handler(BC, 0); break;
	case out_c_a:     { z80_wait_states iCycleCount = Ox_;} z80_OUT_handler(BC, _rA); WZ.w.l = _rBC+1; break;
	case out_c_b:     { z80_wait_states iCycleCount = Ox_;} z80_OUT_handler(BC, _rB); break;
	case out_c_c:     { z80_wait_states iCycleCount = Ox_;} z80_OUT_handler(BC, _rC); break;
	case out_c_d:     { z80_wait_states iCycleCount = Ox_;} z80_OUT_handler(BC, _rD); break;
	case out_c_e:     { z80_wait_states iCycleCount = Ox_;} z80_OUT_handler(BC, _rE); break;
	case out_c_h:     { z80_wait_states iCycleCount = Ox_;} z80_OUT_handler(BC, _rH); break;
	case out_c_l:     { z80_wait_states iCycleCount = Ox_;} z80_OUT_handler(BC, _rL); break;
	case reti:        _rIFF1 = _rIFF2; RET; break;
	case reti_1:      _rIFF1 = _rIFF2; RET; break;
	case reti_2:      _rIFF1 = _rIFF2; RET; break;
	case reti_3:      _rIFF1 = _rIFF2; RET; break;
	case retn:        _rIFF1 = _rIFF2; RET; break;
	case retn_1:      _rIFF1 = _rIFF2; RET; break;
	case retn_2:      _rIFF1 = _rIFF2; RET; break;
	case retn_3:      _rIFF1 = _rIFF2; RET; break;
	case rld:         RLD; break;
	case rrd:         RRD; break;
	case sbc_hl_bc:   SBC16(BC); break;
	case sbc_hl_de:   SBC16(DE); break;
	case sbc_hl_hl:   SBC16(HL); break;
	case sbc_hl_sp:   SBC16(SP); break;
   }
}



void t_z80regs::z80_pfx_fd(void)
{
	byte bOpCode;

	bOpCode = read_mem(_rPC++);
	iCycleCount += cc_xy[bOpCode];
	_rR++;
	switch(bOpCode)
	{
	case adc_a:       ADC(_rA); break;
	case adc_b:       ADC(_rB); break;
	case adc_byte:    ADC(read_mem(_rPC++)); break;
	case adc_c:       ADC(_rC); break;
	case adc_d:       ADC(_rD); break;
	case adc_e:       ADC(_rE); break;
	case adc_h:       ADC(_rIYh); break;
	case adc_l:       ADC(_rIYl); break;
	case adc_mhl:     { signed char o = read_mem(_rPC++); ADC(read_mem(_rIY+o)); WZ.w.l=_rIY+o; } break;
	case add_a:       ADD(_rA); break;
	case add_b:       ADD(_rB); break;
	case add_byte:    ADD(read_mem(_rPC++)); break;
	case add_c:       ADD(_rC); break;
	case add_d:       ADD(_rD); break;
	case add_e:       ADD(_rE); break;
	case add_h:       ADD(_rIYh); break;
	case add_hl_bc:   ADD16(IY, BC); break;
	case add_hl_de:   ADD16(IY, DE); break;
	case add_hl_hl:   ADD16(IY, IY); break;
	case add_hl_sp:   ADD16(IY, SP); break;
	case add_l:       ADD(_rIYl); break;
	case add_mhl:     { signed char o = read_mem(_rPC++); ADD(read_mem(_rIY+o)); WZ.w.l=_rIY+o;  } break;
	case and_a:       AND(_rA); break;
	case and_b:       AND(_rB); break;
	case and_byte:    AND(read_mem(_rPC++)); break;
	case and_c:       AND(_rC); break;
	case and_d:       AND(_rD); break;
	case and_e:       AND(_rE); break;
	case and_h:       AND(_rIYh); break;
	case and_l:       AND(_rIYl); break;
	case and_mhl:     { signed char o = read_mem(_rPC++); AND(read_mem(_rIY+o)); WZ.w.l=_rIY+o;  } break;
	case call:        GET_ADDR; CALL; break;
	case call_c:      GET_ADDR; if (_rF & Cflag) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_m:      GET_ADDR; if (_rF & Sflag) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_nc:     GET_ADDR; if (!(_rF & Cflag)) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_nz:     GET_ADDR; if (!(_rF & Zflag)) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_p:      GET_ADDR; if (!(_rF & Sflag)) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_pe:     GET_ADDR; if (_rF & Pflag) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_po:     GET_ADDR; if (!(_rF & Pflag)) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case call_z:      GET_ADDR; if (_rF & Zflag) { iCycleCount += cc_ex[bOpCode]; CALL } break;
	case ccf:         _rF = ((_rF & (Sflag | Zflag | Pflag | Cflag)) | ((_rF & CF) << 4) | (_rA & Xflags)) ^ CF; break;
	case cpl:         _rA ^= 0xff; _rF = (_rF & (Sflag | Zflag | Pflag | Cflag)) | Hflag | Nflag | (_rA & Xflags); break;
	case cp_a:        CP(_rA); break;
	case cp_b:        CP(_rB); break;
	case cp_byte:     CP(read_mem(_rPC++)); break;
	case cp_c:        CP(_rC); break;
	case cp_d:        CP(_rD); break;
	case cp_e:        CP(_rE); break;
	case cp_h:        CP(_rIYh); break;
	case cp_l:        CP(_rIYl); break;
	case cp_mhl:      { signed char o = read_mem(_rPC++); CP(read_mem(_rIY+o)); WZ.w.l=_rIY+o;  } break;
	case daa:         DAA; break;
	case dec_a:       DEC(_rA); break;
	case dec_b:       DEC(_rB); break;
	case dec_bc:      _rBC--; iWSAdjust++; break;
	case dec_c:       DEC(_rC); break;
	case dec_d:       DEC(_rD); break;
	case dec_de:      _rDE--; iWSAdjust++; break;
	case dec_e:       DEC(_rE); break;
	case dec_h:       DEC(_rIYh); break;
	case dec_hl:      _rIY--; iWSAdjust++; break;
	case dec_l:       DEC(_rIYl); break;
	case dec_mhl:     { signed char o = read_mem(_rPC++); byte b = read_mem(_rIY+o); DEC(b); write_mem(_rIY+o, b); WZ.w.l=_rIY+o;  } break;
	case dec_sp:      _rSP--; iWSAdjust++; break;
	case di:          _rIFF1 = _rIFF2 = 0; EI_issued = 0; break;
	case djnz:        if (--_rB) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; } break;
	case ei:          EI_issued = 2; break;
	case exx:         EXX; break;
	case ex_af_af:    EX(AF, AFx); break;
	case ex_de_hl:    EX(DE, HL); break;
	case ex_msp_hl:   EX_SP(IY); iWSAdjust++; break;
	case halt:        _rHALT = 1; _rPC--; break;
	case ina:         { z80_wait_states iCycleCount = Ia_;} { WZ.b.l = read_mem(_rPC++); WZ.b.h = _rA; _rA = z80_IN_handler(WZ); WZ.w.l++;} break;
	case inc_a:       INC(_rA); break;
	case inc_b:       INC(_rB); break;
	case inc_bc:      _rBC++; iWSAdjust++; break;
	case inc_c:       INC(_rC); break;
	case inc_d:       INC(_rD); break;
	case inc_de:      _rDE++; iWSAdjust++; break;
	case inc_e:       INC(_rE); break;
	case inc_h:       INC(_rIYh); break;
	case inc_hl:      _rIY++; iWSAdjust++; break;
	case inc_l:       INC(_rIYl); break;
	case inc_mhl:     { signed char o = read_mem(_rPC++); byte b = read_mem(_rIY+o); INC(b); write_mem(_rIY+o, b); WZ.w.l=_rIY+o;  } break;
	case inc_sp:      _rSP++; iWSAdjust++; break;
	case jp:          GET_ADDR; JP; break;
	case jp_c:        GET_ADDR; if (_rF & Cflag) { JP } break;
	case jp_m:        GET_ADDR; if (_rF & Sflag) { JP } break;
	case jp_nc:       GET_ADDR; if (!(_rF & Cflag)) { JP } break;
	case jp_nz:       GET_ADDR; if (!(_rF & Zflag)) { JP } break;
	case jp_p:        GET_ADDR; if (!(_rF & Sflag)) { JP } break;
	case jp_pe:       GET_ADDR; if (_rF & Pflag) { JP } break;
	case jp_po:       GET_ADDR; if (!(_rF & Pflag)) { JP } break;
	case jp_z:        GET_ADDR; if (_rF & Zflag) { JP } break;
	case jr:          JR; break;
	case jr_c:        if (_rF & Cflag) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; }; break;
	case jr_nc:       if (!(_rF & Cflag)) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; }; break;
	case jr_nz:       if (!(_rF & Zflag)) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; }; break;
	case jr_z:        if (_rF & Zflag) { iCycleCount += cc_ex[bOpCode]; JR } else { _rPC++; }; break;
	case ld_a_a:      break;
	case ld_a_b:      _rA = _rB; break;
	case ld_a_byte:   _rA = read_mem(_rPC++); break;
	case ld_a_c:      _rA = _rC; break;
	case ld_a_d:      _rA = _rD; break;
	case ld_a_e:      _rA = _rE; break;
	case ld_a_h:      _rA = _rIYh; break;
	case ld_a_l:      _rA = _rIYl; break;
	case ld_a_mbc:    _rA = read_mem(_rBC); break;
	case ld_a_mde:    _rA = read_mem(_rDE); break;
	case ld_a_mhl:    { signed char o = read_mem(_rPC++); _rA = read_mem(_rIY+o); WZ.w.l=_rIY+o;  } break;
	case ld_a_mword:  { WZ.b.l = read_mem(_rPC++); WZ.b.h = read_mem(_rPC++); _rA = read_mem(WZ.w.l); WZ.w.l++;} break;
	case ld_bc_word:  BC.b.l = read_mem(_rPC++); BC.b.h = read_mem(_rPC++); break;
	case ld_b_a:      _rB = _rA; break;
	case ld_b_b:      break;
	case ld_b_byte:   _rB = read_mem(_rPC++); break;
	case ld_b_c:      _rB = _rC; break;
	case ld_b_d:      _rB = _rD; break;
	case ld_b_e:      _rB = _rE; break;
	case ld_b_h:      _rB = _rIYh; break;
	case ld_b_l:      _rB = _rIYl; break;
	case ld_b_mhl:    { signed char o = read_mem(_rPC++); _rB = read_mem(_rIY+o); WZ.w.l=_rIY+o;  } break;
	case ld_c_a:      _rC = _rA; break;
	case ld_c_b:      _rC = _rB; break;
	case ld_c_byte:   _rC = read_mem(_rPC++); break;
	case ld_c_c:      break;
	case ld_c_d:      _rC = _rD; break;
	case ld_c_e:      _rC = _rE; break;
	case ld_c_h:      _rC = _rIYh; break;
	case ld_c_l:      _rC = _rIYl; break;
	case ld_c_mhl:    { signed char o = read_mem(_rPC++); _rC = read_mem(_rIY+o); WZ.w.l=_rIY+o;  } break;
	case ld_de_word:  DE.b.l = read_mem(_rPC++); DE.b.h = read_mem(_rPC++); break;
	case ld_d_a:      _rD = _rA; break;
	case ld_d_b:      _rD = _rB; break;
	case ld_d_byte:   _rD = read_mem(_rPC++); break;
	case ld_d_c:      _rD = _rC; break;
	case ld_d_d:      break;
	case ld_d_e:      _rD = _rE; break;
	case ld_d_h:      _rD = _rIYh; break;
	case ld_d_l:      _rD = _rIYl; break;
	case ld_d_mhl:    { signed char o = read_mem(_rPC++); _rD = read_mem(_rIY+o); WZ.w.l=_rIY+o;  } break;
	case ld_e_a:      _rE = _rA; break;
	case ld_e_b:      _rE = _rB; break;
	case ld_e_byte:   _rE = read_mem(_rPC++); break;
	case ld_e_c:      _rE = _rC; break;
	case ld_e_d:      _rE = _rD; break;
	case ld_e_e:      break;
	case ld_e_h:      _rE = _rIYh; break;
	case ld_e_l:      _rE = _rIYl; break;
	case ld_e_mhl:    { signed char o = read_mem(_rPC++); _rE = read_mem(_rIY+o); WZ.w.l=_rIY+o;  } break;
	case ld_hl_mword: LD16_MEM(IY); break;
	case ld_hl_word:  IY.b.l = read_mem(_rPC++); IY.b.h = read_mem(_rPC++); break;
	case ld_h_a:      _rIYh = _rA; break;
	case ld_h_b:      _rIYh = _rB; break;
	case ld_h_byte:   _rIYh = read_mem(_rPC++); break;
	case ld_h_c:      _rIYh = _rC; break;
	case ld_h_d:      _rIYh = _rD; break;
	case ld_h_e:      _rIYh = _rE; break;
	case ld_h_h:      break;
	case ld_h_l:      _rIYh = _rIYl; break;
	case ld_h_mhl:    { signed char o = read_mem(_rPC++); _rH = read_mem(_rIY+o); WZ.w.l=_rIY+o;  } break;
	case ld_l_a:      _rIYl = _rA; break;
	case ld_l_b:      _rIYl = _rB; break;
	case ld_l_byte:   _rIYl = read_mem(_rPC++); break;
	case ld_l_c:      _rIYl = _rC; break;
	case ld_l_d:      _rIYl = _rD; break;
	case ld_l_e:      _rIYl = _rE; break;
	case ld_l_h:      _rIYl = _rIYh; break;
	case ld_l_l:      break;
	case ld_l_mhl:    { signed char o = read_mem(_rPC++); _rL = read_mem(_rIY+o); WZ.w.l=_rIY+o;  } break;
	case ld_mbc_a:    write_mem(_rBC, _rA); break;
	case ld_mde_a:    write_mem(_rDE, _rA); break;
	case ld_mhl_a:    { signed char o = read_mem(_rPC++); write_mem(_rIY+o, _rA); WZ.w.l=_rIY+o;  } break;
	case ld_mhl_b:    { signed char o = read_mem(_rPC++); write_mem(_rIY+o, _rB); WZ.w.l=_rIY+o;  } break;
	case ld_mhl_byte: { signed char o = read_mem(_rPC++); byte b = read_mem(_rPC++); write_mem(_rIY+o, b); WZ.w.l=_rIY+o;  } break;
	case ld_mhl_c:    { signed char o = read_mem(_rPC++); write_mem(_rIY+o, _rC); WZ.w.l=_rIY+o;  } break;
	case ld_mhl_d:    { signed char o = read_mem(_rPC++); write_mem(_rIY+o, _rD); WZ.w.l=_rIY+o;  } break;
	case ld_mhl_e:    { signed char o = read_mem(_rPC++); write_mem(_rIY+o, _rE); WZ.w.l=_rIY+o;  } break;
	case ld_mhl_h:    { signed char o = read_mem(_rPC++); write_mem(_rIY+o, _rH); WZ.w.l=_rIY+o;  } break;
	case ld_mhl_l:    { signed char o = read_mem(_rPC++); write_mem(_rIY+o, _rL); WZ.w.l=_rIY+o;  } break;
	case ld_mword_a:  { WZ.b.l = read_mem(_rPC++); WZ.b.h = read_mem(_rPC++); write_mem(WZ.w.l, _rA); WZ.w.l++; WZ.b.h=_rA;} break;
	case ld_mword_hl: LDMEM_16(IY); break;
	case ld_pc_hl:    _rPC = _rIY; break;
	case ld_sp_hl:    _rSP = _rIY; iWSAdjust++; break;
	case ld_sp_word:  SP.b.l = read_mem(_rPC++); SP.b.h = read_mem(_rPC++); break;
	case nop:         break;
	case or_a:        OR(_rA); break;
	case or_b:        OR(_rB); break;
	case or_byte:     OR(read_mem(_rPC++)); break;
	case or_c:        OR(_rC); break;
	case or_d:        OR(_rD); break;
	case or_e:        OR(_rE); break;
	case or_h:        OR(_rIYh); break;
	case or_l:        OR(_rIYl); break;
	case or_mhl:      { signed char o = read_mem(_rPC++); OR(read_mem(_rIY+o)); WZ.w.l=_rIY+o;  } break;
	case outa:        { z80_wait_states iCycleCount = Oa_;} { WZ.b.l = read_mem(_rPC++); WZ.b.h = _rA; z80_OUT_handler(WZ, _rA); WZ.b.l++; } break;
	case pfx_cb:      z80_pfx_fdcb(); break;
	case pfx_dd:      z80_pfx_dd(); break;
	case pfx_ed:      z80_pfx_ed(); break;
	case pfx_fd:      z80_pfx_fd(); break;
	case pop_af:      POP(AF); break;
	case pop_bc:      POP(BC); break;
	case pop_de:      POP(DE); break;
	case pop_hl:      POP(IY); break;
	case push_af:     PUSH(AF); break;
	case push_bc:     PUSH(BC); break;
	case push_de:     PUSH(DE); break;
	case push_hl:     PUSH(IY); break;
	case ret:         RET; break;
	case ret_c:       if (_rF & Cflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_m:       if (_rF & Sflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_nc:      if (!(_rF & Cflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_nz:      if (!(_rF & Zflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_p:       if (!(_rF & Sflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_pe:      if (_rF & Pflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_po:      if (!(_rF & Pflag)) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case ret_z:       if (_rF & Zflag) { iCycleCount += cc_ex[bOpCode]; RET } else { iWSAdjust++; } ; break;
	case rla:         RLA; break;
	case rlca:        RLCA; break;
	case rra:         RRA; break;
	case rrca:        RRCA; break;
	case rst00:       RST(0x0000); break;
	case rst08:       RST(0x0008); break;
	case rst10:       RST(0x0010); break;
	case rst18:       RST(0x0018); break;
	case rst20:       RST(0x0020); break;
	case rst28:       RST(0x0028); break;
	case rst30:       RST(0x0030); break;
	case rst38:       RST(0x0038); break;
	case sbc_a:       SBC(_rA); break;
	case sbc_b:       SBC(_rB); break;
	case sbc_byte:    SBC(read_mem(_rPC++)); break;
	case sbc_c:       SBC(_rC); break;
	case sbc_d:       SBC(_rD); break;
	case sbc_e:       SBC(_rE); break;
	case sbc_h:       SBC(_rIYh); break;
	case sbc_l:       SBC(_rIYl); break;
	case sbc_mhl:     { signed char o = read_mem(_rPC++); SBC(read_mem(_rIY+o)); WZ.w.l=_rIY+o;  } break;
	case scf:         _rF = (_rF & (Sflag | Zflag | Pflag)) | Cflag | (_rA & Xflags); break;
	case sub_a:       SUB(_rA); break;
	case sub_b:       SUB(_rB); break;
	case sub_byte:    SUB(read_mem(_rPC++)); break;
	case sub_c:       SUB(_rC); break;
	case sub_d:       SUB(_rD); break;
	case sub_e:       SUB(_rE); break;
	case sub_h:       SUB(_rIYh); break;
	case sub_l:       SUB(_rIYl); break;
	case sub_mhl:     { signed char o = read_mem(_rPC++); SUB(read_mem(_rIY+o)); WZ.w.l=_rIY+o;  } break;
	case xor_a:       XOR(_rA); break;
	case xor_b:       XOR(_rB); break;
	case xor_byte:    XOR(read_mem(_rPC++)); break;
	case xor_c:       XOR(_rC); break;
	case xor_d:       XOR(_rD); break;
	case xor_e:       XOR(_rE); break;
	case xor_h:       XOR(_rIYh); break;
	case xor_l:       XOR(_rIYl); break;
	case xor_mhl:     { signed char o = read_mem(_rPC++); XOR(read_mem(_rIY+o)); WZ.w.l=_rIY+o;  } break;
   }
}



void t_z80regs::z80_pfx_fdcb(void)
{
	signed char o;
	byte bOpCode;

	o = read_mem(_rPC++); // offset
	bOpCode = read_mem(_rPC++);
	iCycleCount += cc_xycb[bOpCode];
	 WZ.w.l=_rIY+o; 
	switch(bOpCode)
	{
	case bit0_a:      BIT_XY(0, read_mem(_rIY+o)); break;
	case bit0_b:      BIT_XY(0, read_mem(_rIY+o)); break;
	case bit0_c:      BIT_XY(0, read_mem(_rIY+o)); break;
	case bit0_d:      BIT_XY(0, read_mem(_rIY+o)); break;
	case bit0_e:      BIT_XY(0, read_mem(_rIY+o)); break;
	case bit0_h:      BIT_XY(0, read_mem(_rIY+o)); break;
	case bit0_l:      BIT_XY(0, read_mem(_rIY+o)); break;
	case bit0_mhl:    BIT_HL(0, read_mem(_rIY+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit1_a:      BIT_XY(1, read_mem(_rIY+o)); break;
	case bit1_b:      BIT_XY(1, read_mem(_rIY+o)); break;
	case bit1_c:      BIT_XY(1, read_mem(_rIY+o)); break;
	case bit1_d:      BIT_XY(1, read_mem(_rIY+o)); break;
	case bit1_e:      BIT_XY(1, read_mem(_rIY+o)); break;
	case bit1_h:      BIT_XY(1, read_mem(_rIY+o)); break;
	case bit1_l:      BIT_XY(1, read_mem(_rIY+o)); break;
	case bit1_mhl:    BIT_HL(1, read_mem(_rIY+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit2_a:      BIT_XY(2, read_mem(_rIY+o)); break;
	case bit2_b:      BIT_XY(2, read_mem(_rIY+o)); break;
	case bit2_c:      BIT_XY(2, read_mem(_rIY+o)); break;
	case bit2_d:      BIT_XY(2, read_mem(_rIY+o)); break;
	case bit2_e:      BIT_XY(2, read_mem(_rIY+o)); break;
	case bit2_h:      BIT_XY(2, read_mem(_rIY+o)); break;
	case bit2_l:      BIT_XY(2, read_mem(_rIY+o)); break;
	case bit2_mhl:    BIT_HL(2, read_mem(_rIY+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit3_a:      BIT_XY(3, read_mem(_rIY+o)); break;
	case bit3_b:      BIT_XY(3, read_mem(_rIY+o)); break;
	case bit3_c:      BIT_XY(3, read_mem(_rIY+o)); break;
	case bit3_d:      BIT_XY(3, read_mem(_rIY+o)); break;
	case bit3_e:      BIT_XY(3, read_mem(_rIY+o)); break;
	case bit3_h:      BIT_XY(3, read_mem(_rIY+o)); break;
	case bit3_l:      BIT_XY(3, read_mem(_rIY+o)); break;
	case bit3_mhl:    BIT_HL(3, read_mem(_rIY+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit4_a:      BIT_XY(4, read_mem(_rIY+o)); break;
	case bit4_b:      BIT_XY(4, read_mem(_rIY+o)); break;
	case bit4_c:      BIT_XY(4, read_mem(_rIY+o)); break;
	case bit4_d:      BIT_XY(4, read_mem(_rIY+o)); break;
	case bit4_e:      BIT_XY(4, read_mem(_rIY+o)); break;
	case bit4_h:      BIT_XY(4, read_mem(_rIY+o)); break;
	case bit4_l:      BIT_XY(4, read_mem(_rIY+o)); break;
	case bit4_mhl:    BIT_HL(4, read_mem(_rIY+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit5_a:      BIT_XY(5, read_mem(_rIY+o)); break;
	case bit5_b:      BIT_XY(5, read_mem(_rIY+o)); break;
	case bit5_c:      BIT_XY(5, read_mem(_rIY+o)); break;
	case bit5_d:      BIT_XY(5, read_mem(_rIY+o)); break;
	case bit5_e:      BIT_XY(5, read_mem(_rIY+o)); break;
	case bit5_h:      BIT_XY(5, read_mem(_rIY+o)); break;
	case bit5_l:      BIT_XY(5, read_mem(_rIY+o)); break;
	case bit5_mhl:    BIT_HL(5, read_mem(_rIY+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit6_a:      BIT_XY(6, read_mem(_rIY+o)); break;
	case bit6_b:      BIT_XY(6, read_mem(_rIY+o)); break;
	case bit6_c:      BIT_XY(6, read_mem(_rIY+o)); break;
	case bit6_d:      BIT_XY(6, read_mem(_rIY+o)); break;
	case bit6_e:      BIT_XY(6, read_mem(_rIY+o)); break;
	case bit6_h:      BIT_XY(6, read_mem(_rIY+o)); break;
	case bit6_l:      BIT_XY(6, read_mem(_rIY+o)); break;
	case bit6_mhl:    BIT_HL(6, read_mem(_rIY+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case bit7_a:      BIT_XY(7, read_mem(_rIY+o)); break;
	case bit7_b:      BIT_XY(7, read_mem(_rIY+o)); break;
	case bit7_c:      BIT_XY(7, read_mem(_rIY+o)); break;
	case bit7_d:      BIT_XY(7, read_mem(_rIY+o)); break;
	case bit7_e:      BIT_XY(7, read_mem(_rIY+o)); break;
	case bit7_h:      BIT_XY(7, read_mem(_rIY+o)); break;
	case bit7_l:      BIT_XY(7, read_mem(_rIY+o)); break;
	case bit7_mhl:    BIT_HL(7, read_mem(_rIY+o)); _rF = (_rF & ~Xflags) | (WZ.b.h & Xflags);break;
	case res0_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = RES(0, _rA)); break;
	case res0_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = RES(0, _rB)); break;
	case res0_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = RES(0, _rC)); break;
	case res0_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = RES(0, _rD)); break;
	case res0_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = RES(0, _rE)); break;
	case res0_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = RES(0, _rH)); break;
	case res0_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = RES(0, _rL)); break;
	case res0_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, RES(0, b)); } break;
	case res1_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = RES(1, _rA)); break;
	case res1_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = RES(1, _rB)); break;
	case res1_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = RES(1, _rC)); break;
	case res1_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = RES(1, _rD)); break;
	case res1_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = RES(1, _rE)); break;
	case res1_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = RES(1, _rH)); break;
	case res1_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = RES(1, _rL)); break;
	case res1_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, RES(1, b)); } break;
	case res2_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = RES(2, _rA)); break;
	case res2_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = RES(2, _rB)); break;
	case res2_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = RES(2, _rC)); break;
	case res2_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = RES(2, _rD)); break;
	case res2_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = RES(2, _rE)); break;
	case res2_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = RES(2, _rH)); break;
	case res2_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = RES(2, _rL)); break;
	case res2_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, RES(2, b)); } break;
	case res3_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = RES(3, _rA)); break;
	case res3_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = RES(3, _rB)); break;
	case res3_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = RES(3, _rC)); break;
	case res3_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = RES(3, _rD)); break;
	case res3_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = RES(3, _rE)); break;
	case res3_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = RES(3, _rH)); break;
	case res3_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = RES(3, _rL)); break;
	case res3_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, RES(3, b)); } break;
	case res4_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = RES(4, _rA)); break;
	case res4_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = RES(4, _rB)); break;
	case res4_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = RES(4, _rC)); break;
	case res4_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = RES(4, _rD)); break;
	case res4_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = RES(4, _rE)); break;
	case res4_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = RES(4, _rH)); break;
	case res4_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = RES(4, _rL)); break;
	case res4_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, RES(4, b)); } break;
	case res5_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = RES(5, _rA)); break;
	case res5_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = RES(5, _rB)); break;
	case res5_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = RES(5, _rC)); break;
	case res5_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = RES(5, _rD)); break;
	case res5_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = RES(5, _rE)); break;
	case res5_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = RES(5, _rH)); break;
	case res5_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = RES(5, _rL)); break;
	case res5_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, RES(5, b)); } break;
	case res6_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = RES(6, _rA)); break;
	case res6_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = RES(6, _rB)); break;
	case res6_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = RES(6, _rC)); break;
	case res6_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = RES(6, _rD)); break;
	case res6_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = RES(6, _rE)); break;
	case res6_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = RES(6, _rH)); break;
	case res6_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = RES(6, _rL)); break;
	case res6_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, RES(6, b)); } break;
	case res7_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = RES(7, _rA)); break;
	case res7_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = RES(7, _rB)); break;
	case res7_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = RES(7, _rC)); break;
	case res7_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = RES(7, _rD)); break;
	case res7_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = RES(7, _rE)); break;
	case res7_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = RES(7, _rH)); break;
	case res7_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = RES(7, _rL)); break;
	case res7_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, RES(7, b)); } break;
	case rlc_a:       _rA = read_mem(_rIY+o); _rA = RLC(_rA); write_mem(_rIY+o, _rA); break;
	case rlc_b:       _rB = read_mem(_rIY+o); _rB = RLC(_rB); write_mem(_rIY+o, _rB); break;
	case rlc_c:       _rC = read_mem(_rIY+o); _rC = RLC(_rC); write_mem(_rIY+o, _rC); break;
	case rlc_d:       _rD = read_mem(_rIY+o); _rD = RLC(_rD); write_mem(_rIY+o, _rD); break;
	case rlc_e:       _rE = read_mem(_rIY+o); _rE = RLC(_rE); write_mem(_rIY+o, _rE); break;
	case rlc_h:       _rH = read_mem(_rIY+o); _rH = RLC(_rH); write_mem(_rIY+o, _rH); break;
	case rlc_l:       _rL = read_mem(_rIY+o); _rL = RLC(_rL); write_mem(_rIY+o, _rL); break;
	case rlc_mhl:     { byte b = read_mem(_rIY+o); write_mem(_rIY+o, RLC(b)); } break;
	case rl_a:        _rA = read_mem(_rIY+o); _rA = RL(_rA); write_mem(_rIY+o, _rA); break;
	case rl_b:        _rB = read_mem(_rIY+o); _rB = RL(_rB); write_mem(_rIY+o, _rB); break;
	case rl_c:        _rC = read_mem(_rIY+o); _rC = RL(_rC); write_mem(_rIY+o, _rC); break;
	case rl_d:        _rD = read_mem(_rIY+o); _rD = RL(_rD); write_mem(_rIY+o, _rD); break;
	case rl_e:        _rE = read_mem(_rIY+o); _rE = RL(_rE); write_mem(_rIY+o, _rE); break;
	case rl_h:        _rH = read_mem(_rIY+o); _rH = RL(_rH); write_mem(_rIY+o, _rH); break;
	case rl_l:        _rL = read_mem(_rIY+o); _rL = RL(_rL); write_mem(_rIY+o, _rL); break;
	case rl_mhl:      { byte b = read_mem(_rIY+o); write_mem(_rIY+o, RL(b)); } break;
	case rrc_a:       _rA = read_mem(_rIY+o); _rA = RRC(_rA); write_mem(_rIY+o, _rA); break;
	case rrc_b:       _rB = read_mem(_rIY+o); _rB = RRC(_rB); write_mem(_rIY+o, _rB); break;
	case rrc_c:       _rC = read_mem(_rIY+o); _rC = RRC(_rC); write_mem(_rIY+o, _rC); break;
	case rrc_d:       _rD = read_mem(_rIY+o); _rD = RRC(_rD); write_mem(_rIY+o, _rD); break;
	case rrc_e:       _rE = read_mem(_rIY+o); _rE = RRC(_rE); write_mem(_rIY+o, _rE); break;
	case rrc_h:       _rH = read_mem(_rIY+o); _rH = RRC(_rH); write_mem(_rIY+o, _rH); break;
	case rrc_l:       _rL = read_mem(_rIY+o); _rL = RRC(_rL); write_mem(_rIY+o, _rL); break;
	case rrc_mhl:     { byte b = read_mem(_rIY+o); write_mem(_rIY+o, RRC(b)); } break;
	case rr_a:        _rA = read_mem(_rIY+o); _rA = RR(_rA); write_mem(_rIY+o, _rA); break;
	case rr_b:        _rB = read_mem(_rIY+o); _rB = RR(_rB); write_mem(_rIY+o, _rB); break;
	case rr_c:        _rC = read_mem(_rIY+o); _rC = RR(_rC); write_mem(_rIY+o, _rC); break;
	case rr_d:        _rD = read_mem(_rIY+o); _rD = RR(_rD); write_mem(_rIY+o, _rD); break;
	case rr_e:        _rE = read_mem(_rIY+o); _rE = RR(_rE); write_mem(_rIY+o, _rE); break;
	case rr_h:        _rH = read_mem(_rIY+o); _rH = RR(_rH); write_mem(_rIY+o, _rH); break;
	case rr_l:        _rL = read_mem(_rIY+o); _rL = RR(_rL); write_mem(_rIY+o, _rL); break;
	case rr_mhl:      { byte b = read_mem(_rIY+o); write_mem(_rIY+o, RR(b)); } break;
	case set0_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = SET(0, _rA)); break;
	case set0_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = SET(0, _rB)); break;
	case set0_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = SET(0, _rC)); break;
	case set0_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = SET(0, _rD)); break;
	case set0_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = SET(0, _rE)); break;
	case set0_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = SET(0, _rH)); break;
	case set0_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = SET(0, _rL)); break;
	case set0_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, SET(0, b)); } break;
	case set1_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = SET(1, _rA)); break;
	case set1_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = SET(1, _rB)); break;
	case set1_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = SET(1, _rC)); break;
	case set1_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = SET(1, _rD)); break;
	case set1_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = SET(1, _rE)); break;
	case set1_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = SET(1, _rH)); break;
	case set1_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = SET(1, _rL)); break;
	case set1_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, SET(1, b)); } break;
	case set2_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = SET(2, _rA)); break;
	case set2_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = SET(2, _rB)); break;
	case set2_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = SET(2, _rC)); break;
	case set2_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = SET(2, _rD)); break;
	case set2_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = SET(2, _rE)); break;
	case set2_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = SET(2, _rH)); break;
	case set2_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = SET(2, _rL)); break;
	case set2_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, SET(2, b)); } break;
	case set3_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = SET(3, _rA)); break;
	case set3_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = SET(3, _rB)); break;
	case set3_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = SET(3, _rC)); break;
	case set3_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = SET(3, _rD)); break;
	case set3_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = SET(3, _rE)); break;
	case set3_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = SET(3, _rH)); break;
	case set3_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = SET(3, _rL)); break;
	case set3_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, SET(3, b)); } break;
	case set4_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = SET(4, _rA)); break;
	case set4_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = SET(4, _rB)); break;
	case set4_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = SET(4, _rC)); break;
	case set4_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = SET(4, _rD)); break;
	case set4_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = SET(4, _rE)); break;
	case set4_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = SET(4, _rH)); break;
	case set4_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = SET(4, _rL)); break;
	case set4_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, SET(4, b)); } break;
	case set5_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = SET(5, _rA)); break;
	case set5_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = SET(5, _rB)); break;
	case set5_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = SET(5, _rC)); break;
	case set5_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = SET(5, _rD)); break;
	case set5_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = SET(5, _rE)); break;
	case set5_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = SET(5, _rH)); break;
	case set5_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = SET(5, _rL)); break;
	case set5_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, SET(5, b)); } break;
	case set6_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = SET(6, _rA)); break;
	case set6_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = SET(6, _rB)); break;
	case set6_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = SET(6, _rC)); break;
	case set6_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = SET(6, _rD)); break;
	case set6_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = SET(6, _rE)); break;
	case set6_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = SET(6, _rH)); break;
	case set6_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = SET(6, _rL)); break;
	case set6_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, SET(6, b)); } break;
	case set7_a:      _rA = read_mem(_rIY+o); write_mem(_rIY+o, _rA = SET(7, _rA)); break;
	case set7_b:      _rB = read_mem(_rIY+o); write_mem(_rIY+o, _rB = SET(7, _rB)); break;
	case set7_c:      _rC = read_mem(_rIY+o); write_mem(_rIY+o, _rC = SET(7, _rC)); break;
	case set7_d:      _rD = read_mem(_rIY+o); write_mem(_rIY+o, _rD = SET(7, _rD)); break;
	case set7_e:      _rE = read_mem(_rIY+o); write_mem(_rIY+o, _rE = SET(7, _rE)); break;
	case set7_h:      _rH = read_mem(_rIY+o); write_mem(_rIY+o, _rH = SET(7, _rH)); break;
	case set7_l:      _rL = read_mem(_rIY+o); write_mem(_rIY+o, _rL = SET(7, _rL)); break;
	case set7_mhl:    { byte b = read_mem(_rIY+o); write_mem(_rIY+o, SET(7, b)); } break;
	case sla_a:       _rA = read_mem(_rIY+o); _rA = SLA(_rA); write_mem(_rIY+o, _rA); break;
	case sla_b:       _rB = read_mem(_rIY+o); _rB = SLA(_rB); write_mem(_rIY+o, _rB); break;
	case sla_c:       _rC = read_mem(_rIY+o); _rC = SLA(_rC); write_mem(_rIY+o, _rC); break;
	case sla_d:       _rD = read_mem(_rIY+o); _rD = SLA(_rD); write_mem(_rIY+o, _rD); break;
	case sla_e:       _rE = read_mem(_rIY+o); _rE = SLA(_rE); write_mem(_rIY+o, _rE); break;
	case sla_h:       _rH = read_mem(_rIY+o); _rH = SLA(_rH); write_mem(_rIY+o, _rH); break;
	case sla_l:       _rL = read_mem(_rIY+o); _rL = SLA(_rL); write_mem(_rIY+o, _rL); break;
	case sla_mhl:     { byte b = read_mem(_rIY+o); write_mem(_rIY+o, SLA(b)); } break;
	case sll_a:       _rA = read_mem(_rIY+o); _rA = SLL(_rA); write_mem(_rIY+o, _rA); break;
	case sll_b:       _rB = read_mem(_rIY+o); _rB = SLL(_rB); write_mem(_rIY+o, _rB); break;
	case sll_c:       _rC = read_mem(_rIY+o); _rC = SLL(_rC); write_mem(_rIY+o, _rC); break;
	case sll_d:       _rD = read_mem(_rIY+o); _rD = SLL(_rD); write_mem(_rIY+o, _rD); break;
	case sll_e:       _rE = read_mem(_rIY+o); _rE = SLL(_rE); write_mem(_rIY+o, _rE); break;
	case sll_h:       _rH = read_mem(_rIY+o); _rH = SLL(_rH); write_mem(_rIY+o, _rH); break;
	case sll_l:       _rL = read_mem(_rIY+o); _rL = SLL(_rL); write_mem(_rIY+o, _rL); break;
	case sll_mhl:     { byte b = read_mem(_rIY+o); write_mem(_rIY+o, SLL(b)); } break;
	case sra_a:       _rA = read_mem(_rIY+o); _rA = SRA(_rA); write_mem(_rIY+o, _rA); break;
	case sra_b:       _rB = read_mem(_rIY+o); _rB = SRA(_rB); write_mem(_rIY+o, _rB); break;
	case sra_c:       _rC = read_mem(_rIY+o); _rC = SRA(_rC); write_mem(_rIY+o, _rC); break;
	case sra_d:       _rD = read_mem(_rIY+o); _rD = SRA(_rD); write_mem(_rIY+o, _rD); break;
	case sra_e:       _rE = read_mem(_rIY+o); _rE = SRA(_rE); write_mem(_rIY+o, _rE); break;
	case sra_h:       _rH = read_mem(_rIY+o); _rH = SRA(_rH); write_mem(_rIY+o, _rH); break;
	case sra_l:       _rL = read_mem(_rIY+o); _rL = SRA(_rL); write_mem(_rIY+o, _rL); break;
	case sra_mhl:     { byte b = read_mem(_rIY+o); write_mem(_rIY+o, SRA(b)); } break;
	case srl_a:       _rA = read_mem(_rIY+o); _rA = SRL(_rA); write_mem(_rIY+o, _rA); break;
	case srl_b:       _rB = read_mem(_rIY+o); _rB = SRL(_rB); write_mem(_rIY+o, _rB); break;
	case srl_c:       _rC = read_mem(_rIY+o); _rC = SRL(_rC); write_mem(_rIY+o, _rC); break;
	case srl_d:       _rD = read_mem(_rIY+o); _rD = SRL(_rD); write_mem(_rIY+o, _rD); break;
	case srl_e:       _rE = read_mem(_rIY+o); _rE = SRL(_rE); write_mem(_rIY+o, _rE); break;
	case srl_h:       _rH = read_mem(_rIY+o); _rH = SRL(_rH); write_mem(_rIY+o, _rH); break;
	case srl_l:       _rL = read_mem(_rIY+o); _rL = SRL(_rL); write_mem(_rIY+o, _rL); break;
	case srl_mhl:     { byte b = read_mem(_rIY+o); write_mem(_rIY+o, SRL(b)); } break;
   }
}

//break points code

void t_z80regs::add_break_point(dword adress)
{
  break_points.insert(adress);
}

void t_z80regs::remove_break_point(dword adress)
{
  break_points.erase(adress);
}
