// VoDView.cpp : implementation of the CVoDView class
// /verbose:lib
// /force:multiple


#include "stdafx.h"

#include "VoDView.inl"
//#include "vodview.h"

// CVoDView

IMPLEMENT_DYNCREATE(CVoDView, CFormView)

BEGIN_MESSAGE_MAP(CVoDView, CFormView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CODE, OnBnClickedCode)
	ON_WM_SIZE()
	ON_MESSAGE (WM_CHANGE_TAB, ChangeTab)
	ON_MESSAGE (WM_CLOSEPROJECT, CloseProject)
	ON_MESSAGE(WM_DESIGN, OnBnClickedButton2Mes)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON4, OnStop)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON3, OnPlay)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, OnSlide)
END_MESSAGE_MAP()

// CVoDView construction/destruction

CVoDView::CVoDView()
	: CFormView(CVoDView::IDD)
{
	Paint=TRUE;
	m_State = 0;
	Created = FALSE;
	PageCount=0;
	wsprintf(PrName,"Dummy");
	wsprintf(PrName1,"Dummy");
	CoInitialize(NULL);
}

CVoDView::~CVoDView()
{
	 FreeDirectShow();
	 CoUninitialize();
}


LONG CVoDView::CloseProject(WPARAM wParam,LPARAM lParam)
{
	m_Tab.DestroyWindow();
	m_Ruler.DestroyWindow();
	UpdateWindow();
	
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	pM->SetFile("VoD.html");
	::SendMessage(pM->m_hWnd,WM_SWITCH,2,0);
	Sleep(100);
	::SendMessage(pM->m_hWnd,WM_SWITCH,5,0);
	return 0;
}

CString CVoDView::CreateGuid(void)
{
	GUID m_guid;    // generated GUID
	// create random GUID
	m_guid = GUID_NULL;
	::CoCreateGuid(&m_guid);
	if (m_guid == GUID_NULL)
	{
		AfxMessageBox("Error creating guid");
	}
	CString strFormat,rString;
	strFormat.LoadString(GUIDFORMAT);

	// then format into destination
	rString.Format(strFormat,
		// first copy...
		m_guid.Data1, m_guid.Data2, m_guid.Data3,
		m_guid.Data4[0], m_guid.Data4[1], m_guid.Data4[2], m_guid.Data4[3],
		m_guid.Data4[4], m_guid.Data4[5], m_guid.Data4[6], m_guid.Data4[7]);
	return(rString);
}
extern int canceled;

LONG CVoDView::ChangeTab(WPARAM wParam,LPARAM lParam)
{
	mts =(MTS*) wParam;
	TCHAR sz[1024],szError[255],text[MAX_PATH];
	int err;
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	
	m_Tab.AddPage(_T("Project Details"), 1, &m_Manage);
	m_Tab.ActivatePage(mts->TabIndex);
	extern TCHAR DBIDext[100];
	strcpy(DBIDext,mts->DBID);
	// what ever condition happens open the video and serve it
	wsprintf(sz,"Select * from vod.project where id='%s'",mts->DBID);
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		Warn(szError);
		return 1;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	
	while ( pM->m_Sql.row = mysql_fetch_row( pM->m_Sql.res ) ) 
	{
		m_Manage.m_Name.SetWindowText(pM->m_Sql.row[1]);
		wsprintf(sz,"VoD - Current Project:%s",pM->m_Sql.row[1]);
		wsprintf(PrName,"%s",pM->m_Sql.row[1]);
		wsprintf(text,"VoD %s project location. Put your files here",pM->m_Sql.row[1]);
		wsprintf(PrName1,"%s",pM->m_Sql.row[5]);
		m_Manage.m_Subject.SetWindowText(pM->m_Sql.row[3]);
        m_Manage.m_Desc.SetWindowText(pM->m_Sql.row[4]);
		wsprintf(pM->m_Sql.FTP,"%s",pM->m_Sql.row[7]);
	}
	//wsprintf(PrName1,"%s%s",PrName1,PrName);
	TCHAR szTemp[MAX_PATH],temp[MAX_PATH];
	GetTempPath(MAX_PATH,szTemp);
	SetCurrentDirectory(szTemp);
	// make GUID here
	//CString Guid = CreateGuid();
	//wsprintf(PrName,"%s",Guid);
	//AfxMessageBox(PrName);
	CreateDirectory(PrName1,NULL);
	SetCurrentDirectory(PrName1);
	
	GetCurrentDirectory(MAX_PATH,PrName);
	CreateLink(PrName,text);
	extern TCHAR TempPath[];
	wsprintf(TempPath,PrName);
	extern int ProjectActive;
	ProjectActive = 1;
	wsprintf(temp,"%s\\%s",PrName,PrName);
	
	switch(mts->isLocal)
	{
	case TRUE:
		// Then get the file and copy it to temp
		PageCount=0;
		m_Manage.m_Path.EnableWindow(TRUE);
		m_Manage.m_Path.SetWindowText(mts->Path);
		m_Manage.m_BrButton.SetWindowText("Change File");
		//pM->SetFile(temp);
		AddPage(0,temp,1);
		//m_Ruler.AddSeperator( 0   , 1 ,1);

		//m_Ruler.AddSeperator( 10   , 2 );
		// create new html blank html file
		// switch to design mode
		// reset ruler, there might be some indicators
		GoStartVideo();
		break;
	case FALSE:
		// Then download file from Server
		//m_Manage.m_Path.EnableWindow(FALSE);
		PageCount=0;
		//AddPage(0,temp,1);
		m_Manage.m_Path.EnableWindow(false);
		//m_Manage.m_Path.SetWindowText("E:\\starsailor.mpg");
		m_Manage.m_BrButton.EnableWindow(false);
		// retrieve html page(s) and video from server by ftp
		// and put them to local temp
		// use PrName1 for file name as 10temp 1 vs
		// use PrName for local temp dir
		// saving as chapter name in the temp
		CFtpDown Download;
		Download.DoModal();
		if(canceled == 1){
			goto Quit;
		}
		
		TCHAR DatFile[MAX_PATH];
		wsprintf(DatFile,"%s.dat",PrName1);
		try 
		{
			Pref.Open(DatFile,CFile::modeReadWrite | CFile::shareDenyNone);
		}
		catch (CFileException *e)
		{
			e->ReportError();
		}
		PAGESDAT PageDat;
		Pref.Read(&PageDat,sizeof(PageDat));
		PageCount =0;
		for(signed i=0;i<PageDat.PageCount;i++)
		{
			Pages[i].Frame = PageDat.Pages[i].Frame;
			Pages[i].PageID = PageDat.Pages[i].PageID;
			Pages[i].PageSeenBefore = 0;
			strcpy(Pages[i].PageName,PageDat.Pages[i].PageName);
			if(i==0) AddPage(Pages[i].Frame,Pages[i].PageName,0);
			else AddPage1(Pages[i].Frame,Pages[i].PageName,0);
		}
		GoStartVideo(PageDat.Path);
		break;
	}
	delete []mts;
	pM->SetWindowText(sz);
	// goto start video
	return 0;
Quit:
	::PostMessage(this->m_hWnd,WM_CLOSEPROJECT,0,0);
	return 0;
}

void CVoDView::GoStartVideo(TCHAR *Video)
{
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	HRESULT hr;
	TCHAR File[MAX_PATH];
    if(Video==NULL) m_Manage.m_Path.GetWindowText(File,sizeof(File));
	else wsprintf(File,"%s",Video);
	strcpy(VideoPath,File);
	// Remember current play state to restart playback
    int nCurrentState = g_psCurrent;
	// First release any existing interfaces
    ResetDirectShow();
	// Load the selected media file
	hr = PrepareMedia(File);
	if (FAILED(hr))
    {
        // Error - disable play button and give feedback
        AfxMessageBox("File failed to render!");
        m_Play.EnableWindow(FALSE);
        FreeDirectShow();
        return;
    }
    else
    {
        m_Play.EnableWindow(TRUE);
    }
	ConfigureSeekbar();
	// If the user has asked to mute audio then we
    // need to mute this new clip before continuing.
    if (g_bGlobalMute)
        MuteAudio();
	 // If we were running when the user changed selection,
    // start running the newly selected clip
    if (nCurrentState == State_Running)
    {
		Play=1;
        OnPlay();
    }
    else
    {
        // Cue the first video frame
        Play=0;
        OnPlay();
    }
}



void CVoDView::Warn(TCHAR *TextToWarn)
{
	MessageBox(TextToWarn,"Warning",MB_OK|MB_ICONWARNING);
}

void CVoDView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_View);
	DDX_Control(pDX, IDC_BUTTON2, m_Design);
	DDX_Control(pDX, IDC_CODE, m_Code);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_ANIMATE2, m_AVoD);
	DDX_Control(pDX, IDC_BUTTON3, m_Play);
	DDX_Control(pDX, IDC_BUTTON4, m_Rec);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
	DDX_Control(pDX, IDC_CUSTOM1, m_Ruler);
	DDX_Control(pDX, IDC_SCREEN, m_Screen);
}

BOOL CVoDView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}

void CVoDView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_Screen.ModifyStyle(0, WS_CLIPCHILDREN);
	Play=3;
	m_nScrolPos = 0;
	m_Slider.SetRange(0, 100);
	m_Ruler.SetMargin(200);
	m_Ruler.SetBackGroundColor( RGB( 99 , 99 , 255 ) );
	m_Ruler.SetSeperatorSize( 6);
	m_Ruler.SetMilimeterPixel( 3 );
	
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	m_Design.SetIcon(IDI_LEDOFF);
	m_View.SetIcon(IDI_LEDON);
	m_Code.SetIcon(IDI_LEDOFF);
	m_Proj.Create(IDD_TABPROJECT, this);
	m_Tab.AddPage(_T("Project"), 0, &m_Proj);
	m_Manage.Create(IDD_TABMANAGE,this);
	
	RECT rt;
	GetClientRect(&rt);
	x=rt.right,y=rt.bottom;
	Created=TRUE;
	m_AVoD.Open(IDR_AVI1);
	m_Play.SetIcon(IDI_PLAY1,IDI_PLAY);
	m_Play.DrawBorder(FALSE);
	m_Rec.SetIcon(IDI_STOP1,IDI_STOP);
	m_Rec.DrawBorder(FALSE);
	////////////////////////////////////////////////////////////////////////
    //
    //  DirectShow-specific initialization code

    // Initialize COM
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    TCHAR sz[1024],szError[512];
    // Initialize DirectShow and query for needed interfaces
    HRESULT hr = InitDirectShow();
    if(FAILED(hr))
    {
        wsprintf(szError, "Failed to initialize DirectShow!  hr=0x%x",hr) ;
		MessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
        return ;
    }
	// -------- Check Ftp site
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	wsprintf(sz,"Select * from vod.ftp");
	int err,i;
	err = mysql_real_query(pM->m_Sql.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)pM->m_Sql.myData)) ;
		MessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	pM->m_Sql.res = mysql_store_result( pM->m_Sql.myData );
	i = (int) mysql_num_rows( pM->m_Sql.res );
	if(i==0)
	{
		i = AfxMessageBox("There are no ftp servers found on the system\n Press OK to add one.\n You will also be able to add one from the menu.",MB_OKCANCEL |MB_ICONASTERISK);
		if(i == IDOK)
		{
			CNewFtp ftp;
			ftp.DoModal();
		}
		else return ;
	}
}


// CVoDView diagnostics

#ifdef _DEBUG
void CVoDView::AssertValid() const
{
	CFormView::AssertValid();
}

void CVoDView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}


#endif //_DEBUG


// CVoDView message handlers

void CVoDView::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==0)
	{
		CMainFrame * pMainframe = (CMainFrame *) AfxGetApp()->m_pMainWnd;
        pMainframe->Ping();
	}
	if(nIDEvent==TIMERID)
	{
		ReadMediaPosition();    
	}
	CFormView::OnTimer(nIDEvent);
}

int CVoDView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTimer(1,10000,NULL);
	return 0;
}

void CVoDView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//m_Ruler.SetScrollPos( m_nScrolPos*10 );
}

LONG CVoDView::OnBnClickedButton2Mes(WPARAM wParam,LPARAM lParam)
{
	if ((int)wParam==0)this->OnBnClickedButton2();
	else if ((int)wParam==1)this->OnBnClickedButton1();
	return 0;
}

void CVoDView::OnBnClickedButton2() // design button
{
	if(m_State!=1)
	{
		m_Design.SetIcon(IDI_LEDON);
		m_View.SetIcon(IDI_LEDOFF);
		m_Code.SetIcon(IDI_LEDOFF);
		m_State = 1;
		AfxGetMainWnd ()->PostMessage(WM_SWITCH,3,0);
	}
}

void CVoDView::OnBnClickedButton1()  // view button
{
	if(m_State!=0)
	{
		m_Design.SetIcon(IDI_LEDOFF);
		m_Code.SetIcon(IDI_LEDOFF);
		m_View.SetIcon(IDI_LEDON);
		m_State = 0;
		AfxGetMainWnd ()->PostMessage(WM_SWITCH,2,0);
	}
}

void CVoDView::OnBnClickedCode()
{
	if(m_State!=2)
	{
		m_Design.SetIcon(IDI_LEDOFF);
		m_Code.SetIcon(IDI_LEDON);
		m_View.SetIcon(IDI_LEDOFF);
		m_State = 2;
		AfxGetMainWnd ()->PostMessage(WM_SWITCH,4,0);
	}
}


void CVoDView::OnSize(UINT nType, int cx, int cy)
{
	if(Created)
	{
		int nx,ny;
		RECT rt,rt2,rt3;
		if(m_Tab) m_Tab.GetClientRect(&rt);
		//GetClientRect(&rt1);
		nx=x-cx;ny=y-cy;
		if(m_Tab) m_Tab.SetWindowPos(NULL,rt.left,rt.top,rt.right-nx,rt.bottom,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(m_Proj) m_Proj.GetClientRect(&rt2);
		if(m_Proj)m_Proj.SetWindowPos(NULL,rt2.left,rt2.top,rt2.right-nx,rt2.bottom,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(m_Ruler) m_Ruler.GetClientRect(&rt3);
		if(m_Ruler) m_Ruler.SetWindowPos(NULL,rt3.left,rt3.top,rt3.right-nx+4,rt3.bottom+4,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(m_Ruler) m_Ruler.InvalidateRect(NULL,TRUE);
        x=cx;
	}
	CFormView::OnSize(nType, cx, cy);
}



void CVoDView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{//GetPos ();
	switch( nSBCode ) 
	{
	case SB_TOP:
		m_nScrolPos = 0;
		break;
	case SB_BOTTOM:
		m_nScrolPos = 100;
		break;
	case SB_LINEUP:
		m_nScrolPos -= 1;
		break;
	case SB_LINEDOWN:
		m_nScrolPos += 1;
		break;
	/*case SB_PAGEUP:
		m_nScrolPos -= si.nPage;
		break;*/
	/*case SB_PAGEDOWN:
		m_nScrolPos += si.nPage;
		break;
	case SB_THUMBTRACK:
		m_nScrolPos = si.nTrackPos;
		break;*/
	}; 
	
	HRESULT hr;
	
    static OAFilterState state;
    static BOOL bStartOfScroll = TRUE;

    if ((!pMS) || (!pMC))
        return;

    // If the file is not seekable, the trackbar is disabled. 
    DWORD dwPosition = m_Slider.GetPos();

    // Pause when the scroll action begins.
    if (bStartOfScroll) 
    {       
        hr = pMC->GetState(10, &state);
        bStartOfScroll = FALSE;
        hr = pMC->Pause();
    }
    
    // Update the position continuously.
	__int64 nTotalMS = g_rtTotalTime / 1000; // 100ns -> ms
	//int nSeconds = nTotalMS / 1000;
	

    REFERENCE_TIME rtNew = (g_rtTotalTime * dwPosition) / (nTotalMS/1000);

    hr = pMS->SetPositions(&rtNew, AM_SEEKING_AbsolutePositioning,
                           NULL,   AM_SEEKING_NoPositioning);

    // Restore the state at the end.
	
    if (nSBCode == TB_ENDTRACK)
    {
        if (state == State_Stopped)
            hr = pMC->Stop();
        else if (state == State_Running) 
            hr = pMC->Run();

        bStartOfScroll = TRUE;
    }

    // Update the 'current position' string on the main dialog.
    UpdatePosition(rtNew);

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}



BOOL CVoDView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CRuler::RULERWNDNOTIFY_INFO *pRulerNM = (CRuler::RULERWNDNOTIFY_INFO *)lParam;
	switch (LOWORD(wParam))
	{
	case TCN_SELCHANGE:
		AfxMessageBox("ok");
		break;
	}

	switch( pRulerNM->nSubMessage )
	{
	case NMSUB_RULER_SEPERATORCHANGE :
		//AfxMessageBox("ok");
		Pages[pRulerNM->iSepID].Frame = pRulerNM->iNewPos;
		break;
	case NMSUB_RULER_SEPERATORCHANGING :
		/*preFrame = Pages[pRulerNM->iSepID].PageID-1;
		neFrame = Pages[pRulerNM->iSepID].PageID+1;
		preF = (Pages[preFrame].Frame) +1;
		
		Pages[pRulerNM->iSepID].Frame;
		
		neF = (Pages[neFrame].Frame)-1;
		if(neF<0) Warn("ok");*/
		
		break; 
	};

	return CFormView::OnNotify(wParam, lParam, pResult);
}


void CVoDView::OnDestroy()
{
	CFormView::OnDestroy();

	FreeDirectShow();
}

int	CVoDView::AddPage(int Frame,TCHAR *Name, int type)
{
	TCHAR Path[MAX_PATH];
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	m_Ruler.AddSeperator( Frame   , PageCount ,type);
	Pages[PageCount].Frame = Frame;
	Pages[PageCount].PageID = PageCount;
	wsprintf(Pages[PageCount].PageName,"%d.html",PageCount);
	wsprintf(Path,"%s//%s",PrName,Pages[PageCount].PageName);
	
	pM->SetFileManually(Path);
	pM->SendMessage(WM_SWITCH,3,0);
	CDesign * Cd = (CDesign*) pM->m_wndSplitter.GetPane(1,0);
	Cd->SendMessage(WM_SETFILE,0,0);
	Pages[PageCount].PageSeenBefore=0;
	m_Ruler.SetScrollPos(Frame);
	m_Ruler.Invalidate(TRUE);
	Play=TRUE;
	OnPlay();
	Sleep(10);
	Play=FALSE;
	OnPlay();
	PageCount++;
	return 0;
}

int	CVoDView::AddPage1(int Frame,TCHAR *Name, int type)
{
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	m_Ruler.AddSeperator( Frame   , PageCount ,type);
	Pages[PageCount].Frame = Frame;
	Pages[PageCount].PageID = PageCount;
	wsprintf(Pages[PageCount].PageName,"%d.html",PageCount);
	PageCount++;
	return 0;
}


int	CVoDView::SetPage(TCHAR *Name)
{
	TCHAR Path[MAX_PATH];
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	wsprintf(Path,"%s//%s",PrName,Name);
	pM->SetFileManually(Path);
	//pM->SendMessage(WM_SWITCH,3,0);
	CDesign * Cd = (CDesign*) pM->m_wndSplitter.GetPane(1,0);
	Cd->SendMessage(WM_SETFILE,0,0);

	return 0;
}

int CVoDView::DeletePage(int PageID)
{
	return 0;
}

int CVoDView::MovePage(int PageID)
{
	return 0;
}

BOOL CVoDView::DestroyWindow()
{
	return CFormView::DestroyWindow();
}



int CVoDView::CreateLink(TCHAR *FilePath, TCHAR *Description) 
{ 
	TCHAR Desktop[MAX_PATH];TCHAR FileName[MAX_PATH] = "VoD.lnk";
	SHGetFolderPath(this->m_hWnd,CSIDL_DESKTOP ,NULL,0,Desktop);
	//TCHAR File[MAX_PATH];
	extern TCHAR ExtFile[];
	wsprintf(ExtFile,"%s\\%s",Desktop,FileName);
	//AfxMessageBox(File);
	//AfxMessageBox(FilePath);
	IShellLink *pShellLink;

	if(FAILED(CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(LPVOID *)&pShellLink)))
	{
		AfxMessageBox("Failed to create instance");
		return 4;
	}

	IPersistFile *pPersistFile;
	if(FAILED(pShellLink->QueryInterface(IID_IPersistFile,(LPVOID *)&pPersistFile)))
	{
		AfxMessageBox("Failed to create shell");
		pShellLink->Release();
		return 5;
	}
	if(FAILED(pShellLink->SetPath(FilePath)))
	{
		AfxMessageBox("Failed to create shell 2");
		pPersistFile->Release();
		pShellLink->Release();
		return 6;
	}

	if(FAILED(pShellLink->SetDescription(Description)))
	{
		AfxMessageBox("Failed to create shell 2");
		pPersistFile->Release();
		pShellLink->Release();
		return 6;
	}
	TCHAR Icon[MAX_PATH],Temp[MAX_PATH];
	GetTempPath(MAX_PATH,Temp);
	wsprintf(Icon,"%s\\VoD.ico",Temp);
	if(FAILED(pShellLink->SetIconLocation(Icon,0)))
	{
		pPersistFile->Release();
		pShellLink->Release();
		return 9;
	}
	
	WORD wFileTo[MAX_PATH];
	MultiByteToWideChar(CP_ACP,0,ExtFile,-1,(LPWSTR)wFileTo,MAX_PATH);
	if(FAILED(pPersistFile->Save((LPCWSTR)wFileTo,TRUE)))
	{
		AfxMessageBox("Failed to save shortcut");
		pPersistFile->Release();
		pShellLink->Release();
		return 10;
	}

	pPersistFile->Release();
	pShellLink->Release();
    return 0; 
} 

