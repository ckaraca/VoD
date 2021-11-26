// WndSplitterEx.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "WndSplitterEx.h"


// CWndSplitterEx

IMPLEMENT_DYNAMIC(CWndSplitterEx, CSplitterWnd)
CWndSplitterEx::CWndSplitterEx()
{
}

CWndSplitterEx::~CWndSplitterEx()
{
}


BEGIN_MESSAGE_MAP(CWndSplitterEx, CSplitterWnd)
END_MESSAGE_MAP()

BOOL CWndSplitterEx::ReplaceView(int row, int col,CRuntimeClass * pViewClass,SIZE size)
{
  CCreateContext context;
  BOOL bSetActive;
	       
   
  if ((GetPane(row,col)->IsKindOf(pViewClass))==TRUE)
       return FALSE;
				    
   
   // Get pointer to CDocument object so that it can be used in the creation 
   // process of the new view
   CDocument * pDoc= ((CView *)GetPane(row,col))->GetDocument();
   CView * pActiveView=GetParentFrame()->GetActiveView();
   if (pActiveView==NULL || pActiveView==GetPane(row,col))
      bSetActive=TRUE;
   else
      bSetActive=FALSE;

    // set flag so that document will not be deleted when view is destroyed
	pDoc->m_bAutoDelete=FALSE;    
    // Delete existing view 
   ((CView *) GetPane(row,col))->DestroyWindow();
    // set flag back to default 
    pDoc->m_bAutoDelete=TRUE;
 
    // Create new view                      
   
   context.m_pNewViewClass=pViewClass;
   context.m_pCurrentDoc=pDoc;
   context.m_pNewDocTemplate=NULL;
   context.m_pLastView=NULL;
   context.m_pCurrentFrame=NULL;
   
   CreateView(row,col,pViewClass,size, &context);
   
   CView * pNewView= (CView *)GetPane(row,col);
   
   if (bSetActive==TRUE)
      GetParentFrame()->SetActiveView(pNewView);

   //SetRowInfo(0,800,800);
   RecalcLayout(); 
   pNewView->OnInitialUpdate();
   GetPane(row,col)->SendMessage(WM_PAINT);
   
   return TRUE;
}


// CWndSplitterEx message handlers

