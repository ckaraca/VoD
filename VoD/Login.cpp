// Login.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "Login.h"
#include "MainFrm.h"

// Login

IMPLEMENT_DYNCREATE(Login, CFormView)

Login::Login()
	: CFormView(Login::IDD)
{
	ptp = new THREADPARMS;
	ptp->Sql = &Log;
	Log.Connected = FALSE;

}

Login::~Login()
{
	delete ptp;
}

LONG Login::OnThreadFinished (WPARAM wParam, LPARAM /*lParam*/)
{
	m_AVoD.Play( 0, -1,-1 );
	m_AVoD.Stop();
	TCHAR *Response = (TCHAR*)wParam;
	CMainFrame * pMainframe = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	pMainframe->m_wndStatusBar.SetPaneText(0,Response,TRUE);
	if(Log.Connected) // blah blah
	{
		pMainframe->SetSql(Log);
		AfxGetMainWnd ()->PostMessage(WM_SWITCH,0,0);
	}
	else 
	{
		m_login.EnableWindow(TRUE);
		m_UserName.EnableWindow(TRUE);
		m_PassWord.EnableWindow(TRUE);
		m_IP.EnableWindow(TRUE);
	}
	delete Response;
	return 0;
}

void Login::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_login);
	DDX_Control(pDX, IDC_EDIT1, m_UserName);
	DDX_Control(pDX, IDC_EDIT2, m_PassWord);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP);
	DDX_Control(pDX, IDC_ANIMATE1, m_AVoD);
}

BEGIN_MESSAGE_MAP(Login, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_MESSAGE (WM_USER_THREAD_FINISHED, OnThreadFinished)
END_MESSAGE_MAP()


// Login diagnostics

#ifdef _DEBUG
void Login::AssertValid() const
{
	CFormView::AssertValid();
}

void Login::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


void Login::OnInitialUpdate()
{
	short	shBtnColor = 30;
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
	AUTH *Auth;
	UINT n;
	// BufferFTP = this->GetProfileInt("Buffer","BUFFERFTP",16384);
	
	BOOL iptest = AfxGetApp()->GetProfileBinary("User","AUTH",(LPBYTE*)&Auth, &n);
	
	if(n) {
        m_IP.SetAddress(Auth->IP1,Auth->IP2,Auth->IP3,Auth->IP4);
		m_UserName.SetWindowText(Auth->UN);
		m_PassWord.SetWindowText(Auth->UP);
	} else
	{
		//m_IP.SetAddress(0,0,0,0);
		m_UserName.SetWindowText("Enter a user name");
		m_PassWord.SetWindowText("***");
	}
	delete [] Auth;

	//m_IP.SetAddress(213,208,49,37);
	//m_UserName.SetWindowText("cem");
	//m_PassWord.SetWindowText("ankara");
	m_login.SetIcon(IDI_ICON1);
	m_login.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_UserName.SetLimitText(16);
	m_PassWord.SetLimitText(16);
	m_AVoD.Open(IDR_AVI1);
}

void Login::OnBnClickedButton1()
{
	m_AVoD.Play( 0, -1,-1 );
	m_login.EnableWindow(FALSE);
	m_UserName.EnableWindow(FALSE);
	m_PassWord.EnableWindow(FALSE);
	m_IP.EnableWindow(FALSE);
	BYTE IP1,IP2,IP3,IP4;
	m_UserName.GetWindowText(Log.User.user);
	m_PassWord.GetWindowText(Log.User.pass);
	m_IP.GetAddress(IP1,IP2,IP3,IP4);
	Log.User.m_IP.Empty();
	Log.User.m_IP.Format("%d.%d.%d.%d",IP1,IP2,IP3,IP4);
	ptp->hWnd = m_hWnd;
	AUTH Ax;// = new AUTH;
			Ax.IP1 = IP1;
			Ax.IP2 = IP2;
			Ax.IP3 = IP3;
			Ax.IP4 = IP4;
			wsprintf(Ax.UN,"%s",Log.User.user);
			wsprintf(Ax.UP,"%s",Log.User.pass);
			//AfxMessageBox(Ax->UN,MB_OK);
			//WriteProfileBinary(pszKey, "ComplexData", (LPBYTE)&myData, sizeof(myData) );
			AfxGetApp()->WriteProfileBinary("User","AUTH",(LPBYTE)&Ax,sizeof(Ax));
			//delete [] Ax;



	AfxBeginThread(MySqlConnect,ptp);
}


UINT MySqlConnect(LPVOID pParam)
{
	THREADPARMS* ptp1 = (THREADPARMS*) pParam;
	HWND hWnd = ptp1->hWnd;
	CSql *tSql = ptp1->Sql;
	TCHAR *Response = new TCHAR[255];
	TCHAR sz[512];
	int err;
	//BYTE IP1,IP2,IP3,IP4;

	if ((tSql->myData = mysql_init((MYSQL*) 0)) &&  mysql_real_connect( tSql->myData,
		tSql->User.m_IP, "control", "ankara", NULL, MYSQL_PORT,  NULL, 0  ) )
	{
		if ( mysql_select_db( tSql->myData, "vod" ) < 0 )
		{
			wsprintf(Response, "error: %s",mysql_error((MYSQL*)tSql->myData) ) ;
			tSql->Connected=FALSE;
			::PostMessage (hWnd, WM_USER_THREAD_FINISHED, (WPARAM)Response, 0);
			return 1;
		}
		else 
		{ 
			wsprintf(sz,"Select * from vod.user where user='%s' AND pass=PASSWORD('%s')",tSql->User.user,tSql->User.pass);
			err = mysql_real_query(tSql->myData,sz,sizeof(sz));
			if(err!=0)
			{
				wsprintf(Response,"No users in DB!");
				tSql->Connected=FALSE;
				::PostMessage (hWnd, WM_USER_THREAD_FINISHED, (WPARAM)Response, 0);
				return 1;
			}
			tSql->res = mysql_store_result( tSql->myData );
			int i = (int) mysql_num_rows( tSql->res );
			if(i==0)
			{
				wsprintf(Response,"You are not allowed to access VoD DB");
				tSql->Connected=FALSE;
				::PostMessage (hWnd, WM_USER_THREAD_FINISHED, (WPARAM)Response, 0);
				return 1;
			}
			wsprintf(Response, "Connected as %s",tSql->User.user) ;
			mysql_free_result( tSql->res ) ;
			tSql->Connected=TRUE;			
			::PostMessage (hWnd, WM_USER_THREAD_FINISHED, (WPARAM)Response, 0);
		}
	}
	else 
	{
		wsprintf(Response,"error: %s",mysql_error((MYSQL*)tSql->myData));
		tSql->Connected=FALSE;
		::PostMessage (hWnd, WM_USER_THREAD_FINISHED, (WPARAM)Response, 0);
	}
	return 0;
}

BOOL Login::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void Login::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	
}
