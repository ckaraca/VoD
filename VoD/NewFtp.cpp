// NewFtp.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "NewFtp.h"
#include "MainFrm.h"

// CNewFtp dialog

IMPLEMENT_DYNAMIC(CNewFtp, CDialog)
CNewFtp::CNewFtp(CWnd* pParent /*=NULL*/)
	: CDialog(CNewFtp::IDD, pParent)
{
}

CNewFtp::~CNewFtp()
{
}

void CNewFtp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_IP);
	DDX_Control(pDX, IDC_EDIT2, m_user);
	DDX_Control(pDX, IDC_EDIT6, m_Pass);
	DDX_Control(pDX, IDC_EDIT7, m_Port);
}


BEGIN_MESSAGE_MAP(CNewFtp, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CNewFtp message handlers


void CNewFtp::OnBnClickedOk()
{
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	TCHAR Host[220],Port[10],User[20],Pass[20],sz[1024],szError[512];
	int err;
	m_IP.GetWindowText(Host,sizeof(Host));
	m_Port.GetWindowText(Port,sizeof(Port));
	m_Pass.GetWindowText(Pass,sizeof(Pass));
	m_user.GetWindowText(User,sizeof(User));
	sprintf(sz,"INSERT INTO `ftp` ( `id` , `Host`, `Port`, `User`, `Pass` ) VALUES ('', '%s', '%s', '%s', '%s');",Host,Port,User,Pass);
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		MessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
		return ;
	}
	OnOK();
}
