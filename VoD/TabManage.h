#pragma once
#include "TabPage.h"
#include "afxwin.h"


// CTabManage form view

class CTabManage : public CTabPage
{
	DECLARE_DYNCREATE(CTabManage)
	CTabManage();           // protected constructor used by dynamic creation
	virtual ~CTabManage();

public:
	enum { IDD = IDD_TABMANAGE };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_BrButton;
	CEdit m_Path;
	afx_msg void OnBrowse();
	CEdit m_Name;
	CEdit m_Subject;
	CEdit m_Desc;
};


