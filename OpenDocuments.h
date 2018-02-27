// implementatin of this specific plug-in is here:
//

#define VIEW_FULL	0
#define VIEW_TITLE  1
#define VIEW_BLEND  2


#define ZERO_INIT_FIRST_MEM(classname, firstmem)  ZeroMemory( &firstmem, sizeof( classname ) - ((char*)&firstmem - (char*)this) );

INT_PTR CALLBACK PropDlg( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK ListProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );


void CenterWindow( HWND hDlg )
{
	_ASSERT(::IsWindow(hDlg));
	HWND hwndParent = GetParent(hDlg);

	RECT rcDlg;
	::GetWindowRect(hDlg, &rcDlg);
	RECT rcArea;
	RECT rcCenter;

	if( hwndParent != NULL ){
		DWORD dwStyleCenter = ::GetWindowLong(hwndParent, GWL_STYLE);
		if(!(dwStyleCenter & WS_VISIBLE) || (dwStyleCenter & WS_MINIMIZE))
			hwndParent = NULL;
	}

	HMONITOR hMonitor = NULL;
	if(hwndParent != NULL) {
		hMonitor = ::MonitorFromWindow(hwndParent, MONITOR_DEFAULTTONEAREST);
	}
	else {
		hMonitor = ::MonitorFromWindow(hDlg, MONITOR_DEFAULTTONEAREST);
	}
	_ASSERT(hMonitor != NULL);

	MONITORINFO minfo;
	minfo.cbSize = sizeof(MONITORINFO);
	VERIFY( ::GetMonitorInfo(hMonitor, &minfo) );

	rcArea = minfo.rcWork;

	if(hwndParent == NULL)
		rcCenter = rcArea;
	else
		::GetWindowRect(hwndParent, &rcCenter);

	int DlgWidth = rcDlg.right - rcDlg.left;
	int DlgHeight = rcDlg.bottom - rcDlg.top;

	int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
	int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

	if(xLeft + DlgWidth > rcArea.right)
		xLeft = rcArea.right - DlgWidth;
	if(xLeft < rcArea.left)
		xLeft = rcArea.left;

	if(yTop + DlgHeight > rcArea.bottom)
		yTop = rcArea.bottom - DlgHeight;
	if(yTop < rcArea.top)
		yTop = rcArea.top;

	::SetWindowPos(hDlg, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void GetCurrDir( LPCTSTR pszPath, LPTSTR pszCurrDir )
{
	if( *pszPath == '\"' ){  // grep title
		LPCTSTR p = _tcschr( pszPath + 1, L'\"' );
		if( !p )  return;
		p++;
		if( *p == ' ' )  p++;
		_ASSERT( *p != ' ' );
		StringCopy( pszCurrDir, MAX_PATH, p );
	}
	else {
		StringCopy( pszCurrDir, MAX_PATH, pszPath );
	}
	PathRemoveFileSpec( pszCurrDir );
}


bool IsPathEqual( LPCTSTR szPath1, LPCTSTR szPath2 )
{
	//OSVERSIONINFO osvi;
	//osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	//VERIFY( GetVersionEx(&osvi) );
	//bool bIsWindowsXPorLater = (osvi.dwMajorVersion > 5) || ( (osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 1) );
	//DWORD lcid = bIsWindowsXPorLater ? LOCALE_INVARIANT : MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT);
	return (CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, szPath1, -1, szPath2, -1) == CSTR_EQUAL);
}

class CRedraw
{
public:
	HWND m_hwnd;
	CRedraw( HWND hwnd ) {
		m_hwnd = hwnd;
		SendMessage( hwnd, WM_SETREDRAW, FALSE, 0 );
	}
	~CRedraw() {
		SendMessage( m_hwnd, WM_SETREDRAW, TRUE, 0 );
	}
};

class CWaitCursor
{
public:
	HCURSOR m_hPrevCursor;
	CWaitCursor() {
		m_hPrevCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	};
	~CWaitCursor() {
		SetCursor( m_hPrevCursor );
	};
};

class CMyFrame : public CETLFrame<CMyFrame>
{
public:
	// _loc.dll in MUI sub folder?
	enum { _USE_LOC_DLL			= LOC_USE_EMEDLOC_DLL	};

	// string ID
	enum { _IDS_MENU			= IDS_OPENDOCUMENTS_MENU_TEXT	};   // name of command, menu
	enum { _IDS_STATUS			= IDS_OPENDOCUMENTS_MENU_TEXT	};   // description of command, status bar
	enum { _IDS_NAME			= IDS_OPENDOCUMENTS_MENU_TEXT	};   // name of plug-in, plug-in settings dialog box
	enum { _IDS_VER				= IDS_PLUGIN_VERSION			};   // version string of plug-in, plug-in settings dialog box

	// bitmaps
	enum { _IDB_BITMAP			= IDB_BITMAP			};
	enum { _IDB_16C_24			= IDB_16C_24			};
	//enum { _IDB_256C_16_DEFAULT = IDB_TRUE_16_DEFAULT	};
	//enum { _IDB_256C_16_HOT		= IDB_TRUE_16_HOT		};
	//enum { _IDB_256C_16_BW		= IDB_TRUE_16_BW		};
	//enum { _IDB_256C_24_DEFAULT = IDB_TRUE_24_DEFAULT	};
	//enum { _IDB_256C_24_HOT		= IDB_TRUE_24_HOT		};
	//enum { _IDB_256C_24_BW		= IDB_TRUE_24_BW		};
	enum { _IDB_TRUE_16_DEFAULT = IDB_TRUE_16_DEFAULT	};
	enum { _IDB_TRUE_16_HOT		= IDB_TRUE_16_HOT		};
	enum { _IDB_TRUE_16_BW		= IDB_TRUE_16_BW		};
	enum { _IDB_TRUE_24_DEFAULT = IDB_TRUE_24_DEFAULT	};
	enum { _IDB_TRUE_24_HOT		= IDB_TRUE_24_HOT		};
	enum { _IDB_TRUE_24_BW		= IDB_TRUE_24_BW		};

	// masks
	enum { _MASK_TRUE_COLOR		= CLR_NONE				};
	//enum { _MASK_256_COLOR		= CLR_NONE				};

	// whether to allow a file is opened in the same window group during the plug-in execution.
	enum { _ALLOW_OPEN_SAME_GROUP = TRUE				};

	// whether to allow multiple instances.
	enum { _ALLOW_MULTIPLE_INSTANCES = TRUE				};

	// supporting EmEditor newest version * 1000
	enum { _MAX_EE_VERSION		= 14900					};

	// supporting EmEditor oldest version * 1000
	enum { _MIN_EE_VERSION		= 12000					};

	// supports EmEditor Professional
	enum { _SUPPORT_EE_PRO		= TRUE					};

	// supports EmEditor Standard
	enum { _SUPPORT_EE_STD		= FALSE					};

	// user-defined members
	enum { IDW_LIST	= 100 };

	// data that can be set zeros below
	HWND m_hwndList;
	WNDPROC m_lpOldListProc;
	HIMAGELIST m_himl;
	TCHAR m_szCurrDir[MAX_PATH];
	int m_iDefaultTabIcon;
//	int m_iSetTextImage;
	UINT m_nClientID;
	int  m_iPos;
	int  m_iOldPos;
	int  m_nViewType;
	bool m_bProfileLoaded;
	bool m_bOpenStartup;
	bool m_bFirstIdle;
	bool m_bRefreshIdle;
	bool m_bRefreshFileOpened;
	bool m_bRefreshSetTextImage;
	bool m_bSetRedraw;
	bool m_bIgnoreSelChanged;
	bool m_bUninstalling;
	bool m_bIgnoreNMClick;
	bool m_bFocusView;

	BOOL DisableAutoComplete( HWND /* hwnd */ )
	{
		return FALSE;
	}

	BOOL UseDroppedFiles( HWND /* hwnd */ )
	{
		return FALSE;
	}

	LRESULT UserMessage( HWND /*hwnd*/, WPARAM /*wParam*/, LPARAM /*lParam*/ )
	{
		return 0;
	}

	void OnCommand( HWND /*hwndView*/ )
	{
		if( m_hwndList == NULL ){
			OpenCustomBar();
		}
		else {
			CloseCustomBar();
		}
	}

	void OpenCustomBar()
	{
		if( m_hwndList )  return;

		BOOL bEnableTab = (BOOL)Editor_Info( m_hWnd, EI_IS_WINDOW_COMBINED, 0 );
		if( !bEnableTab )  return;

		TCHAR sz[260];
		TCHAR szAppName[80];
		LoadString( EEGetLocaleInstanceHandle(), IDS_OPENDOCUMENTS_MENU_TEXT, szAppName, _countof( szAppName ) );
		if( Editor_GetVersion( m_hWnd ) < 8000 ){
			LoadString( EEGetLocaleInstanceHandle(), IDS_INVALID_VERSION, sz, _countof( sz ) );
			MessageBox( m_hWnd, sz, szAppName, MB_OK | MB_ICONSTOP );
			return;
		}

		m_bIgnoreNMClick = GetProfileInt( EEREG_COMMON, NULL, _T("AutoSort"), 0 ) && GetProfileInt( EEREG_COMMON, NULL, _T("TabSort"), 0 ) == 3;

		DWORD dwStyles = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_SHOWSELALWAYS | LVS_REPORT | LVS_SHAREIMAGELISTS | LVS_NOCOLUMNHEADER  ;
		// A temporary parent window is specified here.  The parent window will be substituted by the custom bar window when Editor_CustomBarOpen is called.
		m_hwndList = CreateWindowEx( WS_EX_CLIENTEDGE, WC_LISTVIEW, NULL, dwStyles, 0, 0, 0, 0, m_hWnd, (HMENU)IDW_LIST, NULL, NULL );
		if( m_hwndList != NULL ){
			CUSTOM_BAR_INFO cbi;
			ZeroMemory( &cbi, sizeof( cbi ) );
			cbi.cbSize = sizeof( cbi );
			cbi.hwndClient = m_hwndList;
			cbi.iPos = m_iPos;
			cbi.pszTitle = szAppName;
			m_nClientID = Editor_CustomBarOpen( m_hWnd, &cbi );

			if( !m_nClientID ){
				CustomBarClosed();
			}
			else {
				// The parent window is set to the custom bar window.
				_ASSERTE( cbi.hwndCustomBar && GetParent( m_hwndList ) == cbi.hwndCustomBar );
				SubclassList( m_hwndList );
				OnCreate();
				SetListColor();
			}
		}
	}

	void CloseCustomBar()
	{
		if( !m_hwndList )  return;

		_ASSERTE( m_nClientID );
		BOOL bClosed = Editor_CustomBarClose( m_hWnd, m_nClientID );
		UNUSED( bClosed );
		_ASSERTE( bClosed );
		_ASSERTE( m_lpOldListProc == NULL );
		_ASSERTE( m_hwndList == NULL );
		UnsubclassList( m_hwndList );
		CustomBarClosed();  //  call just in case.  should not be needed since EVENT_CUSTOM_BAR_CLOSED is called.
	}

	void CustomBarClosed()
	{
		if( m_hwndList ){
			if( IsWindow( m_hwndList ) ) {
				DestroyWindow( m_hwndList );
			}
			m_hwndList = NULL;
		}
		m_nClientID = 0;
	}

	BOOL QueryStatus( HWND /*hwndView*/, LPBOOL pbChecked )
	{		
		*pbChecked = (m_hwndList != NULL);
		BOOL bEnableTab = (BOOL)Editor_Info( m_hWnd, EI_IS_WINDOW_COMBINED, 0 );
		return bEnableTab;
	}

	void OnEvents( HWND hwndView, UINT nEvent, LPARAM lParam )
	{
		if( nEvent & EVENT_CREATE_FRAME ){
//			HKEY hKey = GetRootKey();
			m_bOpenStartup = !!GetProfileInt( _T("OpenStartup"), FALSE );
			m_iPos = GetProfileInt( _T("CustomBarPos"), CUSTOM_BAR_LEFT );
			m_nViewType = GetProfileInt( _T("ViewType"), VIEW_BLEND );
			m_iOldPos = m_iPos;
//			if( hKey )  RegCloseKey( hKey );

			if( m_bOpenStartup ){
				OnCommand( hwndView );
			}
			else {
			}
		}
		if( nEvent & EVENT_CLOSE_FRAME ){
			CloseCustomBar();
		}
		if( nEvent & EVENT_CUSTOM_BAR_CLOSING ){
			// this message arrives even if plug-in does not own this custom bar, so make sure it is mine.
			if( m_hwndList != NULL ){
				CUSTOM_BAR_CLOSE_INFO* pCBCI = (CUSTOM_BAR_CLOSE_INFO*)lParam;
				if( pCBCI->nID == m_nClientID ){
					_ASSERTE( m_hwndList );
					_ASSERTE( GetParent( m_hwndList ) );
					UnsubclassList( m_hwndList );
				}
			}
		}
		if( nEvent & EVENT_CUSTOM_BAR_CLOSED ){
			// this message arrives even if plug-in does not own this custom bar, so make sure it is mine.
			if( m_hwndList != NULL ){  
				CUSTOM_BAR_CLOSE_INFO* pCBCI = (CUSTOM_BAR_CLOSE_INFO*)lParam;
				if( pCBCI->nID == m_nClientID ){
					_ASSERT( !IsWindow( m_hwndList ) );
					CustomBarClosed();
					m_bOpenStartup = (pCBCI->dwFlags & CLOSED_FRAME_WINDOW);
//					HKEY hKey = GetRootKey();
					if( !m_bUninstalling ){
						WriteProfileInt( _T("OpenStartup"), m_bOpenStartup );
						WriteProfileInt( _T("ViewType"), m_nViewType );
					}
//					if( hKey )  RegCloseKey( hKey );
				}
			}
		}
		if( nEvent & EVENT_IDLE ){
			OnIdle();
		}
		if( nEvent & EVENT_CONFIG_CHANGED ) {
			if( m_hwndList ){
				OnConfigChanged();
			}
		}
		if( nEvent & EVENT_FILE_OPENED ){
			if( m_hwndList ){
				OnFileOpened();
			}
		}
		if( nEvent & EVENT_DOC_CLOSE ){
			if( m_hwndList ){
				OnDocClose( (HEEDOC)lParam );
			}
		}
		if( nEvent & EVENT_MODIFIED ){
			if( m_hwndList ){
				OnModified();
			}
		}
		if( nEvent & EVENT_DOC_SEL_CHANGED ){
			if( m_hwndList ){
//				OnRefresh();
				OnSelChanged();
			}
		}
		if( nEvent & EVENT_TAB_MOVED ){
			if( m_hwndList ){
				OnTabMoved();
			}
		}
		if( nEvent & EVENT_CONFIG_CHANGED ){
			SetListColor();
		}
	}

	BOOL QueryUninstall( HWND /*hDlg*/ )
	{
		return TRUE;
	}

	BOOL SetUninstall( HWND hDlg, LPTSTR pszUninstallCommand, LPTSTR pszUninstallParam )
	{
		TCHAR szProductCode[80] = { 0 };
		HKEY hKey = NULL;
		if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("Software\\EmSoft\\EmEditorPlugIns\\OpenDocuments"), 0, KEY_READ, &hKey ) == ERROR_SUCCESS && hKey ){
			GetProfileStringReg( hKey, _T("ProductCode"), szProductCode, _countof( szProductCode ), _T("") );
			if( szProductCode[0] ){
				GetSystemDirectory( pszUninstallCommand, MAX_PATH );
				PathAppend( pszUninstallCommand, _T("msiexec.exe") );
				StringPrintf( pszUninstallParam, MAX_PATH, _T("/X%s"), szProductCode );
				RegCloseKey( hKey );
				return UNINSTALL_RUN_COMMAND;
			}
		}
		TCHAR sz[80];
		TCHAR szAppName[80];
		LoadString( EEGetLocaleInstanceHandle(), IDS_SURE_TO_UNINSTALL, sz, sizeof( sz ) / sizeof( TCHAR ) );
		LoadString( EEGetLocaleInstanceHandle(), IDS_OPENDOCUMENTS_MENU_TEXT, szAppName, sizeof( szAppName ) / sizeof( TCHAR ) );
		if( MessageBox( hDlg, sz, szAppName, MB_YESNO | MB_ICONEXCLAMATION ) == IDYES ){
			EraseProfile();
			m_bUninstalling = true;
			return UNINSTALL_SIMPLE_DELETE;
		}
		return UNINSTALL_FALSE;
	}

	BOOL QueryProperties( HWND /*hDlg*/ )
	{
		return TRUE;
	}

	BOOL SetProperties( HWND hDlg )
	{
		DialogBox( EEGetLocaleInstanceHandle(), MAKEINTRESOURCE( IDD_PLUGIN_PROP ), hDlg, PropDlg );
		return TRUE;
	}

	BOOL PreTranslateMessage( HWND /*hwndView*/, MSG* pMsg )
	{
		if( m_hwndList && m_hwndList == GetFocus() ){
			if( pMsg->message == WM_KEYDOWN ){
				bool bCtrl = GetKeyState( VK_CONTROL ) < 0;
				bool bShift = GetKeyState( VK_SHIFT ) < 0;
				if( !bCtrl ){
					if( pMsg->wParam == VK_ESCAPE ){
						if( !bShift ){
							Editor_ExecCommand( m_hWnd, EEID_ACTIVE_PANE );
							return TRUE;
						}
					}
					//else if( pMsg->wParam == VK_F6 ){
					//	Editor_ExecCommand( m_hWnd, bShift ? EEID_PREV_PANE : EEID_NEXT_PANE );
					//	return TRUE;
					//}
					else if( pMsg->wParam == VK_F5 ){
						if( !bShift ){
							OnRefresh();
						}
					}
				}
				if( bCtrl ){
					if( pMsg->wParam == 'A' ){
						OnSelectAll();
					}
				}

				if( pMsg->wParam >= VK_PRIOR && pMsg->wParam <= VK_DELETE || pMsg->wParam == VK_TAB || pMsg->wParam == VK_BACK || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN ){
					SendMessage( GetFocus(), pMsg->message, pMsg->wParam, pMsg->lParam );
					return TRUE;
				}
			}
			if( IsDialogMessage( m_hwndList, pMsg ) ){
				return TRUE;
			}
		}
		return FALSE;
	}

	CMyFrame()
	{
		ZERO_INIT_FIRST_MEM( CMyFrame, m_hwndList );
		m_nViewType = VIEW_BLEND;
	}

	~CMyFrame()
	{
	}

#pragma warning( push )
#pragma warning( disable : 4244 ) // 'argument' : conversion from 'LONG_PTR' to 'LONG', possible loss of data
#pragma warning( disable : 4312 )  // 'type cast' : conversion from 'LONG' to 'WNDPROC' of greater size

	void SubclassList( HWND hwndList )
	{
		_ASSERTE( m_lpOldListProc == NULL );
		m_lpOldListProc = (WNDPROC)SetWindowLongPtr( hwndList, GWLP_WNDPROC, (LONG_PTR)ListProc );
	}

	void UnsubclassList( HWND hwndList )
	{
		if( m_lpOldListProc != NULL ){
			SetWindowLongPtr( hwndList, GWLP_WNDPROC, (LONG_PTR)m_lpOldListProc );
			m_lpOldListProc = NULL;
		}
	}

#pragma warning( pop )

	BOOL OnInitDialog( HWND hwnd )
	{
		CenterWindow( hwnd );
		TCHAR sz[256];
		for( int i = 0; i < 4; i++ ){
			LoadString( EEGetLocaleInstanceHandle(), IDS_POS_LEFT + i, sz, _countof( sz ) );
			SendDlgItemMessage( hwnd, IDC_COMBO_POS, CB_ADDSTRING, 0, (LPARAM)sz );
		}
		SendDlgItemMessage( hwnd, IDC_COMBO_POS, CB_SETCURSEL, m_iPos, 0 );
		LoadString( EEGetLocaleInstanceHandle(), IDS_OPENDOCUMENTS_MENU_TEXT, sz, _countof( sz ) );
		SetWindowText( hwnd, sz );
		m_iOldPos = m_iPos;
		return TRUE;
	}
	
	void OnDlgCommand( HWND hwnd, WPARAM wParam )
	{
		switch( wParam ){
		case IDOK:
			{
				m_iPos = (int)SendDlgItemMessage( hwnd, IDC_COMBO_POS, CB_GETCURSEL, 0, 0 );
//				HKEY hKey = GetRootKey();
				WriteProfileInt( _T("CustomBarPos"), m_iPos );
//				if( hKey )  RegCloseKey( hKey );

				EndDialog( hwnd, IDOK );
				if( m_iPos != m_iOldPos ){
					if( m_hwndList ){
						OnCommand( NULL );
						OnCommand( NULL );
					}
				}
			}
			break;

		case IDCANCEL:
			EndDialog( hwnd, IDCANCEL );
			break;
		}
		return;
	}


	void OnCreate()
	{
		_ASSERT( m_hwndList );

		DWORD dwFlags = LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_LABELTIP | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT;
		ListView_SetExtendedListViewStyleEx( m_hwndList, dwFlags, dwFlags );

		TCHAR szText[32] = { 0 };
		LV_COLUMN lvC;
		ZeroMemory( &lvC, sizeof(lvC) );
		lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvC.fmt = LVCFMT_LEFT;   // left-align column
		lvC.pszText = szText;
//		HINSTANCE hinstRes = EEGetInstanceHandle();
//		LoadString( hinstRes, nIDString, szText, _countof( szText ) );
		int nIndex = 0;
		lvC.cx = 100000;
		VERIFY( ListView_InsertColumn( m_hwndList, nIndex, &lvC ) != -1 );

		TCHAR szPath[MAX_PATH];
		GetModuleFileName( NULL, szPath, _countof( szPath ) );
		SHFILEINFO sfi;
		m_himl = (HIMAGELIST)SHGetFileInfo( szPath, 0, &sfi, sizeof( sfi ), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );
		m_iDefaultTabIcon = sfi.iIcon;
		ListView_SetImageList( m_hwndList, m_himl, LVSIL_SMALL );

//		LRESULT  nPreviousTime = ListView_SetHoverTime( m_hwndList, 0 );

		m_bRefreshIdle = true;
	}


	int GetIconImage( LPCTSTR pszPath )
	{
		int iImage = m_iDefaultTabIcon;
		if( pszPath[0] ){
			SHFILEINFO sfi;
			HIMAGELIST hImage = (HIMAGELIST)SHGetFileInfo( pszPath, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof( sfi ), SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES );
			if( hImage != NULL ){
				_ASSERT( hImage == m_himl );
				iImage = sfi.iIcon;
			}
		}
		return iImage;
	}

	void OnFileOpened()
	{
		if( !m_hwndList )  return;

		int nDocCount = (int)Editor_Info( m_hWnd, EI_GET_DOC_COUNT, 0 );
		for( int i = 0; i < nDocCount; i++ ){
			HEEDOC hDoc = (HEEDOC)Editor_Info( m_hWnd, EI_INDEX_TO_DOC, i );
			_ASSERT( hDoc );
			if( hDoc ){
				int iFound = FindDocFromList( hDoc );
				if( iFound < 0 ){
					LVITEM item = { 0 };
					item.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT;
					item.iItem = i;
					item.pszText = LPSTR_TEXTCALLBACK;
					item.iImage = I_IMAGECALLBACK;
					item.lParam = (LPARAM)hDoc;
					int iNewItem = ListView_InsertItem( m_hwndList, &item );
					_ASSERT( iNewItem == i );

					SelectSingleItem( iNewItem );

					ListView_Update( m_hwndList, iNewItem );
				}
//				else {
//					SelectDoc();
//				}
			}
		}
		SelectDoc();
	}

	void OnDocClose( HEEDOC hDoc )
	{
		if( !m_hwndList )  return;
		int iDoc = (int)Editor_Info( m_hWnd, EI_DOC_TO_INDEX, (LPARAM)hDoc );
		_ASSERT( iDoc >= 0 );
		if( iDoc >= 0 ){
			int iFound = FindDocFromList( hDoc );
			_ASSERT( iFound >= 0 );
			if( iFound >= 0 ){
				ListView_DeleteItem( m_hwndList, iFound );
			}
		}
	}

	void OnConfigChanged()
	{
		if( !m_hwndList )  return;
//		m_bRefreshSetTextImage = true;
		m_bRefreshFileOpened = true;
//		OnFileOpened();
	}

	int FindDocFromList( HEEDOC hDoc )
	{
		LVFINDINFO lfi = { 0 };
		lfi.flags = LVFI_PARAM;
		lfi.lParam = (LPARAM)hDoc;
		int iFound = ListView_FindItem( m_hwndList, -1, &lfi );
		return iFound;
	}

	void SelectSingleItem( int iItem )
	{
		int nCount = ListView_GetItemCount( m_hwndList );
		for( int i = 0; i < nCount; i++ ){
			ListView_SetItemState( m_hwndList, i, 0, LVIS_SELECTED );
		}
		ListView_SetItemState( m_hwndList, iItem, LVIS_SELECTED, LVIS_SELECTED );
		ListView_EnsureVisible( m_hwndList, iItem, TRUE );
	}

	void OnRefresh()
	{
		if( !m_hwndList )  return;
		if( !m_bSetRedraw ){
			SendMessage( m_hwndList, WM_SETREDRAW, FALSE, 0 );
		}
		ListView_DeleteAllItems( m_hwndList );
		int nDocCount = (int)Editor_Info( m_hWnd, EI_GET_DOC_COUNT, 0 );
		for( int i = 0; i < nDocCount; i++ ){
			HEEDOC hDoc = (HEEDOC)Editor_Info( m_hWnd, EI_INDEX_TO_DOC, i );
			_ASSERT( hDoc );
			if( hDoc ){
				LVITEM item = { 0 };
				item.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT;
				item.iItem = INT_MAX;
				item.pszText = LPSTR_TEXTCALLBACK;
				item.iImage = I_IMAGECALLBACK;
				item.lParam = (LPARAM)hDoc;
				int iNewItem = ListView_InsertItem( m_hwndList, &item );
				UNUSED( iNewItem );
				_ASSERT( iNewItem == i );
			}
		}
		if( nDocCount > 0 ){
			SelectDoc();
		}
		if( !m_bSetRedraw ){
			SendMessage( m_hwndList, WM_SETREDRAW, TRUE, 0 );
		}
//		OnSelChanged();
	}

	void OnIdle()
	{
		if( m_hwndList ){
			if( m_bRefreshIdle ){
				OnRefresh();
				if( m_bSetRedraw ){
					SendMessage( m_hwndList, WM_SETREDRAW, TRUE, 0 );
					m_bSetRedraw = false;
				}
				m_bRefreshIdle = false;
			}
			if( m_bRefreshFileOpened ){
				OnFileOpened();
				m_bRefreshFileOpened = false;
			}
			if( m_bRefreshSetTextImage ){
				int iDoc = (int)Editor_Info( m_hWnd, EI_GET_ACTIVE_INDEX, 0 );
				_ASSERT( iDoc >= 0 );
				if( iDoc >= 0 ){
					LVITEM item = { 0 };
					item.mask = LVIF_TEXT | LVIF_IMAGE;
					item.pszText = _T("");
					item.iImage = 0;
					item.iItem = iDoc;
					VERIFY( ListView_SetItem( m_hwndList, &item ) );
					item.mask = LVIF_TEXT | LVIF_IMAGE;
					item.pszText = LPSTR_TEXTCALLBACK;
					item.iImage = I_IMAGECALLBACK;
					item.iItem = iDoc;
					VERIFY( ListView_SetItem( m_hwndList, &item ) );
				}
				m_bRefreshSetTextImage = false;
			}
			if( m_bFocusView ){
				Editor_ExecCommand( m_hWnd, EEID_ACTIVE_PANE );
				m_bFocusView = false;
			}
		}
	}

	void OnTabMoved()
	{
		if( m_hwndList ){
			OnRefresh();
		}
	}

	void OnSelChanged()
	{
		SelectDoc();
	}

	void SelectDoc()
	{
		if( !m_hwndList )  return;

		if( m_bIgnoreSelChanged )  return;

		int nDocCount = (int)Editor_Info( m_hWnd, EI_GET_DOC_COUNT, 0 );
		if( nDocCount != ListView_GetItemCount( m_hwndList ) ){
			m_bRefreshIdle = true;
			return;
		}


		int iDoc = (int)Editor_Info( m_hWnd, EI_GET_ACTIVE_INDEX, 0 );
		_ASSERT( iDoc >= 0 );
		if( iDoc >= 0 ){
			TCHAR szOldDir[MAX_PATH];
			StringCopy( szOldDir, MAX_PATH, m_szCurrDir );
			TCHAR szPath[MAX_PATH] = { 0 };
			Editor_DocInfo( m_hWnd, iDoc, EI_GET_FILE_NAMEW, (LPARAM)szPath );
			GetCurrDir( szPath, m_szCurrDir );
			if( m_nViewType == VIEW_BLEND ){
				if( !IsPathEqual( szOldDir, m_szCurrDir ) ){
					OnRefresh();
				}
			}

			HEEDOC hDoc = (HEEDOC)Editor_Info( m_hWnd, EI_INDEX_TO_DOC, iDoc );
			_ASSERT( hDoc );
			if( hDoc ){
				LVFINDINFO lfi = { 0 };
				lfi.flags = LVFI_PARAM;
				lfi.lParam = (LPARAM)hDoc;
				int iFound = ListView_FindItem( m_hwndList, -1, &lfi );
//				_ASSERT( iFound >= 0 );
				if( iFound >= 0 ){
					SelectSingleItem( iFound );
					ListView_Update( m_hwndList, iFound );
				}
			}

		}
	}

	void OnModified()
	{
		if( !m_hwndList )  return;
		int iActiveDoc = (int)Editor_Info( m_hWnd, EI_GET_ACTIVE_INDEX, 0 );
		ListView_Update( m_hwndList, iActiveDoc );

		TCHAR szOldDir[MAX_PATH];
		StringCopy( szOldDir, MAX_PATH, m_szCurrDir );
		TCHAR szPath[MAX_PATH] = { 0 };
		Editor_DocInfo( m_hWnd, iActiveDoc, EI_GET_FILE_NAMEW, (LPARAM)szPath );
		GetCurrDir( szPath, m_szCurrDir );
		if( m_nViewType == VIEW_BLEND ){
			if( !IsPathEqual( szOldDir, m_szCurrDir ) ){
				OnRefresh();
			}
		}


//		SelectDoc();
	}

	void OnGetDispInfo( NMLVDISPINFO* pGetDispInfo )
	{
		if( pGetDispInfo->hdr.hwndFrom == m_hwndList ){
//			int iDoc = (int)Editor_Info( m_hWnd, EI_DOC_TO_INDEX, pGetDispInfo->item.lParam );
//			_ASSERT( iDoc >= 0 );
			if( pGetDispInfo->item.iItem >= 0 ){
//				_ASSERT( pGetDispInfo->item.iItem < ListView_GetItemCount() );
				int nCount = (int)Editor_Info( m_hWnd, EI_GET_DOC_COUNT, 0 );
				if( pGetDispInfo->item.iItem < nCount ){
					TCHAR szPath[MAX_PATH] = { 0 };
					Editor_DocInfo( m_hWnd, pGetDispInfo->item.iItem, EI_GET_FILE_NAMEW, (LPARAM)szPath );
					if( pGetDispInfo->item.mask & LVIF_IMAGE ){
						pGetDispInfo->item.iImage = szPath[0] ? GetIconImage( szPath ) : m_iDefaultTabIcon;
					}
					if( pGetDispInfo->item.mask & LVIF_TEXT ){
						bool bShortTitle;
						if( m_nViewType == VIEW_BLEND ){
							TCHAR szDir[MAX_PATH];
							GetCurrDir( szPath, szDir );
							bShortTitle = IsPathEqual( szDir, m_szCurrDir );
						}
						else if( m_nViewType == VIEW_TITLE ) {
							bShortTitle = true;
						}
						else {
							bShortTitle = false;
						}
						TCHAR szTitle[MAX_PATH] = { 0 };
						Editor_DocInfo( m_hWnd, pGetDispInfo->item.iItem, bShortTitle ? EI_GET_SHORT_TITLEW : EI_GET_FULL_TITLEW, (LPARAM)szTitle );
						StringCopy( pGetDispInfo->item.pszText, pGetDispInfo->item.cchTextMax, szTitle );
					}
				}
			}
		}
	}

	void OnSize()
	{
		if( !m_hwndList )  return;

//		CRedraw r( m_hwndList );

		if( !m_bSetRedraw ){
			SendMessage( m_hwndList, WM_SETREDRAW, FALSE, 0 );
			m_bSetRedraw = true;
		}

		RECT rc;
		GetClientRect( m_hwndList, &rc );

		ListView_SetColumnWidth( m_hwndList, 0, rc.right - rc.left );
//		OnRefresh();
		m_bRefreshIdle = true;
	}

	void OnActivate( int iItem )
	{
		Editor_DocInfo( m_hWnd, iItem, EI_SET_ACTIVE_INDEX, 0 );
		m_bFocusView = true;
	}

	void OnItemActivate( NMITEMACTIVATE* pItemActivate )
	{
		if( pItemActivate->iItem >= 0 ){
			OnActivate( pItemActivate->iItem );
		}
	}

	void OnItemChanged( NMLISTVIEW* pListView )
	{
		if( pListView->iItem >= 0 ){
			if( pListView->uNewState & LVIS_SELECTED ){

			}
		}
	}

	void OnSave()
	{
//		m_bIgnoreSelChanged = true;

		int iItem = -1;
		for( ;; ){
			iItem = ListView_GetNextItem( m_hwndList, iItem, LVNI_SELECTED );
			if( iItem < 0 )  break;
			Editor_DocInfo( m_hWnd, iItem, EI_SAVE_DOC, 0 );
			ListView_Update( m_hwndList, iItem );
		}

//		m_bIgnoreSelChanged = false;
	}

	void OnMButtonDown()
	{
		LVHITTESTINFO hti = { 0 };
		GetCursorPos( &hti.pt );
		ScreenToClient( m_hwndList, &hti.pt );
		int iItem = ListView_HitTest( m_hwndList, &hti );
		if( iItem >= 0 ){
			int nDocCount = (int)Editor_Info( m_hWnd, EI_GET_DOC_COUNT, 0 );
			if( nDocCount == 1 ){
				Editor_ExecCommand( m_hWnd, EEID_APP_EXIT );
			}
			else {
				Editor_DocInfo( m_hWnd, iItem, EI_CLOSE_DOC, 0 );
			}
		}
	}

	void OnClose()
	{
		m_bIgnoreSelChanged = true;
		int nSelected = ListView_GetSelectedCount( m_hwndList );
		for( int i = 0; i < nSelected; i++ ){
			int iItem = ListView_GetNextItem( m_hwndList, -1, LVNI_SELECTED );
			if( iItem < 0 )  break;

			if( i == nSelected - 1 ){
				int nDocCount = (int)Editor_Info( m_hWnd, EI_GET_DOC_COUNT, 0 );
				if( nDocCount == 1 ){
					Editor_ExecCommand( m_hWnd, EEID_APP_EXIT );
					break;
				}
			}
			Editor_DocInfo( m_hWnd, iItem, EI_CLOSE_DOC, 0 );
		}
		m_bIgnoreSelChanged = false;

	}

	void QueryCheckMenu( HMENU hMenu, int nID )
	{
		BOOL bChecked = FALSE;
		BOOL bEnable = Editor_QueryStatus( m_hWnd, nID, &bChecked );
		CheckMenuItem( hMenu, nID, MF_BYCOMMAND | (bChecked ? MF_CHECKED : MF_UNCHECKED) );
		EnableMenuItem( hMenu, nID, MF_BYCOMMAND | (bEnable ? MF_ENABLED : MF_GRAYED) );
	}

	void ContextMenu( POINT* pptPos, int iItem )
	{
		HMENU hMainMenu = LoadMenu( EEGetLocaleInstanceHandle(), MAKEINTRESOURCE(IDR_OPENDOCUMENTS_CONTEXT_MENU) );
		if( hMainMenu == NULL )  return;

		HMENU hMenu = GetSubMenu( hMainMenu, 0 );
		if( hMenu == NULL )  return;
		QueryCheckMenu( hMenu, ID_GROUP_CLOSE_OTHERS );
		QueryCheckMenu( hMenu, ID_GROUP_CLOSE_LEFT );
		QueryCheckMenu( hMenu, ID_GROUP_CLOSE_RIGHT );
		QueryCheckMenu( hMenu, ID_NEW_GROUP );
		QueryCheckMenu( hMenu, ID_NEW_GROUP_MINIMIZE );
		QueryCheckMenu( hMenu, ID_MOVE_PREV_GROUP );
		QueryCheckMenu( hMenu, ID_MOVE_NEXT_GROUP );

		HMENU hArrangeMenu = GetSubMenu( hMenu, 17 );
		_ASSERT( hArrangeMenu );

		QueryCheckMenu( hArrangeMenu, ID_SORT_FILE_NAME );
		QueryCheckMenu( hArrangeMenu, ID_SORT_TYPE );
		QueryCheckMenu( hArrangeMenu, ID_SORT_MODIFIED );
		QueryCheckMenu( hArrangeMenu, ID_SORT_ACCESSED );
		QueryCheckMenu( hArrangeMenu, ID_SORT_ASCENDING );
		QueryCheckMenu( hArrangeMenu, ID_SORT_DESCENDING );
		QueryCheckMenu( hArrangeMenu, ID_AUTO_SORT );

		HMENU hViewMenu = GetSubMenu( hMenu, 18 );
		_ASSERT( hViewMenu );

		CheckMenuRadioItem( hViewMenu, ID_VIEW_FULL, ID_VIEW_BLEND, ID_VIEW_FULL + m_nViewType, MF_BYCOMMAND );

		SetMenuDefaultItem( hMenu, ID_ACTIVATE, FALSE );

		int nID = TrackPopupMenu( hMenu, (GetSystemMetrics( SM_MENUDROPALIGNMENT ) ? TPM_RIGHTALIGN : TPM_LEFTALIGN) | TPM_RIGHTBUTTON | TPM_RETURNCMD, pptPos->x, pptPos->y, 0, m_hwndList, NULL );
		DestroyMenu( hMainMenu );

		switch( nID ){
		case ID_ACTIVATE:
			OnActivate( iItem );
			break;
		case ID_SAVE:
			OnSave();
			break;
		case ID_CLOSE:
			OnClose();
			break;
		case ID_REFRESH:
			OnRefresh();
			break;
		case ID_PROP:
			SetProperties( m_hWnd );
			break;
		case ID_SELECT_ALL:
			OnSelectAll();
			break;
		case ID_GROUP_CLOSE_OTHERS:
		case ID_GROUP_CLOSE_LEFT:
		case ID_GROUP_CLOSE_RIGHT:
		case ID_NEW_GROUP:
		case ID_NEW_GROUP_MINIMIZE:
		case ID_MOVE_PREV_GROUP:
		case ID_MOVE_NEXT_GROUP:
			OnActivate( iItem );
			Editor_ExecCommand( m_hWnd, nID );
			OnRefresh();
			break;
		case ID_SORT_FILE_NAME:
		case ID_SORT_TYPE:
		case ID_SORT_MODIFIED:
		case ID_SORT_ACCESSED:
		case ID_SORT_ASCENDING:
		case ID_SORT_DESCENDING:
		case ID_AUTO_SORT:
			Editor_ExecCommand( m_hWnd, nID );
			break;

		case ID_VIEW_FULL:
			m_nViewType = VIEW_FULL;
			OnRefresh();
			break;
		case ID_VIEW_TITLE:
			m_nViewType = VIEW_TITLE;
			OnRefresh();
			break;
		case ID_VIEW_BLEND:
			m_nViewType = VIEW_BLEND;
			OnRefresh();
			break;

		}

	}


	void OnContextMenu()
	{
		POINT ptPos;
		ptPos.x = ptPos.y = 4;
		int iItem = ListView_GetNextItem( m_hwndList, -1, LVNI_SELECTED );
		if( iItem >= 0 ){
			RECT rc = { 0 };
			ListView_GetItemRect( m_hwndList, iItem, &rc, LVIR_LABEL );
			ptPos.x = rc.left;
			ptPos.y = (rc.top + rc.bottom) / 2;
		}

		ClientToScreen( m_hwndList, &ptPos );
		ContextMenu( &ptPos, iItem );
	}

	void OnNMRClick( NMITEMACTIVATE* pItemActivate )
	{
		int iItem = pItemActivate->iItem;
		POINT ptPos = { 0, 0 };
		::GetCursorPos(&ptPos);
		return ContextMenu( &ptPos, iItem );
	}

	void OnSelectAll()
	{
		int nCount = ListView_GetItemCount( m_hwndList );
		for( int i = 0; i < nCount; i++ ){
			ListView_SetItemState( m_hwndList, i, LVIS_SELECTED, LVIS_SELECTED );
		}
	}

	void SetListColor()
	{
		if( m_hwndList ){
			DWORD dwColor = (DWORD)Editor_Info( m_hWnd, EI_GET_BAR_BACK_COLOR, SMART_COLOR_NORMAL );
			ListView_SetBkColor( m_hwndList, dwColor );
			ListView_SetTextBkColor( m_hwndList, dwColor );
			dwColor = (DWORD)Editor_Info( m_hWnd, EI_GET_BAR_TEXT_COLOR, SMART_COLOR_NORMAL );
			ListView_SetTextColor( m_hwndList, dwColor );
			InvalidateRect( m_hwndList, NULL, TRUE );
		}
	}

};

LRESULT CALLBACK ListProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	CMyFrame* pFrame = (CMyFrame*)GetFrameFromDlg( hwnd );
	_ASSERT( pFrame );
	switch( msg ){
	case OCM_NOTIFY:
		{
			NMHDR* pnmh = (NMHDR*)lParam;
			switch( pnmh->code ) {
			case NM_DBLCLK:
			case NM_RETURN:
				{
				}
				break;
			case NM_RCLICK:
				if( pFrame ){
					pFrame->OnNMRClick( (NMITEMACTIVATE*)pnmh );
					return TRUE;
				}
				break;
			case NM_CLICK:
				if( pFrame ){
					if( !pFrame->m_bIgnoreNMClick ){
						pFrame->OnItemActivate( (NMITEMACTIVATE*)pnmh );
					}
				}
				break;
			case LVN_GETDISPINFO:
				if( pFrame ){
					pFrame->OnGetDispInfo( (NMLVDISPINFO*)pnmh );
				}
				break;
			case LVN_ITEMACTIVATE:
				if( pFrame ){
					pFrame->OnItemActivate( (NMITEMACTIVATE*)pnmh );
				}
				break;
			case LVN_ITEMCHANGED:
				if( pFrame ){
					pFrame->OnItemChanged( (NMLISTVIEW*)pnmh );
				}
				break;
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
		}
		break;
	case WM_MBUTTONDOWN:
		if( pFrame ){
			pFrame->OnMButtonDown();
		}
		break;
	case WM_CONTEXTMENU:
		{
			if( pFrame ){
				pFrame->OnContextMenu();
			}
		}
		break;
	case WM_SIZE:
		if( pFrame ){
			pFrame->OnSize();
		}
		break;
//	case WM_INITMENUPOPUP:

	}
	if( pFrame != NULL && pFrame->m_lpOldListProc != NULL ){
		return CallWindowProc( pFrame->m_lpOldListProc, hwnd, msg, wParam, lParam);
	}
	else {
		return 0;
	}
}

INT_PTR CALLBACK PropDlg( HWND hwnd, UINT msg, WPARAM wParam, LPARAM /*lParam*/ )
{
	BOOL bResult = FALSE;
	switch( msg ){
	case WM_INITDIALOG:
		{
			CMyFrame* pFrame = static_cast<CMyFrame*>(GetFrameFromDlg( hwnd ));
			_ASSERTE( pFrame );
			bResult = pFrame->OnInitDialog( hwnd );
		}
		break;
	case WM_COMMAND:
		{
			CMyFrame* pFrame = static_cast<CMyFrame*>(GetFrameFromDlg( hwnd ));
			_ASSERTE( pFrame );
			pFrame->OnDlgCommand( hwnd, wParam );
		}
		break;
	}
	return bResult;
}

// the following line is needed after CMyFrame definition
_ETL_IMPLEMENT

