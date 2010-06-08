//
// Caprice32 audio PSG emulator
//

#ifndef _PSG_H_
#define _PSG_H_

#include "configBis.h"

#include "cap32type.h"

class t_CPC;
class t_Tape;

#ifdef ST_SOUND
#include "Ym2149Ex.h"
#endif

#ifdef AYEMU
#include "ayemu.h"
#endif

class t_PSG
{
private:
	t_CPC&		CPC;
	t_Tape&		Tape;
	unsigned char reg_select;
	unsigned char control;

	bool isInit;

#ifdef ST_SOUND
    CYm2149Ex* m_Ym2149;
#endif
#ifdef AYEMU
    ayemu_ay_t m_ayemu ;
    ayemu_ay_reg_frame_t m_ayemu_reg_frame ;
#endif
	double cycle_count;

	unsigned int buffer_full;
	int AmplitudeEnv;
	bool FirstPeriod;

public:
	double snd_cycle_count;
	t_PSG(t_CPC &cpc, t_Tape &tape);
	~t_PSG();

	void Emulate(int iCycleCount);

	void Init(int enableSound);

	void Reset();

	unsigned char GetAYRegister(int Num);
	void SetAYRegister(int Num, unsigned char Value);
    void fillSample(int nbSample);

	inline void psg_write(unsigned char psg_data)
	{
		// isolate PSG control bits
		byte ctrl = control & 0xc0;
		// latch address?
		if (ctrl == 0xc0)
		{
			// select new PSG register
			reg_select = psg_data;
		}
		// write?
		else if (ctrl == 0x80)
		{
			// valid register?
			if (reg_select < 16)
			{
				SetAYRegister(reg_select, psg_data);
			}
		}
	}


	void					SetEnvDirection(unsigned char dir);
	unsigned char GetEnvDirection() const;

	inline unsigned char	GetControl() const					{ return control;		}
	inline void				SetControl(unsigned char r)			{ control = r;			}
	inline unsigned char	GetRegSelect() const				{ return reg_select;	}
	inline void				SetRegSelect(unsigned char r)		{ reg_select = r;		}
	inline unsigned int		GetBufferFull() const				{ return buffer_full;	}
	inline void				SetBufferFull(unsigned int b)		{ buffer_full = b;		}
	inline int				GetAmplitudeEnv() const				{ return AmplitudeEnv;	}
	inline void				SetAmplitudeEnv(int a)				{ AmplitudeEnv = a;		}
	inline bool				GetFirstPeriod() const				{ return FirstPeriod;	}
	inline void				SetFirstPeriod(bool f)				{ FirstPeriod = f;		}

#ifdef AYEMU
	inline ayemu_ay_t &GetAYEmu(){ return m_ayemu; }
	inline ayemu_ay_reg_frame_t &GetAYFrame(){return m_ayemu_reg_frame;}
#endif
private:
	void InitAYCounterVars();
};

#endif
