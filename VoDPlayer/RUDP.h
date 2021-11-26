#pragma once
#include "NewFtp.h"
#include "Sql.h"
#include "CRC.h"
#define BUFFER_SIZE		61400	//65467

// CRUDP

class CRUDP : public CWinThread
{
	DECLARE_DYNCREATE(CRUDP)

public:
	CRUDP();           // protected constructor used by dynamic creation
	virtual ~CRUDP();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
private:
	TCHAR Message[512];

protected:
	SOCKET m_Socket;
	sockaddr_in m_Socketin;
	typedef struct _RU {
		char ptBuf[BUFFER_SIZE];
		UINT Size;
		int CRC;
		UINT PacketNumber;
	} RU;
	DECLARE_MESSAGE_MAP()
public:
	// Gets Socket error
	TCHAR * GetError(void);
	CCRC crc;
protected:
	double speed;
public:
	double m_percent;
	int PacketNumber;
};


