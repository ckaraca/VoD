// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "VoDUDPServer.h"
extern int m_Ssp;
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_SYSTRAY,OnSysTray)
	ON_COMMAND(ID_UDP_SHUTDOWNVODTCPSERVER, OnUdpShutdownvodtcpserver)
	ON_COMMAND(ID_UDP_HIDESERVER, OnUdpHideserver)
	ON_COMMAND(ID_UDP_SHOWSERVER, OnUdpShowserver)
	ON_WM_TIMER()
//	ON_WM_NCLBUTTONDOWN()
//ON_WM_MOVE()
//ON_WM_MOVING()
ON_WM_SYSCOMMAND()
ON_COMMAND(ID_FILE_SETSERVERSPEED, OnFileSetserverspeed)
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
	m_SysTray.Init(this->m_hWnd,(LPCTSTR)IDR_MAINFRAME,"VoD UDP server",12767,IDR_MENU1);

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


void CMainFrame::OnUdpShutdownvodtcpserver()
{
	this->PostMessage(WM_QUIT,0,0);
}

void CMainFrame::OnUdpHideserver()
{
	ShowWindow(SW_HIDE);
}

void CMainFrame::OnUdpShowserver()
{
	ShowWindow(SW_SHOW);
}

extern CUDP *UPar;

void CMainFrame::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==55)
	{
		for(unsigned i=0;i<MAX_CLIENTS;i++)
		{
			if(UPar[i].Free == FALSE) Client[i].PostThreadMessage(WM_TTIMER,0,0);
		}
	}
	CFrameWnd::OnTimer(nIDEvent);
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

void CMainFrame::OnFileSetserverspeed()
{
	m_speed.DoModal();	
}
