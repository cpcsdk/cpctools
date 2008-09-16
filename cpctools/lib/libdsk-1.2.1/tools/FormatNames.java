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

/* Query the library for supported formats, and select one */

import uk.co.demon.seasip.libdsk.*;

class FormatNames
{
	static void validFormats()
	{
		int fmt = FormatType.FMT_180K;
		String fn[] = new String[2];

		System.err.println("\nValid formats are:");

		/* Continue until a 'format out of range' exception is
		 * generated. */
		try
		{
			while (true)
			{
				FormatType.stdFormat(fmt++, null, fn);
				System.err.println(fn[0] + " : " + fn[1]);
			}
		}
		catch (DskException e) 
		{

		}
	}

	static int checkFormat(String arg, String args[])
	{
		int fmt;
		int n = UtilOpts.findArg(arg, args);

		if (n < 0) return -1;
		++n;
		if (n >= args.length) 
		{
			System.err.println("Syntax error: use '" + 
						arg + "<format>'");
			System.exit(1);
		}
		try
		{	
			fmt = FormatType.FMT_180K;
			String fname[] = new String[2];

			while (true)
			{
				FormatType.stdFormat(fmt, null, fname);
				if (fname[0].equals(args[n])) return fmt;
				++fmt;
			}
		}
		catch (DskException e)
		{
			System.err.println("Format name " + args[n] +
					" not recognised.");
			System.exit(1);
		}
	return FormatType.FMT_180K;
	}
}


