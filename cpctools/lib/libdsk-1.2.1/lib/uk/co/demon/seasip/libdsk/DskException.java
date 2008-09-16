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

/** This exception is thrown when an underlying LibDsk function fails. */
public class DskException extends Exception
{
	private int errCode;

	DskException(String error, int nerr)
	{
		super(error);
		errCode = nerr;
	}
	/** Return the LibDsk error code that caused this exception. */
	public final int getError() { return errCode; }

	/** No error */
	public static final int DSK_ERR_OK	 =  0;	
	/** Bad pointer passed to LibDsk. */
	public static final int DSK_ERR_BADPTR	 =  -1;	
	/** Division by zero. */
	public static final int DSK_ERR_DIVZERO  =  -2;	
	/** Bad parameter passed to LibDsk. */
	public static final int DSK_ERR_BADPARM  =  -3;	
	/** No such LibDsk driver. */
	public static final int DSK_ERR_NODRVR   =  -4;	
	/** LibDsk driver rejected the disc. */
	public static final int DSK_ERR_NOTME    =  -5;	
	/** System call failed. */
	public static final int DSK_ERR_SYSERR   =  -6;	
	/** Out of memory */
	public static final int DSK_ERR_NOMEM  	 =  -7;	
	/** Function not implemented */
	public static final int DSK_ERR_NOTIMPL  =  -8;	
	/** Sector read did not match sector on disc */
	public static final int DSK_ERR_MISMATCH =  -9;	
	/** Drive not ready. */
	public static final int DSK_ERR_NOTRDY   =  -10;
	/** Drive is read-only. */
	public static final int DSK_ERR_RDONLY	 =  -11;
	/** Seek failed. */
	public static final int DSK_ERR_SEEKFAIL =  -12;
	/** Data error. */
	public static final int DSK_ERR_DATAERR  =  -13;
	/** No data. */
	public static final int DSK_ERR_NODATA   =  -14;
	/** Missing address mark. */
	public static final int DSK_ERR_NOADDR	 =  -15;
	/** Bad format. */
	public static final int DSK_ERR_BADFMT   =  -16;
	/** Disc changed while LibDsk was using it. */
	public static final int DSK_ERR_CHANGED  =  -19;	
	/** Equipment check. */
	public static final int DSK_ERR_ECHECK	 =  -20;	
	/** Overrun. */
	public static final int DSK_ERR_OVERRUN  =  -21;
	/** Access denied. */
	public static final int DSK_ERR_ACCESS   =  -22;
	/** Controller failed. */
	public static final int DSK_ERR_CTRLR 	 =  -23;
	/** Unknown error. */
	public static final int DSK_ERR_UNKNOWN  =  -99;
}


