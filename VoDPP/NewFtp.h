#pragma once
#include "afxwin.h"
#include "Sql.h"

// CNewFtp dialog

class CNewFtp : public CDialog
{
	DECLARE_DYNAMIC(CNewFtp)

public:
	CNewFtp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewFtp();

// Dialog Data
	enum { IDD = IDD_NEWFTP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CEdit m_IP;
	CEdit m_user;
	CEdit m_Pass;
	afx_msg void OnBnClickedOk();
	CEdit m_Port;
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
