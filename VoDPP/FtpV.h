#pragma once
#include "afxinet.h"

#include <string>
#include <vector>
using namespace std;


// CFtpV

class CFtpV : public CWinThread
{
	DECLARE_DYNCREATE(CFtpV)

public:
	CFtpV();           // protected constructor used by dynamic creation
	virtual ~CFtpV();
	void Connect(void);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DWORD ResumeThread();
private:
	HANDLE hFile,hMemFile;
	char* dataPointer,*datapointer1;
	TCHAR Host[220],Port[10],User[20],Pass[20];
	void OnThreadExit(WPARAM wParam, LPARAM lParam);
	void Warn(TCHAR *TextToWarn); // Type the text that you want it to appear in the list box
	CInternetSession* m_pInetSession;
	CFtpConnection* m_pFtpConnection;
	ULONGLONG m_Size;
	void GetLastResponse(string& strResponse);
protected:
	DECLARE_MESSAGE_MAP()
protected:
	double speed;
public:
	double m_percent;
	int PacketNumber;
};


