//
// Caprice32 input manager
//

#ifndef _INPUT_H_
#define _INPUT_H_

#include <memory>
#include <map>

#include "cap32type.h"

using std::shared_ptr;

class t_CPC;

#define MOD_CPC_SHIFT   (0x01 << 8)
#define MOD_CPC_CTRL    (0x02 << 8)
#define MOD_EMU_KEY     (0x10 << 8)

#define MOD_PC_SHIFT    /*wxMOD_SHIFT */(KMOD_SHIFT << 16)
#define MOD_PC_CTRL     /*wxMOD_CONTROL */(KMOD_CTRL << 16)
#define MOD_PC_MODE     /*wxMOD_ALT */(KMOD_MODE << 16)

#define KBD_MAX_ENTRIES 160

class t_Input
{
public:
	t_Input(shared_ptr<t_CPC> config);
	void input_swap_joy (shared_ptr<t_CPC> config);
	void setupKey(int,int,int=-1,int=-1,int=-1);
	void Reset();
public:
	byte				keyboard_matrix[16];
	std::map<unsigned int,dword> keyboard_normal;
	std::map<unsigned int,dword> keyboard_shift;
	std::map<unsigned int,dword> keyboard_ctrl;
	std::map<unsigned int,dword> keyboard_mode;
private:
	//! CPC keyboard layout (English, French, Spanish)
	static dword		cpc_kbd[86];
	static int			joy_layout[12][2];
};

#endif
