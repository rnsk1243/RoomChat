#include"MySocket.h"
#include"Room.h"
#include"Accept.h"
#include"NetWork.h"
#include"RoomShift.h"

DWORD WINAPI Accept(PVOID param);

void main()
{
	CMySocket* mySocket = new CMySocket();
	// room옮기기 전문 객체
	CRoomShift* myRoomShift = new CRoomShift();
	// [서버 소켓 & recv,send 클래스 & room옮기는 객체] 가 있는 구조체
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
