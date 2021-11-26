// Design.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "Design.h"
#include "MainFrm.h"
#include "Html.h"
#include "Source.h"
#include <atlstr.h>
// CDesign

IMPLEMENT_DYNCREATE(CDesign, CHtmlEditView)

CDesign::CDesign()
{
}

CDesign::~CDesign()
{
}

BEGIN_MESSAGE_MAP(CDesign, CHtmlEditView)
	ON_WM_CREATE()
	ON_MESSAGE (WM_SETFILE, SetFile)
END_MESSAGE_MAP()

BEGIN_DHTMLEDITING_CMDMAP(CDesign)
	DHTMLEDITING_CMD_ENTRY(ID_EDIT_COPY, IDM_COPY)
	DHTMLEDITING_CMD_ENTRY(ID_EDIT_CUT,  IDM_CUT)
	DHTMLEDITING_CMD_ENTRY(ID_EDIT_PASTE,IDM_PASTE)
	DHTMLEDITING_CMD_ENTRY(ID_EDIT_UNDO, IDM_UNDO)

	DHTMLEDITING_CMD_ENTRY(ID_UNDERLINE, IDM_UNDERLINE)
	DHTMLEDITING_CMD_ENTRY(ID_ITALIC,    IDM_ITALIC)
	DHTMLEDITING_CMD_ENTRY(ID_BOLD,      IDM_BOLD)
	DHTMLEDITING_CMD_ENTRY(ID_LEFT,      IDM_JUSTIFYLEFT)
	DHTMLEDITING_CMD_ENTRY(ID_CENTER,    IDM_JUSTIFYCENTER)
	DHTMLEDITING_CMD_ENTRY(ID_RIGHT,     IDM_JUSTIFYRIGHT)

	DHTMLEDITING_CMD_ENTRY(ID_HYPERLINK,   IDM_HYPERLINK)
	DHTMLEDITING_CMD_ENTRY(ID_IMAGE,       IDM_IMAGE)
	DHTMLEDITING_CMD_ENTRY(ID_FONT,        IDM_FONT)
	DHTMLEDITING_CMD_ENTRY(ID_INDENT,      IDM_INDENT)
	DHTMLEDITING_CMD_ENTRY(ID_OUTDENT,     IDM_OUTDENT)
	DHTMLEDITING_CMD_ENTRY(ID_ORDERLIST,   IDM_ORDERLIST )
	DHTMLEDITING_CMD_ENTRY(ID_UNORDERLIST, IDM_UNORDERLIST)
END_DHTMLEDITING_CMDMAP()
// CDesign diagnostics

#ifdef _DEBUG
void CDesign::AssertValid() const
{
	CHtmlEditView::AssertValid();
}

void CDesign::Dump(CDumpContext& dc) const
{
	CHtmlEditView::Dump(dc);
}
#endif //_DEBUG


// CDesign message handlers

void CDesign::OnInitialUpdate()
{
	CHtmlEditView::OnInitialUpdate();
	// just to see if the file exists or not, if not create then
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CFile File;

	CFileException fileException;
	if ( !File.Open( pFrame->GetFile(), CFile::modeReadWrite|CFile::shareDenyNone, &fileException ) ) //
	{
		TRACE( "Can't open file %s, error = %u\n",
			pFrame->GetFile(), fileException.m_cause );
		//CHtmlEditView::DestroyWindow();
		if ( !File.Open( pFrame->GetFile(), CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone, &fileException ) ) //
		{
			TRACE( "Can't open file %s, error = %u\n",
				pFrame->GetFile(), fileException.m_cause );
		}
	
	}
	
	Navigate(pFrame->GetFile(),0,NULL,NULL,NULL,0);
	File.Close();
}

LONG CDesign::SetFile(WPARAM /*wParam*/,LPARAM /*lParam*/)
{
	//MessageBox("ok","ok",MB_OK);
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CFile File;
	CFileException fileException;
	if ( !File.Open( pFrame->GetFile(), CFile::modeReadWrite|CFile::shareDenyNone, &fileException ) ) //
	{
		TRACE( "Can't open file %s, error = %u\n",
			pFrame->GetFile(), fileException.m_cause );
		//CHtmlEditView::DestroyWindow();
		if ( !File.Open( pFrame->GetFile(), CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone, &fileException ) ) //
		{
			TRACE( "Can't open file %s, error = %u\n",
				pFrame->GetFile(), fileException.m_cause );
		}
	
	}
	Navigate(pFrame->GetFile(),0,NULL,NULL,NULL,0);
	return 0;
}

void CDesign::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
		
	CHtmlEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CDesign::DestroyWindow()
{
	long id = GetDlgCtrlID();
	if(id==AFX_IDW_PANE_LAST) //we change the ID if we hide this window
		return S_OK;
	if(S_OK == GetIsDirty())
	{
		// save the temp file
		CFile File;
		CFileException fileException;
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		if ( !File.Open( pFrame->GetFile(), CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone, &fileException ) )
		{
			TRACE( "Can't open file %s, error = %u\n",
				pFrame->GetFile(), fileException.m_cause );
			CHtmlEditView::DestroyWindow();
		}
		CString Html1;
		GetDocumentHTML(Html1);
		//AfxMessageBox(TempPath);
		File.Write(Html1,Html1.GetLength());
		File.Close();
		pFrame->SetSource(Html1);
	}
	return CHtmlEditView::DestroyWindow();
}

HRESULT CDesign::NewDocument()
{
	 
	return 0;
}



int CDesign::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CHtmlEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

LRESULT CDesign::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	switch(message)
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
			case ID_BACKCOLOR:
			
				break;
			}
			break;
	}


	return CHtmlEditView::WindowProc(message, wParam, lParam);
}

void CDesign::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	
}

void CDesign::OnQuit()
{
	
	CHtmlEditView::OnQuit();
}
