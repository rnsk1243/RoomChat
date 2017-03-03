#pragma once
#include<WinSock2.h>
#include<iostream>
using namespace std;
#include"Room.h"
#include"NetWork.h"
#include"MySocket.h"
#include"RoomShift.h"

struct StructParam
{
	SOCKET* serverSocket;
	// sendn,recvn 함수가 있는 클래스
	CNetWork* netWork; // <- 이 값은 매번 Aceept할때마다 새로은 netWork객체 주소로 갱신되기 때문에 주소를 잃고 싶지 않으면 send, recv 함수를 돌리려는 스레드 안에서 포인터 정보를 필히 복사해서 가지고 있어야 한다.
	// 방 옮겨주는 클래스
	CRoomShift* roomShift;
	// 스레드가 생성되고 netWork 포인터 값을 제대로 복사할 동안 Aceept가 일어나지 않도록 기다리는 이벤트
	HANDLE netWorkCopyWaitEvent;
	StructParam()
	{
		netWorkCopyWaitEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	}
};

class CAccept
{
public:
	CAccept();
	~CAccept();
	void Accept(StructParam* SP);
};

