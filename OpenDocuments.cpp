// WordCount.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#pragma warning( push )
#pragma warning( disable : 4005 ) // macro redefinition
#include "resource.h"
//#include "..\mui\opendocuments_loce\resource.h"
#include "..\..\emedloce\resource.h"
#pragma warning( pop )

// the following line is needed before #include <etlframe.h>
#define ETL_FRAME_CLASS_NAME CMyFrame
#include "etlframe.h"
#include "mystring.h"
#include "myreg.h"

// returns true if right-handed (if menu should pop up left), should NOT be used for main menu or pull-down menus (BCN_DROPDOWN)
[[nodiscard]] inline bool RightAligned()
{
	return ( GetSystemMetrics( SM_MENUDROPALIGNMENT ) );
}

// returns right or left aligned flag used for TrackPopupMenu()
[[nodiscard]] inline UINT GetMenuAlign()
{
	return RightAligned() ? TPM_RIGHTALIGN : TPM_LEFTALIGN;
}

#include "OpenDocuments.h"

