// VoDPlayerDlg.h : header file
//

#pragma once
#include "BtnSt.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "Login.h"
#include "Project.h"
#include "Video.h"
#include "NewFtp.h"
#include "Stats.h"
#include "Drag.h"

extern CStats *_Stats;

extern PROJECTDB *Pr;
extern CSql Log;
// CVoDPlayerDlg dialog
class CVoDPlayerDlg : public CDialog
{
// Construction
public:
	CVoDPlayerDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CVoDPlayerDlg();
	LONG SetPr(WPARAM wParam, LPARAM lParam);
	void Shape(CDC &bmapDC,BITMAP &btmap,HRGN &retcombine);
	MTS *mts;

	
	BOOL ProjectOpen; // Flag to set or gather if any project is open or not
// Dialog Data
	enum { IDD = IDD_VODPLAYER_DIALOG };
	void CVoDPlayerDlg::CloseProject(void);
protected:
	int OpenWindows(void);
	int GetProjectFromDB(TCHAR *PrID);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	//CButtonST m_Close;
	HICON m_hIcon;
	CBitmap bitmap;
	BITMAP bmap;
	HRGN combine;
	CDC CcD;
	RECT rc;
	COLORREF trans;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void ExtractResource(TCHAR *path,int DATA);
	void Extract(void);
	DECLARE_MESSAGE_MAP()
public:
	CDrag *Drag;
	afx_msg UINT OnNcHitTest(CPoint point);
	CButtonST m_Play;
	CButtonST m_Stop;
	CButtonST m_Open;
	CButtonST m_Info;
	afx_msg void OnPopupExit();
	afx_msg void OnOpen();
	afx_msg void OnDisconnect();
	afx_msg void OnAbout();
	CVideo *Video;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// -------------------- Variables that should be initialized or deleted after close or disconnect
protected:
	
	TCHAR PrID[20];
public:
	afx_msg void OnConfigureFtpsite();
	afx_msg void OnPopupStatistics();
	afx_msg void OnConnectionprotocolUdp();
	afx_msg void OnConnectionprotocolRawftp();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnConnectionprotocolTcp();
	afx_msg void OnConfigureBuffersize();
	afx_msg void OnConnectionprotocolRudp();
};
