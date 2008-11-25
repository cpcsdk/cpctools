/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2004 Ulrich Doewich

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/* Nec uPD765A Floppy Disk Controller emulation
   (c) Copyright 1997-2003 Ulrich Doewich

   Nov 08, 2000 - 18:02 started conversion of assembly FDC routines to C
   Nov 22, 2000 - 17:50 added fdc_read_status()
   Dec 19, 2000 - 00:05 added fdc_seek()
   Jan 15, 2001 - 16:52 fdc_read() done
   Jan 17, 2001 - 19:15 updated/fixed fdc_read(), fdc_readID, fdc_intstat; added fdc_recalib
   Jan 18, 2001 - 23:43 inlined cmd_read(); added #defines for loading the result buffer; added fdc_drvstat()
   Jan 24, 2001 - 22:26 fixed fdc_readID() (the result got overwritten by LOAD_RESULT_WITH_CHRN!)
   Aug 03, 2001 - 00:07 added fdc_write()
   Aug 04, 2001 - 10:40 added fdc_writeID()
   Oct 06, 2001 - 23:21 added fdc_readtrk()
   Oct 07, 2001 - 11:37 got fdc_readtrk() to work properly

   Jan 12, 2003 - 18:49 fdc_drvstat now reports an error condition when accessing side 2 on a one sided disk
   Jan 21, 2003 - 18:13 mod to fdc_drvstat was incorrect: drive falls back to 1st side on a single head drive
   Jan 25, 2003 - 15:57 data transfers can now time out and will be reported via OVERRUN condition
   Feb 05, 2003 - 19:42 changed the OVERRUN_TIMEOUT to 26µs as per NEC's documentation
   Feb 08, 2003 - 16:26 added a delay to fdc_read_status in execution phase: FDC first indicates busy before
                        starting data transfer ("Last Mission" loads)
   Feb 10, 2003 - 21:45 fixed cmd_read: multi-sector reads end on the first sector that is found to have the
                        opposite of the requested AM ("Nigel Mansell's Grand Prix" loads)
   Feb 11, 2003 - 18:35 increased OVERRUN_TIMEOUT again ("Puffy's Saga" loads)
   Mar 16, 2003 - 00:19 added the missing scan equal, scan low or equal and scan high or equal commands
   Mar 16, 2003 - 12:22 modified find_sector: if a sector cannot be found, table index should be 0
   Mar 16, 2003 - 16:12 fdc_seek now only changes track if the drive is ready (i.e. has a disk inserted)
   Mar 17, 2003 - 22:42 updated fdc_intstat to report status changes (drive ready/not ready)
   Mar 17, 2003 - 23:04 fixed fdc_drvstat: reset ready if motor is off or no disk inserted
   Mar 22, 2003 - 18:54 added support for the "flipped" disk condition
   Jun 03, 2003 - 18:34 fixed Bad Cylinder and No Cylinder handling when a sector cannot be found:
                        ("Starfox - CPM version" loads)
*/

#ifndef HAVE_LIB765_H


#include "fdc.h"
#include "cap32.h"
#include "config.h"
#include "debug.h"

extern byte *pbGPBuffer;

#ifdef DEBUG_FDC
extern FILE *pfoDebug;
extern dword dwDebugFlag;
dword dwBytesTransferred = 0;
#endif

t_FDC::fdc_cmd_table_def t_FDC::fdc_cmd_table[MAX_CMD_COUNT] = 
{
/* syntax is:
command code, number of bytes for command, number of bytes for result, direction, pointer to command handler
	*/
	{0x03, 3, 0, FDC_TO_CPU, fdc_specify}, // specify
	{0x04, 2, 1, FDC_TO_CPU, fdc_drvstat}, // sense device status
	{0x07, 2, 0, FDC_TO_CPU, fdc_recalib}, // recalibrate
	{0x08, 1, 2, FDC_TO_CPU, fdc_intstat}, // sense interrupt status
	{0x0f, 3, 0, FDC_TO_CPU, fdc_seek},    // seek
	{0x42, 9, 7, FDC_TO_CPU, fdc_readtrk}, // read diagnostic
	{0x45, 9, 7, CPU_TO_FDC, fdc_write},   // write data
	{0x46, 9, 7, FDC_TO_CPU, fdc_read},    // read data
	{0x49, 9, 7, CPU_TO_FDC, fdc_write},   // write deleted data
	{0x4a, 2, 7, FDC_TO_CPU, fdc_readID},  // read id
	{0x4c, 9, 7, FDC_TO_CPU, fdc_read},    // read deleted data
	{0x4d, 6, 7, CPU_TO_FDC, fdc_writeID}, // write id
	{0x51, 9, 7, CPU_TO_FDC, fdc_scan},    // scan equal
	{0x59, 9, 7, CPU_TO_FDC, fdc_scan},    // scan low or equal
	{0x5d, 9, 7, CPU_TO_FDC, fdc_scan},    // scan high or equal
};

t_FDC::t_FDC(t_CPC &cpc, t_drive &dA, t_drive &dB) :
CPC(cpc),
driveA(dA),
driveB(dB),
read_status_delay(0)
{
}

void t_FDC::Reset()
{
	timeout = 0;
	motor = 0;
	led = 0;
	flags = STATUSDRVA_flag | STATUSDRVB_flag;
	phase = CMD_PHASE;
	byte_count = 0;
	buffer_count = 0;
	cmd_length = 0;
	res_length = 0;
	cmd_direction = 0;
	cmd_handler = NULL;
	buffer_ptr = NULL;
	buffer_endptr = NULL;
	memset(&command, 0, 12);
	memset(&result, 0, 8);

	read_status_delay = 0;
}

void t_FDC::Emulate(int iCycleCount)
{
	if (phase == EXEC_PHASE)
	{
		timeout -= iCycleCount;
		if (timeout <= 0)
		{
			flags |= OVERRUN_flag;
			if (cmd_direction == FDC_TO_CPU)
			{
				fdc_read_data();
			}
			else
			{
				fdc_write_data(0xff);
			}
		}
	}
}

void t_FDC::check_unit(void)
{
	switch (command[CMD_UNIT] & 1) // check unit selection bits of active command
	{
	case 0: // target for command is drive A
		active_drive = &driveA;
		break;
	case 1: // target for command is drive B
		active_drive = &driveB;
		break;
	}
}



int t_FDC::init_status_regs(void)
{
	byte val;
	
	memset(&result, 0, sizeof(result)); // clear result codes buffer
	val = command[CMD_UNIT] & 7; // keep head and unit of command
	if ((active_drive->tracks == 0) || (!motor)) { // no DSK in the drive, or drive motor is turned off?
		val |= 0x48; // Abnormal Termination + Not Ready
	}
	result[RES_ST0] = val; // write ST0 to result codes buffer
	return (val & 8); // return value indicates whether drive is ready (0) or not (8)
}



t_sector* t_FDC::find_sector(byte *requested_CHRN)
{
	int loop_count;
	dword idx;
	t_sector *sector;
	
	sector = NULL; // return value indicates 'sector not found' by default
	loop_count = 0; // detection of index hole counter
	idx = active_drive->current_sector; // get the active sector index
	do {
		if (!(memcmp(&active_track->sector[idx].CHRN, requested_CHRN, 4))) { // sector matches requested ID?
			sector = &active_track->sector[idx]; // return value points to sector information
			if ((sector->flags[0] & 0x20) || (sector->flags[1] & 0x20)) { // any Data Errors?
				if (active_drive->random_DEs) { // simulate 'random' DEs?
					flags |= RNDDE_flag;
				}
			}
			result[RES_ST2] &= ~(0x02 | 0x10); // remove possible Bad Cylinder + No Cylinder flags
			break;
		}
		byte cylinder = active_track->sector[idx].CHRN[0]; // extract C
		if (cylinder == 0xff) {
			result[RES_ST2] |= 0x02; // Bad Cylinder
		}
		else if (cylinder != command[CMD_C]) { // does not match requested C?
			result[RES_ST2] |= 0x10; // No Cylinder
		}
		idx++; // increase sector table index
		if (idx >= active_track->sectors) { // index beyond number of sectors for this track?
			idx = 0; // reset index
			loop_count++; // increase 'index hole' count
		}
	} while (loop_count < 2); // loop until sector is found, or index hole has passed twice
	if (result[RES_ST2] & 0x02) { // Bad Cylinder set?
		result[RES_ST2] &= ~0x10; // remove possible No Cylinder flag
	}
	active_drive->current_sector = idx; // update sector table index for active drive
	return sector;
}

void t_FDC::cmd_write(void)
{
	t_sector *sector;
	
	sector = find_sector(&command[CMD_C]); // locate the requested sector on the current track
	if (sector) { // sector found
		int sector_size;
		
		sector->flags[0] = 0; // clear ST1 for this sector
		if (command[CMD_CODE] == 0x45) { // write data command?
			sector->flags[1] = 0; // clear ST2
		}
		else { // write deleted data
			sector->flags[1] = 0x40; // set Control Mark
		}
		
		if (command[CMD_N] == 0) { // use DTL for length?
			sector_size = command[CMD_DTL]; // size of sector is defined by DTL value
			if (sector_size > 0x80) {
				sector_size = 0x80; // max DTL value is 128
			}
		}
		else {
			sector_size = 128 << command[CMD_N]; // determine number of bytes from N value
		}
		buffer_count = sector_size; // init number of bytes to transfer
		buffer_ptr = sector->data; // pointer to sector data
		buffer_endptr = active_track->data + active_track->size; // pointer beyond end of track data
		timeout = INITIAL_TIMEOUT;
		read_status_delay = 1;
	}
	else { // sector not found
		result[RES_ST0] |= 0x40; // AT
		result[RES_ST1] |= 0x04; // No Data
		
		LOAD_RESULT_WITH_CHRN();
			
			phase = RESULT_PHASE; // switch to result phase
	}
}



void t_FDC::cmd_read(void)
{
	t_sector *sector;
	
loop:
	sector = find_sector(&command[CMD_C]); // locate the requested sector on the current track
	if (sector) { // sector found
		result[RES_ST1] = sector->flags[0] & 0x25; // copy ST1 to result, ignoring unused bits
		result[RES_ST2] = sector->flags[1] & 0x61; // copy ST2 to result, ignoring unused bits
		if (command[CMD_CODE] == 0x4c) { // read deleted data command?
			result[RES_ST2] ^= 0x40; // invert Control Mark
		}
		if ((flags & SKIP_flag) && (result[RES_ST2] &= 0x40)) { // skip sector?
			if (command[CMD_R] != command[CMD_EOT]) { // continue looking?
				command[CMD_R]++; // advance to next sector
				goto loop;
			}
			else { // no data to transfer -> no execution phase
				LOAD_RESULT_WITH_STATUS();
					
				LOAD_RESULT_WITH_CHRN();
					
					phase = RESULT_PHASE; // switch to result phase
			}
		}
		else { // sector data is to be transferred
			int sector_size;
			
			if (result[RES_ST2] &= 0x40) { // does the sector have an AM opposite of what we want?
				command[CMD_EOT] = command[CMD_R]; // execution ends on this sector
			}
			if (command[CMD_N] == 0) { // use DTL for length?
				sector_size = command[CMD_DTL]; // size of sector is defined by DTL value
				if (sector_size > 0x80) {
					sector_size = 0x80; // max DTL value is 128
				}
			}
			else {
				sector_size = 128 << command[CMD_N]; // determine number of bytes from N value
			}
			buffer_count = sector_size; // init number of bytes to transfer
			buffer_ptr = sector->data; // pointer to sector data
			buffer_endptr = active_track->data + active_track->size; // pointer beyond end of track data
			timeout = INITIAL_TIMEOUT;
			read_status_delay = 1;
		}
	}
	else { // sector not found
		result[RES_ST0] |= 0x40; // AT
		result[RES_ST1] |= 0x04; // No Data
		
		LOAD_RESULT_WITH_CHRN();
			
			phase = RESULT_PHASE; // switch to result phase
	}
}



void t_FDC::cmd_readtrk(void)
{
	int sector_size;
	t_sector *sector;
	
	sector = &active_track->sector[active_drive->current_sector];
	if (memcmp(&sector->CHRN, &command[CMD_C], 4)) { // sector does not match requested ID?
		result[RES_ST1] |= 0x04; // No Data
	}
	result[RES_ST2] &= 0xbf; // clear Control Mark, if it was set
	result[RES_ST1] |= sector->flags[0] & 0x25; // copy ST1 to result, ignoring unused bits
	result[RES_ST2] |= sector->flags[1] & 0x61; // copy ST2 to result, ignoring unused bits
	if (command[CMD_N] == 0) { // use DTL for length?
		sector_size = command[CMD_DTL]; // size of sector is defined by DTL value
		if (sector_size > 0x80) {
			sector_size = 0x80; // max DTL value is 128
		}
	}
	else {
		sector_size = 128 << command[CMD_N]; // determine number of bytes from N value
	}
	buffer_count = sector_size; // init number of bytes to transfer
	buffer_ptr = sector->data; // pointer to sector data
	buffer_endptr = active_track->data + active_track->size; // pointer beyond end of track data
	timeout = INITIAL_TIMEOUT;
	read_status_delay = 1;
}



void t_FDC::cmd_scan(void)
{
	t_sector *sector;
	
loop:
	sector = find_sector(&command[CMD_C]); // locate the requested sector on the current track
	if (sector) { // sector found
		result[RES_ST1] = sector->flags[0] & 0x25; // copy ST1 to result, ignoring unused bits
		result[RES_ST2] = sector->flags[1] & 0x61; // copy ST2 to result, ignoring unused bits
		if ((flags & SKIP_flag) && (result[RES_ST2] &= 0x40)) { // skip sector?
			if (command[CMD_R] != command[CMD_EOT]) { // continue looking?
				command[CMD_R] += command[CMD_STP]; // advance to next sector
				goto loop;
			}
			else { // no data to transfer -> no execution phase
				LOAD_RESULT_WITH_STATUS();
					
				LOAD_RESULT_WITH_CHRN();
					
				phase = RESULT_PHASE; // switch to result phase
			}
		}
		else { // sector data is to be transferred
			int sector_size;
			
			if (result[RES_ST2] &= 0x40) { // does the sector have an AM opposite of what we want?
				command[CMD_EOT] = command[CMD_R]; // execution ends on this sector
			}
			sector_size = 128 << command[CMD_N]; // determine number of bytes from N value
			buffer_count = sector_size; // init number of bytes to transfer
			buffer_ptr = sector->data; // pointer to sector data
			buffer_endptr = active_track->data + active_track->size; // pointer beyond end of track data
			flags &= ~SCANFAILED_flag; // reset scan failed flag
			result[RES_ST2] |= 0x08; // assume data matches: set Scan Equal Hit
			timeout = INITIAL_TIMEOUT;
			read_status_delay = 1;
		}
	}
	else { // sector not found
		result[RES_ST0] |= 0x40; // AT
		result[RES_ST1] |= 0x04; // No Data
		
		LOAD_RESULT_WITH_CHRN();
			
		phase = RESULT_PHASE; // switch to result phase
	}
}



void t_FDC::fdc_write_data(byte val)
{
	int idx;
	
#ifdef DEBUG_FDC
	if (dwDebugFlag) 
	{
		if (phase == CMD_PHASE) {
			if (byte_count) {
				fprintf(pfoDebug, "FDCPhase : %02x\n", val);
			}
			else {
				fprintf(pfoDebug, "FDCPhase : %02x\n", val);
			}
		}
	}
#endif
	
	switch (phase)
	{
	case CMD_PHASE: // in command phase?
		if (byte_count) { // receiving command parameters?
            command[byte_count++] = val; // copy to buffer
            if (byte_count == cmd_length) { // received all command bytes?
				byte_count = 0; // clear byte counter
				phase = EXEC_PHASE; // switch to execution phase
				cmd_handler(*this);
            }
		}
		else { // first command byte received
            if (val & 0x20) { // skip DAM or DDAM?
				flags |= SKIP_flag; // DAM/DDAM will be skipped
				val &= ~0x20; // reset skip bit in command byte
            }
            else {
				flags &= ~SKIP_flag; // make sure skip inidicator is off
            }
            for (idx = 0; idx < MAX_CMD_COUNT; idx++) { // loop through all known FDC commands
				if (fdc_cmd_table[idx].cmd == val) { // do we have a match?
					break;
				}
            }
            if (idx != MAX_CMD_COUNT) { // valid command received
				cmd_length = fdc_cmd_table[idx].cmd_length; // command length in bytes
				res_length = fdc_cmd_table[idx].res_length; // result length in bytes
				cmd_direction = fdc_cmd_table[idx].cmd_direction; // direction is CPU to FDC, or FDC to CPU
				cmd_handler = fdc_cmd_table[idx].cmd_handler; // pointer to command handler
				
				command[byte_count++] = val; // copy command code to buffer
				if (byte_count == cmd_length) { // already received all command bytes?
					byte_count = 0; // clear byte counter
					phase = EXEC_PHASE; // switch to execution phase
					cmd_handler(*this);
				}
            }
            else { // unknown command received
				result[0] = 0x80; // indicate invalid command
				res_length = 1;
				phase = RESULT_PHASE; // switch to result phase
            }
		}
		break;
	case EXEC_PHASE: // in execution phase?
		if (cmd_direction == CPU_TO_FDC) { // proper direction?
            timeout = OVERRUN_TIMEOUT;
            if ((flags & SCAN_flag)) { // processing any of the scan commands?
				if (val != 0xff) { // no comparison on CPU byte = 0xff
					switch((command[CMD_CODE] & 0x1f))
					{
					case 0x51: // scan equal
                        if (val != *buffer_ptr) {
							result[RES_ST2] &= 0xf7; // reset Scan Equal Hit
							flags |= SCANFAILED_flag;
                        }
                        break;
					case 0x59: // scan low or equal
                        if (val != *buffer_ptr) {
							result[RES_ST2] &= 0xf7; // reset Scan Equal Hit
                        }
                        if (val > *buffer_ptr) {
							flags |= SCANFAILED_flag;
                        }
                        break;
					case 0x5d: // scan high or equal
                        if (val != *buffer_ptr) {
							result[RES_ST2] &= 0xf7; // reset Scan Equal Hit
                        }
                        if (val < *buffer_ptr) {
							flags |= SCANFAILED_flag;
                        }
                        break;
					}
				}
				buffer_ptr++; // advance sector data pointer
            }
            else {
				*buffer_ptr++ = val; // write byte to sector
            }
            if (buffer_ptr > buffer_endptr) {
				buffer_ptr = active_track->data; // wrap around
            }
            if (--buffer_count == 0) { // processed all data?
				if ((flags & SCAN_flag)) { // processing any of the scan commands?
					if ((flags & SCANFAILED_flag) && (command[CMD_R] != command[CMD_EOT])) {
						command[CMD_R] += command[CMD_STP]; // advance to next sector
						cmd_scan();
					}
					else {
						if ((flags & SCANFAILED_flag)) {
							result[RES_ST2] |= 0x04; // Scan Not Satisfied
						}
						
						LOAD_RESULT_WITH_CHRN();
							
						phase = RESULT_PHASE; // switch to result phase
					}
				}
				else if (command[CMD_CODE] == 0x4d) { // write ID command?
					dword sector_size, track_size;
					byte *pbPtr, *pbDataPtr;
					
					if (active_track->sectors != 0) { // track is formatted?
						free(active_track->data); // dealloc memory for old track data
					}
					sector_size = 128 << command[CMD_C]; // determine number of bytes from N value
					if (((sector_size + 62 + command[CMD_R]) * command[CMD_H]) > CPC.max_tracksize) { // track size exceeds maximum?
						active_track->sectors = 0; // 'unformat' track
					}
					else {
						int sector;
						
						track_size = sector_size * command[CMD_H];
						active_track->sectors = command[CMD_H];
						active_track->data = (byte *)malloc(track_size); // attempt to allocate the required memory
						pbDataPtr = active_track->data;
						pbPtr = pbGPBuffer;
						for (sector = 0; sector < command[CMD_H]; sector++) {
							memcpy(active_track->sector[sector].CHRN, pbPtr, 4); // copy CHRN
							memset(active_track->sector[sector].flags, 0, 2); // clear ST1 & ST2
							active_track->sector[sector].data = pbDataPtr; // store pointer to sector data
							pbDataPtr += sector_size;
							pbPtr += 4;
						}
						memset(active_track->data, command[CMD_N], track_size); // fill track data with specified byte value
					}
					pbPtr = pbGPBuffer + ((command[CMD_H]-1) * 4); // pointer to the last CHRN passed to writeID
					memcpy(&result[RES_C], pbPtr, 4); // copy sector's CHRN to result buffer
					result[RES_N] = command[CMD_C]; // overwrite with the N value from the writeID command
					
					active_drive->altered = 1; // indicate that the image has been modified
					phase = RESULT_PHASE; // switch to result phase
				}
				else if (command[CMD_R] != command[CMD_EOT]) { // haven't reached End of Track?
					command[CMD_R]++; // advance to next sector
					cmd_write();
				}
				else {
					active_drive->altered = 1; // indicate that the image has been modified
					
					result[RES_ST0] |= 0x40; // AT
					result[RES_ST1] |= 0x80; // End of Cylinder
					
					LOAD_RESULT_WITH_CHRN();
						
					phase = RESULT_PHASE; // switch to result phase
				}
            }
         }
         break;
   }
}



byte t_FDC::fdc_read_status(void)
{
	byte val;
	
	val = 0x80; // data register ready
	if (phase == EXEC_PHASE) { // in execution phase?
		if (read_status_delay) {
			val = 0x10; // FDC is busy
			read_status_delay--;
		}
		else {
			val |= 0x30; // FDC is executing & busy
		}
		if (cmd_direction == FDC_TO_CPU) {
			val |= 0x40; // FDC is sending data to the CPU
		}
	}
	else if (phase == RESULT_PHASE) { // in result phase?
		val |= 0x50; // FDC is sending data to the CPU, and is busy
	}
	else { // in command phase
		if (byte_count) { // receiving command parameters?
			val |= 0x10; // FDC is busy
		}
	}
	return val;
}



byte t_FDC::fdc_read_data(void)
{
	byte val;
	
	val = 0xff; // default value
	switch (phase)
	{
	case EXEC_PHASE: // in execution phase?
		if (cmd_direction == FDC_TO_CPU) { // proper direction?
            timeout = OVERRUN_TIMEOUT;
            val = *buffer_ptr++; // read byte from current sector
#ifdef DEBUG_FDC
			if (dwDebugFlag) 
			{
				if (!(flags & OVERRUN_flag)) {
					dwBytesTransferred++;
				}
			}
#endif
            if (buffer_ptr >= buffer_endptr) {
				buffer_ptr = active_track->data; // wrap around
            }
            if (!(--buffer_count)) { // completed the data transfer?
				if (flags & RNDDE_flag) { // simulate random Data Errors?
					// ***! random DE handling
				}
				active_drive->current_sector++; // increase sector index
				if (flags & OVERRUN_flag) { // overrun condition detected?
					flags &= ~OVERRUN_flag;
					result[RES_ST0] |= 0x40; // AT
					result[RES_ST1] |= 0x10; // Overrun
					
					LOAD_RESULT_WITH_CHRN();
						
					phase = RESULT_PHASE; // switch to result phase
				}
				else {
					if (command[CMD_CODE] == 0x42) { // read track command?
						if ((--command[CMD_EOT])) { // continue reading sectors?
							if (active_drive->current_sector >= active_track->sectors) { // index beyond number of sectors for this track?
								active_drive->current_sector = 0; // reset index
							}
							command[CMD_R]++; // advance to next sector
							cmd_readtrk();
						}
						else {
							LOAD_RESULT_WITH_STATUS();
								
							LOAD_RESULT_WITH_CHRN();
							
							phase = RESULT_PHASE; // switch to result phase
						}
					}
					else { // normal read (deleted) data command
						if (!((result[RES_ST1] & 0x31) || (result[RES_ST2] & 0x21))) { // no error bits set?
							if (command[CMD_R] != command[CMD_EOT]) { // haven't reached End of Track?
								command[CMD_R]++; // advance to next sector
								cmd_read();
							}
							else {
								LOAD_RESULT_WITH_STATUS();
									
								LOAD_RESULT_WITH_CHRN();
								
								phase = RESULT_PHASE; // switch to result phase
							}
						}
						else {
							LOAD_RESULT_WITH_STATUS();
								
							LOAD_RESULT_WITH_CHRN();
							
							phase = RESULT_PHASE; // switch to result phase
						}
					}
				}
            }
		}
		break;
	case RESULT_PHASE: // in result phase?
		val = result[byte_count++]; // copy value from buffer
		
#ifdef DEBUG_FDC
		if (dwDebugFlag)
		{
			if (dwBytesTransferred) {
				fprintf(pfoDebug, "FDCByteTransfert : {%d}\n", dwBytesTransferred);
				dwBytesTransferred = 0;
			}
			fprintf(pfoDebug, "FDCResultPhase : [%02x]\n", val);
		}
#endif
		
		if (byte_count == res_length) { // sent all result bytes?
            flags &= ~SCAN_flag; // reset scan command flag
            byte_count = 0; // clear byte counter
            phase = CMD_PHASE; // switch to command phase
            led = 0; // turn the drive LED off
		}
		break;
	}
	return val;
}



void t_FDC::fdc_specify(t_FDC &FDC)
{
	FDC.phase = CMD_PHASE; // switch back to command phase (fdc_specify has no result phase!)
}



void t_FDC::fdc_drvstat(t_FDC &FDC)
{
	byte val;
	
	FDC.check_unit(); // switch to target drive
	val = FDC.command[CMD_UNIT] & 7; // keep head and unit of command
	if ((FDC.active_drive->write_protected) || (FDC.active_drive->tracks == 0)) { // write protected, or disk missing?
		val |= 0x48; // set Write Protect + Two Sided (?)
	}
	if ((FDC.active_drive->tracks) && (FDC.motor)) {
		val |= 0x20; // set Ready
	}
	if (FDC.active_drive->current_track == 0) { // drive head is over track 0?
		val |= 0x10; // set Track 0
	}
	FDC.result[RES_ST0] = val;
	FDC.phase = RESULT_PHASE; // switch to result phase
}



void t_FDC::fdc_recalib(t_FDC &FDC)
{
	FDC.command[CMD_C] = 0; // seek to track 0
	fdc_seek(FDC);
}



void t_FDC::fdc_intstat(t_FDC &FDC)
{
	byte val;
	
	val = FDC.result[RES_ST0] & 0xf8; // clear Head Address and Unit bits
	if (FDC.flags & SEEKDRVA_flag) { // seek completed on drive A?
		val |= 0x20; // set Seek End
		FDC.flags &= ~(SEEKDRVA_flag | STATUSDRVA_flag); // clear seek done and status change flags
		FDC.result[RES_ST0] = val;
		FDC.result[RES_ST1] = FDC.driveA.current_track;
	}
	else if (FDC.flags & SEEKDRVB_flag) { // seek completed on drive B?
		val |= 0x21; // set Seek End
		FDC.flags &= ~(SEEKDRVB_flag | STATUSDRVB_flag); // clear seek done and status change flags
		FDC.result[RES_ST0] = val;
		FDC.result[RES_ST1] = FDC.driveB.current_track;
	}
	else if (FDC.flags & STATUSDRVA_flag) { // has the status of drive A changed?
		val = 0xc0; // status change
		if ((FDC.driveA.tracks == 0) || (!FDC.motor)) { // no DSK in the drive, or drive motor is turned off?
			val |= 0x08; // not ready
		}
		FDC.flags &= ~STATUSDRVA_flag; // clear status change flag
		FDC.result[RES_ST0] = val;
		FDC.result[RES_ST1] = FDC.driveA.current_track;
	}
	else if (FDC.flags & STATUSDRVB_flag) { // has the status of drive B changed?
		val = 0xc1; // status change
		if ((FDC.driveB.tracks == 0) || (!FDC.motor)) { // no DSK in the drive, or drive motor is turned off?
			val |= 0x08; // not ready
		}
		FDC.flags &= ~STATUSDRVB_flag; // clear status change flag
		FDC.result[RES_ST0] = val;
		FDC.result[RES_ST1] = FDC.driveB.current_track;
	}
	else {
		val = 0x80; // Invalid Command
		FDC.result[RES_ST0] = val;
		FDC.res_length = 1;
	}
	FDC.phase = RESULT_PHASE; // switch to result phase
}



void t_FDC::fdc_seek(t_FDC &FDC)
{
	FDC.check_unit(); // switch to target drive
	if (FDC.init_status_regs() == 0) { // drive Ready?
		FDC.active_drive->current_track = FDC.command[CMD_C];
		if (FDC.active_drive->current_track >= FDC_TRACKMAX) { // beyond valid range?
			FDC.active_drive->current_track = FDC_TRACKMAX-1; // limit to maximum
		}
	}
	FDC.flags |= (FDC.command[CMD_UNIT] & 1) ? SEEKDRVB_flag : SEEKDRVA_flag; // signal completion of seek operation
	FDC.phase = CMD_PHASE; // switch back to command phase (fdc_seek has no result phase!)
}



void t_FDC::fdc_readtrk(t_FDC &FDC)
{
	FDC.led = 1; // turn the drive LED on
	FDC.check_unit(); // switch to target drive
	if (FDC.init_status_regs() == 0) { // drive Ready?
		FDC.active_drive->current_side = (FDC.command[CMD_UNIT] & 4) >> 2; // extract target side
		dword side = FDC.active_drive->sides ? FDC.active_drive->current_side : 0; // single sided drives only acccess side 1
		if ((FDC.active_drive->flipped)) { // did the user request to access the "other" side?
			side = side ? 0 : 1; // reverse the side to access
		}
		FDC.active_track = &FDC.active_drive->track[FDC.active_drive->current_track][side];
		if (FDC.active_track->sectors != 0) { // track is formatted?
			FDC.command[CMD_R] = 1; // set sector ID to 1
			FDC.active_drive->current_sector = 0; // reset sector table index
			
			FDC.cmd_readtrk();
		}
		else { // unformatted track
			FDC.result[RES_ST0] |= 0x40; // AT
			FDC.result[RES_ST1] |= 0x01; // Missing AM
			
			FDC.LOAD_RESULT_WITH_CHRN();
				
			FDC.phase = RESULT_PHASE; // switch to result phase
		}
	}
	else { // drive was not ready
		FDC.LOAD_RESULT_WITH_CHRN();
			
		FDC.phase = RESULT_PHASE; // switch to result phase
	}
}



void t_FDC::fdc_write(t_FDC &FDC)
{
	FDC.led = 1; // turn the drive LED on
	FDC.check_unit(); // switch to target drive
	if (FDC.init_status_regs() == 0) { // drive Ready?
		FDC.active_drive->current_side = (FDC.command[CMD_UNIT] & 4) >> 2; // extract target side
		dword side = FDC.active_drive->sides ? FDC.active_drive->current_side : 0; // single sided drives only acccess side 1
		if ((FDC.active_drive->flipped)) { // did the user request to access the "other" side?
			side = side ? 0 : 1; // reverse the side to access
		}
		FDC.active_track = &FDC.active_drive->track[FDC.active_drive->current_track][side];
		if (FDC.active_drive->write_protected) { // is write protect tab set?
			FDC.result[RES_ST0] |= 0x40; // AT
			FDC.result[RES_ST1] |= 0x02; // Not Writable
			
			FDC.LOAD_RESULT_WITH_CHRN();
				
			FDC.phase = RESULT_PHASE; // switch to result phase
		}
		else if (FDC.active_track->sectors != 0) { // track is formatted?
			FDC.cmd_write();
		}
		else { // unformatted track
			FDC.result[RES_ST0] |= 0x40; // AT
			FDC.result[RES_ST1] |= 0x01; // Missing AM
			
			FDC.LOAD_RESULT_WITH_CHRN();
				
			FDC.phase = RESULT_PHASE; // switch to result phase
		}
	}
	else { // drive was not ready
		FDC.LOAD_RESULT_WITH_CHRN();
			
		FDC.phase = RESULT_PHASE; // switch to result phase
	}
}



void t_FDC::fdc_read(t_FDC &FDC)
{
	FDC.led = 1; // turn the drive LED on
	FDC.check_unit(); // switch to target drive
	if (FDC.init_status_regs() == 0) { // drive Ready?
		FDC.active_drive->current_side = (FDC.command[CMD_UNIT] & 4) >> 2; // extract target side
		dword side = FDC.active_drive->sides ? FDC.active_drive->current_side : 0; // single sided drives only acccess side 1
		if ((FDC.active_drive->flipped)) { // did the user request to access the "other" side?
			side = side ? 0 : 1; // reverse the side to access
		}
		FDC.active_track = &FDC.active_drive->track[FDC.active_drive->current_track][side];
		if (FDC.active_track->sectors != 0) { // track is formatted?
			FDC.cmd_read();
		}
		else { // unformatted track
			FDC.result[RES_ST0] |= 0x40; // AT
			FDC.result[RES_ST1] |= 0x01; // Missing AM
			
			FDC.LOAD_RESULT_WITH_CHRN();
				
			FDC.phase = RESULT_PHASE; // switch to result phase
		}
	}
	else { // drive was not ready
		FDC.LOAD_RESULT_WITH_CHRN();
			
		FDC.phase = RESULT_PHASE; // switch to result phase
	}
}



void t_FDC::fdc_readID(t_FDC &FDC)
{
	FDC.led = 1; // turn the drive LED on
	FDC.check_unit(); // switch to target drive
	if (FDC.init_status_regs() == 0) { // drive Ready?
		FDC.active_drive->current_side = (FDC.command[CMD_UNIT] & 4) >> 2; // extract target side
		dword side = FDC.active_drive->sides ? FDC.active_drive->current_side : 0; // single sided drives only acccess side 1
		if ((FDC.active_drive->flipped)) { // did the user request to access the "other" side?
			side = side ? 0 : 1; // reverse the side to access
		}
		FDC.active_track = &FDC.active_drive->track[FDC.active_drive->current_track][side];
		if (FDC.active_track->sectors != 0) { // track is formatted?
			dword idx;
			
			idx = FDC.active_drive->current_sector; // get the active sector index
			if (idx >= FDC.active_track->sectors) { // index beyond number of sectors for this track?
				idx = 0; // reset index
			}
			memcpy(&FDC.result[RES_C], &FDC.active_track->sector[idx].CHRN, 4); // copy sector's CHRN to result buffer
			FDC.active_drive->current_sector = idx + 1; // update sector table index for active drive
		}
		else { // unformatted track
			FDC.result[RES_ST0] |= 0x40; // AT
			FDC.result[RES_ST1] |= 0x01; // Missing AM
			
			FDC.LOAD_RESULT_WITH_CHRN();
		}
	}
	FDC.phase = RESULT_PHASE; // switch to result phase
}



void t_FDC::fdc_writeID(t_FDC &FDC)
{
	FDC.led = 1; // turn the drive LED on
	FDC.check_unit(); // switch to target drive
	if (FDC.init_status_regs() == 0) { // drive Ready?
		FDC.active_drive->current_side = (FDC.command[CMD_UNIT] & 4) >> 2; // extract target side
		dword side = FDC.active_drive->sides ? FDC.active_drive->current_side : 0; // single sided drives only acccess side 1
		if ((FDC.active_drive->flipped)) { // did the user request to access the "other" side?
			side = side ? 0 : 1; // reverse the side to access
		}
		FDC.active_track = &FDC.active_drive->track[FDC.active_drive->current_track][side];
		if (FDC.active_drive->write_protected) { // is write protect tab set?
			FDC.result[RES_ST0] |= 0x40; // AT
			FDC.result[RES_ST1] |= 0x02; // Not Writable
			
			FDC.LOAD_RESULT_WITH_CHRN();
				
			FDC.phase = RESULT_PHASE; // switch to result phase
		}
		else {
			FDC.buffer_count = FDC.command[CMD_H] << 2; // number of sectors * 4 = number of bytes still outstanding
			FDC.buffer_ptr = pbGPBuffer; // buffer to temporarily hold the track format
			FDC.buffer_endptr = pbGPBuffer + FDC.buffer_count;
			FDC.timeout = INITIAL_TIMEOUT;
			FDC.read_status_delay = 1;
		}
	}
	else { // drive was not ready
		FDC.LOAD_RESULT_WITH_CHRN();
			
		FDC.phase = RESULT_PHASE; // switch to result phase
	}
}



void t_FDC::fdc_scan(t_FDC &FDC)
{
	FDC.led = 1; // turn the drive LED on
	FDC.check_unit(); // switch to target drive
	if (FDC.init_status_regs() == 0) { // drive Ready?
		FDC.active_drive->current_side = (FDC.command[CMD_UNIT] & 4) >> 2; // extract target side
		dword side = FDC.active_drive->sides ? FDC.active_drive->current_side : 0; // single sided drives only acccess side 1
		if ((FDC.active_drive->flipped)) { // did the user request to access the "other" side?
			side = side ? 0 : 1; // reverse the side to access
		}
		FDC.active_track = &FDC.active_drive->track[FDC.active_drive->current_track][side];
		if (FDC.active_track->sectors != 0) { // track is formatted?
			if (FDC.command[CMD_STP] > 2) {
				FDC.command[CMD_STP] = 2; // step can only be 1 or 2
			}
			FDC.flags |= SCAN_flag; // scan command active
			FDC.cmd_scan();
		}
		else { // unformatted track
			FDC.result[RES_ST0] |= 0x40; // AT
			FDC.result[RES_ST1] |= 0x01; // Missing AM
			
			FDC.LOAD_RESULT_WITH_CHRN();
				
			FDC.phase = RESULT_PHASE; // switch to result phase
		}
	}
	else { // drive was not ready
		FDC.LOAD_RESULT_WITH_CHRN();
			
		FDC.phase = RESULT_PHASE; // switch to result phase
	}
}


#endif

