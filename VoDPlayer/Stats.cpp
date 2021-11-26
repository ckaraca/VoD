// Stats.cpp : implementation file
//

#include "stdafx.h"
#include "VoDPlayer.h"
#include "Stats.h"
#include "VoDPlayerDlg.h"
#include "Video.h"

#include <process.h>

extern PROJECTDB *Pr;
extern CString Connection;
extern TCHAR TempPath[MAX_PATH];
// CStats dialog
extern int cs;		//Connection speed
IMPLEMENT_DYNAMIC(CStats, CDialog)
CStats::CStats(CWnd* pParent /*=NULL*/)
	: CDialog(CStats::IDD, pParent)
	,m_cs(0)
{
	for(unsigned i=0;i<8;i++)
	{
		m_Rec[i] = FALSE;
		m_FileS[i] = FALSE;
	}
	
}

CStats::~CStats()
{
	
}


void CStats::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_gb[0]);
	DDX_Control(pDX, IDC_STATIC2, m_gb[1]);
	DDX_Control(pDX, IDC_STATIC3, m_gb[2]);
	DDX_Control(pDX, IDC_STATIC4, m_gb[3]);
	DDX_Control(pDX, IDC_STATIC5, m_gb[4]);
	DDX_Control(pDX, IDC_STATIC6, m_gb[5]);
	DDX_Control(pDX, IDC_STATIC7, m_gb[6]);
	DDX_Control(pDX, IDC_STATIC8, m_gb[7]);
	DDX_Control(pDX, IDC_STATICa, m_[0]);
	DDX_Control(pDX, IDC_STATICb, m_[1]);
	DDX_Control(pDX, IDC_STATICc, m_[2]);
	DDX_Control(pDX, IDC_STATICd, m_[3]);
	DDX_Control(pDX, IDC_STATICe, m_[4]);
	DDX_Control(pDX, IDC_STATICf, m_[5]);
	DDX_Control(pDX, IDC_STATICg, m_[6]);
	DDX_Control(pDX, IDC_STATICh, m_[7]);	
	DDX_Control(pDX, IDC_BUTTON1, m_rec[0]);
	DDX_Control(pDX, IDC_BUTTON2, m_rec[1]);
	DDX_Control(pDX, IDC_BUTTON3, m_rec[2]);
	DDX_Control(pDX, IDC_BUTTON4, m_rec[3]);
	DDX_Control(pDX, IDC_BUTTON5, m_rec[4]);
	DDX_Control(pDX, IDC_BUTTON6, m_rec[5]);
	DDX_Control(pDX, IDC_BUTTON7, m_rec[6]);
	DDX_Control(pDX, IDC_BUTTON8, m_rec[7]);
}


BEGIN_MESSAGE_MAP(CStats, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnRec1)
	ON_BN_CLICKED(IDC_BUTTON2, OnRec2)
	ON_BN_CLICKED(IDC_BUTTON3, OnRec3)
	ON_BN_CLICKED(IDC_BUTTON4, OnRec4)
	ON_BN_CLICKED(IDC_BUTTON5, OnRec5)
	ON_BN_CLICKED(IDC_BUTTON6, OnRec6)
	ON_BN_CLICKED(IDC_BUTTON7, OnRec7)
	ON_BN_CLICKED(IDC_BUTTON8, OnRec8)
	ON_MESSAGE(WM_STATS,Closed)
	ON_MESSAGE(WM_SETSTATS, SetText)
	ON_MESSAGE(WM_SETHIST, SetHist)
END_MESSAGE_MAP()


// CStats message handlers

BOOL CStats::OnInitDialog()
{
	CDialog::OnInitDialog();
	//CVoDPlayerDlg * pM =  (CVoDPlayerDlg*)AfxGetApp()->m_pMainWnd;
	//CVideo *vd = pM->Video;
	//if(vd==NULL) return TRUE;
	//if(!IsWindow(vd->m_hWnd)) return TRUE;
	HWND hWnd = ::GetDesktopWindow();
	RECT rc;
	::GetWindowRect(hWnd,&rc);
	this->MoveWindow(rc.right-250,0,249,440);
	this->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	for(unsigned i=0;i<8;i++)
	{
		m_rec[i].SetIcon(IDI_LEDON,IDI_LEDOFF);
		m_gb[i].SetWindowText("N/A");
		m_[i].SetWindowText("No data");
	}
	CRect rect;
	GetDlgItem(IDC_STATIC_HIST)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_Hc.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect, this, 1000);
	
	m_Hc.SetSpeed(CHistogramCtrl::HIGH_SPEED);
	switch(cs)
	{
	case 0:
		m_cs = 1000;
		m_Hc.SetRange(1, m_cs);
		break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


LONG CStats::Closed(WPARAM wParam,LPARAM lParam)
{
	OnCancel();
	return 0;
}



void CStats::SetGrText(UINT index, TCHAR * text)
{	
	try{
		m_gb[index].SetWindowText(text);
	} catch(...) {}

}
LONG CStats::SetHist(WPARAM wParam, LPARAM lParam)
{
	double val = (double)wParam;
	val*=100;
	
	m_Hc.SetPos((UINT)val);
	return 0;
}

LONG CStats::SetText(WPARAM wParam, LPARAM lParam)
{
	UINT index; TCHAR * text;
	index = (UINT)wParam;
	text = (TCHAR*)lParam;
	try { 
		m_[index].SetWindowText(text); 
	} catch(...) {}
	if(m_Rec[index])
	{
		if(m_FileS[index]==FALSE)
		{
			m_FileS[index]=TRUE;
			CString name,n;
			m_gb[index].GetWindowText(n);
			name.Format("%s-%s-%s.txt",n,Pr->name,Connection);
			CFileException e;
			::SetCurrentDirectory(TempPath);
			if(!m_File[index].Open(name,CFile::modeReadWrite|CFile::modeCreate|CFile::shareDenyNone
				,&e))
			{
				e.ReportError();
				e.Delete();
			}
		}
		CString ttw;
		ttw.Empty();
		ttw.Format("%s\n",text);
		m_File[index].WriteString(ttw);
	}
	delete []text;
	return 0;
}

void CStats::SetText(UINT index, TCHAR * text)
{
	try { 
		m_[index].SetWindowText(text); 
	} catch(...) {}
	if(m_Rec[index])
	{
		if(m_FileS[index]==FALSE)
		{
			m_FileS[index]=TRUE;
			CString name,n;
			m_gb[index].GetWindowText(n);
			name.Format("%s-%s-%s.txt",n,Pr->name,Connection);
			CFileException e;
			::SetCurrentDirectory(TempPath);
			if(!m_File[index].Open(name,CFile::modeReadWrite|CFile::modeCreate|CFile::shareDenyNone
				,&e))
			{
				e.ReportError();
				e.Delete();
			}
		}
		CString ttw;
		ttw.Empty();
		ttw.Format("%s\n",text);
		m_File[index].WriteString(ttw);
	}
}

void CStats::OnCancel()
{
	for(unsigned i=0;i<8;i++)
	{
		if(m_FileS[i]==TRUE)
		{
			m_File[i].Close();
		}
	}
	CDialog::OnCancel();
}

void CStats::OnRec1()
{
	m_Rec[0] = TRUE;
}

void CStats::OnRec2()
{
	m_Rec[1] = TRUE;	
}
void CStats::OnRec3()
{
	m_Rec[2] = TRUE;	
}
void CStats::OnRec4()
{
	m_Rec[3] = TRUE;	
}
void CStats::OnRec5()
{
	m_Rec[4] = TRUE;
}
void CStats::OnRec6()
{
	m_Rec[5] = TRUE;
}
void CStats::OnRec7()
{
	m_Rec[6] = TRUE;
}
void CStats::OnRec8()
{
	m_Rec[7] = TRUE;
}

void CStats::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();
}
