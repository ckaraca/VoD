#pragma once



// test

class test : public CWinThread
{
	DECLARE_DYNCREATE(test)

protected:
	test();           // protected constructor used by dynamic creation
	virtual ~test();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


