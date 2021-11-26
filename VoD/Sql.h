#pragma once
#include <mysql.h>
typedef struct _USER {
	CString m_IP;
	CString user;
	CString pass;
} USER;


class CSql
{
public:
	CSql(void);
	~CSql(void);
USER User;	
	TCHAR FTP[100];
	BOOL Connected;
	void Ping(void);
	BOOL Close(void);

public:
	MYSQL		* myData ;
	MYSQL_RES	* res ,*res1,*res2,*res3,*res4;
	MYSQL_FIELD	* fd ;
	MYSQL_ROW	row ;
};

typedef struct tagTHREADPARMS {
	HWND hWnd;
	CSql *Sql;
} THREADPARMS;
