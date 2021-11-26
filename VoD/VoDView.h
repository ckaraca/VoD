#pragma once

#include "Sql.h"
#include "afxwin.h"
#include "BtnST.h"
#include "afxcmn.h"

#include "TabCtrlEx.h"
#include "TabManage.h"
#include "TabProject.h"
#include "Ruler.h"
#include "Ftpd.h"
#include "NewFtp.h"
#include "FtpDown.h"
#include <ShFolder.h>

#include <dshow.h>

typedef struct _PAGES {
	int PageID;
	TCHAR PageName[100];
	int Frame;
	BOOL PageSeenBefore;
} PAGES;

const int TICKLEN=100, TIMERID=55;

class CVoDView : public CFormView
{
protected: // create from serialization only
	CVoDView();
	DECLARE_DYNCREATE(CVoDView)

public:
	enum{ IDD = IDD_VOD_FORM };
	MTS *mts;


// Operations
public:
	//CSql ActiveUser;

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	BOOL Paint;
	unsigned Play;
	void ResetDirectShow(void);
	void GoStartVideo(TCHAR *Video=NULL);
// Implementation
public:
	virtual ~CVoDView();
	HRESULT PrepareMedia(LPTSTR lpszMovie);
	void CenterVideo(void);
	BOOL IsWindowsMediaFile(LPTSTR lpszFile);
	HRESULT RenderWMFile(LPCWSTR wFile);
	HRESULT CreateFilter(REFCLSID clsid, IBaseFilter **ppFilter);
	HRESULT RenderOutputPins(IGraphBuilder *pGB, IBaseFilter *pReader);
	int CreateLink(TCHAR *FilePath, TCHAR *Description);
	void ReadMediaPosition(void);
	void StartSeekTimer(void);
	void UpdatePosition(REFERENCE_TIME rtNow);
	void ConfigureSeekbar(void);
	void SetPlay(int Play);
	int	AddPage(int Frame,TCHAR *Name,int type);
	int	AddPage1(int Frame,TCHAR *Name,int type);
	int SetPage(TCHAR *Name);
	int DeletePage(int PageID);
	int MovePage(int PageID);
	CString CreateGuid(void);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL Created;
	LONG CloseProject(WPARAM wParam=NULL, LPARAM lParam=NULL);
	void Warn(TCHAR *TextToWarn);
	int x,y;
	HRESULT InitDirectShow(void);
	HRESULT FreeDirectShow(void);

	
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	REFERENCE_TIME g_rtTotalTime;
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT ChangeTab(WPARAM wParam, LPARAM lParam);
	LONG OnBnClickedButton2Mes(WPARAM wParam,LPARAM lParam);
	CButtonST m_View;
	CButtonST m_Design;
	int m_State;
	CButtonST m_Code;
	afx_msg void OnBnClickedCode();
	// Tab view handler
	CTabCtrlEx	m_Tab;
	CTabManage	m_Manage;
	//CTabPref	m_Pref;
	CTabProject	m_Proj;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CAnimateCtrl m_AVoD;
	CButtonST m_Play;
	CButtonST m_Rec;
	CSliderCtrl m_Slider;
	CRuler m_Ruler;
	int m_nScrolPos;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnStop();
	TCHAR PrName[MAX_PATH],PrName1[MAX_PATH];
	PAGES Pages[200];
	UINT PageCount;
	TCHAR VideoPath[MAX_PATH];
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnDestroy();
	CStatic m_Screen;
//	virtual void OnInitialUpdate();
	afx_msg void OnPlay();
	afx_msg void OnSlide(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditindEditthisinformation();
	// ------------- edit
	CFile Pref;
	typedef struct _PAGESDAT {
		int SliderRange;
		int PageCount;
		PAGES Pages[200];
		TCHAR Path[MAX_PATH];
		__int64 TotalTime;
	} PAGESDAT;
	virtual BOOL DestroyWindow();
	// directx 
public:
	UINT_PTR g_wTimerID;
protected:
	void StopSeekTimer(void);

};

/*#ifndef _DEBUG  // debug version in VoDView.cpp
inline CVoDDoc* CVoDView::GetDocument() const
   { return reinterpret_cast<CVoDDoc*>(m_pDocument); }
#endif*/

