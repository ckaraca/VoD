// VoDPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VoDPlayer.h"
#include "VoDPlayerDlg.h"
#include "Video.h"
#include "BufferDlg.h"
#include "Info.h"

extern BOOL Stats;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern TCHAR TempPath[MAX_PATH];
extern int ProjectActive;
extern int	BufferFTP,BufferTCP,BufferUDP;
// CVoDPlayerDlg dialog


CVoDPlayerDlg::CVoDPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVoDPlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Pr = new PROJECTDB;
	ProjectOpen = FALSE;
	
}
CVoDPlayerDlg::~CVoDPlayerDlg()
{
	/*try {
        delete []Drag;
	} catch(...)
	{}*/
}

void CVoDPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_Play);
	DDX_Control(pDX, IDC_BUTTON2, m_Stop);
	DDX_Control(pDX, IDC_BUTTON4, m_Open);
	DDX_Control(pDX, IDC_BUTTON3, m_Info);
}

BEGIN_MESSAGE_MAP(CVoDPlayerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
	ON_COMMAND(ID_POPUP_EXIT, OnPopupExit)
	ON_BN_CLICKED(IDC_BUTTON4, OnOpen)
	ON_COMMAND(ID_POPUP_MINIMIZE, OnDisconnect)
	ON_COMMAND(ID_POPUP_ABOUTVODPLAYER, OnAbout)
	ON_MESSAGE(WM_SETPR, SetPr)
	ON_COMMAND(ID_CONFIGURE_FTPSITE, OnConfigureFtpsite)
	ON_COMMAND(ID_POPUP_STATISTICS, OnPopupStatistics)
	ON_COMMAND(ID_CONNECTIONPROTOCOL_UDP, OnConnectionprotocolUdp)
	ON_COMMAND(ID_CONNECTIONPROTOCOL_RAWFTP, OnConnectionprotocolRawftp)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_COMMAND(ID_CONNECTIONPROTOCOL_TCP, OnConnectionprotocolTcp)
	ON_COMMAND(ID_CONFIGURE_BUFFERSIZE, OnConfigureBuffersize)
	ON_COMMAND(ID_CONNECTIONPROTOCOL_RUDP, OnConnectionprotocolRudp)
END_MESSAGE_MAP()

// CVoDPlayerDlg message handlers
LONG CVoDPlayerDlg::SetPr(WPARAM wParam, LPARAM lParam)
{
    mts = (MTS*)wParam;
	wsprintf(PrID,"%s",mts->DBID);
	delete mts;
	if(GetProjectFromDB(PrID)!=0) goto Restore;
    if(OpenWindows()!=0) goto Restore;
	ProjectOpen = TRUE;
	return 0;
Restore:
	return 1;
}

int CVoDPlayerDlg::OpenWindows(void)
{
	Video = new CVideo;//Video->m_hWnd = NULL;
	Video->Create(IDD_VIDEO,GetDesktopWindow());
	Video->ShowWindow(SW_SHOW);
	Video->UpdateWindow();
	Drag = new CDrag;
	Drag->Create(IDD_DIALOGBAR,GetDesktopWindow());
	Drag->ShowWindow(SW_SHOW);
	Drag->UpdateWindow();
	
	
	return 0;
}

int CVoDPlayerDlg::GetProjectFromDB(TCHAR *PrID)
{
	TCHAR sz[1024],szError[512];int err;
	wsprintf(sz,"SELECT * FROM vod.project WHERE id=\'%s\'",PrID);
	err = mysql_real_query(Log.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)Log.myData)) ;
		AfxMessageBox(szError);
		return -1;
	}
	Log.res = mysql_store_result( Log.myData );
	while ( Log.row = mysql_fetch_row(Log.res ) ) 
	{
		strcpy(Pr->id,Log.row[0]);
		strcpy(Pr->name,Log.row[1]);
		strcpy(Pr->chid,Log.row[2]);
		strcpy(Pr->subject,Log.row[3]);
		strcpy(Pr->Projectdesc,Log.row[4]);
		strcpy(Pr->VideoPath,Log.row[5]);
		strcpy(Pr->VideoName,Log.row[6]);
		strcpy(Pr->FTPID, Log.row[7]);
	}
	return 0;
}

extern HMENU		m_hMenu;			// Handle to associated menu

extern CString Connection;
extern int	Buffer;

BOOL CVoDPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->SetWindowText("VoD Player - Control panel");
	CDC *cD = GetDC();
		//Handle to the region we'll use to combine
	bitmap.LoadBitmap(IDB_BITMAP2);
	trans = RGB(33,00,00);
	CcD.CreateCompatibleDC(cD);
	GetObject(bitmap,sizeof(bmap),&bmap);
	SelectObject(CcD,bitmap);
	//cD->DeleteDC();
	cD->ReleaseOutputDC();
	Shape(CcD,bmap,combine);
	SetWindowRgn(combine,TRUE);
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog


	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_Play.SetColor(0,RGB(102,102,102),TRUE);
	m_Play.SetIcon(IDI_PLAY1,IDI_PLAY);
	m_Play.SetTooltipText("Press here to play",TRUE);
	m_Stop.SetIcon(IDI_STOP1,IDI_STOP);
	m_Stop.SetTooltipText("Press here to stop",TRUE);
	m_Open.SetIcon(IDI_OPEN1,IDI_OPEN);
	m_Open.SetTooltipText("Press here to load project",TRUE);
	m_Info.SetIcon(IDI_INFO1,IDI_INFO);
	m_Info.SetTooltipText("Info",TRUE);
	m_Info.SetMenu(IDR_MENU1,this->m_hWnd,TRUE);
	UpdateWindow();
	
	HMENU sub =m_Info.GetMenuHandle();
	if(Stats==0)
	{
        CheckMenuItem(sub,ID_POPUP_STATISTICS,MF_UNCHECKED);
	}
	else 
	{
		CheckMenuItem(sub,ID_POPUP_STATISTICS,MF_CHECKED);
	}
		

	// -------------exract
	GetTempPath(MAX_PATH,TempPath);

	Extract();
	// end extract
	CLogin Login(NULL);
	INT_PTR res=-1;
	while(res!=0)
	{
		res = Login.DoModal();
		if(res==IDCANCEL) 
		{
			delete []Pr;
			EndDialog(0);
			return FALSE;
		}

	}

	

	if(Connection == "UDP") CheckMenuRadioItem(sub,ID_CONNECTIONPROTOCOL_RAWFTP,ID_CONNECTIONPROTOCOL_RUDP,ID_CONNECTIONPROTOCOL_UDP,MF_BYCOMMAND);
	else if(Connection == "RAWFTP")CheckMenuRadioItem(sub,ID_CONNECTIONPROTOCOL_RAWFTP,ID_CONNECTIONPROTOCOL_RUDP,ID_CONNECTIONPROTOCOL_RAWFTP,MF_BYCOMMAND);
	else if(Connection == "TCP") CheckMenuRadioItem(sub,ID_CONNECTIONPROTOCOL_RAWFTP,ID_CONNECTIONPROTOCOL_RUDP,ID_CONNECTIONPROTOCOL_TCP,MF_BYCOMMAND);
	else if(Connection == "RUDP") CheckMenuRadioItem(sub,ID_CONNECTIONPROTOCOL_RAWFTP,ID_CONNECTIONPROTOCOL_RUDP,ID_CONNECTIONPROTOCOL_RUDP,MF_BYCOMMAND);
	if(Stats)
	{
		_Stats = new CStats;
		_Stats->Create(IDD_STATS,GetDesktopWindow());
		_Stats->ShowWindow(SW_SHOW);
		_Stats->UpdateWindow();
	}
	
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVoDPlayerDlg::Extract(void)
{
	TCHAR Path[MAX_PATH];
	wsprintf(Path,"%sVoDanm",TempPath);
	ExtractResource(Path,IDR_DATA1);
	wsprintf(Path,"%sVoD.html",TempPath);
	ExtractResource(Path,IDR_DATA2);
}

void CVoDPlayerDlg::Shape(CDC &bmapDC,BITMAP &btmap,HRGN &retcombine)
{
    int x,y,startx=0;
	HRGN temp;
	COLORREF tst;

	trans = GetPixel(bmapDC,0,0);	//Transparent color
	//TCHAR text[255];
	//wsprintf(text,"%x",trans);
	//AfxMessageBox(text);
	retcombine=CreateRectRgn(0,0,0,0);	//We have to make the rgn before we combine to it
	temp = CreateRectRgn(0,0,0,0);		//Temporary region

	for(y=0;y<=btmap.bmHeight-1;y++)	//Loop through height
	{
		for(x=0;x<=btmap.bmWidth-1;x++)	//Loop through width
		{
			tst=GetPixel(bmapDC,x,y);	//Test the pixel thats at the current coordinate

			if(tst!=trans)				//Is it the transparent color
			{							//No it's not, which means we want it to be in the rgn
				startx=x;				//Save the current x coord
				while((x<=btmap.bmWidth)&&((GetPixel(bmapDC,x,y))!=trans))
				{x++;}	//As long as the color isn't the transparent one, keep moving right
				x--;	//Compensate for the pixel we moved to that was transparent
				temp=CreateRectRgn(startx,y,x,y+1);	//Make a rgn based on that info
				CombineRgn(retcombine,retcombine,temp,RGN_OR);	//Combine them
			}
		}		//That's all there is to it, easy huh?
	}
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVoDPlayerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		//BitBlt(hdc,0,0,bmap.bmWidth,bmap.bmHeight,ifaceDC,0,0,SRCCOPY);
		//	EndPaint(hWnd,&ps);
		CDC *dC = GetDC();
		dC->BitBlt(0,0,bmap.bmWidth,bmap.bmHeight,&CcD,0,0,SRCCOPY);
		dC->ReleaseOutputDC();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVoDPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


UINT CVoDPlayerDlg::OnNcHitTest(CPoint point)
{
	return 2;
}

void CVoDPlayerDlg::OnPopupExit()
{
	CloseProject();
	try{
	delete []Video;
	//_Stats->SendMessage(WM_STATS,0,0);
	delete []_Stats;
	delete []Pr;
	delete []Drag;
	} catch(...) {}
	
	EndDialog(0);
}

void CVoDPlayerDlg::CloseProject(void)
{
	try
	{
		if(::IsWindow(Video->m_hWnd))
		{
			ProjectActive = 0;
			Video->OnStop();
			Video->FreeDirectShow();
			//Video->DestroyWindow();
			//
			try
			{
				Video->OnCancel();
				Drag->OnOK();
			}
			catch(...)
			{
				goto Quit;
			}
			try
			{
				Video->EndDialog(0);
			}
			catch(...)
			{
				goto Quit;
			}
		/*if(Stats)
		{
			try
			{
				if(_Stats) delete _Stats;
			}
			catch(...)
			{
			}
		}*/
			try
			{
				if(Video) delete Video;
			}
			catch(...)
			{
				goto Quit;
			}
		}
	}
	catch(...)
	{
		goto Quit;
	}
Quit:
	Extract();
}

void CVoDPlayerDlg::OnOpen()
{
	CProject Project;
    if(ProjectOpen)
	{
		if(AfxMessageBox("Load new project and close this one?",MB_OKCANCEL)==IDCANCEL) return;
		else {
			CloseProject();
			ProjectOpen=FALSE;
			Project.DoModal();
			
		}
	}
	else Project.DoModal();
}

void CVoDPlayerDlg::OnDisconnect()
{
	CloseProject();
	ShowWindow(SW_HIDE);
	EnableWindow(FALSE);
	UpdateWindow();
	Log.Connected = FALSE;
	Log.myData = NULL;
	Log.res = NULL;
	CLogin Login(NULL);
	INT_PTR res=-1;
	while(res!=0)
	{
		res = Login.DoModal();
		if(res==IDCANCEL) 
		{
			EndDialog(0);
			return ;
		}
	}
	EnableWindow(TRUE);
	ShowWindow(SW_SHOW);
	UpdateWindow();
	ProjectOpen = FALSE;
}

void CVoDPlayerDlg::OnAbout()
{
	//CDialog About(IDD_ABOUT);
	//About.DoModal();
	CInfo About;
	About.DoModal();
}

BOOL CVoDPlayerDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	
	return CDialog::PreCreateWindow(cs);
}

void CVoDPlayerDlg::ExtractResource(TCHAR *path,int DATA)
{
	HRSRC hRes = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(DATA),RT_RCDATA);//RT_RCDATA
	DWORD dwSize = SizeofResource(AfxGetResourceHandle() , hRes);
	HGLOBAL MemoryHandle = LoadResource(AfxGetResourceHandle(), hRes);
	if(MemoryHandle != NULL)
	{
		// LockResource returns a BYTE pointer to the raw data in the resource
		try{
            CFile file(path, CFile::modeCreate | CFile::modeWrite);
			file.Write(MemoryHandle,dwSize);
			file.Close();
		} catch (CFileException *e)
		{
			e->ReportError();
			e->Delete();
		}
	}
	FreeResource((HANDLE)hRes);
}


void CVoDPlayerDlg::OnConfigureFtpsite()
{
	AfxMessageBox("This option is disabled in multi-server environments");
	//CNewFtp Ftp;
	//Ftp.DoModal();
}

void CVoDPlayerDlg::OnPopupStatistics()
{
	HMENU sub =m_Info.GetMenuHandle();
	if(Stats==1)
	{
        CheckMenuItem(sub,ID_POPUP_STATISTICS,MF_UNCHECKED);
		Stats = 0;
		AfxGetApp()->WriteProfileInt("Stats","STATS",Stats);
	}
	else 
	{
		CheckMenuItem(sub,ID_POPUP_STATISTICS,MF_CHECKED);
		Stats = 1;
		AfxGetApp()->WriteProfileInt("Stats","STATS",Stats);
	}
}

void CVoDPlayerDlg::OnConnectionprotocolUdp()
{
	HMENU sub =m_Info.GetMenuHandle();
	CheckMenuRadioItem(sub,ID_CONNECTIONPROTOCOL_RAWFTP,ID_CONNECTIONPROTOCOL_RUDP,ID_CONNECTIONPROTOCOL_UDP,MF_BYCOMMAND);
	AfxGetApp()->WriteProfileString("Connection","CON_TYP","UDP");
	Connection = "UDP";
}

void CVoDPlayerDlg::OnConnectionprotocolRawftp()
{
	HMENU sub =m_Info.GetMenuHandle();
	CheckMenuRadioItem(sub,ID_CONNECTIONPROTOCOL_RAWFTP,ID_CONNECTIONPROTOCOL_RUDP,ID_CONNECTIONPROTOCOL_RAWFTP,MF_BYCOMMAND);
	AfxGetApp()->WriteProfileString("Connection","CON_TYP","RAWFTP");
	Connection = "RAWFTP";
}

void CVoDPlayerDlg::OnBnClickedButton1()
{
	try{
	if(IsWindow(Video->m_hWnd)!=0)
	{
	       	Video->OnPlay();
	}
	} catch(...){}
}

void CVoDPlayerDlg::OnBnClickedButton2()
{
	try{
	if(IsWindow(Video->m_hWnd)!=0)
	{
		Video->OnStop();
	}
	}catch(...){}
}

void CVoDPlayerDlg::OnConnectionprotocolTcp()
{
	HMENU sub =m_Info.GetMenuHandle();
	CheckMenuRadioItem(sub,ID_CONNECTIONPROTOCOL_RAWFTP,ID_CONNECTIONPROTOCOL_RUDP,ID_CONNECTIONPROTOCOL_TCP,MF_BYCOMMAND);
	AfxGetApp()->WriteProfileString("Connection","CON_TYP","TCP");
	Connection = "TCP";
}

void CVoDPlayerDlg::OnConfigureBuffersize()
{
	CBufferDlg Buffer;
	Buffer.DoModal();
}

void CVoDPlayerDlg::OnConnectionprotocolRudp()
{
	HMENU sub =m_Info.GetMenuHandle();
	CheckMenuRadioItem(sub,ID_CONNECTIONPROTOCOL_RAWFTP,ID_CONNECTIONPROTOCOL_RUDP,ID_CONNECTIONPROTOCOL_RUDP,MF_BYCOMMAND);
	AfxGetApp()->WriteProfileString("Connection","CON_TYP","RUDP");
	Connection = "RUDP";	
}
