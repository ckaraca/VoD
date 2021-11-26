// TabProject.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "TabProject.h"
#include "MainFrm.h"
#include "VoDView.h"
#include "mediatypes.h"

// CTabProject

IMPLEMENT_DYNCREATE(CTabProject, CTabPage)

CTabProject::CTabProject()
	: CTabPage(CTabProject::IDD)
{
	list=0;
	temp = new TREE;
	Tree = new TREE;
	nT = new TREE;
	Created=FALSE;
}

CTabProject::~CTabProject()
{
	//delete(temp);
	//delete(Tree);
	//delete(nT);
}

void CTabProject::DoDataExchange(CDataExchange* pDX)
{
	CTabPage::DoDataExchange(pDX);
}

void CTabProject::Warn(TCHAR *TextToWarn)
{
	MessageBox(TextToWarn,"Warning",MB_OK|MB_ICONWARNING);
}

BEGIN_MESSAGE_MAP(CTabProject, CTabPage)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ADD1, OnAdd1)
	ON_COMMAND(ID_ADD2, OnAdd2)
	ON_COMMAND(ID_ADD3, OnAdd3)
	ON_COMMAND(ID_ADD4, OnAdd4)
	ON_COMMAND(ID_ADD5, OnAdd5)
	ON_WM_SIZE()
//	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_MENU_EDITTHISPROJECT, OnMenuEditthisproject)
END_MESSAGE_MAP()


// CTabProject diagnostics

#ifdef _DEBUG
void CTabProject::AssertValid() const
{
	CTabPage::AssertValid();
}

void CTabProject::Dump(CDumpContext& dc) const
{
	CTabPage::Dump(dc);
}
#endif //_DEBUG


// CTabProject message handlers

BOOL CTabProject::OnInitDialog()
{
	CTabPage::OnInitDialog();
	InitTree();
	Created=TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// Tree initilization
void CTabProject::InitTree(void)
{
	RECT r;
	GetClientRect(&r);
	r.bottom-=20;
	m_Tree.Create(
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | 
			TVS_LINESATROOT, // styles
		r,	// bounding rectangle,
		this, //parent
		NULL); //id
	
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	int err,err1,err2,err3,err4;
	TCHAR sz[255],sz1[255],sz2[255],sz3[255],sz4[255],szError[255];
	Tree->tvis.hParent =NULL;
	Tree->tvis.item.mask =TVIF_TEXT ;
	Tree->tvis.hInsertAfter = NULL;
	strcpy(Tree->name,_T("Faculties"));
	Tree->tvis.item.pszText = Tree->name;
	Tree->hWnd = m_Tree.InsertItem(&Tree->tvis);
	Tree->id = 0;
	Tree->prev=NULL;
	Tree->next=nT;
	nT->prev = Tree;
	nT->next = NULL;
	strcpy(nT->name,"«New»");
	wsprintf(sz,"Select * from vod.faculty ");
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError," %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		MessageBox(szError,"DB Error",MB_OK);
		return;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res ) ) 
	{
		TVAddItem(Tree->hWnd,pM->m_Sql.row[1],nT,1);
		HTREEITEM hW=nT->prev->hWnd;
		wsprintf(sz1,"Select * from vod.department where facid='%s'",pM->m_Sql.row[0]);
		err1 = mysql_real_query(pM->m_Sql.myData,sz1,sizeof(sz1));
		if(err1!=0)
		{
			wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
			MessageBox(szError,"DB Error",MB_OK);
			return;
		}
		pM->m_Sql.res1 = mysql_store_result( pM->m_Sql.myData );
		// Department search
		while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res1 ) ) 
		{
			TVAddItem(hW,pM->m_Sql.row[2],nT,2);
			HTREEITEM hW1=nT->prev->hWnd;
			wsprintf(sz2,"Select * from vod.course where depid='%s'",pM->m_Sql.row[0]);
			err2 = mysql_real_query(pM->m_Sql.myData,sz2,sizeof(sz2));
			if(err2!=0)
			{
				wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
				MessageBox(szError,"DB Error",MB_OK);
				return;
			}
			pM->m_Sql.res2 = mysql_store_result(pM->m_Sql.myData );
			// Course search
			while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res2 ) ) 
			{
				wsprintf(name,"%s - %s",pM->m_Sql.row[2],pM->m_Sql.row[3]);
				TVAddItem(hW1,name,nT,3,pM->m_Sql.row[2],pM->m_Sql.row[0]);
				//--->
				HTREEITEM hW2=nT->prev->hWnd;
				wsprintf(sz3,"Select * from chapter where csid='%s'",pM->m_Sql.row[0]);
				err3 = mysql_real_query(pM->m_Sql.myData,sz3,sizeof(sz3));
				if(err3!=0)
				{
					wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
					MessageBox(szError,"DB Error",MB_OK);
					return;
				}
				pM->m_Sql.res3 = mysql_store_result( pM->m_Sql.myData );
				// Chapter search
				while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res3 ) ) 
				{
					
					TVAddItem(hW2,pM->m_Sql.row[1],nT,4,pM->m_Sql.row[1],pM->m_Sql.row[0]);
					//Project search
					HTREEITEM hW3=nT->prev->hWnd;
					wsprintf(sz4,"Select * from project where chid='%s'",pM->m_Sql.row[0]);
					err4 = mysql_real_query(pM->m_Sql.myData,sz4,sizeof(sz4));
					if(err4!=0)
					{
						wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
						MessageBox(szError,"DB Error",MB_OK);
						return;
					}
					pM->m_Sql.res4 = mysql_store_result( pM->m_Sql.myData );
					while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res4 ) ) 
					{
						TVAddItem(hW3,pM->m_Sql.row[1],nT,5,pM->m_Sql.row[1],pM->m_Sql.row[0]);
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

int CTabProject::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}
// TVAddItem
void CTabProject::TVAddItem(HTREEITEM iparent,TCHAR *itext,TREE *last,int id)
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
}

void CTabProject::TVAddItem(HTREEITEM iparent,TCHAR *itext,TREE *last,int id,TCHAR *RealName)
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
}

void CTabProject::TVAddItem(HTREEITEM iparent,TCHAR *itext,TREE *last,int id,TCHAR *RealName,TCHAR *RealID)
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
}

TCHAR* CTabProject::AddS(TCHAR * sentence) // Adds slashes to " ' " as " \'" in order to avoid sql errors
{
	unsigned int temp=0;
	TCHAR *chtemp = new TCHAR[1024];
	for(unsigned i=0;i<strlen(sentence);i++)
	{
		if(sentence[i]==(char)39)
		{
			sprintf(&chtemp[temp],"\\");
			temp++;
		} 
		chtemp[temp]=sentence[i];
		temp++;
	}
	chtemp[temp]='\0';
	return chtemp;
}

BOOL CTabProject::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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
						break;
					} else  temp=temp->next;	
					
				}
				break;
		}
	return CTabPage::OnNotify(wParam, lParam, pResult);
}

// On Right Click Menu
void CTabProject::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	HMENU hMenu,hSubMenu;
	HINSTANCE hThis=AfxGetInstanceHandle( );
	switch(temp->id)
		{
		case 0:
			hMenu = LoadMenu(hThis,(LPCTSTR)IDR_MENU1);
			
			hSubMenu = GetSubMenu(hMenu,0);
			TrackPopupMenu(hSubMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON ,
				point.x+10, point.y, 0,this->m_hWnd,NULL);
			DestroyMenu(hMenu); 
			break;
		case 1:
			hMenu = LoadMenu(hThis,(LPCTSTR)IDR_MENU2);
			hSubMenu = GetSubMenu(hMenu,0);
			TrackPopupMenu(hSubMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON ,
       				point.x+10, point.y, 0,this->m_hWnd,NULL);
			DestroyMenu(hMenu); 
			break;
		case 2:
			hMenu = LoadMenu(hThis,(LPCTSTR)IDR_MENU3);
			hSubMenu = GetSubMenu(hMenu,0);
			TrackPopupMenu(hSubMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON ,
       				point.x+10, point.y, 0,this->m_hWnd,NULL);
			DestroyMenu(hMenu); 
			break;
		case 3:
			hMenu = LoadMenu(hThis,(LPCTSTR)IDR_MENU4);
			hSubMenu = GetSubMenu(hMenu,0);
			TrackPopupMenu(hSubMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON ,
       				point.x+10, point.y, 0,this->m_hWnd,NULL);
			DestroyMenu(hMenu); 
			break;
		case 4:
			hMenu = LoadMenu(hThis,(LPCTSTR)IDR_MENU5);
			hSubMenu = GetSubMenu(hMenu,0);
			TrackPopupMenu(hSubMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON ,
       				point.x+10, point.y, 0,this->m_hWnd,NULL);
			DestroyMenu(hMenu);
			break;
		case 5:
			hMenu = LoadMenu(hThis,(LPCTSTR)IDR_MENU6);
			hSubMenu = GetSubMenu(hMenu,0);
			TrackPopupMenu(hSubMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON ,
       				point.x+10, point.y, 0,this->m_hWnd,NULL);
			DestroyMenu(hMenu);
			break;
		}
	
}
// Friend functions
void CTabProject::GetFac(CFaculty &Fac)
{
	strcpy(name,Fac.name);
	return;
}

void CTabProject::GetDep(CDepartment &Dep)
{
	strcpy(name,Dep.name);
	return;
}

void CTabProject::GetCou(CCourse &Cou)
{
	strcpy(m_ID,Cou.Code);
	strcpy(name,Cou.Name);
	strcpy(m_Inst,Cou.Inst);
	strcpy(m_Desc,Cou.Desc);
}

void CTabProject::GetCha(CChapter &Cha)
{
	strcpy(name,Cha.Name);
	strcpy(m_Desc,Cha.Desc);
}

void CTabProject::GetPro(CProject &Pro)
{
	strcpy(Path,Pro.BrEdit);
	strcpy(name,Pro.Name);
	strcpy(m_Inst,Pro.Subject);
	strcpy(m_Desc,Pro.Description);
	strcpy(m_FTP,Pro.FTP);
	//strcpy(mn_VideoName,Pro.m_VideoName);
	mn_VideoName = Pro.m_VideoName;
    
}
// Add Faculty
void CTabProject::OnAdd1()
{
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	CFaculty Faculty;
	INT_PTR Ret = -1;
	Ret = Faculty.DoModal();
	if(Ret == IDCANCEL) return; 
	GetFac(Faculty);
	int err,i;
	TCHAR sz[512],szError[512];
	wsprintf(sz,"Select * from vod.faculty where name='%s'",AddS(name));
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		MessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	i = (int) mysql_num_rows( pM->m_Sql.res );
	if(i>0)
	{
		MessageBox("The faculty name already added",NULL,MB_OK|MB_ICONWARNING);
		return ;
	}
	sprintf(sz,"INSERT INTO `faculty` ( `id` , `name` ) VALUES ('', '%s');",AddS(name));
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		MessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
		return ;
	}
	TVAddItem(Tree->hWnd,name,nT,1);
	m_Tree.InvalidateRect(NULL,FALSE);	
}
// Add department
void CTabProject::OnAdd2()
{
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	CDepartment Department;
	INT_PTR Ret = -1;
	Ret = Department.DoModal();
	if(Ret == IDCANCEL) return; 
	GetDep(Department);
	int err,i;
    TCHAR sz[512],szError[512],name1[255];
	wsprintf(sz,"Select * from vod.department where name='%s'",AddS(name));
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	i = (int) mysql_num_rows( pM->m_Sql.res );
	if(i>0)
	{
		Warn("The department name already added");
		return;
	}
	sprintf(sz,"Select id from faculty where name='%s'",temp->name);
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res ) ) 
	{
		sprintf(name1,pM->m_Sql.row[0]);
	}
	sprintf(sz,"INSERT INTO `department` (`id`, `facid`, `name`) VALUES ('','%s','%s');",name1,AddS(name));
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return;
	}
	TVAddItem(temp->hWnd,name,nT,2);
}
// Add Course
void CTabProject::OnAdd3()
{
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	CCourse Course;
	INT_PTR Ret = -1;
	Ret = Course.DoModal();
	if(Ret == IDCANCEL) return;
    GetCou(Course); // m_ID , name, m_Inst, m_Desc
	int err,i;
	TCHAR DepName[100];
	TCHAR sz[512],szError[255];
	wsprintf(sz,"Select * from vod.course where cscode='%s'",AddS(name));
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	i = (int) mysql_num_rows( pM->m_Sql.res );
	if(i>0)
	{
		Warn("The course already added");
		return;
	}
	sprintf(sz,"Select id from department where name='%s'",temp->name);
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return ;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res ) ) 
	{
		sprintf(DepName,pM->m_Sql.row[0]);
	}
	// m_ID , name, m_Inst, m_Desc
	sprintf(sz,"INSERT INTO `course` (`id`, `depid`, `cscode`, `name`, `instructor`,`coursedesc`) VALUES ('','%s','%s','%s','%s','%s');"
		,DepName,m_ID,AddS(name),AddS(m_Inst),AddS(m_Desc));
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return ;
	}
	sprintf(sz,"select id from course where course.cscode='%s' AND course.depid='%s'",m_ID,DepName);
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return ;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res ) ) 
	{
		sprintf(m_Desc,pM->m_Sql.row[0]);
	}

	wsprintf(szError,"%s - %s",m_ID,name);
	TVAddItem(temp->hWnd,szError,nT,3,m_ID,m_Desc);
}	
// Add chapter
void CTabProject::OnAdd4()	
{
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	CChapter Chapter;
	INT_PTR Ret = -1;
	Ret = Chapter.DoModal();
	if(Ret == IDCANCEL) return;
	GetCha(Chapter);
	int i,err;
	TCHAR sz[512],szError[256];
	// name,m_Desc
	wsprintf(sz,"Select * from vod.chapter where name='%s' AND csid='%s'",AddS(name),temp->DBID);
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return ;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	i = (int) mysql_num_rows( pM->m_Sql.res );
	if(i>0)
	{
		Warn("The chapter already added");
		return ;
	}
	sprintf(sz,"INSERT INTO `chapter` (`id`, `name`, `csid`, `chapterdesc`) VALUES ('','%s','%s','%s');"
				,AddS(name),temp->DBID,AddS(m_Desc));
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(name));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return ;
	}
	wsprintf(name,"%s",name);
	wsprintf(sz,"Select * from chapter where name='%s'",AddS(name));
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData));
		Warn(szError);
		return;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	// Chapter search
	while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res ) ) 
	{
		TVAddItem(temp->hWnd,name,nT,4,name,pM->m_Sql.row[0]);
	}
}

void CTabProject::OnAdd5()	// Add Project
{
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	CProject Project;
	INT_PTR Ret = -1;	
	Ret = Project.DoModal();
	if(Ret == IDCANCEL) return;
	GetPro(Project);
	int err,i;
	TCHAR sz[2048],szError[255];
	wsprintf(sz,"Select * from vod.project where name='%s' AND chid='%s'",AddS(name),temp->DBID);
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return ;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	CVoDView * cF = (CVoDView*)pM->m_wndSplitter.GetPane(0,0);
	i = (int) mysql_num_rows( pM->m_Sql.res );
	if(i>0)
	{
		Warn("The project already added");
		return ;
	}
	TCHAR *ext,text[MAX_PATH];
	CString Guid = cF->CreateGuid();
	wsprintf(text,"%s",Guid);
	sprintf(sz,"INSERT INTO `project` (`id`, `name`, `chid`, `subject`, `projectdesc`, `VideoPath`, `VideoName`, `ftpid`) VALUES ('','%s','%s','%s','%s','%s','%s','%s');"
		,ext=AddS(name),temp->DBID,AddS(m_Inst),AddS(m_Desc),text,mn_VideoName,m_FTP);
	wsprintf(pM->m_Sql.FTP,"%s",m_FTP);
	//return;
	delete ext;
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(name));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return ;
	}
	wsprintf(sz,"%s",name);
	TVAddItem(temp->hWnd,sz,nT,5);
	
	wsprintf(sz,"Select * from vod.project where name='%s' AND chid='%s'",AddS(name),temp->DBID);
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return ;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	mts = new MTS;
	while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res ) ) 
	{
		sprintf(mts->DBID,pM->m_Sql.row[0]);
	}
	
	mts->isLocal=TRUE;
	wsprintf(mts->Path,Path);
	mts->TabIndex=1;
	CWnd* Parent = GetParent();
	Parent->GetParent()->SendMessage (WM_USER+1,(WPARAM) mts, (LPARAM)temp->DBID);
}



// CFaculty dialog

IMPLEMENT_DYNAMIC(CFaculty, CDialog)
CFaculty::CFaculty(CWnd* pParent /*=NULL*/)
	: CDialog(CFaculty::IDD, pParent)
{
}

CFaculty::~CFaculty()
{
}

void CFaculty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDFAC, m_Faculty);
}


BEGIN_MESSAGE_MAP(CFaculty, CDialog)
END_MESSAGE_MAP()


// CFaculty message handlers

void CFaculty::OnOK()
{
	m_Faculty.GetWindowText(name,sizeof(name));
	CDialog::OnOK();
}
// TabProject.cpp : implementation file
//


// CDepartment dialog

IMPLEMENT_DYNAMIC(CDepartment, CDialog)
CDepartment::CDepartment(CWnd* pParent /*=NULL*/)
	: CDialog(CDepartment::IDD, pParent)
{
}

CDepartment::~CDepartment()
{
}

void CDepartment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, 1001, m_DepName);
}


BEGIN_MESSAGE_MAP(CDepartment, CDialog)
END_MESSAGE_MAP()


// CDepartment message handlers

void CDepartment::OnOK()
{
	m_DepName.GetWindowText(name,sizeof(name));
	CDialog::OnOK();
}
// TabProject.cpp : implementation file
//

// CCourse dialog

IMPLEMENT_DYNAMIC(CCourse, CDialog)
CCourse::CCourse(CWnd* pParent /*=NULL*/)
	: CDialog(CCourse::IDD, pParent)
{
}

CCourse::~CCourse()
{
}

void CCourse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, 1001, m_Code);
	DDX_Control(pDX, 1002, m_Name);
	DDX_Control(pDX, IDC_EDIT3, m_Inst);
	DDX_Control(pDX, IDC_EDIT4, m_Desc);
}


BEGIN_MESSAGE_MAP(CCourse, CDialog)
END_MESSAGE_MAP()


// CCourse message handlers

void CCourse::OnOK()
{
	m_Code.GetWindowText(Code,sizeof(Code));
	m_Name.GetWindowText(Name,sizeof(Name));
	m_Inst.GetWindowText(Inst,sizeof(Inst));
	m_Desc.GetWindowText(Desc,sizeof(Desc));
	CDialog::OnOK();
}
// TabProject.cpp : implementation file
//

// CChapter dialog

IMPLEMENT_DYNAMIC(CChapter, CDialog)
CChapter::CChapter(CWnd* pParent /*=NULL*/)
	: CDialog(CChapter::IDD, pParent)
{
}

CChapter::~CChapter()
{
}

void CChapter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, 1001, m_Name);
	DDX_Control(pDX, IDC_EDIT4, m_Desc);
}


BEGIN_MESSAGE_MAP(CChapter, CDialog)
END_MESSAGE_MAP()


// CChapter message handlers

void CChapter::OnOK()
{
	m_Name.GetWindowText(Name,sizeof(Name));
	m_Desc.GetWindowText(Desc,sizeof(Desc));
	CDialog::OnOK();
}
// TabProject.cpp : implementation file
//


IMPLEMENT_DYNAMIC(CProject, CDialog)
CProject::CProject(CWnd* pParent /*=NULL*/)
	: CDialog(CProject::IDD, pParent)
{
	ContentServer=-1;
}

CProject::~CProject()
{
}

void CProject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BrEdit, m_BrEdit);
	DDX_Control(pDX, 1003, m_Br);
	DDX_Control(pDX, IDC_Name, m_Name);
	DDX_Control(pDX, IDC_Subject, m_Subject);
	DDX_Control(pDX, IDC_Description, m_Description);
	DDX_Control(pDX, IDOK, m_IDOK);
	DDX_Control(pDX, IDC_COMBO3, m_combo);
}


BEGIN_MESSAGE_MAP(CProject, CDialog)
	ON_BN_CLICKED(1003, OnBrowse)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnCbnSelchangeCombo3)
END_MESSAGE_MAP()


// CProject message handlers

void CTabProject::OnSize(UINT nType, int cx, int cy)
{
	if(Created)
	{
        CTabPage::OnSize(nType, cx, cy);
		RECT rt;
		GetClientRect(&rt);
		m_Tree.SetWindowPos(NULL,rt.left,rt.top,rt.right,rt.bottom,SWP_NOMOVE|SWP_NOOWNERZORDER);
	}
}

void CProject::OnBrowse()
{
	CFileDialog dlgFile(TRUE);
    CString title;
    CString strFilter, strDefault, strFilename;

    VERIFY(title.LoadString(AFX_IDS_OPENFILE));

	

	// Initialize the file extensions and descriptions
    strFilter += "Media Files (*.avi, *.mpg, *.wav, *.mid)\0";
    strFilter += (TCHAR)'\0';
    strFilter += _T("*.avi;*.mpg;*.wav;*.mid\0");
    strFilter += (TCHAR)'\0';
    dlgFile.m_ofn.nMaxCustFilter++;

    CString allFilter;
    VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	
	for (int i=0; i < NUM_MEDIA_TYPES; i++)
    {
        //m_ListFiles.Dir(attr, TypeInfo[i].pszType);
		strFilter += TypeInfo[i].pszName;
		strFilter += (TCHAR)'\0';
		strFilter += TypeInfo[i].pszType;
		strFilter += (TCHAR)'\0';
		dlgFile.m_ofn.nMaxCustFilter++;
    }
    // Append the "*.*" all files filter
    strFilter += allFilter;
    strFilter += (TCHAR)'\0';     // next string
    strFilter += _T("*.*\0");
    strFilter += (TCHAR)'\0\0';   // last string
    dlgFile.m_ofn.nMaxCustFilter++;

    dlgFile.m_ofn.lpstrFilter = strFilter;
    dlgFile.m_ofn.lpstrTitle  = title;
    dlgFile.m_ofn.lpstrFile   = strFilename.GetBuffer(_MAX_PATH);

    // Display the file open dialog
    INT_PTR nResult = dlgFile.DoModal();

    // If a file was selected, update the main dialog
    if (nResult == IDOK)
    {
        m_BrEdit.SetWindowText(strFilename);
		m_BrEdit.GetWindowText(Path,MAX_PATH);
		m_IDOK.EnableWindow(TRUE);
        // Render this file and show the first video frame, if present
    }

    strFilename.ReleaseBuffer();
}

void CProject::OnOK()
{
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	m_BrEdit.GetWindowText(BrEdit,sizeof(BrEdit));
	CFile m_File;
	try{
        m_File.Open(BrEdit,CFile::modeRead|CFile::shareDenyNone,NULL);
		m_VideoName = m_File.GetFileName();
	}
	catch (CFileException *e)
	{
		e->ReportError();
		e->Delete();
	}
	m_Name.GetWindowText(Name,sizeof(Name));
	m_Subject.GetWindowText(Subject,sizeof(Subject));
	m_Description.GetWindowText(Description,sizeof(Description));
	int cur=m_combo.GetCurSel();
	CString rString;
	m_combo.GetLBText(cur,rString);
	TCHAR sz[1024],szError[255];
	wsprintf(sz,"Select id from vod.ftp WHERE dsc LIKE '%s'",rString);
	//AfxMessageBox(sz);
	int err;
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		MessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res ) ) 
	{
		wsprintf(FTP,"%s",pM->m_Sql.row[0]);
	}

	
	CDialog::OnOK();
	
}

BOOL CProject::OnInitDialog()
{
	CDialog::OnInitDialog();
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;

	m_IDOK.EnableWindow(FALSE);
	TCHAR sz[1024],szError[255];
	wsprintf(sz,"Select id,dsc from vod.ftp");
	int err;
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		MessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
		return FALSE;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res ) ) 
	{
		m_combo.AddString(pM->m_Sql.row[1]);
	}
	m_combo.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CTabProject::OnMenuEditthisproject()
{
	mts = new MTS;
	mts->isLocal=FALSE;
	strcpy(mts->DBID,temp->DBID);
	wsprintf(mts->Path,"");
	mts->TabIndex=1;
	CWnd* Parent = GetParent();
	extern int ProjectActive;
	ProjectActive = 0;
	Parent->GetParent()->SendMessage (WM_USER+1,(WPARAM) mts, (LPARAM)temp->DBID);
}

void CProject::OnCbnSelchangeCombo3()
{
	//AfxMessageBox("ok");
}
