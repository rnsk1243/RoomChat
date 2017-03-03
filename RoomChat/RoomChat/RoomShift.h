#pragma once
#include"Room.h"
#include<list>
#include<WinSock2.h>

class CRoomShift
{
	list<CRoom*>* RoomList;
	// RoomList �߰� ������ ���� �Ӱ迪��
	CRITICAL_SECTION* CS;
public:
	CRoomShift();
	~CRoomShift();
	// ��ű� �Լ�(�ű�� ����, ���� ����ִ� ��, �ű���� �ϴ� �� ��ȣ or ������� �ϴ� �� ��ȣ)
	CRoom* roomShift(SOCKET* targetClientSocket, CRoom* currentRoom, int targetRoomNo); // �ű� room�� �ּҸ� ��ȯ
	// ����room���� ������ (���������ϴ� ����, ���������ϴ� ��)
	bool roomExit(SOCKET* targetClientSocket, CRoom* currentRoom);
	list<CRoom*>* getRoomList() { return RoomList; }
	// ���� �ڽ��� ���� ���� ������ �� �����(�� ����, ����� �� ��ȣ) 
	CRoom* roomMake(SOCKET* targetClientSocket, int targetRoomNo); // �ű� room�� �ּҸ� ��ȯ
};

