// VoDUDPServerDoc.cpp : implementation of the CVoDUDPServerDoc class
//

#include "stdafx.h"
#include "VoDUDPServer.h"

#include "VoDUDPServerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVoDUDPServerDoc

IMPLEMENT_DYNCREATE(CVoDUDPServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CVoDUDPServerDoc, CDocument)
END_MESSAGE_MAP()


// CVoDUDPServerDoc construction/destruction

CVoDUDPServerDoc::CVoDUDPServerDoc()
{
	// TODO: add one-time construction code here

}

CVoDUDPServerDoc::~CVoDUDPServerDoc()
{
}

BOOL CVoDUDPServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CVoDUDPServerDoc serialization

void CVoDUDPServerDoc::Serialize(CArchive& ar)
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


// CVoDUDPServerDoc diagnostics

#ifdef _DEBUG
void CVoDUDPServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVoDUDPServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVoDUDPServerDoc commands
