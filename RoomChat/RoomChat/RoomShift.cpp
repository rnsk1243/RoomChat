#include "RoomShift.h"



CRoomShift::CRoomShift()
{
	CS = new CRITICAL_SECTION();
	InitializeCriticalSection(CS);
	RoomList = new list<CRoom*>();
}


CRoomShift::~CRoomShift()
{
	DeleteCriticalSection(CS);
}

// ��ű� �Լ�(�ű�� ����, ���� ����ִ� ��, �ű���� �ϴ� �� ��ȣ or ������� �ϴ� �� ��ȣ)
CRoom* CRoomShift::roomShift(SOCKET * targetClientSocket, CRoom * currentRoom, int targetRoomNo)
{
	// room ���������� // roomExit�Լ��� false�� ȣ���ϸ� ���������µ� �����ϸ� return �Ͽ� �Լ��� ����
	if (!roomExit(targetClientSocket, currentRoom)){ cout << "room ���������� ����" << endl; return currentRoom; }
	// room �ֳ�? Ȯ�� ���� ����
	bool isRoomHave = false;
	list<CRoom*>::iterator iterBegin = RoomList->begin();
	list<CRoom*>::iterator iterEnd = RoomList->end();
	// �ű���� �ϴ� ��ȣ�� room�� �ֳ�?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetRoomNo == (*iterBegin)->getRoomNo())
		{
			cout << "�̹� ������� ������ �̵� �մϴ�." << endl;
			isRoomHave = true; // �ִ�
			EnterCriticalSection((*iterBegin)->getCritical()); // �� �� ���� ������ �Ӱ迵���� ����
			(*iterBegin)->getRoomSocketList()->push_front(targetClientSocket); // �濡 �־��ֱ�
			LeaveCriticalSection((*iterBegin)->getCritical());
			return (*iterBegin); // �� �̻� ���� �����Ƿ� �Լ��� ����
		}
	}
	// ��� ���� �� ������ �ű���� �ϴ� ���� ����
	if (!isRoomHave)
	{
		cout << "���ο� ���� ���� ���� �մϴ�." << endl;
		// ���ο� room ����
		CRoom* newRoom = new CRoom(targetRoomNo);
		EnterCriticalSection(CS); // RoomShift Ŭ������ RoomList ���� �Ӱ迵��
		RoomList->push_front(newRoom);
		LeaveCriticalSection(CS);

		// ���ο� room�� ���� �־���
		EnterCriticalSection(newRoom->getCritical());
		newRoom->getRoomSocketList()->push_front(targetClientSocket);
		LeaveCriticalSection(newRoom->getCritical());
		return newRoom;
	}
	cout << "�� �ű�� ����" << endl;
	return currentRoom;
}
// �� ������
bool CRoomShift::roomExit(SOCKET * targetClientSocket, CRoom * currentRoom)
{
	cout << currentRoom->getRoomNo() << "�� ���� �����ϴ�." << endl;
	EnterCriticalSection(currentRoom->getCritical()); // �� �� ���� ������ �Ӱ迵���� ����
	list<SOCKET*>::iterator iterBegin = currentRoom->getRoomSocketList()->begin();
	list<SOCKET*>::iterator iterEnd = currentRoom->getRoomSocketList()->end();
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin) == targetClientSocket)
		{
			iterBegin = currentRoom->getRoomSocketList()->erase(iterBegin); // ���� �ִ� �濡�� ����
			LeaveCriticalSection(currentRoom->getCritical()); // �Ӱ迵�� ���������� (���������� ���� return�ع����� ��� �ɱ�?)
			if (currentRoom->getRoomSocketList()->empty())
			{
				cout << currentRoom->getRoomNo() << "�� �濡 �ƹ��� �����ϴ�." << endl;
				delete currentRoom;
			}
			return true;
		}
	}
	LeaveCriticalSection(currentRoom->getCritical()); // �Ӱ迵�� ���������� ����
	// �� ������ ����
	return false;
}
// �濡 ������ �ʴ� ������ �� �����
CRoom * CRoomShift::roomMake(SOCKET * targetClientSocket, int targetRoomNo)
{
	list<CRoom*>::iterator iterBegin = RoomList->begin();
	list<CRoom*>::iterator iterEnd = RoomList->end();
	// ������� �ϴ� ��ȣ�� room�� �̹� �ֳ�?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetRoomNo == (*iterBegin)->getRoomNo())
		{
			EnterCriticalSection((*iterBegin)->getCritical()); // �� �� ���� ������ �Ӱ迵���� ����
			(*iterBegin)->getRoomSocketList()->push_front(targetClientSocket); // �濡 �־��ֱ�
			LeaveCriticalSection((*iterBegin)->getCritical());
			return (*iterBegin); // �� �̻� ���� �����Ƿ� �Լ��� ����
		}
	}

	// ���ο� room ����
	CRoom* newRoom = new CRoom(targetRoomNo);
	EnterCriticalSection(CS); // RoomShift Ŭ������ RoomList ���� �Ӱ迵��
	RoomList->push_front(newRoom);
	LeaveCriticalSection(CS);

	// ���ο� room�� ���� �־���
	EnterCriticalSection(newRoom->getCritical());
	newRoom->getRoomSocketList()->push_front(targetClientSocket);
	LeaveCriticalSection(newRoom->getCritical());
	return newRoom;
}
