#pragma once
#include "btnst.h"
#include "afxwin.h"



// CDrag dialog

class CDrag : public CDialog
{
	DECLARE_DYNAMIC(CDrag)

public:
	CDrag(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDrag();

// Dialog Data
	enum { IDD = IDD_DIALOGBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	typedef struct _uPAGESDAT { 
		int PageCount;
		uPAGES Pages[400];
	} uPAGESDAT;
	uPAGES uPages[200];
	
public:
	UINT uPageCount;
	virtual BOOL OnInitDialog();
	int	AddPage(int Frame,TCHAR *Name);
	CButtonST m_drag;
	afx_msg void OnDragLink();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg UINT OnNcHitTest(CPoint point);
	virtual void OnOK();
};
