#pragma once

#ifndef _DEBUG
#define _SECURE_SCL	0
#endif

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINDOWS 0x0500
#define _WIN32_WINNT  0x0501
#define _WIN32_IE  0x0501

#include <windows.h>
#include <windowsx.h>
#include <shtypes.h>
#include <tchar.h>
#include <malloc.h>
#include <crtdbg.h>
#include <olectl.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shlguid.h>
#include <shellapi.h>
//#include <commctrl.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>
#pragma warning( push )
#pragma warning( disable : 4838 ) // conversion from 'int' to 'UINT' requires a narrowing conversion
#pragma warning( disable : 4996 ) // 'sprintf' was declared deprecated
#pragma warning( disable : 4302 ) // 'type cast': truncation from 'LPCTSTR' to 'WORD'
#include "..\..\wtl\atlapp.h"
#pragma warning( pop )
#include <atlwin.h>
#include "..\..\wtl\atlctrls.h"

#ifdef _DEBUG
#define VERIFY(f)          _ASSERTE(f)
#else
#define VERIFY(f)          ((void)(f))
#endif

#pragma warning( push )
#pragma warning( disable : 4995 ) // 'function': name was marked as #pragma deprecated
#pragma warning( disable : 4244 )  // 'argument' : conversion from '__w64 unsigned int' to 'rsize_t', possible loss of data
#include <strsafe.h>
#include <vector>
#pragma warning( pop )

using namespace std;

#ifdef _DEBUG
#define TRACE	AfxTrace
inline void __cdecl AfxTrace(LPCTSTR lpszFormat, ...)
{
	int nBuf;
	TCHAR szBuffer[512];
	va_list args;
	va_start(args, lpszFormat);
	nBuf = StringCchVPrintf( szBuffer, _countof( szBuffer ), lpszFormat, args );
	_ASSERTE(nBuf < _countof(szBuffer));
	OutputDebugString( szBuffer );
	va_end(args);
}
#define UNUSED(x) __noop
#else
#define TRACE  __noop // (void(0))
#define UNUSED(x) x
#endif
