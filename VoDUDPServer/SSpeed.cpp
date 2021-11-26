// SSpeed.cpp : implementation file
//

#include "stdafx.h"
#include "VoDUDPServer.h"
#include "SSpeed.h"
extern int m_Ssp;

// CSSpeed dialog

IMPLEMENT_DYNAMIC(CSSpeed, CDialog)
CSSpeed::CSSpeed(CWnd* pParent /*=NULL*/)
	: CDialog(CSSpeed::IDD, pParent)
{
}

CSSpeed::~CSSpeed()
{
}

void CSSpeed::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CSSpeed, CDialog)
END_MESSAGE_MAP()


// CSSpeed message handlers

BOOL CSSpeed::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Combo.AddString("10BASE-T  - 10 Mbps");
	m_Combo.AddString("100BASE-T - 100 Mbps");
	m_Combo.AddString("ATM       - 135.6 Mbps");
	m_Combo.AddString("T1        - 1.544 Mbps");
	m_Combo.AddString("T3        - 45 Mbps");
	m_Combo.AddString("ISDN      - 64 kbps");
	m_Combo.AddString("ISDN      - 128 kbps");
	m_Combo.AddString("Dialup    - 56 kbps");
	m_Combo.SetCurSel(m_Ssp);
	m_Combo.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSSpeed::OnOK()
{
	m_Ssp = m_Combo.GetCurSel();
	::AfxGetApp()->WriteProfileInt("Speed","SP",m_Ssp);
	CDialog::OnOK();
}
