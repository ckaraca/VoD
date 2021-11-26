// FtpDown.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "FtpDown.h"
#define BUFFER 4096

// CFtpDown dialog

IMPLEMENT_DYNAMIC(CFtpDown, CDialog)
CFtpDown::CFtpDown(CWnd* pParent /*=NULL*/)
	: CDialog(CFtpDown::IDD, pParent)
{
	nIndex = 0;
	mts = new MTS1;
	pos = 0;
	m_Event = FALSE;
	Cancel1 =FALSE;
}

CFtpDown::~CFtpDown()
{
	if (m_pFtpConnection)
	{
		m_pFtpConnection->Close();
		delete m_pFtpConnection;
	}
	if (m_pInetSession)
	{
		m_pInetSession->Close();
		delete m_pInetSession;
	}
	if(mts)
		delete mts;
}

void CFtpDown::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
}

LONG CFtpDown::OnCan(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	Sleep(100);
	OnCancel();
	return 0;
}

BEGIN_MESSAGE_MAP(CFtpDown, CDialog)
	ON_MESSAGE(WM_INCT, OnInc)
	ON_MESSAGE(WM_INF, inf)
	ON_MESSAGE(WM_END, End)
	ON_MESSAGE(WM_CANCEL1, OnCan)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

LONG CFtpDown::inf(WPARAM wParam, LPARAM)
{
	Warn((TCHAR*)wParam);
	return 0;
}

LONG CFtpDown::End(WPARAM wParam, LPARAM)
{
	CMainFrame *pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	ASSERT(pM);
	CVoDView * cF = (CVoDView *)pM->m_wndSplitter.GetPane(0,0);
	ASSERT(cF);
	cF->m_AVoD.Play( 0, -1,-1 );
	cF->m_AVoD.Stop();
	m_Cancel.SetWindowText("Finish");
	Cancel1=TRUE;
	return 0;
}
void CFtpDown::Warn(TCHAR *TextToWarn) // Type the text that you want it to appear in the list box
{
	m_List.InsertString(nIndex++,TextToWarn);
	m_List.SetCurSel(nIndex-1);
}
// CFtpDown message handlers
void CFtpDown::Connect()
{
	//NumFiles = 0;
	//Files =0;
	//Num = &NumFiles;
	CMainFrame *pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	ASSERT(pM);
	CVoDView * cF = (CVoDView *)pM->m_wndSplitter.GetPane(0,0);
	ASSERT(cF);
	TCHAR text[512];
	cF->m_AVoD.Play( 0, -1,-1 );
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
	Warn("Searching path");
	// use a file find object to enumerate files
	CFtpFileFind finder(m_pFtpConnection);
	BOOL bWorking = finder.FindFile(cF->PrName1);
	if (GetLastError() == ERROR_NO_MORE_FILES)
	{
		AfxMessageBox("The Project you requested cannot be found on this server");
		//cF->SendMessage(WM_CLOSEPROJECT,0,0);
		canceled = 1;
		OnCancel();
		return;
	}
	try
	{
        m_pFtpConnection->SetCurrentDirectory(cF->PrName1);
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
	wsprintf(text,"Dricetory set to %s",cF->PrName1);
	Warn(text);
	TCHAR FileName[MAX_PATH],FilePath[MAX_PATH];
	SetCurrentDirectory(cF->PrName);
	// determine total download size
	bWorking = finder.FindFile(_T("*"));
	//FileLength += finder.GetLength();
	//NumFiles++;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		FileLength += finder.GetLength();		
	}
	m_Progress.SetRange32(0,(int)(FileLength/1024));
	wsprintf(text,"Total download size %li kb",FileLength/1024);
	Warn(text);
	mts->m_pFtpConnection = m_pFtpConnection;
	mts->hWnd = this->m_hWnd;
	mts->Event = m_Event;
	mts->Cancel1 = &Cancel1;
	bWorking = finder.FindFile(_T("*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		wsprintf(FilePath,"%s", (LPCTSTR) finder.GetFileURL());
		wsprintf(FileName,"%s", (LPCTSTR) finder.GetFileName());
		m_FileLength = finder.GetLength();
		Sleep(100);
		Numof1++;
		try
		{
			wsprintf(mts->FileName,"%s",FileName);
			mts->m_FileLength = m_FileLength;
			AfxBeginThread(Download,mts);
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
}

UINT Download(LPVOID pParam)
{
	MTS1 *mts =(MTS1*) pParam;
	TCHAR FileName[MAX_PATH];
	wsprintf(FileName,mts->FileName);
	while(mts->Event == TRUE)
	{
		if(*(mts->Cancel1))
		{
			SendMessage(mts->hWnd,WM_CANCEL1,0,0);
			AfxEndThread(-1,TRUE);
		}
		Sleep(100);
	}
	mts->Event = TRUE;
	LONGLONG FileLength = mts->m_FileLength;
	LONGLONG m_dwFileLength;
	CFile m_File;
	CInternetFile* pInternetFile;
	TCHAR text[512];
	wsprintf(text,"Downloading file:%s",FileName);
	SendMessage(mts->hWnd,WM_INF,(WPARAM)text,0);
	try
	{
		m_File.Open(FileName, CFile::modeCreate | CFile::modeWrite, NULL);
	}
	catch (CFileException *ex)
	{
		ex->ReportError();
	}
	try
	{
        pInternetFile = mts->m_pFtpConnection->OpenFile(FileName);
		m_dwFileLength = pInternetFile->GetLength();
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
			mts->m_pFtpConnection = NULL;
		return 1;
	}

	// download
	char buffer[BUFFER];
	unsigned int nRead = BUFFER;
	double nTotalRead = 0;
	while (nRead == BUFFER)
	{
		if(*(mts->Cancel1))
		{
			m_File.Close();
			pInternetFile->Close();
			delete pInternetFile;
			SendMessage(mts->hWnd,WM_END,0,0);
			SendMessage(mts->hWnd,WM_CANCEL,0,0);
			AfxEndThread(-1,TRUE);
		}
		// read remote data into buffer
		nRead = pInternetFile->Read(buffer, BUFFER);
		// write buffer to data file
		m_File.Write(buffer, nRead);
		nTotalRead += nRead;

		
			int nPos = (int)(nTotalRead/1024);
			::SendMessage(mts->hWnd,WM_INCT,nRead,0);
			//m_ProgressDlg.SetPos(nPos);
		
	}
	// close the file
	m_File.Close();
	double Down = nTotalRead/1024;
	CString Dd;
	Dd.Format("%.2f kb Downloaded",Down);
	wsprintf(text,"%s",Dd);
	SendMessage(mts->hWnd,WM_INF,(WPARAM)text,0);
	// close internet file
	pInternetFile->Close();

	delete pInternetFile;
	
	Numof2++;
    if(Numof1==Numof2)
	{
		SendMessage(mts->hWnd,WM_END,0,0);
	}
	mts->Event = FALSE;
	return 0;
}

LONG CFtpDown::OnInc(WPARAM wParam, LPARAM lParam)
{
	if((int)lParam==1)
	{
		m_Progress.SetPos(0);
	}else
	{
        pos+= (unsigned int)wParam;
		m_Progress.SetPos((int)pos/1024);
	}
	return 0;	
}
BOOL CFtpDown::OnInitDialog()
{
	CDialog::OnInitDialog();
	FileLength = 0;
	CMainFrame *pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	CVoDView * cF = (CVoDView *)pM->m_wndSplitter.GetPane(0,0);
	TCHAR sz[1024],szError[512];
	wsprintf(sz,"Select * from vod.ftp where ftp.id=%s",pM->m_Sql.FTP);
//	AfxMessageBox(sz);
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
		i = AfxMessageBox("There are no Content servers found on the system\n Press OK to add one.\n You will also be able to add one from the menu.",MB_OKCANCEL |MB_ICONASTERISK);
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
	if(i>1)
	{
		AfxMessageBox(" implemented yet\nUsing last server",MB_OK);
	}
	if(i>=1)
	{
		while(pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res ))
		{
			sprintf(Host,pM->m_Sql.row[1]);
			sprintf(Port,pM->m_Sql.row[2]);
			sprintf(User,pM->m_Sql.row[3]);
			sprintf(Pass,pM->m_Sql.row[4]);
		}
	}
	Warn("Building download list..");
	Connect();	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFtpDown::OnBnClickedCancel()
{
	if(Cancel1==TRUE) { OnOK(); return;}
	Cancel1 = TRUE;	
	canceled =1;
}
