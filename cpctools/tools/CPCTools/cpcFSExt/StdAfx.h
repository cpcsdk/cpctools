// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__9D5DF74F_8CF8_4676_8661_A862A0FA75B9__INCLUDED_)
#define AFX_STDAFX_H__9D5DF74F_8CF8_4676_8661_A862A0FA75B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500             // enable Win 2K features
#endif
#define WINVER 0x0500                   // enable 98/2K features
#define _WIN32_IE 0x0400                // enable IE4+ features

#define _ATL_APARTMENT_THREADED

#include <afxwin.h>
#include <afxdisp.h>
#include <afxdlgs.h>
#include <afxole.h>
#include <afxcmn.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9D5DF74F_8CF8_4676_8661_A862A0FA75B9__INCLUDED)
