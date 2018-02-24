#pragma once

HRESULT StringCopyDebugW(
	LPWSTR pszDest,
    size_t cchDest,
    LPCWSTR pszSrc,
	LPCSTR pszFile,
	int nLine
);

HRESULT StringCopyDebugA(
	LPSTR pszDest,
    size_t cchDest,
    LPCSTR pszSrc,
	LPCSTR pszFile,
	int nLine
);

HRESULT StringCopyNDebugW(
	LPWSTR pszDest,
    size_t cchDest,
    LPCWSTR pszSrc,
	size_t cchSrc,
	LPCSTR pszFile,
	int nLine
);

HRESULT StringCopyNDebugA(
	LPSTR pszDest,
    size_t cchDest,
    LPCSTR pszSrc,
	size_t cchSrc,
	LPCSTR pszFile,
	int nLine
);

HRESULT StringCatDebugW(
	LPWSTR pszDest,
    size_t cchDest,
    LPCWSTR pszSrc,
	LPCSTR pszFile,
	int nLine
);

HRESULT StringCatDebugA(
	LPSTR pszDest,
    size_t cchDest,
    LPCSTR pszSrc,
	LPCSTR pszFile,
	int nLine
);

HRESULT StringCatNDebugW(
	LPWSTR pszDest,
    size_t cchDest,
    LPCWSTR pszSrc,
	size_t cchMaxAppend,
	LPCSTR pszFile,
	int nLine
);

HRESULT StringCatNDebugA(
	LPSTR pszDest,
    size_t cchDest,
    LPCSTR pszSrc,
	size_t cchMaxAppend,
	LPCSTR pszFile,
	int nLine
);

HRESULT StringVPrintfDebugW(
    LPWSTR pszDest,
    size_t cchDest,
    LPCWSTR pszFormat,
    va_list argList,
	LPCSTR pszFile,
	int nLine
);

HRESULT StringVPrintfDebugA(
    LPSTR pszDest,
    size_t cchDest,
    LPCSTR pszFormat,
    va_list argList,
	LPCSTR pszFile,
	int nLine
);

HRESULT StringPrintfDebugW(
    LPWSTR pszDest,
    size_t cchDest,
    LPCWSTR pszFormat,
	...
);

HRESULT StringPrintfDebugA(
    LPSTR pszDest,
    size_t cchDest,
    LPCSTR pszFormat,
	...
);


#ifdef _DEBUG
#define STRING_FLAGS	(STRSAFE_FILL_BEHIND_NULL | 0xcc)
#else
#define STRING_FLAGS	0
#endif

#ifdef UNICODE

#define StringCopy		StringCopyW
#define StringCopyN		StringCopyNW
#define StringCat		StringCatW
#define StringCatN		StringCatNW
#define StringVPrintf	StringVPrintfW
#define StringPrintf	StringPrintfW

#else

#define StringCopy		StringCopyA
#define StringCopyN		StringCopyNA
#define StringCat		StringCatA
#define StringCatN		StringCatNA
#define StringVPrintf	StringVPrintfA
#define StringPrintf	StringPrintfA

#endif



#ifdef _DEBUG
#define StringCopyW(pszDest, cchDest, pszSrc )					StringCopyDebugW(pszDest, cchDest, pszSrc, __FILE__, __LINE__)
#define StringCopyA(pszDest, cchDest, pszSrc )					StringCopyDebugA(pszDest, cchDest, pszSrc, __FILE__, __LINE__)
#define StringCopyNW(pszDest, cchDest, pszSrc, cchSrc )			StringCopyNDebugW(pszDest, cchDest, pszSrc, cchSrc, __FILE__, __LINE__)
#define StringCopyNA(pszDest, cchDest, pszSrc, cchSrc )			StringCopyNDebugA(pszDest, cchDest, pszSrc, cchSrc, __FILE__, __LINE__)
#define StringCatW(pszDest, cchDest, pszSrc )					StringCatDebugW(pszDest, cchDest, pszSrc, __FILE__, __LINE__)
#define StringCatA(pszDest, cchDest, pszSrc )					StringCatDebugA(pszDest, cchDest, pszSrc, __FILE__, __LINE__)
#define StringCatNW(pszDest, cchDest, pszSrc, cchMaxAppend )	StringCatNDebugW(pszDest, cchDest, pszSrc, cchMaxAppend, __FILE__, __LINE__)
#define StringCatNA(pszDest, cchDest, pszSrc, cchMaxAppend )	StringCatNDebugA(pszDest, cchDest, pszSrc, cchMaxAppend, __FILE__, __LINE__)
#define StringVPrintfW(pszDest, cchDest, pszFormat, argList )	StringVPrintfDebugW(pszDest, cchDest, pszFormat, argList, __FILE__, __LINE__)
#define StringVPrintfA(pszDest, cchDest, pszFormat, argList )	StringVPrintfDebugA(pszDest, cchDest, pszFormat, argList, __FILE__, __LINE__)
#define StringPrintfW											StringPrintfDebugW
#define StringPrintfA											StringPrintfDebugA
#else
#define StringCopyW												StringCchCopyW
#define StringCopyA												StringCchCopyA
#define StringCopyNW											StringCchCopyNW
#define StringCopyNA											StringCchCopyNA
#define StringCatW												StringCchCatW
#define StringCatA												StringCchCatA
#define StringCatNW												StringCchCatNW
#define StringCatNA												StringCchCatNA
#define StringVPrintfW											StringCchVPrintfW
#define StringVPrintfA											StringCchVPrintfA
#define StringPrintfW											StringCchPrintfW
#define StringPrintfA											StringCchPrintfA
#endif

