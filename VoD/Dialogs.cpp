// Dialogs.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "Dialogs.h"

void xxx(TCHAR* TextToDisplay)
{
	MessageBox(NULL,TextToDisplay,"Warning",MB_OK|MB_ICONWARNING);
}

TCHAR* AddS(TCHAR * sentence) // Adds slashes to " ' " as " \'" in order to avoid sql errors
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
	DDX_Control(pDX, 1001, m_Edit);
}


BEGIN_MESSAGE_MAP(CFaculty, CDialog)
END_MESSAGE_MAP()


// CFaculty message handlers
// Dialogs.cpp : implementation file
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
	DDX_Control(pDX, 1001, m_Edit);
}


BEGIN_MESSAGE_MAP(CDepartment, CDialog)
END_MESSAGE_MAP()





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
	DDX_Control(pDX, IDC_EDIT3, m_Instructor);
	DDX_Control(pDX, IDC_EDIT4, m_Description);
}


BEGIN_MESSAGE_MAP(CCourse, CDialog)
END_MESSAGE_MAP()


// CCourse message handlers
// Dialogs.cpp : implementation file
//

// Dialogs.cpp : implementation file
//


// CProject dialog

IMPLEMENT_DYNAMIC(CProject, CDialog)
CProject::CProject(CWnd* pParent /*=NULL*/)
	: CDialog(CProject::IDD, pParent)
{
}

CProject::~CProject()
{
}

void CProject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, 1002, m_Video);
	DDX_Control(pDX, 1003, m_Browse);
	DDX_Control(pDX, 1001, m_ProjectName);
	DDX_Control(pDX, IDC_EDIT5, m_PrSubject);
	DDX_Control(pDX, IDC_EDIT4, m_PrDescription);
}


BEGIN_MESSAGE_MAP(CProject, CDialog)
END_MESSAGE_MAP()


// CProject message handlers
// Dialogs.cpp : implementation file
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
	DDX_Control(pDX, 1001, m_ChName);
	DDX_Control(pDX, IDC_EDIT4, m_ChDescription);
}


BEGIN_MESSAGE_MAP(CChapter, CDialog)
END_MESSAGE_MAP()


// CChapter message handlers
// Dialogs.cpp : implementation file
//

// CTreeWndEx

IMPLEMENT_DYNCREATE(CTreeWndEx, CTreeView)

CTreeWndEx::CTreeWndEx()
{
}

CTreeWndEx::~CTreeWndEx()
{
}

BEGIN_MESSAGE_MAP(CTreeWndEx, CTreeView)
END_MESSAGE_MAP()


// CTreeWndEx diagnostics

#ifdef _DEBUG
void CTreeWndEx::AssertValid() const
{
	CTreeView::AssertValid();
}

void CTreeWndEx::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG


// CTreeWndEx message handlers

BOOL CFaculty::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFaculty::OnOK()
{
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	int err,i;
	TCHAR sz[512],szError[512];
	m_Edit.GetWindowText(name,sizeof(name));
	wsprintf(sz,"Select * from vod.faculty where name='%s'",AddS(name));
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		xxx(szError);
		return;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	i = (int) mysql_num_rows( pM->m_Sql.res );
	if(i>0)
	{
		xxx("The faculty name already added");
		return ;
	}
	sprintf(sz,"INSERT INTO `faculty` ( `id` , `name` ) VALUES ('', '%s');",AddS(name));
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(name));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		xxx(szError);
		return ;
	}
//	CTabProject::TVAddItem();
//	TVAddItem(Tree->hWnd,name,nT,1);
	InvalidateRect(NULL,TRUE);
	CDialog::OnOK();
}

void CFaculty::OnCancel()
{
	//CTabPage *DParent = (CTabPage*) AfxGetApp()->m_pMainWnd;
	
	wsprintf(name,"");

	CDialog::OnCancel();
}

