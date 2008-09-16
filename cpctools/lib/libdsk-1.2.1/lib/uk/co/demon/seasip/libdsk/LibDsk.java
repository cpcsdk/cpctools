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

public class LibDsk
{
	static
	{
		try
		{
			System.loadLibrary("dsk");
		}
		catch (UnsatisfiedLinkError e)
		{
			System.err.println("Could not load LibDsk dynamic libraries.");
			System.exit(1);
		}
	}

	/* Dummy call to ensure that this class is loaded
	 * before Geometry, FormatType or Drive try to use native
	 * methods. */
	static void forceInit()
	{

	}

/** Get LibDsk version string.
 * @return The library version - eg, "0.9.0" */
	public static native String version();

/** Convert sector size to a physical sector shift as used by the controller.
 * To go the other way, size = 128 &lt;&lt; psh  
 * @param sectorSize Sector size in bytes: 128, 256, 512, ...
 * @return The corresponding sector shift. */
	public static native byte getPsh(int sectorSize);


/** Open a DSK file, POSIX image, drive or whatever. 
 * @param filename The file specification to open.
 * @param type The LibDsk driver to use. Can be null to autodetect.
 * @param comp The compression system to use. null to autodetect.
 * @exception DskException If the drive could not be opened. 
 * <p>Supported drivers are:
 * <dl>
 * <dt>null    <dd> Autodetect file type from name or magic number
 * <dt>"dsk"   <dd> CPCEMU DSK or EDSK format 
 * <dt>"raw"   <dd> Raw dd if=foo of=bar file
 * <dt>"floppy"<dd> Host system's floppy drive 
 * <dt>"myz80" <dd> MYZ80 image file
 * <dt>"cfi"   <dd> CFI image file
 * </dl>
 */
	public static native Drive open(String filename, String type, String comp) throws DskException;


/** Create a new DSK file, POSIX image, etc.
 * As for "open", but creates a new image file. On a floppy drive 
 * this will act exactly as "open"; for an image this will attempt to 
 * create the file new or truncate it.
 * Note that "type" cannot be NULL. 
 * @param filename The file specification to open.
 * @param type The LibDsk driver to use. Must not be null.
 * @param comp The compression type to use. null for uncompressed.
 * @exception DskException If the drive could not be created. */
	public static native Drive create(String filename, String type, String comp) throws DskException;


	private static Reporter rep = null;

/** Get the object to which progress messages will be sent. */
	public static Reporter getReporter()
	{
		return rep;
	}

	private static native void setReporter(boolean javamode);

/** Set the object that will be called with progress messages 
 *  @param r The object to which progress messages will be sent. */
	public static void setReporter(Reporter r)
	{
		rep = r;
		setReporter(r != null);
	}

/** Send a string to the current reporter, if any 
 *  @param s The string to print */
	public static void report(String s)
	{
		if (rep != null) rep.report(s);
	}


/** Remove any string displayed by the current reporter */
	public static void reportEnd()
	{
		if (rep != null) rep.reportEnd();
	}
}
