//
// Caprice32 FDC emulator
//

#ifndef HAVE_LIB765_H


#ifndef _FDC_OLD_H_
#define _FDC_OLD_H_

#include "cap32type.h"

#include <memory>
#include <string>

using std::shared_ptr;
using std::weak_ptr;
using std::string;

class Emulator;

#define FDC_TO_CPU      0
#define CPU_TO_FDC      1

#define CMD_PHASE       0
#define EXEC_PHASE      1
#define RESULT_PHASE    2

#define SKIP_flag       1     // skip sectors with DDAM/DAM
#define SEEKDRVA_flag   2     // seek operation has finished for drive A
#define SEEKDRVB_flag   4     // seek operation has finished for drive B
#define RNDDE_flag      8     // simulate random DE sectors
#define OVERRUN_flag    16    // data transfer timed out
#define SCAN_flag       32    // one of the three scan commands is active
#define SCANFAILED_flag 64    // memory and sector data does not match
#define STATUSDRVA_flag 128   // status change of drive A
#define STATUSDRVB_flag 256   // status change of drive B

#define FDC_TRACKMAX    102   // max amount that fits in a DSK header
#define FDC_SIDEMAX     2
#define FDC_SECTORMAX   29    // max amount that fits in a track header

#define CMD_CODE  0
#define CMD_UNIT  1
#define CMD_C     2
#define CMD_H     3
#define CMD_R     4
#define CMD_N     5
#define CMD_EOT   6
#define CMD_GPL   7
#define CMD_DTL   8
#define CMD_STP   8

#define RES_ST0   0
#define RES_ST1   1
#define RES_ST2   2
#define RES_C     3
#define RES_H     4
#define RES_R     5
#define RES_N     6

#define OVERRUN_TIMEOUT 128*4
// #define INITIAL_TIMEOUT OVERRUN_TIMEOUT*4
#define INITIAL_TIMEOUT OVERRUN_TIMEOUT*10 // This makes Orion Prime work.

#define MAX_CMD_COUNT 15

typedef struct {
   unsigned char CHRN[4]; // the CHRN for this sector
   unsigned char flags[4]; // ST1 and ST2 - reflects any possible error conditions
   unsigned int size; // sector size in bytes
   unsigned char *data; // pointer to sector data
   unsigned int declared_size; // most of the time equals size, except for "weak" sectors
} t_sector;

typedef struct {
   unsigned int sectors; // sector count for this track
   unsigned int size; // track size in bytes
   unsigned char *data; // pointer to track data
   unsigned char gap3;
   t_sector sector[FDC_SECTORMAX]; // array of sector information structures
} t_track;

typedef struct {
   unsigned int tracks; // total number of tracks
   unsigned int current_track; // location of drive head
   unsigned int sides; // total number of sides
   unsigned int current_side; // side being accessed
   unsigned int current_sector; // sector being accessed
   unsigned int altered; // has the image been modified?
   unsigned int write_protected; // is the image write protected?
   unsigned int random_DEs; // sectors with Data Errors return random data?
   unsigned int flipped; // reverse the side to access?
   t_track track[FDC_TRACKMAX][FDC_SIDEMAX]; // array of track information structures
} t_drive;

class t_FDC
{
private:

	typedef void (t_FDC::*CmdHandler)();
	typedef struct fdc_cmd_table_def {
		int cmd;
		int cmd_length;
		int res_length;
		int cmd_direction;
		CmdHandler cmd_handler;
	} fdc_cmd_table_def;

private:
	static fdc_cmd_table_def fdc_cmd_table[MAX_CMD_COUNT];

private:
	weak_ptr<Emulator> _emulator;

	t_drive driveA;
	t_drive driveB;


	t_drive *active_drive; // reference to the currently selected drive
	t_track *active_track; // reference to the currently selected track, of the active_drive
	dword read_status_delay;

	int timeout;
	int motor;
	int flags;
	int phase;
	int byte_count;
	int buffer_count;
	int cmd_length;
	int res_length;
	int cmd_direction;
	CmdHandler cmd_handler;
	unsigned char *buffer_ptr;
	unsigned char *buffer_endptr;
	unsigned char command[12];
	unsigned char result[8];

public:
	t_FDC(shared_ptr<Emulator> emulator);

	void Reset();

	unsigned char getST(int n) const
	{
		return result[n];
	}

	void Emulate(int iCycleCount);

	inline int			GetTimeout() const					{ return timeout;		}
	inline void			SetTimeout(int t)					{ timeout = t;			}
	inline int			GetMotor() const					{ return motor;			}
	void			SetMotor(int m);
	inline int			GetFlags() const					{ return flags;			}
	inline void			SetFlags(int f)						{ flags = f;			}
	inline int			GetPhase() const					{ return phase;			}
	inline void			SetPhase(int p)						{ phase = p;			}
	inline int			GetCmdDirection() const				{ return cmd_direction;	}

	inline t_drive&		GetDriveA()							{ return driveA;		}
	inline t_drive&		GetDriveB()							{ return driveB;		}

	int insertA(const string filename, const char *type = NULL);
	int insertB(const string filename, const char *type = NULL);

	void fdc_write_data(unsigned char val);
	unsigned char fdc_read_status(void);
	unsigned char fdc_read_data(void);

	string files[2];
private:
	void check_unit(void);
	int init_status_regs(void);
	t_sector* find_sector(byte *requested_CHRN);

	void cmd_write(void);
	void cmd_read(void);
	void cmd_readtrk(void);
	void cmd_scan(void);


	inline void LOAD_RESULT_WITH_STATUS()
	{
		// AT
		result[RES_ST0] |= 0x40;
		// End of Cylinder
		result[RES_ST1] |= 0x80;

		// continue only if not a read track command
		if (command[CMD_CODE] != 0x42)
		{
			// any 'error bits' set?
			if ((result[RES_ST1] & 0x7f) || (result[RES_ST2] & 0x7f))
			{
				// mask out End of Cylinder
				result[RES_ST1] &= 0x7f;

				// DE and/or DD?
				if ((result[RES_ST1] & 0x20) || (result[RES_ST2] & 0x20))
				{
					// mask out Control Mark
					result[RES_ST2] &= 0xbf;
				}
				// Control Mark?
				else if (result[RES_ST2] & 0x40)
				{
					// mask out AT
					result[RES_ST0] &= 0x3f;
					// mask out End of Cylinder
					result[RES_ST1] &= 0x7f;
				}
			}
		}
	}

	inline void LOAD_RESULT_WITH_CHRN()
	{
		// load result with current CHRN values
		result[RES_C] = command[CMD_C];
		result[RES_H] = command[CMD_H];
		result[RES_R] = command[CMD_R];
		result[RES_N] = command[CMD_N];
	}

private:
	void fdc_specify();
	void fdc_drvstat();
	void fdc_recalib();
	void fdc_intstat();
	void fdc_seek();
	void fdc_readtrk();
	void fdc_write();
	void fdc_read();
	void fdc_readID();
	void fdc_writeID();
	void fdc_scan();
	void fdc_scanlo();
	void fdc_scanhi();
};

#endif

#endif
