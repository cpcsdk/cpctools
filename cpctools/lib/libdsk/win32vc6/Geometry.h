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

#ifndef __GEOMETRY_H_
#define __GEOMETRY_H_

#include "resource.h"       // main symbols
#include "libdsk.h"

/////////////////////////////////////////////////////////////////////////////
// CGeometry
class ATL_NO_VTABLE CGeometry : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGeometry, &CLSID_Geometry>,
	public ISupportErrorInfo,
	public IDispatchImpl<IGeometry, &IID_IGeometry, &LIBID_LIBDSK>
{
public:
	CGeometry()
	{
	}

	DSK_GEOMETRY m_g;
	HRESULT MapError(dsk_err_t err);

DECLARE_REGISTRY_RESOURCEID(IDR_GEOMETRY)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGeometry)
	COM_INTERFACE_ENTRY(IGeometry)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IGeometry
public:
	STDMETHOD(stdformat)(/*[in]*/ int formatid, /*[out]*/ BSTR *name, /*[out]*/ BSTR *description);
	STDMETHOD(ps2ls)(/*[in]*/ long cyl, /*[in]*/ short head, /*[in]*/ short sector, /*[out,retval]*/ long *lsect);
	STDMETHOD(pt2lt)(/*[in]*/ long cyl, /*[in]*/ short head, /*[out,retval]*/ long *lsect);
	STDMETHOD(ls2ps)(/*[in]*/ long lsect, /*[out]*/ long *cyl, /*[out]*/ short *head, /*[out]*/ short *sector);
	STDMETHOD(lt2pt)(/*[in]*/ long ltrack, /*[out]*/ long *cyl, /*[out]*/ short *head);
	STDMETHOD(get_noskip)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_noskip)(/*[in]*/ short newVal);
	STDMETHOD(get_nomulti)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_nomulti)(/*[in]*/ short newVal);
	STDMETHOD(get_fm)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_fm)(/*[in]*/ short newVal);
	STDMETHOD(get_fmtgap)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_fmtgap)(/*[in]*/ short newVal);
	STDMETHOD(get_rwgap)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_rwgap)(/*[in]*/ short newVal);
	STDMETHOD(get_secsize)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_secsize)(/*[in]*/ long newVal);
	STDMETHOD(get_datarate)(/*[out, retval]*/ DataRate *pVal);
	STDMETHOD(put_datarate)(/*[in]*/ DataRate newVal);
	STDMETHOD(get_secbase)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_secbase)(/*[in]*/ short newVal);
	STDMETHOD(get_sectors)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_sectors)(/*[in]*/ short newVal);
	STDMETHOD(get_heads)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_heads)(/*[in]*/ short newVal);
	STDMETHOD(get_cylinders)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_cylinders)(/*[in]*/ long newVal);
	STDMETHOD(get_sidedness)(/*[out, retval]*/ Sidedness *pVal);
	STDMETHOD(put_sidedness)(/*[in]*/ Sidedness newVal);
};

#endif //__GEOMETRY_H_
