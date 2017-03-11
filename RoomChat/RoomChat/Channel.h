#pragma once
#include<list>
#include<iostream>
#include"Room.h"

using namespace std;
class CChannel
{
	int ChannelNum;
	list<CRoom*>* RoomList;
	list<SOCKET*>* ChannelSocketList;
	CRITICAL_SECTION* CS_ChannelRoomList;
	CRITICAL_SECTION* CS_ChannelSocketList;
public:
	CChannel(int channelNum);
	~CChannel();
	int getChannelNum() { return ChannelNum; }
	CRITICAL_SECTION* getCreticalRoomList() { return CS_ChannelRoomList; }
	CRITICAL_SECTION* getCreticalSocketList() { return CS_ChannelSocketList; }
	list<CRoom*>* getRoomList() { return RoomList; }
	list<SOCKET*>* getChannelSocketList() { return ChannelSocketList; }
};

