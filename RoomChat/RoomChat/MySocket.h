#pragma once
#include<WinSock2.h>
#include"Room.h"
#include<iostream>
using namespace std;
#define PORT 9000

class CMySocket
{
	SOCKET* hServSock;
public:
	CMySocket()
	{
		WSADATA wsaData;
		SOCKADDR_IN servAddr;
		hServSock = new SOCKET();
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			err_display("WSAStartup() error!");

		*hServSock = socket(PF_INET, SOCK_STREAM, 0);
		if (*hServSock == INVALID_SOCKET)
			err_display("socket() error");

		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons(PORT);

		if (bind(*hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
			err_display("bind() error");
		if (listen(*hServSock, 5) == SOCKET_ERROR)
			err_display("listen() error");
		else
			printf("listen...");
	}

	~CMySocket(){}

	SOCKET* getServSock() { return hServSock; }

	void err_display(char * msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);
		printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
	}

};

