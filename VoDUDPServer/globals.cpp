#include "stdafx.h"
#include "Client.h"
#include "UDP.h"

TCHAR ServerPath[MAX_PATH+255],ServerPathName[MAX_PATH];
HWND hWnd;

unsigned ThQe = 0;
CClient *Client;


CUDP *UPar = new CUDP[MAX_CLIENTS];
SOCKET m_Socket;
int m_Ssp;
int m_Ratio;