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

#include "stdafx.h"
#include "Atlibdsk.h"
#include "Geometry.h"

/////////////////////////////////////////////////////////////////////////////
// CGeometry

STDMETHODIMP CGeometry::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IGeometry
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


HRESULT CGeometry::MapError(dsk_err_t err)
{
	char *desc = dsk_strerror(err);
	// These errors all map onto well-known HRESULTS:
	switch(err)
	{
		case DSK_ERR_OK:      return S_OK;
		case DSK_ERR_BADPTR:  return Error(desc, IID_IGeometry, E_POINTER);
		case DSK_ERR_BADPARM: return Error(desc, IID_IGeometry, E_INVALIDARG);
		case DSK_ERR_NOMEM:   return Error(desc, IID_IGeometry, E_OUTOFMEMORY);
		case DSK_ERR_NOTIMPL: return Error(desc, IID_IGeometry, E_NOTIMPL);
		case DSK_ERR_ACCESS:  return Error(desc, IID_IGeometry, E_ACCESSDENIED);
	}
	return Error(desc, IID_IGeometry, MAKE_HRESULT(1, FACILITY_ITF, err));
}

STDMETHODIMP CGeometry::get_sidedness(Sidedness *pVal)
{
	*pVal = (Sidedness)m_g.dg_sidedness;
	return S_OK;
}

STDMETHODIMP CGeometry::put_sidedness(Sidedness newVal)
{
	m_g.dg_sidedness = (dsk_sides_t)newVal;
	return S_OK;
}

STDMETHODIMP CGeometry::get_cylinders(long *pVal)
{
	*pVal = m_g.dg_cylinders;
	return S_OK;
}

STDMETHODIMP CGeometry::put_cylinders(long newVal)
{
	m_g.dg_cylinders = newVal;
	return S_OK;
}

STDMETHODIMP CGeometry::get_heads(short *pVal)
{
	*pVal = m_g.dg_heads;
	return S_OK;
}

STDMETHODIMP CGeometry::put_heads(short newVal)
{
	m_g.dg_heads = newVal;
	return S_OK;
}

STDMETHODIMP CGeometry::get_sectors(short *pVal)
{
	*pVal = m_g.dg_sectors;
	return S_OK;
}

STDMETHODIMP CGeometry::put_sectors(short newVal)
{
	m_g.dg_sectors = newVal;
	return S_OK;
}

STDMETHODIMP CGeometry::get_secbase(short *pVal)
{
	*pVal = m_g.dg_secbase;
	return S_OK;
}

STDMETHODIMP CGeometry::put_secbase(short newVal)
{
	m_g.dg_secbase = newVal;
	return S_OK;
}

STDMETHODIMP CGeometry::get_datarate(DataRate *pVal)
{
	*pVal = (DataRate)(m_g.dg_datarate);
	return S_OK;
}

STDMETHODIMP CGeometry::put_datarate(DataRate newVal)
{
	m_g.dg_datarate = (dsk_rate_t)newVal;
	return S_OK;
}

STDMETHODIMP CGeometry::get_secsize(long *pVal)
{
	*pVal = m_g.dg_secsize;
	return S_OK;
}

STDMETHODIMP CGeometry::put_secsize(long newVal)
{
	m_g.dg_secsize = newVal;
	return S_OK;
}

STDMETHODIMP CGeometry::get_rwgap(short *pVal)
{
	*pVal = m_g.dg_rwgap;
	return S_OK;
}

STDMETHODIMP CGeometry::put_rwgap(short newVal)
{
	m_g.dg_rwgap = (unsigned char)newVal;
	return S_OK;
}

STDMETHODIMP CGeometry::get_fmtgap(short *pVal)
{
	*pVal = m_g.dg_fmtgap;
	return S_OK;
}

STDMETHODIMP CGeometry::put_fmtgap(short newVal)
{
	m_g.dg_fmtgap = (unsigned char)newVal;
	return S_OK;
}

STDMETHODIMP CGeometry::get_fm(short *pVal)
{
	*pVal = m_g.dg_fm;
	return S_OK;
}

STDMETHODIMP CGeometry::put_fm(short newVal)
{
	m_g.dg_fm = newVal;
	return S_OK;
}

STDMETHODIMP CGeometry::get_nomulti(short *pVal)
{
	*pVal = m_g.dg_nomulti;
	return S_OK;
}

STDMETHODIMP CGeometry::put_nomulti(short newVal)
{
	m_g.dg_nomulti = newVal;
	return S_OK;
}

STDMETHODIMP CGeometry::get_noskip(short *pVal)
{
	*pVal = m_g.dg_noskip;
	return S_OK;
}

STDMETHODIMP CGeometry::put_noskip(short newVal)
{
	m_g.dg_noskip = newVal;
	return S_OK;
}

STDMETHODIMP CGeometry::ls2ps(long lsect, long *cyl, short *head, short *sector)
{
	HRESULT hr;
	dsk_pcyl_t c;
	dsk_phead_t h;
	dsk_psect_t s;
	hr = MapError(dg_ls2ps(&m_g, lsect, &c, &h, &s));
	*cyl = c;
	*head = h;
	*sector = s;
	return hr;
}

STDMETHODIMP CGeometry::lt2pt(long lsect, long *cyl, short *head)
{
	HRESULT hr;
	dsk_pcyl_t c;
	dsk_phead_t h;
	hr = MapError(dg_lt2pt(&m_g, lsect, &c, &h));
	*cyl = c;
	*head = h;
	return hr;
}

STDMETHODIMP CGeometry::ps2ls(long cyl, short head, short sector, long *lsect)
{
	dsk_lsect_t ls;
	HRESULT hr = MapError(dg_ps2ls(&m_g, cyl, head, sector, &ls));
	*lsect = ls;
	return hr;
}

STDMETHODIMP CGeometry::pt2lt(long cyl, short head, long *lsect)
{
	dsk_ltrack_t lt;
	HRESULT hr = MapError(dg_pt2lt(&m_g, cyl, head, &lt));
	*lsect = lt;
	return hr;
}

STDMETHODIMP CGeometry::stdformat(int formatid, BSTR *name, BSTR *description)
{
	dsk_cchar_t fname, fdesc;

	HRESULT hr = MapError(dg_stdformat(&m_g, (dsk_format_t)formatid, &fname, &fdesc));
	if (FAILED(hr)) return hr;

	*name        = CComBSTR(fname).Detach();
	*description = CComBSTR(fdesc).Detach();

	return S_OK;
}
