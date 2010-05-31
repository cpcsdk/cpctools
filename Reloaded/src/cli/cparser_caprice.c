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


#include "cparser.h"
#include "cparser_token.h"

#include "caprice_cli_routs.h"
typedef unsigned char byte;

/**
 * Quit the application
 * @todo clean before quit
 */
cparser_result_t
cparser_cmd_quit(cparser_context_t *context)
{
  return cparser_quit(context->parser);
}


/**
 * Shows registers on screen
 */
cparser_result_t
cparser_cmd_show_registers(cparser_context_t *context)
{
  caprice_cli_show_registers();
  return CPARSER_OK;
}

cparser_result_t
cparser_cmd_show_registers_crtc(cparser_context_t *context)
{
  return CPARSER_OK;
}

cparser_result_t
cparser_cmd_show_registers_ga(cparser_context_t *context)
{
  return CPARSER_OK;
}

cparser_result_t
cparser_cmd_show_registers_z80(cparser_context_t *context)
{
  return CPARSER_OK;
}

//Breakpoints
cparser_result_t
cparser_cmd_breakpoints_add_adress(cparser_context_t *context, uint32_t *adress_ptr)
{
  caprice_cli_add_breakpoint(*adress_ptr);
  return CPARSER_OK;
}

cparser_result_t
cparser_cmd_breakpoints_remove_adress(cparser_context_t *context, uint32_t *adress_ptr)
{
  caprice_cli_remove_breakpoint(*adress_ptr);
  return CPARSER_OK;
}

cparser_result_t
cparser_cmd_breakpoints_list(cparser_context_t *context)
{
  caprice_cli_show_breakpoints();
  return CPARSER_OK;
}

cparser_result_t
cparser_cmd_step(cparser_context_t * context)
{
  caprice_cli_step();
  return CPARSER_OK;
}

cparser_result_t
cparser_cmd_continue(cparser_context_t * context)
{
  caprice_cli_continue();
  return CPARSER_OK;
}
//Video
cparser_result_t
cparser_cmd_video_color_on(cparser_context_t *context)
{
  caprice_cli_video_color(1);
  return CPARSER_OK;
}

cparser_result_t
cparser_cmd_video_color_grey(cparser_context_t *context)
{
  caprice_cli_video_color(2);
  return CPARSER_OK;
}

cparser_result_t
cparser_cmd_video_color_green(cparser_context_t *context)
{
  caprice_cli_video_color(3);
  return CPARSER_OK;
}

//Memory
cparser_result_t
cparser_cmd_memory_peek_address(cparser_context_t *context, uint32_t *address_ptr)
{
  caprice_cli_memory_peek(*address_ptr & 0xffff);
  return CPARSER_OK;
}

cparser_result_t
cparser_cmd_memory_poke_address_value(cparser_context_t *context, uint32_t *address_ptr, uint32_t *value_ptr)
{
  caprice_cli_memory_poke(*address_ptr & 0xffff, *value_ptr & 0xffff);
  return CPARSER_OK;
}

//BUG memory disassemble do not work ...
cparser_result_t
cparser_cmd_memory_disassemble_address_quantity(  cparser_context_t *context,
                                                  uint32_t *address_ptr, uint32_t *size_ptr)
{

  int adr  = (address_ptr ? *address_ptr & 0xffff: -1);
  int size = (size_ptr    ? *size_ptr : 0x0);

  if (size == 0)
  {
    if (adr == -1)
    {
      return CPARSER_OK ;
      caprice_cli_memory_disasm_pc(5);
    }
    else
    {
      caprice_cli_memory_disasm( adr );
    }
  }
  else
  {
    caprice_cli_memory_disasm_quantity( adr, size & 0xffff);
  }

  return CPARSER_OK ;
}

cparser_result_t
cparser_cmd_memory_disassemble_pc_quantity(cparser_context_t *context, uint32_t *size_ptr)
{

  int size = (size_ptr    ? *size_ptr : 1);
  caprice_cli_memory_disasm_pc(size);
  return CPARSER_OK ;
}
//Emu
cparser_result_t
cparser_cmd_reset(cparser_context_t *context)
{
  caprice_cli_emu_reset() ;
  return CPARSER_OK;
}

//Help
cparser_result_t
cparser_cmd_help_filter (cparser_context_t *context, char **filter)
{
    return cparser_help_cmd(context->parser, filter ? *filter : 0);
}

cparser_result_t
cparser_cmd_emp_help_filter (cparser_context_t *context, char **filter)
{
    return cparser_help_cmd(context->parser, filter ? *filter : 0);
}

//Assembly
cparser_result_t
cparser_cmd_asm_compile_filename ( cparser_context_t *context, char **filename)
{

  caprice_cli_asm_compile_file(*filename);
  return CPARSER_OK ;
}

cparser_result_t
cparser_cmd_asm_launch_address( cparser_context_t *context, uint32_t *address_ptr)
{
  int address = (address_ptr) ? *address_ptr : -1 ;
  caprice_cli_asm_launch(address);
  return CPARSER_OK;
}


cparser_result_t
cparser_cmd_screenshot_save( cparser_context_t *context)
{
  caprice_cli_screenshot_save();
  return CPARSER_OK;
}
