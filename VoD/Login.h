#pragma once
#include "afxwin.h"
#include "BtnST.h"
#include "afxcmn.h"
#include "Sql.h"

// Login form view
UINT MySqlConnect(LPVOID pParam);

class Login : public CFormView
{
	DECLARE_DYNCREATE(Login)

protected:
	Login();           // protected constructor used by dynamic creation
	virtual ~Login();

public:
	enum { IDD = IDD_LOGIN };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LONG OnThreadFinished (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CButtonST m_login;
	afx_msg void OnBnClickedButton1();
	CEdit m_UserName;
	CEdit m_PassWord;
	CIPAddressCtrl m_IP;
	CAnimateCtrl m_AVoD;
	THREADPARMS* ptp;
	CSql	Log;
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
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


