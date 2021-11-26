// VoD.h : main header file for the VoD application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols



#define WM_USER_THREAD_FINISHED WM_USER+0x100

// CVoDApp:
// See VoD.cpp for the implementation of this class
//

class CVoDApp : public CWinApp
{
public:
	CVoDApp();
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVoDApp theApp;