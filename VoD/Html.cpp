// Html.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "Html.h"
#include "MainFrm.h"


// CHtml

IMPLEMENT_DYNCREATE(CHtml, CHtmlView)

CHtml::CHtml()
{
}

CHtml::~CHtml()
{
}

void CHtml::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHtml, CHtmlView)
	ON_MESSAGE (WM_SETFILE, SetFile)
END_MESSAGE_MAP()


// CHtml diagnostics

#ifdef _DEBUG
void CHtml::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHtml::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// CHtml message handlers
LONG CHtml::SetFile(WPARAM /*wParam*/,LPARAM /*lParam*/)
{
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

void CHtml::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	CMainFrame* pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	CString Location = pM->GetFile();
	//MessageBox(Location,"ok",MB_OK);
    Navigate (Location);
	CString Source;
	GetSource(Source);
	pM->SetSource(Source);
	//Navigate("http://www.ikikule.com",NULL,NULL,NULL,NULL);
}
