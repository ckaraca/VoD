#pragma once
#include "afxwin.h"
#include "AFXINET.H"
#include "afxcmn.h"
#include "SADirRead.h"
#define WM_INC	WM_USER+0x110
#define WM_INC1	WM_USER+0x111
#define WM_CANCEL WM_USER+0x112
// CFtpd dialog
UINT SendViaFtp(LPVOID pParam);

typedef struct tagTHREAD {
	HWND hWnd;
	CFtpConnection* m_pFtpConnection;
	TCHAR VideoPath[MAX_PATH];
	TCHAR VideoName[MAX_PATH];
	BOOL *Cancel1;
} THREAD;

extern int canceled;
class CFtpd : public CDialog
{
	DECLARE_DYNAMIC(CFtpd)
	typedef struct _PAGES {
	int PageID;
	TCHAR PageName[100];
	int Frame;
	BOOL PageSeenBefore;
	} PAGES;
	typedef struct _PAGESDAT { 
		int SliderRange;
		int PageCount;
        PAGES Pages[200];
		TCHAR Path[MAX_PATH];
		__int64 TotalTime;
	} PAGESDAT;
	PAGESDAT Dat;
	typedef struct _tagFILES { TCHAR File[MAX_PATH]; } FILES;
public:
	TCHAR*AddS(TCHAR * sentence); // Adds slashes to " ' " as " \'" in order to avoid sql errors
	CFtpd(CWnd* pParent = NULL);   // standard constructor
	//LONG GetFile(WPARAM wParam,LPARAM lParam);
	LONG OnInc(WPARAM wParam, LPARAM /*lParam*/);
	LONG OnInc1(WPARAM wParam, LPARAM /*lParam*/);
	LONG OnCan(WPARAM /*wParam*/, LPARAM /*lParam*/);
	void Warn(TCHAR *TextToWarn);
	int SendStuff(TCHAR * LocalPath,TCHAR * RemoteName);
	TCHAR Path[MAX_PATH],VideoPath[MAX_PATH],VideoName[MAX_PATH];
	PAGES Pages[200];
	int PageCount;
	unsigned nIndex;
	TCHAR Host[220],Port[10],User[20],Pass[20];
	virtual ~CFtpd();
	THREAD* fts;
	BOOL Cancel1;
// Dialog Data
	enum { IDD = IDD_FTPUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CInternetSession* m_pInetSession;
	CFtpConnection* m_pFtpConnection;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk2();
	virtual BOOL OnInitDialog();
	CListBox m_List;
	CProgressCtrl m_Progress;
	afx_msg void OnBnClickedCancel();
	CButton m_Start;
	CButton m_Cancel;
protected:
	virtual void OnCancel();
public:
	virtual BOOL DestroyWindow();
};
