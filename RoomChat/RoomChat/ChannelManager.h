#pragma once
#include"Channel.h"
#include"NetWork.h"
#include<list>

class CMyInfo;

class CChannelManager
{
	list<CChannel*>* ChannelList;
public:
	CChannelManager();
	~CChannelManager();
	// 채널옮김 함수(옮기는 소켓, 옮기고싶은 채널 번호)
	bool channelEnter(CMyInfo* myInfo, int targetChannelNo);
	// 채널 에서 나가기 (나가고자하는 소켓)
	bool channelExit(CMyInfo* myInfo);
	list<CChannel*>* getChannelList() { return ChannelList; }
};

