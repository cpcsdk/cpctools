//
// Caprice32 audio PSG emulator
//

#ifndef _PSG_H_
#define _PSG_H_

#include <memory>
#include "cap32type.h"

#ifdef ST_SOUND
#include "Ym2149Ex.h"
#endif

#ifdef AYEMU
#include "ayemu.h"
#endif

using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

class Emulator;

class t_PSG
{
private:
	unsigned char reg_select;
	unsigned char control;

#ifdef ST_SOUND
	unique_ptr<CYm2149Ex> m_Ym2149;
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
	t_PSG(shared_ptr<Emulator> emulator);
	~t_PSG();

	void Emulate(int iCycleCount);

	void Init(int enableSound);

	void Reset();

	unsigned char GetAYRegister(int Num) const
	{
#ifdef ST_SOUND
		return m_Ym2149->readRegister(Num);
#endif
#ifdef AYLET
		return sound_ay_read(Num);
#endif
#ifdef AYEMU
		return m_ayemu_reg_frame[Num];
#endif
	}

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
	weak_ptr<Emulator> _emulator;
};

#endif
