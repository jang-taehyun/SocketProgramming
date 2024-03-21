#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define MESSAGE_MAX_LENGTH 30

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;							// ������ ���� ���̺귯�� ����
	SOCKET HandleClientSocket;					// client socket
	SOCKADDR_IN ServerAddress;					// server �ּ� ����
	int OperationResult;						// �Լ� ���� ���

	char ReceiveMessage[MESSAGE_MAX_LENGTH];	// �������� ���� �޼���(������)
	int StringLength = 0;						// �������� ���� �޼���(������)�� ����
	int Index = 0, ReadLength;					// �޼���(������)�� index, ���� �������� ����

	// argument �˻�
	if (3 != argc)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	// ������ ���� ���̺귯�� ���� �ʱ�ȭ
	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (OperationResult)
		ErrorHandling("WSAStartup() function error");

	// client socket ����(socket)
	HandleClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == HandleClientSocket)
		ErrorHandling("socket() function error");

	// server �ּ� ���� �ʱ�ȭ
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// server�� ���� ��û(connect)
	OperationResult = connect(HandleClientSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("connect() function error");

	// ������ ����
	while (ReadLength = recv(HandleClientSocket, &ReceiveMessage[Index++], 1, 0))		// 1byte�� ����
	{
		if (-1 == ReadLength)
			ErrorHandling("recv() function error");

		StringLength += ReadLength;
	}

	printf("message from server : %s\n", ReceiveMessage);
	printf("Function recv() call count : %d\n", StringLength);

	// socket ����
	closesocket(HandleClientSocket);

	// ������ ���� ���̺귯�� ����
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}