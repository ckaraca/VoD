#include "StdAfx.h"
#include "sql.h"

CSql::CSql(void)
{
	Connected = FALSE;
}

CSql::~CSql(void)
{
	if(myData!=NULL)
	{
		//mysql_close( myData );
		//MessageBox(NULL,"Disconnecting",NULL,MB_OK);
	}
}
