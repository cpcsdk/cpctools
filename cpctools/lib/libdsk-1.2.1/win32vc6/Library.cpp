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
#include "Library.h"
#include "Disk.h"
#include "Geometry.h"

void dg_to_g(DSK_GEOMETRY *dg, IGeometry *g);
void g_to_dg(IGeometry *g, DSK_GEOMETRY *dg);
char *get_string(BSTR bs);
BSTR ret_string(const char *s);

/////////////////////////////////////////////////////////////////////////////
// CLibrary

STDMETHODIMP CLibrary::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ILibrary
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CLibrary::MapError(dsk_err_t err)
{
	char *desc = dsk_strerror(err);
	// These errors all map onto well-known HRESULTS:
	switch(err)
	{
		case DSK_ERR_OK:      return S_OK;
		case DSK_ERR_BADPTR:  return Error(desc, IID_ILibrary, E_POINTER);
		case DSK_ERR_BADPARM: return Error(desc, IID_ILibrary, E_INVALIDARG);
		case DSK_ERR_NOMEM:   return Error(desc, IID_ILibrary, E_OUTOFMEMORY);
		case DSK_ERR_NOTIMPL: return Error(desc, IID_ILibrary, E_NOTIMPL);
		case DSK_ERR_ACCESS:  return Error(desc, IID_ILibrary, E_ACCESSDENIED);
	}
	return Error(desc, IID_ILibrary, MAKE_HRESULT(1, FACILITY_ITF, -err));
}


HRESULT CLibrary::GetBuffer(VARIANT *buf, unsigned char **bufout, unsigned length)
{
	long lBound, uBound;
	void HUGEP* FAR pData;
	SAFEARRAY *psa;
	HRESULT hr;

	if (V_VT(buf) != (VT_ARRAY | VT_UI1)) 
	{
		return Error("Passed parameter must be an array of unsigned characters", IID_ILibrary, E_INVALIDARG);
	}
	psa = V_ARRAY(buf);
      // Check dimensions of the array.
	if (SafeArrayGetDim(psa) != 1)
	{
		return Error("Passed parameter must be a 1-dimensional array", IID_ILibrary, E_INVALIDARG);
	}

	// Get array bounds.
	hr = SafeArrayGetLBound(psa, 1, &lBound);
	if (FAILED(hr))
	{
		return Error("Could not get array bounds", IID_ILibrary, hr);
	}
	hr = SafeArrayGetUBound(psa, 1, &uBound);
	if (FAILED(hr))
	{
		return Error("Could not get array bounds", IID_ILibrary, hr);
	}
	if (uBound + 1 - lBound < (long)length)
	{
		return Error("Passed array is too short", IID_ILibrary, E_INVALIDARG);
	}
	// Get a pointer to the elements of the array.
	hr = SafeArrayAccessData(psa, &pData);
	if (FAILED(hr))
	{
		return Error("Could not access array data", IID_ILibrary, E_INVALIDARG);
	}
	*bufout = (unsigned char *)dsk_malloc(length);
	if (!bufout) return MapError(DSK_ERR_NOMEM);
	memcpy(bufout, pData, length);
	SafeArrayUnaccessData(psa);
	return S_OK;
}


STDMETHODIMP CLibrary::open(BSTR f, BSTR d, BSTR c, IDisk **ppDisk)
{
	DSK_PDRIVER pd;
	dsk_err_t err;
	char *filename;
	char *driver;
	char *compress;

	if (!f || !ppDisk)
	{
		return MapError(DSK_ERR_BADPTR);
	}
	filename = get_string(f);
	driver   = get_string(d);
	compress = get_string(c);

	if (driver[0] == 0)   
	{
		dsk_free(driver);
		driver   = NULL;
	}
	if (compress[0] == 0) 
	{
		dsk_free(compress);
		compress = NULL;
	}
	err = dsk_open(&pd, filename, driver, compress);
	if (filename) dsk_free(filename);
	if (driver)   dsk_free(driver);
	if (compress) dsk_free(compress);

	if (err)
	{
		return MapError(err);
	}
	HRESULT hr = CDisk::CreateInstance(ppDisk);
	if (FAILED(hr)) return hr;

	((CDisk *)ppDisk[0])->m_driver = pd;

	return hr;
}




STDMETHODIMP CLibrary::create(BSTR f, BSTR d, BSTR c, IDisk **ppDisk)
{
	DSK_PDRIVER pd;
	dsk_err_t err;
	char *filename;
	char *driver;
	char *compress;

	if (!f || !ppDisk)
	{
		return MapError(DSK_ERR_BADPTR);
	}
	filename = get_string(f);
	driver   = get_string(d);
	compress = get_string(c);

	if (driver[0] == 0)   
	{
		dsk_free(driver);
		driver   = NULL;
	}
	if (compress[0] == 0) 
	{
		dsk_free(compress);
		compress = NULL;
	}
	err = dsk_creat(&pd, filename, driver, compress);
	if (filename) dsk_free(filename);
	if (driver)   dsk_free(driver);
	if (compress) dsk_free(compress);

	if (err)
	{
		return MapError(err);
	}
	HRESULT hr = CDisk::CreateInstance(ppDisk);
	if (FAILED(hr)) return hr;

	((CDisk *)ppDisk[0])->m_driver = pd;

	return hr;
}

STDMETHODIMP CLibrary::stdformat(int formatid, BSTR *name, BSTR *description, IGeometry **ppGeom)
{
	dsk_cchar_t fname, fdesc;
	DSK_GEOMETRY dg;

	HRESULT hr = MapError(dg_stdformat(&dg, (dsk_format_t)formatid, &fname, &fdesc));
	if (FAILED(hr)) return hr;

	*name        = CComBSTR(fname).Detach();
	*description = CComBSTR(fdesc).Detach();

	if (ppGeom)
	{
		hr = CGeometry::CreateInstance(ppGeom);
		if (!FAILED(hr)) dg_to_g(&dg, *ppGeom);
	}
	return hr;
}

STDMETHODIMP CLibrary::get_psh(long sectorsize, short *shift)
{
	shift[0] = dsk_get_psh(sectorsize);
	return S_OK;
}

static IReporter *gl_reporter = NULL;

void rep_on(const char *s)
{
	if (!gl_reporter) return;
	CComBSTR bs(s);
	gl_reporter->report(bs);
}


void rep_off(void)
{
	if (!gl_reporter) return;
	gl_reporter->endreport();
}

/*
STDMETHODIMP CLibrary::set_reporter(IReporter *pv)
{
	gl_reporter = pv;
	dsk_reportfunc_set(rep_on, rep_off);
	return S_OK;
}

STDMETHODIMP CLibrary::get_reporter(IReporter **ppv)
{
	*ppv = gl_reporter;
	return S_OK;
}
*/

STDMETHODIMP CLibrary::get_stdformat_count(long *pVal)
{
	int n = 0;
	static int rv = -1;	// Cache the result so we only have to add them up once

	if (rv != -1)
	{
		*pVal = rv;
		return S_OK;
	}

	while (dg_stdformat(NULL, (dsk_format_t)n, NULL, NULL) == DSK_ERR_OK)
	{
		n++;
	}
	*pVal = rv = n;
	return S_OK;
}

STDMETHODIMP CLibrary::dosgeom(VARIANT *buffer, IGeometry **geom)
{
	unsigned char *header;
	DSK_GEOMETRY dg;

	HRESULT hr = GetBuffer(buffer, &header, 512);
	if (FAILED(hr)) return hr;
	hr = MapError(dg_dosgeom(&dg, header));
	dsk_free(header);
	if (!FAILED(hr)) hr = CGeometry::CreateInstance(geom);
	if (!FAILED(hr)) dg_to_g(&dg, *geom);
	return hr;
}


STDMETHODIMP CLibrary::cpm86geom(VARIANT *buffer, IGeometry **geom)
{
	unsigned char *header;
	DSK_GEOMETRY dg;

	HRESULT hr = GetBuffer(buffer, &header, 512);
	if (FAILED(hr)) return hr;
	hr = MapError(dg_cpm86geom(&dg, header));
	dsk_free(header);
	if (!FAILED(hr)) hr = CGeometry::CreateInstance(geom);
	if (!FAILED(hr)) dg_to_g(&dg, *geom);
	return hr;
}


STDMETHODIMP CLibrary::aprigeom(VARIANT *buffer, IGeometry **geom)
{
	unsigned char *header;
	DSK_GEOMETRY dg;

	HRESULT hr = GetBuffer(buffer, &header, 512);
	if (FAILED(hr)) return hr;
	hr = MapError(dg_aprigeom(&dg, header));
	dsk_free(header);
	if (!FAILED(hr)) hr = CGeometry::CreateInstance(geom);
	if (!FAILED(hr)) dg_to_g(&dg, *geom);
	return hr;
}



STDMETHODIMP CLibrary::pcwgeom(VARIANT *buffer, IGeometry **geom)
{
	unsigned char *header;
	DSK_GEOMETRY dg;

	HRESULT hr = GetBuffer(buffer, &header, 512);
	if (FAILED(hr)) return hr;
	hr = MapError(dg_pcwgeom(&dg, header));
	dsk_free(header);
	if (!FAILED(hr)) hr = CGeometry::CreateInstance(geom);
	if (!FAILED(hr)) dg_to_g(&dg, *geom);
	return hr;
}

STDMETHODIMP CLibrary::type_enum(long idx, BSTR *name, BOOL *valid)
{
	dsk_err_t err;
	char *opt;
	err = dsk_type_enum(idx, &opt);

	switch(err)
	{
		case DSK_ERR_OK:     *valid = true; 
							 *name = ret_string(opt); break;
		case DSK_ERR_NODRVR: *valid = false; break;
		default: return MapError(err);
	}
	return S_OK;
}

STDMETHODIMP CLibrary::comp_enum(long idx, BSTR *name, BOOL *valid)
{
	dsk_err_t err;
	char *opt;
	err = dsk_comp_enum(idx, &opt);

	switch(err)
	{
		case DSK_ERR_OK:     *valid = true; 
							 *name = ret_string(opt); break;
		case DSK_ERR_NODRVR: *valid = false; break;
		default: return MapError(err);
	}
	return S_OK;
}

STDMETHODIMP CLibrary::get_reporter(IDispatch **pVal)
{
	*pVal = gl_reporter;
	return S_OK;
}

STDMETHODIMP CLibrary::put_reporter(IDispatch *newVal)
{
	IReporter *pReporter;
	HRESULT hr;

	hr = newVal->QueryInterface(IID_IReporter, (void **)&pReporter);
	if (FAILED(hr)) return hr;

	if (gl_reporter) gl_reporter->Release();
	gl_reporter = pReporter;

	dsk_reportfunc_set(rep_on, rep_off);
	return S_OK;
}
