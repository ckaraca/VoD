#define BUFFER	1024
extern CString VideoStream;
extern CString VideoPath;
extern HWND hWnd;
extern int	BufferTCP;
UINT TCPThread(LPVOID pPram)
{
	double speed=0;
	double m_percent=0;
	int PacketNumber=0;
	int Read1 = 0;
	
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
	_Stats->SetText(1,"TCP/IP");
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
if(Stats)
{
	_Stats->SetText(0,"Getting server info");
}
	err = mysql_real_query(Log.myData,sz,sizeof(sz));
	if(err!=0)
	{
		wsprintf(szError, " %s",mysql_error((MYSQL*)Log.myData)) ;
		AfxMessageBox(szError,NULL,MB_OK|MB_ICONWARNING);
		return 0;
	}
	Log.res = mysql_store_result( Log.myData );
	i = (int) mysql_num_rows( Log.res );
	
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

	SOCKET m_Client;
	m_Client = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_Client==INVALID_SOCKET)
	{
if(Stats)
{
	_Stats->SetText(0,"Invalid Socket");
} else 
		AfxMessageBox("Invalid socket");
		return 0;
	}
	unsigned long Addr;
	struct sockaddr_in server;
	Addr = inet_addr(Host);
	server.sin_addr.S_un.S_addr = Addr;
	server.sin_family=AF_INET;
	server.sin_port=htons(21000);
	if(connect(m_Client,(struct sockaddr*)&server,sizeof(server)))
	{
		goto Quit;
	}
if(Stats)
{
	_Stats->SetText(0,"Establishing connection");
}
	char buff[BUFFER];
	int z;
	z=recv(m_Client,buff,BUFFER,0);
	buff[z]='\0';
	if(strcmp(buff,"0 Server Ready\r\n")!=0)
	{
if(Stats)
{
	_Stats->SetText(0,"TCP Server is down");
}
		goto Quit;
	}
if(Stats)
{
	_Stats->SetText(0,"Welcome message received,sending password");
}
	send(m_Client,"2 Vo\0",5,0);
	z=recv(m_Client,buff,BUFFER,0);
	buff[z]='\0';
	//AfxMessageBox(buff);
	if(strcmp(buff,"User accepted\r\n")!=0)
	{
if(Stats)
{
	_Stats->SetText(0,"Wrong user or password");
}
		goto Quit;
	}
	TCHAR Mes[BUFFER];
	wsprintf(Mes,"3 %s\r\n",Pr->VideoPath);
	send(m_Client,Mes,sizeof(Mes),0);
	z=recv(m_Client,buff,BUFFER,0);
	buff[z]='\0';
	wsprintf(Mes,"Directory set to %s\r\n",Pr->VideoPath);
	if(strcmp(buff,Mes)!=0)
	{
		goto Quit;
	}
	
	wsprintf(Mes,"7 %s\r\n",Pr->VideoName);
	send(m_Client,Mes,sizeof(Mes),0);
	z=recv(m_Client,buff,BUFFER,0);
	buff[z]='\0';
	unsigned __int64 size = _atoi64(buff);
if(Stats)
{
	//wsprintf(sz,"%d.%.3f",size/1024,size%1024);
	sprintf(sz,"%.f kB - %d Bytes",(float)size/1024,BufferTCP);
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
        FILE_ATTRIBUTE_NORMAL,//flags and attr
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
		if(datapointer1!=NULL) UnmapViewOfFile(datapointer1);
	}
if(Stats)
{
	_Stats->SetText(3,"0 %");
}
	Sleep(100);
	ULONG nTotalRead = 0;
//	ULONG nPartialRead = 0;
	UINT nCount = 0;
	char *buffer = new char[BufferTCP];
	unsigned int nRead = BufferTCP;
	
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
	wsprintf(Mes,"4 %s\r\n",Pr->VideoName);
	send(m_Client,Mes,sizeof(Mes),0);
	unsigned int setbuf = 512000;
	int optlen = sizeof(setbuf);
	if(setsockopt(m_Client,SOL_SOCKET,SO_RCVBUF,(const char*)&setbuf,optlen)<0) 
	{
		TCHAR * errt = new TCHAR[256];
		int err = WSAGetLastError();
		wsprintf(errt,"Error #:%d",err);
		AfxMessageBox(err);
		delete []errt;
		goto Quit;
	}
	
	//char *Part = new char[gr];
if(Stats)
{
	_Stats->SetText(0,"Started to downloading video");
}
	DWORD Start= 0;
	Start = GetTickCount();
	//while (nRead < BufferTCP)
	char *buf1 = new char[gr];
	char *bufpt = buf1;
	UINT bfsize = gr;
	while (nTotalRead < size)
	{		
		if(hWnd==NULL) goto Quit;
		// read remote data into buffer
		//memset(buffer,'c',BufferTCP);
		nRead = recv(m_Client,buffer,BufferTCP,0);
		//nRead = m_Asc.Receive(buffer,BufferTCP);
		if(nRead<0)
		{
			TCHAR * errt = new TCHAR[256];
			int err = WSAGetLastError();
			wsprintf(errt,"Error #:%d",err);
			AfxMessageBox(errt);
			delete []errt;
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
			memcpy(dataPointer,buf1,gr);
			nCount++;
			if(dataPointer!=NULL) UnmapViewOfFile(dataPointer);
			dataPointer=(char*)MapViewOfFile(hMemFile,FILE_MAP_ALL_ACCESS,0,nCount*gr,gr);
			// End memory map
            char *pt = buffer;
			pt += bfsize;
			memcpy(bufpt,pt,(nRead-bfsize));
			bufpt += (nRead-bfsize);
			bfsize = gr-(nRead-bfsize);
			nTotalRead += nRead;
		}
		if(set)
		{
			if(nTotalRead>65535 && !_tcsstr(Pr->VideoName, TEXT(".avi")))
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
	double percent = (double)((double)100*nTotalRead/size);
	if(m_percent!=percent)
	{
		m_percent = percent;
		TCHAR *sz3 = new TCHAR[200];
		sprintf(sz3,"%.1f %%",m_percent);
		_Stats->PostMessage(WM_SETSTATS,3,(LPARAM)sz3);
	}
	if(Read1!= (int)nRead)
	{
		Read1 = nRead;
		TCHAR *sz4 = new TCHAR[200];
		sprintf(sz4,"%.f kB - %d Bytes",(float)size/1024,Read1);
		_Stats->PostMessage(WM_SETSTATS,4,(LPARAM)sz4);
	}
	TCHAR *sz5 = new TCHAR[200];
	wsprintf(sz5,"%d",PacketNumber);
	_Stats->PostMessage(WM_SETSTATS,7,(LPARAM)sz5);
	TCHAR *sz6 = new TCHAR[200];
	wsprintf(sz6,"%lu kB",(ULONG)(nTotalRead/1024));
	_Stats->PostMessage(WM_SETSTATS,5,(LPARAM)sz6);
	
}
	}
Quit1:
	//::PostMessage(hWnd,WM_PLAY,0,0);
	delete []buffer;
	delete []buf1;
Quit:
//	delete []Part;
	/*double vs = (double)(GetTickCount()-Start);
	TCHAR *sz1 = new TCHAR[200];
	sprintf(sz1,"%.f ms",vs);
	AfxMessageBox(sz1,MB_OK);*/
if(Stats)
{
	for(unsigned i=0;i<100;i++)
	{
		_Stats->PostMessage(WM_SETHIST,(WPARAM)0.1,0);
	}
}
	closesocket(m_Client);
	try{
		if(dataPointer!=NULL) UnmapViewOfFile(dataPointer);
		if(hMemFile!=NULL) CloseHandle(hMemFile);
		
	} catch(...)
	{}

	return 0;
}
