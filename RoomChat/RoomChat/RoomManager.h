#pragma once
#include<WinSock2.h>
#include<list>
#include"Room.h"
#include"NetWork.h"
#include"MyInfo.h"
class CMyInfo;

class CRoomManager
{
	list<CRoom*>* RoomList;
	// RoomList �߰� ������ ���� �Ӱ迪��
	CRITICAL_SECTION* CS_RoomList;
public:
	CRoomManager();
	~CRoomManager();
	// room���� ������ (���������ϴ� ����)
	bool roomExit(CMyInfo* myInfo);
	// ���� �ڽ��� ���� ���� ������ �� �����(�� ����, ����� �� ��ȣ) 
	bool roomMake(CMyInfo* myInfo, int targetRoomNo);
	// ���� �ڽ��� ���� ���� ������ �� ���� �ϱ�
	bool roomEnter(CMyInfo* myInfo, int targetRoomNo);
};