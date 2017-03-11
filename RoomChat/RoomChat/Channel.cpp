#include "Channel.h"


CChannel::CChannel(int channelNum):ChannelNum(channelNum)
{
	RoomList = new list<CRoom*>();
	ChannelSocketList = new list<SOCKET*>();
	CS_ChannelRoomList = new CRITICAL_SECTION();
	InitializeCriticalSection(CS_ChannelRoomList);

	CS_ChannelSocketList = new CRITICAL_SECTION();
	InitializeCriticalSection(CS_ChannelSocketList);
}

CChannel::~CChannel()
{
}
