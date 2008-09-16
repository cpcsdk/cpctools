/*
 ***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2001  John Elliott <jce@seasip.demon.co.uk>            *
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

package uk.co.demon.seasip.libdsk;

/** This class contains all the format types that LibDsk supports. */
public class FormatType
{
	static 
	{
		LibDsk.forceInit();
	}
/** 9 sectors, 1 side, 40 tracks, 512 byte sectors. 
 * This corresponds to the standard Spectrum +3 / PCW8256 format, as well as 
 * the PCDOS 180k format. */
	public static final int FMT_180K = 0;	
/** CPC system 180K - as FMT_180K, but sectors start at 0x41 not 0x01. */
	public static final int FMT_CPCSYS = 1;
/** CPC data 180K - as FMT_180K, but sectors start at 0xC1 not 0x01. */
	public static final int FMT_CPCDATA = 2;
/** 9 sectors, 2 sides, 80 tracks, 512 byte sectors. 
 * This corresponds to the PCW 720k format and the PCDOS 720k format. */
	public static final int FMT_720K = 3;
/** 18 sectors, 2 sides, 80 tracks, 512 byte sectors.
 * This corresponds to the PCDOS / PCW16 1440k formaat. */
	public static final int FMT_1440K = 4;
/** 8 sectors, 1 side, 40 tracks, 512 byte sectors.
 * This corresponds to the PCDOS 1.0 160k format. */
	public static final int FMT_160K = 5;
/** 8 sectors, 2 sides, 40 tracks, 512 byte sectors. 
 * This corresponds to the PCDOS 320k format. */
	public static final int FMT_320K = 6;
/** 9 sectors, 2 sides, 40 tracks, 512 byte sectors. 
 * This corresponds to the PCDOS 160k format. */
	public static final int FMT_360K = 7;
/** As FMT_720K, but the sides are out-and-back rather than alternating. 
   This format is used by 144FEAT for its CP/M-86 720k format. */
	public static final int FMT_720F = 8;
/** As FMT_720F, but with 15 sectors per track. 
   This format is used by 144FEAT for its CP/M-86 1200k format. */
	public static final int FMT_1200F = 9;	
/** As FMT_720F, but with 18 sectors per track. 
   This format is used by 144FEAT for its CP/M-86 1440k format. */
	public static final int FMT_1440F = 10;	
/** 16 sectors, 1 side, 40 tracks, 256 byte sectors. 
   First sector number is 0. This corresponds to the ADFS 'S' format. */
	public static final int FMT_ACORN160 = 11;
/** 16 sectors, 1 side, 80 tracks, 256 byte sectors. 
   First sector number is 0. This corresponds to the ADFS 'M' format. */
	public static final int FMT_ACORN320 = 12;
/** 16 sectors, 2 sides, 80 tracks, 256 byte sectors. 
   First sector number is 0. This corresponds to the ADFS 'L' format. */
	public static final int FMT_ACORN640 = 13;
/** 5 sectors, 2 sides, 80 tracks, 1024 byte sectors. 
   First sector number is 0. This corresponds to the ADFS 'D' and 'E' formats. */
	public static final int FMT_ACORN800 = 14;
/** 10 sectors, 2 sides, 80 tracks, 1024 byte sectors. 
   First sector number is 0. This corresponds to the ADFS 'F' format. */
	public static final int FMT_ACORN1600 = 15;
/** 10 sectors, 2 sides, 80 tracks, 512 byte sectors. 
   First sector number is 1. This corresponds to the PCW XCF2DD format. */
	public static final int FMT_800K = 16;
/** 10 sectors, 1 side, 40 tracks, 512 byte sectors. 
   First sector number is 1. This corresponds to the PCW XCF2 format. */
	public static final int FMT_200K = 17;
/** 10 sectors, 1 side, 40 tracks, 256 byte sectors, MFM. 
   First sector number is 0. This corresponds to the Acorn DFS 40-track format. */
	public static final int FMT_BBC100 = 18;
/** 10 sectors, 1 side, 80 tracks, 256 byte sectors, MFM.
   First sector number is 0. This corresponds to the Acorn DFS 80-track format. */
	public static final int FMT_BBC200 = 19;

/** Initialise a Geometry object with a standard format. 
 * @param formatId One of the format IDs provided by this class.
 * @param g The Geometry object to initialise. Can be null.
 * @param namedesc An array of 2 strings, which will be set to the
 *          name and description of the format. Can be null.
 * @exception DskException If the format ID is unrecognised. */
	public static native void stdFormat(int formatId, Geometry g, String[] namedesc)
		throws DskException; 
}


