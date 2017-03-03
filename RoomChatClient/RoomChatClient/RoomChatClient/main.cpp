#include"MySocket.h"
#include"NetWork.h"
#include<iostream>
using namespace std;

struct StructParam
{
	SOCKET* clientSockt;
	CNetWork* netWork;
};

DWORD WINAPI Recv(PVOID param)
{
	StructParam* SP = (StructParam*)param;

	SOCKET* clientSockt = SP->clientSockt;
	CNetWork* netWork = SP->netWork;

	while (true)
	{
		netWork->recvn(clientSockt);
	}
}

void main()
{
	CMySocket* MySocket = new CMySocket();
	CNetWork* netWork = new CNetWork();

	SOCKET* clientSocket = MySocket->getSocket();
	char* sendMessage = new char[BUF_SIZE];

	StructParam* SP = new StructParam();
	SP->clientSockt = clientSocket;
	SP->netWork = netWork;

	CreateThread(NULL, NULL, Recv, SP, NULL, NULL);

	while (true)
	{
		cin >> sendMessage;
		netWork->sendn(clientSocket, sendMessage);
	}
}