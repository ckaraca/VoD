// Client.cpp : implementation file
//

#include "stdafx.h"
#include "VoDUDPServer.h"
#include "Client.h"
#include "UDP.h"

extern CUDP *UPar;

#define TIME_OUT	6	//10* seconds
extern HWND hWnd;
extern unsigned ThQe;

extern TCHAR ServerPath[];
// CClient

IMPLEMENT_DYNCREATE(CClient, CWinThread)

extern CUDP *UPar;

CClient::CClient()
: Auth(false)
, Set(false)
, PacketNumber(0)
, m_cs(0)
, amount(50)
{
	ticks = 0;  // dont forget to reset ticks if sending or receiving UDP Packets
	Active = FALSE;
	SetCurrentDirectory(ServerPath);
}

CClient::~CClient()
{
}

BOOL CClient::InitInstance()
{
	AfxSocketInit();
	return TRUE;
}

int CClient::ExitInstance()
{
	
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CClient, CWinThread)
	ON_THREAD_MESSAGE(WM_MESSAGE, OnMessage)
	ON_THREAD_MESSAGE(WM_TTIMER, OnTtimer)
END_MESSAGE_MAP()

void CClient::OnMessage(WPARAM wParam, LPARAM lParam)
{
	/* 
	QUIT	1
	AUTH	2
	SCD		3
	READ	4
	SET		5
	SETREAD	6
	GFS		7*/
	TCHAR st[512];
	MESSAGE *Parse=NULL;
	CString Temp;
	if((int)lParam==-1)
	{
		IID = (unsigned int) wParam;
		sendto(m_Socket,"Welcome\r\n",9,0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
		::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
	} else // get params
	{	// delete Parse
		Parse=(MESSAGE*)wParam;
		Temp.Empty();
		switch(Parse->retcode)
		{
		case QUIT:
          	wsprintf(st,"Closing connection\r\n");
			sendto(m_Socket,st,(int)strlen(st),0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
			sendto(m_Socket,st,0,0,(const sockaddr*)&UPar[IID].Far,sizeof(sockaddr_in));
			Deactive();
			break;
		case AUTH:
			if(Parse->NumOfSt == 1)
			{
				sendto(m_Socket,"Usage: 2 <Password>\r\n\0",22,0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
				::SendMessage(hWnd,WM_SETLIST,(WPARAM)"020",(LPARAM)"Authenticating user");
				::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Bad suffix!",0);
				break;
			} else if (Parse->NumOfSt == 2)
			{
				if(Parse->Params == "Vo")
				{
					Auth = TRUE;
					sendto(m_Socket,"User accepted\r\n\0",16,0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"020",(LPARAM)"Authenticating user");
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
				} else
				{
					Auth = FALSE;
					sendto(m_Socket,"Not logged in!\r\n\0",17,0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"020",(LPARAM)"Authenticating user");
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Not logged in!",0);
				}
			}
			break;
		case SCD:
			if(Auth)
			{
				SetCurrentDirectory(ServerPath);
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
					Temp.Format("Directory %s cannot be set\r\n\0",Parse->Params);
					sendto(m_Socket,Temp,Temp.GetLength(),0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"021",(LPARAM)text);
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Failed!",0);
				} else {
					Temp.Format("Directory set to %s\r\n\0",Parse->Params);
					sendto(m_Socket,Temp,Temp.GetLength(),0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"021",(LPARAM)text);
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
				}
			} else 
			{
				sendto(m_Socket,"First log-in!\r\n\0",16,0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
				::SendMessage(hWnd,WM_SETLIST,(WPARAM)"021",(LPARAM)"User not logged in");
				::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"warning",0);
			}
			break;
		case READ:
			if(Auth)
			{
				TCHAR text[1024];
				CString Param = Parse->Params;
				wsprintf(text,"%s",Parse->Params);
				int set=0;
				//CreateFile
				CFile File;
				try {
					set = File.Open(text,CFile::modeRead|CFile::shareDenyNone|CFile::typeBinary);
				} catch(CFileException *e)
				{
					Temp.Format("Fatal error: %d \r\n\0",e->m_cause);
					sendto(m_Socket,Temp,Temp.GetLength(),0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
					e->Delete();
				}
				
				wsprintf(text,"Opening file %s",(LPCTSTR)Param);
				if(set==0)
				{
					Temp.Format("%s cannot be opened! \r\n\0",(LPCTSTR)Param);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"022",(LPARAM)text);
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"Failed!",0);
					sendto(m_Socket,Temp,Temp.GetLength(),0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
					/*try {
						if(File!=NULL) File.Close();
					} catch(CFileException *e)
					{
						e->Delete();
					}*/
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
					int NumBytes=0;
					DWORD Start,End;
					unsigned long  speed;
					unsigned long nr=0;
					Start = GetTickCount();
					// -- diag
					CFileException e;
	//::SetCurrentDirectory("c:\\");
	CStdioFile m_File;
	if(!m_File.Open("C:\\udp.txt",CFile::modeReadWrite|CFile::modeCreate|CFile::shareDenyNone
		,&e))
	{
		e.ReportError();
		e.Delete();
	}
	CString ttw;
						ttw.Empty();
						
					while(Read==BUFFER_SIZE)
					{
						
						try{
                            Read = File.Read(Buffer,BUFFER_SIZE);
							Sleep(amount/2);
							nr+= sendto(m_Socket,Buffer,Read,0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
							
							// do it here
							Sleep(amount/2);
							//tval.tv_sec = 1;
    						//tval.tv_usec = 0;//amount*1000;
    						//select ( 0, 0, 0, 0, &tval );
							//NumBytes = recvfrom(m_Socket,rec,512,0,(struct sockaddr *)&UPar[IID].Far,&UPar[IID].addr_len);
							//Sleep(28);

						} catch(...){}
						End = (GetTickCount()-Start);
						if(End!=0)
						{
							speed = (unsigned long)((nr)/(End));
							ttw.Empty();
							ttw.Format("sp->%lu\n",speed);

							m_File.WriteString(ttw);
							switch(m_cs) 
							{
								case 0:	// 10Base-T
                                    if(speed<460)
									{
										amount-=1;
									}
									break;
							}
						}
						Total += Read;
					}
					m_File.Close();
					amount = 50;
					Temp.Format("Sent %d.%d kBytes of file %s",Total/1024,Total%1024,(LPCTSTR)Param);
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
				sendto(m_Socket,Buffer,Read,0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
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
				sendto(m_Socket,"0",1,0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
				::SendMessage(hWnd,WM_SETLIST,(WPARAM)"021",(LPARAM)"Setting pointer");
				::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"FAILED!",0);
				SetFile.Close();
			}
			if(set==0)
			{
				sendto(m_Socket,"0",1,0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
				::SendMessage(hWnd,WM_SETLIST,(WPARAM)"021",(LPARAM)"Setting pointer");
				::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"FAILED!",0);
				SetFile.Close();
			} else
			{
				::SendMessage(hWnd,WM_SETLIST,(WPARAM)"021",(LPARAM)"Setting pointer");
				::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
			}
			break;
			}
		case CS:
	/*
	0 - 10BASE-T  - 10 Mbps
	1 - 100BASE-T - 100 Mbps
	2 - T1		 - 1.544 Mbps
	3 - T3        - 45 Mbps
	4 - ISDN      - 64 kbps
	5 - ISDN      - 128 kbps
	6 - Dialup    - 56 kbps
	*/
			if(Auth)
			{
				int cs = atoi(Parse->Params); 
				m_cs =0; // ms
				//make the ratio here
				

			}
			break;

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
					sendto(m_Socket,Buffer,Buffer.GetLength(),0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
					::SendMessage(hWnd,WM_SETLIST,(WPARAM)"028",(LPARAM)(LPCTSTR)"Sending file size");
					::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
					
					//send(m_Socket,Buffer,Buffer.GetLength(),0);
					finder.Close();
				}
			}
			break;
		case RREAD:
			if(Auth && SetFile.m_hFile!=CFile::hFileNull)
			{
				if(!Set)
				{
					int timeo = 40000;
					int optlen = sizeof(timeo);
					if(setsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeo,optlen)<0)
					{
						TCHAR *err = GetError();
						AfxMessageBox(err);
						delete []err;
					}
					Set = TRUE;
				}
				int dataSize =	BUFFER_R + (3*sizeof(int));
						
				char * Packet = (char *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dataSize);
				if (!Packet) 
				{
					AfxMessageBox("Unable to allocate memory");
					Deactive();
				}	
				memset(Packet,0,dataSize);
				int command = atoi(Parse->Params);
				switch(command)
				{
				case 0:		// first fragment, send beginning of the file
					{
						PacketNumber = 0;
						crc.Init_CRC32_Table();
						int Read = BUFFER_R;
						Read = SetFile.Read(((RU*)Packet)->ptBuf,BUFFER_R);
						((RU*)Packet)->Size = Read;
						((RU*)Packet)->CRC = crc.Get_CRC(((RU*)Packet)->ptBuf);
						((RU*)Packet)->PacketNumber = PacketNumber;
						sendto(m_Socket,Packet,dataSize,0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
						::SendMessage(hWnd,WM_SETLIST,(WPARAM)"031",(LPARAM)"Sending R Packet");
						::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
					}
					break;
				case 1:		// succeeded, send the rest
					{
						PacketNumber++;
						int Read = BUFFER_R;
						Read = SetFile.Read(((RU*)Packet)->ptBuf,BUFFER_R);
						((RU*)Packet)->Size = Read;
						((RU*)Packet)->CRC = crc.Get_CRC(((RU*)Packet)->ptBuf);
						((RU*)Packet)->PacketNumber = PacketNumber;
						sendto(m_Socket,Packet,dataSize,0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
					}	
					break;
				case 2:		// failed, subtrack BUFFER_SIZE from end of file and re send the rest
					{
						int Read = BUFFER_R;
						SetFile.Seek(-BUFFER_R,CFile::current);
						Read = SetFile.Read(((RU*)Packet)->ptBuf,BUFFER_R);
						((RU*)Packet)->Size = Read;
						((RU*)Packet)->CRC = crc.Get_CRC(((RU*)Packet)->ptBuf);
						((RU*)Packet)->PacketNumber = PacketNumber;
						sendto(m_Socket,Packet,dataSize,0,(const sockaddr*)&UPar[IID].Far,UPar[IID].addr_len);
					}	
					break;
				}
				HeapFree(GetProcessHeap(), 0, Packet);
				
				
				
			}
			break;
		default:
			break;
		}
	}
	
	ticks=0;
	//CreateWaitableTimer(
	Active = TRUE;
	if(Parse!=NULL) delete Parse;
	return;
}

void CClient::OnTtimer(WPARAM wParam, LPARAM lParam)
{
	if(Active)
	{
        ticks++;
		if(ticks>TIME_OUT) Deactive();
	}
}


void CClient::Deactive(void)
{
	CString text;
	text.Format("Exiting worker thread %d",this->m_nThreadID);
	::SendMessage(hWnd,WM_SETLIST,(WPARAM)"002",(LPARAM)(LPCTSTR)text);
	sendto(m_Socket,text,0,0,(const sockaddr*)&UPar[IID].Far,sizeof(sockaddr_in));
	Active=FALSE;
	UPar[IID].Free = TRUE;
	UPar[IID].Client = NULL;
	ThQe--;
	if(SetFile.m_hFile!=CFile::hFileNull) SetFile.Close();
	::SendMessage(hWnd,WM_SETLISTS,(WPARAM)"OK!",0);
}

TCHAR * CClient::GetError(void)
{
	TCHAR * errt = new TCHAR[256];
	int err = WSAGetLastError();
	wsprintf(errt,"Error #:%d",err);
	return errt;
}