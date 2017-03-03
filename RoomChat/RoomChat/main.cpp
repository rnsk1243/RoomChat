#include"MySocket.h"
#include"Room.h"
#include"Accept.h"
#include"NetWork.h"
#include"RoomShift.h"

DWORD WINAPI Accept(PVOID param);

void main()
{
	CMySocket* mySocket = new CMySocket();
	// room�ű�� ���� ��ü
	CRoomShift* myRoomShift = new CRoomShift();
	// [���� ���� & recv,send Ŭ���� & room�ű�� ��ü] �� �ִ� ����ü
	StructParam* SP = new StructParam();
	SP->serverSocket = mySocket->getServSock();
	SP->roomShift = myRoomShift;
	CreateThread(NULL, NULL, Accept, SP, NULL, NULL);
	while (true)
	{

	}
}

DWORD WINAPI Accept(PVOID param)
{
	CAccept* myAccept = new CAccept();
	StructParam* SP = (StructParam*)param;
	while (true)
	{
		myAccept->Accept(SP);
	}
}
