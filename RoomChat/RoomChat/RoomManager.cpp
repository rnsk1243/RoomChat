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
	cout << currentRoom->getRoomNo() << "번 방을 나갑니다." << endl;

	bool isRoomEmpty = false;
	if (currentRoom != nullptr)
	{
		EnterCriticalSection(currentRoom->getCritical()); // 각 방 마다 각각의 임계영역이 존재
		list<SOCKET*>::iterator iterBegin = currentRoom->getRoomSocketList()->begin();
		list<SOCKET*>::iterator iterEnd = currentRoom->getRoomSocketList()->end();
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if ((*iterBegin) == myInfo->getClientSocket())
			{
				myInfo->setMyRoomNum(-1);
				iterBegin = currentRoom->getRoomSocketList()->erase(iterBegin); // 원래 있던 방에서 빼기
																				//LeaveCriticalSection(currentRoom->getCritical()); // 임계영역 빠져나오고 (빠져나오기 전에 return해버리면 어떻게 될까?)
				if (currentRoom->getRoomSocketList()->empty())
				{
					cout << currentRoom->getRoomNo() << "번 방에 아무도 없습니다." << endl;
					isRoomEmpty = true; // 방이 빔
				}
				break;
			}
		}
		LeaveCriticalSection(currentRoom->getCritical()); // 임계영역 빠져나오고 리턴

		if (isRoomEmpty && currentRoom->getRoomNo() != 0) // 방이 비어있나?
			delete currentRoom;

		return true;
	}
	else
	{
		cout << "방 나가기 실패" << endl;
		return false;
	}
}

bool CRoomManager::roomMake(CMyInfo* myInfo, int targetRoomNo)
{
	list<CRoom*>::iterator iterBegin = RoomList->begin();
	list<CRoom*>::iterator iterEnd = RoomList->end();
	// 만들고자 하는 번호의 room이 이미 있나?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetRoomNo == (*iterBegin)->getRoomNo())
		{
			//EnterCriticalSection((*iterBegin)->getCritical()); // 각 방 마다 각각의 임계영역이 존재
			//(*iterBegin)->getRoomSocketList()->push_front(targetClientSocket); // 방에 넣어주기
			//LeaveCriticalSection((*iterBegin)->getCritical());
			cout << "이미 있는 방이라서 만들 수 없습니다." << endl;
			return false; // 더 이상 볼일 없으므로 함수를 끝냄
		}
	}

	// 새로운 room 만듬
	CRoom* newRoom = new CRoom(targetRoomNo);
	EnterCriticalSection(CS_RoomList); // RoomShift 클래스의 RoomList 전용 임계영역
	RoomList->push_front(newRoom);
	LeaveCriticalSection(CS_RoomList);

	// 새로운 room에 소켓 넣어줌
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
	// 들어가고자 하는 번호의 room이 있나?
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetRoomNo == (*iterBegin)->getRoomNo())
		{
			EnterCriticalSection((*iterBegin)->getCritical()); // 각 방 마다 각각의 임계영역이 존재
			(*iterBegin)->getRoomSocketList()->push_front(myInfo->getClientSocket()); // 방에 넣어주기
			myInfo->setMyRoomNum(targetRoomNo);
			LeaveCriticalSection((*iterBegin)->getCritical());
			return true; // 더 이상 볼일 없으므로 함수를 끝냄
		}
	}
	cout << "없는 방 입니다." << endl;
	return false;
}
