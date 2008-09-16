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


/** The Geometry class holds information used to convert physical to/from logical
 * sectors and to access the disc. 
 */
public class Geometry
{
        static
        {
                LibDsk.forceInit();
        }

/* Disc sidedness (logical/physical mapping). Use SIDES_ALT for single-sided floppies. */
	/** Track n is cylinder (n/heads) head (n%heads) */
	public static final int SIDES_ALT = 0;
	/** Tracks go out and back. Like this:
	<pre>
(head 0) 0,1,2,3,...37,38,39, then
(head 1) 39,38,37,...,2,1,0</pre> */
	public static final int SIDES_OUTBACK = 1;
	/** Tracks cover all side 0, then all side 1. Like this:
	<pre>
(head 0) 0,1,2,3,...37,38,39, then
(head 1) 0,1,2,3....37,38,39</pre> */
	public static final int SIDES_OUTOUT = 2;


	/** Data rate for 1.4Mb 3.5"  in 3.5"  drive */
	public static final int RATE_HD = 0;
	/** Data rate for 360k  5.25" in 1.2Mb drive */
	public static final int RATE_DD = 1;	
	/** Data rate for 720k  3.5"  in 3.5"  drive */
	public static final int RATE_SD = 2;
	/** Data rate for 2.8Mb 3.5"  in 3.5"  drive */
	public static final int RATE_ED = 3;

	/** How to handle discs with 2 or more heads. 
	 * @see #SIDES_ALT
	 * @see #SIDES_OUTBACK
	 * @see #SIDES_OUTOUT */
	public int	sidedness;
	/** Number of cylinders in this disc. */
	public int	cylinders;
	/** Number of heads (sides) on this disc. */
	public int 	heads;
	/** Sectors per track. */
	public int	sectors;
	/** First physical sector number. */
	public int	secbase;
	/** Sector size in bytes. */
	public int	secsize;
	/** Data rate. 
	 * @see #RATE_DD
	 * @see #RATE_HD
	 * @see #RATE_SD
	 * @see #RATE_ED */
	public int	datarate;
	/** Read/write gap length */
	public byte	rwgap;
	/** Format gap length */
	public byte	fmtgap;
	/** True if this disc Uses FM recording mode. 
	 * The only disc I know of that are recorded in FM mode are BBC 
	 * Micro DFS floppies. */
	public boolean fm;	
	/** True to disable multitrack operation. */
	public boolean nomulti;	
	/** True not to skip deleted data. */
	public boolean noskip;	

	/** Convert physical cylinder/head/sector to logical sector. 
	 * @param cyl Physical cylinder.
	 * @param head Physical head.
	 * @param sect Physical sector.
	 * @return The corresponding logical sector number.
	 * @exception DskException Thrown if the cylinder, head or sector are out of range. */
	public native int ps2ls(int cyl, int head, int sect) throws DskException;

	/** Convert logical sector to physical cylinder / head /sector.
	 *  @param logical The logical sector number to convert.
	 *  @param chs An array of three integers which will receive cylinder, head and sector.
	 *  @exception DskException Thrown if the logical sector is out of range. */
	public native void ls2ps(int logical, int[] chs) throws DskException;

	/** Convert physical cylinder/head to logical track.
	 * @param cyl Physical cylinder.
	 * @param head Physical head.
	 * @return The corresponding logical track number.
	 * @exception DskException Thrown if the cylinder or head are out of range. */
	public native int pt2lt(int cyl, int head) throws DskException;

	/** Convert logical track to physical cylinder/head.
	 *  @param logical The logical track number to convert.
	 *  @param ch An array of two integers which will receive cylinder and head.
	 *  @exception DskException Thrown if the track is out of range. */
	public native void dg_lt2pt(int logical, int[] ch) throws DskException;

	/** Initialise this class from a DOS boot sector. 
	 * @param bootsect The boot sector to read the disc geometry from
	 * @exception DskException Thrown if the boot sector is not a valid
	 *                         DOS boot sector.*/
        public native void dosGeometry(byte[] bootsect) throws DskException;

	/** Initialise this class from an Apricot boot sector. 
	 * @param bootsect The boot sector to read the disc geometry from
	 * @exception DskException Thrown if the boot sector is not a valid
	 *                         DOS boot sector.*/
        public native void apricotGeometry(byte[] bootsect) throws DskException;

	/** Initialise this class from a PCW boot sector. 
	 * @param bootsect The boot sector to read the disc geometry from
	 * @exception DskException Thrown if the boot sector is not a valid
	 *                         PCW boot sector.*/

        public native void pcwGeometry(byte[] bootsect) throws DskException;

	/** Initialise this class from a CP/M-86 boot sector. 
	 * @param bootsect The boot sector to read the disc geometry from
	 * @exception DskException Thrown if the boot sector is not a valid
	 *                         CP/M-86 boot sector.*/
        public native void cpm86Geometry(byte[] bootsect) throws DskException;

}

