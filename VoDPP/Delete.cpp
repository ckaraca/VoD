// Delete.cpp : implementation file
//

#include "stdafx.h"
#include "VoDPlayer.h"
#include "Delete.h"


// CDelete dialog
extern TCHAR TempPr[MAX_PATH];
IMPLEMENT_DYNAMIC(CDelete, CDialog)
CDelete::CDelete(CWnd* pParent /*=NULL*/)
	: CDialog(CDelete::IDD, pParent)
{
}

CDelete::~CDelete()
{
}

void CDelete::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
}


BEGIN_MESSAGE_MAP(CDelete, CDialog)
END_MESSAGE_MAP()


// CDelete message handlers

void CDelete::DeleteFiles(void)
{
	m_Progress.SetRange(0,(short)counter-1);	
	for(unsigned i=0;i<counter;i++)
	{
		try{
			DeleteFile(Files[i]);
			m_Progress.SetPos(i);
		}
		catch (...)
		{
			//e->ReportError();
//			e->Delete();
		}
	}
	
}

BOOL CDelete::OnInitDialog()
{
	CDialog::OnInitDialog();

	extern TCHAR TempPath[MAX_PATH];
	counter = 0;
	DirPresent = FALSE;
	CFileFind finder,finder1;
	GetTempPath(MAX_PATH,Temp);
	SetCurrentDirectory(Temp);
	/*BOOL bWorking = finder.FindFile("VoDanm");
	if(bWorking!=FALSE)
	{
		wsprintf(Files[counter++],"%s\\VoDanm",Temp);
	}
	finder.Close();
	bWorking = finder.FindFile("VoD.html");
	if(bWorking!=FALSE)
	{
		wsprintf(Files[counter++],"%s\\VoD.html",Temp);
	}
	finder.Close();*/
	BOOL bWorking = finder.FindFile("VoD.ico");
	if(bWorking!=FALSE)
	{
		wsprintf(Files[counter++],"%s\\VoD.ico",Temp);
	}
	finder.Close();
	bWorking = finder.FindFile(TempPr);
	if(bWorking!=FALSE)
	{
		DirPresent = TRUE;
		try{
			SetCurrentDirectory(TempPr);
		}
		catch (CFileException *e)
		{
			e->Delete();
		}
		BOOL Working = finder1.FindFile();
		while (Working)
		{
			Working = finder1.FindNextFile();
			if (finder1.IsDots())
				continue;
			wsprintf(Files[counter++],"%s",finder1.GetFilePath());
		}
		finder1.Close();

	}
	finder.Close();
	DeleteFiles();
	if(DirPresent)
	{
		try{
			SetCurrentDirectory(Temp);
			Sleep(200);
			RemoveDirectory(TempPr);
		}
		catch (CFileException *e)
		{
			e->ReportError();
			e->Delete();
		}
	}
	OnCancel();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDelete::OnCancel()
{
	//delete this;

	CDialog::OnCancel();
}
