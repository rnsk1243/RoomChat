#include "Accept.h"

DWORD WINAPI thNetWork(PVOID pvParam)
{
	StructParam* SP = (StructParam*)pvParam;
	CRoomShift* roomShift = SP->roomShift;
	CNetWork* netWork = SP->netWork; // StructParam은 새로 객체를 만들지 않기 때문에 또 다른 소켓이 만들어지고 Accept가 일어나면 가르키는 CNetWork 객체가 달라 질 수 있으므로 
	// 포인터를 복사해 놓고 이후로는 복사한 포인터를 사용한다.
	SetEvent(SP->netWorkCopyWaitEvent); // 복사가 완료 되었으므로 다시 Accept함수에서 새로운 netWork객체가 복사 되도록 한다.
	// 이 스레드의 클라이언트 소켓
	SOCKET* clientSocket = netWork->getMessageStruct()->socket;
	// 이 소켓이 속한 방 포인터
	CRoom* myRoom = netWork->getMessageStruct()->currentRoom;
	if (clientSocket == nullptr)
	{
		cout << "thNetWork에서 clientSocket이 null 이다." << endl;
		return -1;
	}

	while (true)
	{
		// 메시지 받기
		int isRecvSucces = netWork->recvn(roomShift);
		if (isRecvSucces == true)// 메시지 받기 성공 일때 각 클라이언트에게 메시지 보냄
		{
			netWork->sendn();
		}
		else if(isRecvSucces == -1) // 메시지 받기 실패 소켓 해제
		{
			// 소켓 오류 처리
			// 1. 방에서 나가기
			myRoom = netWork->getMessageStruct()->currentRoom; // 현재 방 정보를 갱신하고
			roomShift->roomExit(clientSocket, myRoom); // 방에서 나감.
			cout << "소켓 오류로 인하여 방에서 나갔습니다." << endl;
			// 2. 자원 반환
			closesocket(*clientSocket);
			delete netWork;
			// 스레드 반환
			return 0;
		}
	}
}

CAccept::CAccept()
{
}


CAccept::~CAccept()
{
}

void CAccept::Accept(StructParam* SP)
{
	if (SP->serverSocket == nullptr)
	{
		cout << "Accept Error" << endl;
		return;
	}
	// 소켓&주소 메모리 할당
	SOCKET* hClientSock = new SOCKET();
	SOCKADDR_IN* hClntAddr = new SOCKADDR_IN();
	int szClntAddr;
	// accept
	szClntAddr = sizeof(*hClntAddr);
	*hClientSock = accept(*SP->serverSocket, (SOCKADDR*)hClntAddr, &szClntAddr);// list나 벡터에 저장
																	  //cout << "accept 에서의 주소 = " << hClntSock << endl;
	if (*hClientSock == INVALID_SOCKET)
		cout << "accept() error" << endl;
	else
		cout << "Connected client" << endl;

	// 네트워크 객체 만들기
	CNetWork* NetWork = new CNetWork();
	NetWork->getMessageStruct()->socket = hClientSock;
	// room만들거나 이미 만들어진 0번방에 넣기
	NetWork->getMessageStruct()->currentRoom = SP->roomShift->roomMake(hClientSock, 0);
	// thNetWork 스레드에서 CNetWork 객체 포인터를 복사할 동안 기다리자
	WaitForSingleObject(SP->netWorkCopyWaitEvent, INFINITE);
	// 스레드에게 넘길 CNetWork 객체 갱신
	SP->netWork = NetWork;
	// send,recv 모두를 관리하는 스레드 만들기 
	CreateThread(NULL, 0, thNetWork, SP, 0, NULL);
}

