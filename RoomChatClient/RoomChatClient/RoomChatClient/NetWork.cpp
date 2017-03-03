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
	//cout << "보낼 메세지 = " << message << endl;
	//cout << "보낼 사이즈 = " << size << endl;

	int xxx = send(*clientSocket, (char*)&size, INT, flags);
	while (true)
	{
		temp += send(*clientSocket, message, size, flags);
		if (temp >= size)
			break;
	}
	cout << "보낸 메세지 = " << message << endl;
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
	// 값만 복사
	*MS->sendRecvSize = *(int*)temp; // 만약 MS->sendRecvSize = (int*)temp 이렇게 포인터를 복사하고 delete temp 하면 메모리 할당한 MS->sendRecvSize도 지워지나?
	// temp 메모리 반환
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

	cout << "받은 메시지 = " << MS->message << endl;

	return 1;
}
