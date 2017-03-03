#include "NetWork.h"
#include<iostream>
using namespace std;


CNetWork::CNetWork()
{
	MS = new MessageRoomInfoStruct();
}


CNetWork::~CNetWork()
{
	delete MS;
	cout << "CNetWork 객체 소멸" << endl;
}

int CNetWork::sendn(int flags)
{
	if (MS->currentRoom == nullptr || MS->socket == nullptr)
		return -1;
	int temp = 0;
	char* message = MS->message;
	int* size = MS->sendRecvSize;
	//cout << "보낼 메세지 = " << message << endl;
	//cout << "보낼 사이즈 = " << *size << endl;
	CRoom* room = MS->currentRoom;
	list<SOCKET*>::iterator iterBegin = room->getRoomSocketList()->begin();
	list<SOCKET*>::iterator iterEnd = room->getRoomSocketList()->end();
	// 방에 있는 모든 사람에게 보내기
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		// 메시지 보낸 자신이면
		if ((*iterBegin) == MS->socket)
		{
			continue; // 보내지 않고 통과
		}
		send(*(*iterBegin), (char*)size, INT, flags);
		while (true)
		{
			temp += send(*(*iterBegin), message, *size, flags);
			if (temp >= *size)
				break;
		}
	}
	//cout << "완료" << endl;
	return 1;
}

int CNetWork::recvn(CRoomShift* roomShift, int flags)
{
	char* temp = new char[4];
	SOCKET* clientSocket = MS->socket;
	int isSuccess = recv(*clientSocket, temp, INT, flags);
	
	if (isSuccess == SOCKET_ERROR)
	{
		cout << "1recvn ERROR" << endl;
		return SOCKET_ERROR;
	}
	*MS->sendRecvSize = *(int*)temp; 
	/*
	int * a = new int;
	int b = 5;
	a = &b //  b주소를 넣으면 원래 있던 a주소는 어디로 갈까?
	*/
	// 임시로 만든 temp 메모리 반환
	delete temp;

	int left = *MS->sendRecvSize;
	while (left > 0)
	{
		isSuccess = recv(*clientSocket, MS->message, left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return SOCKET_ERROR;
		}
		else if (isSuccess >= left)
			break;
	}
	MS->socket = clientSocket;
	MS->message[left] = '\0';

	cout << "받은 메시지 = " << MS->message << endl;
	
	int roomNumber = atoi(MS->message); // message를 숫자로 변환하지 못하면 0을 반환
	if (roomNumber)
	{
		// message를 숫자로 반환 하는데 성공
		MS->currentRoom = roomShift->roomShift(clientSocket, MS->currentRoom, roomNumber);
		cout << MS->currentRoom->getRoomNo() << "번으로 방 옮김" << endl;
		return 0;// thNetWork스레드 if문통과 않게 하여 sendn함수 미호출 유도를 위함
	}

	return 1;
}
