// UDP.cpp : implementation file
//

#include "stdafx.h"
#include "VoDPlayer.h"
#include "RUDP.h"
#include "Stats.h"
#define BUFFER	1024
// CRUDP
extern CStats *_Stats;
extern BOOL Stats;
extern int	BufferUDP;
extern CSql	Log;
extern TCHAR TempPath[MAX_PATH];
extern PROJECTDB *Pr;
extern HWND hWnd;
extern CString VideoStream;
extern CString VideoPath;
IMPLEMENT_DYNCREATE(CRUDP, CWinThread)

CRUDP::CRUDP()
: speed(0)
, m_percent(0)
, PacketNumber(0)
{
}

CRUDP::~CRUDP()
{
}

BOOL CRUDP::InitInstance()
{
	BOOL TimeOut= FALSE;
if(Stats)
{
	_Stats->SetGrText(0,"Status");
	_Stats->SetGrText(1,"Connection Type");
	_Stats->SetGrText(2,"Overall Speed");
	_Stats->SetGrText(3,"Download percent");
	_Stats->SetGrText(4,"Video size - Packet #");
	_Stats->SetGrText(5,"Downloaded KB");
	_Stats->SetGrText(6,"Time elapsed");
	_Stats->SetGrText(7,"Packet CRC");
	_Stats->SetText(0,"Initing socket");
	_Stats->SetText(1,"Reliable UDP/IP");
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
	_Stats->SetText(0,"Getting server information");
	if(i==0)
	{
		AfxMessageBox("There are no content servers found on the system.",MB_OKCANCEL |MB_ICONASTERISK);
		return 0;
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
if(Stats)
{
	_Stats->SetText(0,"SO_RCVTIMEO = 4000 ms");
}
	memset(&(m_Socketin.sin_zero), '\0', 8);
	int timeo = 8000;
	int optlen = sizeof(timeo);
	if(setsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeo,optlen)<0)
	{
		TCHAR *err = GetError();
		AfxMessageBox(err);
		delete []err;
		goto Quit;
	}
	// SO_RCVBUF
	int rcv = 1048576;
	optlen = sizeof(rcv);
	if(setsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeo,optlen)==SOCKET_ERROR)
	{
		TCHAR *err = GetError();
		AfxMessageBox(err);
		delete []err;
		goto Quit;
	}
	//---------------------------------------
	sendto(m_Socket,"cem\r\n",5,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
if(Stats)
{
	_Stats->SetText(0,"Establishing reliable connectionless comm.");
}
	int Num=0;
	Num = recvfrom(m_Socket,Message,512,0,(struct sockaddr *)&m_Socketin,&addr_len);
	if (Num == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT) 
		{
			AfxMessageBox("time out");
		}
	}

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
if(Stats)
	_Stats->SetText(0,"Waiting welcome message");
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
if(Stats)
	_Stats->SetText(0,"Setting video path");
	wsprintf(Mes,"3 %s\r\n",Pr->VideoPath);
	sendto(m_Socket,Mes,(int)strlen(Mes)+1,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
	Num=recvfrom(m_Socket,Message,512,0,(struct sockaddr *)&m_Socketin,&addr_len);
	Message[Num]='\0';
	wsprintf(Mes,"Directory set to %s\r\n",Pr->VideoPath);
	if(strcmp(Message,Mes)!=0)
	{
		AfxMessageBox("Project directory cannot be found on the server!");
		goto Quit1;
	}
	wsprintf(Mes,"7 %s\r\n",Pr->VideoName);
	sendto(m_Socket,Mes,(int)strlen(Mes)+1,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
	Num=recvfrom(m_Socket,Message,512,0,(struct sockaddr *)&m_Socketin,&addr_len);
	Message[Num]='\0';
	unsigned __int64 size = _atoi64(Message); // bytes
	
	//////////////////////////////////////////////////////  file size
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
if(Stats)
{
	wsprintf(sz,"%d",(int)size/1024);
	_Stats->SetText(4,sz);
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
	_Stats->SetText(0,"Downloading avi header");
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
			if (Read == SOCKET_ERROR)
			{	
				if (WSAGetLastError() == WSAETIMEDOUT) 
				{
				AfxMessageBox("time out");
				}else AfxMessageBox("Socket error");
			}
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
	unsigned int Read = 0;
//	unsigned int nRead = BufferUDP;
	int opt = 512000;
	optlen = sizeof(opt);
	// WSAETIMEDOUT 
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
	dataPointer=(char*)MapViewOfFile(hMemFile,FILE_MAP_ALL_ACCESS,0,0,gr);
	if(dataPointer==NULL)
	{
		TCHAR szBuf[512];
		DWORD dw = GetLastError(); 
		sprintf(szBuf, "Failed: Error code: %u\n",  dw); 
		AfxMessageBox(szBuf);
		goto Quit;
	}
	wsprintf(Mes,"6 %s\r\n",Pr->VideoName);
	sendto(m_Socket,Mes,(int)strlen(Mes)+1,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
	Sleep(100);
	wsprintf(Mes,"8 0");
if(Stats)
	_Stats->SetText(0,"Sending file request");
	sendto(m_Socket,Mes,(int)strlen(Mes)+1,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
	//Ru = new RU;
	//Ru->ptBuf = new char[BufferUDP];
	int dataSize =	BUFFER_SIZE + (3*sizeof(int));
	char * Packet = (char *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dataSize);
	if (!Packet) 
	{
		AfxMessageBox("Unable to allocate memory");
		goto Quit;
	}	
	memset(Packet,0,dataSize);
	UINT PartCount = 0;
if(Stats)
{
	_Stats->SetText(0,"Starting to download video");
}
crc.Init_CRC32_Table();
DWORD Start= 0;
Start = GetTickCount();
//bool set1 = TRUE;
int pckt = 0;
char *buf1 = new char[gr];
char *bufpt = buf1;
UINT bfsize = gr;


// ---------------------------------------------------------------------------------------------------
READ:
	if(hWnd==NULL) goto Quit;
//if(Stats)
//	_Stats->SetText(0,"Receiving packet");
	if(Read>=size) goto Quit;
	Read = recvfrom(m_Socket,Packet,dataSize,0,(struct sockaddr *)&m_Socketin,&addr_len);
	if (Read == SOCKET_ERROR)
	{	
		if (WSAGetLastError() == WSAETIMEDOUT) 
		{
			AfxMessageBox("time out");
			TimeOut = TRUE;
			//goto TIMEO;
		}else AfxMessageBox("Socket error");
	}
	if(Read<0 || Read==0) 
	{
		TCHAR * errt = new TCHAR[256];
		int err = WSAGetLastError();
		wsprintf(errt,"Error #:%d",err);
		AfxMessageBox(errt);
		delete []errt;
		_Stats->SetText(0,"0 size packet received");
		goto Quit;
	}
	TimeOut = FALSE;
	pckt = ((RU*)Packet)->PacketNumber;

	int cc = crc.Get_CRC(((RU*)Packet)->ptBuf);
	/*if (set1)
	{
		for(unsigned i=0;i<100;i++)
		{
            if(((RU*)Packet)->PacketNumber == 8*i) 
			{
				set1 = FALSE;
				cc = 0;
			}
		}
	}*/
	if(Read<=(4*sizeof(int)) || ((RU*)Packet)->Size==0) goto END;
if(Stats)
{
	double vs = (double)(GetTickCount()-Start);
	TCHAR *sz1 = new TCHAR[200];
	sprintf(sz1,"%.f ms",vs);
	//_Stats->SetText(6,sz);
	_Stats->PostMessage(WM_SETSTATS,6,(LPARAM)sz1);
	if(vs==0) vs=0.1;
	double speed1 = (double)((nTotalRead*8)/(vs*1000));
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
	wsprintf(sz4,"%d",cc);
	//_Stats->SetText(7,sz);
	
	_Stats->PostMessage(WM_SETSTATS,7,(LPARAM)sz4);
	TCHAR *sz8 = new TCHAR[200];
	wsprintf(sz8,"%d",pckt);
	//_Stats->SetText(7,sz);
	
	_Stats->PostMessage(WM_SETSTATS,4,(LPARAM)sz8);
	TCHAR *sz5 = new TCHAR[200];
	wsprintf(sz5,"%lu kB",(ULONG)(nTotalRead/1024));
	//_Stats->SetText(5,sz);
	_Stats->PostMessage(WM_SETSTATS,5,(LPARAM)sz5);
	
}
	if(cc == ((RU*)Packet)->CRC  && (PartCount== pckt))
	{
		if((((RU*)Packet)->Size)<=bfsize)
		{
			memcpy(bufpt,((RU*)Packet)->ptBuf,((RU*)Packet)->Size);
			bfsize -= ((RU*)Packet)->Size;
			bufpt +=  ((RU*)Packet)->Size;
			nTotalRead += Read;
			PartCount++;
			//AfxMessageBox("ok");
		} else // not enough space, fill memmap
		{
			memcpy(bufpt,((RU*)Packet)->ptBuf,bfsize); // artýk var þimdi
			bufpt = buf1;
			// memory map
			memcpy(dataPointer,buf1,gr);
			nCount++;
			if(dataPointer!=NULL) UnmapViewOfFile(dataPointer);
			dataPointer=(char*)MapViewOfFile(hMemFile,FILE_MAP_ALL_ACCESS,0,nCount*gr,gr);
			// End memory map
            char *pt = ((RU*)Packet)->ptBuf;
			pt += bfsize;
			memcpy(bufpt,pt,((((RU*)Packet)->Size)-bfsize));
			bufpt += ((((RU*)Packet)->Size)-bfsize);
			bfsize = gr-((((RU*)Packet)->Size)-bfsize);
			nTotalRead += ((RU*)Packet)->Size;
			PartCount++;
			//AfxMessageBox("ok1");
		}
		/*
		try{
            memcpy(dataPointer,((RU*)Packet)->ptBuf,((RU*)Packet)->Size);
		} catch(...)
		{
			
		}
		
		try{
            dataPointer+=((RU*)Packet)->Size;
		} catch(CException *e)
		{
			e->ReportError();
			e->Delete();
		}
		nPartialRead += ((RU*)Packet)->Size;
		nTotalRead += ((RU*)Packet)->Size;
		if(nPartialRead>=gr*2)
		{
			nCount++;
            if(dataPointer!=NULL) UnmapViewOfFile(dataPointer);
			dataPointer=(char*)MapViewOfFile(hMemFile,FILE_MAP_ALL_ACCESS,0,nCount*gr*2,gr*3);
			if(dataPointer==NULL)
			{
				dataPointer=(char*)MapViewOfFile(hMemFile,FILE_MAP_ALL_ACCESS,0,nCount*gr,gr);
				if(dataPointer==NULL)
				{
					goto END;
				}
			}
			nPartialRead = 0;
		}

		PartCount++;
		*/
		if(set)
		{
			if(nTotalRead>65535 && !_tcsstr(Pr->VideoName, TEXT(".avi")))
			{
				double vs = (double)(GetTickCount()-Start);
				int speed1 = (int)((nTotalRead*8)/vs);
				set = FALSE;
				::PostMessage(hWnd,WM_PLAY,(ULONG)((size*8)/1000),speed1);

			}//
			if(nTotalRead>size/40 && _tcsstr(Pr->VideoName, TEXT(".avi")))
			{
				set = FALSE;
				double vs = (double)(GetTickCount()-Start);
				int speed1 = (int)((nTotalRead*8)/vs);
				//this->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
				::PostMessage(hWnd,WM_PLAY,(ULONG)(((size*8)-size/20)/1000),speed1);
			}
		}
		
		int nPos = (int)(nTotalRead/(1024));
		::PostMessage(hWnd,WM_SLI,nPos,0);
        // datayý iþle
		wsprintf(Mes,"8 1");
		sendto(m_Socket,Mes,(int)strlen(Mes)+1,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
if(Stats)
{
	//wsprintf(sz,"storing packet %d",pckt);
	TCHAR *sz4 = new TCHAR[200];
	wsprintf(sz4,"Storing packet %d",pckt);
	_Stats->PostMessage(WM_SETSTATS,0,(LPARAM)sz4);
}
	//set1 = TRUE;
		goto LOOP;
	} else 
	{
if(Stats)
{
	TCHAR *sz4 = new TCHAR[200];
	wsprintf(sz4,"Packet lost #%d, received %d",PartCount,pckt);
	_Stats->PostMessage(WM_SETSTATS,0,(LPARAM)sz4);
}
		wsprintf(Mes,"8 2");
		sendto(m_Socket,Mes,(int)strlen(Mes)+1,0,(const sockaddr*)&m_Socketin,sizeof(sockaddr_in));
		goto LOOP;
	}

LOOP:
	goto READ;
END:
	//::PostMessage(hWnd,WM_PLAY,0,0);
	//delete []buffer;
	delete []buf1;
	HeapFree(GetProcessHeap(), 0, Packet);
Quit:
//	double vs = (double)(GetTickCount()-Start);
Quit1:
	//TCHAR *sz1 = new TCHAR[200];
	//sprintf(sz1,"%.f ms",vs);
	//AfxMessageBox(sz1,MB_OK);
if(Stats)
{
	for(unsigned i=0;i<100;i++)
	{
		_Stats->PostMessage(WM_SETHIST,(WPARAM)0.1,0);
	}
}
	closesocket(m_Socket);
	try{
		if(dataPointer!=NULL) UnmapViewOfFile(dataPointer);
		//AfxMessageBox("unmap");
		if(hMemFile!=NULL) CloseHandle(hMemFile);
		
	} catch(...)
	{}
	
	return TRUE;
}
	

int CRUDP::ExitInstance()
{
	
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRUDP, CWinThread)
END_MESSAGE_MAP()


// Gets Socket error

TCHAR * CRUDP::GetError(void)
{
	TCHAR * errt = new TCHAR[256];
	int err = WSAGetLastError();
	wsprintf(errt,"Error #:%d",err);
	return errt;
}
