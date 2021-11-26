// Drag.cpp : implementation file
//

#include "stdafx.h"
#include "VoDPlayer.h"
#include "VoDPlayerDlg.h"

#include "Drag.h"
#include "Video.h"

extern TCHAR VoDPath[MAX_PATH];
// CDrag dialog

IMPLEMENT_DYNAMIC(CDrag, CDialog)
CDrag::CDrag(CWnd* pParent /*=NULL*/)
	: CDialog(CDrag::IDD, pParent)
{
	//CVoDPlayerDlg * pM = (CVoDPlayerDlg *) AfxGetApp()->m_pMainWnd;
	
	//Video = (CVoDPlayerDlg*)
	//uPageCount=0;
}

CDrag::~CDrag()
{
}

void CDrag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DRAG, m_drag);
}


BEGIN_MESSAGE_MAP(CDrag, CDialog)
	ON_BN_CLICKED(IDC_DRAG, OnDragLink)
	ON_WM_DROPFILES()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDrag message handlers

BOOL CDrag::OnInitDialog()
{
	CDialog::OnInitDialog();

	HWND hWnd = ::GetDesktopWindow();
	RECT rc;
	::GetWindowRect(hWnd,&rc);
	this->MoveWindow(rc.left+250,0,86,100);
	this->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	//m_drag.SetIcon(IDB_BITMAP4);
	m_drag.SetBitmaps(IDB_BITMAP4,0);			//SetBitmap(IDB_BITMAP4);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDrag::OnDragLink()
{
	CVoDPlayerDlg * pM = (CVoDPlayerDlg *) AfxGetApp()->m_pMainWnd;
	try{
	if(IsWindow(pM->Video->m_hWnd)!=0)
	{
	       	pM->Video->OnPlay();
	}
	} catch(...){}
}
extern TCHAR uDatFile[MAX_PATH];
extern TCHAR VoDPrPath[MAX_PATH];

void CDrag::OnDropFiles(HDROP hDropInfo)
{
	TCHAR file[MAX_PATH];
	char newfile[MAX_PATH];
	char oldfile[MAX_PATH];
	uPAGESDAT uPageDat;
	uPageCount=0;
	::DragQueryFile(hDropInfo,0,file,sizeof(file));
	CFile dropcuk;
	dropcuk.Open(file,CFile::modeRead|CFile::shareDenyNone);
	CString ilen = dropcuk.GetFileName();
	dropcuk.Close();
	wsprintf(oldfile,"%s",file);
	wsprintf(newfile,"%s\\%s",VoDPrPath,ilen);
	CopyFile(oldfile,newfile,FALSE);
	
	CFile uPref;
	if(!uPref.Open(uDatFile,CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone))
	{
		//AfxMessageBox("ok");
		uPref.Open(uDatFile,CFile::modeCreate|CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone);
		uPageCount=0;
		uPageDat.PageCount=0;
		uPref.Write(&uPageDat,sizeof(uPageDat));
	}
	try{
        uPref.Read(&uPageDat,sizeof(uPageDat));
		uPref.Close();
		//AfxMessageBox(text);
	}
	catch(CFileException *e)
	{
		e->ReportError();
		e->Delete();
	}
	uPageCount = uPageDat.PageCount;

	//AfxMessageBox(text);
	if(uPageCount!=0)
	{
		for(signed i=0;i<uPageDat.PageCount;i++)
		{
			uPages[i].Frame = uPageDat.Pages[i].Frame;
			uPages[i].PageID = uPageDat.Pages[i].PageID;
			uPages[i].PageSeenBefore = 0;
			strcpy(uPages[i].PageName,uPageDat.Pages[i].PageName);
		}
		uPageCount = uPageDat.PageCount;
	}
	/// add new ones
	CVoDPlayerDlg * pM = (CVoDPlayerDlg *) AfxGetApp()->m_pMainWnd;

	CVideo *pV = pM->Video;
	
	//TCHAR test[100];
	//wsprintf(test,"%d",pV->m_Slider.GetPos());
	//AfxMessageBox(test);
	AddPage(pV->m_Slider.GetPos(),newfile);
	
	// end add new ones
	// rebuilt index
	uPageDat.PageCount = uPageCount;
	for(unsigned i=0;i<uPageCount;i++)
	{
		uPageDat.Pages[i].Frame = uPages[i].Frame;
		uPageDat.Pages[i].PageID = uPages[i].PageID;
		strcpy(uPageDat.Pages[i].PageName,uPages[i].PageName);
	}
	uPref.Open(uDatFile, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);
	uPref.Write(&uPageDat,sizeof(uPAGESDAT));
	uPref.Close();
	pV->Refresh();
	CDialog::OnDropFiles(hDropInfo);
}

int	CDrag::AddPage(int Frame,TCHAR *Name)
{
	uPages[uPageCount].Frame = Frame;
	uPages[uPageCount].PageID = uPageCount;
	wsprintf(uPages[uPageCount].PageName,"%s",Name);
	//AfxMessageBox(uPages[uPageCount].PageName);
	uPages[uPageCount].PageSeenBefore=0;
	uPageCount++;
	return 0;
}

UINT CDrag::OnNcHitTest(CPoint point)
{
	return 2;

	//return CDialog::OnNcHitTest(point);
}

void CDrag::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();
}
