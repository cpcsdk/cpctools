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
#include "Disk.h"
#include "Geometry.h"

/////////////////////////////////////////////////////////////////////////////
// CDisk

CDisk::~CDisk()
{
	if (m_driver) close();
}

STDMETHODIMP CDisk::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDisk
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


HRESULT CDisk::MapError(int err)
{
	char *desc = dsk_strerror(err);
	// These errors all map onto well-known HRESULTS:
	switch(err)
	{
		case DSK_ERR_OK:      return S_OK;
		case DSK_ERR_BADPTR:  return Error(desc, IID_IDisk, E_POINTER);
		case DSK_ERR_BADPARM: return Error(desc, IID_IDisk, E_INVALIDARG);
		case DSK_ERR_NOMEM:   return Error(desc, IID_IDisk, E_OUTOFMEMORY);
		case DSK_ERR_NOTIMPL: return Error(desc, IID_IDisk, E_NOTIMPL);
		case DSK_ERR_ACCESS:  return Error(desc, IID_IDisk, E_ACCESSDENIED);
	}
	return Error(desc, IID_IDisk, MAKE_HRESULT(1, FACILITY_ITF, -err));
}

void dg_to_g(DSK_GEOMETRY *dg, IGeometry *g)
{
#define COPYOVER(v) g-> put_##v( dg-> dg_##v )

	g->put_sidedness((Sidedness)dg->dg_sidedness);
	g->put_datarate ((DataRate)dg->dg_datarate);
	COPYOVER(cylinders);
	COPYOVER(heads);
	COPYOVER(sectors);
	COPYOVER(secbase);
	COPYOVER(secsize);
	COPYOVER(rwgap);
	COPYOVER(fmtgap);
	COPYOVER(fm);
	COPYOVER(nomulti);
	COPYOVER(noskip);
#undef COPYOVER
}


void g_to_dg(IGeometry *g, DSK_GEOMETRY *dg)
{
#define COPYOVER(T, v) { T value; g->get_##v(&value); dg->dg_##v = value; }
#define COPYOVER2(T, T2, v) { T value; g->get_##v(&value); dg->dg_##v = (T2)value; }

	COPYOVER2(Sidedness, dsk_sides_t, sidedness)
	COPYOVER2(DataRate,  dsk_rate_t,  datarate)		
	COPYOVER(long, cylinders)
	COPYOVER(short, heads)
	COPYOVER(short, sectors)
	COPYOVER(short, secbase)
	COPYOVER(long, secsize)
	COPYOVER2(short, unsigned char, rwgap)
	COPYOVER2(short, unsigned char, fmtgap)
	COPYOVER(short, fm)
	COPYOVER(short, nomulti)
	COPYOVER(short, noskip)

#undef COPYOVER
#undef COPYOVER2
}

char *get_string(BSTR bs)
{
	char *buf;
	int len;
	
	len = WideCharToMultiByte(CP_UTF8, 0, bs, -1, NULL, 0, NULL, NULL);
	buf = (char *)dsk_malloc(len + 1);
	if (!buf) return NULL;
	WideCharToMultiByte(CP_UTF8, 0, bs, -1, buf, len, NULL, NULL);
	buf[len] = 0;
	return buf;
}

BSTR ret_string(const char *s)
{
	int len;
	OLECHAR *buf;
	BSTR bs;

	len = MultiByteToWideChar(CP_UTF8, 0, s, strlen(s), NULL, 0);
	buf = (OLECHAR *)dsk_malloc((len + 1) * sizeof(OLECHAR));
	if (!buf)
	{
		return CComBSTR(s).Detach();
	}
	buf[len] = 0;
	MultiByteToWideChar(CP_UTF8, 0, s, strlen(s), buf, len);
	bs = SysAllocString(buf);
	dsk_free(buf);
	return bs;
}


HRESULT CDisk::GetBuffer(VARIANT *buf, unsigned char **bufout, unsigned length)
{
	long lBound, uBound;
	void HUGEP* FAR pData;
	SAFEARRAY *psa;
	HRESULT hr;

	if (V_VT(buf) != (VT_ARRAY | VT_UI1)) 
	{
		return Error("Passed parameter must be an array of unsigned characters", IID_IDisk, E_INVALIDARG);
	}
	psa = V_ARRAY(buf);
      // Check dimensions of the array.
	if (SafeArrayGetDim(psa) != 1)
	{
		return Error("Passed parameter must be a 1-dimensional array", IID_IDisk, E_INVALIDARG);
	}

	// Get array bounds.
	hr = SafeArrayGetLBound(psa, 1, &lBound);
	if (FAILED(hr))
	{
		return Error("Could not get array bounds", IID_IDisk, hr);
	}
	hr = SafeArrayGetUBound(psa, 1, &uBound);
	if (FAILED(hr))
	{
		return Error("Could not get array bounds", IID_IDisk, hr);
	}
	if (uBound + 1 - lBound < (long)length)
	{
		return Error("Passed array is too short", IID_IDisk, E_INVALIDARG);
	}
	// Get a pointer to the elements of the array.
	hr = SafeArrayAccessData(psa, &pData);
	if (FAILED(hr))
	{
		return Error("Could not access array data", IID_IDisk, E_INVALIDARG);
	}
	*bufout = (unsigned char *)dsk_malloc(length);
	if (!*bufout) return MapError(DSK_ERR_NOMEM);
	memcpy(*bufout, pData, length);
	SafeArrayUnaccessData(psa);
	return S_OK;
}


HRESULT CDisk::GetFormat(VARIANT *buf, DSK_FORMAT **bufout, unsigned length)
{
	long lBound, uBound;
	unsigned char HUGEP* FAR pData;
	SAFEARRAY *psa;
	HRESULT hr;
	unsigned n;
	DSK_FORMAT *fmt;

	if (V_VT(buf) != (VT_ARRAY | VT_UI1)) 
	{
		return Error("Passed parameter must be an array of unsigned characters", IID_IDisk, E_INVALIDARG);
	}
	psa = V_ARRAY(buf);
      // Check dimensions of the array.
	if (SafeArrayGetDim(psa) != 1)
	{
		return Error("Passed parameter must be a 1-dimensional array", IID_IDisk, E_INVALIDARG);
	}

	// Get array bounds.
	hr = SafeArrayGetLBound(psa, 1, &lBound);
	if (FAILED(hr))
	{
		return Error("Could not get array bounds", IID_IDisk, hr);
	}
	hr = SafeArrayGetUBound(psa, 1, &uBound);
	if (FAILED(hr))
	{
		return Error("Could not get array bounds", IID_IDisk, hr);
	}
	if (uBound + 1 - lBound < (long)length*4)
	{
		return Error("Passed array is too short", IID_IDisk, E_INVALIDARG);
	}
	// Get a pointer to the elements of the array.
	hr = SafeArrayAccessData(psa, (void HUGEP * FAR *)&pData);
	if (FAILED(hr))
	{
		return Error("Could not access array data", IID_IDisk, E_INVALIDARG);
	}
	*bufout = (DSK_FORMAT *)dsk_malloc(length * sizeof(DSK_FORMAT));
	if (!bufout) return MapError(DSK_ERR_NOMEM);
	fmt = bufout[0];
	for (n = 0; n < length; n++)
	{
		fmt->fmt_cylinder = *pData++;
		fmt->fmt_head     = *pData++;
		fmt->fmt_sector   = *pData++;
		fmt->fmt_secsize  = 128 << (*pData++);
		++fmt;
	}
	SafeArrayUnaccessData(psa);
	return S_OK;
}


HRESULT CDisk::PutBuffer(unsigned char *bufout, VARIANT *var, unsigned length, dsk_err_t err)
{
	HRESULT hr;
	SAFEARRAY *psa;
	void HUGEP* FAR pData;

	psa = SafeArrayCreateVector(VT_UI1, 0, length);
	hr = SafeArrayAccessData(psa, &pData);
	if (FAILED(hr))
	{
		return hr;
	}
	memcpy(pData, bufout, length);
	SafeArrayUnaccessData(psa);
	var->vt = VT_ARRAY | VT_UI1;
	var->parray = psa;
	return MapError(err);
}


STDMETHODIMP CDisk::get_geometry(IGeometry **g)
{
	DSK_GEOMETRY dg;
	dsk_err_t err;
	HRESULT hr;

	err = dsk_getgeom(m_driver, &dg);
	if (err)
	{
		if (g) *g = NULL;
		return MapError(err);
	}
	hr = CGeometry::CreateInstance(g);
	if (hr) return hr;

	dg_to_g(&dg, *g);
	return S_OK;
}

STDMETHODIMP CDisk::close()
{
	return MapError(dsk_close(&m_driver));
}

STDMETHODIMP CDisk::drive_status(IGeometry *g, short head, short *status)
{
	dsk_err_t err;
	unsigned char st;
	DSK_GEOMETRY geom;

	g_to_dg(g, &geom);
	err = dsk_drive_status(m_driver, &geom, head, &st);
	*status = st;
	return MapError(err);
}



STDMETHODIMP CDisk::pread(IGeometry *g, long cylinder, short head, short sector, VARIANT *var)
{
	DSK_GEOMETRY geom;
	HRESULT hr;
	dsk_err_t err;

	g_to_dg(g, &geom);
	unsigned char *tmpbuf = (unsigned char *)dsk_malloc(geom.dg_secsize);
	if (!tmpbuf) return MapError(DSK_ERR_NOMEM);
	
	err = dsk_pread(m_driver, &geom, tmpbuf, cylinder, head, sector);
	hr = PutBuffer(tmpbuf, var, geom.dg_secsize, err);
	dsk_free(tmpbuf);
	return hr;
}


STDMETHODIMP CDisk::lread(IGeometry *g, long lsect, VARIANT *var)
{
	DSK_GEOMETRY geom;
	dsk_err_t err;
	HRESULT hr;

	g_to_dg(g, &geom);
	unsigned char *tmpbuf = (unsigned char *)dsk_malloc(geom.dg_secsize);
	if (!tmpbuf) return MapError(DSK_ERR_NOMEM);
	
	err = dsk_lread(m_driver, &geom, tmpbuf, lsect);
	hr = PutBuffer(tmpbuf, var, geom.dg_secsize, err);
	dsk_free(tmpbuf);
	return hr;
}



STDMETHODIMP CDisk::xread(IGeometry *g, long cylinder, short head, 
						  long cyl_expected, short head_expected, short sector, long sector_len, short *deleted, VARIANT *var)
{
	DSK_GEOMETRY geom;
	dsk_err_t err;
	HRESULT hr;
	int del = deleted[0];

	g_to_dg(g, &geom);
	unsigned char *tmpbuf = (unsigned char *)dsk_malloc(sector_len);
	if (!tmpbuf) return MapError(DSK_ERR_NOMEM);
	
	err = dsk_xread(m_driver, &geom, tmpbuf, cylinder, head, cyl_expected, head_expected, sector, sector_len, &del);
	hr = PutBuffer(tmpbuf, var, sector_len, err);
	dsk_free(tmpbuf);
	deleted[0] = del;
	return hr;
}



STDMETHODIMP CDisk::ptread(IGeometry *g, long cylinder, short head, VARIANT *var)
{
	DSK_GEOMETRY geom;
	HRESULT hr;
	dsk_err_t err;

	g_to_dg(g, &geom);
	unsigned char *tmpbuf = (unsigned char *)dsk_malloc(geom.dg_secsize * geom.dg_sectors);
	if (!tmpbuf) return MapError(DSK_ERR_NOMEM);
	
	err = dsk_ptread(m_driver, &geom, tmpbuf, cylinder, head);
	hr = PutBuffer(tmpbuf, var, geom.dg_secsize * geom.dg_sectors, err);
	dsk_free(tmpbuf);
	return hr;
}


STDMETHODIMP CDisk::ltread(IGeometry *g, long track, VARIANT *var)
{
	DSK_GEOMETRY geom;
	dsk_err_t err;
	HRESULT hr;

	g_to_dg(g, &geom);
	unsigned char *tmpbuf = (unsigned char *)dsk_malloc(geom.dg_secsize * geom.dg_sectors);
	if (!tmpbuf) return MapError(DSK_ERR_NOMEM);
	
	err = dsk_ltread(m_driver, &geom, tmpbuf, track);
	hr = PutBuffer(tmpbuf, var, geom.dg_secsize * geom.dg_sectors, err);
	dsk_free(tmpbuf);
	return hr;
}



STDMETHODIMP CDisk::xtread(IGeometry *g, long cylinder, short head, 
						  long cyl_expected, short head_expected, VARIANT *var)
{
	DSK_GEOMETRY geom;
	dsk_err_t err;
	HRESULT hr;

	g_to_dg(g, &geom);
	unsigned char *tmpbuf = (unsigned char *)dsk_malloc(geom.dg_secsize * geom.dg_sectors);
	if (!tmpbuf) return MapError(DSK_ERR_NOMEM);
	
	err = dsk_xtread(m_driver, &geom, tmpbuf, cylinder, head, cyl_expected, head_expected);
	hr = PutBuffer(tmpbuf, var, geom.dg_secsize * geom.dg_sectors, err);
	dsk_free(tmpbuf);
	return hr;
}




STDMETHODIMP CDisk::pwrite(IGeometry *g, VARIANT *buf, long cylinder, short head, short sector)
{
	HRESULT hr;
	DSK_GEOMETRY geom;
	unsigned char *pData;

	g_to_dg(g, &geom);
	hr = GetBuffer(buf, &pData, geom.dg_secsize);
	if (FAILED(hr)) return hr;
	hr = MapError(dsk_pwrite(m_driver, &geom, pData, cylinder, head, sector));
	dsk_free(pData);
	return hr;
}


STDMETHODIMP CDisk::lwrite(IGeometry *g, VARIANT *buf, long lsect)
{
	HRESULT hr;
	DSK_GEOMETRY geom;
	unsigned char *pData;

	g_to_dg(g, &geom);
	hr = GetBuffer(buf, &pData, geom.dg_secsize);
	if (FAILED(hr)) return hr;
	hr = MapError(dsk_lwrite(m_driver, &geom, pData, lsect));
	dsk_free(pData);
	return hr;
}



STDMETHODIMP CDisk::xwrite(IGeometry *g, VARIANT *buf, long cylinder, short head, long cyl_expected, short head_expected, /*[in]*/ short sector,long sector_len,  short deleted)
{
	HRESULT hr;
	DSK_GEOMETRY geom;
	unsigned char *pData;

	g_to_dg(g, &geom);
	hr = GetBuffer(buf, &pData, sector_len);
	if (FAILED(hr)) return hr;
	hr = MapError(dsk_xwrite(m_driver, &geom, pData, cylinder, head, cyl_expected, head_expected, sector, sector_len, deleted));
	dsk_free(pData);
	return hr;
}



STDMETHODIMP CDisk::pcheck(IGeometry *g, VARIANT *buf, long cylinder, short head, short sector)
{
	HRESULT hr;
	DSK_GEOMETRY geom;
	unsigned char *pData;

	g_to_dg(g, &geom);
	hr = GetBuffer(buf, &pData, geom.dg_secsize);
	if (FAILED(hr)) return hr;
	hr = MapError(dsk_pcheck(m_driver, &geom, pData, cylinder, head, sector));
	dsk_free(pData);
	return hr;
}


STDMETHODIMP CDisk::lcheck(IGeometry *g, VARIANT *buf, long lsect)
{
	HRESULT hr;
	DSK_GEOMETRY geom;
	unsigned char *pData;

	g_to_dg(g, &geom);
	hr = GetBuffer(buf, &pData, geom.dg_secsize);
	if (FAILED(hr)) return hr;
	hr = MapError(dsk_lcheck(m_driver, &geom, pData, lsect));
	dsk_free(pData);
	return hr;
}



STDMETHODIMP CDisk::xcheck(IGeometry *g, VARIANT *buf, long cylinder, short head, long cyl_expected, short head_expected, /*[in]*/ short sector,long sector_len)
{
	HRESULT hr;
	DSK_GEOMETRY geom;
	unsigned char *pData;

	g_to_dg(g, &geom);
	hr = GetBuffer(buf, &pData, sector_len);
	if (FAILED(hr)) return hr;
	hr = MapError(dsk_xcheck(m_driver, &geom, pData, cylinder, head, cyl_expected, head_expected, sector, sector_len));
	dsk_free(pData);
	return hr;
}


STDMETHODIMP CDisk::pformat(IGeometry *g, long cylinder, short head, VARIANT *format, short filler)
{
	HRESULT hr;
	DSK_GEOMETRY geom;
	DSK_FORMAT *fmt;

	g_to_dg(g, &geom);
	hr = GetFormat(format, &fmt, geom.dg_sectors);
	if (FAILED(hr)) return hr;

	hr = MapError(dsk_pformat(m_driver, &geom, cylinder, head, fmt, (unsigned char)filler));
	dsk_free(fmt);
	dg_to_g(&geom, g);
	return hr;
}

STDMETHODIMP CDisk::lformat(IGeometry *g, long track, VARIANT *format, short filler)
{
	HRESULT hr;
	DSK_GEOMETRY geom;
	DSK_FORMAT *fmt;

	g_to_dg(g, &geom);
	hr = GetFormat(format, &fmt, geom.dg_sectors);
	if (FAILED(hr)) return hr;

	hr = MapError(dsk_lformat(m_driver, &geom, track, fmt, (unsigned char)filler));
	dsk_free(fmt);
	dg_to_g(&geom, g);
	return hr;
}


STDMETHODIMP CDisk::apform(IGeometry *g, long cylinder, short head, short filler)
{
	HRESULT hr;
	DSK_GEOMETRY geom;

	g_to_dg(g, &geom);
	hr = MapError(dsk_apform(m_driver, &geom, cylinder, head, (unsigned char)filler));
	dg_to_g(&geom, g);
	return hr;
}

STDMETHODIMP CDisk::alform(IGeometry *g, long track, short filler)
{
	HRESULT hr;
	DSK_GEOMETRY geom;

	g_to_dg(g, &geom);
	hr = MapError(dsk_alform(m_driver, &geom, track, (unsigned char)filler));
	dg_to_g(&geom, g);
	return hr;
}

STDMETHODIMP CDisk::psecid(IGeometry *g, long cylinder, short sector, VARIANT *buffer)
{
	DSK_GEOMETRY geom;
	DSK_FORMAT fmt;
	unsigned char buf[4];
	dsk_err_t err;

	g_to_dg(g, &geom);
	err = dsk_psecid(m_driver, &geom, cylinder, sector, &fmt);
	buf[0] = fmt.fmt_cylinder;
	buf[1] = fmt.fmt_head;
	buf[2] = fmt.fmt_sector;
	buf[3] = dsk_get_psh(fmt.fmt_secsize);

	return PutBuffer(buf, buffer, 4, err);
}

STDMETHODIMP CDisk::lsecid(IGeometry *g, long track, VARIANT *buffer)
{
	DSK_GEOMETRY geom;
	DSK_FORMAT fmt;
	unsigned char buf[4];
	dsk_err_t err;

	g_to_dg(g, &geom);
	err = dsk_lsecid(m_driver, &geom, track, &fmt);
	buf[0] = fmt.fmt_cylinder;
	buf[1] = fmt.fmt_head;
	buf[2] = fmt.fmt_sector;
	buf[3] = dsk_get_psh(fmt.fmt_secsize);

	return PutBuffer(buf, buffer, 4, err);
}

STDMETHODIMP CDisk::lseek(IGeometry *g, long track)
{
	DSK_GEOMETRY geom;
	g_to_dg(g, &geom);
	return MapError(dsk_lseek(m_driver, &geom, track));
}

STDMETHODIMP CDisk::pseek(IGeometry *g, long cylinder, short sector)
{
	DSK_GEOMETRY geom;
	g_to_dg(g, &geom);
	return MapError(dsk_pseek(m_driver, &geom, cylinder, sector));
}

STDMETHODIMP CDisk::get_comment(BSTR *pVal)
{
	char *cmt = NULL;
	dsk_err_t err = dsk_get_comment(m_driver, &cmt);

	if (cmt) *pVal = ret_string(cmt);
	return MapError(err);
}

STDMETHODIMP CDisk::put_comment(BSTR newVal)
{
	char *buf;
	dsk_err_t err;
	
	buf = get_string(newVal);
	if (!buf) return MapError(DSK_ERR_NOMEM);
	err = dsk_set_comment(m_driver, buf);
	dsk_free(buf);
	return MapError(err);
}

STDMETHODIMP CDisk::get_option(BSTR name, long *pVal)
{
	char *buf;
	dsk_err_t err;
	int value;
	
	buf = get_string(name);
	if (!buf) return MapError(DSK_ERR_NOMEM);
	err = dsk_get_option(m_driver, buf, &value);
	dsk_free(buf);
	*pVal = value;
	return MapError(err);
}

STDMETHODIMP CDisk::put_option(BSTR name, long newVal)
{
	char *buf;
	dsk_err_t err;
	
	buf = get_string(name);
	if (!buf) return MapError(DSK_ERR_NOMEM);
	err = dsk_set_option(m_driver, buf, newVal);
	dsk_free(buf);
	return MapError(err);
}

STDMETHODIMP CDisk::option_enum(long idx, BSTR *name, BOOL *valid)
{
	dsk_err_t err;
	char *opt;
	err = dsk_option_enum(m_driver, idx, &opt);

	switch(err)
	{
		case DSK_ERR_OK:     *valid = true; 
							 *name = ret_string(opt); break;
		case DSK_ERR_BADOPT: *valid = false; break;
		default: return MapError(err);
	}
	return S_OK;
}

STDMETHODIMP CDisk::get_retries(short *pVal)
{
	unsigned r;
	dsk_err_t err = dsk_get_retry(m_driver, &r);
	*pVal = r;
	return MapError(err);
}

STDMETHODIMP CDisk::put_retries(short newVal)
{
	return MapError(dsk_set_retry(m_driver, newVal));
}

STDMETHODIMP CDisk::get_drvname(BSTR *pVal)
{
	const char *name = dsk_drvname(m_driver);
	*pVal = ret_string(name);
	return S_OK;
}

STDMETHODIMP CDisk::get_drvdesc(BSTR *pVal)
{
	const char *name = dsk_drvdesc(m_driver);
	*pVal = ret_string(name);
	return S_OK;
}

STDMETHODIMP CDisk::get_compname(BSTR *pVal)
{
	const char *name = dsk_compname(m_driver);
	if (!name) name = "";
	*pVal = ret_string(name);
	return S_OK;
}

STDMETHODIMP CDisk::get_compdesc(BSTR *pVal)
{
	const char *name = dsk_compdesc(m_driver);
	if (!name) name = "";
	*pVal = ret_string(name);
	return S_OK;
}



STDMETHODIMP CDisk::get_dirty(short *pVal)
{
	*pVal = dsk_dirty(m_driver);
	return S_OK;
}


STDMETHODIMP CDisk::ptrkid(IGeometry *g, long cylinder, short sector, short *count, VARIANT *buffer)
{
	DSK_GEOMETRY geom;
	DSK_FORMAT *fmt;
	unsigned char *buf;
	dsk_err_t err;
	dsk_psect_t n, acount = 0;
	HRESULT hr;

	g_to_dg(g, &geom);
	err = dsk_ptrackids(m_driver, &geom, cylinder, sector, &acount, &fmt);

	if (acount)
	{
		if (count) *count = acount;
		buf = new unsigned char[4 * acount];
		for (n = 0; n < acount; n++)
		{
			buf[n*4  ] = fmt[n].fmt_cylinder;
			buf[n*4+1] = fmt[n].fmt_head;
			buf[n*4+2] = fmt[n].fmt_sector;
			buf[n*4+3] = dsk_get_psh(fmt[n].fmt_secsize);
		}
		hr = PutBuffer(buf, buffer, 4 * acount, err);
		delete buf;
		return hr;
	}
	return MapError(err);
}

STDMETHODIMP CDisk::ltrkid(IGeometry *g, long track, short *count, VARIANT *buffer)
{
	DSK_GEOMETRY geom;
	DSK_FORMAT *fmt;
	unsigned char *buf;
	dsk_err_t err;
	dsk_psect_t n, acount = 0;
	HRESULT hr;

	g_to_dg(g, &geom);
	err = dsk_ltrackids(m_driver, &geom, track, &acount, &fmt);
	if (acount)
	{
		if (count) *count = acount;
		buf = new unsigned char[4 * acount];
		for (n = 0; n < acount; n++)
		{
			buf[n*4  ] = fmt[n].fmt_cylinder;
			buf[n*4+1] = fmt[n].fmt_head;
			buf[n*4+2] = fmt[n].fmt_sector;
			buf[n*4+3] = dsk_get_psh(fmt[n].fmt_secsize);
		}
		hr = PutBuffer(buf, buffer, 4 * acount, err);
		delete buf;
		return hr;
	}
	return MapError(err);
}
