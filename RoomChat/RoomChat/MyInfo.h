#pragma once
#include<WinSock2.h>
#include"Room.h"
#include"ChannelManager.h"
class CRoom;
class CChannel;
class CChannelManager;
#define BUF_SIZE 1024

// 클라이언트가 개인적으로 가지고 있는 구조체 (개인적인 것들이 선언되어 있다...)
struct MessageStruct
{
	char* message = new char[BUF_SIZE];
	int* sendRecvSize = new int;
};

class CMyInfo
{
	// 현재 내가 속한 방 번호
	int MyRoomNum;
	// 내가 속한 방
	CRoom* MyRoomObj;
	// 현재 내가 속한 채널 번호
	int MyChannelNum;
	// 내가 속한 채널
	CChannel* MyChannelObj;
	// 클라이언트 소켓
	SOCKET* ClientSocket;
	MessageStruct* MS;
public:
	CMyInfo(SOCKET* clientSocket);
	~CMyInfo();
	MessageStruct* getMessageStruct() { return MS; }
	SOCKET* getClientSocket() { return ClientSocket; }
	CRoom* getMyRoom(CChannelManager* channelManager);
	CRoom* getMyRoom() { return MyRoomObj != nullptr ? MyRoomObj : nullptr; }
	CChannel* getMyChannel(CChannelManager* channelManager);
	CChannel* getMyChannel() { return MyChannelObj != nullptr ? MyChannelObj : nullptr; }
	int getMyRoomNum() { return MyRoomNum; }
	int getMyChannelNum() { return MyChannelNum; }
	void setMyRoomNum(int myRoomNum) { MyRoomNum = myRoomNum; }
	void setMyChannelNum(int myChannelNum) { MyChannelNum = myChannelNum; }
	void setMyRoomObj(CChannelManager* channelManager) { MyRoomObj = getMyRoom(channelManager); }
	void setMyChannelObj(CChannelManager* channelManager) { MyChannelObj = getMyChannel(channelManager); }
};