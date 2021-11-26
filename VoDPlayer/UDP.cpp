// UDP.cpp : implementation file
//

#include "stdafx.h"
#include "VoDPlayer.h"
#include "UDP.h"
#include "Stats.h"

extern CStats *_Stats;
extern BOOL Stats;

#define BUFFER	1024
// CUDP
extern int	BufferUDP;
extern CSql	Log;
extern TCHAR TempPath[MAX_PATH];
extern PROJECTDB *Pr;
extern HWND hWnd;
extern CString VideoStream;
extern CString VideoPath;
IMPLEMENT_DYNCREATE(CUDP, CWinThread)

CUDP::CUDP()
: speed(0)
, m_percent(0)
, PacketNumber(0)
{
}

CUDP::~CUDP()
{
}

BOOL CUDP::InitInstance()
{
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
	_Stats->SetText(1,"UDP/IP");
	_Stats->SetText(2,"0 bps");
	_Stats->SetText(3,"0 %");
	_Stats->SetText(4,"N/A - N/A");
	_Stats->SetText(5,"0 kB");
	_Stats->SetText(6,"0 sec");
	_Stats->SetText(7,"0");
	_Stats->PostMessage(WM_SETHIST,1,0);
}
	AfxSocketInit();
	HANDLE hFile,hMemFile=NULL;
	char* dataPointer = NULL;
	TCHAR sz[1024],szError[512];
	TCHAR Host[220];
	wsprintf(sz,"Select * from vod.ftp where ftp.id=%s",Pr->FTPID);
	int err,i;
	TCHAR system[100],sysPath[10];
	_tcsncpy(sysPath,TempPath,3);
	sysPath[3] = '\0';
	GetVolumeInformation(sysPath,NULL,0,NULL,NULL,NULL,system,sizeof(system));
	err = mysql_real_query(Log.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)Log.myData)) ;
		AfxMessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
		return 0;
	}
	Log.res = mysql_store_result( Log.myData );
	i = (int) mysql_num_rows( Log.res );
if(Stats)
{
	_Stats->SetText(0,"Getting server info");
}
	if(i==0)
	{
		i = AfxMessageBox("There are no ftp servers found on the system\n Press OK to add one.\n TCP connection uses the same IP of the FTP server.",MB_OKCANCEL |MB_ICONASTERISK);
		if(i == IDOK)
		{
			CNewFtp ftp;
			ftp.DoModal();
		}
		else return 0;
	}
	if(i>=1)
	{
		while(Log.row = mysql_fetch_row( Log.res ))
		{
			sprintf(Host,Log.row[1]);
		}
	}
	int addr_len = sizeof(struct sockaddr);
	m_Socketin.sin_family = AF_INET;
	m_Socketin.sin_port = htons(22000);
	unsigned long Addr;
	Addr = inet_addr(Host);
	m_Socketin.sin_addr.S_un.S_addr = Addr;
	m_Socket=socket(AF_INET,SOCK_DGRAM,0);
	if(m_Socket == INVALID_SOCKET)
	{
		TCHAR *err = GetError();
if(Stats)
{
	_Stats->SetText(0,err);
} else
		
		AfxMessageBox(err);
		delete []err;
		goto Quit;
	}
	
	memset(&(m_Socketin.sin_zero), '\0', 8);
	int timeo = 4000;
	int optlen = sizeof(timeo);
if(Stats)
{
	_Stats->SetText(0,"SO_RCVTIMEO = 2000 ms");
}
	if(setsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeo,optlen)<0)
	{
		TCHAR *err = GetError();
		AfxMessageBox(err);
		delete []err;
		goto Quit;
	}
if(Stats)
{
	_Stats->SetText(0,"Establishing connectionless comm.");
}
	sendto(m_Socket,"cem\r\n",5,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
	int Num=0;
	Num = recvfrom(m_Socket,Message,512,0,(struct sockaddr *)&m_Socketin,&addr_len);
	if(Num==0)
	{
if(Stats)
{
	_Stats->SetText(0,"UDP Server is down");
}
		AfxMessageBox("UDP Server is down");
		goto Quit;
	} else if(Num<0)
	{
		TCHAR *err = GetError();
if(Stats)
{
	_Stats->SetText(0,err);
}else
AfxMessageBox(err);
		delete []err;
		goto Quit;
	}
	Message[Num]='\0';
	if(strcmp(Message,"Welcome\r\n")!=0)
	{
		goto Quit;
	}
if(Stats)
{
	_Stats->SetText(0,"Welcome message received,sending password");
}

	sendto(m_Socket,"2 Vo\0",5,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
	Num = recvfrom(m_Socket,Message,512,0,(struct sockaddr *)&m_Socketin,&addr_len);
	Message[Num]='\0';
	if(strcmp(Message,"User accepted\r\n")!=0)
	{
if(Stats)
{
	_Stats->SetText(0,"Wrong user or password");
}else
		AfxMessageBox("Wrong user or password");
		goto Quit;
	}
	TCHAR Mes[BUFFER];
	wsprintf(Mes,"3 %s\r\n",Pr->VideoPath);
	sendto(m_Socket,Mes,(int)strlen(Mes)+1,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
	Num=recvfrom(m_Socket,Message,512,0,(struct sockaddr *)&m_Socketin,&addr_len);
	Message[Num]='\0';
	wsprintf(Mes,"Directory set to %s\r\n",Pr->VideoPath);
	if(strcmp(Message,Mes)!=0)
	{
		AfxMessageBox("Project directory cannot be found on the server!");
		goto Quit;
	}
	wsprintf(Mes,"7 %s\r\n",Pr->VideoName);
	sendto(m_Socket,Mes,(int)strlen(Mes)+1,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
	Num=recvfrom(m_Socket,Message,512,0,(struct sockaddr *)&m_Socketin,&addr_len);
	Message[Num]='\0';
	unsigned __int64 size = _atoi64(Message);
if(Stats)
{
	//wsprintf(sz,"%d.%.3f",size/1024,size%1024);
	sprintf(sz,"%.f kB - %d Bytes",(float)size/1024,BufferUDP);
	_Stats->SetText(4,sz);
}
	if(size==0)
	{
		wsprintf(szError,"The project media cannot be found on the server");
        AfxMessageBox(szError);
		goto Quit;
	}
	int KB = (int)size/1024;
	::SendMessage(hWnd,WM_SETSL,KB,0);
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
		FILE_ATTRIBUTE_NORMAL,
		//        FILE_ATTRIBUTE_TEMPORARY | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED | FILE_FLAG_RANDOM_ACCESS,//flags and attr
        NULL);//template file
		SetFilePointer(hFile, DWORD(size & 0xFFFFFFFF),NULL,FILE_BEGIN);
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

	// start partial download from here
	if(_tcsstr(Pr->VideoName, TEXT(".avi")))
	{
if(Stats)
{
	_Stats->SetText(0,"Receiving AVI header");
}
		DWORD rest =(DWORD)(size & 0xFFFFFFFF)-65535;
		DWORD kalan = (DWORD)(size & 0xFFFFFFFF)- rest;
		char *datapointer1 = (char*)MapViewOfFile(hMemFile,FILE_MAP_ALL_ACCESS,0,0,0);
		datapointer1 += rest;
		TCHAR Mes[1024];
		wsprintf(Mes,"6 %s\r\n",Pr->VideoName);
		sendto(m_Socket,Mes,(int)strlen(Mes)+1,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
		Sleep(100);
		wsprintf(Mes,"5 %lu\r\n",rest);
		sendto(m_Socket,Mes,(int)strlen(Mes)+1,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
		//Sleep(100);
		int Read = BufferUDP;
		char *buff1 = new char[BufferUDP];
       	DWORD TotalRead = 0;
		while(TotalRead < kalan)
		{
			Read = recvfrom(m_Socket,buff1,BufferUDP,0,(struct sockaddr *)&m_Socketin,&addr_len);
			memcpy(datapointer1,buff1,Read);
			datapointer1+=Read;
			TotalRead += Read;
		}
		delete []buff1;
	}
if(Stats)
{
	_Stats->SetText(3,"0 %");
}
	ULONG nTotalRead = 0;
//	ULONG nPartialRead = 0;
	UINT nCount = 0;
	char *buffer = new char[BufferUDP];
	unsigned int nRead = BufferUDP;
	int opt = 10*BufferUDP;
	optlen = sizeof(opt);
	if(setsockopt(m_Socket,SOL_SOCKET,SO_RCVBUF,(const char*)&opt,optlen)<0) 
	{
		TCHAR *err = GetError();
		AfxMessageBox(err);
		delete []err;
		goto Quit;
	}

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
	wsprintf(Mes,"4 %s\r\n",Pr->VideoName);
	sendto(m_Socket,Mes,(int)strlen(Mes)+1,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
	
if(Stats)
{
	_Stats->SetText(3,"0 %");
}
if(Stats)
{
	_Stats->SetText(0,"Starting to download video");
}
DWORD Start = GetTickCount();
DWORD Start1;
	char *buf1 = new char[gr*2];
	char *bufpt = buf1;
	UINT bfsize = gr*2;
	int chance = 0; // chance before exiting on buffering errors
	while (nTotalRead < size-1)
	{
		PacketNumber++;
		// read remote data into buffer
		if(hWnd==NULL) goto Quit;
		Start1 = GetTickCount();
		nRead = recvfrom(m_Socket,buffer,BufferUDP,0,(struct sockaddr *)&m_Socketin,&addr_len);
		if(nRead<=0)
		{
			TCHAR *err = GetError();
			AfxMessageBox(err);
			delete []err;
			goto Quit1;
		} else if(nRead==0) goto Quit1;
		
		if(nRead<=bfsize)
		{
			memcpy(bufpt,buffer,nRead);
			bfsize -= nRead;
			bufpt += nRead;
			nTotalRead += nRead;
		} else // not enough space, fill memmap
		{
			memcpy(bufpt,buffer,bfsize); // artýk var þimdi
			bufpt = buf1;
			// memory map
			memcpy(dataPointer,buf1,gr*2);
			nCount++;
			if(dataPointer!=NULL) UnmapViewOfFile(dataPointer);
			dataPointer=(char*)MapViewOfFile(hMemFile,FILE_MAP_ALL_ACCESS,0,nCount*gr*2,gr*2);
			// End memory map
            char *pt = buffer;
			pt += bfsize;
			try {
                memcpy(bufpt,pt,(nRead-bfsize));
				//CopyMemory(
			}catch(...)
			{
				AfxMessageBox("A memory exception occured.\nTerminating UDP thread!");
				if(chance>50) 
				{
					
					goto Quit1;
				} else chance++;
			}
			/// burada
			bufpt += (nRead-bfsize);
			bfsize = (gr*2)-(nRead-bfsize);
			nTotalRead += nRead;
		}

		if(set)
		{
			if(nTotalRead>10*128768 && !_tcsstr(Pr->VideoName, TEXT(".avi")))
			{
				double vs = (double)(GetTickCount()-Start);
				int speed1 = (int)((nTotalRead*8)/vs);
				set = FALSE;
				::PostMessage(hWnd,WM_PLAY,(int)((size*8)/1000),speed1);
			}
			if(nTotalRead>size/20 && _tcsstr(Pr->VideoName, TEXT(".avi")))
			{
				double vs = (double)(GetTickCount()-Start);
				int speed1 = (int)((nTotalRead*8)/vs);
				set = FALSE;
				::PostMessage(hWnd,WM_PLAY,(int)((size*8)/1000),speed1);
			}
		}
		
		int nPos = (int)(nTotalRead/(1024));
		::PostMessage(hWnd,WM_SLI,nPos,0);
if(Stats)
{
	double vs = (double)(GetTickCount()-Start);
	double vs1 = (double)(GetTickCount()-Start1);
	TCHAR *sz1 = new TCHAR[200];
	sprintf(sz1,"%.f ms",vs);
	//_Stats->SetText(6,sz);
	_Stats->PostMessage(WM_SETSTATS,6,(LPARAM)sz1);
	if(vs==0) vs=0.1;
	//double speed1 = (double)((nTotalRead*8)/(vs*1000));
	double speed1 = (double)((nRead*8)/(vs1*1000));
	if(speed!=speed1)	// do not print same value
	{
		speed=speed1;
		TCHAR *sz2 = new TCHAR[200];
        sprintf(sz2,"%.2f Mbps",speed);
		//_Stats->SetText(2,sz);
		_Stats->PostMessage(WM_SETHIST,(WPARAM)speed,0);
		_Stats->PostMessage(WM_SETSTATS,2,(LPARAM)sz2);
	}
	double percent = (double)((double)100*nTotalRead/size);
	if(m_percent!=percent)
	{
		m_percent = percent;
		TCHAR *sz3 = new TCHAR[200];
		sprintf(sz3,"%.1f %%",m_percent);
		//_Stats->SetText(3,sz);
		_Stats->PostMessage(WM_SETSTATS,3,(LPARAM)sz3);
	}
	TCHAR *sz4 = new TCHAR[200];
	wsprintf(sz4,"%d",PacketNumber);
	//_Stats->SetText(7,sz);
	
	_Stats->PostMessage(WM_SETSTATS,7,(LPARAM)sz4);
	TCHAR *sz5 = new TCHAR[200];
	wsprintf(sz5,"%lu kB",(ULONG)(nTotalRead/1024));
	//_Stats->SetText(5,sz);
	_Stats->PostMessage(WM_SETSTATS,5,(LPARAM)sz5);
	
}
	}
Quit1:
	if(Stats)
{
	_Stats->SetText(0,"Finished downloading video");
	for(unsigned i=0;i<100;i++)
	{
		_Stats->PostMessage(WM_SETHIST,(WPARAM)0.1,0);
	}
}
	
	delete []buffer;
Quit:
	//::PostMessage(hWnd,WM_PLAY,0,0);
	closesocket(m_Socket);
	try{
		if(dataPointer!=NULL) UnmapViewOfFile(dataPointer);
		//AfxMessageBox("unmap");
		if(hMemFile!=NULL) CloseHandle(hMemFile);
		
	} catch(...)
	{}
	/*double vs = (double)(GetTickCount()-Start);
	TCHAR *sz1 = new TCHAR[200];
	sprintf(sz1,"%.f ms",vs);
	AfxMessageBox(sz1,MB_OK);*/
	
	return TRUE;
}

int CUDP::ExitInstance()
{
	
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUDP, CWinThread)
END_MESSAGE_MAP()


// Gets Socket error

TCHAR * CUDP::GetError(void)
{
	TCHAR * errt = new TCHAR[256];
	int err = WSAGetLastError();
	wsprintf(errt,"Error #:%d",err);
	return errt;
}
