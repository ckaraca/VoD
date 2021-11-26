#pragma once
#include "afxcmn.h"


// CDelete dialog

class CDelete : public CDialog
{
	DECLARE_DYNAMIC(CDelete)

private:
	TCHAR Temp[MAX_PATH];
	TCHAR Files[200][MAX_PATH];
	UINT counter;
	BOOL DirPresent;
public:
	CDelete(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDelete();
	void DeleteFiles(void);
// Dialog Data
	enum { IDD = IDD_DELETE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_Progress;
	virtual BOOL OnInitDialog();
};
