#include "stdafx.h"
#include "mystring.h"

#ifdef _DEBUG
#define new _DEBUG_NEW
#endif

#ifndef _DEBUG
#pragma warning( push )
#pragma warning( disable : 4100 )  // 'parameter': unreferenced formal parameter
#endif

HRESULT StringCopyDebugW(
	LPWSTR pszDest,
    size_t cchDest,
    LPCWSTR pszSrc,
	LPCSTR pszFile,
	int nLine
)
{
	VERIFY( !IsBadWritePtr( pszDest, cchDest * sizeof(WCHAR) ) );
	HRESULT hr = StringCchCopyExW( pszDest, cchDest, pszSrc, NULL, NULL, STRING_FLAGS );
	if( FAILED( hr ) ) {
		_RPT3( _CRT_ERROR, "%hs (%d) StringCopy failed. cchDest = %u\n", pszFile, nLine, cchDest );
	}
	return hr;
}

HRESULT StringCopyDebugA(
	LPSTR pszDest,
    size_t cchDest,
    LPCSTR pszSrc,
	LPCSTR pszFile,
	int nLine
)
{
	VERIFY( !IsBadWritePtr( pszDest, cchDest * sizeof(char) ) );
	HRESULT hr = StringCchCopyExA( pszDest, cchDest, pszSrc, NULL, NULL, STRING_FLAGS );
	if( FAILED( hr ) ) {
		_RPT3( _CRT_ERROR, "%hs (%d) StringCopy failed. cchDest = %u\n", pszFile, nLine, cchDest );
	}
	return hr;
}

HRESULT StringCopyNDebugW(
	LPWSTR pszDest,
    size_t cchDest,
    LPCWSTR pszSrc,
	size_t cchSrc,
	LPCSTR pszFile,
	int nLine
)
{
	VERIFY( !IsBadWritePtr( pszDest, cchDest * sizeof(WCHAR) ) );
	HRESULT hr = StringCchCopyNExW( pszDest, cchDest, pszSrc, cchSrc, NULL, NULL, STRING_FLAGS );
	if( FAILED( hr ) ) {
		_RPT4( _CRT_ERROR, "%hs (%d) StringCopyN failed. cchDest = %u, cchSrc = %u\n", pszFile, nLine, cchDest, cchSrc );
	}
	return hr;
}

HRESULT StringCopyNDebugA(
	LPSTR pszDest,
    size_t cchDest,
    LPCSTR pszSrc,
	size_t cchSrc,
	LPCSTR pszFile,
	int nLine
)
{
	VERIFY( !IsBadWritePtr( pszDest, cchDest * sizeof(char) ) );
	HRESULT hr = StringCchCopyNExA( pszDest, cchDest, pszSrc, cchSrc, NULL, NULL, STRING_FLAGS );
	if( FAILED( hr ) ) {
		_RPT4( _CRT_ERROR, "%hs (%d) StringCopyN failed. cchDest = %u, cchSrc = %u\n", pszFile, nLine, cchDest, cchSrc );
	}
	return hr;
}

HRESULT StringCatDebugW(
	LPWSTR pszDest,
    size_t cchDest,
    LPCWSTR pszSrc,
	LPCSTR pszFile,
	int nLine
)
{
	VERIFY( !IsBadWritePtr( pszDest, cchDest * sizeof(WCHAR) ) );
	HRESULT hr = StringCchCatExW( pszDest, cchDest, pszSrc, NULL, NULL, STRING_FLAGS );
	if( FAILED( hr ) ) {
		_RPT3( _CRT_ERROR, "%hs (%d) StringCat failed. cchDest = %u\n", pszFile, nLine, cchDest );
	}
	return hr;
}

HRESULT StringCatDebugA(
	LPSTR pszDest,
    size_t cchDest,
    LPCSTR pszSrc,
	LPCSTR pszFile,
	int nLine
)
{
	VERIFY( !IsBadWritePtr( pszDest, cchDest * sizeof(char) ) );
	HRESULT hr = StringCchCatExA( pszDest, cchDest, pszSrc, NULL, NULL, STRING_FLAGS );
	if( FAILED( hr ) ) {
		_RPT3( _CRT_ERROR, "%hs (%d) StringCat failed. cchDest = %u\n", pszFile, nLine, cchDest );
	}
	return hr;
}

HRESULT StringCatNDebugW(
	LPWSTR pszDest,
    size_t cchDest,
    LPCWSTR pszSrc,
	size_t cchMaxAppend,
	LPCSTR pszFile,
	int nLine
)
{
	VERIFY( !IsBadWritePtr( pszDest, cchDest * sizeof(WCHAR) ) );
	HRESULT hr = StringCchCatNExW( pszDest, cchDest, pszSrc, cchMaxAppend, NULL, NULL, STRING_FLAGS );
	if( FAILED( hr ) ) {
		_RPT4( _CRT_ERROR, "%hs (%d) StringCatN failed. cchDest = %u, cchMaxAppend = %u\n", pszFile, nLine, cchDest, cchMaxAppend );
	}
	return hr;
}

HRESULT StringCatNDebugA(
	LPSTR pszDest,
    size_t cchDest,
    LPCSTR pszSrc,
	size_t cchMaxAppend,
	LPCSTR pszFile,
	int nLine
)
{
	VERIFY( !IsBadWritePtr( pszDest, cchDest * sizeof(char) ) );
	HRESULT hr = StringCchCatNExA( pszDest, cchDest, pszSrc, cchMaxAppend, NULL, NULL, STRING_FLAGS );
	if( FAILED( hr ) ) {
		_RPT4( _CRT_ERROR, "%hs (%d) StringCatN failed. cchDest = %u, cchMaxAppend = %u\n", pszFile, nLine, cchDest, cchMaxAppend );
	}
	return hr;
}

HRESULT StringVPrintfDebugW(
    LPWSTR pszDest,
    size_t cchDest,
    LPCWSTR pszFormat,
    va_list argList,
	LPCSTR pszFile,
	int nLine
)
{
	VERIFY( !IsBadWritePtr( pszDest, cchDest * sizeof(WCHAR) ) );
	HRESULT hr = StringCchVPrintfExW( pszDest, cchDest, NULL, NULL, STRING_FLAGS, pszFormat, argList );
	if( FAILED( hr ) ) {
		_RPT3( _CRT_ERROR, "%hs (%d) StringVPrintf failed. cchDest = %u\n", pszFile, nLine, cchDest );
	}
	return hr;
}

HRESULT StringVPrintfDebugA(
    LPSTR pszDest,
    size_t cchDest,
    LPCSTR pszFormat,
    va_list argList,
	LPCSTR pszFile,
	int nLine
)
{
	VERIFY( !IsBadWritePtr( pszDest, cchDest * sizeof(char) ) );
	HRESULT hr = StringCchVPrintfExA( pszDest, cchDest, NULL, NULL, STRING_FLAGS, pszFormat, argList );
	if( FAILED( hr ) ) {
		_RPT3( _CRT_ERROR, "%hs (%d) StringVPrintf failed. cchDest = %u\n", pszFile, nLine, cchDest );
	}
	return hr;
}

HRESULT StringPrintfDebugW(
    LPWSTR pszDest,
    size_t cchDest,
    LPCWSTR pszFormat,
	...
)
{
	VERIFY( !IsBadWritePtr( pszDest, cchDest * sizeof(WCHAR) ) );
    va_list argList;
    va_start(argList, pszFormat);
	HRESULT hr = StringCchVPrintfExW( pszDest, cchDest, NULL, NULL, STRING_FLAGS, pszFormat, argList );
    va_end(argList);
	if( FAILED( hr ) ) {
		_RPT3( _CRT_ERROR, "%hs (%d) StringVPrintf failed. cchDest = %u\n", __FILE__, __LINE__, cchDest );
	}
	return hr;
}

HRESULT StringPrintfDebugA(
    LPSTR pszDest,
    size_t cchDest,
    LPCSTR pszFormat,
	...
)
{
	VERIFY( !IsBadWritePtr( pszDest, cchDest * sizeof(char) ) );
    va_list argList;
    va_start(argList, pszFormat);
	HRESULT hr = StringCchVPrintfExA( pszDest, cchDest, NULL, NULL, STRING_FLAGS, pszFormat, argList );
    va_end(argList);
	if( FAILED( hr ) ) {
		_RPT3( _CRT_ERROR, "%hs (%d) StringVPrintf failed. cchDest = %u\n", __FILE__, __LINE__, cchDest );
	}
	return hr;
}

#ifndef _DEBUG
#pragma warning( pop )
#endif
