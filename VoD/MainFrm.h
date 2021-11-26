// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "WndSplitterEx.h"
#include "Sql.h"
#include "Html.h"
#include "Ftpd.h"
#include "Empty.h"
#include "Delete.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
// Attributes
public:

// Operations
public:
	CStatusBar  m_wndStatusBar;
	CSql m_Sql;
	TCHAR szPath[MAX_PATH];
	TCHAR szTempPath[MAX_PATH];
	CString m_File;
	CString Source;
	HKEY hRegKey;
	unsigned long datasize,result;
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void SetSql(CSql refSql);
	void SetFile(TCHAR *File);
	void SetFileManually(TCHAR *File);
	void ExtractResource(TCHAR *path,int DATA);
	CString GetFile();
	void SetSource(CString Source);
	CString* GetSource();
	void Say(TCHAR *Text);
// Implementation
public:
	virtual ~CMainFrame();
	CWndSplitterEx m_wndSplitter;
	CToolBar    m_wndFormatBar;
	CMenu m_hMenu,m_hMenu1,m_hMenu2;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	
	TCHAR* ResourceToURL(LPCTSTR lpszURL);
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT Switch(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	// // ping DB
	void Ping(void);
	afx_msg void OnBuildUploadproject();
	afx_msg void OnBuildAddfileserver();
	afx_msg void OnFileCloseproject();
	afx_msg void OnFileOpen();
	virtual BOOL DestroyWindow();
};


