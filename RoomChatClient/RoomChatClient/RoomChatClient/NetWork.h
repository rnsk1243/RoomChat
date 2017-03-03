#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include"MySocket.h"

struct MessageStruct
{
	char* message = new char[BUF_SIZE];
	int* sendRecvSize = new int;
};

class CNetWork
{
	MessageStruct* MS;
public:
	CNetWork();
	~CNetWork();
	int sendn(SOCKET * clientSocket, char* message, int flags = 0);
	int recvn(SOCKET* clientSocket, int flags = 0);
};

