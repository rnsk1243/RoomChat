#pragma once
#include<WinSock2.h>
#include"Room.h"
#include"RoomShift.h"

#define BUF_SIZE 1024
#define INT 4

// Ŭ���̾�Ʈ�� ���������� ������ �ִ� ����ü (�������� �͵��� ����Ǿ� �ִ�...)
struct MessageRoomInfoStruct
{
	// ���� ���� ���� �� ������
	CRoom* currentRoom = nullptr;
	// �޼��� ����
	SOCKET* socket = nullptr;
	char* message = new char[BUF_SIZE];
	int* sendRecvSize = new int;
};

class CNetWork
{
	MessageRoomInfoStruct* MS;
public:
	CNetWork();
	~CNetWork();
	int sendn(int flags = 0);
	int recvn(CRoomShift* roomShift, int flags = 0);
	MessageRoomInfoStruct* getMessageStruct() { return MS; }
};

