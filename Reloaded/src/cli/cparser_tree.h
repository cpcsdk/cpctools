/*----------------------------------------------------------------------
 * This file is generated by mk_parser.py.
 *----------------------------------------------------------------------*/
#ifndef __CPARSER_TREE_H__
#define __CPARSER_TREE_H__

extern cparser_node_t cparser_root;

cparser_result_t cparser_cmd_show_registers(cparser_context_t *context);
cparser_result_t cparser_cmd_show_registers_crtc(cparser_context_t *context);
cparser_result_t cparser_cmd_show_registers_ga(cparser_context_t *context);
cparser_result_t cparser_cmd_show_registers_z80(cparser_context_t *context);
cparser_result_t cparser_cmd_breakpoints_add_adress(cparser_context_t *context,
    uint32_t *adress_ptr);
cparser_result_t cparser_cmd_breakpoints_remove_adress(cparser_context_t *context,
    uint32_t *adress_ptr);
cparser_result_t cparser_cmd_breakpoints_list(cparser_context_t *context);
cparser_result_t cparser_cmd_continue(cparser_context_t *context);
cparser_result_t cparser_cmd_step(cparser_context_t *context);
cparser_result_t cparser_cmd_video_color_on(cparser_context_t *context);
cparser_result_t cparser_cmd_video_color_grey(cparser_context_t *context);
cparser_result_t cparser_cmd_video_color_green(cparser_context_t *context);
cparser_result_t cparser_cmd_memory_poke_address_value(cparser_context_t *context,
    uint32_t *address_ptr,
    uint32_t *value_ptr);
cparser_result_t cparser_cmd_memory_peek_address(cparser_context_t *context,
    uint32_t *address_ptr);
cparser_result_t cparser_cmd_memory_disassemble_address_quantity(cparser_context_t *context,
    uint32_t *address_ptr,
    uint32_t *quantity_ptr);
cparser_result_t cparser_cmd_memory_disassemble_pc_quantity(cparser_context_t *context,
    uint32_t *quantity_ptr);
cparser_result_t cparser_cmd_asm_compile_filename(cparser_context_t *context,
    char **filename_ptr);
cparser_result_t cparser_cmd_asm_launch_address(cparser_context_t *context,
    uint32_t *address_ptr);
cparser_result_t cparser_cmd_screenshot_save(cparser_context_t *context);
cparser_result_t cparser_cmd_quit(cparser_context_t *context);
cparser_result_t cparser_cmd_reset(cparser_context_t *context);
cparser_result_t cparser_cmd_help_filter(cparser_context_t *context,
    char **filter_ptr);

#endif /* __CPARSER_TREE_H__ */
