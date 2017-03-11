#include "MyInfo.h"



CMyInfo::CMyInfo(SOCKET* clientSocket)
	:ClientSocket(clientSocket), MyRoomNum(-1), MyChannelNum(0)
{
	MS = new MessageStruct();
	MyChannelObj = nullptr;
	MyRoomObj = nullptr;
}


CMyInfo::~CMyInfo()
{
	cout << "myInfo 소멸자 호출" << endl;
}

CRoom * CMyInfo::getMyRoom(CChannelManager* channelManager)
{
	if (-1 == MyRoomNum)
	{
		cout << "방에 들어 있지 않습니다." << endl;
		return nullptr;
	}
	CChannel* myChannel = getMyChannel(channelManager);

	list<CRoom*>::iterator iterBegin = myChannel->getRoomList()->begin();
	list<CRoom*>::iterator iterEnd = myChannel->getRoomList()->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (MyRoomNum == (*iterBegin)->getRoomNo())
		{
			return (*iterBegin);
		}
	}
	cout << "방 가져오기 실패" << endl;
	return nullptr;
}

CChannel * CMyInfo::getMyChannel(CChannelManager* channelManager)
{
	list<CChannel*>::iterator iterBegin = channelManager->getChannelList()->begin();
	list<CChannel*>::iterator iterEnd = channelManager->getChannelList()->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin)->getChannelNum() == MyChannelNum)
			return (*iterBegin);
	}
	cout << "내 채널 가져오기 실패" << endl;
	return nullptr;
}
