#pragma once
#include "Client.h"
#include "UDP.h"


// CTCPListener

UINT UDPListener(LPVOID pParam);

extern CUDP *UPar;
typedef struct _MESSAGE {
	int retcode;
	int NumOfSt;
	CString Params;
} MESSAGE;

MESSAGE* SplitCommand(char * String);
