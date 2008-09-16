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

import uk.co.demon.seasip.libdsk.*;

/* Simple wrapper around dsk_getgeom() */

public class DskID 
{
	static void help()
	{
		System.err.println("Syntax: java DskID <dskimage> { -type <type> } { -side <side> }");
		System.err.println("Default type is autodetect.\n");
		System.err.println("eg: java DskID myfile.DSK");
		System.err.println("    java DskID /dev/fd0 -type floppy -side 1");	
		System.exit(1);
	}


	public static void main(String args[])
	{
		String outtyp = null;
		String outcomp = null;
		int forcehead = -1;

		LibDsk.setReporter(new ScreenReporter());

                if (UtilOpts.findArg("--help", args) >= 0) help();
                if (UtilOpts.findArg("--version", args) >= 0) UtilOpts.version();
                if (args.length < 1) help();

		outtyp    = UtilOpts.checkType("-type", args);
		outcomp   = UtilOpts.checkType("-comp", args);
		forcehead = UtilOpts.checkForceHead("-side", args);	

		Drive outdr = null;
		Geometry dg = new Geometry();
		byte drvStatus;

		try
		{
			outdr = LibDsk.open(args[0], outtyp, outcomp);
			outdr.setForceHead(forcehead);
			outdr.probeGeometry(dg);
				
			System.out.println("Driver:        " + outdr.getDriverDesc());
			System.out.println("Compression:   " + outdr.getCompressDesc());
			
			if (forcehead >= 0) System.out.println("[Forced to read from side"
					+ Integer.toString(forcehead) +" ]");
			System.out.println("Sidedness:     " + Integer.toString(dg.sidedness));
			System.out.println("Cylinders:     " + Integer.toString(dg.cylinders));
			System.out.println("Heads:         " + Integer.toString(dg.heads));
			System.out.println("Sectors:       " + Integer.toString(dg.sectors));
			System.out.println("First sector:  " + Integer.toString(dg.secbase));
			System.out.println("Sector size:   " + Integer.toString(dg.secsize));
			System.out.println("Data rate:     " + Integer.toString(dg.datarate));
			System.out.println("R/W gap:     0x" + Integer.toHexString(dg.rwgap));
			System.out.println("Format gap:  0x" + Integer.toHexString(dg.fmtgap));
			drvStatus = outdr.status(dg, 0);
			System.out.println("Drive status:  " + Integer.toHexString(drvStatus));
			if (outdr.getComment() != null)
				System.out.println("Comment:       " + outdr.getComment());
		}
		catch (DskException e)
		{
			System.err.println(e.getMessage());
		}

		try 
		{
			if (outdr != null) outdr.close(); 
		}
		catch (DskException e)
		{
			System.err.println(e.getMessage());
		}

	}
}

