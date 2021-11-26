#pragma once
#include "afx.h"
#include "CRC.h"
// CClient


extern SOCKET m_Socket;
#define QUIT	1
#define AUTH	2
#define SCD		3
#define READ	4
#define SET		5
#define SETREAD	6
#define GFS		7
#define RREAD	8
#define ROK		9
#define RFAIL	10
#define CS		11
#define BUFFER_SIZE		16384	// also in VoDUDPServer 32768
#define BUFFER_R		61400	//65467 test this!!!
	
extern int m_Ratio;
class CClient : public CWinThread
{
	DECLARE_DYNCREATE(CClient)

	CClient();           // protected constructor used by dynamic creation
	virtual ~CClient();
private:
	int ticks;
	bool Active;
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void OnTimer(UINT nIDEvent);
	void OnMessage(WPARAM wParam, LPARAM lParam);
	void OnTtimer(WPARAM wParam, LPARAM lParam);
protected:
	void Deactive(void);
	unsigned int IID;
	typedef struct _MESSAGE {
	int retcode;
	int NumOfSt;
	CString Params;
	} MESSAGE;
	LONG pointer;
	TCHAR * GetError(void);
	DECLARE_MESSAGE_MAP()
public:
	// // User authenticated or not
	bool Auth;
private:
	bool Set;
protected:
	CFile SetFile;
	typedef struct _RU {
		char ptBuf[BUFFER_R];
		UINT Size;
		int CRC;
		UINT PacketNumber;
	} RU;
	CCRC crc;
	UINT PacketNumber;
	int m_cs;
	int amount;
	struct timeval tval;

};


