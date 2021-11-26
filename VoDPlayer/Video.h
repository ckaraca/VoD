#pragma once
//#define TSAT
#include "afxwin.h"
#include "explorer1.h"
#include "SplitterControl.h"
#include "Slider.h"
#include "Sql.h"
#include <dshow.h>
#include "NewFtp.h"
#include "FtpDown.h"
#include "Delete.h"
#include "FtpV.h"
#include "UDP.h"
#include "RUDP.h"
#include "afxcmn.h"
#include "Stats.h"


extern CStats *_Stats;
extern BOOL Stats;


extern CStats *_Stats;

typedef struct _PAGES {
	int PageID;
	TCHAR PageName[100];
	int Frame;
	BOOL PageSeenBefore;
} PAGES;

typedef struct _uPAGES {
	int PageID;
	TCHAR PageName[MAX_PATH];
	int Frame;
	BOOL PageSeenBefore;
} uPAGES;
typedef struct _uPAGESDAT { 
		int PageCount;
		uPAGES Pages[400];
	} uPAGESDAT;

// CVideo dialog
const int TICKLEN=100, TIMERID=55;
class CVideo : public CDialog
{
	DECLARE_EASYSIZE
	DECLARE_DYNAMIC(CVideo)
	
public:
	CVideo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideo();
	void Error(LPSTR lpszFunction);
	void Refresh(void);

// Dialog Data
	enum { IDD = IDD_VIDEO };

protected:
	DWORD SetBitmap(int nBitmap, COLORREF crTransColor);
	typedef struct _PAGESDAT { 
		int SliderRange;
		int PageCount;
		PAGES Pages[200];
		TCHAR Path[MAX_PATH];
		__int64 grTotalTime;
	} PAGESDAT;
	HMENU hMenu,hSubMenu;
	CFile Pref,uPref;
	BOOL Cont;
	PAGES Pages[200];
	uPAGES uPages[200];
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CSplitterControl m_Splitter;
	void DoResize(int delta);
	DECLARE_MESSAGE_MAP()
	RECT WindowRect;
public:
	unsigned Play;
	HICON m_hIcon;
	UINT_PTR g_wTimerID;
	virtual BOOL OnInitDialog();
	RECT rc;
	CRect Rect;
	BOOL FullScreen;
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	CExplorer1 m_explorer;
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	CStatic m_Screen;
	LONG SetSlider(WPARAM wParam,LPARAM lParam);
	LONG SetProgress(WPARAM wParam,LPARAM lParam);
	LONG StartPlay(WPARAM wParam,LPARAM lParam);
	void OnPlay();
	void OnStop();
	int m_pos;
protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CSlider m_Slider;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	UINT PageCount,uPageCount;
	// directx declerations
	HRESULT PrepareMedia(LPTSTR lpszMovie);
	void CenterVideo(void);
	BOOL IsWindowsMediaFile(LPTSTR lpszFile);
	HRESULT RenderWMFile(LPCWSTR wFile);
	HRESULT RenderAVIFile(LPCWSTR wFile);	
	HRESULT CreateFilter(REFCLSID clsid, IBaseFilter **ppFilter);
	HRESULT RenderOutputPins(IGraphBuilder *pGB, IBaseFilter *pReader);
	void ReadMediaPosition(void);
	void StartSeekTimer(void);
	void UpdatePosition(REFERENCE_TIME rtNow);
	void ConfigureSeekbar(void);
	void SetPlay(int Play);
	REFERENCE_TIME g_rtTotalTime;

protected:
	// time stats
#ifdef TSAT
	DWORD Start;
	CFile tfile;
	void GetTime(TCHAR *function);
#endif
	// 
	HRESULT InitDirectShow(void);
	LOGFONT lf;
	TEXTMETRIC tm;
	HFONT hfnt;
	SIZE size;
	void StopSeekTimer(void);
	BOOL Focus;
public:
	HRESULT FreeDirectShow(void);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	CFtpV *pFtp;
	CUDP *pUdp;
	CRUDP *pRudp;
public:
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//CProgressCtrl m_Progress;
	int m_TotalTime;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnRatKeepaspectratio();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRatDoublesize();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
