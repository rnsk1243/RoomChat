#include "NetWork.h"
#include<iostream>
using namespace std;


CNetWork::CNetWork()
{
	MS = new MessageRoomInfoStruct();
}


CNetWork::~CNetWork()
{
	delete MS;
	cout << "CNetWork ��ü �Ҹ�" << endl;
}

int CNetWork::sendn(int flags)
{
	if (MS->currentRoom == nullptr || MS->socket == nullptr)
		return -1;
	int temp = 0;
	char* message = MS->message;
	int* size = MS->sendRecvSize;
	//cout << "���� �޼��� = " << message << endl;
	//cout << "���� ������ = " << *size << endl;
	CRoom* room = MS->currentRoom;
	list<SOCKET*>::iterator iterBegin = room->getRoomSocketList()->begin();
	list<SOCKET*>::iterator iterEnd = room->getRoomSocketList()->end();
	// �濡 �ִ� ��� ������� ������
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		// �޽��� ���� �ڽ��̸�
		if ((*iterBegin) == MS->socket)
		{
			continue; // ������ �ʰ� ���
		}
		send(*(*iterBegin), (char*)size, INT, flags);
		while (true)
		{
			temp += send(*(*iterBegin), message, *size, flags);
			if (temp >= *size)
				break;
		}
	}
	//cout << "�Ϸ�" << endl;
	return 1;
}

int CNetWork::recvn(CRoomShift* roomShift, int flags)
{
	char* temp = new char[4];
	SOCKET* clientSocket = MS->socket;
	int isSuccess = recv(*clientSocket, temp, INT, flags);
	
	if (isSuccess == SOCKET_ERROR)
	{
		cout << "1recvn ERROR" << endl;
		return SOCKET_ERROR;
	}
	*MS->sendRecvSize = *(int*)temp; 
	/*
	int * a = new int;
	int b = 5;
	a = &b //  b�ּҸ� ������ ���� �ִ� a�ּҴ� ���� ����?
	*/
	// �ӽ÷� ���� temp �޸� ��ȯ
	delete temp;

	int left = *MS->sendRecvSize;
	while (left > 0)
	{
		isSuccess = recv(*clientSocket, MS->message, left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return SOCKET_ERROR;
		}
		else if (isSuccess >= left)
			break;
	}
	MS->socket = clientSocket;
	MS->message[left] = '\0';

	cout << "���� �޽��� = " << MS->message << endl;
	
	int roomNumber = atoi(MS->message); // message�� ���ڷ� ��ȯ���� ���ϸ� 0�� ��ȯ
	if (roomNumber)
	{
		// message�� ���ڷ� ��ȯ �ϴµ� ����
		MS->currentRoom = roomShift->roomShift(clientSocket, MS->currentRoom, roomNumber);
		cout << MS->currentRoom->getRoomNo() << "������ �� �ű�" << endl;
		return 0;// thNetWork������ if����� �ʰ� �Ͽ� sendn�Լ� ��ȣ�� ������ ����
	}

	return 1;
}
