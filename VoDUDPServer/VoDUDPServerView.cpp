// VoDUDPServerView.cpp : implementation of the CVoDUDPServerView class
//

#include "stdafx.h"
#include "VoDUDPServer.h"
#include "MainFrm.h"

#include "VoDUDPServerDoc.h"
#include "VoDUDPServerView.h"
#include "UDPListener.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern HWND hWnd;
// CVoDUDPServerView

IMPLEMENT_DYNCREATE(CVoDUDPServerView, CListView)

BEGIN_MESSAGE_MAP(CVoDUDPServerView, CListView)
	ON_MESSAGE(WM_SETLIST,SetListEx)
	ON_MESSAGE(WM_SETLISTS,SetListSuccessEx)
//	ON_WM_TIMER()
END_MESSAGE_MAP()

// CVoDUDPServerView construction/destruction

CVoDUDPServerView::CVoDUDPServerView()
{
	index = 0;

}

CVoDUDPServerView::~CVoDUDPServerView()
{
}

BOOL CVoDUDPServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~LVS_TYPEMASK;
    cs.style |= LVS_REPORT;
	//cs.style |= LVS_EX_GRIDLINES;
	//cs.style |=LVS_EX_FULLROWSELECT;

	return CListView::PreCreateWindow(cs);
}

void CVoDUDPServerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	RECT rc;
	GetClientRect(&rc);
	GetListCtrl ().InsertColumn (0, _T ("Status ID"), LVCFMT_LEFT, 100);
	GetListCtrl ().InsertColumn (1, _T ("Event"), LVCFMT_LEFT, 300);
	GetListCtrl ().InsertColumn (2, _T ("Success"), LVCFMT_LEFT,rc.right-400);
	// begin adding here
	
	SetList("002","Starting Windows sockets");
	SetListSuccess("OK!");
	SetList("003","Starting UDP Listener thread");
    //Listen = new CTCPListener;
	hWnd = this->m_hWnd;
	Udp = NULL;
	Udp = AfxBeginThread(UDPListener,(LPVOID)0);
	if(Udp!=NULL)
	{
		CMainFrame *pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
		pM->SetTimer(55,10000,NULL);
	}
	SetListSuccess("OK!");
}
void CVoDUDPServerView::SetList(TCHAR *Code,TCHAR *Event)
{
	GetListCtrl ().InsertItem (index,Code);
	GetListCtrl ().SetItemText (index, 1, Event);
}

void CVoDUDPServerView::SetListSuccess(TCHAR *Success)
{
	GetListCtrl ().SetItemText (index++, 2, Success);
}
LONG CVoDUDPServerView::SetListEx(WPARAM wParam,LPARAM lParam)
{
	SetList((TCHAR *)wParam,(TCHAR *)lParam);
	return 0;
}

LONG CVoDUDPServerView::SetListSuccessEx(WPARAM wParam,LPARAM /*lParam*/)
{
	SetListSuccess((TCHAR *)wParam);
	return 0;
}


// CVoDUDPServerView diagnostics

#ifdef _DEBUG
void CVoDUDPServerView::AssertValid() const
{
	CListView::AssertValid();
}

void CVoDUDPServerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CVoDUDPServerDoc* CVoDUDPServerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVoDUDPServerDoc)));
	return (CVoDUDPServerDoc*)m_pDocument;
}
#endif //_DEBUG

