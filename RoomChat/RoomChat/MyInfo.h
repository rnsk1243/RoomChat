#pragma once
#include<WinSock2.h>
#include"Room.h"
#include"ChannelManager.h"
class CRoom;
class CChannel;
class CChannelManager;
#define BUF_SIZE 1024

// Ŭ���̾�Ʈ�� ���������� ������ �ִ� ����ü (�������� �͵��� ����Ǿ� �ִ�...)
struct MessageStruct
{
	char* message = new char[BUF_SIZE];
	int* sendRecvSize = new int;
};

class CMyInfo
{
	// ���� ���� ���� �� ��ȣ
	int MyRoomNum;
	// ���� ���� ��
	CRoom* MyRoomObj;
	// ���� ���� ���� ä�� ��ȣ
	int MyChannelNum;
	// ���� ���� ä��
	CChannel* MyChannelObj;
	// Ŭ���̾�Ʈ ����
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