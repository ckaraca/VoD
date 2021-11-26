// VoDDoc.cpp : implementation of the CVoDDoc class
//

#include "stdafx.h"
#include "VoD.h"
#include "MainFrm.h"
#include "VoDDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVoDDoc

IMPLEMENT_DYNCREATE(CVoDDoc, CDocument)

BEGIN_MESSAGE_MAP(CVoDDoc, CDocument)
	ON_COMMAND(ID_LOGIN_DISCONNECT, OnLoginDisconnect)
END_MESSAGE_MAP()


// CVoDDoc construction/destruction

CVoDDoc::CVoDDoc()
{
	// TODO: add one-time construction code here

}

CVoDDoc::~CVoDDoc()
{
}

BOOL CVoDDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}




// CVoDDoc serialization

void CVoDDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}



// CVoDDoc diagnostics

#ifdef _DEBUG
void CVoDDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVoDDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVoDDoc commands

void CVoDDoc::OnLoginDisconnect()
{
	CMainFrame * pMainframe = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	pMainframe->SetFile("VoD.html");
	AfxGetMainWnd ()->PostMessage(WM_SWITCH,1,0);
	AfxGetMainWnd ()->PostMessage(WM_SWITCH,2,0);
}


