#include"MySocket.h"
#include"Room.h"
#include"NetWork.h"
#include"ChannelManager.h"
#include"RoomManager.h"
#include<process.h>
#include"MyInfo.h"

struct threadParam
{
	CNetWork* netWork;
	CMyInfo* myInfo;
	threadParam(CNetWork* net, CMyInfo* info)
	{
		netWork = net;
		myInfo = info;
	}
};

unsigned int __stdcall thNetWork(PVOID pvParam)
{
	threadParam* param = (threadParam*)pvParam;
	CNetWork* netWork = param->netWork;
	CMyInfo* myInfo = param->myInfo;

	
	// 이 스레드의 클라이언트 소켓
	SOCKET* clientSocket = myInfo->getClientSocket(); // new하고 nullptr체크 필요
	// 이 소켓이 속한 방 포인터
	CRoom* myRoom = nullptr;
	if (clientSocket == nullptr)
	{
		cout << "thNetWork에서 clientSocket이 null 이다." << endl;
		return -1;
	}

	while (true)
	{
		// 메시지 받기
		int isRecvSucces = netWork->recvn(myInfo);
		if (isRecvSucces == 1)// 메시지 받기 성공 일때 각 클라이언트에게 메시지 보냄
		{
			netWork->sendn(myInfo);
		}
		else if (0 == isRecvSucces) // 방 번호 바뀜(방 이동 됨)
		{
			// room 갱신
			myInfo->setMyRoomObj(netWork->getChannelManager());
		}
		else if (isRecvSucces == -1) // 메시지 받기 실패 소켓 해제
		{
			// 소켓 오류 처리
			// 1. 방에서 나가기
			netWork->getChannelManager()->channelExit(myInfo);
			cout << "소켓 오류로 인하여 방에서 나갔습니다." << endl;
			// 2. 자원 반환
			closesocket(*clientSocket);
			delete myInfo;;
			_endthreadex(0);
			// 스레드 반환
			return 0;
		}
	}
}

void main()
{
	CMySocket* mySocket = new CMySocket();
	// room옮기기 전문 객체
	CRoomManager* roomManager = new CRoomManager();
	CChannelManager* channelManager = new CChannelManager();
	// send, recv 해주는 객체 만들기
	CNetWork* myNetWork = new CNetWork(roomManager, channelManager);
	// 0~4번 채널 만들기
	for (int i = 0; i < 5; i++)
	{
		CChannel* myChannel = new CChannel(i);
		channelManager->getChannelList()->push_front(myChannel);
	}

	while (true)
	{
		SOCKET* clientSocket = mySocket->Accept();
		CMyInfo* myInfo = new CMyInfo(clientSocket);
		if (channelManager->channelEnter(myInfo, 0))// 0번 채널에 넣기성공
		{
			myInfo->setMyChannelObj(channelManager); // 채널 포인터 지정
			threadParam param(myNetWork, myInfo);
			_beginthreadex(NULL, NULL, thNetWork, &param, 0, NULL);
		}
	
	}
	
	//getchar(); // 메인 안죽게 
}
