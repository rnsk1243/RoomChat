#pragma once
#include"Room.h"
#include<list>
#include<WinSock2.h>

class CRoomShift
{
	list<CRoom*>* RoomList;
	// RoomList 추가 삭제를 위한 임계역역
	CRITICAL_SECTION* CS;
public:
	CRoomShift();
	~CRoomShift();
	// 방옮김 함수(옮기는 소켓, 현재 들어있는 방, 옮기고자 하는 방 번호 or 만들고자 하는 방 번호)
	CRoom* roomShift(SOCKET* targetClientSocket, CRoom* currentRoom, int targetRoomNo); // 옮긴 room의 주소를 반환
	// 원래room에서 나가기 (나가고자하는 소켓, 나가고자하는 방)
	bool roomExit(SOCKET* targetClientSocket, CRoom* currentRoom);
	list<CRoom*>* getRoomList() { return RoomList; }
	// 현재 자신이 속한 방이 없을때 방 만들기(들어갈 소켓, 만드는 방 번호) 
	CRoom* roomMake(SOCKET* targetClientSocket, int targetRoomNo); // 옮긴 room의 주소를 반환
};

