// TCPListener.cpp : implementation file
//

#include "stdafx.h"
#include "VoDUDPServer.h"
#include "UDPListener.h"
#define UDP_PORT		22000
#define BUFFER			512
//#define BUFFER_SIZE		61440

// CTCPListener
extern HWND hWnd;
extern unsigned ThQe;
extern CClient *Client;
extern SOCKET m_Socket;

UINT UDPListener(LPVOID pParam)
{
	AfxSocketInit();
	sockaddr_in m_Socketin;
	
	m_Socketin.sin_family = AF_INET;
	m_Socketin.sin_port = htons(UDP_PORT);
	m_Socketin.sin_addr.S_un.S_addr = INADDR_ANY;
	m_Socket=socket(AF_INET,SOCK_DGRAM,0);
	memset(&(m_Socketin.sin_zero), '\0', 8);
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
	Client = new CClient[MAX_CLIENTS];
	for(unsigned i=0;i<MAX_CLIENTS;i++)
	{
        if (!Client[i].CreateThread(THREAD_PRIORITY_NORMAL))
		{
			delete Client[i];
		}
	}
	
	
	for(unsigned j=0;j<MAX_CLIENTS;j++)
	{
		UPar[j].Client = NULL;
		UPar[j].Free = TRUE;
	}
	sockaddr_in m_Client;
	int NumBytes = 0;
	int addr_len = sizeof(struct sockaddr);
	char buff[BUFFER];
	for(;;)
	{
		MESSAGE *Parse;
        NumBytes = recvfrom(m_Socket,buff,BUFFER,0,(struct sockaddr *)&m_Client,&addr_len);
		if (NumBytes == SOCKET_ERROR)
		{
			buff[0] = '\0';
			goto END;
		}
		for(unsigned j=0;j<MAX_CLIENTS;j++)
		{
			if(m_Client.sin_port == UPar[j].Far.sin_port) // burada sorun war
			{
				

				if(UPar[j].Free ==FALSE)
				{
                    Parse = SplitCommand(buff);//,Params);
					UPar[j].Client->PostThreadMessage(WM_MESSAGE,(WPARAM)Parse,(LPARAM)1);
				}
				
				goto END;
			} 
		}
		for(unsigned j=0;j<MAX_CLIENTS;j++)
		{
			if(UPar[j].Free == TRUE)
			{
				// assign ThQe'th thread with this call
				UPar[j].Free = FALSE;
				UPar[j].Client = &Client[j];
				UPar[j].Far = m_Client;
				UPar[j].addr_len = addr_len;
				CString text;
				text.Format("Starting worker thread %d",Client[j].m_nThreadID);
				Client[j].PostThreadMessage(WM_MESSAGE,(WPARAM)(unsigned int)j,(LPARAM)(int)-1); // means first time and get id "j"
				ThQe++;
				::SendMessage(hWnd,WM_SETLIST,(WPARAM)"001",(LPARAM)(LPCTSTR)text);
				goto END;
			}
		}
END:
		NumBytes = 0;
	}
	closesocket(m_Socket);
	return TRUE;
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
