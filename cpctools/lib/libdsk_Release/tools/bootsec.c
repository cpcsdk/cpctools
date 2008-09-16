/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2006  John Elliott <jce@seasip.demon.co.uk>            *
 *                                                                         *
 *    This library is free software; you can redistribute it and/or        *
 *    modify it under the terms of the GNU Library General Public          *
 *    License as published by the Free Software Foundation; either         *
 *    version 2 of the License, or (at your option) any later version.     *
 *                                                                         *
 *    This library is distributed in the hope that it will be useful,      *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    *
 *    Library General Public License for more details.                     *
 *                                                                         *
 *    You should have received a copy of the GNU Library General Public    *
 *    License along with this library; if not, write to the Free           *
 *    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,      *
 *    MA 02111-1307, USA                                                   *
 *                                                                         *
 ***************************************************************************/

#include "apriboot.h"

/* This file deals with transforming an Apricot boot sector to/from PCDOS
 * format. In an attempt to make repeated conversion easier, it saves 
 * the vital bits of the original boot sector in unused areas of the 
 * converted sector, and indicates this with a magic number. */

unsigned char apri_magic[12] = 
{
	0x04, 0x35, 'A', 'P', 'R', 'I', 'B', 'O', 'O', 'T', '=', '>'
};       

unsigned char pcd_magic[12] = 
{
	0x05, 0x35, 'A', 'P', 'R', 'I', 'B', 'O', 'O', 'T', '=', '>'
};   
       
unsigned char pcdos_dummy[] =
{   
/*     0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f */
	0xE9, 0x40, 0x90,  'I',  'B',  'M',  ' ',  ' ',  '3',  '.',  '3', 0x00, 0x02, 0x01, 0x01, 0x00,
	0x02, 0x80, 0x00, 0x76, 0x02, 0xFC, 0x02, 0x00, 0x09, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x90, 0x90, 0xCD, 0x18	
};               
    
    
unsigned char apricot_dummy[] =
{
/*     0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f */
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
	0x09, 0x00, 0x50, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0e, 0x00,
	0x00, 0x00, 0x00, 0x1E, 0x00, 0x03, 0x00, 0x1E, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x02, 0x01, 0x00, 0x02, 0xB0, 0x00, 0xA0, 0x05, 0xFE, 0x03, 0x00, 0x02, 0x00, 0x00,
     'F',  'O',  'N',  'T',  '=',  'B',  'R',  'I',  'T',  '0',  '3',  ' ',  ' ',  ' ',  ' ',  ' ',	
     'K',  'E',  'Y',  'S',  '=',  'A',  'C',  'T',  '0',  '0',  '2',  ' ',  ' ',  ' ',  ' ',  ' ',	
	0x06, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x22, 0x14, 0x02, 0x36, 0x00, 0x12, 0x00, 0x26, 0x00,
    0x08, 0x01, 0x19, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0E, 0x0E, 0x08, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x11, 0x13, 0x1E, 0x00, 0x00, 0x00, 0x00
};                                                                  

    
static int pcdos_to_apricot(byte *buf, const char *filename, int verbose)
{
	byte buf2[512];

	memset(buf2, 0, sizeof(buf2));	
	/* Is there a saved copy of the PCDOS boot sector? If so, use it */           
	if (!memcmp(buf + 0x144, apri_magic, sizeof(apri_magic)))
	{
		memcpy(buf2, buf + 0x150, 0xA0);				/* Copy in saved sector */
	}
	else	/* If not, generate the Apricot sector from the PCDOS one */
	{
		int bootsecs = buf[14] + 256 * buf[15];  
		int fatcopies= buf[16];
		int dirsize  = buf[17] + 256 * buf[18];
		long totsecs = buf[19] + 256 * buf[20];	
		int fatsize  = buf[22] + 256 * buf[23];
		int sectors  = buf[24] + 256 * buf[25];
		int heads    = buf[26] + 256 * buf[27];
	/* Might want to expand this later on to handle 32-bit sector counts */
		long cylinders = totsecs / (heads * sectors);
		long dataoff   = bootsecs + (fatcopies * fatsize) + ((dirsize + 15) / 16);
		   
		memcpy(buf2, apricot_dummy, sizeof(apricot_dummy));
		memcpy(buf2,	  buf + 3, 8);		/* OEM ID */
		buf2[0x10] = buf[0x18];				/* Count of sectors */
		buf2[0x11] = buf[0x19];
		buf2[0x12] = (byte)(cylinders & 0xFF);
		buf2[0x13] = (byte)((cylinders >>  8) & 0xFF);
		buf2[0x14] = (byte)((cylinders >> 16) & 0xFF);
		buf2[0x15] = (byte)((cylinders >> 24) & 0xFF);
		buf2[0x16] = buf[0x1A];				/* Count of heads */

	/* Offset to data area */   
		buf2[0x28] = (byte)(dataoff & 0xFF); 
		buf2[0x29] = (byte)((dataoff >> 8)  & 0xFF); 
		buf2[0x2a] = (byte)((dataoff >> 16) & 0xFF); 
		buf2[0x2b] = (byte)((dataoff >> 24) & 0xFF); 
		
		memcpy(buf2 + 0x50, buf + 0x0b, 13); /* BPB */ 
		/* Disk type */	
		if (heads == 1)
		{
			if (cylinders == 70) 
		 		 buf2[0x54] = 0;
			else buf2[0x54] = 1;
		}
		else     buf2[0x54] = 2;
	}
	/* Save the original PCDOS boot sector */
	memcpy(buf2 + 0x144, pcd_magic, sizeof(pcd_magic));
	memcpy(buf2 + 0x150, buf, 0xA0);
	
	memcpy(buf, buf2, sizeof(buf2));
	if (verbose) fprintf(stderr,"%s: Bootsector converted to Apricot format\n", filename); 
	return 0;
}


static int apricot_to_pcdos(byte *buf, const char *filename, int verbose)
{      
	byte buf2[512];

	memset(buf2, 0, sizeof(buf2));	
	/* Is there a saved copy of the PCDOS boot sector? If so, use it */
	if (!memcmp(buf + 0x144, pcd_magic, sizeof(pcd_magic)))
	{
		memcpy(buf2, buf + 0x150, 0xA0);				/* Copy in saved sector */
		memcpy(buf2, pcdos_dummy, 3);  /* and patch its code to be */
		memcpy(buf2 + 0x40, pcdos_dummy + 0x40, 4);
						/* the dummy boot sequence */
	}
	else	/* If not, generate the PCDOS sector from the Apricot one */
	{
		int heads     = buf[0x16];
		int sectors   = buf[0x10] + 256 * buf[0x11];

		memcpy(buf2, pcdos_dummy, sizeof(pcdos_dummy));
		memcpy(buf2 + 3,    buf,         8);		/* OEM ID */
		memcpy(buf2 + 0x0b, buf + 0x50, 13);		/* BPB */ 
		buf2[24] = sectors & 0xFF;
		buf2[25] = sectors >> 8;
		buf2[26] = heads & 0xFF;
		buf2[27] = heads >> 8;
	}
	/* Save the original Apricot boot sector */
	memcpy(buf2 + 0x144, apri_magic, sizeof(apri_magic));
	memcpy(buf2 + 0x150, buf, 0xA0);
	buf2[0x1FE] = 0x55;
	buf2[0x1FF] = 0xAA;	/* Boot record signature */
	
	memcpy(buf, buf2, sizeof(buf2)); 
	if (verbose) fprintf(stderr,"%s: Bootsector converted to PCDOS format\n", filename); 
	return 0;
}



/* Check for a PCDOS BPB. */
static int sector_is_pcdos(byte *buf)
{
	if (buf[0] != 0 && buf[0] != 0xE9 && buf[0] != 0xEB) 
	{
		return 0;	/* Must start with an IBM or Atari jump */
	}
	if (buf[11] != 0 || buf[12] != 2)
	{
		return 0;	/* Must have a 512-byte sector size */
	}
	return 1;
}


static int sector_is_apricot(byte *buf)
{
	/* Check that the OEM ID is either ASCII or zeroes */
	int n;
	
	for (n = 0; n < 8; n++)
	{
		if (buf[n] != 0 && (buf[n] < ' ' || buf[n] > '~'))
		{
			return 0;
		}
	}
	/* Check that we have 512-byte sectors */
	if (buf[80] != 0 || buf[81] != 2)
	{
		return 0;
	}
	/* XXX More validation here */
	return 1;	
}

 
 
int transform(MODE md, byte *buf, const char *filename, int verbose)
{
	int is_pcdos = 0;
	
	/* OK. Firstly, determine whether this is a PC-DOS bootsector,
	  an Apricot bootsector, or something else. */
	if (sector_is_pcdos(buf))
	{			   
		is_pcdos = 1;
		if (md == FORCE_PCDOS) 	/* Nothing to do! */	
		{
			fprintf(stderr, "%s: Is already in PCDOS format\n",
				filename);
			return 0;
		}
	}
	else if (sector_is_apricot(buf))
	{
		if (md == FORCE_APRICOT) 
		{
			fprintf(stderr, "%s: Is already in Apricot format\n",
				filename);
			return 0;
		}
	}
	else
	{
		fprintf(stderr, "%s: Could not identify the boot sector\n",
			filename);
		return -1;
	}
	/* Need to swap. */
	if (is_pcdos) return pcdos_to_apricot(buf, filename, verbose);
	else	      return apricot_to_pcdos(buf, filename, verbose);
}
