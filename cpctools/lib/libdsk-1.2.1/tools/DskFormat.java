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

/* Example formatter */

import uk.co.demon.seasip.libdsk.*;

class DskFormat
{
	static byte spec169 [] = { 0,    0, 40, 9, 2, 2, 3, 2, 0x2A, 0x52 };
	static byte spec180 [] = { 0,    0, 40, 9, 2, 1, 3, 2, 0x2A, 0x52 };
	static byte spec200 [] = { 0,    0, 40,10, 2, 1, 3, 3, 0x0C, 0x17 };
	static byte spec720 [] = { 3, (byte)0x81, 80, 9, 2, 1, 4, 4, 0x2A, 0x52 };
	static byte spec800 [] = { 3, (byte)0x81, 80,10, 2, 1, 4, 4, 0x0C, 0x17 };

	static void help()
	{
		System.err.println("Syntax: \n" +
                "      java DskFormat dskimage { -format <format> } " +
                " { -type <type> } { -side <side> }");
		System.err.println("\nDefault type is DSK.\nDefault format is PCW 180k.\n");
		
		System.err.println("eg: java DskFormat myfile.DSK\n" +
                              "    java DskFormat /dev/fd0 -type floppy -format cpcsys -side 1");

		FormatNames.validFormats();
		System.exit(1);
	}


	public static void main(String args[])
	{
		String outType, outComp;
		int forceHead;
		int format;

                LibDsk.setReporter(new ScreenReporter());

		if (UtilOpts.findArg("--help", args) >= 0) help();	
		if (UtilOpts.findArg("--version", args) >= 0) UtilOpts.version();	
		if (args.length < 1) help(); 
		
		outType = UtilOpts.checkType("-type", args);
		outComp = UtilOpts.checkType("-comp", args);
		if (outType == null) outType = "dsk";
		forceHead = UtilOpts.checkForceHead("-side", args);
		format    = FormatNames.checkFormat("-format", args);
		if (format == -1) format = FormatType.FMT_180K;

		doFormat(args[0], outType, outComp, forceHead, format);
	}

	private static void doFormat(String outfile, String outtyp, String compress, int forceHead, int format)
	{
		Drive outdr = null;
		int cyl, head;
		Geometry dg = new Geometry();
		String desc[] = new String[2];
	
		try
		{
			outdr = LibDsk.create(outfile, outtyp, compress);
			outdr.setForceHead(forceHead);
			FormatType.stdFormat(format, dg, desc);
	
			System.out.println("Formatting " + outfile + " as " +
				desc[1]);
			for (cyl = 0; cyl < dg.cylinders; cyl++)
			{
			    for (head = 0; head < dg.heads; head++)
			    {
				System.out.print("Cylinder " + 
					Integer.toString(cyl + 1) + "/" +
					Integer.toString(dg.cylinders) + " Head " +
					Integer.toString(head + 1) + "/" +
					Integer.toString(dg.heads) + "     \r");
				System.out.flush();
	
				outdr.autoFormat(dg, cyl, head, (byte)0xE5);
			    }
			}
		/* Create a disc spec on the first sector, if the format's recognised */
			byte bootsec[] = new byte[512];
			boolean do_bootsec = false;
			int n;
	
			for (n = 0; n < 512; n++) bootsec[n] = (byte)0xE5;
			switch(format)
			{
				case FormatType.FMT_180K:
					for (n = 0; n < spec180.length; n++) 
						bootsec[n] = spec180[n];
					do_bootsec = true;
					break;
				case FormatType.FMT_CPCSYS:
					for (n = 0; n < spec169.length; n++) 
						bootsec[n] = spec169[n];
					do_bootsec = true;
					break;
				case FormatType.FMT_CPCDATA:
					break;
				case FormatType.FMT_200K:
					for (n = 0; n < spec200.length; n++) 
						bootsec[n] = spec200[n];
					do_bootsec = true;
					break;
				case FormatType.FMT_720K:
					for (n = 0; n < spec720.length; n++) 
						bootsec[n] = spec720[n];
					do_bootsec = true;
					break;
				case FormatType.FMT_800K:
					for (n = 0; n < spec800.length; n++) 
						bootsec[n] = spec800[n];
					do_bootsec = true;
					break;
				default:
					break;
			}
			if (do_bootsec) outdr.write(dg, bootsec, 0);
			outdr.close();
		}
		catch (DskException e)
		{
			if (outdr != null)
			{
				try { outdr.close(); } catch (DskException e2) {}
			}	
			System.out.print("\r                                         \r");
			System.err.println(e.getMessage());
		}
		System.out.print("\r                                         \r");
	}
}	

