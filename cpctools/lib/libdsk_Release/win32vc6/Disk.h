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

#ifndef __DISK_H_
#define __DISK_H_

#include "resource.h"       // main symbols
#include "libdsk.h"

/////////////////////////////////////////////////////////////////////////////
// CDisk
class ATL_NO_VTABLE CDisk : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDisk, &CLSID_Disk>,
	public ISupportErrorInfo,
	public IDispatchImpl<IDisk, &IID_IDisk, &LIBID_LIBDSK>
{
public:
	CDisk()
	{
		m_driver = NULL;
	}
	~CDisk();

	DSK_PDRIVER m_driver;

DECLARE_REGISTRY_RESOURCEID(IDR_DISK)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDisk)
	COM_INTERFACE_ENTRY(IDisk)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IDisk
public:
	STDMETHOD(get_compdesc)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_compname)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_drvdesc)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_drvname)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_retries)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_retries)(/*[in]*/ short newVal);
	STDMETHOD(option_enum)(/*[in]*/ long idx, /*[out]*/ BSTR *name, /*[out,retval]*/ BOOL *valid);
	STDMETHOD(get_option)(/*[in]*/ BSTR name, /*[out, retval]*/ long *pVal);
	STDMETHOD(put_option)(/*[in]*/ BSTR name, /*[in]*/ long newVal);
	STDMETHOD(get_comment)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_comment)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_dirty)(/*[out, retval]*/ short *pVal);
	STDMETHOD(pseek)(/*[in]*/ IGeometry *geom, /*[in]*/ long cylinder, /*[in]*/ short sector);
	STDMETHOD(lseek)(/*[in]*/ IGeometry *geom, /*[in]*/ long track);
	STDMETHOD(ltrkid)(/*[in]*/ IGeometry *g, /*[in]*/ long track, short *count, /*[out]*/ VARIANT *buffer);
	STDMETHOD(ptrkid)(/*[in]*/ IGeometry *g, /*[in]*/ long cylinder, /*[in]*/ short sector, short *count, /*[out]*/ VARIANT *buffer);
	STDMETHOD(lsecid)(/*[in]*/ IGeometry *g, /*[in]*/ long track, /*[out]*/ VARIANT *buffer);
	STDMETHOD(psecid)(/*[in]*/ IGeometry *g, /*[in]*/ long cylinder, /*[in]*/ short sector, /*[out]*/ VARIANT *buffer);
	STDMETHOD(xtread)(/*[in]*/ IGeometry *g, /*[in]*/ long cylinder, /*[in]*/ short head, 
		long cyl_expected, short head_expected, /*[out]*/ VARIANT *buf);
	STDMETHOD(ltread)(/*[in]*/ IGeometry *g, /*[in]*/ long ltrack, /*[out]*/ VARIANT *buf);
	STDMETHOD(ptread)(/*[in]*/ IGeometry *g, /*[in]*/ long cylinder, /*[in]*/ short head, /*[out]*/ VARIANT *buf);
	STDMETHOD(alform)(/*[in]*/ IGeometry *geom, /*[in]*/ long track, /*[in]*/ short filler);
	STDMETHOD(apform)(/*[in]*/ IGeometry *geom, /*[in]*/ long cylinder, /*[in]*/ short head, /*[in]*/ short filler);
	STDMETHOD(lformat)(/*[in]*/ IGeometry *geom, /*[in]*/ long track, /*[in]*/ VARIANT *format, /*[in]*/ short filler);
	STDMETHOD(pformat)(/*[in]*/ IGeometry *geom, /*[in]*/ long cylinder, /*[in]*/ short head, /*[in]*/ VARIANT *format, /*[in]*/ short filler);
	STDMETHOD(xcheck)(/*[in]*/ IGeometry *g, /*[in]*/ VARIANT *buf, /*[in]*/ long cylinder, /*[in]*/ short head, 
		long cyl_expected, short head_expected, /*[in]*/ short sector,long sector_len);
	STDMETHOD(lcheck)(/*[in]*/ IGeometry *g, /*[in]*/ VARIANT *buf, /*[in]*/ long lsect);
	STDMETHOD(pcheck)(/*[in]*/ IGeometry *g, /*[in]*/ VARIANT *buf, /*[in]*/ long cylinder, /*[in]*/ short head, /*[in]*/ short sector);
	STDMETHOD(xwrite)(/*[in]*/ IGeometry *g, /*[in]*/ VARIANT *buf, /*[in]*/ long cylinder, /*[in]*/ short head, 
		long cyl_expected, short head_expected, /*[in]*/ short sector,long sector_len,  short deleted);
	STDMETHOD(lwrite)(/*[in]*/ IGeometry *g, /*[in]*/ VARIANT *buf, /*[in]*/ long lsect);
	STDMETHOD(pwrite)(/*[in]*/ IGeometry *g, /*[in]*/ VARIANT *buf, /*[in]*/ long cylinder, /*[in]*/ short head, /*[in]*/ short sector);
	STDMETHOD(xread)(/*[in]*/ IGeometry *g, /*[in]*/ long cylinder, /*[in]*/ short head, 
		long cyl_expected, short head_expected, /*[in]*/ short sector,long sector_len,  short *deleted, /*[out]*/ VARIANT *buf);
	STDMETHOD(lread)(/*[in]*/ IGeometry *g, /*[in]*/ long lsect, /*[out]*/ VARIANT *buf);
	STDMETHOD(pread)(/*[in]*/ IGeometry *g, /*[in]*/ long cylinder, /*[in]*/ short head, /*[in]*/ short sector, /*[out]*/ VARIANT *buf);
	STDMETHOD(drive_status)(/*[in]*/ IGeometry *g, /*[in]*/ short head, /*[out,retval]*/ short *status);
	STDMETHOD(close)();
	STDMETHOD(get_geometry)(/*[out]*/ IGeometry **g);
private:
	HRESULT MapError(dsk_err_t err);
	HRESULT GetBuffer(VARIANT *buf, unsigned char **bufout, unsigned length);
	HRESULT GetFormat(VARIANT *buf, DSK_FORMAT **bufout, unsigned length);
	HRESULT PutBuffer(unsigned char *bufout, VARIANT *buf, unsigned length, dsk_err_t err);
};

#endif //__DISK_H_
