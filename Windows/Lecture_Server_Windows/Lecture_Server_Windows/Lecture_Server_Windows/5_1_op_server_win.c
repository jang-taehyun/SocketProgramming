#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define BUF_SIZE 1024
#define OPERAND_SIZE 4
#define BACK_LOG_QUEUE_SIZE 5
#define CONNECT_COUNT 3

void ErrorHandling(const char* message);
int Calculate(int OperandCount, int Operand[], char Operator);

int main(int argc, char* argv[])
{
	WSADATA wsaData;											// ������ ���� ���̺귯�� ����
	SOCKET HandleServerSocket, HandleClientSocket;				// server socket, client socket
	SOCKADDR_IN ServerAddress, ClientAddress;					// server address, client address
	int ClientAddressSize;										// client address�� ũ��
	int OperationResult;										// �Լ� ���� ���

	int ReceiveCount;											// �� ���� ������ �������� ũ��
	int ReceiveLength;											// ������� ���� �������� ũ��

	char OperationInfo[BUF_SIZE];								// ������ �޽���
	int result;													// ���� ���
	int OperandCount;											// �ǿ������� ����


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

	ClientAddressSize = sizeof(ClientAddress);
	memset(OperationInfo, 0, sizeof(OperationInfo));
	for (int i = 0; i < CONNECT_COUNT; i++)
	{
		OperandCount = 0;

		// client�� ���� ��û ����(accept)
		HandleClientSocket = accept(HandleServerSocket, (SOCKADDR*)&ClientAddress, &ClientAddressSize);
		if (-1 == HandleClientSocket)
			ErrorHandling("accept() error");
		else
			printf("Connected Client %d\n", i + 1);

		// client�� ���� �����͸� ����(read)
		recv(HandleClientSocket, &OperandCount, 1, 0);

		ReceiveLength = 0;
		while ((OperandCount * OPERAND_SIZE + 1) > ReceiveLength)
		{
			ReceiveCount = recv(HandleClientSocket, &OperationInfo[ReceiveLength], BUF_SIZE - 1, 0);
			ReceiveLength += ReceiveCount;
		}

		// ����
		result = Calculate(OperandCount, (int*)OperationInfo, OperationInfo[ReceiveLength - 1]);

		// client�� �����͸� ����(write)
		send(HandleClientSocket, (char*)&result, sizeof(result), 0);

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

int Calculate(int OperandCount, int Operand[], char Operator)
{
	int result = Operand[0];

	switch (Operator)
	{
	case '+':
	{
		for (int i = 1; i < OperandCount; i++)
			result += Operand[i];
	}
		break;
	case '-':
	{
		for (int i = 1; i < OperandCount; i++)
			result -= Operand[i];
	}
		break;
	case '*':
	{
		for (int i = 1; i < OperandCount; i++)
			result *= Operand[i];
	}
		break;
	}

	return result;
}