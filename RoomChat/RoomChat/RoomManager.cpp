#include "RoomManager.h"



CRoomManager::CRoomManager()
{
	CS_RoomList = new CRITICAL_SECTION();
	InitializeCriticalSection(CS_RoomList);
	RoomList = new list<CRoom*>();
}


CRoomManager::~CRoomManager()
{
}

bool CRoomManager::roomExit(CMyInfo* myInfo)
{
	CRoom* currentRoom = myInfo->getMyRoom();
	cout << currentRoom->getRoomNo() << "�� ���� �����ϴ�." << endl;

	bool isRoomEmpty = false;
	if (currentRoom != nullptr)
	{
		EnterCriticalSection(currentRoom->getCritical()); // �� �� ���� ������ �Ӱ迵���� ����
		list<SOCKET*>::iterator iterBegin = currentRoom->getRoomSocketList()->begin();
		list<SOCKET*>::iterator iterEnd = currentRoom->getRoomSocketList()->end();
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if ((*iterBegin) == myInfo->getClientSocket())
			{
				myInfo->setMyRoomNum(-1);
				iterBegin = currentRoom->getRoomSocketList()->erase(iterBegin); // ���� �ִ� �濡�� ����
																				//LeaveCriticalSection(currentRoom->getCritical()); // �Ӱ迵�� ���������� (���������� ���� return�ع����� ��� �ɱ�?)
				if (currentRoom->getRoomSocketList()->empty())
				{
					cout << currentRoom->getRoomNo() << "�� �濡 �ƹ��� �����ϴ�." << endl;
					isRoomEmpty = true; // ���� ��
				}
				break;
			}
		}
		LeaveCriticalSection(currentRoom->getCritical()); // �Ӱ迵�� ���������� ����

		if (isRoomEmpty && currentRoom->getRoomNo() != 0) // ���� ����ֳ�?
			delete currentRoom;

		return true;
	}
	else
	{
		cout << "�� ������ ����" << endl;
		return false;
	}
}

bool CRoomManager::roomMake(CMyInfo* myInfo, int targetRoomNo)
{
	list<CRoom*>::iterator iterBegin = RoomList->begin();
	list<CRoom*>::iterator iterEnd = RoomList->end();
	// ������� �ϴ� ��ȣ�� room�� �̹� �ֳ�?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetRoomNo == (*iterBegin)->getRoomNo())
		{
			//EnterCriticalSection((*iterBegin)->getCritical()); // �� �� ���� ������ �Ӱ迵���� ����
			//(*iterBegin)->getRoomSocketList()->push_front(targetClientSocket); // �濡 �־��ֱ�
			//LeaveCriticalSection((*iterBegin)->getCritical());
			cout << "�̹� �ִ� ���̶� ���� �� �����ϴ�." << endl;
			return false; // �� �̻� ���� �����Ƿ� �Լ��� ����
		}
	}

	// ���ο� room ����
	CRoom* newRoom = new CRoom(targetRoomNo);
	EnterCriticalSection(CS_RoomList); // RoomShift Ŭ������ RoomList ���� �Ӱ迵��
	RoomList->push_front(newRoom);
	LeaveCriticalSection(CS_RoomList);

	// ���ο� room�� ���� �־���
	EnterCriticalSection(newRoom->getCritical());
	newRoom->getRoomSocketList()->push_front(myInfo->getClientSocket());
	myInfo->setMyRoomNum(targetRoomNo);
	LeaveCriticalSection(newRoom->getCritical());
	return true;
}

bool CRoomManager::roomEnter(CMyInfo* myInfo, int targetRoomNo)
{
	list<CRoom*>::iterator iterBegin = RoomList->begin();
	list<CRoom*>::iterator iterEnd = RoomList->end();
	// ������ �ϴ� ��ȣ�� room�� �ֳ�?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetRoomNo == (*iterBegin)->getRoomNo())
		{
			EnterCriticalSection((*iterBegin)->getCritical()); // �� �� ���� ������ �Ӱ迵���� ����
			(*iterBegin)->getRoomSocketList()->push_front(myInfo->getClientSocket()); // �濡 �־��ֱ�
			myInfo->setMyRoomNum(targetRoomNo);
			LeaveCriticalSection((*iterBegin)->getCritical());
			return true; // �� �̻� ���� �����Ƿ� �Լ��� ����
		}
	}
	cout << "���� �� �Դϴ�." << endl;
	return false;
}
