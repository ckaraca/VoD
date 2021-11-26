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
#ifdef _DEBUG
	AfxEnableMemoryTracking(TRUE);
#endif
}

CVoDPlayerApp::~CVoDPlayerApp()
{

#ifdef _DEBUG

#endif
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

	// exception
	// Expect this code to fail on any operating system prior to Windows XP
// Hence take no error handling action if it fails

typedef BOOL (__stdcall *AddERExcludedApplicationW_func)(LPCWSTR	szApplicationName);
HMODULE	hModule;

hModule = LoadLibrary(_T("faultrep.dll"));
if (hModule != NULL)
{
	AddERExcludedApplicationW_func	addExFunc;

	addExFunc = (AddERExcludedApplicationW_func)GetProcAddress(hModule, "AddERExcludedApplicationW");
	if (addExFunc != NULL)
	{
		// note that because we chose the UNICODE (W suffix) version of the API, we must
		// force UNICODE characters using L (not _T("") which can also do ANSI)

		(addExFunc)(L"testER.exe");
	}

	FreeLibrary(hModule);
}

	// end micro


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
	Connection = this->GetProfileString("Connection","CON_TYP","UDP");
	cs = this->GetProfileInt("Connection","CON_SP",0);
	BufferFTP = this->GetProfileInt("Buffer","BUFFERFTP",16384);
	BufferTCP = this->GetProfileInt("Buffer","BUFFERTCP",16384);
	BufferUDP = this->GetProfileInt("Buffer","BUFFERUDP",16384);
	Stats = this->GetProfileInt("Stats","STATS",0);
	Aspect = this->GetProfileInt("Aspect","ASPECT",0);
	Size = this->GetProfileInt("Size","ASPECT",0);
}
