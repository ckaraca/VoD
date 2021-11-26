// TabPage.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "TabPage.h"


// TabPage dialog

IMPLEMENT_DYNAMIC(CTabPage, CDialog)

CTabPage::CTabPage() {
#ifndef _AFX_NO_OCC_SUPPORT
	AfxEnableControlContainer();
#endif // !_AFX_NO_OCC_SUPPORT
}

CTabPage::CTabPage (UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent) {
#ifndef _AFX_NO_OCC_SUPPORT
	AfxEnableControlContainer ();
#endif // !_AFX_NO_OCC_SUPPORT
}

CTabPage::~CTabPage()
{
}

void CTabPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabPage, CDialog)
END_MESSAGE_MAP()


// TabPage message handlers
void CTabPage::OnOK(void) {
	//
	// Prevent CDialog::OnOK from calling EndDialog.
	//
}

void CTabPage::OnCancel(void) {
	//
	// Prevent CDialog::OnCancel from calling EndDialog.
	//
}

BOOL CTabPage::OnCommand(WPARAM wParam, LPARAM lParam) {
	// Call base class OnCommand to allow message map processing
	CDialog::OnCommand (wParam, lParam);
	//
	// Forward WM_COMMAND messages to the dialog's parent.
	//
	return (BOOL)GetParent ()->SendMessage (WM_COMMAND, wParam, lParam);
}

BOOL CTabPage::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* /*pResult*/) {
	//
	// Forward WM_NOTIFY messages to the dialog's parent.
	//
	return (BOOL)GetParent ()->SendMessage (WM_NOTIFY, wParam, lParam);
}

BOOL CTabPage::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo) {
	//
	// Forward ActiveX control events to the dialog's parent.
	//
#ifndef _AFX_NO_OCC_SUPPORT
	if (nCode == CN_EVENT)
		return GetParent()->OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
#endif // !_AFX_NO_OCC_SUPPORT

	return CDialog::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
}

