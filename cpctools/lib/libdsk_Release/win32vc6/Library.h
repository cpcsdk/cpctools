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

#ifndef __LIBRARY_H_
#define __LIBRARY_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CLibrary
class ATL_NO_VTABLE CLibrary : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLibrary, &CLSID_Library>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILibrary, &IID_ILibrary, &LIBID_LIBDSK>
{
public:
	CLibrary()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LIBRARY)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CLibrary)
	COM_INTERFACE_ENTRY(ILibrary)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ILibrary
public:
	STDMETHOD(get_reporter)(/*[out, retval]*/ IDispatch* *pVal);
	STDMETHOD(put_reporter)(/*[in]*/ IDispatch* newVal);
	STDMETHOD(comp_enum)(/*[in]*/ long idx, /*[out]*/ BSTR *name, /*[out,retval]*/ BOOL *valid);
	STDMETHOD(type_enum)(/*[in]*/ long idx, /*[out]*/ BSTR *name, /*[out,retval]*/ BOOL *valid);
	STDMETHOD(dosgeom)(/*[in]*/ VARIANT *buffer, /*[out]*/ IGeometry **geom);
	STDMETHOD(cpm86geom)(/*[in]*/ VARIANT *buffer, /*[out]*/ IGeometry **geom);
	STDMETHOD(pcwgeom)(/*[in]*/ VARIANT *buffer, /*[out]*/ IGeometry **geom);
	STDMETHOD(aprigeom)(/*[in]*/ VARIANT *buffer, /*[out]*/ IGeometry **geom);
	STDMETHOD(get_stdformat_count)(/*[out, retval]*/ long *pVal);
//	STDMETHOD(get_reporter)(/*[out]*/ IReporter **ppv);
//	STDMETHOD(set_reporter)(/*[in]*/ IReporter *pv);
	STDMETHOD(get_psh)(/*[in]*/ long sectorsize, /*[out,retval]*/ short *shift);
	STDMETHOD(stdformat)(/*[in]*/ int formatid, /*[out]*/ BSTR *name, /*[out]*/ BSTR *description, IGeometry **ppGeom);
	STDMETHOD(create)(/*[in,string]*/ BSTR filename, /*[in,string]*/ BSTR driver, /*[in,string]*/ BSTR compress, /*[out]*/ IDisk **ppDisk);
	STDMETHOD(open)(/*[in,string]*/ BSTR filename, /*[in, string]*/ BSTR driver, /*[in,string]*/ BSTR compress, 
		/*[out, result]*/ IDisk **ppDisk);

private:	
	HRESULT MapError(int err);
	HRESULT GetBuffer(VARIANT *buf, unsigned char **bufout, unsigned length);
};

#endif //__LIBRARY_H_
