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

// 방옮김 함수(옮기는 소켓, 현재 들어있는 방, 옮기고자 하는 방 번호 or 만들고자 하는 방 번호)
CRoom* CRoomShift::roomShift(SOCKET * targetClientSocket, CRoom * currentRoom, int targetRoomNo)
{
	// room 빠져나오기 // roomExit함수가 false를 호출하며 빠져나오는데 실패하면 return 하여 함수를 끝냄
	if (!roomExit(targetClientSocket, currentRoom)){ cout << "room 빠져나오기 실패" << endl; return currentRoom; }
	// room 있나? 확인 변수 선언
	bool isRoomHave = false;
	list<CRoom*>::iterator iterBegin = RoomList->begin();
	list<CRoom*>::iterator iterEnd = RoomList->end();
	// 옮기고자 하는 번호의 room이 있나?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetRoomNo == (*iterBegin)->getRoomNo())
		{
			cout << "이미 만들어진 방으로 이동 합니다." << endl;
			isRoomHave = true; // 있다
			EnterCriticalSection((*iterBegin)->getCritical()); // 각 방 마다 각각의 임계영역이 존재
			(*iterBegin)->getRoomSocketList()->push_front(targetClientSocket); // 방에 넣어주기
			LeaveCriticalSection((*iterBegin)->getCritical());
			return (*iterBegin); // 더 이상 볼일 없으므로 함수를 끝냄
		}
	}
	// 모든 방을 다 뒤져도 옮기고자 하는 방이 없다
	if (!isRoomHave)
	{
		cout << "새로운 방을 만들어서 입장 합니다." << endl;
		// 새로운 room 만듬
		CRoom* newRoom = new CRoom(targetRoomNo);
		EnterCriticalSection(CS); // RoomShift 클래스의 RoomList 전용 임계영역
		RoomList->push_front(newRoom);
		LeaveCriticalSection(CS);

		// 새로운 room에 소켓 넣어줌
		EnterCriticalSection(newRoom->getCritical());
		newRoom->getRoomSocketList()->push_front(targetClientSocket);
		LeaveCriticalSection(newRoom->getCritical());
		return newRoom;
	}
	cout << "방 옮기기 실패" << endl;
	return currentRoom;
}
// 방 나가기
bool CRoomShift::roomExit(SOCKET * targetClientSocket, CRoom * currentRoom)
{
	cout << currentRoom->getRoomNo() << "번 방을 나갑니다." << endl;
	EnterCriticalSection(currentRoom->getCritical()); // 각 방 마다 각각의 임계영역이 존재
	list<SOCKET*>::iterator iterBegin = currentRoom->getRoomSocketList()->begin();
	list<SOCKET*>::iterator iterEnd = currentRoom->getRoomSocketList()->end();
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin) == targetClientSocket)
		{
			iterBegin = currentRoom->getRoomSocketList()->erase(iterBegin); // 원래 있던 방에서 빼기
			LeaveCriticalSection(currentRoom->getCritical()); // 임계영역 빠져나오고 (빠져나오기 전에 return해버리면 어떻게 될까?)
			if (currentRoom->getRoomSocketList()->empty())
			{
				cout << currentRoom->getRoomNo() << "번 방에 아무도 없습니다." << endl;
				delete currentRoom;
			}
			return true;
		}
	}
	LeaveCriticalSection(currentRoom->getCritical()); // 임계영역 빠져나오고 리턴
	// 방 나가기 실패
	return false;
}
// 방에 속하지 않는 소켓이 방 만들기
CRoom * CRoomShift::roomMake(SOCKET * targetClientSocket, int targetRoomNo)
{
	list<CRoom*>::iterator iterBegin = RoomList->begin();
	list<CRoom*>::iterator iterEnd = RoomList->end();
	// 만들고자 하는 번호의 room이 이미 있나?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetRoomNo == (*iterBegin)->getRoomNo())
		{
			EnterCriticalSection((*iterBegin)->getCritical()); // 각 방 마다 각각의 임계영역이 존재
			(*iterBegin)->getRoomSocketList()->push_front(targetClientSocket); // 방에 넣어주기
			LeaveCriticalSection((*iterBegin)->getCritical());
			return (*iterBegin); // 더 이상 볼일 없으므로 함수를 끝냄
		}
	}

	// 새로운 room 만듬
	CRoom* newRoom = new CRoom(targetRoomNo);
	EnterCriticalSection(CS); // RoomShift 클래스의 RoomList 전용 임계영역
	RoomList->push_front(newRoom);
	LeaveCriticalSection(CS);

	// 새로운 room에 소켓 넣어줌
	EnterCriticalSection(newRoom->getCritical());
	newRoom->getRoomSocketList()->push_front(targetClientSocket);
	LeaveCriticalSection(newRoom->getCritical());
	return newRoom;
}
