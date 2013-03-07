//
// Caprice32 GateArray emulator
//

#ifndef _GATEARRAY_H_
#define _GATEARRAY_H_

#include <cstdlib>
#include <memory>

using std::shared_ptr;
using std::weak_ptr;

class Renderer;
class t_z80regs;

class t_GateArray
{
private:
	weak_ptr<t_z80regs> _z80;
	Renderer	& _renderer;
	
	//! Requested mode (will be active on next HSync)
	unsigned int requested_scr_mode;
	//! Current mode
	unsigned int scr_mode;
	
	//! Current pen
	unsigned char pen;
	//! Current ink value
	unsigned char ink_values[17];
	
	//! HSync counter
	unsigned int hs_count;
	//! Scanline counter
	unsigned char sl_count;
	unsigned char int_delay;

public:
	//! Default constructor
	t_GateArray(Renderer & render, shared_ptr<t_z80regs> z80 = NULL);

	//! Reset Gate Array
	void Reset(int mode = 1);

	//! Return current mode
	inline unsigned int GetMode()	const					{ return scr_mode;			}
	//! Set mode (request)
	inline void SetMode(unsigned int v)						{ requested_scr_mode = v;	}
															
	inline unsigned char GetPen() const						{ return pen;				}
	inline unsigned char GetInk(unsigned int i) const		{ return ink_values[i];		}
	void SetInk(unsigned int i, unsigned char v);

	inline unsigned int GetHSCount() const					{ return hs_count;			}
	inline void SetHSCount(unsigned int v)					{ hs_count = v;				}
	inline unsigned int GetSLCount() const					{ return sl_count;			}
	inline void SetSLCount(unsigned int v)					{ sl_count = v;				}
							
	// Accessed by IO
	//! Select pen
	inline void SelectPen(unsigned char v)					{ pen = v;					}
	//! Set color (handle video palette update, with antialiasing)
	void SetColour(unsigned char col);
	
	//! Match Horizontal Sync Width
	void MatchHsw(void);

	//! Change mode on HSync
	void HSyncChangeMode();
};

#endif
