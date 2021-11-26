#pragma once
#include "afxwin.h"


// CSSpeed dialog

class CSSpeed : public CDialog
{
	DECLARE_DYNAMIC(CSSpeed)

public:
	CSSpeed(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSSpeed();

// Dialog Data
	enum { IDD = IDD_SSPEED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Combo;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
