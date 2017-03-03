#include "Accept.h"

DWORD WINAPI thNetWork(PVOID pvParam)
{
	StructParam* SP = (StructParam*)pvParam;
	CRoomShift* roomShift = SP->roomShift;
	CNetWork* netWork = SP->netWork; // StructParam�� ���� ��ü�� ������ �ʱ� ������ �� �ٸ� ������ ��������� Accept�� �Ͼ�� ����Ű�� CNetWork ��ü�� �޶� �� �� �����Ƿ� 
	// �����͸� ������ ���� ���ķδ� ������ �����͸� ����Ѵ�.
	SetEvent(SP->netWorkCopyWaitEvent); // ���簡 �Ϸ� �Ǿ����Ƿ� �ٽ� Accept�Լ����� ���ο� netWork��ü�� ���� �ǵ��� �Ѵ�.
	// �� �������� Ŭ���̾�Ʈ ����
	SOCKET* clientSocket = netWork->getMessageStruct()->socket;
	// �� ������ ���� �� ������
	CRoom* myRoom = netWork->getMessageStruct()->currentRoom;
	if (clientSocket == nullptr)
	{
		cout << "thNetWork���� clientSocket�� null �̴�." << endl;
		return -1;
	}

	while (true)
	{
		// �޽��� �ޱ�
		int isRecvSucces = netWork->recvn(roomShift);
		if (isRecvSucces == true)// �޽��� �ޱ� ���� �϶� �� Ŭ���̾�Ʈ���� �޽��� ����
		{
			netWork->sendn();
		}
		else if(isRecvSucces == -1) // �޽��� �ޱ� ���� ���� ����
		{
			// ���� ���� ó��
			// 1. �濡�� ������
			myRoom = netWork->getMessageStruct()->currentRoom; // ���� �� ������ �����ϰ�
			roomShift->roomExit(clientSocket, myRoom); // �濡�� ����.
			cout << "���� ������ ���Ͽ� �濡�� �������ϴ�." << endl;
			// 2. �ڿ� ��ȯ
			closesocket(*clientSocket);
			delete netWork;
			// ������ ��ȯ
			return 0;
		}
	}
}

CAccept::CAccept()
{
}


CAccept::~CAccept()
{
}

void CAccept::Accept(StructParam* SP)
{
	if (SP->serverSocket == nullptr)
	{
		cout << "Accept Error" << endl;
		return;
	}
	// ����&�ּ� �޸� �Ҵ�
	SOCKET* hClientSock = new SOCKET();
	SOCKADDR_IN* hClntAddr = new SOCKADDR_IN();
	int szClntAddr;
	// accept
	szClntAddr = sizeof(*hClntAddr);
	*hClientSock = accept(*SP->serverSocket, (SOCKADDR*)hClntAddr, &szClntAddr);// list�� ���Ϳ� ����
																	  //cout << "accept ������ �ּ� = " << hClntSock << endl;
	if (*hClientSock == INVALID_SOCKET)
		cout << "accept() error" << endl;
	else
		cout << "Connected client" << endl;

	// ��Ʈ��ũ ��ü �����
	CNetWork* NetWork = new CNetWork();
	NetWork->getMessageStruct()->socket = hClientSock;
	// room����ų� �̹� ������� 0���濡 �ֱ�
	NetWork->getMessageStruct()->currentRoom = SP->roomShift->roomMake(hClientSock, 0);
	// thNetWork �����忡�� CNetWork ��ü �����͸� ������ ���� ��ٸ���
	WaitForSingleObject(SP->netWorkCopyWaitEvent, INFINITE);
	// �����忡�� �ѱ� CNetWork ��ü ����
	SP->netWork = NetWork;
	// send,recv ��θ� �����ϴ� ������ ����� 
	CreateThread(NULL, 0, thNetWork, SP, 0, NULL);
}

