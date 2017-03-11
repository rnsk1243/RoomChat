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
	// ä�οű� �Լ�(�ű�� ����, �ű����� ä�� ��ȣ)
	bool channelEnter(CMyInfo* myInfo, int targetChannelNo);
	// ä�� ���� ������ (���������ϴ� ����)
	bool channelExit(CMyInfo* myInfo);
	list<CChannel*>* getChannelList() { return ChannelList; }
};

