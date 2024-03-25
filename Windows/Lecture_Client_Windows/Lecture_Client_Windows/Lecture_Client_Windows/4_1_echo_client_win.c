#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define BUF_SIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;						// ������ ���� ���̺귯�� ����
	SOCKET HandleClientSocket;				// client socket
	SOCKADDR_IN ServerAddress;				// server address ����
	int StringLength;						// ������ �޼���(������)�� ����

	char message[BUF_SIZE];					// ������ �޼���
	int OperationResult;					// �Լ� ���� ���

	// argument �˻�
	if (3 != argc)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	// ������ ���� ���̺귯�� ���� �ʱ�ȭ
	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (OperationResult)
		ErrorHandling("WSAStartup() error");

	// client socket ����(socket)
	HandleClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == HandleClientSocket)
		ErrorHandling("socket() error");

	// server address ���� �Է�
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// server�� ���� ��û(connect)
	OperationResult = connect(HandleClientSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("connect() error");
	else
		puts("Connected............");

	while (1)
	{
		memset(message, 0, sizeof(message));
		fputs("Input message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		// server�� �޽���(������) ����(send)
		send(HandleClientSocket, message, strlen(message), 0);
		
		// server���� �� �޼���(������) ����(recv)
		StringLength = recv(HandleClientSocket, message, BUF_SIZE - 1, 0);

		message[StringLength] = NULL;
		printf("Message from server : %s\n", message);
	}

	// client socket ����
	closesocket(HandleClientSocket);

	// ������ ���� ���̺귯�� ���� ����
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}