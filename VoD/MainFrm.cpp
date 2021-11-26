// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "VoD.h"
#include "MainFrm.h"
#include "VoDView.h"
#include "Html.h"
#include "Source.h"
#include "Design.h"
#include "Login.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE (WM_SWITCH, Switch)
	ON_COMMAND(ID_BUILD_UPLOADPROJECT, OnBuildUploadproject)
	ON_COMMAND(ID_BUILD_ADDFILESERVER, OnBuildAddfileserver)
	ON_COMMAND(ID_FILE_CLOSEPROJECT, OnFileCloseproject)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	::GetModuleFileName (NULL, szPath, sizeof (szPath) / sizeof (TCHAR));
	TCHAR* strPath;
	
	TCHAR Path[MAX_PATH];
	strPath = ResourceToURL("VoD.html");
	// Now extract banner and swf to temp dir
    GetTempPath(MAX_PATH,szTempPath);
	wsprintf(Path,"%sVoD.html",szTempPath);
	ExtractResource(Path,IDR_DATA1);
	wsprintf(Path,"%sVoDanm",szTempPath);
	ExtractResource(Path,IDR_DATA2);
    SetFile("VoD.html");
	wsprintf(Path,"%sVoD.ico",szTempPath);
	ExtractResource(Path,IDR_DATA3);
	RegCreateKeyEx(HKEY_CURRENT_USER,
				"AppEvents\\Schemes\\Apps\\Explorer\\Navigating\\.current",
				0, "", 0, KEY_ALL_ACCESS,
				NULL, &hRegKey, &result);
	char text[MAX_PATH];
	wsprintf(text,"C:\\WINNT\\media\\start.wa1");
	RegSetValueEx(hRegKey,"",0,REG_SZ,(LPBYTE)text,(DWORD)strlen(text));
	RegCloseKey(hRegKey);
	delete []strPath;
}

void CMainFrame::ExtractResource(TCHAR *path,int DATA)
{
	HRSRC hRes = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(DATA),RT_RCDATA);//RT_RCDATA
	DWORD dwSize = SizeofResource(AfxGetResourceHandle() , hRes);
	HGLOBAL MemoryHandle = LoadResource(AfxGetResourceHandle(), hRes);
	if(MemoryHandle != NULL)
	{
		// LockResource returns a BYTE pointer to the raw data in the resource
		try{
            CFile file(path, CFile::modeCreate | CFile::modeWrite);
			file.Write(MemoryHandle,dwSize);
			file.Close();
		} catch (CFileException *e)
		{
			e->ReportError();
			e->Delete();
		}
	
	}
	FreeResource((HANDLE)hRes);
}

void CMainFrame::SetSource(CString Source)
{
	this->Source = Source;
}

CString* CMainFrame::GetSource(void)
{
	return &Source;
}

TCHAR* CMainFrame::ResourceToURL(LPCTSTR lpszURL)
{
	TCHAR *m_strURL = new TCHAR[MAX_PATH+100];
	HINSTANCE hInstance = AfxGetResourceHandle();
	ASSERT(hInstance != NULL);
	
	LPTSTR lpszModule = new TCHAR[_MAX_PATH];
	
	if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
	{
		wsprintf(m_strURL,_T("res://%s/%s"), lpszModule, lpszURL);
	}
	
	delete []lpszModule;

	return m_strURL;
}

CMainFrame::~CMainFrame()
{
	m_hMenu.DestroyMenu();
	m_hMenu1.DestroyMenu();
	extern TCHAR ExtFile[];
	extern TCHAR TempPath[];
	try
	{
		CFile::Remove(ExtFile);
	}
	catch (CFileException* pEx)
	{
		//pEx->ReportError();
		pEx->Delete();
	}
	RegCreateKeyEx(HKEY_CURRENT_USER,
				"AppEvents\\Schemes\\Apps\\Explorer\\Navigating\\.current",
				0, "", 0, KEY_ALL_ACCESS,
				NULL, &hRegKey, &result);
	//RegSetValueEx(hRegKey,"siir",0,REG_BINARY,(LPBYTE) &poem,sizeof(poem));
	TCHAR text[MAX_PATH];
	wsprintf(text,"C:\\WINNT\\media\\start.wav");
	RegSetValueEx(hRegKey,"",0,REG_SZ,(LPBYTE)&text,(DWORD)strlen(text));
	RegCloseKey(hRegKey);
	//delete m_Sql;
	// delete files in temp!!!
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	if (!m_wndFormatBar.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndFormatBar.LoadToolBar(IDR_FORMATBAR))
	{
		TRACE0("Failed to create formatbar\n");
		return -1;		// fail to create
	}
	

	m_wndFormatBar.SetBarStyle(m_wndFormatBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndFormatBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFormatBar.SetBorders(3, 3, 3, 3);
	EnableDocking(CBRS_ALIGN_ANY);
	RECT rt;
	GetWindowRect(&rt);
	DockControlBar(&m_wndFormatBar, AFX_IDW_DOCKBAR_LEFT ,&rt);
	ShowControlBar(&m_wndFormatBar,FALSE,TRUE);
	m_hMenu.LoadMenu(IDR_MAINFRAME);
	m_hMenu1.LoadMenu(IDR_MAINFRAME1);
	m_hMenu2.LoadMenu(IDR_MAINFRAME2);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	
	
	return TRUE;
}
LONG CMainFrame::Switch(WPARAM wParam,LPARAM lParam)
{
	CVoDView * cF;
	CDesign * cD;
	switch(wParam)
	{
	case 0:
		SetWindowPos(NULL,0,0,700,650,SWP_NOMOVE|SWP_NOOWNERZORDER);
		try {
            m_wndSplitter.ReplaceView(0,0,RUNTIME_CLASS(CVoDView), CSize (620, 400));
		}
		catch (CException *e)
		{
			e->ReportError();
			e->Delete();
		}
		try
		{
			CMenu menu; HMENU menu1;
			menu1  = menu.GetSafeHmenu();
			DestroyMenu(menu1);
			SetMenu(&m_hMenu1);
		}
		catch(CException *e)
		{
			e->ReportError();
			e->Delete();
		}
		break;
	case 1:
		SetWindowPos(NULL,0,0,700,450,SWP_NOMOVE|SWP_NOOWNERZORDER);
		try {
			m_wndSplitter.ReplaceView(0,0,RUNTIME_CLASS(Login), CSize (600, 150));
		}
		catch (CException *e)
		{
			e->ReportError();
			e->Delete();
		}
		try
		{
			CMenu menu; HMENU menu1;
			menu1  = menu.GetSafeHmenu();
			DestroyMenu(menu1);
			SetMenu(&m_hMenu);
		}
		catch(CException *e)
		{
			e->ReportError();
			e->Delete();
		}
		break;
	case 2:
		ShowControlBar(&m_wndFormatBar,FALSE,TRUE);
		try {
			m_wndSplitter.ReplaceView(1,0,RUNTIME_CLASS(CHtml), CSize (600, 150));
		}
		catch (CException *e)
		{
			e->ReportError();
			e->Delete();
		}
		
		break;
	case 3:
		try {
			m_wndSplitter.ReplaceView(1,0,RUNTIME_CLASS(CDesign), CSize (600, 220));
		}
		catch (CException *e)
		{
			e->ReportError();
			e->Delete();
		}
		ShowControlBar(&m_wndFormatBar,TRUE,TRUE);
		cF = (CVoDView *)this->m_wndSplitter.GetPane(0,0);
		ASSERT(cF);
		cD = (CDesign *)this->m_wndSplitter.GetPane(1,0);
		ASSERT(cD);
		RecalcLayout(TRUE);
		cF->ResizeParentToFit(FALSE);
		cF->SendMessage(WM_DESIGN,0,0);
		break;
	case 4:
		ShowControlBar(&m_wndFormatBar,FALSE,TRUE);
		try {
			m_wndSplitter.ReplaceView(1,0,RUNTIME_CLASS(CSource), CSize (600, 150));
		}
		catch (CException *e)
		{
			e->ReportError();
			e->Delete();
		}
		
		//cF = (CVoDView *)this->m_wndSplitter.GetPane(0,0);
		break;
	case 5:
		try {
			m_wndSplitter.ReplaceView(0,0,RUNTIME_CLASS(Empty), CSize (600, 250));
		}
		catch (CException *e)
		{
			e->ReportError();
			e->Delete();
		}
		try
		{
			CMenu menu; HMENU menu1;
			menu1  = menu.GetSafeHmenu();
			DestroyMenu(menu1);
			SetMenu(&m_hMenu2);
		}
		catch(CException *e)
		{
			e->ReportError();
			e->Delete();
		}
		RecalcLayout(TRUE);
		break;
	}	
	return 0;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::SetSql(CSql refSql)
{
	m_Sql = refSql;
}
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	SetWindowPos(NULL,0,0,600,435,SWP_NOMOVE|SWP_NOOWNERZORDER);
	if (!m_wndSplitter.CreateStatic (this, 2, 1) ||
        !m_wndSplitter.CreateView (0, 0, RUNTIME_CLASS
            (Login), CSize (600, 150), pContext) || 
        !m_wndSplitter.CreateView (1, 0, RUNTIME_CLASS (CHtml),
            CSize (600, 130), pContext))
        return FALSE;
	return TRUE;
}

CString CMainFrame::GetFile(void)
{
	return m_File;
}

void CMainFrame::SetFile(TCHAR *File)
{
	m_File.Format("%s%s",szTempPath,File);
}

void CMainFrame::SetFileManually(TCHAR *File)
{
	m_File.Format("%s",File);
}

// // ping DB
void CMainFrame::Ping(void)
{
	 CString pings;
	 int ping=0;
	 if(ping = mysql_ping(m_Sql.myData)==-1) pings.Format("DB LOst!, will try to reconnect");
	 else pings.Format("Pinging DB: %d ms",ping);
	 m_wndStatusBar.SetPaneText(0,pings,TRUE);
}

void CMainFrame::Say(TCHAR *Text)
{
	m_wndStatusBar.SetPaneText(0,Text,TRUE);
}


void CMainFrame::OnBuildUploadproject()
{
	CFtpd ftp;
	ftp.DoModal();
}
#include "NewFtp.h"
void CMainFrame::OnBuildAddfileserver()
{
	AfxMessageBox("This option is disabled in multi-server environments");
	//CNewFtp ftp;
	//ftp.DoModal();
}

void CMainFrame::OnFileCloseproject()
{
	CVoDView * cF = (CVoDView *)this->m_wndSplitter.GetPane(0,0);
	ASSERT(cF);
	cF->SendMessage(WM_CLOSEPROJECT,0,0);
	
}

void CMainFrame::OnFileOpen()
{
	::PostMessage(this->m_hWnd,WM_SWITCH,0,0);
}

BOOL CMainFrame::DestroyWindow()
{
	CDelete *Delete = new CDelete;
	Delete->Create(IDD_DELETE,NULL);

	return CFrameWnd::DestroyWindow();
}
