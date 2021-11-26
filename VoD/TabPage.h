#pragma once


// TabPage dialog

class CTabPage : public CDialog
{
	DECLARE_DYNAMIC(CTabPage)

public:
	CTabPage();   // standard constructor
	CTabPage(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CTabPage();

// Dialog Data
	//enum { IDD = IDD_TABPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// Message Handlers
	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify (WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnOK (void);
	virtual void OnCancel (void);
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	DECLARE_MESSAGE_MAP()
public:
};
