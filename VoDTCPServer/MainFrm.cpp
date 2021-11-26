// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "VoDTCPServer.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern TCHAR ServerPathName[];
extern TCHAR ServerPath[];
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_TCP_SHUTDOWNVODTCPSERVER, OnTcpShutdownvodtcpserver)
	ON_COMMAND(ID_TCP_HIDESERVER, OnTcpHideserver)
	ON_COMMAND(ID_TCP_SHOWSERVER, OnTcpShowserver)
	ON_MESSAGE(WM_SYSTRAY,OnSysTray)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

LONG CMainFrame::OnSysTray(WPARAM wParam,LPARAM lParam)
{
	m_SysTray.TrayNotification(wParam,lParam);
	return 0;
}

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
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
	m_SysTray.Init(this->m_hWnd,(LPCTSTR)IDR_MAINFRAME,"VoD TCP server",12768,IDR_MENU1);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
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


// CMainFrame message handlers


void CMainFrame::OnTcpShutdownvodtcpserver()
{
	this->PostMessage(WM_QUIT,0,0);
}

void CMainFrame::OnTcpHideserver()
{
	ShowWindow(SW_HIDE);
}

void CMainFrame::OnTcpShowserver()
{
	ShowWindow(SW_SHOW);
}


void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID==SC_CLOSE)
	{
		this->ShowWindow(SW_HIDE);
		return;
	}
	CFrameWnd::OnSysCommand(nID, lParam);
}
