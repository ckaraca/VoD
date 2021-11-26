// Source.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "Source.h"
#include "MainFrm.h"
#include "Design.h"
#include "Html.h"
// Source

IMPLEMENT_DYNCREATE(CSource, CEditView)

CSource::CSource()
{
}

CSource::~CSource()
{
}

BEGIN_MESSAGE_MAP(CSource, CEditView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
//	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
END_MESSAGE_MAP()


// Source diagnostics

#ifdef _DEBUG
void CSource::AssertValid() const
{
	CEditView::AssertValid();
}

void CSource::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG


// Source message handlers

int CSource::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CEdit &edit = GetEditCtrl();
	//try for courier, if that fails, go for the ANSI fixed font
	if(!m_font.CreateFont(-MulDiv(10, GetDeviceCaps(edit.GetDC()->m_hDC, LOGPIXELSY), 72)
			,0,0,0,FW_DONTCARE,0,0,0,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS
			,DEFAULT_QUALITY,DEFAULT_PITCH,_T("COURIER")))	
	{
		m_font.CreateStockObject(ANSI_FIXED_FONT);
	}
	edit.SetFont(&m_font);

	return 0;
}

void CSource::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if(bActivate && pActivateView==this && pDeactiveView != this)
	{
		//we're being activated get the HTML from the WebView
		UpdateView();
	}
	CEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CSource::UpdateView()
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pFrame);
	
	CString *strHTML;
	strHTML = pFrame->GetSource();
	SetWindowText(*strHTML);
}

void CSource::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pFrame);
	
	CString *strHTML;
	strHTML = pFrame->GetSource();
	SetWindowText(*strHTML);
}


void CSource::OnDestroy()
{
	CEditView::OnDestroy();
	
		
}

void CSource::OnEnChange()
{
	CFile File;
	CFileException fileException;
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if ( !File.Open( pFrame->GetFile(), CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone, &fileException ) )
	{
		TRACE( "Can't open file %s, error = %u\n",
			pFrame->GetFile(), fileException.m_cause );
		return;
	}
	CString Html1;
	GetWindowText(Html1);
	File.Write(Html1,Html1.GetLength());
	File.Close();
	pFrame->SetSource(Html1);
}
