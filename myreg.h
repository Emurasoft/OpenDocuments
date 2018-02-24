#pragma once

void GetProfileStringReg( HKEY hKey, LPCTSTR lpszEntry, LPTSTR lpszBuf, DWORD cchBufSize, LPCTSTR lpszDefault )
{
	DWORD dwType, dwCount;
	dwCount = cchBufSize * sizeof( TCHAR );
	if( hKey == NULL 
	  || RegQueryValueEx( hKey, lpszEntry, NULL, &dwType, (LPBYTE)lpszBuf, &dwCount) != ERROR_SUCCESS ){
		StringCopy( lpszBuf, cchBufSize, lpszDefault );
	}
}

