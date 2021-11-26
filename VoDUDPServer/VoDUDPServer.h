// VoDUDPServer.h : main header file for the VoDUDPServer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CVoDUDPServerApp:
// See VoDUDPServer.cpp for the implementation of this class
//

class CVoDUDPServerApp : public CWinApp
{
public:
	CVoDUDPServerApp();
	BOOL BrowseFolder(LPSTR Folder, LPSTR Name);


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileSetserverpath();
};

extern CVoDUDPServerApp theApp;