#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "BtnSt.h"
#include "Sql.h"
// CLogin dialog

extern CSql Log;
class CLogin : public CDialog
{
	DECLARE_DYNAMIC(CLogin)
	//

public:
	CLogin(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogin();

// Dialog Data
	enum { IDD = IDD_LOGIN };

protected:
	char pass[10];
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int Resp(TCHAR *text,UINT Type);
	DECLARE_MESSAGE_MAP()
public:
	CAnimateCtrl m_Animate;
	CButtonST m_Login;
	CIPAddressCtrl m_IP;
	CEdit m_User;
	CEdit m_Pass;
	virtual BOOL OnInitDialog();
	afx_msg void OnLogin();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();
	typedef struct _Auth //AUTH Authentication struct to memorize login settings;
	{
		BYTE	IP1;	// 1st field of IP box
		BYTE	IP2;	// 2nd field of IP box
		BYTE	IP3;	// 3rd field of IP box
		BYTE	IP4;	// 4th field of IP box
		TCHAR	UN[40];	// User Name
		TCHAR	UP[20]; // User Password
	} AUTH;
};
