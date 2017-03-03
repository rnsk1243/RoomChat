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
	// sendn,recvn �Լ��� �ִ� Ŭ����
	CNetWork* netWork; // <- �� ���� �Ź� Aceept�Ҷ����� ������ netWork��ü �ּҷ� ���ŵǱ� ������ �ּҸ� �Ұ� ���� ������ send, recv �Լ��� �������� ������ �ȿ��� ������ ������ ���� �����ؼ� ������ �־�� �Ѵ�.
	// �� �Ű��ִ� Ŭ����
	CRoomShift* roomShift;
	// �����尡 �����ǰ� netWork ������ ���� ����� ������ ���� Aceept�� �Ͼ�� �ʵ��� ��ٸ��� �̺�Ʈ
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

