// VoDTCPServer.h : main header file for the VoDTCPServer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CVoDTCPServerApp:
// See VoDTCPServer.cpp for the implementation of this class
//

class CVoDTCPServerApp : public CWinApp
{
public:
	CVoDTCPServerApp();
	BOOL BrowseFolder(LPSTR Folder, LPSTR Name);


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileSetserverpath();
};

extern CVoDTCPServerApp theApp;