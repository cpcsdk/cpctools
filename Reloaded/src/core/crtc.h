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

#ifndef CRTC_H
#define CRTC_H

#include <memory>
#include "cap32type.h"

using std::shared_ptr;

class t_GateArray;
class t_VDU;

class t_CRTC
{
	protected:
		unsigned char	_regSelect; // Selected register
		unsigned char	_registers[18]; // Register file

		// Crtc1 VBL duration
		virtual void setReg3(unsigned char val);
		unsigned int	_hsw; // HSync width
		unsigned int	_vsw; // VSync width

		bool			_inVSync; // True if we are inside the VSync

		// Crtc1 unbuffered R12 and 13 during 1st block
		virtual void setReg12(unsigned char val);
		virtual void setReg13(unsigned char val);
		unsigned int	_lineCount;
		unsigned int	_requestedAddr;
		unsigned int	_nextAddr;
		unsigned int	_addr;

		// Crtc1 unbuffered R4
		virtual void setReg4(unsigned char val);
		bool		_startVerticalTotalAdjust;

		// Crtc 1 split border
		virtual void setReg6(unsigned char val);

		virtual void setReg9(unsigned char val);
		unsigned int	_rasterCount;
		unsigned int	_r9Match;
		bool			_resScan;
		unsigned int	_charCount;
		void			(*_charInstMR)(t_CRTC &CRTC);
		bool			_resChar;
		bool			_inVerticalTotalAdjust;

	// The next 4 bytes must remain together
	typedef union 
	{
		dword combined;
		struct 
		{
			byte monVSYNC;
			byte inHSYNC;
			union 
			{
				word combined;
				struct 
				{
					byte DISPTIMG;
					byte HDSPTIMG;
				};
			} dt;
		};
	} t_flags1;

	// The next two bytes must remain together
	typedef union 
	{
		word combined;
		struct 
		{
			byte NewDISPTIMG;
			byte NewHDSPTIMG;
		};
	} t_new_dt;

	shared_ptr<t_VDU>			_vdu;
	t_flags1		_flags1;
	t_new_dt		_newDT;
	unsigned int	_nextAddress;
	unsigned int	_scrBase;
	word			_maxLate[0x7400];
	unsigned int	_lastHEnd;
	bool			_hadHSync;
	bool			_newScan;
	bool			_resFrame;
	bool			_resNext;
	bool			_resVSync;
	unsigned int	_reg5;
	unsigned int	_hswCount;
	unsigned int	_vswCount;
	unsigned int	_hStart;
	unsigned int	_hEnd;

	void			(*_charInstSL)(t_CRTC &CRTC);
private:
	void dispRegs();
	
	shared_ptr<t_GateArray>	 _gateArray;
	
	
	bool			_inMonHSync;
	unsigned int	_r7Match;

public:
	t_CRTC(shared_ptr<t_GateArray> ga, shared_ptr<t_VDU> vdu);
	virtual ~t_CRTC() {};

	void Reset();
	
	virtual void Emulate(int repeat_count);
	
	void RegisterSelect(unsigned char val);
	unsigned char GetRegisterSelect() const;
	
	void WriteData(unsigned char val);

	virtual unsigned char ReadData() const;
	virtual unsigned char ReadStatus() const {return 255;}; //Hi-Z, should be random
	
	unsigned char GetRegisterValue(unsigned char reg) const;
	void SetRegisterValue(unsigned char reg, unsigned char val);
	
	inline unsigned int GetAddr() const				{ return _addr;						}
	inline void SetAddr(unsigned int v)				{ _addr = v;						}
	inline unsigned int GetCharCount() const		{ return _charCount;				}
	inline void SetCharCount(unsigned int v)		{ _charCount = v;					}
	inline unsigned int GetLineCount() const		{ return _lineCount;				}
	inline void SetLineCount(unsigned int v)		{ _lineCount = v;					}
	inline unsigned int GetRasterCount() const		{ return _rasterCount;				}
	inline void SetRasterCount(unsigned int v)		{ _rasterCount = v;					}
	inline unsigned int GetHswCount() const			{ return _hswCount;					}
	inline void SetHswCount(unsigned int v)			{ _hswCount = v;					}
	inline unsigned int GetVswCount() const			{ return _vswCount;					}
	inline void SetVswCount(unsigned int v)			{ _vswCount = v;					}
	inline bool GetFlagInVSync() const				{ return _inVSync;					}
	inline void SetFlagInVSync(bool v)				{ _inVSync = v;						}
	inline bool GetFlagHadHSync() const				{ return _hadHSync;					}
	inline void SetFlagHadHSync(bool v)				{ _hadHSync = v;					}
	inline bool GetFlagInMonHSync() const			{ return _inMonHSync;				}
	inline void SetFlagInMonHSync(bool v)			{ _inMonHSync = v;					}
	inline bool GetFlagInVta() const				{ return _inVerticalTotalAdjust;	}
	inline void SetFlagInVta(bool v)				{ _inVerticalTotalAdjust = v;		}
	
	inline unsigned int GetNextAddress() const		{ return _nextAddress;				}
	inline void SetNextAddress(unsigned int v)		{ _nextAddress = v;					}
																						
	inline byte GetFlag1InHSync() const				{ return _flags1.inHSYNC;			}
	inline void SetFlag1InHSync(byte v)				{ _flags1.inHSYNC = v;				}

protected:
	void UpdateSkew();
	void MatchLineCount();
	void ReloadAddr();
	void RestartFrame();
	void ChangeMode();
	void MatchHsw();

	void DisplayDebug() const;
	
protected:
	inline static void NoChar(t_CRTC &CRTC) {/* Nothing */}
	
	static void CharSL2(t_CRTC &CRTC);
	static void CharSL1(t_CRTC &CRTC);
	
	static void CharMR1(t_CRTC &CRTC);
	static void CharMR2(t_CRTC &CRTC);
};

#endif
