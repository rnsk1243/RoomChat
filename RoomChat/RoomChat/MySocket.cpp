#include "MySocket.h"

SOCKET* CMySocket::Accept()
{
	if (hServSock == nullptr)
	{
		cout << "Accept Error" << endl;
		return nullptr;
	}
	// ����&�ּ� �޸� �Ҵ�
	SOCKET* hClientSock = new SOCKET();
	SOCKADDR_IN* hClntAddr = new SOCKADDR_IN();
	int szClntAddr;
	// accept
	szClntAddr = sizeof(*hClntAddr);
	*hClientSock = accept(*hServSock, (SOCKADDR*)hClntAddr, &szClntAddr);// list�� ���Ϳ� ����
																		 //cout << "accept ������ �ּ� = " << hClntSock << endl;
	if (*hClientSock == INVALID_SOCKET)
		cout << "accept() error" << endl;
	else
		cout << "Connected client" << endl;

	return hClientSock;
}