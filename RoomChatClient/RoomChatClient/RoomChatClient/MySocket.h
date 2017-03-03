#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;

#define BUF_SIZE 1024
#define INT 4
#define IP "127.0.0.1"
#define PORT 9000

class CMySocket
{
	SOCKET* hClientSock;
public:

	CMySocket()
	{
		WSADATA wsaData;
		SOCKADDR_IN clientAddr;
		hClientSock = new SOCKET();

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			err_display("WSAStartup() error!");

		*hClientSock = socket(PF_INET, SOCK_STREAM, 0);
		if (*hClientSock == INVALID_SOCKET)
			err_display("socket() error");

		memset(&clientAddr, 0, sizeof(clientAddr));
		clientAddr.sin_family = AF_INET;
		clientAddr.sin_addr.s_addr = inet_addr(IP);
		clientAddr.sin_port = htons(PORT);

		if (connect(*hClientSock, (SOCKADDR*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR)
			err_display("connect() error!");
		else
			puts("Connected.............");
	}

	SOCKET* getSocket() { return hClientSock; }

	~CMySocket()
	{
	}

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

