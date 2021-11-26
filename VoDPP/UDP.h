#pragma once
#include "NewFtp.h"
#include "Sql.h"


// CUDP

class CUDP : public CWinThread
{
	DECLARE_DYNCREATE(CUDP)

public:
	CUDP();           // protected constructor used by dynamic creation
	virtual ~CUDP();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
private:
	TCHAR Message[512];

protected:
	SOCKET m_Socket;
	sockaddr_in m_Socketin;
	DECLARE_MESSAGE_MAP()
public:
	// Gets Socket error
	TCHAR * GetError(void);
protected:
	double speed;
public:
	double m_percent;
	int PacketNumber;
};


