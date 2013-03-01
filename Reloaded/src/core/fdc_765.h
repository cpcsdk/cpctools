/**
 * Caprice32
 * fdc_765.h
 * 
 * Copyright (C) 2008 Krusty/Benediction <krusty@cpcscene.com>
 *	    
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#ifdef HAVE_LIB765_H


#ifndef _FDC_H_
#define _FDC_H_

#include <cerrno>
#include <climits>
#include <cstdarg>
#include <unistd.h>

#include "cap32type.h"
#include "config.h"
#include <cstring>
#include <string>
#include <libdsk.h>
#include <765.h>

using namespace std;

class t_CPC;

typedef struct {
   unsigned int tracks; 	// total number of tracks
   unsigned int current_track; 	// location of drive head
   unsigned int sides; 		// total number of sides
   unsigned int current_side; 	// side being accessed
   unsigned int current_sector; // sector being accessed
   unsigned int altered; 	// has the image been modified?
   unsigned int write_protected; // is the image write protected?
   unsigned int random_DEs; 	// sectors with Data Errors return random data?
   unsigned int flipped; 	// reverse the side to access?
   FDRV_PTR fdl; 		//LibDSK drive
} t_drive;

class t_FDC
{

private:
	shared_ptr<t_CPC>	CPC;

	t_drive		driveA;
	t_drive		driveB;
	
	FDC_PTR m_fdc;                   	//Floppy Controler 

	byte m_terminalCount;             
	bool m_fdcInt;


public:
	t_FDC(shared_ptr<t_CPC> CPC);
	~t_FDC();

	void Reset();

	void Emulate(int iCycleCount);
/*
	inline int			GetTimeout() const					{ return timeout;		}
	inline void			SetTimeout(int t)					{ timeout = t;			}
*/
	inline int			GetMotor() const					{ return 1;	}
	inline void			SetMotor(int m)						{ fdc_set_motor(m_fdc, m);	}

/*
	inline int			GetFlags() const					{ return flags;			}
	inline void			SetFlags(int f)						{ flags = f;			}
*/


/*
	inline int			GetPhase() const					{ return phase;			}
	inline void			SetPhase(int p)						{ phase = p;			}
*/

//	inline int			GetCmdDirection() const				{ return cmd_direction;	}

	inline t_drive&		GetDriveA()							{ return driveA;		}
	inline t_drive&		GetDriveB()							{ return driveB;		}

	void insertA(const string filename, const char *type = NULL);
	void insertB(const string filename, const char *type = NULL);

	void fdc_write_data(unsigned char val);
	unsigned char fdc_read_status(void);
	unsigned char fdc_read_data(void);

};

#endif


#endif
