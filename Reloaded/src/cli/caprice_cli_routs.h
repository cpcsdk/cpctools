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


//TODO: Move in the core this file in order to be also used by gui

#ifndef __CAPRICE_CLI_ROUTS_H__
#define __CAPRICE_CLI_ROUTS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Give informations about registers
 */
void caprice_cli_show_registers();
/**
 * Reset emulator
 */
void caprice_cli_emu_reset();
/**
 * Add a breakpoint
 */
void caprice_cli_add_breakpoint(int adress);
/**
 * Remove a breakpoint
 */
void caprice_cli_remove_breakpoint(int adress);
/**
 * List all the breakpoints
 */
void caprice_cli_show_breakpoints();
/**
 * Continue the flow execution after a breakpoint
 */
void caprice_cli_continue();
void caprice_cli_step();
/**
 * Choose the video color mode
 * @param mode 1 fo color, 2 for gray, 3 for green
 */
void caprice_cli_video_color(int mode);
/**
 * Poke a byte or word in memory
 */
void caprice_cli_memory_poke(int address, int value);
/**
 * Peek a byte in memory
 */
void caprice_cli_memory_peek(int address);
/**
 * Disassemble memory and show it on screen
 * TODO: use a pager
 */
void caprice_cli_memory_disasm(int address);
/**
 * Disassemble memory until attended quantity attended
 */
void caprice_cli_memory_disasm_quantity(int address, int size);
void caprice_cli_memory_disasm_pc(int size);
/**
 * Compile file
 */
void caprice_cli_asm_compile_file(char *filename);
/**
 * Jump at the attended address
 */
void caprice_cli_asm_launch(int address);
/**
 * Save screenshot
 */
void caprice_cli_screenshot_save();
#ifdef __cplusplus
}
#endif

#endif /* __CAPRICE_CLI_ROUTS_H__ */
