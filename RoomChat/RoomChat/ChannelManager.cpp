#include "ChannelManager.h"



CChannelManager::CChannelManager()
{
	ChannelList = new list<CChannel*>();
}


CChannelManager::~CChannelManager()
{
}

// ä�οű� �Լ�(�ű�� ����, �ű���� �ϴ� ä�� ��ȣ)
bool CChannelManager::channelEnter(CMyInfo* myInfo, int targetChannelNo)
{
	list<CChannel*>::iterator iterBegin = ChannelList->begin();
	list<CChannel*>::iterator iterEnd = ChannelList->end();
	// �ű���� �ϴ� ��ȣ�� Channel ������ ���
	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if (targetChannelNo == (*iterBegin)->getChannelNum())
		{
			cout << targetChannelNo  << "�� ä�η� �̵� �մϴ�." << endl;
			EnterCriticalSection((*iterBegin)->getCreticalSocketList()); // �� ä�� ���� ������ �Ӱ迵���� ����
			(*iterBegin)->getChannelSocketList()->push_front(myInfo->getClientSocket()); // ä�ο� �־��ֱ�
			myInfo->setMyChannelNum(targetChannelNo);
			LeaveCriticalSection((*iterBegin)->getCreticalSocketList());
			return true; // �� �̻� ���� �����Ƿ� �Լ��� ����
		}
	}
	cout << "ä�� �ű�� ����" << endl;
	return false;
}

bool CChannelManager::channelExit(CMyInfo* myInfo)
{
	CChannel* myChannel = myInfo->getMyChannel();
	cout << myChannel->getChannelNum() << "�� ä���� �����ϴ�." << endl;
	
	if (myChannel != nullptr)
	{
		EnterCriticalSection(myChannel->getCreticalSocketList()); // �� �� ���� ������ �Ӱ迵���� ����
		list<SOCKET*>::iterator iterBegin = myChannel->getChannelSocketList()->begin();
		list<SOCKET*>::iterator iterEnd = myChannel->getChannelSocketList()->end();
		for (; iterBegin != iterEnd; ++iterBegin)
		{
			if ((*iterBegin) == myInfo->getClientSocket())
			{
				iterBegin = myChannel->getChannelSocketList()->erase(iterBegin); // ���� �ִ� �濡�� ����
				break;
			}
		}
		LeaveCriticalSection(myChannel->getCreticalSocketList()); // �Ӱ迵�� ���������� ����
		return true;
	}
	else
	{
		cout << "ä�� ������ ����" << endl;
		return false;
	}

}

