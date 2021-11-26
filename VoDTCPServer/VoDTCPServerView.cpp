// VoDTCPServerView.cpp : implementation of the CVoDTCPServerView class
//

#include "stdafx.h"
#include "VoDTCPServer.h"

#include "VoDTCPServerDoc.h"
#include "VoDTCPServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WSA_VERSION  MAKEWORD(2,0)

extern TCHAR ServerPathName[];
extern TCHAR ServerPath[];

// CVoDTCPServerView

IMPLEMENT_DYNCREATE(CVoDTCPServerView, CListView)

BEGIN_MESSAGE_MAP(CVoDTCPServerView, CListView)
	ON_MESSAGE(WM_SETLIST,SetListEx)
	ON_MESSAGE(WM_SETLISTS,SetListSuccessEx)
END_MESSAGE_MAP()

// CVoDTCPServerView construction/destruction
extern HWND hWnd;
CVoDTCPServerView::CVoDTCPServerView()
{
	index = 0;

}

CVoDTCPServerView::~CVoDTCPServerView()
{
}

BOOL CVoDTCPServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~LVS_TYPEMASK;
    cs.style |= LVS_REPORT;


	return CListView::PreCreateWindow(cs);
}

void CVoDTCPServerView::OnInitialUpdate()
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
	SetList("003","Starting TCP Listener thread");
    //Listen = new CTCPListener;
	hWnd = this->m_hWnd;
	AfxBeginThread(TCPListener,(LPVOID)0);
	SetListSuccess("OK!");
}

void CVoDTCPServerView::SetList(TCHAR *Code,TCHAR *Event)
{
	GetListCtrl ().InsertItem (index,Code);
	GetListCtrl ().SetItemText (index, 1, Event);
}

void CVoDTCPServerView::SetListSuccess(TCHAR *Success)
{
	GetListCtrl ().SetItemText (index++, 2, Success);
}
LONG CVoDTCPServerView::SetListEx(WPARAM wParam,LPARAM lParam)
{
	SetList((TCHAR *)wParam,(TCHAR *)lParam);
	return 0;
}

LONG CVoDTCPServerView::SetListSuccessEx(WPARAM wParam,LPARAM /*lParam*/)
{
	SetListSuccess((TCHAR *)wParam);
	return 0;
}


// CVoDTCPServerView diagnostics

#ifdef _DEBUG
void CVoDTCPServerView::AssertValid() const
{
	CListView::AssertValid();
}

void CVoDTCPServerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CVoDTCPServerDoc* CVoDTCPServerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVoDTCPServerDoc)));
	return (CVoDTCPServerDoc*)m_pDocument;
}
#endif //_DEBUG


// CVoDTCPServerView message handlers
