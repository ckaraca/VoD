// FtpV.cpp : implementation file
//

#include "stdafx.h"
#include "VoDPlayer.h"
#include "FtpV.h"
#include "Sql.h"
#include "NewFtp.h"
#include "Wininet.h"
#include "Stats.h"

extern CStats *_Stats;
extern BOOL Stats;

extern CSql	Log;
extern PROJECTDB *Pr;

extern int	BufferFTP,BufferTCP;

// CFtpV

extern CString VideoPath;
IMPLEMENT_DYNCREATE(CFtpV, CWinThread)

CFtpV::CFtpV()
: speed(0)
, m_percent(0)
, PacketNumber(0)
{
	m_bAutoDelete = TRUE;
}

CFtpV::~CFtpV()
{
}
extern HWND hWnd;
BOOL CFtpV::InitInstance()
{
	// this is here
if(Stats)
{
	_Stats->SetGrText(0,"Status");
	_Stats->SetGrText(1,"Connection Type");
	_Stats->SetGrText(2,"Overall Speed");
	_Stats->SetGrText(3,"Download percent");
	_Stats->SetGrText(4,"Video size - Packet size");
	_Stats->SetGrText(5,"Downloaded KB");
	_Stats->SetGrText(6,"Time elapsed");
	_Stats->SetGrText(7,"Receiving packet #");
	_Stats->SetText(0,"Initing socket");
	_Stats->SetText(1,"FTP");
	_Stats->SetText(2,"0 bps");
	_Stats->SetText(3,"0 %");
	_Stats->SetText(4,"N/A - N/A");
	_Stats->SetText(5,"0 kB");
	_Stats->SetText(6,"0 sec");
	_Stats->SetText(7,"0");
	_Stats->PostMessage(WM_SETHIST,1,0);
}
	TCHAR sz[1024],szError[512];
	wsprintf(sz,"Select * from vod.ftp where ftp.id=%s",Pr->FTPID);
	int err,i;
	err = mysql_real_query(Log.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)Log.myData)) ;
		AfxMessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
		ExitInstance();
	}
	Log.res = mysql_store_result( Log.myData );
	i = (int) mysql_num_rows( Log.res );
if(Stats)
{
	_Stats->SetText(0,"Getting server info");
}
	if(i==0)
	{
		i = AfxMessageBox("There are no ftp servers found on the system\n Press OK to add one.\n You will also be able to add one from the menu.",MB_OKCANCEL |MB_ICONASTERISK);
		if(i == IDOK)
		{
			CNewFtp ftp;
			ftp.DoModal();
		}
		else ExitInstance();
	}
	// FTPTREE-specific initialization follows...
	m_pFtpConnection = NULL;
	m_pInetSession = new CInternetSession("VoDFtp", 1, PRE_CONFIG_INTERNET_ACCESS);
	if (!m_pInetSession)
	{
		AfxMessageBox("Cannot open an Internet Session.  Please check your internet configuration.", MB_OK);
		ExitInstance();
	}
	/*if(i>1)
	{
		AfxMessageBox("More than one ftp server configuration not implemented yet\nUsing last server",MB_OK);
	}*/
	if(i>=1)
	{
		while(Log.row = mysql_fetch_row( Log.res ))
		{
			sprintf(Host,Log.row[1]);
			sprintf(Port,Log.row[2]);
			sprintf(User,Log.row[3]);
			sprintf(Pass,Log.row[4]);
		}
	}

	Connect();	
	
	
	return TRUE;
}
extern TCHAR TempPath[];
extern CString VideoStream;
void CFtpV::Connect()
{
	if (m_pFtpConnection != NULL)
		m_pFtpConnection->Close();
	delete m_pFtpConnection;
	CInternetFile* pInternetFile;//,*pInternetFile1;
	m_pFtpConnection = NULL;
	INTERNET_PORT nPort=atoi(Port);
	DWORD dwServiceType = INTERNET_SERVICE_FTP;
	CString strObject;
	CString strServerName;
	//CFile m_File;

	TCHAR system[100],sysPath[10];
	_tcsncpy(sysPath,TempPath,3);
	sysPath[3] = '\0';
	GetVolumeInformation(sysPath,NULL,0,NULL,NULL,NULL,system,sizeof(system));
	if (!AfxParseURL(Host, dwServiceType, strServerName, strObject, nPort))
	{
		// try adding the "ftp://" protocol
		CString strFtpURL = _T("ftp://");
		strFtpURL += Host;

		if (!AfxParseURL(strFtpURL, dwServiceType, strServerName, strObject, nPort))
		{
			AfxMessageBox("Please enter a valid FTP URL", MB_OK);
			ExitInstance();
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
			ExitInstance();
		}
	}
	else
	{
		AfxMessageBox("Please enter a valid FTP URL", MB_OK);
		return;
	}
	CFtpFileFind finder(m_pFtpConnection);
	//AfxMessageBox(Pr->VideoPath);
	BOOL bWorking = finder.FindFile(Pr->VideoPath);
	if (GetLastError() == ERROR_NO_MORE_FILES)
	{
		AfxMessageBox("The Project you requested cannot be found on this server");
		ExitInstance();
		return;
	}
	try
	{
		m_pFtpConnection->SetCurrentDirectory(Pr->VideoPath);
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
		ExitInstance();
	}
	
	// Find video size now
	try
	{
		bWorking = finder.FindFile(_T(Pr->VideoName));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			m_Size = finder.GetLength();
			int KB = (int)m_Size/1024;
			::SendMessage(hWnd,WM_SETSL,KB,0);
		}
		finder.Close();
	} catch ( CInternetException *e)
	{
		e->ReportError();
		e->Delete();
		ExitInstance();
	}

if(Stats)
{
	_Stats->SetText(0,"Establishing connection");
}
	try 
	{
		if(strcmp(system,"NTFS")==0) VideoStream.Format("%s:%s",VideoPath,"stream");
		else 
		VideoStream.Format("%s",VideoPath);
		CFile cem;
		cem.Open(VideoPath,CFile::modeCreate|CFile::modeReadWrite,NULL);
		cem.Write("Cem KARACA :)\n\r\0",16);
		cem.Close();
		hFile=CreateFile(VideoStream,//name of the file
        GENERIC_READ | GENERIC_WRITE,//desired access
        FILE_SHARE_READ,//share mode
        NULL,//security attributes
        OPEN_ALWAYS,//creation disposition
        FILE_ATTRIBUTE_NORMAL,//flags and attr
        NULL);//template file
		SetFilePointer(hFile, DWORD(m_Size & 0xFFFFFFFF),NULL,FILE_BEGIN);
		SetEndOfFile(hFile);
		SetFilePointer(hFile,0,NULL,FILE_BEGIN);
		hMemFile = CreateFileMapping(hFile,//handle
        NULL,//security
        PAGE_READWRITE,//flProtect
        0,0,//max size
        NULL);//name
		CloseHandle(hFile);
	
	}
	catch (CFileException *e)
	{
		e->ReportError();
		e->Delete();
		goto Quit;
	}
	// use new for avi files
	ULONG nTotalRead = 0;
	// start avi from here
	if(_tcsstr(Pr->VideoName, TEXT(".avi")))
	{
if(Stats)
{
	_Stats->SetText(0,"Receiving AVI header");
}
		//m_pFtpConnection->Command("REST 0");
		// m_Size -65535 might be enough
		DWORD rest =(DWORD)(m_Size & 0xFFFFFFFF)-65535;
		DWORD kalan = (DWORD)(m_Size & 0xFFFFFFFF)- rest;
		datapointer1 = (char*)MapViewOfFile(hMemFile,FILE_MAP_ALL_ACCESS,0,0,0);
		datapointer1 += rest;

		SOCKET m_Client;
		m_Client = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(m_Client==INVALID_SOCKET)
			return;
		unsigned long Addr;
		struct sockaddr_in server;
		Addr = inet_addr(Host);
		server.sin_addr.S_un.S_addr = Addr;
		server.sin_family=AF_INET;
		server.sin_port=htons(21000);
		if(connect(m_Client,(struct sockaddr*)&server,sizeof(server)))
		{
			closesocket(m_Client);
			return ;
		}
		char *buff = new char[BufferTCP];
		int z;
		z=recv(m_Client,buff,BufferTCP,0);
		buff[z]='\0';
		if(strcmp(buff,"0 Server Ready\r\n")!=0)
		{
			delete []buff;
			closesocket(m_Client);
			return ;
		}
		send(m_Client,"2 Vo\0",5,0);
		z=recv(m_Client,buff,BufferTCP,0);
		buff[z]='\0';
		if(strcmp(buff,"User accepted\r\n")!=0)
		{
			closesocket(m_Client);
			return ;
		}
		TCHAR Mes[1024];
		wsprintf(Mes,"3 %s\r\n",Pr->VideoPath);
		send(m_Client,Mes,sizeof(Mes),0);
		z=recv(m_Client,buff,BufferTCP,0);
		buff[z]='\0';
		wsprintf(Mes,"Directory set to %s\r\n",Pr->VideoPath);
		if(strcmp(buff,Mes)!=0)
		{
			AfxMessageBox("Directory cannot be set");
			closesocket(m_Client);
			return ;
		}
		wsprintf(Mes,"6 %s\r\n",Pr->VideoName);
		send(m_Client,Mes,sizeof(Mes),0);
		Sleep(100);
		wsprintf(Mes,"5 %lu\r\n",rest);
		send(m_Client,Mes,sizeof(Mes),0);
		Sleep(100);
		int Read = BufferTCP;
		char *buff1 = new char[BufferTCP];
       	DWORD TotalRead = 0;
		while(TotalRead < kalan)
		{
			Read = recv(m_Client,buff1,BufferTCP,0);
			memcpy(datapointer1,buff1,Read);
			datapointer1+=Read;
			TotalRead += Read;
		}
		delete []buff1;
		delete []buff;
		
		closesocket(m_Client);
	}
if(Stats)
{
	_Stats->SetText(3,"0 %");
}
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);
	UnmapViewOfFile(datapointer1);

	try
	{
		pInternetFile = m_pFtpConnection->OpenFile(Pr->VideoName);
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
		goto Quit;
	}
	char *buffer = new char[BufferFTP];
	unsigned int nRead = BufferFTP;
	ULONG nPartialRead = 0;
	UINT nCount = 0;

	BOOL set = TRUE;
	SYSTEM_INFO sy;
	GetSystemInfo(&sy);
	DWORD gr = sy.dwAllocationGranularity;
	
	dataPointer=(char*)MapViewOfFile(hMemFile,FILE_MAP_ALL_ACCESS,0,0,gr*2);
	if(dataPointer==NULL)
	{
		TCHAR szBuf[512];
		DWORD dw = GetLastError(); 
		sprintf(szBuf, "Failed: Error code: %u\n",  dw); 
		AfxMessageBox(szBuf);
		goto Quit;
	}
if(Stats)
{
	_Stats->SetText(0,"Started to downloading video");
}
int Read1 = 0;
	DWORD Start = GetTickCount();
	do
	{
		if(hWnd==NULL) goto Quit;
		// read remote data into buffer
		nRead = pInternetFile->Read(buffer, BufferFTP);
		// write buffer to data file
		memcpy(dataPointer,buffer,nRead);
		dataPointer+=nRead;
		nTotalRead += nRead;
		nPartialRead += nRead;
		if(nPartialRead>=gr)
		{
			nCount++;
            if(dataPointer!=NULL) UnmapViewOfFile(dataPointer);
			dataPointer=(char*)MapViewOfFile(hMemFile,FILE_MAP_ALL_ACCESS,0,nCount*gr,gr*2);
			//AfxMessageBox("passed");
			if(dataPointer==NULL)
			{
				dataPointer=(char*)MapViewOfFile(hMemFile,FILE_MAP_ALL_ACCESS,0,nCount*gr,gr);
				if(dataPointer==NULL)
				{
					goto Quit1;
				}
			}
			nPartialRead = 0;
		}
		
		if(set)
		{
			if(nTotalRead>65535 && !_tcsstr(Pr->VideoName, TEXT(".avi")))
			{
				double vs = (double)(GetTickCount()-Start);
				int speed1 = (int)((nTotalRead*8)/vs);
				set = FALSE;
				::PostMessage(hWnd,WM_PLAY,(int)((m_Size*8)/1000),speed1);
			}
			if(nTotalRead>m_Size/10 && _tcsstr(Pr->VideoName, TEXT(".avi")))
			{
				double vs = (double)(GetTickCount()-Start);
				int speed1 = (int)((nTotalRead*8)/vs);
				set = FALSE;
				::PostMessage(hWnd,WM_PLAY,(int)((m_Size*8)/1000),speed1);
			}
		}
		int nPos = (int)(nTotalRead/(1024));
		::PostMessage(hWnd,WM_SLI,nPos,0);
		PacketNumber++;
if(Stats)
{
	double vs = (double)(GetTickCount()-Start);
	TCHAR *sz1 = new TCHAR[200];
	sprintf(sz1,"%.f ms",vs);
	_Stats->PostMessage(WM_SETSTATS,6,(LPARAM)sz1);
	if(vs==0) vs=0.1;
	double speed1 = (double)((nTotalRead*8)/(vs*1000));
	if(speed!=speed1)	// do not print same value
	{
		speed=speed1;
		TCHAR *sz2 = new TCHAR[200];
        sprintf(sz2,"%.2f Mbps",speed);
		_Stats->PostMessage(WM_SETSTATS,2,(LPARAM)sz2);
		_Stats->PostMessage(WM_SETHIST,(WPARAM)speed,0);
	}
	double percent = (double)((double)100*nTotalRead/m_Size);
	if(m_percent!=percent)
	{
		m_percent = percent;
		TCHAR *sz3 = new TCHAR[200];
		sprintf(sz3,"%.1f %%",m_percent);
		_Stats->PostMessage(WM_SETSTATS,3,(LPARAM)sz3);
	}
	if(Read1!= nRead)
	{
		Read1 = nRead;
		TCHAR *sz4 = new TCHAR[200];
		sprintf(sz4,"%.f kB - %d Bytes",(float)m_Size/1024,Read1);
		_Stats->PostMessage(WM_SETSTATS,4,(LPARAM)sz4);

	}
	TCHAR *sz5 = new TCHAR[200];
	wsprintf(sz5,"%d",PacketNumber);
	_Stats->PostMessage(WM_SETSTATS,7,(LPARAM)sz5);
	TCHAR *sz6 = new TCHAR[200];
	wsprintf(sz6,"%lu kB",(ULONG)(nTotalRead/1024));
	_Stats->PostMessage(WM_SETSTATS,5,(LPARAM)sz6);
	
}
	}while (nRead == BufferFTP);
Quit1:
	if(Stats)
{
	_Stats->SetText(0,"Finished downloading video");
	for(unsigned i=0;i<100;i++)
	{
		_Stats->PostMessage(WM_SETHIST,(WPARAM)0.1,0);
	}
}
		
Quit:
	//if(_tcsstr(Pr->VideoName, TEXT(".avi"))) ::PostMessage(hWnd,WM_PLAY,0,0);

if(Stats)
{
	for(unsigned i=0;i<100;i++)
	{
		_Stats->PostMessage(WM_SETHIST,(WPARAM)0.1,0);
	}
}
	delete []buffer;
	UnmapViewOfFile(dataPointer);
	CloseHandle(hMemFile);
	/*double vs = (double)(GetTickCount()-Start);
	TCHAR *sz1 = new TCHAR[200];
	sprintf(sz1,"%.f ms",vs);
	//AfxMessageBox(sz1,MB_OK);*/
	return;
}

void CFtpV::GetLastResponse(string& strResponse)
{
	DWORD dwError;
	TCHAR tpszBuff[2*1024];
	DWORD dwBuffLen = 2*1024;
	BOOL bRet = ::InternetGetLastResponseInfo(&dwError, tpszBuff, &dwBuffLen); 
	strResponse = tpszBuff;
}

void CFtpV::Warn(TCHAR *TextToWarn)
{
	AfxMessageBox(TextToWarn);
}

int CFtpV::ExitInstance()
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
	return CWinThread::ExitInstance();
}

DWORD CFtpV::ResumeThread()
{
	return 0;
}
void CFtpV::OnThreadExit(WPARAM wParam, LPARAM lParam)
{
	UnmapViewOfFile(dataPointer);
	CloseHandle(hMemFile);
	this->ExitInstance();
	return ;
}

BEGIN_MESSAGE_MAP(CFtpV, CWinThread)
	ON_THREAD_MESSAGE(WM_EXITTHREAD,OnThreadExit)
END_MESSAGE_MAP()


// CFtpV message handlers
