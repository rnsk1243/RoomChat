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
	// RoomList 추가 삭제를 위한 임계역역
	CRITICAL_SECTION* CS_RoomList;
public:
	CRoomManager();
	~CRoomManager();
	// room에서 나가기 (나가고자하는 소켓)
	bool roomExit(CMyInfo* myInfo);
	// 현재 자신이 속한 방이 없을때 방 만들기(들어갈 소켓, 만드는 방 번호) 
	bool roomMake(CMyInfo* myInfo, int targetRoomNo);
	// 현재 자신이 속한 방이 없을때 방 입장 하기
	bool roomEnter(CMyInfo* myInfo, int targetRoomNo);
};