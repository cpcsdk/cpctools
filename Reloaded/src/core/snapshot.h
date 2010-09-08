//
// Caprice32 Snapshot manager
//

#ifndef _SNAPSHOT_H_
#define _SNAPSHOT_H_

class Emulator;

typedef struct {							// Start offset
   char id[8];								// 0
   char unused1[8];							// 8
   unsigned char version;					// 10
   unsigned char AF[2];						// 11
   unsigned char BC[2];						// 13
   unsigned char DE[2];						// 15
   unsigned char HL[2];						// 17
   unsigned char R;							// 19
   unsigned char I;
   unsigned char IFF0;						// 1B
   unsigned char IFF1;						// 1C
   unsigned char IX[2];						// 1D
   unsigned char IY[2];						// 1F
   unsigned char SP[2];						// 21
   unsigned char PC[2];						// 23
   unsigned char IM;						// 25
   unsigned char AFx[2];					// 26
   unsigned char BCx[2];					// 28
   unsigned char DEx[2];					// 2A
   unsigned char HLx[2];					// 2C
   unsigned char ga_pen;					// 2E
   unsigned char ga_ink_values[17];			// 2F
   unsigned char ga_ROM_config;				// 40
   unsigned char ga_RAM_config;				// 41
   unsigned char crtc_reg_select;			// 42
   unsigned char crtc_registers[18];		// 43
   unsigned char upper_ROM;					// 55
   unsigned char ppi_A;						// 56
   unsigned char ppi_B;						// 57
   unsigned char ppi_C;						// 58
   unsigned char ppi_control;				// 59
   unsigned char psg_reg_select;			// 5A
   unsigned char psg_registers[16];			// 5B
   unsigned char ram_size[2];				// 6B
// version 2 info follows
   unsigned char cpc_model;					// 6D
   unsigned char last_interrupt;			// 6E
   unsigned char scr_modes[6];				// 6F
// version 3 info follows
   unsigned char drvA_DOSfilename[13];		// 75
   unsigned char drvB_DOSfilename[13];
   unsigned char cart_DOSfilename[13];
   unsigned char fdc_motor;					// 9C
   unsigned char drvA_current_track;		// 9D
   unsigned char drvB_current_track;
   unsigned char drvC_current_track;
   unsigned char drvD_current_track;
   unsigned char printer_data;				// A1
   unsigned char psg_env_step;
   unsigned char psg_env_direction;
   unsigned char crtc_type;					// A4
   unsigned char crtc_addr[2];
   unsigned char crtc_scanline[2];
   unsigned char crtc_char_count[2];		// A9
   unsigned char crtc_line_count;			// AB
   unsigned char crtc_raster_count;			// AC
   unsigned char crtc_vt_adjust_count;		// AD
   unsigned char crtc_hsw_count;			// AE
   unsigned char crtc_vsw_count;			// AF
   unsigned char crtc_flags[2];				// B0
   unsigned char ga_int_delay;				// B2
   unsigned char ga_sl_count;				// B3
   unsigned char z80_int_pending;			// B4
   unsigned char unused2[75];				// B5
} t_SNA_header;

int snapshot_load (Emulator &emulator, const char *pchFileName);
int snapshot_save (Emulator &emulator, const char *pchFileName);

#endif
