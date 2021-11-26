// TabManage.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "TabManage.h"

// CTabManage

IMPLEMENT_DYNCREATE(CTabManage, CTabPage)

CTabManage::CTabManage()
	: CTabPage(CTabManage::IDD)
{
	
}

CTabManage::~CTabManage()
{
}

void CTabManage::DoDataExchange(CDataExchange* pDX)
{
	CTabPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_BrButton);
	DDX_Control(pDX, IDC_EDIT7, m_Path);
	DDX_Control(pDX, IDC_EDIT1, m_Name);
	DDX_Control(pDX, IDC_EDIT2, m_Subject);
	DDX_Control(pDX, IDC_EDIT6, m_Desc);
}

BEGIN_MESSAGE_MAP(CTabManage, CTabPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnBrowse)
END_MESSAGE_MAP()


// CTabManage diagnostics

#ifdef _DEBUG
void CTabManage::AssertValid() const
{
	CTabPage::AssertValid();
}

void CTabManage::Dump(CDumpContext& dc) const
{
	CTabPage::Dump(dc);
}
#endif //_DEBUG


// CTabManage message handlers

void CTabManage::OnBrowse()
{
	// TODO: Add your control notification handler code here
}
