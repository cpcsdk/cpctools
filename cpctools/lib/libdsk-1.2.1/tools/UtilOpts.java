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

class UtilOpts
{
	static void version()
	{
		System.out.print("libdsk version ");
		System.out.println(uk.co.demon.seasip.libdsk.LibDsk.version());
		System.exit(0);
	}


	static int findArg(String arg, String args[])
	{
		int n;

		for (n = 0; n < args.length; n++)
		{
			if (arg.equalsIgnoreCase(args[n])) return n;
		}
		return -1;
	}



	static int checkForceHead(String arg, String args[])
	{
		int n = findArg(arg, args);

		if (n < 0) return -1;	
		++n;
		if (n >= args.length || 
		         args[n].charAt(0) < '0' || 
			 args[n].charAt(1) > '1') 
		{
			System.err.println("Syntax error: use '" + arg + 
					   "0' or '" + arg + " 1'");
			System.exit(1);
		}
		return args[n].charAt(0) - '0';
	}

	static String checkType(String arg, String args[])
	{
		int n = findArg(arg, args);

		if (n < 0) return null;
		++n;
		if (n >= args.length)
		{
			System.err.println("Syntax error: use '" + arg + " <type>'");
			System.exit(1);
		}
		if ("auto".equalsIgnoreCase(args[n])) return null;
		return args[n];
	}
}

