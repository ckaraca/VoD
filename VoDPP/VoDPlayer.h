// VoDPlayer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define CLASSNAME "VideoOnDemand"
// CVoDPlayerApp:
// See VoDPlayer.cpp for the implementation of this class
//

class CVoDPlayerApp : public CWinApp
{
public:
	CVoDPlayerApp();
	~CVoDPlayerApp();
	void GetProfiles(void);
// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation
public:
	HKEY hRegKey;
	unsigned long datasize,result;
	
	
	DECLARE_MESSAGE_MAP()
};

extern CVoDPlayerApp theApp;