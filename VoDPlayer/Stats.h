#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxmt.h"
#include "BtnST.h"
#include "HistogramCtrl.h"
// CStats dialog

class CStats : public CDialog
{
	DECLARE_DYNAMIC(CStats)

public:
	CStats(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStats();

// Dialog Data
	enum { IDD = IDD_STATS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	CStatic m_gb[8];
	CStatic m_[8];
	LONG Closed(WPARAM wParam,LPARAM lParam);
	void SetGrText(UINT index, TCHAR * text);
	void SetText(UINT index, TCHAR * text);
	LONG SetText(WPARAM wParam, LPARAM lParam);
	LONG SetHist(WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnCancel();
public:
	CButtonST m_rec[8];
	afx_msg void OnRec1();
	afx_msg void OnRec2();
	afx_msg void OnRec3();
	afx_msg void OnRec4();
	afx_msg void OnRec5();
	afx_msg void OnRec6();
	afx_msg void OnRec7();
	afx_msg void OnRec8();
private:
	BOOL m_Rec[8];
	CStdioFile m_File[8];
	BOOL m_FileS[8];
protected:
	CHistogramCtrl m_Hc;
	int m_cs;
	
};
