// Video.cpp : implementation file
//

#include "stdafx.h"
extern TCHAR TempPr[MAX_PATH];
extern TCHAR VoDPath[MAX_PATH];
extern TCHAR uDatFile[MAX_PATH];
#include "Directx.inl"
#include "video.h"
#include "VoDPlayerDlg.h"
#include "TCP.inl"


// CVideo dialog

extern PROJECTDB *Pr;
extern int Return;
extern int Aspect;
extern int Size;
extern BOOL Stats;
#ifdef SMT
	extern int smt_start=0;
	extern CFile smt_genel,smt_ftp,smt_content;
#endif

IMPLEMENT_DYNAMIC(CVideo, CDialog)
CVideo::CVideo(CWnd* pParent /*=NULL*/)
	: CDialog(CVideo::IDD, pParent)
{
	FullScreen = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	PageCount=0;
	Focus = TRUE;
	hMenu = LoadMenu(AfxGetInstanceHandle(),(LPCTSTR)IDR_MENU2);
	hSubMenu = GetSubMenu(hMenu,0);
	// log font
	lf.lfHeight = -18;
	lf.lfWidth = 12;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 600;						 //LOGFONT
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = TURKISH_CHARSET;
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	lf.lfClipPrecision = CLIP_TT_ALWAYS;
	lf.lfQuality = ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily = FF_DONTCARE; 
	lstrcpy(lf.lfFaceName,"Comic Sans MS");
	hfnt	=	CreateFontIndirect( &lf);
	Cont = FALSE;
	// time 
#ifdef TSAT
	Start = GetTickCount();
	tfile.Open("C:\\tres.txt",CFile::modeCreate|CFile::modeWrite);
#endif
#ifdef SMT
	smt_genel.Open("C:\\smtg.txt",CFile::modeCreate|CFile::modeWrite);
	smt_content.Open("C:\\smtc.txt",CFile::modeCreate|CFile::modeWrite);
	smt_ftp.Open("C:\\smtf.txt",CFile::modeCreate|CFile::modeWrite);
	smt_start = GetTickCount();
#endif
}

#ifdef TSAT 
void CVideo::GetTime(TCHAR *function)
{
	DWORD End = (GetTickCount()-Start);
	TCHAR tres[100];
	wsprintf(tres,"%s %d\r\n",function,End);
	tfile.Write(tres,(UINT)strlen(tres));
}
#endif


CVideo::~CVideo()
{
	hWnd = NULL;
	DeleteObject(hfnt);
#ifdef TSAT
	tfile.Close();
#endif
#ifdef SMT
	smt_genel.Close();
	smt_content.Close();
	smt_ftp.Close();
#endif
}

void CVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_explorer);
	DDX_Control(pDX, IDC_SCREEN, m_Screen);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
}

BEGIN_MESSAGE_MAP(CVideo, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_MESSAGE(WM_SLI,SetSlider)
	ON_MESSAGE(WM_SETSL,SetProgress)
	ON_MESSAGE(WM_PLAY, StartPlay)
	ON_COMMAND(ID_RAT_KEEPASPECTRATIO, OnRatKeepaspectratio)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_RAT_DOUBLESIZE, OnRatDoublesize)
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CVideo)
	EASYSIZE(IDC_SCREEN,ES_BORDER,ES_BORDER,ES_BORDER,IDC_SLIDER1,0)
	EASYSIZE(IDC_SLIDER1, ES_BORDER,IDC_SCREEN,ES_BORDER,IDC_SPLITTER,0)
    EASYSIZE(IDC_SPLITTER,ES_BORDER,IDC_SLIDER1,ES_BORDER,IDC_EXPLORER1,0)
	EASYSIZE(IDC_EXPLORER1,ES_BORDER,IDC_SPLITTER,ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP
//

// CVideo message handlers
extern int canceled;
extern CString Connection;
extern HWND hWnd;
extern TCHAR TempPr[MAX_PATH];
extern TCHAR VoDPrPath[MAX_PATH];
extern CString VideoPath;
extern int ProjectActive;
extern CString VideoStream;
extern LONG	pBitRate;


BOOL CVideo::OnInitDialog()
{
	CDialog::OnInitDialog();
#ifdef TSAT
	GetTime("Video dialog initialized");
#endif
#ifdef SMT
	GetTime("Video dialog initialized");
#endif


	if(Aspect==1)
	{
		CheckMenuItem(hSubMenu,ID_RAT_KEEPASPECTRATIO,MF_CHECKED);
		EnableMenuItem(hSubMenu,ID_RAT_DOUBLESIZE,MF_ENABLED);
	}
	else 
	{
		CheckMenuItem(hSubMenu,ID_RAT_KEEPASPECTRATIO,MF_UNCHECKED);
		EnableMenuItem(hSubMenu,ID_RAT_DOUBLESIZE,MF_GRAYED|MF_DISABLED);
	}


	TCHAR szWindowName[MAX_PATH];
	try{
		wsprintf(szWindowName,"VoD Player - Project: %s",Pr->name);
	}
	catch(CMemoryException *e)
	{
		e->ReportError();
		e->Delete();
	}
	SetWindowText(szWindowName);
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    


	TCHAR sz[1024],szError[512];

    // Initialize DirectShow and query for needed interfaces
    HRESULT hr = InitDirectShow();
#ifdef TSAT
	GetTime("DirectShow Init finished");
#endif
    if(FAILED(hr))
    {
        wsprintf(szError, "Failed to initialize DirectShow!  hr=0x%x",hr) ;
		AfxMessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
        return FALSE;
    }
	wsprintf(sz,"Select * from ftp,project where project.ftpid=%s",Pr->FTPID);
	int err,i;
	err = mysql_real_query(Log.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)Log.myData)) ;
		MessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
		return FALSE;
	}
#ifdef TSAT
	GetTime("Query made to search FTP ID");
#endif
	Log.res = mysql_store_result( Log.myData );
	i = (int) mysql_num_rows( Log.res );
	if(i==0)
	{
		i = AfxMessageBox("There are no ftp servers found on the system\n Press OK to add one.\n You will also be able to add one from the menu.",MB_OKCANCEL |MB_ICONASTERISK);
		if(i == IDOK)
		{
			CNewFtp ftp;
			ftp.DoModal();
			if (Return!=0) 
			{
				EndDialog(0);
				return TRUE;
			}
		}
		else
		{
			EndDialog(0);
			return TRUE;
		}
	}
#ifdef TSAT
	GetTime("MySQL Store Result");
#endif
	if(SetCurrentDirectory(TempPath)==NULL)	Error("SetCurrentDirectory");
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	CRect rc;
	CWnd* pWnd;

	pWnd = GetDlgItem(IDC_SPLITTER);
	m_Screen.GetClientRect(rc);
	ScreenToClient(rc);

	pWnd = GetDlgItem(IDC_SPLITTER);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_Splitter.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER);
	m_Splitter.SetRange(50, 350, -1);
#ifdef TSAT
	GetTime("Splitter created");
#endif
	// 0. Init VoD html screen
	TCHAR Path[MAX_PATH];
	wsprintf(Path,"%sVoD.html",TempPath);
	// Set temp directory
	CreateDirectory(Pr->VideoPath,NULL);
	if(SetCurrentDirectory(Pr->VideoPath)==NULL) Error("Set Directory");
	wsprintf(TempPr,"%s%s",TempPath,Pr->VideoPath);
	VideoPath.Format("%s\\%s",TempPr,Pr->VideoName);
	PageCount=0;
	CVoDPlayerDlg *dlg = (CVoDPlayerDlg*)AfxGetMainWnd();
	dlg->m_Play.SetIcon(IDI_PAUSE1,IDI_PAUSE);
#ifdef TSAT
	GetTime("Downloading content here");
#endif
	CFtpDown Download;
	if(Download.DoModal()!=0) Error("Ftp dialog");
	if(canceled == 1){
		goto Quit;
	}
#ifdef TSAT
	GetTime("Content download finished");
#endif
	// 1. Init video screen
	
	// 2. Connect to ftp and transfer files
	// 3. Select preferred video stream
	// 4. set timelines and load html files
	TCHAR DatFile[MAX_PATH];
	
	TCHAR szTemp[MAX_PATH];
	wsprintf(DatFile,"%s\\%s.dat",TempPr,Pr->VideoPath);
	
	//PathAppend(szTemp,VoDPath);
	wsprintf(szTemp,"%s",VoDPath);
	SetCurrentDirectory(szTemp);
	wsprintf(uDatFile,"%s",Pr->VideoPath);
	CreateDirectory(uDatFile,NULL);
	wsprintf(VoDPrPath,"%s\\%s",szTemp,uDatFile);
#ifdef TSAT
	GetTime("Directory created");
#endif
	//wsprintf(VoDPath,"%s",szTemp);
	
	wsprintf(uDatFile,"%s\\%s.dat",VoDPrPath,Pr->VideoPath);
	try
	{
		Pref.Open(DatFile,CFile::modeReadWrite | CFile::shareDenyNone);
		//uPref.Open(szTemp,CFile::modeCreate|CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone);
		

		//AfxMessageBox("look now");
	}
	catch (CFileException *e)
	{
		e->ReportError();
	}
	PAGESDAT PageDat;
	
	PageCount =0;
	
	try{
        Pref.Read(&PageDat,sizeof(PageDat));
		
		Pref.Close();
		
	}
	catch(CFileException *e)
	{
		e->ReportError();
		e->Delete();
	}
	for(signed i=0;i<PageDat.PageCount;i++)
	{
		Pages[i].Frame = PageDat.Pages[i].Frame;
		Pages[i].PageID = PageDat.Pages[i].PageID;
		Pages[i].PageSeenBefore = 0;
		strcpy(Pages[i].PageName,PageDat.Pages[i].PageName);
	}
	PageCount = PageDat.PageCount;
	extern int SliderRange;
	SliderRange = PageDat.SliderRange;
	TCHAR deb[100];
	wsprintf(deb,"%d",SliderRange);
	//AfxMessageBox(deb);
	m_Slider.SetRange(0, SliderRange,TRUE);
	m_Slider.SetLineSize(200);
	m_Slider.SetPageSize(200);

	for(signed i=0;i<PageDat.PageCount;i++)
	{
		m_Slider.SetTic((Pages[i].Frame));
	}
	m_Slider.GlbTicCnt();
	Refresh();
	m_TotalTime = (int)(PageDat.grTotalTime)/1000;
	//m_Progress.SetRange32(0,m_TotalTime);
	m_Slider.SetProRange(0,m_TotalTime);
	
	// select transfer protocol
	//---------------------------------------------directshow
	/// edited                      //FreeDirectShow();
	//InitDirectShow();
	
#ifdef TSAT
	GetTime("Sql query finished selecting desired protocol");
#endif
	hWnd = this->m_hWnd;
	m_pos = 0;
	if(Connection=="UDP")
	{
		pUdp = new CUDP;
		if (pUdp != NULL)
		{
			ASSERT_VALID(pUdp);
			if (!pUdp->CreateThread(THREAD_PRIORITY_NORMAL))
			{
				delete pUdp;
			}
		}
	}
	if(Connection=="RUDP")
	{
		pRudp = new CRUDP;
		if (pRudp != NULL)
		{
			ASSERT_VALID(pRudp);
			if (!pRudp->CreateThread(THREAD_PRIORITY_NORMAL))
			{
				delete pRudp;
			}
		}
	}

	if(Connection=="TCP")
	{
		AfxBeginThread(TCPThread,0,THREAD_PRIORITY_NORMAL);
	}
	if(Connection == "RAWFTP")
	{
		pFtp = new CFtpV;
		if (pFtp != NULL)
		{
			ASSERT_VALID(pFtp);
			if (!pFtp->CreateThread(THREAD_PRIORITY_NORMAL))
			{
				delete pFtp;
			}
		}

	}
	wsprintf(DatFile,"%d",PageCount);
#ifdef TSAT
	GetTime("Setting navigation");
#endif
	//m_explorer.Navigate(Path,NULL,NULL,NULL,NULL);

	INIT_EASYSIZE;
Quit:		
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CVideo::Refresh()
{
	uPAGESDAT uPageDat;
	uPageCount =0;
	
	
	uPref.m_hFile = CreateFile(uDatFile,           // open MYFILE.TXT 
                GENERIC_READ,              // open for reading 
                FILE_SHARE_READ | FILE_SHARE_WRITE, // share for reading 
                NULL,                      // no security 
                OPEN_EXISTING,             // existing file only 
                FILE_ATTRIBUTE_NORMAL,     // normal file 
                NULL);                     // no attr. template 
 
	if (uPref.m_hFile == INVALID_HANDLE_VALUE) 
	{ 
		//AfxMessageBox("No contributions for this project");
		Cont = FALSE;
	} else Cont=TRUE;
	if(Cont)
	{
		uPref.Read(&uPageDat,sizeof(uPageDat));

		for(signed i=0;i<uPageDat.PageCount;i++)
		{
			uPages[i].Frame = uPageDat.Pages[i].Frame;
			uPages[i].PageID = uPageDat.Pages[i].PageID;
			uPages[i].PageSeenBefore = 0;
			strcpy(uPages[i].PageName,uPageDat.Pages[i].PageName);
		}
		for(signed i=0;i<uPageDat.PageCount;i++)
		{
			m_Slider.SetTic((uPages[i].Frame));
		}
		uPageCount = uPageDat.PageCount;
			
		uPref.Close();
	}
}
extern int tWait;

LONG CVideo::StartPlay(WPARAM wParam,LPARAM lParam)   // wparam = file size in mpbs, lparam=d/l sp in mbps
{
#ifdef TSAT
	GetTime("Prepare media file starts here");
#endif
	//int nCurrentState = g_psCurrent;
	ProjectActive = 1;
	HRESULT hr;
	//CVoDPlayerDlg *dlg = (CVoDPlayerDlg*)AfxGetMainWnd();
	TCHAR File[MAX_PATH];
	wsprintf(File,VideoStream);
	//AfxMessageBox("2",MB_OK);
	hr = PrepareMedia(File);
#ifdef TSAT
	GetTime("Media file prepared");
#endif
	//-------------------------- smooth
	int _Fs = (int)wParam;
	pMS->GetDuration(&g_rtTotalTime);
	ULONG nTotal =(ULONG)(g_rtTotalTime / 10000000);
	//TCHAR tt[200];
	//wsprintf(tt,"tp->%d s, fs->%d, d/l s->%d",nTotal,_Fs,(int)lParam);
	//AfxMessageBox(tt,MB_OK);
	ULONG _Sp = _Fs/nTotal;
	Play=1;
	if(_Sp>(ULONG)lParam) /// making smooth buffering
	{
		tWait = ((_Fs/(ULONG)lParam)-nTotal)-2;
		SetTimer(113115,100,NULL);
		m_Slider.StartBlinking();
		Play =0;

		TCHAR tte[360] = ("<html><head>\r\n<SCRIPT LANGUAGE=\"JavaScript1.1\">\r\n\
function countDown()	{ if(eval(document.sec.elements[0].value)!=0)\r\n\
{ document.sec.elements[0].value--;\r\n\
setTimeout(\"countDown()\",1000);\r\n\
}\r\n\
}</SCRIPT></head>\r\n\
			<body bgcolor=\"FFFFFF\" onLoad=\"countDown();\" LEFTMARGIN=0 TOPMARGIN=0 MARGINWIDTH=0 MARGINHEIGHT=0>\r\n");
		TCHAR tte1[425];
		wsprintf(tte1,"<SCRIPT LANGUAGE=\"JavaScript1.1\">\r\n\
document.write(\"<input type='text' name='showsec' value='%d'\
size='1' maxlength='1' style='BACKGROUND-COLOR: FFFFFF; BORDER-BOTTOM-COLOR: FFFFFF;\
BORDER-BOTTOM-STYLE: solid; BORDER-LEFT-COLOR: FFFFFF; BORDER-LEFT-STYLE: solid; \
BORDER-RIGHT-COLOR: FFFFFF; BORDER-RIGHT-STYLE: solid; BORDER-TOP-COLOR: FFFFFF; \
BORDER-TOP-STYLE: solid;'>\");</SCRIPT>",tWait);
        CFile stats;
		CString html;
		html.Format("%s",TempPr);
		html.Format("%s\\VoDStat.html",html);
		stats.Open(html,CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone);
		TCHAR vr1[167],vr2[71],vr3[49],vr4[39];
		wsprintf(vr1,"<form name='sec'><table border=1><tr><th colspan=2 scope=col>VoD Quick Stats, while you wait.</th></tr><tr><td>Average D/L speed:</td><td>%d kbps.</td></tr>\r\n",(int)lParam);
		wsprintf(vr2,"<tr><td>Average playing speed:</td><td>%d kbps.</td></tr>\r\n",_Sp);
		wsprintf(vr3,"<tr><td>Time to start playing:</td><td>");
		wsprintf(vr4,"</td></tr></table></form>\r\n</html>");
		stats.Write(tte,(UINT)strlen(tte));
		stats.Write(vr1,(UINT)strlen(vr1));
		stats.Write(vr2,(UINT)strlen(vr2));
		stats.Write(vr3,(UINT)strlen(vr3));
		stats.Write(tte1,(UINT)strlen(tte1));
		stats.Write(vr4,(UINT)strlen(vr4));
		stats.Close();

		m_explorer.Navigate(html,NULL,NULL,NULL,NULL);
	}
	//-----------------------------------
	if (FAILED(hr))
    {
        // Error - disable play button and give feedback
		
		TCHAR szErr[MAX_ERROR_TEXT_LEN];
		DWORD res = AMGetErrorText(hr, szErr, MAX_ERROR_TEXT_LEN);
		if (res == 0)
		{
	           wsprintf(szErr, "Unknown Error: 0x%2x", hr);
		}
		MessageBox(szErr, TEXT("Error!"), MB_OK | MB_ICONERROR);
		OnCancel();
        return 1;
    }
    ConfigureSeekbar();
#ifdef TSAT
	GetTime("Seeker bar configured");
#endif
	if (g_bGlobalMute)
       MuteAudio();
	 // If we were running when the user changed selection,
    // start running the newly selected clip
   
	if (g_bAudioOnly)
	{
		RECT w;
		m_Screen.GetClientRect(&w);
		int dy = 25 - w.bottom;
		CSplitterControl::ChangeHeight(&m_Screen, dy, CW_TOPALIGN);
		CSplitterControl::ChangePos(&m_Slider,0,dy);
		//CSplitterControl::ChangePos(&m_Progress,0,dy);
		CSplitterControl::ChangeHeight(&m_explorer, -dy, CW_BOTTOMALIGN);
		Invalidate();
//		m_Progress.Invalidate();
		
		m_Screen.Invalidate();
		UpdateWindow();
	}
    OnPlay();
#ifdef TSAT
	GetTime("OnPlay triggered");
#endif
	return 0;
}

void CVideo::ConfigureSeekbar()
{
    if (pMS && SUCCEEDED(pMS->GetDuration(&g_rtTotalTime)))
	{
		       		
		//__int64 nTotalMS =(g_rtTotalTime / 1000); // 100ns -> ms
		m_Slider.EnableWindow(TRUE);
	}
    else
        m_Slider.EnableWindow(FALSE);
}

extern BOOL TimerisSet;
extern int ProRange,ProPos;
LONG CVideo::SetSlider(WPARAM wParam,LPARAM lParam)
{
	//m_Progress.SetPos((int)wParam);
	
	m_Slider.SetProgressPos(int(wParam));
	//m_Slider.Invalidate(FALSE);
//	CVoDPlayerDlg *dlg = (CVoDPlayerDlg*)AfxGetMainWnd();
	

	if(wParam==0) return 0;
	TCHAR text[256];
	int lower,upper;
	//m_Progress.GetRange(lower,upper);
	m_Slider.GetProRange(lower,upper);
	int percent = (int)wParam*100/upper;
	wsprintf(text,"Buffering: %d percent",(int)percent);
//	m_Play.SetWindowText(text);
	// ----------- Slider
	DWORD dwPosition = m_Slider.GetPos();
	int Range = m_Slider.GetRangeMax();
	int percent1 = dwPosition*100/Range;
	wsprintf(text,"Playing: %d percent",percent1);
//	m_Buffer.SetWindowText(text);
	if(percent1>0 && percent>0)
	{
		if(percent1>=(percent-3) && !TimerisSet) 
		{
			TimerisSet = TRUE;
			SetTimer(112,1000,NULL);
			m_Slider.StartBlinking();
		}
		
	}
	return 0;
}
LONG CVideo::SetProgress(WPARAM wParam,LPARAM lParam)
{
	m_Slider.SetProRange(0,(int)wParam);
	return 0;
}

void CVideo::Error(LPSTR lpszFunction) 
{ 
    CHAR szBuf[80]; 
    DWORD dw = GetLastError(); 
 
    sprintf(szBuf, "%s failed: GetLastError returned %u\n", 
        lpszFunction, dw); 
 
    AfxMessageBox(szBuf); 
} 


UINT CVideo::OnNcHitTest(CPoint point) 
{	
	m_Screen.GetClientRect(&rc);
	ClientToScreen(&rc);
	Rect.CopyRect(&rc);
	if(Rect.PtInRect(point)!=0)
	{
		return 2;
	}
	else
	return CDialog::OnNcHitTest(point);
}

void CVideo::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);
	if(ProjectActive)CenterVideo();
	
	EASYSIZE_MINSIZE(280,250,fwSide,pRect);	
}

void CVideo::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	UPDATE_EASYSIZE;
	if(ProjectActive)CenterVideo();
	if(m_Splitter) 
	{
		RECT rect;
		m_Splitter.GetClientRect(&rect);
		m_Splitter.SetWindowPos(NULL,rect.left,rect.top,cx-5,rect.bottom,SWP_NOMOVE|SWP_NOOWNERZORDER);
		m_Splitter.Invalidate();
	}
}

void CVideo::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(!FullScreen)
	{
		pVW->put_FullScreenMode(OATRUE);
		FullScreen = TRUE;
	}
	else 
	{
		FullScreen = FALSE;
		pVW->put_FullScreenMode(OAFALSE);
	}
		
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CVideo::OnPaint()
{
	CPaintDC dc(this);
	if (g_bAudioOnly)
	{
		CDC *pDC = m_Screen.GetDC();
		pDC->SelectObject(hfnt);
		pDC->GetTextMetrics(&tm);
		RECT rc;
		TCHAR Form[512];
		wsprintf(Form,"%s-%s",Pr->name,Pr->subject);
		m_Screen.GetClientRect(&rc);
		pDC->FillSolidRect(&rc,RGB(0,0,0));
		pDC->SetTextColor(RGB(255,255,102));
		pDC->SetBkColor(RGB(0,0,0));
		//GetTextExtentPoint32(hdc,&poem[count],1,&size);
		int left = ((int)strlen(Form)*tm.tmAveCharWidth)/2;

		pDC->TextOut(((rc.right/2)-left),rc.top,Form);
		
		ReleaseDC(pDC);
		
		goto Quit;
	}
	if(!ProjectActive)
	{
		CDC *pDC = m_Screen.GetDC();
		RECT rc;
		m_Screen.GetClientRect(&rc);
		pDC->FillSolidRect(&rc,RGB(102,102,204));
		ReleaseDC(pDC);
	}
	/*if(ProjectActive && Aspect)
	{
		CDC *pDC = m_Screen.GetDC();
		RECT rc1,rc2;
		long xPos, yPos, cx, cy;
		m_Screen.GetClientRect(&rc2);
		if(!pBV) return;
       
		switch(Size)
		{
		case 0:
			{
			//pDC->FillSolidRect(&rc2,RGB(102,102,204));
			pVW->GetWindowPosition(&xPos, &yPos, &cx, &cy);
			
			SetRect(&rc1, rc2.left, rc2.top, rc2.right, yPos);
			pDC->FillSolidRect(&rc1,RGB(102,102,204));
			}
			break;
		case 1:
			{
			//pBV->GetSourcePosition(&xPos, &yPos, &cx, &cy);
			pVW->GetWindowPosition(&xPos, &yPos, &cx, &cy);
			SetRect(&rc1, xPos, yPos, xPos + cx, yPos + cy);
			ClientToScreen(&rc1);
			HRGN rgnClient = CreateRectRgnIndirect(&rc2);
			HRGN rgnVideo  = CreateRectRgnIndirect(&rc1);
			CombineRgn(rgnClient, rgnClient, rgnVideo, RGN_DIFF);
			HBRUSH hbr = GetSysColorBrush(COLOR_BTNFACE);
	        pDC->FillRgn((CRgn*)&rgnClient, (CBrush*)&hbr);
		    DeleteObject(hbr);
			DeleteObject(rgnClient);
			DeleteObject(rgnVideo);
			}
			break;
		}
		ReleaseDC(pDC);
	}*/
Quit:
	
	//if(::IsWindow(m_Slider.m_hWnd)) m_Slider.Invalidate();
	return;
}

BOOL CVideo::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


LRESULT CVideo::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NOTIFY)
	{
		if (wParam == IDC_SPLITTER)
		{	
			SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
			DoResize(pHdr->delta);
		}
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CVideo::DoResize(int delta)
{
	CSplitterControl::ChangePos(&m_Slider,0,delta);
	CSplitterControl::ChangeHeight(&m_Screen, delta, CW_TOPALIGN);
	if(ProjectActive)CenterVideo();
	CSplitterControl::ChangeHeight(&m_explorer, -delta, CW_BOTTOMALIGN);
	Invalidate();
	m_Screen.Invalidate();
	UpdateWindow();
}

void CVideo::OnLButtonDown(UINT nFlags, CPoint point)
{
	CVoDPlayerDlg *dlg = (CVoDPlayerDlg*)AfxGetMainWnd();
	
	CRect rc;
	m_Screen.GetClientRect(&rc);
	if(rc.PtInRect(point))
	{
		if(Focus)
		{
			dlg->SetFocus();
			Focus=FALSE;
			goto Quit;
		}
		Focus=TRUE;
	}
Quit:
	CDialog::OnLButtonDown(nFlags, point);
}
extern int nCount,ntime;
void CVideo::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==TIMERID)
	{
		ReadMediaPosition();    
	}
	
	if(nIDEvent == 113115)//TimerisSet
	{
		ntime++;
		if(ntime>(tWait*10)) 
		{
			TimerisSet = FALSE;
			nCount = 0;
			Play = 1;OnPlay();
			m_Slider.StopBlinking();
			KillTimer(113115);
			ntime=0;
		}
		else 
		{	
			Play = 0;OnPlay();
			
		}
	}

	if(nIDEvent == 112)//TimerisSet
	{
		nCount++;
		if(nCount>5) 
		{
			TimerisSet = FALSE;
			nCount = 0;
			Play = 1;OnPlay();
			m_Slider.StopBlinking();
			KillTimer(112);
		}
		else 
		{	
			//Play = 0;OnPlay();
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CVideo::OnDestroy()
{
	CDialog::OnDestroy();
}

void CVideo::OnCancel()
{
	hWnd = NULL;
	if (hMenu)DestroyMenu(hMenu); 
	CVoDPlayerDlg *dlg = (CVoDPlayerDlg*)AfxGetMainWnd();
	dlg->ProjectOpen = FALSE;
	Sleep(100);
	FreeDirectShow();
	CoUninitialize();
	Sleep(200);
	//CDelete *Delete = new CDelete;
	//Delete->Create(IDD_DELETE,NULL);
	CDelete Delete;
	Delete.DoModal();
	
	/*try {
		_Stats->Close();
	} catch(...)
	{}*/
	dlg->Drag->OnOK();
	CDialog::OnCancel();
	delete []this;
}

void CVideo::OnOK()
{
	DestroyMenu(hMenu); 
	CVoDPlayerDlg *dlg = (CVoDPlayerDlg*)AfxGetMainWnd();
	dlg->ProjectOpen = FALSE;
	FreeDirectShow();
	Sleep(100);
	CoUninitialize();
	Sleep(200);
	CDelete *Delete = new CDelete;
	Delete->Create(IDD_DELETE,NULL);
	CDialog::OnOK();
}

void CVideo::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString html;
	if(pScrollBar->m_hWnd==m_Slider.m_hWnd)
	{
		int Position = m_Slider.GetPos();
		//m_Slider.SetProgressPos(nPos);
		for(unsigned int i=0;i<PageCount;i++)
		{
			if((int)(Position/10)==(int)(Pages[i].Frame/10))
			{
				html.Format("%s",TempPr);
				
				html.Format("%s\\%s",html,Pages[i].PageName);
				m_explorer.Navigate(html,NULL,NULL,NULL,NULL);
			}
		}
		/*int nMax =	m_Slider.GetRangeMax();
		int range = (nMax*ProPos)/ProRange;
		if(nPos>range-10)
		{
			m_Slider.SetPos(range);
		}*/
	//
	}
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
    UpdatePosition(rtNew);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CVideo::OnRatKeepaspectratio()
{
    if(Aspect==0) Aspect=1; else Aspect=0;
	AfxGetApp()->WriteProfileInt("Aspect","ASPECT",Aspect);
	if(Aspect==1)
	{
		CheckMenuItem(hSubMenu,ID_RAT_KEEPASPECTRATIO,MF_CHECKED);
		EnableMenuItem(hSubMenu,ID_RAT_DOUBLESIZE,MF_ENABLED);
	}
	else 
	{
		CheckMenuItem(hSubMenu,ID_RAT_KEEPASPECTRATIO,MF_UNCHECKED);
		EnableMenuItem(hSubMenu,ID_RAT_DOUBLESIZE,MF_GRAYED|MF_DISABLED);
	}
	CenterVideo();
}

void CVideo::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect rc;
	m_Screen.GetClientRect(&rc);
	if(rc.PtInRect(point))
	{
		ClientToScreen(&point);
		TrackPopupMenu(hSubMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON ,
			point.x+10, point.y, 0,this->m_hWnd,NULL);
	}
	CDialog::OnRButtonDown(nFlags, point);
}

void CVideo::OnRatDoublesize()
{
    if(Size==0) Size=1; else Size=0;
	AfxGetApp()->WriteProfileInt("Size","ASPECT",Size);
	if(Size==1) CheckMenuItem(hSubMenu,ID_RAT_DOUBLESIZE,MF_CHECKED);
	else CheckMenuItem(hSubMenu,ID_RAT_DOUBLESIZE,MF_UNCHECKED);
	CenterVideo();
}

BOOL CVideo::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		if(pMsg->wParam == VK_SPACE)
		{
			OnPlay();
			return 0;
		}else
		if(pMsg->wParam == VK_RETURN)
		{
			if(!FullScreen)
			{
				pVW->put_FullScreenMode(OATRUE);
				FullScreen = TRUE;
			}
			else 
			{
				FullScreen = FALSE;
				pVW->put_FullScreenMode(OAFALSE);	
			}
		}else 
		if(pMsg->wParam == 77)
		{
			MuteAudio();
			return FALSE;
		}
		if(pMsg->wParam == 82)
		{
			ResumeAudio();
			return FALSE;
		}
		break;
		
	}

	return CDialog::PreTranslateMessage(pMsg);
}
