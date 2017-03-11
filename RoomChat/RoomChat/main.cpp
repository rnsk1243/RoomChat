#include"MySocket.h"
#include"Room.h"
#include"NetWork.h"
#include"ChannelManager.h"
#include"RoomManager.h"
#include<process.h>
#include"MyInfo.h"

struct threadParam
{
	CNetWork* netWork;
	CMyInfo* myInfo;
	threadParam(CNetWork* net, CMyInfo* info)
	{
		netWork = net;
		myInfo = info;
	}
};

unsigned int __stdcall thNetWork(PVOID pvParam)
{
	threadParam* param = (threadParam*)pvParam;
	CNetWork* netWork = param->netWork;
	CMyInfo* myInfo = param->myInfo;

	
	// �� �������� Ŭ���̾�Ʈ ����
	SOCKET* clientSocket = myInfo->getClientSocket(); // new�ϰ� nullptrüũ �ʿ�
	// �� ������ ���� �� ������
	CRoom* myRoom = nullptr;
	if (clientSocket == nullptr)
	{
		cout << "thNetWork���� clientSocket�� null �̴�." << endl;
		return -1;
	}

	while (true)
	{
		// �޽��� �ޱ�
		int isRecvSucces = netWork->recvn(myInfo);
		if (isRecvSucces == 1)// �޽��� �ޱ� ���� �϶� �� Ŭ���̾�Ʈ���� �޽��� ����
		{
			netWork->sendn(myInfo);
		}
		else if (0 == isRecvSucces) // �� ��ȣ �ٲ�(�� �̵� ��)
		{
			// room ����
			myInfo->setMyRoomObj(netWork->getChannelManager());
		}
		else if (isRecvSucces == -1) // �޽��� �ޱ� ���� ���� ����
		{
			// ���� ���� ó��
			// 1. �濡�� ������
			netWork->getChannelManager()->channelExit(myInfo);
			cout << "���� ������ ���Ͽ� �濡�� �������ϴ�." << endl;
			// 2. �ڿ� ��ȯ
			closesocket(*clientSocket);
			delete myInfo;;
			_endthreadex(0);
			// ������ ��ȯ
			return 0;
		}
	}
}

void main()
{
	CMySocket* mySocket = new CMySocket();
	// room�ű�� ���� ��ü
	CRoomManager* roomManager = new CRoomManager();
	CChannelManager* channelManager = new CChannelManager();
	// send, recv ���ִ� ��ü �����
	CNetWork* myNetWork = new CNetWork(roomManager, channelManager);
	// 0~4�� ä�� �����
	for (int i = 0; i < 5; i++)
	{
		CChannel* myChannel = new CChannel(i);
		channelManager->getChannelList()->push_front(myChannel);
	}

	while (true)
	{
		SOCKET* clientSocket = mySocket->Accept();
		CMyInfo* myInfo = new CMyInfo(clientSocket);
		if (channelManager->channelEnter(myInfo, 0))// 0�� ä�ο� �ֱ⼺��
		{
			myInfo->setMyChannelObj(channelManager); // ä�� ������ ����
			threadParam param(myNetWork, myInfo);
			_beginthreadex(NULL, NULL, thNetWork, &param, 0, NULL);
		}
	
	}
	
	//getchar(); // ���� ���װ� 
}
