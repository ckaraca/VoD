#pragma once
#include "afxwin.h"
#include "AFXINET.H"
#include "afxcmn.h"
#include "resource.h"
#define WM_INCT	WM_USER+0x121
#define WM_INF WM_USER+0x122
#define WM_END WM_USER+0x123
#define WM_CANCEL1 WM_USER+0x124


// CFtpDown dialog
UINT Download(LPVOID pParam);
extern int Numof1,Numof2;

typedef struct _MTS1 {
	HWND hWnd;
	BOOL Event;
	BOOL *Cancel1;
	TCHAR FileName[MAX_PATH];
    CFtpConnection* m_pFtpConnection;
	LONGLONG m_FileLength;
} MTS1;

class CFtpDown : public CDialog
{
	DECLARE_DYNAMIC(CFtpDown)
	typedef struct _PAGES {
	int PageID;
	TCHAR PageName[100];
	int Frame;
	BOOL PageSeenBefore;
	} PAGES;
	typedef struct _PAGESDAT { int PageCount;PAGES Pages[100];} PAGESDAT;

public:
	DWORD Start;
	LONG inf(WPARAM wParam, LPARAM /*lParam*/);
	LONG End(WPARAM wParam, LPARAM);
	BOOL m_Event;
	LONG OnInc(WPARAM wParam, LPARAM lParam);
	LONG OnCan(WPARAM /*wParam*/, LPARAM /*lParam*/);
	CFtpDown(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFtpDown();
	//UINT NumFiles,*Num;

// Dialog Data
	enum { IDD = IDD_FTPDOWN };

protected:
	TCHAR Host[220],Port[10],User[20],Pass[20];
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void Connect(void);
	void Warn(TCHAR *TextToWarn); // Type the text that you want it to appear in the list box
	int nIndex;
	double pos;
	LONGLONG FileLength,m_FileLength;
	CString m_FileName;
	MTS1 *mts;
	CInternetSession* m_pInetSession;
	CFtpConnection* m_pFtpConnection;
	LONGLONG Timer;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_List;
	BOOL Cancel1;
public:
	CProgressCtrl m_Progress;
	afx_msg void OnBnClickedCancel();
};
