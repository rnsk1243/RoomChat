#pragma once
#include"MyInfo.h"
class CRoomManager;
class CChannelManager;
class CMyInfo; // <- 이거랑 #include"MyInfo.h" 차이?
#define INT 4

class CNetWork
{
	CRoomManager* RoomManager;
	CChannelManager* ChannelManager;
public:
	CNetWork(CRoomManager* roomManager, CChannelManager* channelManager);
	~CNetWork();
	// room에서 메시지 보내기
	int sendn(CMyInfo* myInfo, int flags = 0);
	int recvn(CMyInfo* myInfo, int flags = 0);
	CRoomManager* getRoomManager() { return RoomManager; }
	CChannelManager* getChannelManager() { return ChannelManager; }
};

