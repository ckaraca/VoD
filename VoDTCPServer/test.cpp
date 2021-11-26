// test.cpp : implementation file
//

#include "stdafx.h"
#include "VoDTCPServer.h"
#include "test.h"


// test

IMPLEMENT_DYNCREATE(test, CWinThread)

test::test()
{
}

test::~test()
{
}

BOOL test::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int test::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(test, CWinThread)
END_MESSAGE_MAP()


// test message handlers
