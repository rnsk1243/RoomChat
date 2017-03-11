#include "ChannelManager.h"



CChannelManager::CChannelManager()
{
	ChannelList = new list<CChannel*>();
}


CChannelManager::~CChannelManager()
{
}

// 채널옮김 함수(옮기는 소켓, 옮기고자 하는 채널 번호)
bool CChannelManager::channelEnter(CMyInfo* myInfo, int targetChannelNo)
{
	list<CChannel*>::iterator iterBegin = ChannelList->begin();
	list<CChannel*>::iterator iterEnd = ChannelList->end();
	// 옮기고자 하는 번호의 Channel 포인터 얻기
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetChannelNo == (*iterBegin)->getChannelNum())
		{
			cout << targetChannelNo  << "번 채널로 이동 합니다." << endl;
			EnterCriticalSection((*iterBegin)->getCreticalSocketList()); // 각 채널 마다 각각의 임계영역이 존재
			(*iterBegin)->getChannelSocketList()->push_front(myInfo->getClientSocket()); // 채널에 넣어주기
			myInfo->setMyChannelNum(targetChannelNo);
			LeaveCriticalSection((*iterBegin)->getCreticalSocketList());
			return true; // 더 이상 볼일 없으므로 함수를 끝냄
		}
	}
	cout << "채널 옮기기 실패" << endl;
	return false;
}

bool CChannelManager::channelExit(CMyInfo* myInfo)
{
	CChannel* myChannel = myInfo->getMyChannel();
	cout << myChannel->getChannelNum() << "번 채널을 나갑니다." << endl;
	
	if (myChannel != nullptr)
	{
		EnterCriticalSection(myChannel->getCreticalSocketList()); // 각 방 마다 각각의 임계영역이 존재
		list<SOCKET*>::iterator iterBegin = myChannel->getChannelSocketList()->begin();
		list<SOCKET*>::iterator iterEnd = myChannel->getChannelSocketList()->end();
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if ((*iterBegin) == myInfo->getClientSocket())
			{
				iterBegin = myChannel->getChannelSocketList()->erase(iterBegin); // 원래 있던 방에서 빼기
				break;
			}
		}
		LeaveCriticalSection(myChannel->getCreticalSocketList()); // 임계영역 빠져나오고 리턴
		return true;
	}
	else
	{
		cout << "채널 나가기 실패" << endl;
		return false;
	}

}

