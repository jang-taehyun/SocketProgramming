#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define BUF_SIZE 1024
#define BACK_LOG_QUEUE_SIZE 5
#define CONNECT_COUNT 5

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;											// ������ ���� ���̺귯�� ����
	SOCKET HandleServerSocket, HandleClientSocket;				// server socket, client socket
	SOCKADDR_IN ServerAddress, ClientAddress;					// server address, client address
	int ClientAddressSize;										// client address�� ũ��
	int StringLength;											// ���Ź��� ���ڿ��� ����

	char message[BUF_SIZE];										// ���Ź��� ������(�޽���)
	int OperationResult;										// �Լ� ���� ���

	// argument �˻�
	if (2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// ������ ���� ���̺귯�� ���� �ʱ�ȭ
	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (OperationResult)
		ErrorHandling("WSAStartup() error");

	// server socket ����(socket)
	HandleServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == HandleServerSocket)
		ErrorHandling("socket() error");

	// server address ���� �Է�
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// server socket�� IP, port �Ҵ�
	OperationResult = bind(HandleServerSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("bind() error");

	// server socket�� ���� ��� ���·� ��ȯ(listen)
	OperationResult = listen(HandleServerSocket, BACK_LOG_QUEUE_SIZE);
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("listen() error");

	// client�� ���� ��û ����(accept)
	ClientAddressSize = sizeof(ClientAddress);
	memset(message, 0, sizeof(message));
	for (int i = 0; i < CONNECT_COUNT; i++)
	{
		HandleClientSocket = accept(HandleServerSocket, (SOCKADDR*)&ClientAddress, &ClientAddressSize);
		if (-1 == HandleClientSocket)
			ErrorHandling("accept() error");
		else
			printf("Connected Client %d\n", i + 1);

		int Count = 0;

		// client�� ���� �����͸� ����(read)
		while (StringLength = recv(HandleClientSocket, message, BUF_SIZE, 0))
		{
			printf("Count : %d\n Received message : %s\n", Count++, message);

			// client�� �����͸� ����(write)
			send(HandleClientSocket, message, StringLength, 0);

			memset(message, 0, sizeof(message));
		}

		// client socket ����
		closesocket(HandleClientSocket);
	}

	// server socket ����
	closesocket(HandleServerSocket);

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