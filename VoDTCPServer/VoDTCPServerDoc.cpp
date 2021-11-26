// VoDTCPServerDoc.cpp : implementation of the CVoDTCPServerDoc class
//

#include "stdafx.h"
#include "VoDTCPServer.h"

#include "VoDTCPServerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVoDTCPServerDoc

IMPLEMENT_DYNCREATE(CVoDTCPServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CVoDTCPServerDoc, CDocument)
END_MESSAGE_MAP()


// CVoDTCPServerDoc construction/destruction

CVoDTCPServerDoc::CVoDTCPServerDoc()
{
	// TODO: add one-time construction code here

}

CVoDTCPServerDoc::~CVoDTCPServerDoc()
{
}

BOOL CVoDTCPServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CVoDTCPServerDoc serialization

void CVoDTCPServerDoc::Serialize(CArchive& ar)
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


// CVoDTCPServerDoc diagnostics

#ifdef _DEBUG
void CVoDTCPServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVoDTCPServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVoDTCPServerDoc commands
