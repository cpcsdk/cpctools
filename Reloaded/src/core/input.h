//
// Caprice32 input manager
//

#ifndef _INPUT_H_
#define _INPUT_H_

#include <map>

#include "cap32type.h"
class t_CPC;

#define MOD_CPC_SHIFT   (0x01 << 8)
#define MOD_CPC_CTRL    (0x02 << 8)
#define MOD_EMU_KEY     (0x10 << 8)

#define MOD_PC_SHIFT    /*wxMOD_SHIFT */(KMOD_SHIFT << 16)
#define MOD_PC_CTRL     /*wxMOD_CONTROL */(KMOD_CTRL << 16)
#define MOD_PC_MODE     /*wxMOD_ALT */(KMOD_MODE << 16)

#define KBD_MAX_ENTRIES 160

typedef enum {
	CAP32_EXIT = MOD_EMU_KEY,
	CAP32_FPS,
	CAP32_FULLSCRN,
	CAP32_JOY,
	CAP32_LOADDRVA,
	CAP32_LOADDRVB,
	CAP32_LOADSNAP,
	CAP32_LOADTAPE,
	CAP32_MF2RESET,
	CAP32_MF2STOP,
	CAP32_OPTIONS,
	CAP32_PAUSE,
	CAP32_RESET,
	CAP32_SAVESNAP,
	CAP32_SCRNSHOT,
	CAP32_SPEED,
	CAP32_TAPEPLAY,
	CAP32_DEBUG
} CAP32_KEYS;

class t_Input
{
private:
	//! CPC keyboard layout (English, French, Spanish)
	static dword		cpc_kbd[86];
	static int			joy_layout[12][2];
public:
	byte				keyboard_matrix[16];
	std::map<unsigned int,dword> keyboard_normal;
	std::map<unsigned int,dword> keyboard_shift;
	std::map<unsigned int,dword> keyboard_ctrl;
	std::map<unsigned int,dword> keyboard_mode;
public:
	
	void Reset();

	void input_swap_joy (t_CPC &CPC);
	int input_init (t_CPC &CPC);	
        void setupKey(int,int,int=-1,int=-1,int=-1);
};

#endif
