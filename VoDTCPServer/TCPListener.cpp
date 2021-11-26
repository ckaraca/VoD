// TCPListener.cpp : implementation file
//

#include "stdafx.h"
#include "VoDTCPServer.h"
#include "TCPListener.h"
#define TCP_PORT		21000
#define BUFFER			512
#define BUFFER_SIZE		61440
#define MAX_CLIENTS		20
// CTCPListener
extern HWND hWnd;



UINT TCPListener(LPVOID pParam)
{
	AfxSocketInit();
	sockaddr_in m_Socketin;
	SOCKET m_Socket;
	m_Socketin.sin_family = AF_INET;
	m_Socketin.sin_port = htons(TCP_PORT);
	m_Socketin.sin_addr.S_un.S_addr = INADDR_ANY;
	m_Socket=socket(AF_INET,SOCK_STREAM,0);
	::SendMessage(hWnd,WM_SETLIST,(WPARAM)"004",(LPARAM)"Initalizing socket");
	if(m_Socket==INVALID_SOCKET)
	{
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Failed!!",0);
		return 0;
	}
	::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
	::SendMessage(hWnd,WM_SETLIST,(WPARAM)"005",(LPARAM)"Binding socket");
	int ret = bind(m_Socket,(sockaddr*)&m_Socketin,sizeof(m_Socketin));
	switch(ret)
	{
	case WSANOTINITIALISED:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"A successful WSAStartup call must occur before using this function.",0);
		return 0;
		break;
	case WSAENETDOWN:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"The network subsystem has failed.",0);
		return 0;
		break;
	case 0:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
		break;
	case WSAEACCES:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Port already in use",0);
		return 0;
		break;
	case WSAEADDRINUSE:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"A process on the machine is already bound to the same fully-qualified address.",0);
		return 0;
		break;
	case WSAEADDRNOTAVAIL:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"The specified address is not a valid address for this machine",0);
		return 0;
		break;
	case WSAEFAULT:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"The system detected an invalid pointer address in attempting to use a pointer argument of a call",0);
		return 0;
		break;
	case WSAEINPROGRESS:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"A blocking call is in progress, or the service provider is still processing a callback function.",0);
		return 0;
		break;
	case WSAEINVAL:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"The socket is already bound to an address.",0);
		return 0;
		break;
	case WSAENOBUFS:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Not enough buffers available, too many connections.",0);
		return 0;
		break;
	case WSAENOTSOCK:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"The descriptor is not a socket",0);
		return 0;
		break;
	default:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Port already in use!!",0);
		return 0;
		return 0;
		break;
	}
	TCHAR text[100];
	wsprintf(text,"Listening on Port %d",TCP_PORT);
	::SendMessage(hWnd,WM_SETLIST,(WPARAM)"006",(LPARAM)text);
	ret = listen(m_Socket,MAX_CLIENTS);
	switch(ret)
	{
	case WSANOTINITIALISED:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"A successful WSAStartup call must occur before using this function.",0);
		return 0;
		break;
	case WSAENETDOWN:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"The network subsystem has failed.",0);
		return 0;
		break;
	case 0:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
		break;
	case WSAEISCONN:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"The socket is already connected.",0);
		return 0;
		break;
	case WSAEMFILE:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"No more socket descriptors are available.",0);
		return 0;
		break;
	case WSAENOBUFS:
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"No buffer space is available.",0);
		return 0;
		break;
	}
	SOCKET m_VoDClient;
	sockaddr_in m_VoDSocket;
	int fromlen=sizeof(m_VoDSocket);
	while(true)
	{
		m_VoDClient=accept(m_Socket,	(struct sockaddr*)&m_VoDSocket,&fromlen);
		AfxBeginThread(Client,(LPVOID)m_VoDClient);
		TCHAR peer[100];
		//getpeername(client,(struct sockaddr*)&from,&fromlen);
		wsprintf(peer,"accepting connection from %d.%d.%d.%d",
			m_VoDSocket.sin_addr.S_un.S_un_b.s_b1,
			m_VoDSocket.sin_addr.S_un.S_un_b.s_b2,
			m_VoDSocket.sin_addr.S_un.S_un_b.s_b3,
			m_VoDSocket.sin_addr.S_un.S_un_b.s_b4);
		::SendMessage(hWnd,WM_SETLIST,(WPARAM)"007",(LPARAM)peer);
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Accepted.",0);
	}	

	return TRUE;
}


// client 
extern TCHAR ServerPathName[];
extern TCHAR ServerPath[];
#define QUIT	1
#define AUTH	2
#define SCD		3
#define READ	4
#define SET		5
#define SETREAD	6
#define GFS		7

typedef struct _MESSAGE {
	int retcode;
	int NumOfSt;
	CString Params;
} MESSAGE;
MESSAGE* SplitCommand(char * String);

// TCP Client thread
UINT Client (LPVOID pParam)
{	
	AfxSocketInit();
    BOOL Auth = FALSE;
	BOOL Set = FALSE;
	CString Temp;
	char Message[BUFFER];
	CString Command,Params;
	sprintf(Message,"0 Server Ready\r\n");
	SOCKET m_VoDClient = (SOCKET)pParam;
	send(m_VoDClient,Message,(int)strlen(Message),0);
	int nRead=0;
	CFile SetFile;
	LONG pointer;
	SetCurrentDirectory(ServerPath);
	unsigned int setbuf = 262144;
	int optlen = sizeof(setbuf);
	if(setsockopt(m_VoDClient,SOL_SOCKET,SO_SNDBUF,(const char*)&setbuf,optlen)<0) 
	{
		TCHAR * errt = new TCHAR[256];
		int err = WSAGetLastError();
		wsprintf(errt,"Error #:%d",err);
		AfxMessageBox(err);
		delete []errt;
	}

	CFileException e;
	//::SetCurrentDirectory("c:\\");
	CStdioFile m_File;
	if(!m_File.Open("C:\\TCP.txt",CFile::modeReadWrite|CFile::modeCreate|CFile::shareDenyNone
		,&e))
	{
		e.ReportError();
		e.Delete();
	}
	for(;;)
	{
		MESSAGE *Parse;
		nRead = recv(m_VoDClient,Message,sizeof(Message),0);
		Temp.Empty();
		//Message[nRead]=0;
		if(nRead == SOCKET_ERROR || nRead==0)
		{
			break;
		}
		Parse = SplitCommand(Message);//,Params);
		switch(Parse->retcode)
		{
		case QUIT:
			if(Parse) delete Parse;
			goto End;
			break;
		case AUTH:
			if(Parse->NumOfSt == 1)
			{
				send(m_VoDClient,"Usage: 2 <Password>\r\n",21,0);
				::SendMessage(hWnd,WM_SETLIST,(WPARAM)"020",(LPARAM)"Authenticating user");
				::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Bad suffix!",0);
				break;
			} else if (Parse->NumOfSt == 2)
			{
				if(Parse->Params == "Vo")
				{
					Auth = TRUE;
					send(m_VoDClient,"User accepted\r\n",15,0);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"020",(LPARAM)"Authenticating user");
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
				} else
				{
					Auth = FALSE;
					send(m_VoDClient,"Not logged in!\r\n",16,0);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"020",(LPARAM)"Authenticating user");
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Not logged in!",0);
				}
			}
			break;
		case SCD:
			if(Auth)
			{
				int set=0;
				if(strcmp(Parse->Params,"..")!=0)
				{
				try{
					set = SetCurrentDirectory(Parse->Params);
				} catch(...){}
				}
				TCHAR text[512];
				wsprintf(text,"Setting dir to %s",Parse->Params);
				if(set==0)
				{
					Temp.Format("Directory %s cannot be set\r\n",Parse->Params);
					send(m_VoDClient,Temp,Temp.GetLength(),0);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"021",(LPARAM)text);
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Failed!",0);
				} else {
					Temp.Format("Directory set to %s\r\n",Parse->Params);
					send(m_VoDClient,Temp,Temp.GetLength(),0);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"021",(LPARAM)text);
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
				}
			} else 
			{
				send(m_VoDClient,"First log-in!\r\n",15,0);
				::SendMessage(hWnd,WM_SETLIST,(WPARAM)"021",(LPARAM)"User not logged in");
				::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"warning",0);
			}
			break;
		case READ:
			if(Auth)
			{
				/*BOOL opt = TRUE;
				int optlen = sizeof(opt);
				if(setsockopt(m_VoDClient,IPPROTO_IP,IP_DONTFRAGMENT,(const char*)&setbuf,optlen)<0)
				{
					TCHAR * errt = new TCHAR[256];
					int err = WSAGetLastError();
					wsprintf(errt,"Error #:%d",err);
					AfxMessageBox(err);
				}
				opt = TRUE;
				optlen = sizeof(opt);
				if(setsockopt(m_VoDClient,IPPROTO_TCP,TCP_NODELAY,(const char*)&setbuf,optlen)<0)
				{
					TCHAR * errt = new TCHAR[256];
					int err = WSAGetLastError();
					wsprintf(errt,"Error #:%d",err);
					AfxMessageBox(err);
				}*/

				TCHAR text[512];
				CString Param = Parse->Params;
				int set=0;
				CFile File;
				try {
                    set = File.Open((LPCTSTR)Param,CFile::modeRead|CFile::shareDenyNone|CFile::typeBinary);
				} catch(CFileException *e)
				{
					Temp.Format("Fatal error: %d \r\n",e->m_cause);
					send(m_VoDClient,Temp,Temp.GetLength(),0);
					e->Delete();
				}
				
				wsprintf(text,"Opening file %s",(LPCTSTR)Param);
				if(set==0)
				{
					Temp.Format("%s cannot be opened! \r\n",(LPCTSTR)Param);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"022",(LPARAM)text);
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Failed!",0);
					send(m_VoDClient,Temp,Temp.GetLength(),0);
					try {
						File.Close();
					} catch(CFileException *e)
					{
						e->Delete();
					}
					break;
				} else 
				{
					char Buffer[BUFFER_SIZE];
					int Read = BUFFER_SIZE;
					ULONG Total =0;
					CString temp;
					temp.Format("Started to sending file %s",(LPCTSTR)Param);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"023",(LPARAM)(LPCTSTR)temp);
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Started!",0);

					while(Read==BUFFER_SIZE)
					{
						int ss;
						try{
                            Read = File.Read(Buffer,BUFFER_SIZE);
							ss = send(m_VoDClient,Buffer,Read,0);
						} catch(...){}
						Total += Read;
						CString ttw;
						ttw.Empty();
						ttw.Format("%d\n",ss);
						//m_File.WriteString(ttw);
					}
					
					Temp.Format("Sent %d kBytes of file %s",Total,(LPCTSTR)Param);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"021",(LPARAM)(LPCTSTR)Temp);
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
					try {
						File.Close();
					} catch(CFileException *e)
					{
						e->Delete();
					}
				}
			}
			break;
		case SET:
			if(Auth)
			{
				char part[512];
			if(SetFile.m_hFile!=CFile::hFileNull)
			{
			Set = TRUE;
			pointer = atol(Parse->Params);
			SetFile.Seek(pointer,CFile::begin);
			char Buffer[BUFFER_SIZE];
			int Read = BUFFER_SIZE;
			int total = 0;
			while(true)
			{
				Read = SetFile.Read(Buffer,BUFFER_SIZE);
				total += Read;
				send(m_VoDClient,Buffer,Read,0);
				if(Read<BUFFER_SIZE) break;
			}
			wsprintf(part,"sent %d kBytes",total/1024);
			::SendMessage(hWnd,WM_SETLIST,(WPARAM)"010",(LPARAM)part);
			::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK.",0);
			SetFile.Close();
			}
			}
			break;
		case SETREAD:
			if(Auth)
			{
			int set=0;
			try{
				set = SetFile.Open(Parse->Params,CFile::modeRead|CFile::shareDenyNone);
			} catch(...)
			{
				send(m_VoDClient,"0",1,0);
				::SendMessage(hWnd,WM_SETLIST,(WPARAM)"021",(LPARAM)"Setting pointer");
				::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"FAILED!",0);
				SetFile.Close();
			}
			if(set==0)
			{
				send(m_VoDClient,"0",1,0);
				::SendMessage(hWnd,WM_SETLIST,(WPARAM)"021",(LPARAM)"Setting pointer");
				::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"FAILED!",0);
				SetFile.Close();
			} 
			break;
			}
		case GFS:
			if(Auth)
			{
			{
				CString Buffer;
				CFileFind finder;
				BOOL Working;
				ULONGLONG m_Size = 0;
							
				Working = finder.FindFile(_T(Parse->Params));
				while (Working)
				{
					Working = finder.FindNextFile();
					m_Size = finder.GetLength();
				}
				Buffer.Empty();
				Buffer.Format("%I64u",m_Size);
				
				send(m_VoDClient,Buffer,Buffer.GetLength(),0);
				finder.Close();
			
			}
			}
			break;
		}
		if(Parse) delete Parse;
	}
End:
	m_File.Close();
	::SendMessage(hWnd,WM_SETLIST,(WPARAM)"008",(LPARAM)"Closing connection");
	::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK",0);
	closesocket(m_VoDClient);
	return 0;
}

MESSAGE* SplitCommand(char * String)
{
	MESSAGE *temp = new MESSAGE;
	int index=0;
	CString Split = String;
	Split.TrimLeft();
	Split.TrimRight();
	if((index = Split.Find(' '))==-1)
	{
		Split.MakeUpper();
		temp->NumOfSt = 1;
		temp->Params ="";
		temp->retcode = atoi(Split);
		return temp;
		//return (atoi(Split));
	}
	temp->NumOfSt = 2;
	temp->Params = Split.Mid(index+1);
	temp->retcode = atoi(Split.Left(index));
	return temp;
}

