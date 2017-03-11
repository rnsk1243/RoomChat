#include "NetWork.h"




CNetWork::CNetWork(CRoomManager* roomManager, CChannelManager* channelManager)
	:RoomManager(roomManager), ChannelManager(channelManager){}

CNetWork::~CNetWork()
{
	cout << "CNetWork ��ü �Ҹ��� ȣ��" << endl;
}

int CNetWork::sendn(CMyInfo* myInfo, int flags)
{
	if (myInfo->getMyChannel() == nullptr)
	{
		cout << "���� ä�ΰ�ü�� ã�� �� �����ϴ�." << endl;
		return -1;
	}
	if (myInfo->getMessageStruct()->message == nullptr)
		return -1;
	
	char* message = myInfo->getMessageStruct()->message;
	int* size = myInfo->getMessageStruct()->sendRecvSize;
	cout << "���� �޼��� = " << message << endl;
	cout << "���� ������ = " << *size << endl;

	list<SOCKET*>::iterator iterBegin;
	list<SOCKET*>::iterator iterEnd;

	if (myInfo->getMyRoomNum() == -1)
	{
		// ä�� ���� ����Ʈ iter
		iterBegin = myInfo->getMyChannel()->getChannelSocketList()->begin();
		iterEnd = myInfo->getMyChannel()->getChannelSocketList()->end();
	}
	else
	{
		// �� ���� ����Ʈ iter
		if (myInfo->getMyRoom() == nullptr)
		{
			cout << "���� �水ü�� ã�� �� �����ϴ�" << endl;
			return -1;
		}
		iterBegin = myInfo->getMyRoom()->getRoomSocketList()->begin();
		iterEnd = myInfo->getMyRoom()->getRoomSocketList()->end();
	}

	// �濡 �ִ� ��� ������� ������
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		int temp = 0;
		// �޽��� ���� �ڽ��̸�
		if ((*iterBegin) == myInfo->getClientSocket())
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

int CNetWork::recvn(CMyInfo* myInfo, int flags)
{
	char* temp = new char[4];
	SOCKET* clientSocket = myInfo->getClientSocket();
	MessageStruct* MS = myInfo->getMessageStruct();
	int isSuccess = recv(*clientSocket, temp, INT, flags);
	
	if (isSuccess == SOCKET_ERROR)
	{
		cout << "1recvn ERROR" << endl;
		return SOCKET_ERROR;
	}
	*MS->sendRecvSize = *(int*)temp; 

	// �ӽ÷� ���� temp �޸� ��ȯ
	delete temp;

	int left = *MS->sendRecvSize;
	while (left > 0)
	{
		isSuccess += recv(*clientSocket, MS->message, left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return SOCKET_ERROR;
		}
		else if (isSuccess >= left)
			break;
	}
	MS->message[left] = '\0';

	cout << "���� �޽��� = " << MS->message << endl;
	char* ptr = strchr(MS->message, '/');
	if (ptr != nullptr)
	{
		*ptr++;
		if (*ptr == 'l')
		{
			cout << "�� ����Ʈ ���" << endl;
		}
		else if (*ptr == 'c')
		{
			cout << "ä�� ����Ʈ ���" << endl;
		}
		else if (*ptr == 'm')
		{
			cout << "�� �����" << endl;
		}
		return 0;
	}
	return 1;
}