
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


/** The Drive class represents an open LibDsk drive. */
public class Drive
{
	/* The id is a Java handle to the underlying C object */
	private int id;

	private Drive(int id)
	{
		this.id = id;
	}


	/** Force the driver to use one or other head. 
 	 * <p>(This option is only effective for the Linux floppy driver)
	 * <p>Forces the driver to ignore the head number passed to it and 
	 * always use either side 0 or side 1 of the disc. This is used to 
	 * read discs recorded on PCW / CPC / Spectrum+3 add-on 3.5" drives. 
	 * Instead of the system software being programmed to use both sides 
	 * of the disc, a switch on the drive was used to set which side was 
	 * being used. Thus discs would end up with both sides saying they 
	 * were head 0. 
	 * @param f -1 for normal use, 0 to force head 0, 1 to force head 1. */
	public native void setForceHead(int f);
	/** Find out if the drive is being forced to use one or other head.
	 * @return -1 for normal use, 0 to force head 0, 1 to force head 1. */
	public native int getForceHead();
	
	/** Close a DSK file. 
	 *  After the underlying file has been closed, you may not use 
	 * this object further. Any attempt to use it will cause an 
	 * exception to be thrown.
	 * @exception DskException If the close failed for any reason. */
	public native void close() throws DskException;

/* Get drive status (Ready, Read-Only etc.). The actual status is 
 * based on the FDC's ST3 register. The following bits should be available: */

	/** When returned by status(), indicates drive fault.
	 * @see #status */	
	public static final int DSK_ST3_FAULT	= 0x80;	
	/** When returned by status(), indicates disc read only. *
	 * @see #status */	
	public static final int DSK_ST3_RO	= 0x40;	
	/** When returned by status(), indicates the drive is ready. *	
	 * @see #status */	
	public static final int DSK_ST3_READY	= 0x20;	
	/** When returned by status(), indicates the head is over track 0. *	
	 * @see #status */	
	public static final int DSK_ST3_TRACK0	= 0x10;	
	/** When returned by status(), indicates the drive is double-sided. *	
	 * @see #status */	
	public static final int DSK_ST3_DSDRIVE	= 0x08;	
	/** When returned by status(), indicates the current head is head 1. *	
	 * @see #status */	
	public static final int DSK_ST3_HEAD1	= 0x04;	

/** Get the status of the drive. Most drivers only implement the 
   DSK_ST3_READY and DSK_ST3_RO bits.
 * @param g The geometry to use for the drive.
 * @param head Which head to check (0 or 1). 
 * @return The drive status, in uPD765A format.
 * @exception DskException if there was any error. */
	public native byte status(Geometry g, int head) throws DskException;

/** Read a disc sector using a physical sector address.
  * @param g The drive geometry to use.
  * @param buf The buffer to be filled with data. 
  * @param cyl The physical cylinder containing the sector. 
  * @param head The physical head to use.
  * @param sector The number of the sector.
  * @exception DskException If the read failed for any reason, or the driver
  *                     cannot read sectors.
  */
	public native void read(Geometry g, byte[] buf, int cyl, int head, int sector) throws DskException;

/** Read a disc sector using a logical sector address.
  * @param g The drive geometry to use. This will be used to translate the
  *         sector number to a physical cylinder/head/sector.
  * @param buf The buffer to be filled with data. 
  * @param logsect The number of the sector (0 is the first sector on the disc).
  * @exception DskException If the read failed for any reason, or the driver cannot
  *                     read sectors.
  */
	public native void read(Geometry g, byte[] buf, int logsect) throws DskException;

/** Read a disc sector whose numbering may not match its disc location.
  * Not all drivers implement this function; those that don't will throw
  * a DskException with error DSK_ERR_NOTIMPL.
  * <p>Unless you are emulating a floppy controller, you should not need to 
  * use this call.
  * @param g The drive geometry to use.
  * @param buf The buffer to be filled with data. 
  * @param cyl The physical cylinder containing the sector. 
  * @param head The physical head to use.
  * @param cylExpected The expected cylinder number in the sector header.
  * @param cylExpected The expected head number in the sector header.
  * @param sector The number of the sector.
  * @param sectorLen The number of bytes to transfer (may be less than a 
  *                 full sector).
  * @param deleted On entry, deleted[0] is true to read deleted data, false 
  *               for normal. On return, it's true if the other sort of data
  *               was read, false if the expected sort was read.
  * @exception DskException If the read failed for any reason, or the driver
  *                  does not provide this call. */
	public native void read(Geometry g, byte[] buf, 
			int cyl, int head, 
			int cylExpected, int headExpected,
			int sector, int sectorLen, boolean[] deleted) throws DskException;

/** Write a disc sector using a physical sector address.
  * @param g The drive geometry to use.
  * @param buf The data to be written.
  * @param cyl The physical cylinder containing the sector. 
  * @param head The physical head to use.
  * @param sector The number of the sector.
  * @exception DskException If the write failed for any reason, or the driver
  *                     cannot write sectors. 
  */
	public native void write(Geometry g, byte[] buf, int cyl, int head, int sector) throws DskException;

/** Write a disc sector using a logical sector address.
  * @param g The drive geometry to use. This will be used to translate the
  *         sector number to a physical cylinder/head/sector.
  * @param buf The data to be written.
  * @param logsect The number of the sector (0 is the first sector on the disc).
  * @exception DskException If the write failed for any reason, or the driver cannot
  *                     write sectors.
  */
	public native void write(Geometry g, byte[] buf, int logsect) throws DskException;

/** Write a disc sector whose numbering may not match its disc location.
  * Not all drivers implement this function; those that don't will throw
  * a DskException with error DSK_ERR_NOTIMPL.
  * <p>Unless you are emulating a floppy controller, you should not need to 
  * use this call.
  * @param g The drive geometry to use.
  * @param buf The data to be written.
  * @param cyl The physical cylinder containing the sector. 
  * @param head The physical head to use.
  * @param cylExpected The expected cylinder number in the sector header.
  * @param cylExpected The expected head number in the sector header.
  * @param sector The number of the sector.
  * @param sectorLen The number of bytes to transfer (may be less than a 
  *                 full sector).
  * @param deleted True to write deleted data, otherwise false.
  * @exception DskException If the write failed for any reason, or the driver
  *                     does not provide this functionality.
  */
	public native void write(Geometry g, byte[] buf, 
			int cyl, int head, 
			int cylExpected, int headExpected,
			int sector, int sectorLen, boolean deleted) throws DskException;

/** Verify a disc sector using a physical sector address.
  * @param g The drive geometry to use.
  * @param buf The buffer to compare with what's on the disc. 
  * @param cyl The physical cylinder containing the sector. 
  * @param head The physical head to use.
  * @param sector The number of the sector.
  * @exception DskException If the read failed for any reason; DSK_ERR_MISMATCH
  *                     if the data read don't match the buffer.
  */
	public native void verify(Geometry g, byte[] buf, int cyl, int head, int sector) throws DskException;

/** Verify a disc sector using a logical sector address.
  * @param g The drive geometry to use. This will be used to translate the
  *         sector number to a physical cylinder/head/sector.
  * @param buf The buffer to compare with what's on the disc. 
  * @param logsect The number of the sector (0 is the first sector on the disc).
  * @exception DskException If the read failed for any reason; DSK_ERR_MISMATCH
  *                     if the data read don't match the buffer.
  */
	public native void verify(Geometry g, byte[] buf, int logsect) throws DskException;

/** Verify a disc sector whose numbering may not match its disc location.
  * Not all drivers implement this function; those that don't will throw
  * a DskException with error DSK_ERR_NOTIMPL.
  * <p>Unless you are emulating a floppy controller, you should not need to 
  * use this call.
  * @param g The drive geometry to use.
  * @param buf The data to be compared with what's on disc.
  * @param cyl The physical cylinder containing the sector. 
  * @param head The physical head to use.
  * @param cylExpected The expected cylinder number in the sector header.
  * @param cylExpected The expected head number in the sector header.
  * @param sector The number of the sector.
  * @param sectorLen The number of bytes to transfer (may be less than a 
  *                 full sector).
  * @exception DskException If the read failed for any reason; DSK_ERR_MISMATCH
  *                     if the data read don't match the buffer.
  */
	public native void verify(Geometry g, byte[] buf, 
			int cyl, int head, 
			int cylExpected, int headExpected,
			int sector, int sectorLen) throws DskException;
/* Format a track.
 * Note that the geometry in these functions is not const; the CPCEMU driver
 * will increase the number of tracks/heads as the disc image file outgrows
 * the geometry.
 */
/** Format a track, using physical cylinder/sector.
 *
 * @param g The drive geometry to use.
 * @param c The physical cylinder to use. 
 * @param h The physical head to use.
 * @param fd An array of FormatData objects containing sector headers.
 * @param filler The byte with which to fill the empty sectors. 
 * @exception DskException If the format failed for any reason, or the driver cannot
 *                     format tracks. */
	public native void format(Geometry g, int cyl, int h, FormatData fd[], byte filler) throws DskException;
/** Format a track, generating the sector headers automatically.
 * The resulting track headers will be correct for standard DOS, PCW or 
 * Linux floppies.
 *
 * @param g The drive geometry to use.
 * @param track The logical track to format.
 * @param fd An array of FormatData objects containing sector headers.
 * @param filler The byte with which to fill the empty sectors. 
 * @exception DskException If the format failed for any reason, or the driver cannot
 *                     format tracks. */
	public native void format(Geometry g, int track, FormatData fd[], byte filler) throws DskException;

/** Read a track using a physical head/cylinder number.
  * This will use the FDC's READ TRACK command if it is available; otherwise
  * it will just do repeated sector reads.
  * @param g The drive geometry to use. 
  * @param buf The buffer to be filled with data. 
  * @param cyl The physical cylinder to use. 
  * @param head The physical head to use.
  * @exception DskException If the read failed for any reason, or the driver cannot
  *                     read sectors.
  */
	public native void readTrack(Geometry g, byte buf[], int cylinder, int head) throws DskException;

/** Read a track using a logical track number.
  * This will use the FDC's READ TRACK command if it is available; otherwise
  * it will just do repeated sector reads.
  * @param g The drive geometry to use. This will be used to translate the
  *         track number to a physical cylinder/head.
  * @param buf The buffer to be filled with data. 
  * @param track The number of the track (0 is the first track).
  * @exception DskException If the read failed for any reason, or the driver cannot
  *                     read sectors.
  */
	public native void readTrack(Geometry g, byte buf[], int track) throws DskException;

/** Read a track whose sector headers may not match its location.
  * This will use the FDC's READ TRACK command if it is available; otherwise
  * it will just do repeated sector reads.
  * <p>Not all drivers implement this function; those that don't will throw
  * a DskException with error DSK_ERR_NOTIMPL.
  * <p>Unless you are emulating a floppy controller, you should not need to 
  * use this call.
  * @param g The drive geometry to use. 
  * @param buf The buffer to be filled with data. 
  * @param cyl The physical cylinder to use. 
  * @param head The physical head to use.
  * @param cylExpected The expected cylinder number in the sector header.
  * @param cylExpected The expected head number in the sector header.
  * @exception DskException If the read failed for any reason, or the driver cannot
  *                     read sectors.
  */
	public native void readTrack(Geometry g, byte buf[], int cylinder, int head, int cylExpected, int headExpected) throws DskException;

/** Format a track, generating the sector headers automatically.
 * The resulting track headers will be correct for standard DOS, PCW or 
 * Linux floppies.
 *
 * @param g The drive geometry to use.
 * @param cyl The physical cylinder to use. 
 * @param head The physical head to use.
 * @param filler The byte with which to fill the empty sectors. 
 * @exception DskException If the format failed for any reason, or the driver cannot
 *                     format tracks. */
	public native void autoFormat(Geometry g, int cylinder, int head, byte filler) throws DskException;

/** Format a track, generating the sector headers automatically.
 * @param g The drive geometry to use.
 * @param track The logical track number to format.
 * @param filler The byte with which to fill the empty sectors. 
 * @exception DskException If the format failed for any reason, or the driver cannot 
 *                     format tracks. */
	public native void autoFormat(Geometry g, int track, byte filler) throws DskException;

/** Probe the geometry of a disc. This will use the boot sector and any
 * information the driver can give. 
 * @param g A newly-created Geometry object. This will be populated on 
 *         return.
 * @exception DskException DSK_ERR_BADFMT if the format can't be detected;
 *                     other values if there's a read error.
 **/
	public native void probeGeometry(Geometry g) throws DskException;

/** Read a random sector ID. This can be used to probe for discs
 * with oddly-numbered sectors (eg, numbered 65-74).
 *<p>Note that the Win32c driver implements a limited version of this call,
 * which will work on normal DOS / CP/M86 / PCW discs and CPC discs. However
 * it will not be usable for other purposes.
 * @param g The geometry for the drive.
 * @param track The logical track on which to look for sectors.
 * @param result A new FormatData object which will be populated with the
 *              sector header that was found. 
 * @exception DskException if the driver does not provide this functionality
 *              or no sector was found. */
	public native void sectorID(Geometry g, int track, FormatData result) throws DskException;

/** Read a random sector ID. This can be used to probe for discs
 * with oddly-numbered sectors (eg, numbered 65-74).
 *<p>Note that the Win32c driver implements a limited version of this call,
 * which will work on normal DOS / CP/M86 / PCW discs and CPC discs. However
 * it will not be usable for other purposes.
 * @param g The geometry for the drive.
 * @param cyl The physical cylinder on which to look for sectors.
 * @param head The physical head on which to look for sectors.
 * @param result A new FormatData object which will be populated with the
 *              sector header that was found. 
 * @exception DskException if the driver does not provide this functionality
 *              or no sector was found. */
	public native void sectorID(Geometry g, int cyl, int head, FormatData result) throws DskException;

/** Seek to a track using a logical track number.
  * Unless you are emulating a floppy controller, you should not need to 
  * use this call.
  * @param g The drive geometry to use. This will be used to translate the
  *         track number to a physical cylinder/head.
  * @param track The number of the track (0 is the first track).
  * @exception DskException If the seek failed; DSK_ERR_NOTIMPL if the 
  *               underlying driver doesn't support this. */
	public native void seek(Geometry g, int track) throws DskException;

/** Seek to a track using a physical cylinder/head number.
  * Unless you are emulating a floppy controller, you should not need to 
  * use this call.
  * @param g The drive geometry to use. 
  * @param cyl The cylinder to seek to.
  * @param head The head to use.
  * @exception DskException If the seek failed; DSK_ERR_NOTIMPL if the 
  *               underlying driver doesn't support this. */
	public native void seek(Geometry g, int cyl, int head) throws DskException;

/** Get the name of a built-in driver.
 * 
 * @param index If the index is in range, return the n'th driver name.
 * @return The driver name; null if index is out of range. 
 */
	public static native String enumDrivers(int index);

/** Get the name of the driver being used for this drive. 
 * for example: "dsk". */
	public native String getDriverName();
/** Get the description of the driver being used for this drive. 
 * for example: "CPCEMU .DSK driver" */
	public native String getDriverDesc();
/** Get the name of the compression scheme being used. 
 * for example: "gz". If uncompressed returns null */
	public native String getCompressName();
/** Get the description of the compression scheme being used for this drive. 
 * for example: "Squeezed (Huffman coding)" */
	public native String getCompressDesc();

/** Get the comment for this file (if the file format supports it). 
 * Currently only the APRIDISK format supports comments. Other file
 * formats will return null.
 *
 * @return The comment string - null if there is no comment string. 
 * @exception DskException If there is an error. */
	public native String getComment() throws DskException;

/** Set the comment for this file (if the file format supports it). 
 * Currently only the APRIDISK format supports comments. Other file
 * formats will return null.
 *
 * @param c The comment string - null if there is to be no comment string. 
 * @exception DskException If there is an error. */
	public native void setComment(String c) throws DskException;

/** Set the number of times an operation will be retried.
 * @param n The number of times to retry a read, write or format, 1 or more. 
 * @exception DskException If the number passed is 0. */
	public native void setRetry(int c) throws DskException;	

/** Get the number of times an operation will be retried.
 * @return The number of times to a read, write or format will be retried. 
 * @exception DskException On internal error. */
	public native int getRetry() throws DskException;	

}


