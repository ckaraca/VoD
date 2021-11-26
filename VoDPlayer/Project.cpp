// Project.cpp : implementation file
//

#include "stdafx.h"
#include "VoDPlayer.h"
#include "Project.h"

#define new DEBUG_NEW
// CProject dialog

IMPLEMENT_DYNAMIC(CProject, CDialog)
CProject::CProject(CWnd* pParent /*=NULL*/)
	: CDialog(CProject::IDD, pParent)
{
	list=0;
	//temp = new TREE;
	Tree = new TREE;
	nT = new TREE;
}

CProject::~CProject()
{
}

void CProject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_Select);
}


BEGIN_MESSAGE_MAP(CProject, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnCancel)
	ON_BN_CLICKED(IDOK, OnBnProject)
END_MESSAGE_MAP()


// CProject message handlers

void CProject::OnBnCancel()
{
	
	OnCancel();
}

BOOL CProject::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Select.EnableWindow(FALSE);	
	InitTree();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CProject::InitTree(void)
{
	RECT r;
	GetClientRect(&r);
	r.bottom-=40;
	m_Tree.Create(
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | 
			TVS_LINESATROOT, // styles
		r,	// bounding rectangle,
		this, //parent
		NULL); //id
	
	//CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	int err,err1,err2,err3,err4;
	TCHAR sz[255],sz1[255],sz2[255],sz3[255],sz4[255],szError[255];
	Tree->tvis.hParent =NULL;
	Tree->tvis.item.mask =TVIF_TEXT ;
	Tree->tvis.hInsertAfter = NULL;
	strcpy(Tree->name,_T("Eastern Mediterranean University"));
	Tree->tvis.item.pszText = Tree->name;
	Tree->hWnd = m_Tree.InsertItem(&Tree->tvis);
	Tree->id = 0;
	Tree->prev=NULL;
	Tree->next=nT;
	nT->prev = Tree;
	nT->next = NULL;
	strcpy(nT->name,"«New»");
	wsprintf(sz,"Select * from vod.faculty ");
	err = mysql_real_query(Log.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError," %s",mysql_error((MYSQL*)Log.myData)) ;
		MessageBox(szError,"DB Error",MB_OK);
		return;
	}
	Log.res = mysql_store_result( Log.myData );
	while ( Log.row = mysql_fetch_row( Log.res ) ) 
	{
		TVAddItem(Tree->hWnd,Log.row[1],nT,1);
		HTREEITEM hW=nT->prev->hWnd;
		wsprintf(sz1,"Select * from vod.department where facid='%s'",Log.row[0]);
		err1 = mysql_real_query(Log.myData,sz1,sizeof(sz1));
		if(err1!=0)
		{
			wsprintf(szError, " %s",mysql_error((MYSQL*)Log.myData)) ;
			MessageBox(szError,"DB Error",MB_OK);
			return;
		}
		Log.res1 = mysql_store_result( Log.myData );
		// Department search
		while ( Log.row = mysql_fetch_row( Log.res1 ) ) 
		{
			TVAddItem(hW,Log.row[2],nT,2);
			HTREEITEM hW1=nT->prev->hWnd;
			wsprintf(sz2,"Select * from vod.course where depid='%s'",Log.row[0]);
			err2 = mysql_real_query(Log.myData,sz2,sizeof(sz2));
			if(err2!=0)
			{
				wsprintf(szError, " %s",mysql_error((MYSQL*)Log.myData)) ;
				MessageBox(szError,"DB Error",MB_OK);
				return;
			}
			Log.res2 = mysql_store_result(Log.myData );
			// Course search
			while ( Log.row = mysql_fetch_row( Log.res2 ) ) 
			{
				wsprintf(name,"%s - %s",Log.row[2],Log.row[3]);
				TVAddItem(hW1,name,nT,3,Log.row[2],Log.row[0]);
				//--->
				HTREEITEM hW2=nT->prev->hWnd;
				wsprintf(sz3,"Select * from chapter where csid='%s'",Log.row[0]);
				err3 = mysql_real_query(Log.myData,sz3,sizeof(sz3));
				if(err3!=0)
				{
					wsprintf(szError, " %s",mysql_error((MYSQL*)Log.myData)) ;
					MessageBox(szError,"DB Error",MB_OK);
					return;
				}
				Log.res3 = mysql_store_result( Log.myData );
				// Chapter search
				while ( Log.row = mysql_fetch_row( Log.res3 ) ) 
				{
					
					TVAddItem(hW2,Log.row[1],nT,4,Log.row[1],Log.row[0]);
					//Project search
					HTREEITEM hW3=nT->prev->hWnd;
					wsprintf(sz4,"Select * from project where chid='%s'",Log.row[0]);
					err4 = mysql_real_query(Log.myData,sz4,sizeof(sz4));
					if(err4!=0)
					{
						wsprintf(szError, " %s",mysql_error((MYSQL*)Log.myData)) ;
						MessageBox(szError,"DB Error",MB_OK);
						return;
					}
					Log.res4 = mysql_store_result( Log.myData );
					while ( Log.row = mysql_fetch_row( Log.res4 ) ) 
					{
						TVAddItem(hW3,Log.row[1],nT,5,Log.row[1],Log.row[0]);
					}
		
				}
			}
		}
	}
	nT->tvis.hParent = Tree->hWnd;
	nT->tvis.item.mask =TVIF_TEXT | TVIF_IMAGE ;
	nT->tvis.hInsertAfter = TVI_LAST;
	nT->tvis.item.pszText = "{New}";
	nT->id=0;
	
	m_Tree.InvalidateRect(NULL,FALSE);
}


void CProject::TVAddItem(HTREEITEM iparent,TCHAR *itext,TREE *last,int id)
{
	list++;
	TREE *newt = new TREE;
	strcpy(newt->name,itext);
	newt->tvis.hParent = iparent;
	newt->tvis.item.mask = TVIF_TEXT | TVIF_IMAGE ;
	newt->tvis.hInsertAfter = TVI_SORT;
	newt->tvis.item.pszText = (LPTSTR)itext;
	newt->id=id;
	newt->hWnd = TreeView_InsertItem(m_Tree,&newt->tvis);
	last->prev->next = newt;
	newt->prev = last->prev;
    newt->next = last;
	last->prev = newt;
	//delete []newt;
}

void CProject::TVAddItem(HTREEITEM iparent,TCHAR *itext,TREE *last,int id,TCHAR *RealName)
{
	list++;
	TREE *newt = new TREE;
	strcpy(newt->name,itext);
	newt->tvis.hParent = iparent;
	newt->tvis.item.mask = TVIF_TEXT | TVIF_IMAGE ;
	newt->tvis.hInsertAfter = TVI_SORT;
	newt->tvis.item.pszText = (LPTSTR)itext;
	newt->id=id;
	wsprintf(newt->DBNAME,"%s",RealName);
	newt->hWnd = TreeView_InsertItem(m_Tree,&newt->tvis);
	last->prev->next = newt;
	newt->prev = last->prev;
    newt->next = last;
	last->prev = newt;
	//delete []newt;
}

void CProject::TVAddItem(HTREEITEM iparent,TCHAR *itext,TREE *last,int id,TCHAR *RealName,TCHAR *RealID)
{
	list++;
	TREE *newt = new TREE;
	strcpy(newt->name,itext);
	newt->tvis.hParent = iparent;
	newt->tvis.item.mask = TVIF_TEXT | TVIF_IMAGE ;
	newt->tvis.hInsertAfter = TVI_SORT;
	newt->tvis.item.pszText = (LPTSTR)itext;
	newt->id=id;
	wsprintf(newt->DBNAME,"%s",RealName);
	wsprintf(newt->DBID,"%s",RealID);
	newt->hWnd = TreeView_InsertItem(m_Tree,&newt->tvis);
	last->prev->next = newt;
	newt->prev = last->prev;
    newt->next = last;
	last->prev = newt;
	//delete []newt;
}

BOOL CProject::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMTREEVIEW *nmptr;
	nmptr = (LPNMTREEVIEW) lParam;
	switch(nmptr->hdr.code) 
		{
			case TVN_SELCHANGED:
				hItem= nmptr->itemNew.hItem;
				//InvalidateRect(NULL,1);
				temp = Tree->next;
				for(unsigned int i=0;i<list;i++)
				{
					if(temp->hWnd==hItem)
					{
						if(temp->id==5)
							m_Select.EnableWindow(TRUE);
						else m_Select.EnableWindow(FALSE);
						break;
					} else  temp=temp->next;	
					
				}
				break;
		}
	return CDialog::OnNotify(wParam, lParam, pResult);
}




void CProject::OnBnProject()
{
	mts = new MTS;
	strcpy(mts->DBID,temp->DBID);
	CProject* Parent = (CProject*)GetParent();
	::PostMessage(Parent->m_hWnd,WM_SETPR,(WPARAM)mts,0);
	TREE *tmp = Tree;

	while(tmp->next!=NULL)
	{
		tmp =tmp->next;
	}
	for(unsigned i=0;i<list;i++)
	{
		tmp =tmp->prev;
		delete []tmp->next;
	}
	//delete []nT;
	
	//TREE *temp,*Tree,*nT;
	OnCancel();
}

void CProject::OnCancel()
{
	

	CDialog::OnCancel();
}
