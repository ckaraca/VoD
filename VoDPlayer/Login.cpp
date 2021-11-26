// Login.cpp : implementation file
//

#include "stdafx.h"
#include "VoDPlayer.h"
#include "Login.h"


// CLogin dialog
extern BOOL Logged;
IMPLEMENT_DYNAMIC(CLogin, CDialog)
CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin::IDD, pParent)
{
	//Auth = new AUTH;
	static const char *p1= "a";
	__asm
	{
		NOP
		NOP
	}
	static const char *p2= "nk";
	__asm
	{
		NOP
		NOP
	}
	static const char *p3= "ra";
		__asm
	{
		NOP
		NOP
	}
	5+5;
	wsprintf(pass,"%s%s%s%s",p1,p2,p1,p3);
	__asm
	{
		NOP
		NOP
	}

	
}

CLogin::~CLogin()
{
	
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMATE1, m_Animate);
	DDX_Control(pDX, 1003, m_Login);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP);
	DDX_Control(pDX, IDC_EDIT1, m_User);
	DDX_Control(pDX, IDC_EDIT2, m_Pass);
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
	ON_BN_CLICKED(1003, OnLogin)
END_MESSAGE_MAP()


// CLogin message handlers
extern CVoDPlayerApp* Main;

BOOL CLogin::OnInitDialog()
{
	CDialog::OnInitDialog();
	AUTH *Auth;
	UINT n;
	// BufferFTP = this->GetProfileInt("Buffer","BUFFERFTP",16384);
	BOOL iptest = Main->GetProfileBinary("User","AUTH",(LPBYTE*)&Auth, &n);
	
	ASSERT(iptest);
	if(iptest==0) {
	Auth = new AUTH;
	Auth->IP1 =0;Auth->IP2=0;Auth->IP3=0;Auth->IP4=0;
	wsprintf(Auth->UN,"name");
	wsprintf(Auth->UP,"name");
	}
	m_Login.SetIcon(IDI_LOGIN);
	m_Animate.Open(IDR_AVI1);
	try {
        m_IP.SetAddress(Auth->IP1,Auth->IP2,Auth->IP3,Auth->IP4);
		m_User.SetWindowText(Auth->UN);
		m_Pass.SetWindowText(Auth->UP);
	} catch (...)
	{
		//m_IP.SetAddress(0,0,0,0);
		m_User.SetWindowText("Enter a user name");
		m_Pass.SetWindowText("***");
	}
	delete [] Auth;
	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	m_Login.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int CLogin::Resp(TCHAR *text,UINT Type)
{
	int res=-1;
	res = AfxMessageBox(text,Type);
	return (res);
	//return 0;
}

void CLogin::OnLogin()
{
	int res =-1;
	TCHAR Response[512];
	TCHAR sz[512];
	m_Animate.Play( 0, -1,-1 );
	m_Login.EnableWindow(FALSE);
	m_User.EnableWindow(FALSE);
	m_Pass.EnableWindow(FALSE);
	m_IP.EnableWindow(FALSE);
	BYTE IP1,IP2,IP3,IP4;
	m_User.GetWindowText(Log.User.user);
	m_Pass.GetWindowText(Log.User.pass);
	m_IP.GetAddress(IP1,IP2,IP3,IP4);
	Log.User.m_IP.Empty();
	Log.User.m_IP.Format("%d.%d.%d.%d",IP1,IP2,IP3,IP4);
	int err;
	if ((Log.myData = mysql_init((MYSQL*) 0)) &&  mysql_real_connect( Log.myData,
		Log.User.m_IP, "control", pass, NULL, MYSQL_PORT,  NULL, 0  ) )
	{
		if ( mysql_select_db( Log.myData, "vod" ) < 0 )
		{
			wsprintf(Response, "error: %s",mysql_error((MYSQL*)Log.myData) ) ;
			Log.Connected=FALSE;
			res = Resp(Response,MB_OK);
			EndDialog(res);
			return;
		}
		else 
		{ 
			wsprintf(sz,"Select * from vod.user where user='%s' AND pass=PASSWORD('%s')",Log.User.user,Log.User.pass);
			err = mysql_real_query(Log.myData,sz,sizeof(sz));
			if(err!=0)
			{
				wsprintf(Response,"No users in DB!");
				Log.Connected=FALSE;
				res = Resp(Response,MB_OK);
				EndDialog(res);
				return;
			}
			Log.res = mysql_store_result( Log.myData );
			int i = (int) mysql_num_rows( Log.res );
			if(i==0)
			{
				wsprintf(Response,"You are not allowed to access VoD DB");
				Log.Connected=FALSE;
				res = Resp(Response,MB_RETRYCANCEL);
				EndDialog(res);
				return;
			}
			wsprintf(Response, "Connected as %s",Log.User.user) ;
			mysql_free_result( Log.res ) ;
			Log.Connected=TRUE;
			AUTH Ax;// = new AUTH;
			Ax.IP1 = IP1;
			Ax.IP2 = IP2;
			Ax.IP3 = IP3;
			Ax.IP4 = IP4;
			wsprintf(Ax.UN,"%s",Log.User.user);
			wsprintf(Ax.UP,"%s",Log.User.pass);
			//AfxMessageBox(Ax->UN,MB_OK);
			//WriteProfileBinary(pszKey, "ComplexData", (LPBYTE)&myData, sizeof(myData) );
			Main->WriteProfileBinary("User","AUTH",(LPBYTE)&Ax,sizeof(Ax));
			//delete [] Ax;

			



			EndDialog(0);
			return;
		}
	}
	else 
	{
		wsprintf(Response,"error: %s",mysql_error((MYSQL*)Log.myData));
		Log.Connected=FALSE;
		res = Resp(Response,MB_RETRYCANCEL);
		EndDialog(res);
		return;
	}
	return;
}

BOOL CLogin::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		if(pMsg->wParam == VK_RETURN)
		{
			OnLogin();
			return 0;
		}
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CLogin::OnOK()
{
	OnLogin();

	CDialog::OnOK();
}
