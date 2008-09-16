/***************************************************************************
 *                                                                         *
 *    LIBDSK: General floppy and diskimage access library                  *
 *    Copyright (C) 2005  John Elliott <jce@seasip.demon.co.uk>            *
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

#ifndef __REPORTER_H_
#define __REPORTER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CReporter
class ATL_NO_VTABLE CReporter : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CReporter, &CLSID_Reporter>,
	public IDispatchImpl<IReporter, &IID_IReporter, &LIBID_LIBDSK>
{
public:
	CReporter()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_REPORTER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CReporter)
	COM_INTERFACE_ENTRY(IReporter)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IReporter
public:
	STDMETHOD(endreport)();
	STDMETHOD(report)(/*[in,string]*/ BSTR message);
};

#endif //__REPORTER_H_
