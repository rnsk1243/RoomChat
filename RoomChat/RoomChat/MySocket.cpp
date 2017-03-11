#include "MySocket.h"

SOCKET* CMySocket::Accept()
{
	if (hServSock == nullptr)
	{
		cout << "Accept Error" << endl;
		return nullptr;
	}
	// 소켓&주소 메모리 할당
	SOCKET* hClientSock = new SOCKET();
	SOCKADDR_IN* hClntAddr = new SOCKADDR_IN();
	int szClntAddr;
	// accept
	szClntAddr = sizeof(*hClntAddr);
	*hClientSock = accept(*hServSock, (SOCKADDR*)hClntAddr, &szClntAddr);// list나 벡터에 저장
																		 //cout << "accept 에서의 주소 = " << hClntSock << endl;
	if (*hClientSock == INVALID_SOCKET)
		cout << "accept() error" << endl;
	else
		cout << "Connected client" << endl;

	return hClientSock;
}