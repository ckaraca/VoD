#pragma once
#include "Client.h"
class CUDP
{
public:
	CUDP(void);
	~CUDP(void);
	sockaddr_in Far;
	CClient *Client;
	BOOL Free;
	int addr_len;
};
