// VoDTCPServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VoDTCPServer.h"
#include "MainFrm.h"

#include "VoDTCPServerDoc.h"
#include "VoDTCPServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern TCHAR ServerPathName[];
extern TCHAR ServerPath[];
// CVoDTCPServerApp

BEGIN_MESSAGE_MAP(CVoDTCPServerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_SETSERVERPATH, OnFileSetserverpath)
END_MESSAGE_MAP()


// CVoDTCPServerApp construction

CVoDTCPServerApp::CVoDTCPServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CVoDTCPServerApp object

CVoDTCPServerApp theApp;

// CVoDTCPServerApp initialization

BOOL CVoDTCPServerApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Video on Demand Server"));
	CString Path,aPath;
	Path = this->GetProfileString("Path","SP","NULL");
	if(Path == "NULL")
	{
		this->BrowseFolder(ServerPath,ServerPathName);
		this->WriteProfileString("Path","SP",ServerPathName);
		this->WriteProfileString("Path","ASP",ServerPath);
	}
	aPath = this->GetProfileString("Path","ASP");
	wsprintf(ServerPath,"%s",aPath);
	wsprintf(ServerPathName,"%s",Path);

	//LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CVoDTCPServerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CVoDTCPServerView));
	AddDocTemplate(pDocTemplate);
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CVoDTCPServerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
BOOL CVoDTCPServerApp::BrowseFolder(LPSTR Folder, LPSTR Name)
{
	BROWSEINFO lpbi = {0};
	lpbi.hwndOwner = NULL;
	lpbi.pidlRoot = NULL;
	lpbi.pszDisplayName = Name;
	lpbi.lpszTitle = "Please Select the server path that the media files are located";
	lpbi.ulFlags = BIF_NONEWFOLDERBUTTON;
	lpbi.lpfn = NULL;
	LPITEMIDLIST pidl = SHBrowseForFolder ( &lpbi );
	if ( pidl != 0 )
    {
        // get the name of the folder and put it in path
        SHGetPathFromIDList ( pidl, Folder );

        //Set the current directory to path
        SetCurrentDirectory ( Folder );

        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
    } else return -1;
	return 0;
}


void CVoDTCPServerApp::OnFileSetserverpath()
{
    CString Path,aPath;
	this->BrowseFolder(ServerPath,ServerPathName);
	this->WriteProfileString("Path","SP",ServerPathName);
	this->WriteProfileString("Path","ASP",ServerPath);
	aPath = this->GetProfileString("Path","ASP");
	Path = this->GetProfileString("Path","SP","NULL");
	wsprintf(ServerPath,"%s",aPath);
	wsprintf(ServerPathName,"%s",Path);
}

