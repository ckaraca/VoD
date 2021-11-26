// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "SysTray.h"
#include "Client.h"
#include "UDP.h"
#include "SSpeed.h"
extern unsigned ThQe;
extern CClient *Client;




class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CSysTray	m_SysTray;

// Generated message map functions
protected:
	LONG OnSysTray(WPARAM wParam,LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CSSpeed m_speed;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUdpShutdownvodtcpserver();
	afx_msg void OnUdpHideserver();
	afx_msg void OnUdpShowserver();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnFileSetserverspeed();
};


