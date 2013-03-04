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

#ifndef Z80_H
#define Z80_H


#include <set>
#include <memory>

#include "cap32type.h"
#include "ioport.h"
#include "memory.h"

#define Sflag  0x80 // sign flag
#define Zflag  0x40 // zero flag
#define Hflag  0x10 // halfcarry flag
#define Pflag  0x04 // parity flag
#define Vflag  0x04 // overflow flag
#define Nflag  0x02 // negative flag
#define Cflag  0x01 // carry flag
#define Xflags 0x28 // bit 5 & 3 flags

#define _rA        AF.b.h
#define _rF        AF.b.l
#define _rAF       AF.w.l
#define _rAFdword  AF.d
#define _rB        BC.b.h
#define _rC        BC.b.l
#define _rBC       BC.w.l
#define _rBCdword  BC.d
#define _rD        DE.b.h
#define _rE        DE.b.l
#define _rDE       DE.w.l
#define _rDEdword  DE.d
#define _rH        HL.b.h
#define _rL        HL.b.l
#define _rHL       HL.w.l
#define _rHLdword  HL.d
#define _rPC       PC.w.l
#define _rPCdword  PC.d
#define _rSP       SP.w.l

#define _rIXh      IX.b.h
#define _rIXl      IX.b.l
#define _rIX       IX.w.l
#define _rIXdword  IX.d
#define _rIYh      IY.b.h
#define _rIYl      IY.b.l
#define _rIY       IY.w.l
#define _rIYdword  IY.d

#define _rI        I
#define _rR        R
#define _rRb7      Rb7
#define _rIFF1     IFF1
#define _rIFF2     IFF2
#define _rIM       IM
#define _rHALT     HALT

#define EC_BREAKPOINT      10
#define EC_TRACE           20
#define EC_FRAME_COMPLETE  30
#define EC_CYCLE_COUNT     40
#define EC_SOUND_BUFFER    50

using std::weak_ptr;

class t_GateArray;
class t_VDU;
class t_PSG;
class t_CPC;
class Emulator;

class t_z80regs
{
private:
	static byte SZ[256]; // zero and sign flags
	static byte SZ_BIT[256]; // zero, sign and parity/overflow (=zero) flags for BIT opcode
	static byte SZP[256]; // zero, sign and parity flags
	static byte SZHV_inc[256]; // zero, sign, half carry and overflow flags INC r8
	static byte SZHV_dec[256]; // zero, sign, half carry and overflow flags DEC r8

private:
	int iCycleCount, iWSAdjust;

	weak_ptr<Emulator> emulator;
	IOPort			_ioPort;
	shared_ptr<t_VDU>			VDU;
	shared_ptr<t_PSG>			PSG;
	shared_ptr<t_Memory>		Memory;
	shared_ptr<t_CPC>			CPC;

  /**
   * List of adresses where to break
   */
  std::set<dword> break_points;
  /**
   * Flag to know if the memory have already been blocked
   */
  bool adressAlreadyBlocked;

public:
	reg_pair AF;
	reg_pair BC;
	reg_pair DE;
	reg_pair HL;
	reg_pair PC;
	reg_pair SP;
	reg_pair AFx;
	reg_pair BCx;
	reg_pair DEx;
	reg_pair HLx;
	reg_pair IX;
	reg_pair IY;
	reg_pair WZ;  // The internal "work" register used by the z80

	byte I;
	byte R;
	byte Rb7;
	byte IFF1;
	byte IFF2;
	byte IM;
	byte HALT;
	byte EI_issued;
	byte int_pending;
	dword trace;

public:
	t_z80regs(shared_ptr<Emulator> emulator);

	void reset();

	inline byte read_mem(word addr) 
	{
		return Memory->Read(addr);
	}

	inline void write_mem(word addr, byte val) 
	{
		Memory->Write(addr, val);
	}

	inline byte z80_IN_handler(reg_pair port)
	{
		return _ioPort.z80_IN_handler(port);
	}
	inline void z80_OUT_handler(reg_pair port, byte val)
	{
		_ioPort.z80_OUT_handler(port, val);
	}

    inline std::set<dword> GetBreakpoints() const
    {
        return break_points;
    }

	void z80_init_tables(void);
	void z80_mf2stop(void);
	int z80_execute(void);
	void z80_pfx_cb(void);
	void z80_pfx_dd(void);
	void z80_pfx_ddcb(void);
	void z80_pfx_ed(void);
	void z80_pfx_fd(void);
	void z80_pfx_fdcb(void);

  //Break points managment
  /**
   * Add a break point to the emulator
   * @param adress Adress where to break
   */
  void add_break_point(dword adress);
  /**
   * Remove a break point to the emulator
   * @param adress Adress where to remove
   */
  void remove_break_point(dword adress);

protected:
	inline byte RES(byte bit, byte val) const
	{
		return val & ~(1 << bit);
	}

	inline byte RLC(byte val)
	{
		unsigned res = val;
		unsigned carry = (res & 0x80) ? Cflag : 0;
		res = ((res << 1) | (res >> 7)) & 0xff;
		_rF = SZP[res] | carry;
		return res;
	}

	inline byte RL(byte val)
	{
		unsigned res = val;
		unsigned carry = (res & 0x80) ? Cflag : 0;
		res = ((res << 1) | (_rF & Cflag)) & 0xff;
		_rF = SZP[res] | carry;
		return res;
	}

	inline byte RRC(byte val)
	{
		unsigned res = val;
		unsigned carry = (res & 0x01) ? Cflag : 0;
		res = ((res >> 1) | (res << 7)) & 0xff;
		_rF = SZP[res] | carry;
		return res;
	}

	inline byte RR(byte val)
	{
		unsigned res = val;
		unsigned carry = (res & 0x01) ? Cflag : 0;
		res = ((res >> 1) | (_rF << 7)) & 0xff;
		_rF = SZP[res] | carry;
		return res;
	}

	inline byte SET(byte bit, byte val)
	{
		return val | (1 << bit);
	}

	inline byte SLA(byte val)
	{
		unsigned res = val;
		unsigned carry = (res & 0x80) ? Cflag : 0;
		res = (res << 1) & 0xff;
		_rF = SZP[res] | carry;
		return res;
	}

	inline byte SLL(byte val)
	{
		unsigned res = val;
		unsigned carry = (res & 0x80) ? Cflag : 0;
		res = ((res << 1) | 0x01) & 0xff;
		_rF = SZP[res] | carry;
		return res;
	}

	inline byte SRA(byte val)
	{
		unsigned res = val;
		unsigned carry = (res & 0x01) ? Cflag : 0;
		res = ((res >> 1) | (res & 0x80)) & 0xff;
		_rF = SZP[res] | carry;
		return res;
	}

	inline byte SRL(byte val)
	{
		unsigned res = val;
		unsigned carry = (res & 0x01) ? Cflag : 0;
		res = (res >> 1) & 0xff;
		_rF = SZP[res] | carry;
		return res;
	}
};

#endif

