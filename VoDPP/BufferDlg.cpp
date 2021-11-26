// BufferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VoDPlayer.h"
#include "BufferDlg.h"


// CBufferDlg dialog

IMPLEMENT_DYNAMIC(CBufferDlg, CDialog)
CBufferDlg::CBufferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBufferDlg::IDD, pParent)
{
}

CBufferDlg::~CBufferDlg()
{
}

void CBufferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_FTP);
	DDX_Control(pDX, IDC_EDIT2, m_TCP);
	DDX_Control(pDX, IDC_EDIT3, m_UDP);
	DDX_Control(pDX, IDC_GRAN, m_Gran);
	DDX_Control(pDX, IDC_COMBO4, m_Combo);
}


BEGIN_MESSAGE_MAP(CBufferDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()



void CBufferDlg::OnBnClickedOk()
{
	BufferFTP = GetDlgItemInt(IDC_EDIT1);
	BufferTCP = GetDlgItemInt(IDC_EDIT2);
	BufferUDP = GetDlgItemInt(IDC_EDIT3);
	cs = m_Combo.GetCurSel();
	if(BufferFTP>131072) BufferFTP=131072;
	if(BufferTCP>131072) BufferTCP=131072;
	if(BufferUDP>131072) BufferUDP=131072;
	AfxGetApp()->WriteProfileInt("Buffer","BUFFERFTP",BufferFTP);
	AfxGetApp()->WriteProfileInt("Buffer","BUFFERTCP",BufferTCP);
	AfxGetApp()->WriteProfileInt("Buffer","BUFFERUDP",BufferUDP);
	AfxGetApp()->WriteProfileInt("Connection","CON_SP",cs);
	OnOK();
}

BOOL CBufferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemInt(IDC_EDIT1,BufferFTP);
	SetDlgItemInt(IDC_EDIT2,BufferTCP);
	SetDlgItemInt(IDC_EDIT3,BufferUDP);
	SYSTEM_INFO sy;
	GetSystemInfo(&sy);
	TCHAR text[100];
    m_Combo.AddString("10BASE-T  - 10 Mbps");
	m_Combo.AddString("100BASE-T - 100 Mbps");
	m_Combo.AddString("T1		 - 1.544 Mbps");
	m_Combo.AddString("T3        - 45 Mbps");
	m_Combo.AddString("ISDN      - 64 kbps");
	m_Combo.AddString("ISDN      - 128 kbps");
	m_Combo.AddString("Dialup    - 56 kbps");
	m_Combo.SetCurSel(cs);
	
	wsprintf(text,"%lu Bytes",sy.dwAllocationGranularity);
	m_Gran.SetWindowText(text);

	return TRUE;  // return TRUE unless you set the focus to a control
}

