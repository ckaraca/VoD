// Ftpd.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "Ftpd.h"
#include "VoDView.h"


// CFtpd dialog

IMPLEMENT_DYNAMIC(CFtpd, CDialog)
CFtpd::CFtpd(CWnd* pParent /*=NULL*/)
	: CDialog(CFtpd::IDD, pParent)
{
	PageCount = 0;
	nIndex = 0;
	fts = new THREAD;
	Cancel1 =FALSE;
}

CFtpd::~CFtpd()
{
	if (m_pFtpConnection != NULL)
	{
		m_pFtpConnection->Close();
		delete m_pFtpConnection;
	}
	if (m_pInetSession != NULL)
	{
		m_pInetSession->Close();
		delete m_pInetSession;
	}
	if(fts!=NULL)
		delete fts;
}

void CFtpd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDOK2, m_Start);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
}


BEGIN_MESSAGE_MAP(CFtpd, CDialog)
	ON_BN_CLICKED(IDOK2, OnBnClickedOk2)
	ON_MESSAGE(WM_INC, OnInc)
	ON_MESSAGE(WM_INC1, OnInc1)
	ON_MESSAGE(WM_CANCEL, OnCan)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CFtpd message handlers
LONG CFtpd::OnInc(WPARAM wParam, LPARAM /*lParam*/)
{
	m_Progress.SetPos((int)wParam);
	return 0;	
}

TCHAR* CFtpd::AddS(TCHAR * sentence) // Adds slashes to " ' " as " \'" in order to avoid sql errors
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



void CFtpd::OnBnClickedOk2()
{
	/* 
	- gather dir name
	- gather page count
	- video loc
	- get ftp address, user, pass from db
	- connect to ftp server
	- create dir
	- send page data to server
	- send video to dir.
	- send directory to server
	*/
	m_Start.EnableWindow(FALSE);
    CMainFrame *pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	ASSERT(pM);
	CVoDView * cF = (CVoDView *)pM->m_wndSplitter.GetPane(0,0);
	ASSERT(cF);
	extern TCHAR DBIDext[100];
    TCHAR text[1024],szError[512];
	TCHAR Name[100], Subject[200], Desc[700];
	cF->m_Manage.m_Name.GetWindowText(Name,sizeof(Name));
	cF->m_Manage.m_Subject.GetWindowText(Subject,sizeof(Subject));
	cF->m_Manage.m_Desc.GetWindowText(Desc,sizeof(Desc));
	TCHAR sz[2048];int err;
	wsprintf(sz,"UPDATE project SET name=\'%s\',chid=chid,subject=\'%s\',projectdesc=\'%s\',VideoPath=VideoPath, VideoName=VideoName WHERE id=\'%s\'",
		AddS(Name),AddS(Subject),AddS(Desc),AddS(DBIDext));
	//AfxMessageBox(sz);
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return ;
	}
	wsprintf(Path,"Dummy");
	
	if(strcmp(cF->PrName1,Path)!=0)
	{
        wsprintf(Path,"%s",cF->PrName1);
		wsprintf(text,"Path to write:%s",Path);
		Warn(text);
	} else
	{
		MessageBox("First develop your project");
		OnCancel();
	}
	PageCount = cF->PageCount;
	for(int i=0;i<PageCount;i++)
	{
		Pages[i].Frame = cF->Pages[i].Frame;
		wsprintf(Pages[i].PageName,"%s",cF->Pages[i].PageName);
		Pages[i].PageID = cF->Pages[i].PageID;
	}
	
	wsprintf(text,"total %d pages to send",Pages[PageCount-1].PageID+1);
	Warn(text);
	wsprintf(VideoPath,cF->VideoPath);
	// get ftp address
	wsprintf(text,"Getting server information");
	Warn(text);
	wsprintf(sz,"Select * from vod.ftp where ftp.id=%s",pM->m_Sql.FTP);
	int ix;
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		MessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
		return ;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	ix = (int) mysql_num_rows( pM->m_Sql.res );
	if(ix==0)
	{
		int ii = AfxMessageBox("There are no content servers found on the system\n Press OK to add one.\n You will also be able to add one from the menu.",MB_OKCANCEL |MB_ICONASTERISK);
		if(ii == IDOK)
		{
			CNewFtp ftp;
			ftp.DoModal();
		}
		else EndDialog(-1);
	}
	if(ix>1)
	{
		AfxMessageBox("Content server not understood",MB_OK);
	}
	if(ix>=1)
	{
		while(pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res ))
		{
			sprintf(Host,pM->m_Sql.row[1]);
			sprintf(Port,pM->m_Sql.row[2]);
			sprintf(User,pM->m_Sql.row[3]);
			sprintf(Pass,pM->m_Sql.row[4]);
		}
	}
	wsprintf(text,"Host:%s",Host);
	Warn(text);
	wsprintf(text,"Port:%s",Port);
	Warn(text);
	// calculate size of data to send
	CFileStatus status;
	LONGLONG Size;
	if( CFile::GetStatus( VideoPath, status ) )   // static function
	{
		Size = status.m_size/1024;
	}
	try
	{
		CFile vid(VideoPath,CFile::modeRead|CFile::shareDenyNone);
		wsprintf(VideoName,vid.GetFileName());
		vid.Close();
	}
	catch (CFileException* pEx)
	{
		// if an error occurs, just make a message box
		pEx->ReportError();
		pEx->Delete();
	}
	extern int SliderRange;
	Dat.SliderRange = SliderRange;
	Dat.TotalTime = cF->g_rtTotalTime;
	m_Progress.SetRange32(0,(int)Size);
	wsprintf(text,"File size:%li kB",Size);
    Warn(text);
	// connect to ftp server
	if (m_pFtpConnection != NULL)
		m_pFtpConnection->Close();
	delete m_pFtpConnection;
	m_pFtpConnection = NULL;
	INTERNET_PORT nPort=atoi(Port);
	DWORD dwServiceType = INTERNET_SERVICE_FTP;
	CString strObject;
	CString strServerName;
	if (!AfxParseURL(Host, dwServiceType, strServerName, strObject, nPort))
	{
		// try adding the "ftp://" protocol
		CString strFtpURL = _T("ftp://");
		strFtpURL += Host;

		if (!AfxParseURL(strFtpURL, dwServiceType, strServerName, strObject, nPort))
		{
			AfxMessageBox("Please enter a valid FTP URL", MB_OK);
			return;
		}
	}
	INTERNET_PORT iPort;
	iPort = atoi(Port);
	
	// Now open an FTP connection to the server
	if ((dwServiceType == INTERNET_SERVICE_FTP) && !strServerName.IsEmpty())
	{
		try
		{
			m_pFtpConnection = m_pInetSession->GetFtpConnection(strServerName,
				User,Pass,iPort);
		}
		catch (CInternetException* pEx)
		{
			// catch errors from WinINet
			TCHAR szErr[1024];
			if (pEx->GetErrorMessage(szErr, 1024))
				AfxMessageBox(szErr, MB_OK);
			else
				AfxMessageBox("An exception occurred when attempting to create an FTP connection.\r\n  Please check your internet configuration and make sure your environment\r\n is set up with WININET.DLL in the path.", MB_OK);
			pEx->Delete();

			m_pFtpConnection = NULL;
			return;
		}
	}
	else
	{
		AfxMessageBox("Please enter a valid FTP URL", MB_OK);
		return;
	}
	wsprintf(text,"Connected to server on port %d",nPort);
	Warn(text);
	try
	{
		m_pFtpConnection->CreateDirectory(Path);
	}
	catch (CInternetException* pEx)
	{
		// catch errors from WinINet
			TCHAR szErr[1024];
			if (pEx->GetErrorMessage(szErr, 1024))
				AfxMessageBox(szErr, MB_OK);
			else
				AfxMessageBox("An exception occurred when attempting to create directory.", MB_OK);
			pEx->Delete();

			m_pFtpConnection = NULL;
			return;
	}
	wsprintf(text,"Directory %s created",Path);
	Warn(text);
	
	try
	{
		m_pFtpConnection->SetCurrentDirectory(Path);
	}
	catch (CInternetException* pEx)
	{
		// catch errors from WinINet
			TCHAR szErr[1024];
			if (pEx->GetErrorMessage(szErr, 1024))
				AfxMessageBox(szErr, MB_OK);
			else
				AfxMessageBox("An exception occurred when attempting to create directory.", MB_OK);
			pEx->Delete();

			m_pFtpConnection = NULL;
			return;
	}
	wsprintf(text,"Setting current dir to %s",Path);
	Warn(text);
	// use thread to send the file
	fts->m_pFtpConnection = m_pFtpConnection;
	strcpy(fts->VideoPath,VideoPath);
	strcpy(fts->VideoName,VideoName);
	fts->hWnd = this->m_hWnd;
	fts->Cancel1 = &Cancel1;
	strcpy(fts->VideoName,VideoName);
	wsprintf(text,"Sending video");
	Warn(text);
	cF->m_AVoD.Play( 0, -1,-1 );
	AfxBeginThread(SendViaFtp,fts);
}

UINT SendViaFtp(LPVOID pParam)
{
	TCHAR Path[MAX_PATH],VideoName[MAX_PATH],text[1024],FtpVideo[MAX_PATH];
	CFile m_File;
	BOOL bWorking =0;	// whether the file is on the server or not, if 0 not found.
	LONGLONG m_FileLength;
	THREAD* fts = (THREAD*) pParam;
	HWND hWnd = fts->hWnd;
	CFtpConnection* m_pFtpConnection = fts->m_pFtpConnection;
	strcpy(Path,fts->VideoPath);
	strcpy(VideoName,fts->VideoName);
	wsprintf(text,"%s",VideoName);
	// open source file
	
	try	{
        m_File.Open(Path, CFile::modeRead|CFile::shareDenyNone);
		m_FileLength = m_File.GetLength();
	}
	catch (CFileException* pEx)
	{
		// if an error occurs, just make a message box
		pEx->ReportError();
		pEx->Delete();
	}
	
	wsprintf(text,"Video name: %s",VideoName);
	//MessageBox(NULL,text,"ok",MB_OK);
	try {
		CFtpFileFind ftpFile(m_pFtpConnection);
		bWorking = ftpFile.FindFile(_T(""));
		while(bWorking)
		{
			bWorking = ftpFile.FindNextFile();
			if (ftpFile.IsDots())
			continue;
			wsprintf(FtpVideo,"%s",ftpFile.GetFileName());
			if(strcmp(VideoName,FtpVideo)==0)
			{
				wsprintf(text,"The video: %s present in the server.\nDo you want to overwrite the existing file?",VideoName);
				if(MessageBox(NULL,text,"Warning",MB_OK|MB_YESNO)== IDNO)
				{
					::SendMessage(hWnd,WM_INC1,0,0);
					m_File.Close();
					AfxEndThread(0,TRUE);
				}
                
			}
		}
	}
	catch (CInternetException* pEx)
	{
		pEx->ReportError();
		pEx->Delete();
	}
	
	CInternetFile* pUp = m_pFtpConnection->OpenFile(VideoName,GENERIC_WRITE);
	if (!pUp)
	{
		wsprintf(text, "Unable to open remote file %s", VideoName); 
		throw text; 
	}
	
	char buffer[4096];
	unsigned int nRead = 4096;
	unsigned int nTotalRead = 0;
	while (nRead == 4096 )
	{
		if(*(fts->Cancel1))
		{
			m_File.Close();
			pUp->Close();
			delete pUp;
			SendMessage(hWnd,WM_CANCEL,0,0);
			AfxEndThread(-1,TRUE);
		}
		// read data into buffer
		nRead = m_File.Read(buffer, 4096);
		// write buffer to remote data file
		try{
            pUp->Write(buffer, nRead);
		} catch (CInternetException *e)
		{
			e->ReportError();
			e->Delete();
		}
		nTotalRead += nRead;
		
		// update progressbar
		if (m_FileLength > 0)
		{
			int nPos = (nTotalRead/1024);
			::SendMessage(hWnd,WM_INC,nPos,0);
		}
	}
	// close the file
	m_File.Close();
	try{
        pUp->Close();
	} catch (CInternetException *e)
	{
		e->ReportError();
		e->Delete();
	}
	delete pUp;
	::SendMessage(hWnd,WM_INC1,0,0);
	
	return 0;
}

LONG CFtpd::OnInc1(WPARAM wParam, LPARAM /*lParam*/)
{
	Warn("Video sent to server");
	Warn("Sending documents");
	TCHAR text[512];CFile m_File;
	CMainFrame *pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	CVoDView * cF = (CVoDView *)pM->m_wndSplitter.GetPane(0,0);
	// first send Pages data
	
	Dat.PageCount = cF->PageCount;
	wsprintf(Dat.Path,"%s",VideoName);
	for(unsigned i=0;i<(cF->PageCount);i++)
	{
		Dat.Pages[i].Frame = Pages[i].Frame;
		Dat.Pages[i].PageID = Pages[i].PageID;
		strcpy(Dat.Pages[i].PageName,Pages[i].PageName);
	}

	TCHAR DatPageName[MAX_PATH];
	wsprintf(DatPageName,"%s\\%s.dat",cF->PrName,Path);
	try	{
		m_File.Open(DatPageName, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);
	}
	catch (CFileException* pEx)
	{
		// if an error occurs, just make a message box
		pEx->ReportError();
		pEx->Delete();
	}
	try	{
		m_File.Write(&Dat,sizeof(Dat));

	}
	catch (CFileException* pEx)
	{
		// if an error occurs, just make a message box
		pEx->ReportError();
		pEx->Delete();
	}

	// send pages, images, documents
	CSADirRead dr;
	dr.ClearFiles();
	dr.GetDirs(cF->PrName, false);
	// dump the current directory list
	CSADirRead::SADirVector &dirs = dr.Dirs();
	
	dr.GetFiles("*.*");
	CSADirRead::SAFileVector &files = dr.Files();
	FILES *Files = new FILES[files.size()];
	int FileCnt = 0;
	for (CSADirRead::SAFileVector::const_iterator fit = files.begin(); fit!=files.end(); fit++)
	{
		wsprintf(Files[FileCnt++].File,"%s", (*fit).m_sName);
	}
	TCHAR RemoteName[MAX_PATH];
	for(signed i=0;i<FileCnt;i++)
	{
		// Send all files to server :)
		try
		{
			CFile vid(Files[i].File,CFile::modeRead|CFile::shareDenyNone);
			wsprintf(RemoteName,vid.GetFileName());
			
			if(strcmp(VideoName,RemoteName)==0)
			{
				continue;
			}
			wsprintf(text,"Local: %s",RemoteName);
			Warn(text);
			vid.Close();
		}
		catch (CFileException* pEx)
		{
			// if an error occurs, just make a message box
			pEx->ReportError();
			pEx->Delete();
		}

		SendStuff(Files[i].File,RemoteName);
	}
	Warn("Completed sending files..");
	cF->m_AVoD.Play( 0, -1,-1 );
	cF->m_AVoD.Stop();
	m_Cancel.SetWindowText("Finish..");
	Cancel1=TRUE;
	return 0;	
}

int CFtpd::SendStuff(TCHAR * LocalPath,TCHAR * RemoteName)
{
	CFile m_File;
	TCHAR text[512];
	try	{
        m_File.Open(LocalPath, CFile::modeRead|CFile::shareDenyNone);
	}
	catch (CFileException* pEx)
	{
		// if an error occurs, just make a message box
		pEx->ReportError();
		pEx->Delete();
	}

	CInternetFile* pUp = m_pFtpConnection->OpenFile(RemoteName,GENERIC_WRITE);
	if (!pUp)
	{
		wsprintf(text, "Unable to open remote file %s", VideoName); 
		throw text; 
	}
	// get file size
	LONGLONG m_FileLength;
	m_FileLength = m_File.GetLength();
	if(m_FileLength>0)
	{
		char buffer[4096];
		unsigned int nRead = 4096;
		unsigned int nTotalRead = 0;
		while (nRead == 4096 )
		{
			// read data into buffer
			nRead = m_File.Read(buffer, 4096);
			// write buffer to remote data file
			pUp->Write(buffer, nRead);
			nTotalRead += nRead;
		}
	}
	// close the file
	m_File.Close();
	pUp->Close();
	delete pUp;
	return 0;
}

void CFtpd::Warn(TCHAR *TextToWarn)
{
	m_List.InsertString(nIndex++,TextToWarn);
	m_List.SetCurSel(nIndex-1);
}




BOOL CFtpd::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	TCHAR sz[1024],szError[512];
	wsprintf(sz,"Select * from vod.ftp where ftp.id=%s",pM->m_Sql.FTP);
	int err,i;
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		MessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
		return FALSE;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	i = (int) mysql_num_rows( pM->m_Sql.res );
	if(i==0)
	{
		i = AfxMessageBox("There are no Content Servers found on the system\n Press OK to add one.\n You will also be able to add one from the menu.",MB_OKCANCEL |MB_ICONASTERISK);
		if(i == IDOK)
		{
			CNewFtp ftp;
			ftp.DoModal();
		}
		else EndDialog(-1);
	}
	// FTPTREE-specific initialization follows...
	m_pFtpConnection = NULL;
	m_pInetSession = new CInternetSession("VoDFtp", 1, PRE_CONFIG_INTERNET_ACCESS);
	if (!m_pInetSession)
	{
		AfxMessageBox("Cannot open an Internet Session.  Please check your internet configuration.", MB_OK);
		OnCancel();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LONG CFtpd::OnCan(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	Sleep(100);
	OnCancel();
	AfxMessageBox("User aborted transfer");
	return 0;
}

void CFtpd::OnBnClickedCancel()
{
	if(Cancel1==TRUE) OnCancel();
	Cancel1 = TRUE;	
}

void CFtpd::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
}

BOOL CFtpd::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DestroyWindow();
}
