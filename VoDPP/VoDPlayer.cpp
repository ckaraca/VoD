// VoDPlayer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VoDPlayer.h"
#include "VoDPlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVoDPlayerApp

BEGIN_MESSAGE_MAP(CVoDPlayerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

extern int Return;
extern TCHAR VoDPath[MAX_PATH];
// CVoDPlayerApp construction


CVoDPlayerApp::CVoDPlayerApp()
{
	Return = 0;
}

CVoDPlayerApp::~CVoDPlayerApp()
{
	RegCreateKeyEx(HKEY_CURRENT_USER,
				"AppEvents\\Schemes\\Apps\\Explorer\\Navigating\\.current",
				0, "", 0, KEY_ALL_ACCESS,
				NULL, &hRegKey, &result);
	//RegSetValueEx(hRegKey,"siir",0,REG_BINARY,(LPBYTE) &poem,sizeof(poem));
	TCHAR text[MAX_PATH];
	wsprintf(text,"C:\\WINNT\\media\\start.wav");
	RegSetValueEx(hRegKey,"",0,REG_SZ,(LPBYTE)&text,(DWORD)strlen(text));
	RegCloseKey(hRegKey);
}

// The one and only CVoDPlayerApp object

CVoDPlayerApp theApp;


// CVoDPlayerApp initialization

BOOL CVoDPlayerApp::InitInstance()
{
	InitCommonControls();
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	
	SetRegistryKey(IDS_APPLICATON);
	
	CVoDPlayerDlg dlg;
	m_pMainWnd = &dlg;
	SetWindowText(dlg.m_hWnd,"Video on Demand");
	GetProfiles();

	if (SUCCEEDED(SHGetSpecialFolderPath(NULL,VoDPath,CSIDL_LOCAL_APPDATA,1)))
	{
		//
		SetCurrentDirectory(VoDPath);
		CreateDirectory("VoD",NULL);
		PathAppend(VoDPath,"VoD");		
	}
	
	RegCreateKeyEx(HKEY_CURRENT_USER,
				"AppEvents\\Schemes\\Apps\\Explorer\\Navigating\\.current",
				0, "", 0, KEY_ALL_ACCESS,
				NULL, &hRegKey, &result);
	char text[MAX_PATH];
	wsprintf(text,"C:\\WINNT\\media\\start.wa1");
	RegSetValueEx(hRegKey,"",0,REG_SZ,(LPBYTE)text,(DWORD)strlen(text));
	RegCloseKey(hRegKey);


	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	return FALSE;
}
extern int Stats;
extern CString Connection;
extern int	BufferFTP,BufferTCP,BufferUDP,cs;
extern int Aspect;
extern int Size;
extern CVoDPlayerApp* Main;

void CVoDPlayerApp::GetProfiles(void)
{
	Main = this;
	Connection = this->GetProfileString("Connection","CON_TYP","RUDP");
	cs = this->GetProfileInt("Connection","CON_SP",0);
	BufferFTP = this->GetProfileInt("Buffer","BUFFERFTP",16384);
	BufferTCP = this->GetProfileInt("Buffer","BUFFERTCP",16384);
	BufferUDP = this->GetProfileInt("Buffer","BUFFERUDP",65536);
	Stats = this->GetProfileInt("Stats","STATS",0);
	Aspect = this->GetProfileInt("Aspect","ASPECT",0);
	Size = this->GetProfileInt("Size","ASPECT",0);
}
