#pragma once
#include<WinSock2.h>
#include<list>
#include<iostream>
using namespace std;

class CRoom
{
	int RoomNo;
	list<SOCKET*>* RoomSocketList;
	// RoomSocketList 추가 삭제를 위한 임계역역
	CRITICAL_SECTION* CS;
public:

	CRoom(int roomNo)
	{
		CS = new CRITICAL_SECTION();
		RoomNo = roomNo;
		RoomSocketList = new list<SOCKET*>();
		InitializeCriticalSection(CS);
	}
	~CRoom() 
	{ 
		delete RoomSocketList;
		DeleteCriticalSection(CS); 
		cout << RoomNo << " 번 방이 삭제 되었습니다." << endl;  
	}
	list<SOCKET*>* getRoomSocketList() { return RoomSocketList; }
	CRITICAL_SECTION* getCritical() { return CS; }
	int getRoomNo() { return RoomNo; }
};

