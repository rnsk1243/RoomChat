#include "NetWork.h"

CNetWork::CNetWork()
{
	MS = new MessageStruct();
}


CNetWork::~CNetWork()
{
}

int CNetWork::sendn(SOCKET * clientSocket, char* message,  int flags)
{
	if (message == nullptr)
		return -1;

	int temp = 0;
	int size = strlen(message);
	//cout << "���� �޼��� = " << message << endl;
	//cout << "���� ������ = " << size << endl;

	int xxx = send(*clientSocket, (char*)&size, INT, flags);
	while (true)
	{
		temp += send(*clientSocket, message, size, flags);
		if (temp >= size)
			break;
	}
	cout << "���� �޼��� = " << message << endl;
	return 1;
}

int CNetWork::recvn(SOCKET * clientSocket, int flags)
{
	char* temp = new char[4];
	int isSuccess = recv(*clientSocket, temp, INT, flags);
	//cout << "isSuccess = " << isSuccess << endl;
	if (isSuccess == SOCKET_ERROR)
	{
		cout << "ERROR" << endl;
		return SOCKET_ERROR;
	}
	// ���� ����
	*MS->sendRecvSize = *(int*)temp; // ���� MS->sendRecvSize = (int*)temp �̷��� �����͸� �����ϰ� delete temp �ϸ� �޸� �Ҵ��� MS->sendRecvSize�� ��������?
	// temp �޸� ��ȯ
	delete temp;

	int left = *MS->sendRecvSize;
	//cout << "left = " << left << endl;
	while (left > 0)
	{
		isSuccess = recv(*clientSocket, MS->message, left, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			cout << "ERROR" << endl;
			return SOCKET_ERROR;
		}
		else if (isSuccess >= left)
			break;
	}
	MS->message[left] = '\0';

	cout << "���� �޽��� = " << MS->message << endl;

	return 1;
}
