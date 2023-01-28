#pragma once

void GetProfileStringReg( HKEY hKey, LPCWSTR lpszEntry, LPWSTR lpszBuf, DWORD cchBufSize, LPCWSTR lpszDefault )
{
	DWORD dwType, dwCount;
	dwCount = cchBufSize * sizeof( WCHAR );
	if( hKey == NULL 
	  || RegQueryValueEx( hKey, lpszEntry, NULL, &dwType, (LPBYTE)lpszBuf, &dwCount) != ERROR_SUCCESS ){
		StringCopy( lpszBuf, cchBufSize, lpszDefault );
	}
}

