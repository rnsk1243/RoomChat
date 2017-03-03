#pragma once
#include<WinSock2.h>
#include"Room.h"
#include"RoomShift.h"

#define BUF_SIZE 1024
#define INT 4

// 클라이언트가 개인적으로 가지고 있는 구조체 (개인적인 것들이 선언되어 있다...)
struct MessageRoomInfoStruct
{
	// 현재 내가 속한 방 포인터
	CRoom* currentRoom = nullptr;
	// 메세지 주인
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

