#include "NetWork.h"




CNetWork::CNetWork(CRoomManager* roomManager, CChannelManager* channelManager)
	:RoomManager(roomManager), ChannelManager(channelManager){}

CNetWork::~CNetWork()
{
	cout << "CNetWork 객체 소멸자 호출" << endl;
}

int CNetWork::sendn(CMyInfo* myInfo, int flags)
{
	if (myInfo->getMyChannel() == nullptr)
	{
		cout << "나의 채널객체를 찾을 수 없습니다." << endl;
		return -1;
	}
	if (myInfo->getMessageStruct()->message == nullptr)
		return -1;
	
	char* message = myInfo->getMessageStruct()->message;
	int* size = myInfo->getMessageStruct()->sendRecvSize;
	cout << "보낼 메세지 = " << message << endl;
	cout << "보낼 사이즈 = " << *size << endl;

	list<SOCKET*>::iterator iterBegin;
	list<SOCKET*>::iterator iterEnd;

	if (myInfo->getMyRoomNum() == -1)
	{
		// 채널 소켓 리스트 iter
		iterBegin = myInfo->getMyChannel()->getChannelSocketList()->begin();
		iterEnd = myInfo->getMyChannel()->getChannelSocketList()->end();
	}
	else
	{
		// 방 소켓 리스트 iter
		if (myInfo->getMyRoom() == nullptr)
		{
			cout << "나의 방객체를 찾을 수 없습니다" << endl;
			return -1;
		}
		iterBegin = myInfo->getMyRoom()->getRoomSocketList()->begin();
		iterEnd = myInfo->getMyRoom()->getRoomSocketList()->end();
	}

	// 방에 있는 모든 사람에게 보내기
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		int temp = 0;
		// 메시지 보낸 자신이면
		if ((*iterBegin) == myInfo->getClientSocket())
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

int CNetWork::recvn(CMyInfo* myInfo, int flags)
{
	char* temp = new char[4];
	SOCKET* clientSocket = myInfo->getClientSocket();
	MessageStruct* MS = myInfo->getMessageStruct();
	int isSuccess = recv(*clientSocket, temp, INT, flags);
	
	if (isSuccess == SOCKET_ERROR)
	{
		cout << "1recvn ERROR" << endl;
		return SOCKET_ERROR;
	}
	*MS->sendRecvSize = *(int*)temp; 

	// 임시로 만든 temp 메모리 반환
	delete temp;

	int left = *MS->sendRecvSize;
	while (left > 0)
	{
		isSuccess += recv(*clientSocket, MS->message, left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "2recvn ERROR" << endl;
			return SOCKET_ERROR;
		}
		else if (isSuccess >= left)
			break;
	}
	MS->message[left] = '\0';

	cout << "받은 메시지 = " << MS->message << endl;
	char* ptr = strchr(MS->message, '/');
	if (ptr != nullptr)
	{
		*ptr++;
		if (*ptr == 'l')
		{
			cout << "방 리스트 출력" << endl;
		}
		else if (*ptr == 'c')
		{
			cout << "채널 리스트 출력" << endl;
		}
		else if (*ptr == 'm')
		{
			cout << "방 만들기" << endl;
		}
		return 0;
	}
	return 1;
}