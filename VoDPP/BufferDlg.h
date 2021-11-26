#pragma once
#include "afxwin.h"

extern int	BufferFTP,BufferTCP,BufferUDP,cs;
// CBufferDlg dialog

class CBufferDlg : public CDialog
{
	DECLARE_DYNAMIC(CBufferDlg)

public:
	CBufferDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBufferDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_FTP;
	CEdit m_TCP;
	CEdit m_UDP;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CStatic m_Gran;
	CComboBox m_Combo;
};
