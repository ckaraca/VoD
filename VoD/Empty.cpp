// Empty.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "Empty.h"


// Empty

IMPLEMENT_DYNCREATE(Empty, CFormView)

Empty::Empty()
	: CFormView(Empty::IDD)
{
}

Empty::~Empty()
{
}

void Empty::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Empty, CFormView)
END_MESSAGE_MAP()


// Empty diagnostics

#ifdef _DEBUG
void Empty::AssertValid() const
{
	CFormView::AssertValid();
}

void Empty::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// Empty message handlers

void Empty::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	ResizeParentToFit(FALSE);
}



