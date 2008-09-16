/*
 ***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2001-2  John Elliott <jce@seasip.demon.co.uk>          *
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

/* Portable equivalent of PCWTRANS */
import uk.co.demon.seasip.libdsk.*;

class DskTrans
{
	static boolean md3     = false;
	static boolean logical = false;
	static int     format  = -1;

	static void help()
	{
		System.err.println("Syntax: \n" +
                       "      java DskTrans in-image out-image { options }");
		System.err.println("\nOptions are:\n" +
			       "-itype <type>   type of input disc image\n" +
                               "-otype <type>   type of output disc image\n" +
                               "-iside <side>   Force side 0 or side 1 of input\n" +
                               "-oside <side>   Force side 0 or side 1 of output\n" +
                               "-md3            Defeat MicroDesign 3 copy protection\n" +
			       "-logical        Rearrange tracks in logical order\n" +
                               "                (Only useful when out-image type is 'raw' and reading discs\n" +
			       "                with non-IBM track layout (eg: 144FEAT 1.4Mb or ADFS 640k)\n" +
			       "-format         Force a specified format name");
		System.err.println("\nDefault in-image type is autodetect." +
		             		   "\nDefault out-image type is DSK.\n");
		System.err.println("eg: java DskTrans /dev/fd0 myfile1.DSK\n" +
                                "    java DskTrans /dev/fd0 myfile2.DSK -side 1\n" +
                                "    java DskTrans /dev/fd0 md3boot.dsk -md3\n" +
                                "    java DskTrans myfile.DSK /dev/fd0 -otype floppy"); 
               FormatNames.validFormats();
               System.exit(1);
	}


	public static void main(String args[])
	{
		String intyp, outtyp;
		String incomp, outcomp;
		int inside, outside;

                LibDsk.setReporter(new ScreenReporter());

                if (UtilOpts.findArg("--help", args) >= 0) help();
                if (UtilOpts.findArg("--version", args) >= 0) UtilOpts.version();
		if (args.length < 2) help();

		intyp     = UtilOpts.checkType("-itype", args); 
	        outtyp    = UtilOpts.checkType("-otype", args);
		incomp    = UtilOpts.checkType("-icomp", args); 
	        outcomp   = UtilOpts.checkType("-ocomp", args);
       		inside    = UtilOpts.checkForceHead("-iside", args);
	        outside   = UtilOpts.checkForceHead("-oside", args);
		if (UtilOpts.findArg("-md3", args) > 0) md3 = true;
		if (UtilOpts.findArg("-logical", args) > 0) logical = true;
		if (outtyp == null) outtyp = "dsk";
 	        format    = FormatNames.checkFormat("-format", args);
		
		doCopy(args[0], args[1], intyp, outtyp, 
				incomp, outcomp, inside, outside);
	}


	static void doCopy(String infile, String outfile, 
		    String intyp,  String outtyp,
		    String incomp, String outcomp,
		    int inside, int outside)
	{
		Drive indr = null, outdr = null;
		int cyl, head, sec;
		byte[] buf = null;
		Geometry dg = new Geometry();
		String op = "Opening";

		try
		{
			indr  = LibDsk.open(infile, intyp, incomp);
			indr.setForceHead(inside);
			outdr = LibDsk.create(outfile, outtyp, outcomp);
			outdr.setForceHead(outside);
			outdr.setComment(indr.getComment());			
			if (format == -1)
			{
				op = "Identifying disc";
				indr.probeGeometry(dg);
			}
			else FormatType.stdFormat(format, dg, null);
			buf = new byte[dg.secsize];
	
			System.out.println("Input driver: " + indr.getDriverDesc());
			System.out.println("Output driver:" + outdr.getDriverDesc());
			if (logical) System.out.println("[tracks rearranged]");

			for (cyl = 0; cyl < dg.cylinders; ++cyl)
			{
			    for (head = 0; head < dg.heads; ++head)
			    {
       	               		if (md3)
                        	{
			// MD3 discs have 256-byte sectors in cyl 1 head 1
				    if (cyl == 1 && head == 1) dg.secsize = 256;
				    else		       dg.secsize = 512;
                        	}
				op = "Formatting";
				// Format track
       				outdr.autoFormat(dg, cyl, head, (byte)0xE5);

				for (sec = 0; sec < dg.sectors; ++sec)
				{
				System.out.print("Cyl " +
					Integer.toString(cyl + 1)      + "/" +
					Integer.toString(dg.cylinders) + " Head " +
					Integer.toString(head + 1)     + "/" +
					Integer.toString(dg.heads)     + " Sector " +
					Integer.toString(sec + dg.secbase) + "/" +
					Integer.toString(dg.sectors + dg.secbase) + 
					"                \r");
				System.out.flush();
			
				op = "Reading";	
				try
				{
					if (logical)
					{
						int ls, si;

	/* Convert sector to logical using SIDES_ALT sidedness. Raw DSKs 
	 * are always created so that the tracks are stored in SIDES_ALT 
	 * order. */
						si = dg.sidedness;
						dg.sidedness = Geometry.SIDES_ALT;
						ls = dg.ps2ls(cyl, head, sec);
						dg.sidedness = si;
						indr.read(dg, buf, ls);
					}
					else indr.read(dg, buf, cyl,head,sec + dg.secbase);
				}
	// MD3 discs have deliberate bad sectors in cyl 1 head 1. Ignore these
	// errors.
				catch (DskException ex)
				{
					if (!md3) throw(ex);
					if (dg.secsize != 256) throw(ex);
					if (ex.getError() != DskException.DSK_ERR_DATAERR) throw(ex);
				}
				op = "Writing";
				outdr.write(dg, buf,cyl,head, sec + dg.secbase);
		 	    }
		        }
		    }
		}	
		catch (DskException e)
		{
			System.err.println(op + ": " + e.getMessage());
		}
		System.out.print("\r                                     \r");
		try
		{
			if (indr  != null) indr.close();
			if (outdr != null) outdr.close();
		}	
		catch (DskException e)
		{
			System.err.println(op + ": " + e.getMessage());
		}
	}
}

