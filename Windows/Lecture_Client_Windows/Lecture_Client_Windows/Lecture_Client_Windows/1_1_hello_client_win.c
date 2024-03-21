#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define MESSAGE_BUFFER 30
#define _WINSOCK_DEPRECATED_NO_WARNINGS

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;				// ������ ���� ���̺귯�� ����
	SOCKET HandleClientSocket;		// client socket handle
	SOCKADDR_IN ServerAddress;		// server address
	int OperationResult;			// �Լ� ���� ���

	char message[MESSAGE_BUFFER];	// ���� �޼��� ����
	int StringLength;				// �޼����� ����

	// argument �˻�
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// ������ ���� ���̺귯�� �ʱ�ȭ
	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (OperationResult)
		ErrorHandling("WSAStartup() function error");

	// client socket ����(socket)
	HandleClientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == HandleClientSocket)
		ErrorHandling("socket() function error");

	// server �ּ� ���� �Է�
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// server�� ���� ��û
	OperationResult = connect(HandleClientSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("connect() function error");

	// �޽��� ����
	StringLength = recv(HandleClientSocket, message, sizeof(message) - 1, 0);
	if (-1 == StringLength)
		ErrorHandling("read() function error");

	// �޽��� ���
	printf("Message from server : %s\n", message);

	// handle ����
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