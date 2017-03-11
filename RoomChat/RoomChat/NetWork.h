#pragma once
#include"MyInfo.h"
class CRoomManager;
class CChannelManager;
class CMyInfo; // <- �̰Ŷ� #include"MyInfo.h" ����?
#define INT 4

class CNetWork
{
	CRoomManager* RoomManager;
	CChannelManager* ChannelManager;
public:
	CNetWork(CRoomManager* roomManager, CChannelManager* channelManager);
	~CNetWork();
	// room���� �޽��� ������
	int sendn(CMyInfo* myInfo, int flags = 0);
	int recvn(CMyInfo* myInfo, int flags = 0);
	CRoomManager* getRoomManager() { return RoomManager; }
	CChannelManager* getChannelManager() { return ChannelManager; }
};

