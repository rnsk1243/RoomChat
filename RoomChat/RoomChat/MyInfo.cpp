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
	cout << "myInfo �Ҹ��� ȣ��" << endl;
}

CRoom * CMyInfo::getMyRoom(CChannelManager* channelManager)
{
	if (-1 == MyRoomNum)
	{
		cout << "�濡 ��� ���� �ʽ��ϴ�." << endl;
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
	cout << "�� �������� ����" << endl;
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
	cout << "�� ä�� �������� ����" << endl;
	return nullptr;
}
